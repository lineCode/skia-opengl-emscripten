// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/bidirectional_stream_quic_impl.h"

#include <utility>

#include "base/memory/ptr_util.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/threading/thread_task_runner_handle.h"
#include "base/time/default_tick_clock.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/completion_once_callback.h"
#include "net/base/ip_address.h"
#include "net/base/load_timing_info.h"
#include "net/base/load_timing_info_test_util.h"
#include "net/base/net_errors.h"
#include "net/http/bidirectional_stream_request_info.h"
#include "net/http/transport_security_state.h"
#include "net/log/net_log_event_type.h"
#include "net/log/test_net_log.h"
#include "net/log/test_net_log_util.h"
#include "net/quic/mock_crypto_client_stream_factory.h"
#include "net/quic/quic_chromium_alarm_factory.h"
#include "net/quic/quic_chromium_connection_helper.h"
#include "net/quic/quic_chromium_packet_reader.h"
#include "net/quic/quic_chromium_packet_writer.h"
#include "net/quic/quic_http_utils.h"
#include "net/quic/quic_server_info.h"
#include "net/quic/quic_stream_factory.h"
#include "net/quic/quic_test_packet_maker.h"
#include "net/quic/test_task_runner.h"
#include "net/socket/socket_test_util.h"
#include "net/test/gtest_util.h"
#include "net/test/test_with_scoped_task_environment.h"
#include "net/third_party/quiche/src/quic/core/crypto/crypto_protocol.h"
#include "net/third_party/quiche/src/quic/core/crypto/null_decrypter.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_decrypter.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_encrypter.h"
#include "net/third_party/quiche/src/quic/core/http/spdy_utils.h"
#include "net/third_party/quiche/src/quic/core/quic_connection.h"
#include "net/third_party/quiche/src/quic/core/tls_client_handshaker.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_arraysize.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_string_piece.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_text_utils.h"
#include "net/third_party/quiche/src/quic/test_tools/crypto_test_utils.h"
#include "net/third_party/quiche/src/quic/test_tools/mock_clock.h"
#include "net/third_party/quiche/src/quic/test_tools/mock_random.h"
#include "net/third_party/quiche/src/quic/test_tools/quic_connection_peer.h"
#include "net/third_party/quiche/src/quic/test_tools/quic_spdy_session_peer.h"
#include "net/third_party/quiche/src/quic/test_tools/quic_test_utils.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

namespace test {

namespace {

const char kUploadData[] = "Really nifty data!";
const char kDefaultServerHostName[] = "www.google.com";
const uint16_t kDefaultServerPort = 80;
// Size of the buffer to be allocated for each read.
const size_t kReadBufferSize = 4096;

enum DelegateMethod {
  kOnStreamReady,
  kOnHeadersReceived,
  kOnTrailersReceived,
  kOnDataRead,
  kOnDataSent,
  kOnFailed
};

class TestDelegateBase : public BidirectionalStreamImpl::Delegate {
 public:
  TestDelegateBase(IOBuffer* read_buf, int read_buf_len)
      : TestDelegateBase(read_buf,
                         read_buf_len,
                         std::make_unique<base::OneShotTimer>()) {}

  TestDelegateBase(IOBuffer* read_buf,
                   int read_buf_len,
                   std::unique_ptr<base::OneShotTimer> timer)
      : read_buf_(read_buf),
        read_buf_len_(read_buf_len),
        timer_(std::move(timer)),
        loop_(nullptr),
        next_proto_(kProtoUnknown),
        received_bytes_(0),
        sent_bytes_(0),
        has_load_timing_info_(false),
        error_(OK),
        on_data_read_count_(0),
        on_data_sent_count_(0),
        not_expect_callback_(false),
        on_failed_called_(false),
        send_request_headers_automatically_(true),
        is_ready_(false),
        trailers_expected_(false),
        trailers_received_(false) {
    loop_.reset(new base::RunLoop);
  }

  ~TestDelegateBase() override {}

  void OnStreamReady(bool request_headers_sent) override {
    CHECK(!is_ready_);
    CHECK(!on_failed_called_);
    EXPECT_EQ(send_request_headers_automatically_, request_headers_sent);
    CHECK(!not_expect_callback_);
    is_ready_ = true;
    loop_->Quit();
  }

  void OnHeadersReceived(
      const spdy::SpdyHeaderBlock& response_headers) override {
    CHECK(!on_failed_called_);
    CHECK(!not_expect_callback_);

    response_headers_ = response_headers.Clone();
    loop_->Quit();
  }

  void OnDataRead(int bytes_read) override {
    CHECK(!on_failed_called_);
    CHECK(!not_expect_callback_);
    CHECK(!callback_.is_null());

    // If read EOF, make sure this callback is after trailers callback.
    if (bytes_read == 0)
      EXPECT_TRUE(!trailers_expected_ || trailers_received_);
    ++on_data_read_count_;
    CHECK_GE(bytes_read, OK);
    data_received_.append(read_buf_->data(), bytes_read);
    std::move(callback_).Run(bytes_read);
  }

  void OnDataSent() override {
    CHECK(!on_failed_called_);
    CHECK(!not_expect_callback_);

    ++on_data_sent_count_;
    loop_->Quit();
  }

  void OnTrailersReceived(const spdy::SpdyHeaderBlock& trailers) override {
    CHECK(!on_failed_called_);
    CHECK(!not_expect_callback_);

    trailers_received_ = true;
    trailers_ = trailers.Clone();
    loop_->Quit();
  }

  void OnFailed(int error) override {
    CHECK(!on_failed_called_);
    CHECK(!not_expect_callback_);
    CHECK_EQ(OK, error_);
    CHECK_NE(OK, error);

    on_failed_called_ = true;
    error_ = error;
    loop_->Quit();
  }

  void Start(const BidirectionalStreamRequestInfo* request_info,
             const NetLogWithSource& net_log,
             std::unique_ptr<QuicChromiumClientSession::Handle> session) {
    not_expect_callback_ = true;
    stream_ = std::make_unique<BidirectionalStreamQuicImpl>(std::move(session));
    stream_->Start(request_info, net_log, send_request_headers_automatically_,
                   this, nullptr, TRAFFIC_ANNOTATION_FOR_TESTS);
    not_expect_callback_ = false;
  }

  void SendRequestHeaders() {
    not_expect_callback_ = true;
    stream_->SendRequestHeaders();
    not_expect_callback_ = false;
  }

  void SendData(const scoped_refptr<IOBuffer>& data,
                int length,
                bool end_of_stream) {
    SendvData({data}, {length}, end_of_stream);
  }

  void SendvData(const std::vector<scoped_refptr<IOBuffer>>& data,
                 const std::vector<int>& lengths,
                 bool end_of_stream) {
    not_expect_callback_ = true;
    stream_->SendvData(data, lengths, end_of_stream);
    not_expect_callback_ = false;
  }

  // Waits until next Delegate callback.
  void WaitUntilNextCallback(DelegateMethod method) {
    ASSERT_FALSE(on_failed_called_);
    bool is_ready = is_ready_;
    bool headers_received = !response_headers_.empty();
    bool trailers_received = trailers_received_;
    int on_data_read_count = on_data_read_count_;
    int on_data_sent_count = on_data_sent_count_;

    loop_->Run();
    loop_.reset(new base::RunLoop);

    EXPECT_EQ(method == kOnFailed, on_failed_called_);
    EXPECT_EQ(is_ready || (method == kOnStreamReady), is_ready_);
    EXPECT_EQ(headers_received || (method == kOnHeadersReceived),
              !response_headers_.empty());
    EXPECT_EQ(trailers_received || (method == kOnTrailersReceived),
              trailers_received_);
    EXPECT_EQ(on_data_read_count + (method == kOnDataRead ? 1 : 0),
              on_data_read_count_);
    EXPECT_EQ(on_data_sent_count + (method == kOnDataSent ? 1 : 0),
              on_data_sent_count_);
  }

  // Calls ReadData on the |stream_| and updates |data_received_|.
  int ReadData(CompletionOnceCallback callback) {
    not_expect_callback_ = true;
    int rv = stream_->ReadData(read_buf_.get(), read_buf_len_);
    not_expect_callback_ = false;
    if (rv > 0)
      data_received_.append(read_buf_->data(), rv);
    if (rv == ERR_IO_PENDING)
      callback_ = std::move(callback);
    return rv;
  }

  NextProto GetProtocol() const {
    if (stream_)
      return stream_->GetProtocol();
    return next_proto_;
  }

  int64_t GetTotalReceivedBytes() const {
    if (stream_)
      return stream_->GetTotalReceivedBytes();
    return received_bytes_;
  }

  int64_t GetTotalSentBytes() const {
    if (stream_)
      return stream_->GetTotalSentBytes();
    return sent_bytes_;
  }

  bool GetLoadTimingInfo(LoadTimingInfo* load_timing_info) {
    if (stream_)
      return stream_->GetLoadTimingInfo(load_timing_info);
    *load_timing_info = load_timing_info_;
    return has_load_timing_info_;
  }

  void DoNotSendRequestHeadersAutomatically() {
    send_request_headers_automatically_ = false;
  }

  // Deletes |stream_|.
  void DeleteStream() {
    next_proto_ = stream_->GetProtocol();
    received_bytes_ = stream_->GetTotalReceivedBytes();
    sent_bytes_ = stream_->GetTotalSentBytes();
    has_load_timing_info_ = stream_->GetLoadTimingInfo(&load_timing_info_);
    stream_.reset();
  }

  void set_trailers_expected(bool trailers_expected) {
    trailers_expected_ = trailers_expected;
  }
  // Const getters for internal states.
  const std::string& data_received() const { return data_received_; }
  int error() const { return error_; }
  const spdy::SpdyHeaderBlock& response_headers() const {
    return response_headers_;
  }
  const spdy::SpdyHeaderBlock& trailers() const { return trailers_; }
  int on_data_read_count() const { return on_data_read_count_; }
  int on_data_sent_count() const { return on_data_sent_count_; }
  bool on_failed_called() const { return on_failed_called_; }
  bool is_ready() const { return is_ready_; }

 protected:
  // Quits |loop_|.
  void QuitLoop() { loop_->Quit(); }

 private:
  std::unique_ptr<BidirectionalStreamQuicImpl> stream_;
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;
  std::unique_ptr<base::OneShotTimer> timer_;
  std::string data_received_;
  std::unique_ptr<base::RunLoop> loop_;
  spdy::SpdyHeaderBlock response_headers_;
  spdy::SpdyHeaderBlock trailers_;
  NextProto next_proto_;
  int64_t received_bytes_;
  int64_t sent_bytes_;
  bool has_load_timing_info_;
  LoadTimingInfo load_timing_info_;
  int error_;
  int on_data_read_count_;
  int on_data_sent_count_;
  // This is to ensure that delegate callback is not invoked synchronously when
  // calling into |stream_|.
  bool not_expect_callback_;
  bool on_failed_called_;
  CompletionOnceCallback callback_;
  bool send_request_headers_automatically_;
  bool is_ready_;
  bool trailers_expected_;
  bool trailers_received_;

  DISALLOW_COPY_AND_ASSIGN(TestDelegateBase);
};

// A delegate that deletes the stream in a particular callback.
class DeleteStreamDelegate : public TestDelegateBase {
 public:
  // Specifies in which callback the stream can be deleted.
  enum Phase {
    ON_STREAM_READY,
    ON_HEADERS_RECEIVED,
    ON_DATA_READ,
    ON_TRAILERS_RECEIVED,
    ON_FAILED,
  };

  DeleteStreamDelegate(IOBuffer* buf, int buf_len, Phase phase)
      : TestDelegateBase(buf, buf_len), phase_(phase) {}
  ~DeleteStreamDelegate() override {}

  void OnStreamReady(bool request_headers_sent) override {
    TestDelegateBase::OnStreamReady(request_headers_sent);
    if (phase_ == ON_STREAM_READY)
      DeleteStream();
  }

  void OnHeadersReceived(
      const spdy::SpdyHeaderBlock& response_headers) override {
    // Make a copy of |response_headers| before the stream is deleted, since
    // the headers are owned by the stream.
    spdy::SpdyHeaderBlock headers_copy = response_headers.Clone();
    if (phase_ == ON_HEADERS_RECEIVED)
      DeleteStream();
    TestDelegateBase::OnHeadersReceived(headers_copy);
  }

  void OnDataSent() override { NOTREACHED(); }

  void OnDataRead(int bytes_read) override {
    DCHECK_NE(ON_HEADERS_RECEIVED, phase_);
    if (phase_ == ON_DATA_READ)
      DeleteStream();
    TestDelegateBase::OnDataRead(bytes_read);
  }

  void OnTrailersReceived(const spdy::SpdyHeaderBlock& trailers) override {
    DCHECK_NE(ON_HEADERS_RECEIVED, phase_);
    DCHECK_NE(ON_DATA_READ, phase_);
    // Make a copy of |response_headers| before the stream is deleted, since
    // the headers are owned by the stream.
    spdy::SpdyHeaderBlock trailers_copy = trailers.Clone();
    if (phase_ == ON_TRAILERS_RECEIVED)
      DeleteStream();
    TestDelegateBase::OnTrailersReceived(trailers_copy);
  }

  void OnFailed(int error) override {
    DCHECK_EQ(ON_FAILED, phase_);
    DeleteStream();
    TestDelegateBase::OnFailed(error);
  }

 private:
  // Indicates in which callback the delegate should cancel or delete the
  // stream.
  Phase phase_;

  DISALLOW_COPY_AND_ASSIGN(DeleteStreamDelegate);
};

}  // namespace

class BidirectionalStreamQuicImplTest
    : public ::testing::TestWithParam<
          std::tuple<quic::ParsedQuicVersion, bool>>,
      public WithScopedTaskEnvironment {
 protected:
  static const bool kFin = true;
  static const bool kIncludeVersion = true;
  static const bool kIncludeCongestionFeedback = true;

  // Holds a packet to be written to the wire, and the IO mode that should
  // be used by the mock socket when performing the write.
  struct PacketToWrite {
    PacketToWrite(IoMode mode, quic::QuicReceivedPacket* packet)
        : mode(mode), packet(packet) {}
    PacketToWrite(IoMode mode, int rv) : mode(mode), packet(nullptr), rv(rv) {}
    IoMode mode;
    quic::QuicReceivedPacket* packet;
    int rv;
  };

  BidirectionalStreamQuicImplTest()
      : version_(std::get<0>(GetParam())),
        client_headers_include_h2_stream_dependency_(std::get<1>(GetParam())),
        crypto_config_(quic::test::crypto_test_utils::ProofVerifierForTesting(),
                       quic::TlsClientHandshaker::CreateSslCtx()),
        read_buffer_(base::MakeRefCounted<IOBufferWithSize>(4096)),
        connection_id_(quic::test::TestConnectionId(2)),
        stream_id_(GetNthClientInitiatedBidirectionalStreamId(0)),
        client_maker_(version_,
                      connection_id_,
                      &clock_,
                      kDefaultServerHostName,
                      quic::Perspective::IS_CLIENT,
                      client_headers_include_h2_stream_dependency_),
        server_maker_(version_,
                      connection_id_,
                      &clock_,
                      kDefaultServerHostName,
                      quic::Perspective::IS_SERVER,
                      false),
        random_generator_(0),
        destination_(kDefaultServerHostName, kDefaultServerPort) {
    IPAddress ip(192, 0, 2, 33);
    peer_addr_ = IPEndPoint(ip, 443);
    self_addr_ = IPEndPoint(ip, 8435);
    clock_.AdvanceTime(quic::QuicTime::Delta::FromMilliseconds(20));
  }

  ~BidirectionalStreamQuicImplTest() {
    session_->CloseSessionOnError(ERR_ABORTED, quic::QUIC_INTERNAL_ERROR,
                                  quic::ConnectionCloseBehavior::SILENT_CLOSE);
    for (size_t i = 0; i < writes_.size(); i++) {
      delete writes_[i].packet;
    }
  }

  void TearDown() override {
    EXPECT_TRUE(socket_data_->AllReadDataConsumed());
    EXPECT_TRUE(socket_data_->AllWriteDataConsumed());
  }

  // Adds a packet to the list of expected writes.
  void AddWrite(std::unique_ptr<quic::QuicReceivedPacket> packet) {
    writes_.push_back(PacketToWrite(SYNCHRONOUS, packet.release()));
  }

  // Adds a write error to the list of expected writes.
  void AddWriteError(IoMode mode, int rv) {
    writes_.push_back(PacketToWrite(mode, rv));
  }

  void ProcessPacket(std::unique_ptr<quic::QuicReceivedPacket> packet) {
    connection_->ProcessUdpPacket(
        quic::QuicSocketAddress(quic::QuicSocketAddressImpl(self_addr_)),
        quic::QuicSocketAddress(quic::QuicSocketAddressImpl(peer_addr_)),
        *packet);
  }

  // Configures the test fixture to use the list of expected writes.
  void Initialize() {
    crypto_client_stream_factory_.set_handshake_mode(
        MockCryptoClientStream::ZERO_RTT);
    mock_writes_.reset(new MockWrite[writes_.size()]);
    for (size_t i = 0; i < writes_.size(); i++) {
      if (writes_[i].packet == nullptr) {
        mock_writes_[i] = MockWrite(writes_[i].mode, writes_[i].rv, i);
      } else {
        mock_writes_[i] = MockWrite(writes_[i].mode, writes_[i].packet->data(),
                                    writes_[i].packet->length());
      }
    }

    socket_data_.reset(new StaticSocketDataProvider(
        base::span<MockRead>(),
        base::make_span(mock_writes_.get(), writes_.size())));

    std::unique_ptr<MockUDPClientSocket> socket(new MockUDPClientSocket(
        socket_data_.get(), net_log().bound().net_log()));
    socket->Connect(peer_addr_);
    runner_ = new TestTaskRunner(&clock_);
    helper_.reset(
        new QuicChromiumConnectionHelper(&clock_, &random_generator_));
    alarm_factory_.reset(new QuicChromiumAlarmFactory(runner_.get(), &clock_));
    connection_ = new quic::QuicConnection(
        connection_id_,
        quic::QuicSocketAddress(quic::QuicSocketAddressImpl(peer_addr_)),
        helper_.get(), alarm_factory_.get(),
        new QuicChromiumPacketWriter(socket.get(), runner_.get()),
        true /* owns_writer */, quic::Perspective::IS_CLIENT,
        quic::test::SupportedVersions(version_));
    if (connection_->version().KnowsWhichDecrypterToUse()) {
      connection_->InstallDecrypter(quic::ENCRYPTION_FORWARD_SECURE,
                                    quic::QuicMakeUnique<quic::NullDecrypter>(
                                        quic::Perspective::IS_CLIENT));
    }
    base::TimeTicks dns_end = base::TimeTicks::Now();
    base::TimeTicks dns_start = dns_end - base::TimeDelta::FromMilliseconds(1);

    session_.reset(new QuicChromiumClientSession(
        connection_, std::move(socket),
        /*stream_factory=*/nullptr, &crypto_client_stream_factory_, &clock_,
        &transport_security_state_, /*ssl_config_service=*/nullptr,
        base::WrapUnique(static_cast<QuicServerInfo*>(nullptr)),
        QuicSessionKey(kDefaultServerHostName, kDefaultServerPort,
                       PRIVACY_MODE_DISABLED, SocketTag()),
        /*require_confirmation=*/false, /*migrate_session_early_v2=*/false,
        /*migrate_session_on_network_change_v2=*/false,
        /*default_network=*/NetworkChangeNotifier::kInvalidNetworkHandle,
        quic::QuicTime::Delta::FromMilliseconds(
            kDefaultRetransmittableOnWireTimeoutMillisecs),
        /*migrate_idle_session=*/false,
        base::TimeDelta::FromSeconds(kDefaultIdleSessionMigrationPeriodSeconds),
        base::TimeDelta::FromSeconds(kMaxTimeOnNonDefaultNetworkSecs),
        kMaxMigrationsToNonDefaultNetworkOnWriteError,
        kMaxMigrationsToNonDefaultNetworkOnPathDegrading,
        kQuicYieldAfterPacketsRead,
        quic::QuicTime::Delta::FromMilliseconds(
            kQuicYieldAfterDurationMilliseconds),
        /*go_away_on_path_degrading*/ false,
        client_headers_include_h2_stream_dependency_, /*cert_verify_flags=*/0,
        quic::test::DefaultQuicConfig(), &crypto_config_, "CONNECTION_UNKNOWN",
        dns_start, dns_end, &push_promise_index_, nullptr,
        base::DefaultTickClock::GetInstance(),
        base::ThreadTaskRunnerHandle::Get().get(),
        /*socket_performance_watcher=*/nullptr, net_log().bound().net_log()));
    session_->Initialize();
    TestCompletionCallback callback;
    session_->CryptoConnect(callback.callback());
    EXPECT_TRUE(session_->IsEncryptionEstablished());
  }

  void ConfirmHandshake() {
    crypto_client_stream_factory_.last_stream()->SendOnCryptoHandshakeEvent(
        quic::QuicSession::HANDSHAKE_CONFIRMED);
  }

  void SetRequest(const std::string& method,
                  const std::string& path,
                  RequestPriority priority) {
    request_headers_ = client_maker_.GetRequestHeaders(method, "http", path);
  }

  spdy::SpdyHeaderBlock ConstructResponseHeaders(
      const std::string& response_code) {
    return server_maker_.GetResponseHeaders(response_code);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructDataPacket(
      uint64_t packet_number,
      bool should_include_version,
      bool fin,
      quic::QuicStreamOffset offset,
      quic::QuicStringPiece data,
      QuicTestPacketMaker* maker) {
    std::unique_ptr<quic::QuicReceivedPacket> packet(maker->MakeDataPacket(
        packet_number, stream_id_, should_include_version, fin, offset, data));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructServerDataPacket(
      uint64_t packet_number,
      bool should_include_version,
      bool fin,
      quic::QuicStreamOffset offset,
      quic::QuicStringPiece data) {
    return ConstructDataPacket(packet_number, should_include_version, fin,
                               offset, data, &server_maker_);
  }

  // Construct a data packet with multiple data frames
  std::unique_ptr<quic::QuicReceivedPacket>
  ConstructClientMultipleDataFramesPacket(
      uint64_t packet_number,
      bool should_include_version,
      bool fin,
      quic::QuicStreamOffset offset,
      const std::vector<std::string>& data_writes) {
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        client_maker_.MakeMultipleDataFramesPacket(packet_number, stream_id_,
                                                   should_include_version, fin,
                                                   offset, data_writes));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructRequestHeadersPacket(
      uint64_t packet_number,
      bool fin,
      RequestPriority request_priority,
      size_t* spdy_headers_frame_length) {
    return ConstructRequestHeadersPacketInner(
        packet_number, stream_id_, fin, request_priority,
        spdy_headers_frame_length, /*offset=*/nullptr);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructRequestHeadersPacketInner(
      uint64_t packet_number,
      quic::QuicStreamId stream_id,
      bool fin,
      RequestPriority request_priority,
      size_t* spdy_headers_frame_length,
      quic::QuicStreamOffset* offset) {
    return ConstructRequestHeadersPacketInner(
        packet_number, stream_id, fin, request_priority, 0,
        spdy_headers_frame_length, offset);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructRequestHeadersPacketInner(
      uint64_t packet_number,
      quic::QuicStreamId stream_id,
      bool fin,
      RequestPriority request_priority,
      quic::QuicStreamId parent_stream_id,
      size_t* spdy_headers_frame_length,
      quic::QuicStreamOffset* offset) {
    spdy::SpdyPriority priority =
        ConvertRequestPriorityToQuicPriority(request_priority);
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        client_maker_.MakeRequestHeadersPacket(
            packet_number, stream_id, kIncludeVersion, fin, priority,
            std::move(request_headers_), parent_stream_id,
            spdy_headers_frame_length, offset));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket>
  ConstructRequestHeadersAndMultipleDataFramesPacket(
      uint64_t packet_number,
      bool fin,
      RequestPriority request_priority,
      quic::QuicStreamOffset* header_stream_offset,
      size_t* spdy_headers_frame_length,
      const std::vector<std::string>& data) {
    spdy::SpdyPriority priority =
        ConvertRequestPriorityToQuicPriority(request_priority);
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        client_maker_.MakeRequestHeadersAndMultipleDataFramesPacket(
            packet_number, stream_id_, kIncludeVersion, fin, priority,
            std::move(request_headers_), 0, header_stream_offset,
            spdy_headers_frame_length, data));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructResponseHeadersPacket(
      uint64_t packet_number,
      bool fin,
      spdy::SpdyHeaderBlock response_headers,
      size_t* spdy_headers_frame_length,
      quic::QuicStreamOffset* offset) {
    return ConstructResponseHeadersPacketInner(
        packet_number, stream_id_, fin, std::move(response_headers),
        spdy_headers_frame_length, offset);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructResponseHeadersPacketInner(
      uint64_t packet_number,
      quic::QuicStreamId stream_id,
      bool fin,
      spdy::SpdyHeaderBlock response_headers,
      size_t* spdy_headers_frame_length,
      quic::QuicStreamOffset* offset) {
    return server_maker_.MakeResponseHeadersPacket(
        packet_number, stream_id, !kIncludeVersion, fin,
        std::move(response_headers), spdy_headers_frame_length, offset);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructResponseTrailersPacket(
      uint64_t packet_number,
      bool fin,
      spdy::SpdyHeaderBlock trailers,
      size_t* spdy_headers_frame_length,
      quic::QuicStreamOffset* offset) {
    return server_maker_.MakeResponseHeadersPacket(
        packet_number, stream_id_, !kIncludeVersion, fin, std::move(trailers),
        spdy_headers_frame_length, offset);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructClientRstStreamPacket(
      uint64_t packet_number) {
    return ConstructRstStreamCancelledPacket(packet_number, !kIncludeVersion, 0,
                                             &client_maker_);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructServerRstStreamPacket(
      uint64_t packet_number) {
    return ConstructRstStreamCancelledPacket(packet_number, !kIncludeVersion, 0,
                                             &server_maker_);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructClientEarlyRstStreamPacket(
      uint64_t packet_number) {
    return ConstructRstStreamCancelledPacket(packet_number, kIncludeVersion, 0,
                                             &client_maker_);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructRstStreamCancelledPacket(
      uint64_t packet_number,
      bool include_version,
      size_t bytes_written,
      QuicTestPacketMaker* maker) {
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        maker->MakeRstPacket(packet_number, include_version, stream_id_,
                             quic::QUIC_STREAM_CANCELLED, bytes_written,
                             /*include_stop_sending_if_v99=*/true));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket>
  ConstructClientAckAndRstStreamPacket(uint64_t packet_number,
                                       uint64_t largest_received,
                                       uint64_t smallest_received,
                                       uint64_t least_unacked) {
    return client_maker_.MakeAckAndRstPacket(
        packet_number, !kIncludeVersion, stream_id_,
        quic::QUIC_STREAM_CANCELLED, largest_received, smallest_received,
        least_unacked, !kIncludeCongestionFeedback);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructAckAndDataPacket(
      uint64_t packet_number,
      bool should_include_version,
      uint64_t largest_received,
      uint64_t smallest_received,
      uint64_t least_unacked,
      bool fin,
      quic::QuicStreamOffset offset,
      quic::QuicStringPiece data,
      QuicTestPacketMaker* maker) {
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        maker->MakeAckAndDataPacket(
            packet_number, should_include_version, stream_id_, largest_received,
            smallest_received, least_unacked, fin, offset, data));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket>
  ConstructAckAndMultipleDataFramesPacket(
      uint64_t packet_number,
      bool should_include_version,
      uint64_t largest_received,
      uint64_t smallest_received,
      uint64_t least_unacked,
      bool fin,
      quic::QuicStreamOffset offset,
      const std::vector<std::string> data_writes) {
    std::unique_ptr<quic::QuicReceivedPacket> packet(
        client_maker_.MakeAckAndMultipleDataFramesPacket(
            packet_number, should_include_version, stream_id_, largest_received,
            smallest_received, least_unacked, fin, offset, data_writes));
    DVLOG(2) << "packet(" << packet_number << "): " << std::endl
             << quic::QuicTextUtils::HexDump(packet->AsStringPiece());
    return packet;
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructClientAckPacket(
      uint64_t packet_number,
      uint64_t largest_received,
      uint64_t smallest_received,
      uint64_t least_unacked) {
    return client_maker_.MakeAckPacket(packet_number, largest_received,
                                       smallest_received, least_unacked,
                                       !kIncludeCongestionFeedback);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructServerAckPacket(
      uint64_t packet_number,
      uint64_t largest_received,
      uint64_t smallest_received,
      uint64_t least_unacked) {
    return server_maker_.MakeAckPacket(packet_number, largest_received,
                                       smallest_received, least_unacked,
                                       !kIncludeCongestionFeedback);
  }

  std::unique_ptr<quic::QuicReceivedPacket> ConstructInitialSettingsPacket(
      uint64_t packet_number,
      quic::QuicStreamOffset* offset) {
    return client_maker_.MakeInitialSettingsPacket(packet_number, offset);
  }

  void ExpectLoadTimingValid(const LoadTimingInfo& load_timing_info,
                             bool session_reused) {
    EXPECT_EQ(session_reused, load_timing_info.socket_reused);

    if (session_reused) {
      ExpectConnectTimingHasNoTimes(load_timing_info.connect_timing);
    } else {
      ExpectConnectTimingHasTimes(
          load_timing_info.connect_timing,
          CONNECT_TIMING_HAS_SSL_TIMES | CONNECT_TIMING_HAS_DNS_TIMES);
    }
    ExpectLoadTimingHasOnlyConnectionTimes(load_timing_info);
  }

  const BoundTestNetLog& net_log() const { return net_log_; }

  QuicChromiumClientSession* session() const { return session_.get(); }

  quic::QuicStreamId GetNthClientInitiatedBidirectionalStreamId(int n) {
    return quic::test::GetNthClientInitiatedBidirectionalStreamId(
        version_.transport_version, n);
  }

  std::string ConstructDataHeader(size_t body_len) {
    if (version_.transport_version != quic::QUIC_VERSION_99) {
      return "";
    }
    quic::HttpEncoder encoder;
    std::unique_ptr<char[]> buffer;
    auto header_length = encoder.SerializeDataFrameHeader(body_len, &buffer);
    return std::string(buffer.get(), header_length);
  }

 protected:
  const quic::ParsedQuicVersion version_;
  const bool client_headers_include_h2_stream_dependency_;
  BoundTestNetLog net_log_;
  scoped_refptr<TestTaskRunner> runner_;
  std::unique_ptr<MockWrite[]> mock_writes_;
  quic::MockClock clock_;
  quic::QuicConnection* connection_;
  std::unique_ptr<QuicChromiumConnectionHelper> helper_;
  std::unique_ptr<QuicChromiumAlarmFactory> alarm_factory_;
  TransportSecurityState transport_security_state_;
  std::unique_ptr<QuicChromiumClientSession> session_;
  quic::QuicCryptoClientConfig crypto_config_;
  HttpRequestHeaders headers_;
  HttpResponseInfo response_;
  scoped_refptr<IOBufferWithSize> read_buffer_;
  spdy::SpdyHeaderBlock request_headers_;
  const quic::QuicConnectionId connection_id_;
  const quic::QuicStreamId stream_id_;
  QuicTestPacketMaker client_maker_;
  QuicTestPacketMaker server_maker_;
  IPEndPoint self_addr_;
  IPEndPoint peer_addr_;
  quic::test::MockRandom random_generator_;
  MockCryptoClientStreamFactory crypto_client_stream_factory_;
  std::unique_ptr<StaticSocketDataProvider> socket_data_;
  std::vector<PacketToWrite> writes_;
  quic::QuicClientPushPromiseIndex push_promise_index_;
  HostPortPair destination_;
};

INSTANTIATE_TEST_SUITE_P(
    Version,
    BidirectionalStreamQuicImplTest,
    ::testing::Combine(::testing::ValuesIn(quic::AllSupportedVersions()),
                       ::testing::Bool()));

TEST_P(BidirectionalStreamQuicImplTest, GetRequest) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_ZERO_RTT);
  AddWrite(ConstructRequestHeadersPacketInner(
      1, GetNthClientInitiatedBidirectionalStreamId(0), kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_FORWARD_SECURE);
  AddWrite(ConstructInitialSettingsPacket(2, &header_stream_offset));
  AddWrite(ConstructClientAckPacket(3, 3, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->set_trailers_expected(true);
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnStreamReady);
  ConfirmHandshake();

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  LoadTimingInfo load_timing_info;
  EXPECT_TRUE(delegate->GetLoadTimingInfo(&load_timing_info));
  ExpectLoadTimingValid(load_timing_info, /*session_reused=*/false);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  // Server sends data.
  std::string header = ConstructDataHeader(strlen(kResponseBody));
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header + kResponseBody));
  EXPECT_EQ(12, cb.WaitForResult());

  EXPECT_EQ(std::string(kResponseBody), delegate->data_received());
  TestCompletionCallback cb2;
  EXPECT_THAT(delegate->ReadData(cb2.callback()), IsError(ERR_IO_PENDING));

  spdy::SpdyHeaderBlock trailers;
  size_t spdy_trailers_frame_length;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  EXPECT_THAT(cb2.WaitForResult(), IsOk());
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());

  EXPECT_THAT(delegate->ReadData(cb2.callback()), IsOk());
  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(2, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
  // Check that NetLog was filled as expected.
  TestNetLogEntry::List entries;
  net_log().GetEntries(&entries);
  size_t pos = ExpectLogContainsSomewhere(
      entries, /*min_offset=*/0,
      NetLogEventType::QUIC_CHROMIUM_CLIENT_STREAM_SEND_REQUEST_HEADERS,
      NetLogEventPhase::NONE);
  pos = ExpectLogContainsSomewhere(
      entries, /*min_offset=*/pos,
      NetLogEventType::QUIC_CHROMIUM_CLIENT_STREAM_SEND_REQUEST_HEADERS,
      NetLogEventPhase::NONE);
  ExpectLogContainsSomewhere(
      entries, /*min_offset=*/pos,
      NetLogEventType::QUIC_CHROMIUM_CLIENT_STREAM_SEND_REQUEST_HEADERS,
      NetLogEventPhase::NONE);
}

TEST_P(BidirectionalStreamQuicImplTest, LoadTimingTwoRequests) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  quic::QuicStreamOffset offset = 0;
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_ZERO_RTT);
  AddWrite(ConstructRequestHeadersPacketInner(
      1, GetNthClientInitiatedBidirectionalStreamId(0), kFin, DEFAULT_PRIORITY,
      nullptr, &offset));
  // SetRequest() again for second request as |request_headers_| was moved.
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(1), kFin, DEFAULT_PRIORITY,
      GetNthClientInitiatedBidirectionalStreamId(0), nullptr, &offset));
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_FORWARD_SECURE);
  AddWrite(ConstructInitialSettingsPacket(3, &offset));
  AddWrite(ConstructClientAckPacket(4, 3, 1, 2));
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  // Start first request.
  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));

  // Start second request.
  scoped_refptr<IOBuffer> read_buffer2 =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate2(
      new TestDelegateBase(read_buffer2.get(), kReadBufferSize));
  delegate2->Start(&request, net_log().bound(),
                   session()->CreateHandle(destination_));

  delegate->WaitUntilNextCallback(kOnStreamReady);
  delegate2->WaitUntilNextCallback(kOnStreamReady);

  ConfirmHandshake();
  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  offset = 0;
  ProcessPacket(ConstructResponseHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), kFin,
      ConstructResponseHeaders("200"), nullptr, &offset));

  ProcessPacket(ConstructResponseHeadersPacketInner(
      3, GetNthClientInitiatedBidirectionalStreamId(1), kFin,
      ConstructResponseHeaders("200"), nullptr, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  delegate2->WaitUntilNextCallback(kOnHeadersReceived);

  LoadTimingInfo load_timing_info;
  EXPECT_TRUE(delegate->GetLoadTimingInfo(&load_timing_info));
  LoadTimingInfo load_timing_info2;
  EXPECT_TRUE(delegate2->GetLoadTimingInfo(&load_timing_info2));
  ExpectLoadTimingValid(load_timing_info, /*session_reused=*/false);
  ExpectLoadTimingValid(load_timing_info2, /*session_reused=*/true);
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  EXPECT_EQ("200", delegate2->response_headers().find(":status")->second);
  // No response body. ReadData() should return OK synchronously.
  TestCompletionCallback dummy_callback;
  EXPECT_EQ(OK, delegate->ReadData(dummy_callback.callback()));
  EXPECT_EQ(OK, delegate2->ReadData(dummy_callback.callback()));
}

// Tests that when request headers are not delayed, only data buffers are
// coalesced.
TEST_P(BidirectionalStreamQuicImplTest, CoalesceDataBuffersNotHeadersFrame) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  const char kBody1[] = "here are some data";
  const char kBody2[] = "data keep coming";
  std::string header = ConstructDataHeader(strlen(kBody1));
  std::string header2 = ConstructDataHeader(strlen(kBody2));
  std::vector<std::string> two_writes = {kBody1, kBody2};
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  if (version_.transport_version != quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, !kFin,
                                                     0, {kBody1, kBody2}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        3, kIncludeVersion, !kFin, 0, {header, kBody1, header2, kBody2}));
  }

  // Ack server's data packet.
  AddWrite(ConstructClientAckPacket(4, 3, 1, 2));
  const char kBody3[] = "hello there";
  const char kBody4[] = "another piece of small data";
  const char kBody5[] = "really small";
  std::string header3 = ConstructDataHeader(strlen(kBody3));
  std::string header4 = ConstructDataHeader(strlen(kBody4));
  std::string header5 = ConstructDataHeader(strlen(kBody5));
  quic::QuicStreamOffset data_offset =
      strlen(kBody1) + strlen(kBody2) + header.length() + header2.length();
  if (version_.transport_version != quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        5, !kIncludeVersion, kFin, data_offset, {kBody3, kBody4, kBody5}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        5, !kIncludeVersion, kFin, data_offset,
        {header3, kBody3, header4, kBody4, header5, kBody5}));
  }

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->DoNotSendRequestHeadersAutomatically();
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  EXPECT_FALSE(delegate->is_ready());
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);
  EXPECT_TRUE(delegate->is_ready());

  // Sends request headers separately, which causes them to be sent in a
  // separate packet.
  delegate->SendRequestHeaders();
  // Send a Data packet.
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody1);
  scoped_refptr<StringIOBuffer> buf2 =
      base::MakeRefCounted<StringIOBuffer>(kBody2);

  std::vector<int> lengths = {buf1->size(), buf2->size()};
  delegate->SendvData({buf1, buf2}, lengths, !kFin);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  std::string header6 = ConstructDataHeader(strlen(kResponseBody));
  // Server sends data.
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header6 + kResponseBody));

  EXPECT_EQ(static_cast<int>(strlen(kResponseBody)), cb.WaitForResult());

  // Send a second Data packet.
  scoped_refptr<StringIOBuffer> buf3 =
      base::MakeRefCounted<StringIOBuffer>(kBody3);
  scoped_refptr<StringIOBuffer> buf4 =
      base::MakeRefCounted<StringIOBuffer>(kBody4);
  scoped_refptr<StringIOBuffer> buf5 =
      base::MakeRefCounted<StringIOBuffer>(kBody5);

  delegate->SendvData({buf3, buf4, buf5},
                      {buf3->size(), buf4->size(), buf5->size()}, kFin);
  delegate->WaitUntilNextCallback(kOnDataSent);

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(2, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(
                spdy_request_headers_frame_length + strlen(kBody1) +
                strlen(kBody2) + strlen(kBody3) + strlen(kBody4) +
                strlen(kBody5) + header.length() + header2.length() +
                header3.length() + header4.length() + header5.length()),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header6.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
}

// Tests that when request headers are delayed, SendData triggers coalescing of
// request headers with data buffers.
TEST_P(BidirectionalStreamQuicImplTest,
       SendDataCoalesceDataBufferAndHeaderFrame) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  const char kBody1[] = "here are some data";
  std::string header = ConstructDataHeader(strlen(kBody1));
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructRequestHeadersAndMultipleDataFramesPacket(
        2, !kFin, DEFAULT_PRIORITY, &header_stream_offset,
        &spdy_request_headers_frame_length, {header, kBody1}));
  } else {
    AddWrite(ConstructRequestHeadersAndMultipleDataFramesPacket(
        2, !kFin, DEFAULT_PRIORITY, &header_stream_offset,
        &spdy_request_headers_frame_length, {kBody1}));
  }

  // Ack server's data packet.
  AddWrite(ConstructClientAckPacket(3, 3, 1, 2));
  const char kBody2[] = "really small";
  std::string header2 = ConstructDataHeader(strlen(kBody2));
  quic::QuicStreamOffset data_offset = strlen(kBody1) + header.length();
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        4, !kIncludeVersion, kFin, data_offset, {header2, kBody2}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(4, !kIncludeVersion, kFin,
                                                     data_offset, {kBody2}));
  }

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->DoNotSendRequestHeadersAutomatically();
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Send a Data packet.
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody1);

  delegate->SendData(buf1, buf1->size(), false);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  // Server sends data.
  std::string header3 = ConstructDataHeader(strlen(kResponseBody));
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header3 + kResponseBody));

  EXPECT_EQ(static_cast<int>(strlen(kResponseBody)), cb.WaitForResult());

  // Send a second Data packet.
  scoped_refptr<StringIOBuffer> buf2 =
      base::MakeRefCounted<StringIOBuffer>(kBody2);

  delegate->SendData(buf2, buf2->size(), true);
  delegate->WaitUntilNextCallback(kOnDataSent);

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(2, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(
      static_cast<int64_t>(spdy_request_headers_frame_length + strlen(kBody1) +
                           strlen(kBody2) + header.length() + header2.length()),
      delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header3.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
}

// Tests that when request headers are delayed, SendvData triggers coalescing of
// request headers with data buffers.
TEST_P(BidirectionalStreamQuicImplTest,
       SendvDataCoalesceDataBuffersAndHeaderFrame) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  const char kBody1[] = "here are some data";
  const char kBody2[] = "data keep coming";
  std::string header = ConstructDataHeader(strlen(kBody1));
  std::string header2 = ConstructDataHeader(strlen(kBody2));

  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructRequestHeadersAndMultipleDataFramesPacket(
        2, !kFin, DEFAULT_PRIORITY, &header_stream_offset,
        &spdy_request_headers_frame_length, {header, kBody1, header2, kBody2}));
  } else {
    AddWrite(ConstructRequestHeadersAndMultipleDataFramesPacket(
        2, !kFin, DEFAULT_PRIORITY, &header_stream_offset,
        &spdy_request_headers_frame_length, {kBody1, kBody2}));
  }

  // Ack server's data packet.
  AddWrite(ConstructClientAckPacket(3, 3, 1, 2));
  const char kBody3[] = "hello there";
  const char kBody4[] = "another piece of small data";
  const char kBody5[] = "really small";
  std::string header3 = ConstructDataHeader(strlen(kBody3));
  std::string header4 = ConstructDataHeader(strlen(kBody4));
  std::string header5 = ConstructDataHeader(strlen(kBody5));
  quic::QuicStreamOffset data_offset =
      strlen(kBody1) + strlen(kBody2) + header.length() + header2.length();
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        4, !kIncludeVersion, kFin, data_offset,
        {header3, kBody3, header4, kBody4, header5, kBody5}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(
        4, !kIncludeVersion, kFin, data_offset, {kBody3, kBody4, kBody5}));
  }

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->DoNotSendRequestHeadersAutomatically();
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Send a Data packet.
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody1);
  scoped_refptr<StringIOBuffer> buf2 =
      base::MakeRefCounted<StringIOBuffer>(kBody2);

  std::vector<int> lengths = {buf1->size(), buf2->size()};
  delegate->SendvData({buf1, buf2}, lengths, !kFin);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  std::string header6 = ConstructDataHeader(strlen(kResponseBody));
  // Server sends data.
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header6 + kResponseBody));

  EXPECT_EQ(static_cast<int>(strlen(kResponseBody)), cb.WaitForResult());

  // Send a second Data packet.
  scoped_refptr<StringIOBuffer> buf3 =
      base::MakeRefCounted<StringIOBuffer>(kBody3);
  scoped_refptr<StringIOBuffer> buf4 =
      base::MakeRefCounted<StringIOBuffer>(kBody4);
  scoped_refptr<StringIOBuffer> buf5 =
      base::MakeRefCounted<StringIOBuffer>(kBody5);

  delegate->SendvData({buf3, buf4, buf5},
                      {buf3->size(), buf4->size(), buf5->size()}, kFin);
  delegate->WaitUntilNextCallback(kOnDataSent);

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(2, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(
                spdy_request_headers_frame_length + strlen(kBody1) +
                strlen(kBody2) + strlen(kBody3) + strlen(kBody4) +
                strlen(kBody5) + header.length() + header2.length() +
                header3.length() + header4.length() + header5.length()),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header6.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
}

// Tests that when request headers are delayed and SendData triggers the
// headers to be sent, if that write fails the stream does not crash.
TEST_P(BidirectionalStreamQuicImplTest,
       SendDataWriteErrorCoalesceDataBufferAndHeaderFrame) {
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWriteError(SYNCHRONOUS, ERR_CONNECTION_REFUSED);

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;
  request.extra_headers.SetHeader("cookie", std::string(2048, 'A'));

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(new DeleteStreamDelegate(
      read_buffer.get(), kReadBufferSize, DeleteStreamDelegate::ON_FAILED));
  delegate->DoNotSendRequestHeadersAutomatically();
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Attempt to send the headers and data.
  const char kBody1[] = "here are some data";
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody1);
  delegate->SendData(buf1, buf1->size(), !kFin);

  delegate->WaitUntilNextCallback(kOnFailed);
}

// Tests that when request headers are delayed and SendvData triggers the
// headers to be sent, if that write fails the stream does not crash.
TEST_P(BidirectionalStreamQuicImplTest,
       SendvDataWriteErrorCoalesceDataBufferAndHeaderFrame) {
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWriteError(SYNCHRONOUS, ERR_CONNECTION_REFUSED);

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;
  request.extra_headers.SetHeader("cookie", std::string(2048, 'A'));

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(new DeleteStreamDelegate(
      read_buffer.get(), kReadBufferSize, DeleteStreamDelegate::ON_FAILED));
  delegate->DoNotSendRequestHeadersAutomatically();
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Attempt to send the headers and data.
  const char kBody1[] = "here are some data";
  const char kBody2[] = "data keep coming";
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody1);
  scoped_refptr<StringIOBuffer> buf2 =
      base::MakeRefCounted<StringIOBuffer>(kBody2);
  std::vector<int> lengths = {buf1->size(), buf2->size()};
  delegate->SendvData({buf1, buf2}, lengths, !kFin);

  delegate->WaitUntilNextCallback(kOnFailed);
}

TEST_P(BidirectionalStreamQuicImplTest, PostRequest) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  std::string header = ConstructDataHeader(strlen(kUploadData));
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {header, kUploadData}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {kUploadData}));
  }

  AddWrite(ConstructClientAckPacket(4, 3, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Send a DATA frame.
  scoped_refptr<StringIOBuffer> buf =
      base::MakeRefCounted<StringIOBuffer>(kUploadData);

  delegate->SendData(buf, buf->size(), true);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  std::string header2 = ConstructDataHeader(strlen(kResponseBody));
  // Server sends data.
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header2 + kResponseBody));

  EXPECT_EQ(static_cast<int>(strlen(kResponseBody)), cb.WaitForResult());

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(1, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length +
                                 strlen(kUploadData) + header.length()),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header2.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, EarlyDataOverrideRequest) {
  SetRequest("PUT", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  std::string header = ConstructDataHeader(strlen(kUploadData));
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {header, kUploadData}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {kUploadData}));
  }

  AddWrite(ConstructClientAckPacket(4, 3, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "PUT";
  request.allow_early_data_override = true;
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Send a DATA frame.
  scoped_refptr<StringIOBuffer> buf =
      base::MakeRefCounted<StringIOBuffer>(kUploadData);

  delegate->SendData(buf, buf->size(), true);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);
  const char kResponseBody[] = "Hello world!";
  // Server sends data.
  std::string header2 = ConstructDataHeader(strlen(kResponseBody));
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header2 + kResponseBody));

  EXPECT_EQ(static_cast<int>(strlen(kResponseBody)), cb.WaitForResult());

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(1, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length +
                                 strlen(kUploadData) + header.length()),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length +
                                 strlen(kResponseBody) + header2.length() +
                                 spdy_trailers_frame_length),
            delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, InterleaveReadDataAndSendData) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));

  std::string header = ConstructDataHeader(strlen(kUploadData));
  if (version_.transport_version != quic::QUIC_VERSION_99) {
    AddWrite(ConstructAckAndDataPacket(3, !kIncludeVersion, 2, 1, 2, !kFin, 0,
                                       kUploadData, &client_maker_));
    AddWrite(ConstructAckAndDataPacket(4, !kIncludeVersion, 3, 3, 3, kFin,
                                       strlen(kUploadData), kUploadData,
                                       &client_maker_));
  } else {
    AddWrite(ConstructAckAndMultipleDataFramesPacket(
        3, !kIncludeVersion, 2, 1, 1, !kFin, 0, {header, kUploadData}));
    AddWrite(ConstructAckAndMultipleDataFramesPacket(
        4, !kIncludeVersion, 3, 3, 3, kFin,
        strlen(kUploadData) + header.length(), {header, kUploadData}));
  }
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, nullptr));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  // Client sends a data packet.
  scoped_refptr<StringIOBuffer> buf =
      base::MakeRefCounted<StringIOBuffer>(kUploadData);

  delegate->SendData(buf, buf->size(), false);
  delegate->WaitUntilNextCallback(kOnDataSent);

  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  const char kResponseBody[] = "Hello world!";

  std::string header2 = ConstructDataHeader(strlen(kResponseBody));
  // Server sends a data packet.
  ProcessPacket(ConstructAckAndDataPacket(3, !kIncludeVersion, 2, 1, 1, !kFin,
                                          0, header2 + kResponseBody,
                                          &server_maker_));

  EXPECT_EQ(static_cast<int64_t>(strlen(kResponseBody)), cb.WaitForResult());
  EXPECT_EQ(std::string(kResponseBody), delegate->data_received());

  // Client sends a data packet.
  delegate->SendData(buf, buf->size(), true);
  delegate->WaitUntilNextCallback(kOnDataSent);

  TestCompletionCallback cb2;
  rv = delegate->ReadData(cb2.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  ProcessPacket(
      ConstructAckAndDataPacket(4, !kIncludeVersion, 3, 1, 1, kFin,
                                strlen(kResponseBody) + header2.length(),
                                header2 + kResponseBody, &server_maker_));

  EXPECT_EQ(static_cast<int64_t>(strlen(kResponseBody)), cb2.WaitForResult());

  std::string expected_body(kResponseBody);
  expected_body.append(kResponseBody);
  EXPECT_EQ(expected_body, delegate->data_received());

  EXPECT_THAT(delegate->ReadData(cb.callback()), IsOk());
  EXPECT_EQ(2, delegate->on_data_read_count());
  EXPECT_EQ(2, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length +
                                 2 * strlen(kUploadData) + 2 * header.length()),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(
      static_cast<int64_t>(spdy_response_headers_frame_length +
                           2 * strlen(kResponseBody) + 2 * header2.length()),
      delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, ServerSendsRstAfterHeaders) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_ZERO_RTT);
  AddWrite(ConstructRequestHeadersPacketInner(
      1, GetNthClientInitiatedBidirectionalStreamId(0), kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_FORWARD_SECURE);
  AddWrite(ConstructInitialSettingsPacket(2, &header_stream_offset));
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnStreamReady);
  ConfirmHandshake();

  // Server sends a Rst.
  ProcessPacket(ConstructServerRstStreamPacket(1));

  delegate->WaitUntilNextCallback(kOnFailed);

  TestCompletionCallback cb;
  EXPECT_THAT(delegate->ReadData(cb.callback()),
              IsError(ERR_QUIC_PROTOCOL_ERROR));

  base::RunLoop().RunUntilIdle();

  EXPECT_THAT(delegate->error(), IsError(ERR_QUIC_PROTOCOL_ERROR));
  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(0, delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, ServerSendsRstAfterReadData) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_ZERO_RTT);
  AddWrite(ConstructRequestHeadersPacketInner(
      1, GetNthClientInitiatedBidirectionalStreamId(0), kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_FORWARD_SECURE);
  AddWrite(ConstructInitialSettingsPacket(2, &header_stream_offset));
  // Why does QUIC ack Rst? Is this expected?
  AddWrite(ConstructClientAckPacket(3, 3, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnStreamReady);
  ConfirmHandshake();

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Server sends a Rst.
  ProcessPacket(ConstructServerRstStreamPacket(3));

  delegate->WaitUntilNextCallback(kOnFailed);

  EXPECT_THAT(delegate->ReadData(cb.callback()),
              IsError(ERR_QUIC_PROTOCOL_ERROR));
  EXPECT_THAT(delegate->error(), IsError(ERR_QUIC_PROTOCOL_ERROR));
  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length),
            delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, SessionClosedBeforeReadData) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  quic::QuicStreamOffset offset = 0;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  session()->connection()->CloseConnection(
      quic::QUIC_NO_ERROR, "test", quic::ConnectionCloseBehavior::SILENT_CLOSE);
  delegate->WaitUntilNextCallback(kOnFailed);

  // Try to send data after OnFailed(), should not get called back.
  scoped_refptr<StringIOBuffer> buf =
      base::MakeRefCounted<StringIOBuffer>(kUploadData);
  delegate->SendData(buf, buf->size(), false);

  EXPECT_THAT(delegate->ReadData(cb.callback()),
              IsError(ERR_QUIC_PROTOCOL_ERROR));
  EXPECT_THAT(delegate->error(), IsError(ERR_QUIC_PROTOCOL_ERROR));
  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length),
            delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, SessionClosedBeforeStartConfirmed) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  ConfirmHandshake();
  session()->connection()->CloseConnection(
      quic::QUIC_NO_ERROR, "test", quic::ConnectionCloseBehavior::SILENT_CLOSE);

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnFailed);
  EXPECT_TRUE(delegate->on_failed_called());
  EXPECT_THAT(delegate->error(), IsError(ERR_CONNECTION_CLOSED));
}

TEST_P(BidirectionalStreamQuicImplTest, SessionClosedBeforeStartNotConfirmed) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  Initialize();

  session()->connection()->CloseConnection(
      quic::QUIC_NO_ERROR, "test", quic::ConnectionCloseBehavior::SILENT_CLOSE);

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnFailed);
  EXPECT_TRUE(delegate->on_failed_called());
  EXPECT_THAT(delegate->error(), IsError(ERR_QUIC_HANDSHAKE_FAILED));
}

TEST_P(BidirectionalStreamQuicImplTest, SessionCloseDuringOnStreamReady) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWriteError(SYNCHRONOUS, ERR_CONNECTION_REFUSED);

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(new DeleteStreamDelegate(
      read_buffer.get(), kReadBufferSize, DeleteStreamDelegate::ON_FAILED));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnFailed);

  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
}

TEST_P(BidirectionalStreamQuicImplTest, DeleteStreamDuringOnStreamReady) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  AddWrite(ConstructClientEarlyRstStreamPacket(3));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(
      new DeleteStreamDelegate(read_buffer.get(), kReadBufferSize,
                               DeleteStreamDelegate::ON_STREAM_READY));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
}

TEST_P(BidirectionalStreamQuicImplTest, DeleteStreamAfterReadData) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  AddWrite(ConstructClientAckAndRstStreamPacket(3, 2, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");
  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, nullptr));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  // Cancel the stream after ReadData returns ERR_IO_PENDING.
  TestCompletionCallback cb;
  EXPECT_THAT(delegate->ReadData(cb.callback()), IsError(ERR_IO_PENDING));
  delegate->DeleteStream();

  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
  EXPECT_EQ(kProtoQUIC, delegate->GetProtocol());
  EXPECT_EQ(static_cast<int64_t>(spdy_request_headers_frame_length),
            delegate->GetTotalSentBytes());
  EXPECT_EQ(static_cast<int64_t>(spdy_response_headers_frame_length),
            delegate->GetTotalReceivedBytes());
}

TEST_P(BidirectionalStreamQuicImplTest, DeleteStreamDuringOnHeadersReceived) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  AddWrite(ConstructClientAckAndRstStreamPacket(3, 2, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(
      new DeleteStreamDelegate(read_buffer.get(), kReadBufferSize,
                               DeleteStreamDelegate::ON_HEADERS_RECEIVED));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, nullptr));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);
  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(0, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
}

TEST_P(BidirectionalStreamQuicImplTest, DeleteStreamDuringOnDataRead) {
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  AddWrite(ConstructClientAckPacket(3, 3, 1, 2));
  AddWrite(ConstructClientRstStreamPacket(4));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(new DeleteStreamDelegate(
      read_buffer.get(), kReadBufferSize, DeleteStreamDelegate::ON_DATA_READ));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, nullptr));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);

  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  const char kResponseBody[] = "Hello world!";
  std::string header = ConstructDataHeader(strlen(kResponseBody));
  // Server sends data.
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header + kResponseBody));
  EXPECT_EQ(static_cast<int64_t>(strlen(kResponseBody)), cb.WaitForResult());

  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
}

TEST_P(BidirectionalStreamQuicImplTest, AsyncFinRead) {
  const char kBody[] = "here is some data";
  SetRequest("POST", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  quic::QuicStreamOffset header_stream_offset = 0;
  AddWrite(ConstructInitialSettingsPacket(1, &header_stream_offset));
  AddWrite(ConstructRequestHeadersPacketInner(
      2, GetNthClientInitiatedBidirectionalStreamId(0), !kFin, DEFAULT_PRIORITY,
      &spdy_request_headers_frame_length, &header_stream_offset));
  std::string header = ConstructDataHeader(strlen(kBody));
  if (version_.transport_version == quic::QUIC_VERSION_99) {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {header, kBody}));
  } else {
    AddWrite(ConstructClientMultipleDataFramesPacket(3, kIncludeVersion, kFin,
                                                     0, {kBody}));
  }
  AddWrite(ConstructClientAckPacket(4, 3, 1, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "POST";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = false;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));

  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  ConfirmHandshake();
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Send a Data packet with fin set.
  scoped_refptr<StringIOBuffer> buf1 =
      base::MakeRefCounted<StringIOBuffer>(kBody);
  delegate->SendData(buf1, buf1->size(), /*fin*/ true);
  delegate->WaitUntilNextCallback(kOnDataSent);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, nullptr));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);

  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  // Read the body, which will complete asynchronously.
  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  const char kResponseBody[] = "Hello world!";
  std::string header2 = ConstructDataHeader(strlen(kResponseBody));

  // Server sends data with the fin set, which should result in the stream
  // being closed and hence no RST_STREAM will be sent.
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, kFin, 0,
                                          header2 + kResponseBody));
  EXPECT_EQ(static_cast<int64_t>(strlen(kResponseBody)), cb.WaitForResult());

  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(1, delegate->on_data_sent_count());
}

TEST_P(BidirectionalStreamQuicImplTest, DeleteStreamDuringOnTrailersReceived) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  size_t spdy_request_headers_frame_length;
  client_maker_.SetEncryptionLevel(quic::ENCRYPTION_ZERO_RTT);
  AddWrite(ConstructRequestHeadersPacket(1, kFin, DEFAULT_PRIORITY,
                                         &spdy_request_headers_frame_length));
  AddWrite(ConstructClientAckPacket(2, 3, 1, 2));  // Ack the data packet
  AddWrite(ConstructClientAckAndRstStreamPacket(3, 4, 4, 2));

  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<DeleteStreamDelegate> delegate(
      new DeleteStreamDelegate(read_buffer.get(), kReadBufferSize,
                               DeleteStreamDelegate::ON_TRAILERS_RECEIVED));
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  delegate->WaitUntilNextCallback(kOnStreamReady);

  // Server acks the request.
  ProcessPacket(ConstructServerAckPacket(1, 1, 1, 1));

  // Server sends the response headers.
  spdy::SpdyHeaderBlock response_headers = ConstructResponseHeaders("200");

  quic::QuicStreamOffset offset = 0;
  size_t spdy_response_headers_frame_length;
  ProcessPacket(ConstructResponseHeadersPacket(
      2, !kFin, std::move(response_headers),
      &spdy_response_headers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnHeadersReceived);

  EXPECT_EQ("200", delegate->response_headers().find(":status")->second);

  TestCompletionCallback cb;
  int rv = delegate->ReadData(cb.callback());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  const char kResponseBody[] = "Hello world!";

  // Server sends data.
  std::string header = ConstructDataHeader(strlen(kResponseBody));
  ProcessPacket(ConstructServerDataPacket(3, !kIncludeVersion, !kFin, 0,
                                          header + kResponseBody));

  EXPECT_EQ(static_cast<int64_t>(strlen(kResponseBody)), cb.WaitForResult());
  EXPECT_EQ(std::string(kResponseBody), delegate->data_received());

  size_t spdy_trailers_frame_length;
  spdy::SpdyHeaderBlock trailers;
  trailers["foo"] = "bar";
  trailers[quic::kFinalOffsetHeaderKey] =
      base::NumberToString(strlen(kResponseBody));
  // Server sends trailers.
  ProcessPacket(ConstructResponseTrailersPacket(
      4, kFin, trailers.Clone(), &spdy_trailers_frame_length, &offset));

  delegate->WaitUntilNextCallback(kOnTrailersReceived);
  trailers.erase(quic::kFinalOffsetHeaderKey);
  EXPECT_EQ(trailers, delegate->trailers());

  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(1, delegate->on_data_read_count());
  EXPECT_EQ(0, delegate->on_data_sent_count());
}

// Tests that if QuicChromiumClientSession is closed after
// BidirectionalStreamQuicImpl::OnStreamReady() but before
// QuicChromiumClientSession::Handle::ReleaseStream() is called, there is no
// crash. Regression test for crbug.com/754823.
TEST_P(BidirectionalStreamQuicImplTest, ReleaseStreamFails) {
  SetRequest("GET", "/", DEFAULT_PRIORITY);
  Initialize();

  BidirectionalStreamRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.google.com/");
  request.end_stream_on_headers = true;
  request.priority = DEFAULT_PRIORITY;

  scoped_refptr<IOBuffer> read_buffer =
      base::MakeRefCounted<IOBuffer>(kReadBufferSize);
  std::unique_ptr<TestDelegateBase> delegate(
      new TestDelegateBase(read_buffer.get(), kReadBufferSize));
  delegate->set_trailers_expected(true);
  // QuicChromiumClientSession::Handle::RequestStream() returns OK synchronously
  // because Initialize() has established a Session.
  delegate->Start(&request, net_log().bound(),
                  session()->CreateHandle(destination_));
  // Now closes the underlying session.
  session_->CloseSessionOnError(ERR_ABORTED, quic::QUIC_INTERNAL_ERROR,
                                quic::ConnectionCloseBehavior::SILENT_CLOSE);
  delegate->WaitUntilNextCallback(kOnFailed);

  EXPECT_THAT(delegate->error(), IsError(ERR_CONNECTION_CLOSED));
}

}  // namespace test

}  // namespace net
