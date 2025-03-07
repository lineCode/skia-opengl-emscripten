// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/socket/transport_client_socket_pool.h"

#include <stdint.h>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/optional.h"
#include "base/run_loop.h"
#include "base/single_thread_task_runner.h"
#include "base/stl_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread_task_runner_handle.h"
#include "base/values.h"
#include "net/base/load_timing_info.h"
#include "net/base/load_timing_info_test_util.h"
#include "net/base/net_errors.h"
#include "net/base/privacy_mode.h"
#include "net/base/proxy_server.h"
#include "net/base/request_priority.h"
#include "net/base/test_completion_callback.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_response_info.h"
#include "net/log/net_log.h"
#include "net/log/net_log_event_type.h"
#include "net/log/net_log_source.h"
#include "net/log/net_log_source_type.h"
#include "net/log/test_net_log.h"
#include "net/log/test_net_log_entry.h"
#include "net/log/test_net_log_util.h"
#include "net/socket/client_socket_factory.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/datagram_client_socket.h"
#include "net/socket/socket_performance_watcher.h"
#include "net/socket/socket_tag.h"
#include "net/socket/socket_test_util.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/stream_socket.h"
#include "net/socket/transport_connect_job.h"
#include "net/ssl/ssl_cert_request_info.h"
#include "net/test/gtest_util.h"
#include "net/test/test_with_scoped_task_environment.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using net::test::IsError;
using net::test::IsOk;

using ::testing::Invoke;
using ::testing::Return;

namespace net {

namespace {

const int kDefaultMaxSockets = 4;
const int kDefaultMaxSocketsPerGroup = 2;
constexpr base::TimeDelta kUnusedIdleSocketTimeout =
    base::TimeDelta::FromSeconds(10);

ClientSocketPool::GroupId TestGroupId(
    const std::string& host,
    int port = 80,
    ClientSocketPool::SocketType socket_type =
        ClientSocketPool::SocketType::kHttp,
    PrivacyMode privacy_mode = PrivacyMode::PRIVACY_MODE_DISABLED) {
  return ClientSocketPool::GroupId(HostPortPair(host, port), socket_type,
                                   privacy_mode);
}

// Make sure |handle| sets load times correctly when it has been assigned a
// reused socket.
void TestLoadTimingInfoConnectedReused(const ClientSocketHandle& handle) {
  LoadTimingInfo load_timing_info;
  // Only pass true in as |is_reused|, as in general, HttpStream types should
  // have stricter concepts of reuse than socket pools.
  EXPECT_TRUE(handle.GetLoadTimingInfo(true, &load_timing_info));

  EXPECT_EQ(true, load_timing_info.socket_reused);
  EXPECT_NE(NetLogSource::kInvalidId, load_timing_info.socket_log_id);

  ExpectConnectTimingHasNoTimes(load_timing_info.connect_timing);
  ExpectLoadTimingHasOnlyConnectionTimes(load_timing_info);
}

// Make sure |handle| sets load times correctly when it has been assigned a
// fresh socket. Also runs TestLoadTimingInfoConnectedReused, since the owner
// of a connection where |is_reused| is false may consider the connection
// reused.
void TestLoadTimingInfoConnectedNotReused(const ClientSocketHandle& handle) {
  EXPECT_FALSE(handle.is_reused());

  LoadTimingInfo load_timing_info;
  EXPECT_TRUE(handle.GetLoadTimingInfo(false, &load_timing_info));

  EXPECT_FALSE(load_timing_info.socket_reused);
  EXPECT_NE(NetLogSource::kInvalidId, load_timing_info.socket_log_id);

  ExpectConnectTimingHasTimes(load_timing_info.connect_timing,
                              CONNECT_TIMING_HAS_CONNECT_TIMES_ONLY);
  ExpectLoadTimingHasOnlyConnectionTimes(load_timing_info);

  TestLoadTimingInfoConnectedReused(handle);
}

// Make sure |handle| sets load times correctly, in the case that it does not
// currently have a socket.
void TestLoadTimingInfoNotConnected(const ClientSocketHandle& handle) {
  // Should only be set to true once a socket is assigned, if at all.
  EXPECT_FALSE(handle.is_reused());

  LoadTimingInfo load_timing_info;
  EXPECT_FALSE(handle.GetLoadTimingInfo(false, &load_timing_info));

  EXPECT_FALSE(load_timing_info.socket_reused);
  EXPECT_EQ(NetLogSource::kInvalidId, load_timing_info.socket_log_id);

  ExpectConnectTimingHasNoTimes(load_timing_info.connect_timing);
  ExpectLoadTimingHasOnlyConnectionTimes(load_timing_info);
}

class MockClientSocket : public StreamSocket {
 public:
  explicit MockClientSocket(net::NetLog* net_log)
      : connected_(false),
        has_unread_data_(false),
        net_log_(NetLogWithSource::Make(net_log, NetLogSourceType::SOCKET)),
        was_used_to_convey_data_(false) {}

  // Sets whether the socket has unread data. If true, the next call to Read()
  // will return 1 byte and IsConnectedAndIdle() will return false.
  void set_has_unread_data(bool has_unread_data) {
    has_unread_data_ = has_unread_data;
  }

  // Socket implementation.
  int Read(IOBuffer* /* buf */,
           int len,
           CompletionOnceCallback /* callback */) override {
    if (has_unread_data_ && len > 0) {
      has_unread_data_ = false;
      was_used_to_convey_data_ = true;
      return 1;
    }
    return ERR_UNEXPECTED;
  }

  int Write(
      IOBuffer* /* buf */,
      int len,
      CompletionOnceCallback /* callback */,
      const NetworkTrafficAnnotationTag& /*traffic_annotation*/) override {
    was_used_to_convey_data_ = true;
    return len;
  }
  int SetReceiveBufferSize(int32_t size) override { return OK; }
  int SetSendBufferSize(int32_t size) override { return OK; }

  // StreamSocket implementation.
  int Connect(CompletionOnceCallback callback) override {
    connected_ = true;
    return OK;
  }

  void Disconnect() override { connected_ = false; }
  bool IsConnected() const override { return connected_; }
  bool IsConnectedAndIdle() const override {
    return connected_ && !has_unread_data_;
  }

  int GetPeerAddress(IPEndPoint* /* address */) const override {
    return ERR_UNEXPECTED;
  }

  int GetLocalAddress(IPEndPoint* /* address */) const override {
    return ERR_UNEXPECTED;
  }

  const NetLogWithSource& NetLog() const override { return net_log_; }

  bool WasEverUsed() const override { return was_used_to_convey_data_; }
  bool WasAlpnNegotiated() const override { return false; }
  NextProto GetNegotiatedProtocol() const override { return kProtoUnknown; }
  bool GetSSLInfo(SSLInfo* ssl_info) override { return false; }
  void GetConnectionAttempts(ConnectionAttempts* out) const override {
    out->clear();
  }
  void ClearConnectionAttempts() override {}
  void AddConnectionAttempts(const ConnectionAttempts& attempts) override {}
  int64_t GetTotalReceivedBytes() const override {
    NOTIMPLEMENTED();
    return 0;
  }
  void ApplySocketTag(const SocketTag& tag) override {}

 private:
  bool connected_;
  bool has_unread_data_;
  NetLogWithSource net_log_;
  bool was_used_to_convey_data_;

  DISALLOW_COPY_AND_ASSIGN(MockClientSocket);
};

class TestConnectJob;

class MockClientSocketFactory : public ClientSocketFactory {
 public:
  MockClientSocketFactory() : allocation_count_(0) {}

  std::unique_ptr<DatagramClientSocket> CreateDatagramClientSocket(
      DatagramSocket::BindType bind_type,
      NetLog* net_log,
      const NetLogSource& source) override {
    NOTREACHED();
    return std::unique_ptr<DatagramClientSocket>();
  }

  std::unique_ptr<TransportClientSocket> CreateTransportClientSocket(
      const AddressList& addresses,
      std::unique_ptr<
          SocketPerformanceWatcher> /* socket_performance_watcher */,
      NetLog* /* net_log */,
      const NetLogSource& /*source*/) override {
    allocation_count_++;
    return nullptr;
  }

  std::unique_ptr<SSLClientSocket> CreateSSLClientSocket(
      std::unique_ptr<StreamSocket> stream_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      const SSLClientSocketContext& context) override {
    NOTIMPLEMENTED();
    return std::unique_ptr<SSLClientSocket>();
  }

  std::unique_ptr<ProxyClientSocket> CreateProxyClientSocket(
      std::unique_ptr<StreamSocket> stream_socket,
      const std::string& user_agent,
      const HostPortPair& endpoint,
      const ProxyServer& proxy_server,
      HttpAuthController* http_auth_controller,
      bool tunnel,
      bool using_spdy,
      NextProto negotiated_protocol,
      ProxyDelegate* proxy_delegate,
      const NetworkTrafficAnnotationTag& traffic_annotation) override {
    NOTIMPLEMENTED();
    return nullptr;
  }

  void WaitForSignal(TestConnectJob* job) { waiting_jobs_.push_back(job); }

  void SignalJobs();

  void SignalJob(size_t job);

  void SetJobLoadState(size_t job, LoadState load_state);

  // Sets the HasConnectionEstablished value of the specified job to true,
  // without invoking the callback.
  void SetJobHasEstablishedConnection(size_t job);

  int allocation_count() const { return allocation_count_; }

 private:
  int allocation_count_;
  std::vector<TestConnectJob*> waiting_jobs_;
};

class TestConnectJob : public ConnectJob {
 public:
  enum JobType {
    kMockJob,
    kMockFailingJob,
    kMockPendingJob,
    kMockPendingFailingJob,
    kMockWaitingJob,

    // Certificate errors return a socket in addition to an error code.
    kMockCertErrorJob,
    kMockPendingCertErrorJob,

    kMockAdditionalErrorStateJob,
    kMockPendingAdditionalErrorStateJob,
    kMockUnreadDataJob,

    kMockAuthChallengeOnceJob,
    kMockAuthChallengeTwiceJob,
    kMockAuthChallengeOnceFailingJob,
    kMockAuthChallengeTwiceFailingJob,
  };

  // The kMockPendingJob uses a slight delay before allowing the connect
  // to complete.
  static const int kPendingConnectDelay = 2;

  TestConnectJob(JobType job_type,
                 RequestPriority request_priority,
                 SocketTag socket_tag,
                 base::TimeDelta timeout_duration,
                 const CommonConnectJobParams* common_connect_job_params,
                 ConnectJob::Delegate* delegate,
                 MockClientSocketFactory* client_socket_factory)
      : ConnectJob(request_priority,
                   socket_tag,
                   timeout_duration,
                   common_connect_job_params,
                   delegate,
                   nullptr /* net_log */,
                   NetLogSourceType::TRANSPORT_CONNECT_JOB,
                   NetLogEventType::TRANSPORT_CONNECT_JOB_CONNECT),
        job_type_(job_type),
        client_socket_factory_(client_socket_factory),
        load_state_(LOAD_STATE_IDLE),
        has_established_connection_(false),
        store_additional_error_state_(false),
        weak_factory_(this) {}

  void Signal() {
    DoConnect(waiting_success_, true /* async */, false /* recoverable */);
  }

  void set_load_state(LoadState load_state) { load_state_ = load_state; }

  void set_has_established_connection() {
    DCHECK(!has_established_connection_);
    has_established_connection_ = true;
  }

  // From ConnectJob:

  LoadState GetLoadState() const override { return load_state_; }

  bool HasEstablishedConnection() const override {
    return has_established_connection_;
  }

  bool IsSSLError() const override { return store_additional_error_state_; }

  scoped_refptr<SSLCertRequestInfo> GetCertRequestInfo() override {
    if (store_additional_error_state_)
      return base::MakeRefCounted<SSLCertRequestInfo>();
    return nullptr;
  }

 private:
  // From ConnectJob:

  int ConnectInternal() override {
    AddressList ignored;
    client_socket_factory_->CreateTransportClientSocket(
        ignored, nullptr, nullptr, NetLogSource());
    switch (job_type_) {
      case kMockJob:
        return DoConnect(true /* successful */, false /* sync */,
                         false /* cert_error */);
      case kMockFailingJob:
        return DoConnect(false /* error */, false /* sync */,
                         false /* cert_error */);
      case kMockPendingJob:
        set_load_state(LOAD_STATE_CONNECTING);

        // Depending on execution timings, posting a delayed task can result
        // in the task getting executed the at the earliest possible
        // opportunity or only after returning once from the message loop and
        // then a second call into the message loop. In order to make behavior
        // more deterministic, we change the default delay to 2ms. This should
        // always require us to wait for the second call into the message loop.
        //
        // N.B. The correct fix for this and similar timing problems is to
        // abstract time for the purpose of unittests. Unfortunately, we have
        // a lot of third-party components that directly call the various
        // time functions, so this change would be rather invasive.
        base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
            FROM_HERE,
            base::BindOnce(base::IgnoreResult(&TestConnectJob::DoConnect),
                           weak_factory_.GetWeakPtr(), true /* successful */,
                           true /* async */, false /* cert_error */),
            base::TimeDelta::FromMilliseconds(kPendingConnectDelay));
        return ERR_IO_PENDING;
      case kMockPendingFailingJob:
        set_load_state(LOAD_STATE_CONNECTING);
        base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
            FROM_HERE,
            base::BindOnce(base::IgnoreResult(&TestConnectJob::DoConnect),
                           weak_factory_.GetWeakPtr(), false /* error */,
                           true /* async */, false /* cert_error */),
            base::TimeDelta::FromMilliseconds(2));
        return ERR_IO_PENDING;
      case kMockWaitingJob:
        set_load_state(LOAD_STATE_CONNECTING);
        client_socket_factory_->WaitForSignal(this);
        waiting_success_ = true;
        return ERR_IO_PENDING;
      case kMockCertErrorJob:
        return DoConnect(false /* error */, false /* sync */,
                         true /* cert_error */);
      case kMockPendingCertErrorJob:
        set_load_state(LOAD_STATE_CONNECTING);
        base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
            FROM_HERE,
            base::BindOnce(base::IgnoreResult(&TestConnectJob::DoConnect),
                           weak_factory_.GetWeakPtr(), false /* error */,
                           true /* async */, true /* cert_error */),
            base::TimeDelta::FromMilliseconds(2));
        return ERR_IO_PENDING;
      case kMockAdditionalErrorStateJob:
        store_additional_error_state_ = true;
        return DoConnect(false /* error */, false /* sync */,
                         false /* cert_error */);
      case kMockPendingAdditionalErrorStateJob:
        set_load_state(LOAD_STATE_CONNECTING);
        store_additional_error_state_ = true;
        base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
            FROM_HERE,
            base::BindOnce(base::IgnoreResult(&TestConnectJob::DoConnect),
                           weak_factory_.GetWeakPtr(), false /* error */,
                           true /* async */, false /* cert_error */),
            base::TimeDelta::FromMilliseconds(2));
        return ERR_IO_PENDING;
      case kMockUnreadDataJob: {
        int ret = DoConnect(true /* successful */, false /* sync */,
                            false /* cert_error */);
        static_cast<MockClientSocket*>(socket())->set_has_unread_data(true);
        return ret;
      }
      case kMockAuthChallengeOnceJob:
        set_load_state(LOAD_STATE_CONNECTING);
        DoAdvanceAuthChallenge(1, true /* succeed_after_last_challenge */);
        return ERR_IO_PENDING;
      case kMockAuthChallengeTwiceJob:
        set_load_state(LOAD_STATE_CONNECTING);
        DoAdvanceAuthChallenge(2, true /* succeed_after_last_challenge */);
        return ERR_IO_PENDING;
      case kMockAuthChallengeOnceFailingJob:
        set_load_state(LOAD_STATE_CONNECTING);
        DoAdvanceAuthChallenge(1, false /* succeed_after_last_challenge */);
        return ERR_IO_PENDING;
      case kMockAuthChallengeTwiceFailingJob:
        set_load_state(LOAD_STATE_CONNECTING);
        DoAdvanceAuthChallenge(2, false /* succeed_after_last_challenge */);
        return ERR_IO_PENDING;
      default:
        NOTREACHED();
        SetSocket(std::unique_ptr<StreamSocket>());
        return ERR_FAILED;
    }
  }

  void ChangePriorityInternal(RequestPriority priority) override {}

  int DoConnect(bool succeed, bool was_async, bool cert_error) {
    int result = OK;
    has_established_connection_ = true;
    if (succeed) {
      SetSocket(std::make_unique<MockClientSocket>(net_log().net_log()));
      socket()->Connect(CompletionOnceCallback());
    } else if (cert_error) {
      SetSocket(std::make_unique<MockClientSocket>(net_log().net_log()));
      result = ERR_CERT_COMMON_NAME_INVALID;
    } else {
      result = ERR_CONNECTION_FAILED;
      SetSocket(std::unique_ptr<StreamSocket>());
    }

    if (was_async)
      NotifyDelegateOfCompletion(result);
    return result;
  }

  void DoAdvanceAuthChallenge(int remaining_challenges,
                              bool succeed_after_last_challenge) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&TestConnectJob::InvokeNextProxyAuthCallback,
                       weak_factory_.GetWeakPtr(), remaining_challenges,
                       succeed_after_last_challenge));
  }

  void InvokeNextProxyAuthCallback(int remaining_challenges,
                                   bool succeed_after_last_challenge) {
    set_load_state(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL);
    if (remaining_challenges == 0) {
      DoConnect(succeed_after_last_challenge, true /* was_async */,
                false /* cert_error */);
      return;
    }

    // Integration tests make sure HttpResponseInfo and HttpAuthController work.
    // The auth tests here are just focused on ConnectJob bookkeeping.
    HttpResponseInfo info;
    NotifyDelegateOfProxyAuth(
        info, nullptr /* http_auth_controller */,
        base::BindOnce(&TestConnectJob::DoAdvanceAuthChallenge,
                       weak_factory_.GetWeakPtr(), remaining_challenges - 1,
                       succeed_after_last_challenge));
  }

  bool waiting_success_;
  const JobType job_type_;
  MockClientSocketFactory* const client_socket_factory_;
  LoadState load_state_;
  bool has_established_connection_;
  bool store_additional_error_state_;

  base::WeakPtrFactory<TestConnectJob> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestConnectJob);
};

class TestConnectJobFactory
    : public TransportClientSocketPool::ConnectJobFactory {
 public:
  TestConnectJobFactory(MockClientSocketFactory* client_socket_factory,
                        NetLog* net_log)
      : common_connect_job_params_(
            nullptr /* client_socket_factory */,
            nullptr /* host_resolver */,
            nullptr /* http_auth_cache */,
            nullptr /* http_auth_handler_factory */,
            nullptr /* spdy_session_pool */,
            nullptr /* quic_supported_versions */,
            nullptr /* quic_stream_factory */,
            nullptr /* proxy_delegate */,
            nullptr /* http_user_agent_settings */,
            SSLClientSocketContext(),
            SSLClientSocketContext(),
            nullptr /* socket_performance_watcher_factory */,
            nullptr /* network_quality_estimator */,
            net_log,
            nullptr /* websocket_endpoint_lock_manager */),
        job_type_(TestConnectJob::kMockJob),
        job_types_(nullptr),
        client_socket_factory_(client_socket_factory) {}

  ~TestConnectJobFactory() override = default;

  void set_job_type(TestConnectJob::JobType job_type) { job_type_ = job_type; }

  void set_job_types(std::list<TestConnectJob::JobType>* job_types) {
    job_types_ = job_types;
    CHECK(!job_types_->empty());
  }

  void set_timeout_duration(base::TimeDelta timeout_duration) {
    timeout_duration_ = timeout_duration;
  }

  // ConnectJobFactory implementation.

  std::unique_ptr<ConnectJob> NewConnectJob(
      ClientSocketPool::GroupId group_id,
      scoped_refptr<ClientSocketPool::SocketParams> socket_params,
      const base::Optional<NetworkTrafficAnnotationTag>& proxy_annotation_tag,
      RequestPriority request_priority,
      SocketTag socket_tag,
      ConnectJob::Delegate* delegate) const override {
    EXPECT_TRUE(!job_types_ || !job_types_->empty());
    TestConnectJob::JobType job_type = job_type_;
    if (job_types_ && !job_types_->empty()) {
      job_type = job_types_->front();
      job_types_->pop_front();
    }
    return std::make_unique<TestConnectJob>(
        job_type, request_priority, socket_tag, timeout_duration_,
        &common_connect_job_params_, delegate, client_socket_factory_);
  }

 private:
  const CommonConnectJobParams common_connect_job_params_;
  TestConnectJob::JobType job_type_;
  std::list<TestConnectJob::JobType>* job_types_;
  base::TimeDelta timeout_duration_;
  MockClientSocketFactory* const client_socket_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestConnectJobFactory);
};

}  // namespace

namespace {

void MockClientSocketFactory::SignalJobs() {
  for (auto it = waiting_jobs_.begin(); it != waiting_jobs_.end(); ++it) {
    (*it)->Signal();
  }
  waiting_jobs_.clear();
}

void MockClientSocketFactory::SignalJob(size_t job) {
  ASSERT_LT(job, waiting_jobs_.size());
  waiting_jobs_[job]->Signal();
  waiting_jobs_.erase(waiting_jobs_.begin() + job);
}

void MockClientSocketFactory::SetJobLoadState(size_t job,
                                              LoadState load_state) {
  ASSERT_LT(job, waiting_jobs_.size());
  waiting_jobs_[job]->set_load_state(load_state);
}

void MockClientSocketFactory::SetJobHasEstablishedConnection(size_t job) {
  ASSERT_LT(job, waiting_jobs_.size());
  waiting_jobs_[job]->set_has_established_connection();
}

class ClientSocketPoolBaseTest : public TestWithScopedTaskEnvironment {
 protected:
  ClientSocketPoolBaseTest()
      : TestWithScopedTaskEnvironment(
            base::test::ScopedTaskEnvironment::MainThreadType::MOCK_TIME),
        params_(ClientSocketPool::SocketParams::CreateForHttpForTesting()) {
    connect_backup_jobs_enabled_ =
        TransportClientSocketPool::connect_backup_jobs_enabled();
    TransportClientSocketPool::set_connect_backup_jobs_enabled(true);
  }

  ~ClientSocketPoolBaseTest() override {
    TransportClientSocketPool::set_connect_backup_jobs_enabled(
        connect_backup_jobs_enabled_);
  }

  void CreatePool(int max_sockets,
                  int max_sockets_per_group,
                  bool enable_backup_connect_jobs = false) {
    CreatePoolWithIdleTimeouts(max_sockets, max_sockets_per_group,
                               kUnusedIdleSocketTimeout,
                               ClientSocketPool::used_idle_socket_timeout(),
                               enable_backup_connect_jobs);
  }

  void CreatePoolWithIdleTimeouts(int max_sockets,
                                  int max_sockets_per_group,
                                  base::TimeDelta unused_idle_socket_timeout,
                                  base::TimeDelta used_idle_socket_timeout,
                                  bool enable_backup_connect_jobs = false) {
    DCHECK(!pool_.get());
    std::unique_ptr<TestConnectJobFactory> connect_job_factory =
        std::make_unique<TestConnectJobFactory>(&client_socket_factory_,
                                                &net_log_);
    connect_job_factory_ = connect_job_factory.get();
    pool_ = TransportClientSocketPool::CreateForTesting(
        max_sockets, max_sockets_per_group, unused_idle_socket_timeout,
        used_idle_socket_timeout, std::move(connect_job_factory),
        nullptr /* ssl_config_service */, enable_backup_connect_jobs);
  }

  int StartRequestWithIgnoreLimits(
      const ClientSocketPool::GroupId& group_id,
      RequestPriority priority,
      ClientSocketPool::RespectLimits respect_limits) {
    return test_base_.StartRequestUsingPool(pool_.get(), group_id, priority,
                                            respect_limits, params_);
  }

  int StartRequest(const ClientSocketPool::GroupId& group_id,
                   RequestPriority priority) {
    return StartRequestWithIgnoreLimits(
        group_id, priority, ClientSocketPool::RespectLimits::ENABLED);
  }

  int GetOrderOfRequest(size_t index) const {
    return test_base_.GetOrderOfRequest(index);
  }

  bool ReleaseOneConnection(ClientSocketPoolTest::KeepAlive keep_alive) {
    return test_base_.ReleaseOneConnection(keep_alive);
  }

  void ReleaseAllConnections(ClientSocketPoolTest::KeepAlive keep_alive) {
    test_base_.ReleaseAllConnections(keep_alive);
  }

  TestSocketRequest* request(int i) { return test_base_.request(i); }
  size_t requests_size() const { return test_base_.requests_size(); }
  std::vector<std::unique_ptr<TestSocketRequest>>* requests() {
    return test_base_.requests();
  }
  // Only counts the requests that get sockets asynchronously;
  // synchronous completions are not registered by this count.
  size_t completion_count() const { return test_base_.completion_count(); }

  TestNetLog net_log_;
  bool connect_backup_jobs_enabled_;
  MockClientSocketFactory client_socket_factory_;
  TestConnectJobFactory* connect_job_factory_;
  // These parameters are never actually used to create a TransportConnectJob.
  scoped_refptr<ClientSocketPool::SocketParams> params_;
  std::unique_ptr<TransportClientSocketPool> pool_;
  ClientSocketPoolTest test_base_;
};

TEST_F(ClientSocketPoolBaseTest, BasicSynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  TestCompletionCallback callback;
  ClientSocketHandle handle;
  BoundTestNetLog log;
  TestLoadTimingInfoNotConnected(handle);

  EXPECT_EQ(OK, handle.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), log.bound()));
  EXPECT_TRUE(handle.is_initialized());
  EXPECT_TRUE(handle.socket());
  TestLoadTimingInfoConnectedNotReused(handle);

  handle.Reset();
  TestLoadTimingInfoNotConnected(handle);

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);

  EXPECT_EQ(5u, entries.size());
  EXPECT_TRUE(LogContainsEvent(
      entries, 0, NetLogEventType::TCP_CLIENT_SOCKET_POOL_REQUESTED_SOCKET,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsBeginEvent(entries, 1, NetLogEventType::SOCKET_POOL));
  EXPECT_TRUE(LogContainsEvent(
      entries, 2, NetLogEventType::SOCKET_POOL_BOUND_TO_CONNECT_JOB,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEvent(entries, 3,
                               NetLogEventType::SOCKET_POOL_BOUND_TO_SOCKET,
                               NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEndEvent(entries, 4, NetLogEventType::SOCKET_POOL));
}

TEST_F(ClientSocketPoolBaseTest, InitConnectionFailure) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockFailingJob);
  BoundTestNetLog log;

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  // Set the additional error state members to ensure that they get cleared.
  handle.set_is_ssl_error(true);
  handle.set_ssl_cert_request_info(base::MakeRefCounted<SSLCertRequestInfo>());
  EXPECT_EQ(
      ERR_CONNECTION_FAILED,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), log.bound()));
  EXPECT_FALSE(handle.socket());
  EXPECT_FALSE(handle.is_ssl_error());
  EXPECT_FALSE(handle.ssl_cert_request_info());
  TestLoadTimingInfoNotConnected(handle);

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);

  EXPECT_EQ(4u, entries.size());
  EXPECT_TRUE(LogContainsEvent(
      entries, 0, NetLogEventType::TCP_CLIENT_SOCKET_POOL_REQUESTED_SOCKET,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsBeginEvent(entries, 1, NetLogEventType::SOCKET_POOL));
  EXPECT_TRUE(LogContainsEvent(
      entries, 2, NetLogEventType::SOCKET_POOL_BOUND_TO_CONNECT_JOB,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEndEvent(entries, 3, NetLogEventType::SOCKET_POOL));
}

// Make sure different groups do not share sockets.
TEST_F(ClientSocketPoolBaseTest, GroupSeparation) {
  CreatePool(1000 /* max_sockets */, 2 /* max_sockets_per_group */);

  const HostPortPair kHostPortPairs[] = {
      {"a", 80},
      {"a", 443},
      {"b", 80},
  };

  const ClientSocketPool::SocketType kSocketTypes[] = {
      ClientSocketPool::SocketType::kHttp,
      ClientSocketPool::SocketType::kSsl,
      ClientSocketPool::SocketType::kFtp,
  };

  const PrivacyMode kPrivacyModes[] = {PrivacyMode::PRIVACY_MODE_DISABLED,
                                       PrivacyMode::PRIVACY_MODE_ENABLED};

  int total_idle_sockets = 0;

  // Walk through each GroupId, making sure that requesting a socket for one
  // group does not return a previously connected socket for another group.
  for (const auto& host_port_pair : kHostPortPairs) {
    SCOPED_TRACE(host_port_pair.ToString());
    for (const auto& socket_type : kSocketTypes) {
      SCOPED_TRACE(static_cast<int>(socket_type));
      for (const auto& privacy_mode : kPrivacyModes) {
        SCOPED_TRACE(privacy_mode);

        connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

        ClientSocketPool::GroupId group_id(host_port_pair, socket_type,
                                           privacy_mode);

        EXPECT_FALSE(pool_->HasGroupForTesting(group_id));

        TestCompletionCallback callback;
        ClientSocketHandle handle;

        // Since the group is empty, requesting a socket should not complete
        // synchronously.
        EXPECT_THAT(
            handle.Init(group_id, params_, base::nullopt, DEFAULT_PRIORITY,
                        SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                        callback.callback(),
                        ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                        NetLogWithSource()),
            IsError(ERR_IO_PENDING));
        EXPECT_TRUE(pool_->HasGroupForTesting(group_id));
        EXPECT_EQ(total_idle_sockets, pool_->IdleSocketCount());

        EXPECT_THAT(callback.WaitForResult(), IsOk());
        EXPECT_TRUE(handle.socket());
        EXPECT_TRUE(pool_->HasGroupForTesting(group_id));
        EXPECT_EQ(total_idle_sockets, pool_->IdleSocketCount());

        // Return socket to pool.
        handle.Reset();
        EXPECT_EQ(total_idle_sockets + 1, pool_->IdleSocketCount());

        // Requesting a socket again should return the same socket as before, so
        // should complete synchronously.
        EXPECT_THAT(
            handle.Init(group_id, params_, base::nullopt, DEFAULT_PRIORITY,
                        SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                        callback.callback(),
                        ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                        NetLogWithSource()),
            IsOk());
        EXPECT_TRUE(handle.socket());
        EXPECT_EQ(total_idle_sockets, pool_->IdleSocketCount());

        // Return socket to pool again.
        handle.Reset();
        EXPECT_EQ(total_idle_sockets + 1, pool_->IdleSocketCount());

        ++total_idle_sockets;
      }
    }
  }
}

TEST_F(ClientSocketPoolBaseTest, TotalLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // TODO(eroman): Check that the NetLog contains this event.

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("c"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("d"), DEFAULT_PRIORITY), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  EXPECT_THAT(StartRequest(TestGroupId("e"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("f"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("g"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));
  EXPECT_EQ(5, GetOrderOfRequest(5));
  EXPECT_EQ(6, GetOrderOfRequest(6));
  EXPECT_EQ(7, GetOrderOfRequest(7));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(8));
}

TEST_F(ClientSocketPoolBaseTest, TotalLimitReachedNewGroup) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // TODO(eroman): Check that the NetLog contains this event.

  // Reach all limits: max total sockets, and max sockets per group.
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  // Now create a new group and verify that we don't starve it.
  EXPECT_THAT(StartRequest(TestGroupId("c"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));
  EXPECT_EQ(5, GetOrderOfRequest(5));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(6));
}

TEST_F(ClientSocketPoolBaseTest, TotalLimitRespectsPriority) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("b"), LOWEST), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), HIGHEST), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());

  EXPECT_THAT(StartRequest(TestGroupId("c"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("b"), HIGHEST), IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  // First 4 requests don't have to wait, and finish in order.
  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));

  // Request ("b", HIGHEST) has the highest priority, then (TestGroupId("a"),
  // MEDIUM), and then ("c", LOWEST).
  EXPECT_EQ(7, GetOrderOfRequest(5));
  EXPECT_EQ(6, GetOrderOfRequest(6));
  EXPECT_EQ(5, GetOrderOfRequest(7));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(9));
}

// Test reprioritizing a request before completion doesn't interfere with
// its completion.
TEST_F(ClientSocketPoolBaseTest, ReprioritizeOne) {
  CreatePool(kDefaultMaxSockets, 1);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(OK));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request(0)->handle()->socket());
  EXPECT_FALSE(request(1)->handle()->socket());

  request(1)->handle()->SetPriority(HIGHEST);

  ReleaseOneConnection(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_TRUE(request(1)->handle()->socket());
}

// Reprioritize a request up past another one and make sure that changes the
// completion order.
TEST_F(ClientSocketPoolBaseTest, ReprioritizeUpReorder) {
  CreatePool(kDefaultMaxSockets, 1);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(OK));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request(0)->handle()->socket());
  EXPECT_FALSE(request(1)->handle()->socket());
  EXPECT_FALSE(request(2)->handle()->socket());

  request(2)->handle()->SetPriority(HIGHEST);

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(3, GetOrderOfRequest(2));
  EXPECT_EQ(2, GetOrderOfRequest(3));
}

// Reprioritize a request without changing relative priorities and check
// that the order doesn't change.
TEST_F(ClientSocketPoolBaseTest, ReprioritizeUpNoReorder) {
  CreatePool(kDefaultMaxSockets, 1);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(OK));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request(0)->handle()->socket());
  EXPECT_FALSE(request(1)->handle()->socket());
  EXPECT_FALSE(request(2)->handle()->socket());

  request(2)->handle()->SetPriority(MEDIUM);

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
}

// Reprioritize a request past down another one and make sure that changes the
// completion order.
TEST_F(ClientSocketPoolBaseTest, ReprioritizeDownReorder) {
  CreatePool(kDefaultMaxSockets, 1);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(OK));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request(0)->handle()->socket());
  EXPECT_FALSE(request(1)->handle()->socket());
  EXPECT_FALSE(request(2)->handle()->socket());

  request(1)->handle()->SetPriority(LOW);

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(3, GetOrderOfRequest(2));
  EXPECT_EQ(2, GetOrderOfRequest(3));
}

// Reprioritize a request to the same level as another and confirm it is
// put after the old request.
TEST_F(ClientSocketPoolBaseTest, ReprioritizeResetFIFO) {
  CreatePool(kDefaultMaxSockets, 1);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(OK));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_TRUE(request(0)->handle()->socket());
  EXPECT_FALSE(request(1)->handle()->socket());
  EXPECT_FALSE(request(2)->handle()->socket());

  request(1)->handle()->SetPriority(MEDIUM);

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(3, GetOrderOfRequest(2));
  EXPECT_EQ(2, GetOrderOfRequest(3));
}

TEST_F(ClientSocketPoolBaseTest, TotalLimitRespectsGroupLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), HIGHEST), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), MEDIUM), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());

  EXPECT_THAT(StartRequest(TestGroupId("c"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("b"), HIGHEST), IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSockets, completion_count());

  // First 4 requests don't have to wait, and finish in order.
  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));

  // Request ("b", 7) has the highest priority, but we can't make new socket for
  // group "b", because it has reached the per-group limit. Then we make
  // socket for ("c", 6), because it has higher priority than ("a", 4),
  // and we still can't make a socket for group "b".
  EXPECT_EQ(5, GetOrderOfRequest(5));
  EXPECT_EQ(6, GetOrderOfRequest(6));
  EXPECT_EQ(7, GetOrderOfRequest(7));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(8));
}

// Make sure that we count connecting sockets against the total limit.
TEST_F(ClientSocketPoolBaseTest, TotalLimitCountsConnectingSockets) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("c"), DEFAULT_PRIORITY), IsOk());

  // Create one asynchronous request.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  EXPECT_THAT(StartRequest(TestGroupId("d"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  // We post all of our delayed tasks with a 2ms delay. I.e. they don't
  // actually become pending until 2ms after they have been created. In order
  // to flush all tasks, we need to wait so that we know there are no
  // soon-to-be-pending tasks waiting.
  FastForwardBy(base::TimeDelta::FromMilliseconds(10));

  // The next synchronous request should wait for its turn.
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);
  EXPECT_THAT(StartRequest(TestGroupId("e"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));
  EXPECT_EQ(5, GetOrderOfRequest(5));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(6));
}

TEST_F(ClientSocketPoolBaseTest, CorrectlyCountStalledGroups) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());

  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());

  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("c"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());

  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));
  EXPECT_EQ(kDefaultMaxSockets + 1, client_socket_factory_.allocation_count());
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));
  EXPECT_EQ(kDefaultMaxSockets + 2, client_socket_factory_.allocation_count());
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));
  EXPECT_EQ(kDefaultMaxSockets + 2, client_socket_factory_.allocation_count());
}

TEST_F(ClientSocketPoolBaseTest, StallAndThenCancelAndTriggerAvailableSocket) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  ClientSocketHandle handles[4];
  for (size_t i = 0; i < base::size(handles); ++i) {
    TestCompletionCallback callback;
    EXPECT_EQ(ERR_IO_PENDING,
              handles[i].Init(
                  TestGroupId("b"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  }

  // One will be stalled, cancel all the handles now.
  // This should hit the OnAvailableSocketSlot() code where we previously had
  // stalled groups, but no longer have any.
  for (size_t i = 0; i < base::size(handles); ++i)
    handles[i].Reset();
}

TEST_F(ClientSocketPoolBaseTest, CancelStalledSocketAtSocketLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  {
    ClientSocketHandle handles[kDefaultMaxSockets];
    TestCompletionCallback callbacks[kDefaultMaxSockets];
    for (int i = 0; i < kDefaultMaxSockets; ++i) {
      EXPECT_EQ(OK,
                handles[i].Init(TestGroupId(base::NumberToString(i)), params_,
                                base::nullopt, DEFAULT_PRIORITY, SocketTag(),
                                ClientSocketPool::RespectLimits::ENABLED,
                                callbacks[i].callback(),
                                ClientSocketPool::ProxyAuthCallback(),
                                pool_.get(), NetLogWithSource()));
    }

    // Force a stalled group.
    ClientSocketHandle stalled_handle;
    TestCompletionCallback callback;
    EXPECT_EQ(ERR_IO_PENDING,
              stalled_handle.Init(
                  TestGroupId("foo"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

    // Cancel the stalled request.
    stalled_handle.Reset();

    EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());
    EXPECT_EQ(0, pool_->IdleSocketCount());

    // Dropping out of scope will close all handles and return them to idle.
  }

  EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());
  EXPECT_EQ(kDefaultMaxSockets, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, CancelPendingSocketAtSocketLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  {
    ClientSocketHandle handles[kDefaultMaxSockets];
    for (int i = 0; i < kDefaultMaxSockets; ++i) {
      TestCompletionCallback callback;
      EXPECT_EQ(ERR_IO_PENDING,
                handles[i].Init(TestGroupId(base::NumberToString(i)), params_,
                                base::nullopt, DEFAULT_PRIORITY, SocketTag(),
                                ClientSocketPool::RespectLimits::ENABLED,
                                callback.callback(),
                                ClientSocketPool::ProxyAuthCallback(),
                                pool_.get(), NetLogWithSource()));
    }

    // Force a stalled group.
    connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
    ClientSocketHandle stalled_handle;
    TestCompletionCallback callback;
    EXPECT_EQ(ERR_IO_PENDING,
              stalled_handle.Init(
                  TestGroupId("foo"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

    // Since it is stalled, it should have no connect jobs.
    EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumUnassignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));

    // Cancel the stalled request.
    handles[0].Reset();

    // Now we should have a connect job.
    EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumUnassignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));

    // The stalled socket should connect.
    EXPECT_THAT(callback.WaitForResult(), IsOk());

    EXPECT_EQ(kDefaultMaxSockets + 1,
              client_socket_factory_.allocation_count());
    EXPECT_EQ(0, pool_->IdleSocketCount());
    EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));
    EXPECT_EQ(0u, pool_->NumUnassignedConnectJobsInGroupForTesting(
                      TestGroupId("foo")));

    // Dropping out of scope will close all handles and return them to idle.
  }

  EXPECT_EQ(1, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, WaitForStalledSocketAtSocketLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  ClientSocketHandle stalled_handle;
  TestCompletionCallback callback;
  {
    EXPECT_FALSE(pool_->IsStalled());
    ClientSocketHandle handles[kDefaultMaxSockets];
    for (int i = 0; i < kDefaultMaxSockets; ++i) {
      TestCompletionCallback callback;
      EXPECT_EQ(
          OK, handles[i].Init(
                  TestGroupId(base::StringPrintf("Take 2: %d", i)), params_,
                  base::nullopt, DEFAULT_PRIORITY, SocketTag(),
                  ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
                  ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                  NetLogWithSource()));
    }

    EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());
    EXPECT_EQ(0, pool_->IdleSocketCount());
    EXPECT_FALSE(pool_->IsStalled());

    // Now we will hit the socket limit.
    EXPECT_EQ(ERR_IO_PENDING,
              stalled_handle.Init(
                  TestGroupId("foo"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
    EXPECT_TRUE(pool_->IsStalled());

    // Dropping out of scope will close all handles and return them to idle.
  }

  // But if we wait for it, the released idle sockets will be closed in
  // preference of the waiting request.
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  EXPECT_EQ(kDefaultMaxSockets + 1, client_socket_factory_.allocation_count());
  EXPECT_EQ(3, pool_->IdleSocketCount());
}

// Regression test for http://crbug.com/40952.
TEST_F(ClientSocketPoolBaseTest, CloseIdleSocketAtSocketLimitDeleteGroup) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
             true /* enable_backup_connect_jobs */);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  for (int i = 0; i < kDefaultMaxSockets; ++i) {
    ClientSocketHandle handle;
    TestCompletionCallback callback;
    EXPECT_EQ(OK, handle.Init(TestGroupId(base::NumberToString(i)), params_,
                              base::nullopt, DEFAULT_PRIORITY, SocketTag(),
                              ClientSocketPool::RespectLimits::ENABLED,
                              callback.callback(),
                              ClientSocketPool::ProxyAuthCallback(),
                              pool_.get(), NetLogWithSource()));
  }

  // Flush all the DoReleaseSocket tasks.
  base::RunLoop().RunUntilIdle();

  // Stall a group.  Set a pending job so it'll trigger a backup job if we don't
  // reuse a socket.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;

  // "0" is special here, since it should be the first entry in the sorted map,
  // which is the one which we would close an idle socket for.  We shouldn't
  // close an idle socket though, since we should reuse the idle socket.
  EXPECT_EQ(OK, handle.Init(
                    TestGroupId("0"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));

  EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());
  EXPECT_EQ(kDefaultMaxSockets - 1, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, PendingRequests) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), IDLE), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::KEEP_ALIVE);
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSocketsPerGroup,
            completion_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(8, GetOrderOfRequest(3));
  EXPECT_EQ(6, GetOrderOfRequest(4));
  EXPECT_EQ(4, GetOrderOfRequest(5));
  EXPECT_EQ(3, GetOrderOfRequest(6));
  EXPECT_EQ(5, GetOrderOfRequest(7));
  EXPECT_EQ(7, GetOrderOfRequest(8));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(9));
}

TEST_F(ClientSocketPoolBaseTest, PendingRequests_NoKeepAlive) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));

  ReleaseAllConnections(ClientSocketPoolTest::NO_KEEP_ALIVE);

  for (size_t i = kDefaultMaxSocketsPerGroup; i < requests_size(); ++i)
    EXPECT_THAT(request(i)->WaitForResult(), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSocketsPerGroup,
            completion_count());
}

TEST_F(ClientSocketPoolBaseTest, ResetAndCloseSocket) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  EXPECT_THAT(callback.WaitForResult(), IsOk());
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  handle.ResetAndCloseSocket();
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
}

// This test will start up a socket request and then call Reset() on the handle.
// The pending ConnectJob should not be destroyed.
TEST_F(ClientSocketPoolBaseTest, CancelRequestKeepsConnectJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  handle.Reset();
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
}

// This test will start up a socket request and then call ResetAndCloseSocket()
// on the handle. The pending ConnectJob or connected socket should be
// destroyed.
TEST_F(ClientSocketPoolBaseTest, CancelRequestAndCloseSocket) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // When true, the socket connects before it's canceled.
  for (bool cancel_when_callback_pending : {false, true}) {
    if (cancel_when_callback_pending) {
      connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
    } else {
      connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
    }
    ClientSocketHandle handle;
    TestCompletionCallback callback;
    EXPECT_EQ(
        ERR_IO_PENDING,
        handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));
    ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
    EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

    if (cancel_when_callback_pending) {
      client_socket_factory_.SignalJobs();
      ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
      EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
    }

    handle.ResetAndCloseSocket();
    ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  }
}

TEST_F(ClientSocketPoolBaseTest,
       CancelRequestAndCloseSocketWhenMoreRequestsThanConnectJobs) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // When true, the sockets connect before they're canceled.
  for (bool cancel_when_callback_pending : {false, true}) {
    if (cancel_when_callback_pending) {
      connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
    } else {
      connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
    }

    std::vector<std::unique_ptr<ClientSocketHandle>> handles;
    TestCompletionCallback callback;
    // Make |kDefaultMaxSockets + 1| socket requests.
    for (int i = 0; i < kDefaultMaxSocketsPerGroup + 1; ++i) {
      std::unique_ptr<ClientSocketHandle> handle =
          std::make_unique<ClientSocketHandle>();
      EXPECT_EQ(ERR_IO_PENDING,
                handle->Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));
      handles.push_back(std::move(handle));
      ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
      EXPECT_EQ(
          static_cast<size_t>(std::min(i + 1, kDefaultMaxSocketsPerGroup)),
          pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
    }

    if (cancel_when_callback_pending) {
      client_socket_factory_.SignalJobs();
      ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
      EXPECT_EQ(kDefaultMaxSocketsPerGroup,
                pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
    }

    // Calling ResetAndCloseSocket() on a handle should not cancel a ConnectJob
    // or close a socket, since there are more requests than ConnectJobs or
    // sockets.
    handles[kDefaultMaxSocketsPerGroup]->ResetAndCloseSocket();
    ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
    if (cancel_when_callback_pending) {
      EXPECT_EQ(kDefaultMaxSocketsPerGroup,
                pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
    } else {
      EXPECT_EQ(static_cast<size_t>(kDefaultMaxSocketsPerGroup),
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
    }

    // Calling ResetAndCloseSocket() on other handles should cancel a ConnectJob
    // or close a socket.
    for (int i = kDefaultMaxSocketsPerGroup - 1; i >= 0; --i) {
      handles[i]->ResetAndCloseSocket();
      if (i > 0) {
        ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
        if (cancel_when_callback_pending) {
          EXPECT_EQ(i,
                    pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
        } else {
          EXPECT_EQ(static_cast<size_t>(i),
                    pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
        }
      } else {
        EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
      }
    }
  }
}

TEST_F(ClientSocketPoolBaseTest, ConnectCancelConnect) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;

  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  handle.Reset();
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // This will create a second ConnectJob, since the other ConnectJob was
  // previously assigned to a request.
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  EXPECT_FALSE(callback.have_result());
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  // One ConnectJob completed, and its socket is now assigned to |handle|.
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  // The other ConnectJob should have either completed, or still be connecting.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")) +
                    pool_->IdleSocketCountInGroup(TestGroupId("a")));

  handle.Reset();
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")) +
                    pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, CancelRequest) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));

  // Cancel a request.
  size_t index_to_cancel = kDefaultMaxSocketsPerGroup + 2;
  EXPECT_FALSE((*requests())[index_to_cancel]->handle()->is_initialized());
  (*requests())[index_to_cancel]->handle()->Reset();

  ReleaseAllConnections(ClientSocketPoolTest::KEEP_ALIVE);

  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            client_socket_factory_.allocation_count());
  EXPECT_EQ(requests_size() - kDefaultMaxSocketsPerGroup - 1,
            completion_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(5, GetOrderOfRequest(3));
  EXPECT_EQ(3, GetOrderOfRequest(4));
  EXPECT_EQ(ClientSocketPoolTest::kRequestNotFound,
            GetOrderOfRequest(5));  // Canceled request.
  EXPECT_EQ(4, GetOrderOfRequest(6));
  EXPECT_EQ(6, GetOrderOfRequest(7));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(8));
}

// Function to be used as a callback on socket request completion.  It first
// disconnects the successfully connected socket from the first request, and
// then reuses the ClientSocketHandle to request another socket.
//
// |nested_callback| is called with the result of the second socket request.
void RequestSocketOnComplete(ClientSocketHandle* handle,
                             TransportClientSocketPool* pool,
                             TestConnectJobFactory* test_connect_job_factory,
                             TestConnectJob::JobType next_job_type,
                             TestCompletionCallback* nested_callback,
                             int first_request_result) {
  EXPECT_THAT(first_request_result, IsOk());

  test_connect_job_factory->set_job_type(next_job_type);

  // Don't allow reuse of the socket.  Disconnect it and then release it.
  if (handle->socket())
    handle->socket()->Disconnect();
  handle->Reset();

  TestCompletionCallback callback;
  int rv = handle->Init(
      TestGroupId("a"),
      ClientSocketPool::SocketParams::CreateForHttpForTesting(), base::nullopt,
      LOWEST, SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
      nested_callback->callback(), ClientSocketPool::ProxyAuthCallback(), pool,
      NetLogWithSource());
  if (rv != ERR_IO_PENDING) {
    DCHECK_EQ(TestConnectJob::kMockJob, next_job_type);
    nested_callback->callback().Run(rv);
  } else {
    DCHECK_EQ(TestConnectJob::kMockPendingJob, next_job_type);
  }
}

// Tests the case where a second socket is requested in a completion callback,
// and the second socket connects asynchronously.  Reuses the same
// ClientSocketHandle for the second socket, after disconnecting the first.
TEST_F(ClientSocketPoolBaseTest, RequestPendingJobTwice) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback second_result_callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED,
      base::BindOnce(&RequestSocketOnComplete, &handle, pool_.get(),
                     connect_job_factory_, TestConnectJob::kMockPendingJob,
                     &second_result_callback),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));

  EXPECT_THAT(second_result_callback.WaitForResult(), IsOk());
}

// Tests the case where a second socket is requested in a completion callback,
// and the second socket connects synchronously.  Reuses the same
// ClientSocketHandle for the second socket, after disconnecting the first.
TEST_F(ClientSocketPoolBaseTest, RequestPendingJobThenSynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback second_result_callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED,
      base::BindOnce(&RequestSocketOnComplete, &handle, pool_.get(),
                     connect_job_factory_, TestConnectJob::kMockPendingJob,
                     &second_result_callback),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));

  EXPECT_THAT(second_result_callback.WaitForResult(), IsOk());
}

// Make sure that pending requests get serviced after active requests get
// cancelled.
TEST_F(ClientSocketPoolBaseTest, CancelActiveRequestWithPendingRequests) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  // Now, kDefaultMaxSocketsPerGroup requests should be active.
  // Let's cancel them.
  for (int i = 0; i < kDefaultMaxSocketsPerGroup; ++i) {
    ASSERT_FALSE(request(i)->handle()->is_initialized());
    request(i)->handle()->Reset();
  }

  // Let's wait for the rest to complete now.
  for (size_t i = kDefaultMaxSocketsPerGroup; i < requests_size(); ++i) {
    EXPECT_THAT(request(i)->WaitForResult(), IsOk());
    request(i)->handle()->Reset();
  }

  EXPECT_EQ(requests_size() - kDefaultMaxSocketsPerGroup,
            completion_count());
}

// Make sure that pending requests get serviced after active requests fail.
TEST_F(ClientSocketPoolBaseTest, FailingActiveRequestWithPendingRequests) {
  const size_t kMaxSockets = 5;
  CreatePool(kMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  const size_t kNumberOfRequests = 2 * kDefaultMaxSocketsPerGroup + 1;
  ASSERT_LE(kNumberOfRequests, kMaxSockets);  // Otherwise the test will hang.

  // Queue up all the requests
  for (size_t i = 0; i < kNumberOfRequests; ++i)
    EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
                IsError(ERR_IO_PENDING));

  for (size_t i = 0; i < kNumberOfRequests; ++i)
    EXPECT_THAT(request(i)->WaitForResult(), IsError(ERR_CONNECTION_FAILED));
}

// Make sure that pending requests that complete synchronously get serviced
// after active requests fail. See https://crbug.com/723748
TEST_F(ClientSocketPoolBaseTest, HandleMultipleSyncFailuresAfterAsyncFailure) {
  const size_t kNumberOfRequests = 10;
  const size_t kMaxSockets = 1;
  CreatePool(kMaxSockets, kMaxSockets);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  connect_job_factory_->set_job_type(TestConnectJob::kMockFailingJob);

  // Queue up all the other requests
  for (size_t i = 1; i < kNumberOfRequests; ++i)
    EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
                IsError(ERR_IO_PENDING));

  // Make sure all requests fail, instead of hanging.
  for (size_t i = 0; i < kNumberOfRequests; ++i)
    EXPECT_THAT(request(i)->WaitForResult(), IsError(ERR_CONNECTION_FAILED));
}

TEST_F(ClientSocketPoolBaseTest, CancelActiveRequestThenRequestSocket) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Cancel the active request.
  handle.Reset();

  rv = handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  EXPECT_FALSE(handle.is_reused());
  TestLoadTimingInfoConnectedNotReused(handle);
  EXPECT_EQ(2, client_socket_factory_.allocation_count());
}

TEST_F(ClientSocketPoolBaseTest, CloseIdleSocketsForced) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  BoundTestNetLog log;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), log.bound());
  EXPECT_THAT(rv, IsOk());
  handle.Reset();
  EXPECT_EQ(1, pool_->IdleSocketCount());
  pool_->CloseIdleSockets();
}

TEST_F(ClientSocketPoolBaseTest, CloseIdleSocketsInGroupForced) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  TestCompletionCallback callback;
  BoundTestNetLog log;
  ClientSocketHandle handle1;
  int rv = handle1.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), log.bound());
  EXPECT_THAT(rv, IsOk());
  ClientSocketHandle handle2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), log.bound());
  ClientSocketHandle handle3;
  rv = handle3.Init(TestGroupId("b"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), log.bound());
  EXPECT_THAT(rv, IsOk());
  handle1.Reset();
  handle2.Reset();
  handle3.Reset();
  EXPECT_EQ(3, pool_->IdleSocketCount());
  pool_->CloseIdleSocketsInGroup(TestGroupId("a"));
  EXPECT_EQ(1, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, CleanUpUnusableIdleSockets) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  BoundTestNetLog log;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), log.bound());
  EXPECT_THAT(rv, IsOk());
  StreamSocket* socket = handle.socket();
  handle.Reset();
  EXPECT_EQ(1, pool_->IdleSocketCount());

  // Disconnect socket now to make the socket unusable.
  socket->Disconnect();
  ClientSocketHandle handle2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), log.bound());
  EXPECT_THAT(rv, IsOk());
  EXPECT_FALSE(handle2.is_reused());
}

// Regression test for http://crbug.com/17985.
TEST_F(ClientSocketPoolBaseTest, GroupWithPendingRequestsIsNotEmpty) {
  const int kMaxSockets = 3;
  const int kMaxSocketsPerGroup = 2;
  CreatePool(kMaxSockets, kMaxSocketsPerGroup);

  const RequestPriority kHighPriority = HIGHEST;

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());

  // This is going to be a pending request in an otherwise empty group.
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  // Reach the maximum socket limit.
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());

  // Create a stalled group with high priorities.
  EXPECT_THAT(StartRequest(TestGroupId("c"), kHighPriority),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("c"), kHighPriority),
              IsError(ERR_IO_PENDING));

  // Release the first two sockets from TestGroupId("a").  Because this is a
  // keepalive, the first release will unblock the pending request for
  // TestGroupId("a").  The second release will unblock a request for "c",
  // because it is the next high priority socket.
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::KEEP_ALIVE));

  // Closing idle sockets should not get us into trouble, but in the bug
  // we were hitting a CHECK here.
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  pool_->CloseIdleSockets();

  // Run the released socket wakeups.
  base::RunLoop().RunUntilIdle();
}

TEST_F(ClientSocketPoolBaseTest, BasicAsynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  BoundTestNetLog log;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), log.bound());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));
  TestLoadTimingInfoNotConnected(handle);

  EXPECT_THAT(callback.WaitForResult(), IsOk());
  EXPECT_TRUE(handle.is_initialized());
  EXPECT_TRUE(handle.socket());
  TestLoadTimingInfoConnectedNotReused(handle);

  handle.Reset();
  TestLoadTimingInfoNotConnected(handle);

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);

  EXPECT_EQ(5u, entries.size());
  EXPECT_TRUE(LogContainsEvent(
      entries, 0, NetLogEventType::TCP_CLIENT_SOCKET_POOL_REQUESTED_SOCKET,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsBeginEvent(entries, 1, NetLogEventType::SOCKET_POOL));
  EXPECT_TRUE(LogContainsEvent(
      entries, 2, NetLogEventType::SOCKET_POOL_BOUND_TO_CONNECT_JOB,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEvent(entries, 3,
                               NetLogEventType::SOCKET_POOL_BOUND_TO_SOCKET,
                               NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEndEvent(entries, 4, NetLogEventType::SOCKET_POOL));
}

TEST_F(ClientSocketPoolBaseTest,
       InitConnectionAsynchronousFailure) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  BoundTestNetLog log;
  // Set the additional error state members to ensure that they get cleared.
  handle.set_is_ssl_error(true);
  handle.set_ssl_cert_request_info(base::MakeRefCounted<SSLCertRequestInfo>());
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), log.bound()));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));
  EXPECT_THAT(callback.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_FALSE(handle.is_ssl_error());
  EXPECT_FALSE(handle.ssl_cert_request_info());

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);

  EXPECT_EQ(4u, entries.size());
  EXPECT_TRUE(LogContainsEvent(
      entries, 0, NetLogEventType::TCP_CLIENT_SOCKET_POOL_REQUESTED_SOCKET,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsBeginEvent(entries, 1, NetLogEventType::SOCKET_POOL));
  EXPECT_TRUE(LogContainsEvent(
      entries, 2, NetLogEventType::SOCKET_POOL_BOUND_TO_CONNECT_JOB,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsEndEvent(entries, 3, NetLogEventType::SOCKET_POOL));
}

// Check that an async ConnectJob failure does not result in creation of a new
// ConnectJob when there's another pending request also waiting on its own
// ConnectJob.  See http://crbug.com/463960.
TEST_F(ClientSocketPoolBaseTest, AsyncFailureWithPendingRequestWithJob) {
  CreatePool(2, 2);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  EXPECT_THAT(request(0)->WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_THAT(request(1)->WaitForResult(), IsError(ERR_CONNECTION_FAILED));

  EXPECT_EQ(2, client_socket_factory_.allocation_count());
}

TEST_F(ClientSocketPoolBaseTest, TwoRequestsCancelOne) {
  // TODO(eroman): Add back the log expectations! Removed them because the
  //               ordering is difficult, and some may fire during destructor.
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;

  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  BoundTestNetLog log2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  handle.Reset();


  // At this point, request 2 is just waiting for the connect job to finish.

  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  handle2.Reset();

  // Now request 2 has actually finished.
  // TODO(eroman): Add back log expectations.
}

TEST_F(ClientSocketPoolBaseTest, CancelRequestLimitsJobs) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), LOWEST), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), LOW), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), MEDIUM), IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), HIGHEST), IsError(ERR_IO_PENDING));

  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));
  (*requests())[2]->handle()->Reset();
  (*requests())[3]->handle()->Reset();
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));

  (*requests())[1]->handle()->Reset();
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));

  (*requests())[0]->handle()->Reset();
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));
}

// When requests and ConnectJobs are not coupled, the request will get serviced
// by whatever comes first.
TEST_F(ClientSocketPoolBaseTest, ReleaseSockets) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // Start job 1 (async OK)
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  std::vector<TestSocketRequest*> request_order;
  size_t completion_count;  // unused
  TestSocketRequest req1(&request_order, &completion_count);
  int rv = req1.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req1.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_THAT(req1.WaitForResult(), IsOk());

  // Job 1 finished OK.  Start job 2 (also async OK).  Request 3 is pending
  // without a job.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  TestSocketRequest req2(&request_order, &completion_count);
  rv = req2.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req2.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  TestSocketRequest req3(&request_order, &completion_count);
  rv = req3.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req3.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Both Requests 2 and 3 are pending.  We release socket 1 which should
  // service request 2.  Request 3 should still be waiting.
  req1.handle()->Reset();
  // Run the released socket wakeups.
  base::RunLoop().RunUntilIdle();
  ASSERT_TRUE(req2.handle()->socket());
  EXPECT_THAT(req2.WaitForResult(), IsOk());
  EXPECT_FALSE(req3.handle()->socket());

  // Signal job 2, which should service request 3.

  client_socket_factory_.SignalJobs();
  EXPECT_THAT(req3.WaitForResult(), IsOk());

  ASSERT_EQ(3u, request_order.size());
  EXPECT_EQ(&req1, request_order[0]);
  EXPECT_EQ(&req2, request_order[1]);
  EXPECT_EQ(&req3, request_order[2]);
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

// The requests are not coupled to the jobs.  So, the requests should finish in
// their priority / insertion order.
TEST_F(ClientSocketPoolBaseTest, PendingJobCompletionOrder) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  // First two jobs are async.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  std::vector<TestSocketRequest*> request_order;
  size_t completion_count;  // unused
  TestSocketRequest req1(&request_order, &completion_count);
  int rv = req1.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req1.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  TestSocketRequest req2(&request_order, &completion_count);
  rv = req2.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req2.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // The pending job is sync.
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  TestSocketRequest req3(&request_order, &completion_count);
  rv = req3.handle()->Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, req3.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  EXPECT_THAT(req1.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_THAT(req2.WaitForResult(), IsOk());
  EXPECT_THAT(req3.WaitForResult(), IsError(ERR_CONNECTION_FAILED));

  ASSERT_EQ(3u, request_order.size());
  EXPECT_EQ(&req1, request_order[0]);
  EXPECT_EQ(&req2, request_order[1]);
  EXPECT_EQ(&req3, request_order[2]);
}

// Test GetLoadState in the case there's only one socket request.
TEST_F(ClientSocketPoolBaseTest, LoadStateOneRequest) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());

  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_SSL_HANDSHAKE);
  EXPECT_EQ(LOAD_STATE_SSL_HANDSHAKE, handle.GetLoadState());

  // No point in completing the connection, since ClientSocketHandles only
  // expect the LoadState to be checked while connecting.
}

// Test GetLoadState in the case there are two socket requests.
TEST_F(ClientSocketPoolBaseTest, LoadStateTwoRequests) {
  CreatePool(2, 2);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_RESOLVING_HOST);

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  client_socket_factory_.SetJobLoadState(1, LOAD_STATE_RESOLVING_HOST);

  // Each handle should reflect the state of its own job.
  EXPECT_EQ(LOAD_STATE_RESOLVING_HOST, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_RESOLVING_HOST, handle2.GetLoadState());

  // Update the state of the first job.
  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_CONNECTING);

  // Only the state of the first request should have changed.
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_RESOLVING_HOST, handle2.GetLoadState());

  // Update the state of the second job.
  client_socket_factory_.SetJobLoadState(1, LOAD_STATE_SSL_HANDSHAKE);

  // Only the state of the second request should have changed.
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_SSL_HANDSHAKE, handle2.GetLoadState());

  // Second job connects and the first request gets the socket.  The
  // second handle switches to the state of the remaining ConnectJob.
  client_socket_factory_.SignalJob(1);
  EXPECT_THAT(callback.WaitForResult(), IsOk());
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle2.GetLoadState());
}

// Test GetLoadState in the case the per-group limit is reached.
TEST_F(ClientSocketPoolBaseTest, LoadStateGroupLimit) {
  CreatePool(2, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, MEDIUM, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());

  // Request another socket from the same pool, buth with a higher priority.
  // The first request should now be stalled at the socket group limit.
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, HIGHEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_AVAILABLE_SOCKET, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle2.GetLoadState());

  // The first handle should remain stalled as the other socket goes through
  // the connect process.

  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_SSL_HANDSHAKE);
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_AVAILABLE_SOCKET, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_SSL_HANDSHAKE, handle2.GetLoadState());

  client_socket_factory_.SignalJob(0);
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_AVAILABLE_SOCKET, handle.GetLoadState());

  // Closing the second socket should cause the stalled handle to finally get a
  // ConnectJob.
  handle2.socket()->Disconnect();
  handle2.Reset();
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());
}

// Test GetLoadState in the case the per-pool limit is reached.
TEST_F(ClientSocketPoolBaseTest, LoadStatePoolLimit) {
  CreatePool(2, 2);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Request for socket from another pool.
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  rv = handle2.Init(TestGroupId("b"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Request another socket from the first pool.  Request should stall at the
  // socket pool limit.
  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  rv = handle3.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // The third handle should remain stalled as the other sockets in its group
  // goes through the connect process.

  EXPECT_EQ(LOAD_STATE_CONNECTING, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_STALLED_SOCKET_POOL, handle3.GetLoadState());

  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_SSL_HANDSHAKE);
  EXPECT_EQ(LOAD_STATE_SSL_HANDSHAKE, handle.GetLoadState());
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_STALLED_SOCKET_POOL, handle3.GetLoadState());

  client_socket_factory_.SignalJob(0);
  EXPECT_THAT(callback.WaitForResult(), IsOk());
  EXPECT_EQ(LOAD_STATE_WAITING_FOR_STALLED_SOCKET_POOL, handle3.GetLoadState());

  // Closing a socket should allow the stalled handle to finally get a new
  // ConnectJob.
  handle.socket()->Disconnect();
  handle.Reset();
  EXPECT_EQ(LOAD_STATE_CONNECTING, handle3.GetLoadState());
}

TEST_F(ClientSocketPoolBaseTest, CertError) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockCertErrorJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_CERT_COMMON_NAME_INVALID,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_TRUE(handle.is_initialized());
  EXPECT_TRUE(handle.socket());
}

TEST_F(ClientSocketPoolBaseTest, AsyncCertError) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingCertErrorJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));
  EXPECT_THAT(callback.WaitForResult(), IsError(ERR_CERT_COMMON_NAME_INVALID));
  EXPECT_TRUE(handle.is_initialized());
  EXPECT_TRUE(handle.socket());
}

TEST_F(ClientSocketPoolBaseTest, AdditionalErrorStateSynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAdditionalErrorStateJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_CONNECTION_FAILED,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_FALSE(handle.is_initialized());
  EXPECT_FALSE(handle.socket());
  EXPECT_TRUE(handle.is_ssl_error());
  EXPECT_TRUE(handle.ssl_cert_request_info());
}

TEST_F(ClientSocketPoolBaseTest, AdditionalErrorStateAsynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(
      TestConnectJob::kMockPendingAdditionalErrorStateJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));
  EXPECT_THAT(callback.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_FALSE(handle.is_initialized());
  EXPECT_FALSE(handle.socket());
  EXPECT_TRUE(handle.is_ssl_error());
  EXPECT_TRUE(handle.ssl_cert_request_info());
}

// Make sure we can reuse sockets.
TEST_F(ClientSocketPoolBaseTest, CleanupTimedOutIdleSocketsReuse) {
  CreatePoolWithIdleTimeouts(
      kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
      base::TimeDelta(),  // Time out unused sockets immediately.
      base::TimeDelta::FromDays(1));  // Don't time out used sockets.

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));
  ASSERT_THAT(callback.WaitForResult(), IsOk());

  // Use and release the socket.
  EXPECT_EQ(1, handle.socket()->Write(nullptr, 1, CompletionOnceCallback(),
                                      TRAFFIC_ANNOTATION_FOR_TESTS));
  TestLoadTimingInfoConnectedNotReused(handle);
  handle.Reset();

  // Should now have one idle socket.
  ASSERT_EQ(1, pool_->IdleSocketCount());

  // Request a new socket. This should reuse the old socket and complete
  // synchronously.
  BoundTestNetLog log;
  rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, CompletionOnceCallback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), log.bound());
  ASSERT_THAT(rv, IsOk());
  EXPECT_TRUE(handle.is_reused());
  TestLoadTimingInfoConnectedReused(handle);

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);
  EXPECT_TRUE(LogContainsEvent(
      entries, 0, NetLogEventType::TCP_CLIENT_SOCKET_POOL_REQUESTED_SOCKET,
      NetLogEventPhase::NONE));
  EXPECT_TRUE(LogContainsBeginEvent(entries, 1, NetLogEventType::SOCKET_POOL));
  EXPECT_TRUE(LogContainsEntryWithType(
      entries, 2, NetLogEventType::SOCKET_POOL_REUSED_AN_EXISTING_SOCKET));
}

// Make sure we cleanup old unused sockets.
TEST_F(ClientSocketPoolBaseTest, CleanupTimedOutIdleSocketsNoReuse) {
  CreatePoolWithIdleTimeouts(
      kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
      base::TimeDelta(),  // Time out unused sockets immediately
      base::TimeDelta());  // Time out used sockets immediately

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  // Startup two mock pending connect jobs, which will sit in the MessageLoop.

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), &handle2));

  // Cancel one of the requests.  Wait for the other, which will get the first
  // job.  Release the socket.  Run the loop again to make sure the second
  // socket is sitting idle and the first one is released (since ReleaseSocket()
  // just posts a DoReleaseSocket() task).

  handle.Reset();
  ASSERT_THAT(callback2.WaitForResult(), IsOk());
  // Use the socket.
  EXPECT_EQ(1, handle2.socket()->Write(nullptr, 1, CompletionOnceCallback(),
                                       TRAFFIC_ANNOTATION_FOR_TESTS));
  handle2.Reset();

  // We post all of our delayed tasks with a 2ms delay. I.e. they don't
  // actually become pending until 2ms after they have been created. In order
  // to flush all tasks, we need to wait so that we know there are no
  // soon-to-be-pending tasks waiting.
  FastForwardBy(base::TimeDelta::FromMilliseconds(10));

  // Both sockets should now be idle.
  ASSERT_EQ(2, pool_->IdleSocketCount());

  // Request a new socket. This should cleanup the unused and timed out ones.
  // A new socket will be created rather than reusing the idle one.
  BoundTestNetLog log;
  TestCompletionCallback callback3;
  rv = handle.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), log.bound());
  ASSERT_THAT(rv, IsError(ERR_IO_PENDING));
  ASSERT_THAT(callback3.WaitForResult(), IsOk());
  EXPECT_FALSE(handle.is_reused());

  // Make sure the idle socket is closed.
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  TestNetLogEntry::List entries;
  log.GetEntries(&entries);
  EXPECT_FALSE(LogContainsEntryWithType(
      entries, 1, NetLogEventType::SOCKET_POOL_REUSED_AN_EXISTING_SOCKET));
}

// Make sure that we process all pending requests even when we're stalling
// because of multiple releasing disconnected sockets.
TEST_F(ClientSocketPoolBaseTest, MultipleReleasingDisconnectedSockets) {
  CreatePoolWithIdleTimeouts(
      kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
      base::TimeDelta(),  // Time out unused sockets immediately.
      base::TimeDelta::FromDays(1));  // Don't time out used sockets.

  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  // Startup 4 connect jobs.  Two of them will be pending.

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  int rv = handle.Init(
      TestGroupId("a"), params_, base::nullopt, LOWEST, SocketTag(),
      ClientSocketPool::RespectLimits::ENABLED, callback.callback(),
      ClientSocketPool::ProxyAuthCallback(), pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsOk());

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  rv = handle2.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsOk());

  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  rv = handle3.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  ClientSocketHandle handle4;
  TestCompletionCallback callback4;
  rv = handle4.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback4.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  // Release two disconnected sockets.

  handle.socket()->Disconnect();
  handle.Reset();
  handle2.socket()->Disconnect();
  handle2.Reset();

  EXPECT_THAT(callback3.WaitForResult(), IsOk());
  EXPECT_FALSE(handle3.is_reused());
  EXPECT_THAT(callback4.WaitForResult(), IsOk());
  EXPECT_FALSE(handle4.is_reused());
}

// Regression test for http://crbug.com/42267.
// When DoReleaseSocket() is processed for one socket, it is blocked because the
// other stalled groups all have releasing sockets, so no progress can be made.
TEST_F(ClientSocketPoolBaseTest, SocketLimitReleasingSockets) {
  CreatePoolWithIdleTimeouts(
      4 /* socket limit */, 4 /* socket limit per group */,
      base::TimeDelta(),  // Time out unused sockets immediately.
      base::TimeDelta::FromDays(1));  // Don't time out used sockets.

  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  // Max out the socket limit with 2 per group.

  ClientSocketHandle handle_a[4];
  TestCompletionCallback callback_a[4];
  ClientSocketHandle handle_b[4];
  TestCompletionCallback callback_b[4];

  for (int i = 0; i < 2; ++i) {
    EXPECT_EQ(OK, handle_a[i].Init(TestGroupId("a"), params_, base::nullopt,
                                   LOWEST, SocketTag(),
                                   ClientSocketPool::RespectLimits::ENABLED,
                                   callback_a[i].callback(),
                                   ClientSocketPool::ProxyAuthCallback(),
                                   pool_.get(), NetLogWithSource()));
    EXPECT_EQ(OK, handle_b[i].Init(TestGroupId("b"), params_, base::nullopt,
                                   LOWEST, SocketTag(),
                                   ClientSocketPool::RespectLimits::ENABLED,
                                   callback_b[i].callback(),
                                   ClientSocketPool::ProxyAuthCallback(),
                                   pool_.get(), NetLogWithSource()));
  }

  // Make 4 pending requests, 2 per group.

  for (int i = 2; i < 4; ++i) {
    EXPECT_EQ(
        ERR_IO_PENDING,
        handle_a[i].Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                         SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                         callback_a[i].callback(),
                         ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                         NetLogWithSource()));
    EXPECT_EQ(
        ERR_IO_PENDING,
        handle_b[i].Init(TestGroupId("b"), params_, base::nullopt, LOWEST,
                         SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                         callback_b[i].callback(),
                         ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                         NetLogWithSource()));
  }

  // Release b's socket first.  The order is important, because in
  // DoReleaseSocket(), we'll process b's released socket, and since both b and
  // a are stalled, but 'a' is lower lexicographically, we'll process group 'a'
  // first, which has a releasing socket, so it refuses to start up another
  // ConnectJob.  So, we used to infinite loop on this.
  handle_b[0].socket()->Disconnect();
  handle_b[0].Reset();
  handle_a[0].socket()->Disconnect();
  handle_a[0].Reset();

  // Used to get stuck here.
  base::RunLoop().RunUntilIdle();

  handle_b[1].socket()->Disconnect();
  handle_b[1].Reset();
  handle_a[1].socket()->Disconnect();
  handle_a[1].Reset();

  for (int i = 2; i < 4; ++i) {
    EXPECT_THAT(callback_b[i].WaitForResult(), IsOk());
    EXPECT_THAT(callback_a[i].WaitForResult(), IsOk());
  }
}

TEST_F(ClientSocketPoolBaseTest,
       ReleasingDisconnectedSocketsMaintainsPriorityOrder) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY),
              IsError(ERR_IO_PENDING));

  EXPECT_THAT((*requests())[0]->WaitForResult(), IsOk());
  EXPECT_THAT((*requests())[1]->WaitForResult(), IsOk());
  EXPECT_EQ(2u, completion_count());

  // Releases one connection.
  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::NO_KEEP_ALIVE));
  EXPECT_THAT((*requests())[2]->WaitForResult(), IsOk());

  EXPECT_TRUE(ReleaseOneConnection(ClientSocketPoolTest::NO_KEEP_ALIVE));
  EXPECT_THAT((*requests())[3]->WaitForResult(), IsOk());
  EXPECT_EQ(4u, completion_count());

  EXPECT_EQ(1, GetOrderOfRequest(1));
  EXPECT_EQ(2, GetOrderOfRequest(2));
  EXPECT_EQ(3, GetOrderOfRequest(3));
  EXPECT_EQ(4, GetOrderOfRequest(4));

  // Make sure we test order of all requests made.
  EXPECT_EQ(ClientSocketPoolTest::kIndexOutOfBounds, GetOrderOfRequest(5));
}

class TestReleasingSocketRequest : public TestCompletionCallbackBase {
 public:
  TestReleasingSocketRequest(TransportClientSocketPool* pool,
                             int expected_result,
                             bool reset_releasing_handle)
      : pool_(pool),
        expected_result_(expected_result),
        reset_releasing_handle_(reset_releasing_handle) {}

  ~TestReleasingSocketRequest() override = default;

  ClientSocketHandle* handle() { return &handle_; }

  CompletionOnceCallback callback() {
    return base::BindOnce(&TestReleasingSocketRequest::OnComplete,
                          base::Unretained(this));
  }

 private:
  void OnComplete(int result) {
    SetResult(result);
    if (reset_releasing_handle_)
      handle_.Reset();

    EXPECT_EQ(
        expected_result_,
        handle2_.Init(
            TestGroupId("a"),
            ClientSocketPool::SocketParams::CreateForHttpForTesting(),
            base::nullopt, DEFAULT_PRIORITY, SocketTag(),
            ClientSocketPool::RespectLimits::ENABLED, CompletionOnceCallback(),
            ClientSocketPool::ProxyAuthCallback(), pool_, NetLogWithSource()));
  }

  TransportClientSocketPool* const pool_;
  int expected_result_;
  bool reset_releasing_handle_;
  ClientSocketHandle handle_;
  ClientSocketHandle handle2_;
};


TEST_F(ClientSocketPoolBaseTest, AdditionalErrorSocketsDontUseSlot) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("a"), DEFAULT_PRIORITY), IsOk());
  EXPECT_THAT(StartRequest(TestGroupId("b"), DEFAULT_PRIORITY), IsOk());

  EXPECT_EQ(static_cast<int>(requests_size()),
            client_socket_factory_.allocation_count());

  connect_job_factory_->set_job_type(
      TestConnectJob::kMockPendingAdditionalErrorStateJob);
  TestReleasingSocketRequest req(pool_.get(), OK, false);
  EXPECT_EQ(ERR_IO_PENDING,
            req.handle()->Init(
                TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                req.callback(), ClientSocketPool::ProxyAuthCallback(),
                pool_.get(), NetLogWithSource()));
  // The next job should complete synchronously
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  EXPECT_THAT(req.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_FALSE(req.handle()->is_initialized());
  EXPECT_FALSE(req.handle()->socket());
  EXPECT_TRUE(req.handle()->is_ssl_error());
  EXPECT_TRUE(req.handle()->ssl_cert_request_info());
}

// http://crbug.com/44724 regression test.
// We start releasing the pool when we flush on network change.  When that
// happens, the only active references are in the ClientSocketHandles.  When a
// ConnectJob completes and calls back into the last ClientSocketHandle, that
// callback can release the last reference and delete the pool.  After the
// callback finishes, we go back to the stack frame within the now-deleted pool.
// Executing any code that refers to members of the now-deleted pool can cause
// crashes.
TEST_F(ClientSocketPoolBaseTest, CallbackThatReleasesPool) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  pool_->FlushWithError(ERR_NETWORK_CHANGED);

  // We'll call back into this now.
  callback.WaitForResult();
}

TEST_F(ClientSocketPoolBaseTest, DoNotReuseSocketAfterFlush) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());
  EXPECT_EQ(ClientSocketHandle::UNUSED, handle.reuse_type());

  pool_->FlushWithError(ERR_NETWORK_CHANGED);

  handle.Reset();
  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());
  EXPECT_EQ(ClientSocketHandle::UNUSED, handle.reuse_type());
}

class ConnectWithinCallback : public TestCompletionCallbackBase {
 public:
  ConnectWithinCallback(
      const ClientSocketPool::GroupId& group_id,
      const scoped_refptr<ClientSocketPool::SocketParams>& params,
      TransportClientSocketPool* pool)
      : group_id_(group_id), params_(params), pool_(pool) {}

  ~ConnectWithinCallback() override = default;

  int WaitForNestedResult() {
    return nested_callback_.WaitForResult();
  }

  CompletionOnceCallback callback() {
    return base::BindOnce(&ConnectWithinCallback::OnComplete,
                          base::Unretained(this));
  }

 private:
  void OnComplete(int result) {
    SetResult(result);
    EXPECT_EQ(
        ERR_IO_PENDING,
        handle_.Init(group_id_, params_, base::nullopt, DEFAULT_PRIORITY,
                     SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                     nested_callback_.callback(),
                     ClientSocketPool::ProxyAuthCallback(), pool_,
                     NetLogWithSource()));
  }

  const ClientSocketPool::GroupId group_id_;
  const scoped_refptr<ClientSocketPool::SocketParams> params_;
  TransportClientSocketPool* const pool_;
  ClientSocketHandle handle_;
  TestCompletionCallback nested_callback_;

  DISALLOW_COPY_AND_ASSIGN(ConnectWithinCallback);
};

TEST_F(ClientSocketPoolBaseTest, AbortAllRequestsOnFlush) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);

  // First job will be waiting until it gets aborted.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  ConnectWithinCallback callback(TestGroupId("a"), params_, pool_.get());
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  // Second job will be started during the first callback, and will
  // asynchronously complete with OK.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  pool_->FlushWithError(ERR_NETWORK_CHANGED);
  EXPECT_THAT(callback.WaitForResult(), IsError(ERR_NETWORK_CHANGED));
  EXPECT_THAT(callback.WaitForNestedResult(), IsOk());
}

TEST_F(ClientSocketPoolBaseTest, BackupSocketWaitsForHostResolution) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets,
             true /* enable_backup_connect_jobs */);

  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  // The backup timer fires but doesn't start a new ConnectJob while resolving
  // the hostname.
  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_RESOLVING_HOST);
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs * 100));
  EXPECT_EQ(1, client_socket_factory_.allocation_count());

  // Once the ConnectJob has finished resolving the hostname, the backup timer
  // will create a ConnectJob when it fires.
  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_CONNECTING);
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs));
  EXPECT_EQ(2, client_socket_factory_.allocation_count());
}

// Test that no backup socket is created when a ConnectJob connects before it
// completes.
TEST_F(ClientSocketPoolBaseTest, NoBackupSocketWhenConnected) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets,
             true /* enable_backup_connect_jobs */);

  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  // The backup timer fires but doesn't start a new ConnectJob while resolving
  // the hostname.
  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_RESOLVING_HOST);
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs * 100));
  EXPECT_EQ(1, client_socket_factory_.allocation_count());

  client_socket_factory_.SetJobLoadState(0, LOAD_STATE_SSL_HANDSHAKE);
  client_socket_factory_.SetJobHasEstablishedConnection(0);
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs * 100));
  EXPECT_EQ(1, client_socket_factory_.allocation_count());
}

// Cancel a pending socket request while we're at max sockets,
// and verify that the backup socket firing doesn't cause a crash.
TEST_F(ClientSocketPoolBaseTest, BackupSocketCancelAtMaxSockets) {
  // Max 4 sockets globally, max 4 sockets per group.
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets,
             true /* enable_backup_connect_jobs */);

  // Create the first socket and set to ERR_IO_PENDING.  This starts the backup
  // timer.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  // Start (MaxSockets - 1) connected sockets to reach max sockets.
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);
  ClientSocketHandle handles[kDefaultMaxSockets];
  for (int i = 1; i < kDefaultMaxSockets; ++i) {
    TestCompletionCallback callback;
    EXPECT_EQ(OK, handles[i].Init(TestGroupId("bar"), params_, base::nullopt,
                                  DEFAULT_PRIORITY, SocketTag(),
                                  ClientSocketPool::RespectLimits::ENABLED,
                                  callback.callback(),
                                  ClientSocketPool::ProxyAuthCallback(),
                                  pool_.get(), NetLogWithSource()));
  }

  base::RunLoop().RunUntilIdle();

  // Cancel the pending request.
  handle.Reset();

  // Wait for the backup timer to fire (add some slop to ensure it fires)
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs / 2 * 3));

  EXPECT_EQ(kDefaultMaxSockets, client_socket_factory_.allocation_count());
}

TEST_F(ClientSocketPoolBaseTest, CancelBackupSocketAfterCancelingAllRequests) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets,
             true /* enable_backup_connect_jobs */);

  // Create the first socket and set to ERR_IO_PENDING.  This starts the backup
  // timer.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("bar")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("bar")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("bar")));
  EXPECT_EQ(
      0u, pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("bar")));

  // Cancel the socket request.  This should cancel the backup timer.  Wait for
  // the backup time to see if it indeed got canceled.
  handle.Reset();
  // Wait for the backup timer to fire (add some slop to ensure it fires)
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs / 2 * 3));
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("bar")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("bar")));
}

TEST_F(ClientSocketPoolBaseTest, CancelBackupSocketAfterFinishingAllRequests) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets,
             true /* enable_backup_connect_jobs */);

  // Create the first socket and set to ERR_IO_PENDING.  This starts the backup
  // timer.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("bar"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("bar")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("bar")));

  // Cancel request 1 and then complete request 2.  With the requests finished,
  // the backup timer should be cancelled.
  handle.Reset();
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  // Wait for the backup timer to fire (add some slop to ensure it fires)
  FastForwardBy(base::TimeDelta::FromMilliseconds(
      ClientSocketPool::kMaxConnectRetryIntervalMs / 2 * 3));
}

// Test delayed socket binding for the case where we have two connects,
// and while one is waiting on a connect, the other frees up.
// The socket waiting on a connect should switch immediately to the freed
// up socket.
TEST_F(ClientSocketPoolBaseTest, DelayedSocketBindingWaitingForConnect) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // No idle sockets, no pending jobs.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Create a second socket to the same host, but this one will wait.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  // No idle sockets, and one connecting job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Return the first handle to the pool.  This will initiate the delayed
  // binding.
  handle1.Reset();

  base::RunLoop().RunUntilIdle();

  // Still no idle sockets, still one pending connect job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // The second socket connected, even though it was a Waiting Job.
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // And we can see there is still one job waiting.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Finally, signal the waiting Connect.
  client_socket_factory_.SignalJobs();
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  base::RunLoop().RunUntilIdle();
}

// Test delayed socket binding when a group is at capacity and one
// of the group's sockets frees up.
TEST_F(ClientSocketPoolBaseTest, DelayedSocketBindingAtGroupCapacity) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // No idle sockets, no pending jobs.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Create a second socket to the same host, but this one will wait.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  // No idle sockets, and one connecting job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Return the first handle to the pool.  This will initiate the delayed
  // binding.
  handle1.Reset();

  base::RunLoop().RunUntilIdle();

  // Still no idle sockets, still one pending connect job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // The second socket connected, even though it was a Waiting Job.
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // And we can see there is still one job waiting.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Finally, signal the waiting Connect.
  client_socket_factory_.SignalJobs();
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  base::RunLoop().RunUntilIdle();
}

// Test out the case where we have one socket connected, one
// connecting, when the first socket finishes and goes idle.
// Although the second connection is pending, the second request
// should complete, by taking the first socket's idle socket.
TEST_F(ClientSocketPoolBaseTest, DelayedSocketBindingAtStall) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // No idle sockets, no pending jobs.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Create a second socket to the same host, but this one will wait.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  ClientSocketHandle handle2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  // No idle sockets, and one connecting job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Return the first handle to the pool.  This will initiate the delayed
  // binding.
  handle1.Reset();

  base::RunLoop().RunUntilIdle();

  // Still no idle sockets, still one pending connect job.
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // The second socket connected, even though it was a Waiting Job.
  EXPECT_THAT(callback.WaitForResult(), IsOk());

  // And we can see there is still one job waiting.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Finally, signal the waiting Connect.
  client_socket_factory_.SignalJobs();
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  base::RunLoop().RunUntilIdle();
}

// Cover the case where on an available socket slot, we have one pending
// request that completes synchronously, thereby making the Group empty.
TEST_F(ClientSocketPoolBaseTest, SynchronouslyProcessOnePendingRequest) {
  const int kUnlimitedSockets = 100;
  const int kOneSocketPerGroup = 1;
  CreatePool(kUnlimitedSockets, kOneSocketPerGroup);

  // Make the first request asynchronous fail.
  // This will free up a socket slot later.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingFailingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Make the second request synchronously fail.  This should make the Group
  // empty.
  connect_job_factory_->set_job_type(TestConnectJob::kMockFailingJob);
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  // It'll be ERR_IO_PENDING now, but the TestConnectJob will synchronously fail
  // when created.
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  EXPECT_THAT(callback1.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_THAT(callback2.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, PreferUsedSocketToUnusedSocket) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets);

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle3.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  EXPECT_THAT(callback3.WaitForResult(), IsOk());

  // Use the socket.
  EXPECT_EQ(1, handle1.socket()->Write(nullptr, 1, CompletionOnceCallback(),
                                       TRAFFIC_ANNOTATION_FOR_TESTS));
  EXPECT_EQ(1, handle3.socket()->Write(nullptr, 1, CompletionOnceCallback(),
                                       TRAFFIC_ANNOTATION_FOR_TESTS));

  handle1.Reset();
  handle2.Reset();
  handle3.Reset();

  EXPECT_EQ(OK, handle1.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));
  EXPECT_EQ(OK, handle2.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));
  EXPECT_EQ(OK, handle3.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));

  EXPECT_TRUE(handle1.socket()->WasEverUsed());
  EXPECT_TRUE(handle2.socket()->WasEverUsed());
  EXPECT_FALSE(handle3.socket()->WasEverUsed());
}

TEST_F(ClientSocketPoolBaseTest, RequestSockets) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(2u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  handle1.Reset();
  handle2.Reset();

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsWhenAlreadyHaveAConnectJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  handle1.Reset();
  handle2.Reset();

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest,
       RequestSocketsWhenAlreadyHaveMultipleConnectJob) {
  CreatePool(4, 4);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle3.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
  EXPECT_THAT(callback3.WaitForResult(), IsOk());
  handle1.Reset();
  handle2.Reset();
  handle3.Reset();

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(3u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsAtMaxSocketLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt,
                        kDefaultMaxSockets, NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(kDefaultMaxSockets,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));
  EXPECT_EQ(
      kDefaultMaxSockets,
      static_cast<int>(pool_->NumNeverAssignedConnectJobsInGroupForTesting(
          TestGroupId("a"))));
  EXPECT_EQ(kDefaultMaxSockets,
            static_cast<int>(pool_->NumUnassignedConnectJobsInGroupForTesting(
                TestGroupId("a"))));

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("b")));

  pool_->RequestSockets(TestGroupId("b"), params_, base::nullopt,
                        kDefaultMaxSockets, NetLogWithSource());

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("b")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsHitMaxSocketLimit) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSockets);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt,
                        kDefaultMaxSockets - 1, NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(kDefaultMaxSockets - 1,
            static_cast<int>(
                pool_->NumConnectJobsInGroupForTesting(TestGroupId("a"))));
  EXPECT_EQ(
      kDefaultMaxSockets - 1,
      static_cast<int>(pool_->NumNeverAssignedConnectJobsInGroupForTesting(
          TestGroupId("a"))));
  EXPECT_EQ(kDefaultMaxSockets - 1,
            static_cast<int>(pool_->NumUnassignedConnectJobsInGroupForTesting(
                TestGroupId("a"))));
  EXPECT_FALSE(pool_->IsStalled());

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("b")));

  pool_->RequestSockets(TestGroupId("b"), params_, base::nullopt,
                        kDefaultMaxSockets, NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_FALSE(pool_->IsStalled());
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsCountIdleSockets) {
  CreatePool(4, 4);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  ASSERT_THAT(callback1.WaitForResult(), IsOk());
  handle1.Reset();

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsCountActiveSockets) {
  CreatePool(4, 4);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  ASSERT_THAT(callback1.WaitForResult(), IsOk());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsSynchronous) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt,
                        kDefaultMaxSocketsPerGroup, NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(pool_->IdleSocketCountInGroup(TestGroupId("a"))));

  pool_->RequestSockets(TestGroupId("b"), params_, base::nullopt,
                        kDefaultMaxSocketsPerGroup, NetLogWithSource());

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("b")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(kDefaultMaxSocketsPerGroup,
            static_cast<int>(pool_->IdleSocketCountInGroup(TestGroupId("b"))));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsSynchronousError) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockFailingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt,
                        kDefaultMaxSocketsPerGroup, NetLogWithSource());

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));

  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAdditionalErrorStateJob);
  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt,
                        kDefaultMaxSocketsPerGroup, NetLogWithSource());

  ASSERT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsMultipleTimesDoesNothing) {
  CreatePool(4, 4);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(2u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(2u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  client_socket_factory_.SignalJob(0);
  EXPECT_THAT(callback1.WaitForResult(), IsOk());

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  client_socket_factory_.SignalJob(0);
  EXPECT_THAT(callback2.WaitForResult(), IsOk());

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  handle1.Reset();
  handle2.Reset();

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestSocketsDifferentNumSockets) {
  CreatePool(4, 4);
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(2u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 3,
                        NetLogWithSource());
  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(3u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(3u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());
  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(3u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(3u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, PreconnectJobsTakenByNormalRequests) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  client_socket_factory_.SignalJobs();
  EXPECT_THAT(callback1.WaitForResult(), IsOk());

  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  // Make sure if a preconnected socket is not fully connected when a request
  // starts, it has a connect start time.
  TestLoadTimingInfoConnectedNotReused(handle1);
  handle1.Reset();

  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

// Checks that fully connected preconnect jobs have no connect times, and are
// marked as reused.
TEST_F(ClientSocketPoolBaseTest, ConnectedPreconnectJobsHaveNoConnectTimes) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);
  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(OK, handle.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));

  // Make sure the idle socket was used.
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  TestLoadTimingInfoConnectedReused(handle);
  handle.Reset();
  TestLoadTimingInfoNotConnected(handle);
}

// http://crbug.com/64940 regression test.
TEST_F(ClientSocketPoolBaseTest, PreconnectClosesIdleSocketRemovesGroup) {
  const int kMaxTotalSockets = 3;
  const int kMaxSocketsPerGroup = 2;
  CreatePool(kMaxTotalSockets, kMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  // Note that group id ordering matters here.  "a" comes before "b", so
  // CloseOneIdleSocket() will try to close "a"'s idle socket.

  // Set up one idle socket in "a".
  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  client_socket_factory_.SignalJobs();
  ASSERT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  handle1.Reset();
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // Set up two active sockets in "b".
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("b"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("b"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("b")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("b")));

  client_socket_factory_.SignalJobs();
  ASSERT_THAT(callback1.WaitForResult(), IsOk());
  ASSERT_THAT(callback2.WaitForResult(), IsOk());
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("b")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(2, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("b")));

  // Now we have 1 idle socket in "a" and 2 active sockets in "b".  This means
  // we've maxed out on sockets, since we set |kMaxTotalSockets| to 3.
  // Requesting 2 preconnected sockets for "a" should fail to allocate any more
  // sockets for "a", and "b" should still have 2 active sockets.

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("b")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("b")));
  EXPECT_EQ(2, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("b")));

  // Now release the 2 active sockets for "b".  This will give us 1 idle socket
  // in "a" and 2 idle sockets in "b".  Requesting 2 preconnected sockets for
  // "a" should result in closing 1 for "b".
  handle1.Reset();
  handle2.Reset();
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(TestGroupId("b")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("b")));

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 2,
                        NetLogWithSource());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("b")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("b")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("b")));
}

TEST_F(ClientSocketPoolBaseTest, PreconnectWithoutBackupJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
             true /* enable_backup_connect_jobs */);

  // Make the ConnectJob hang until it times out, shorten the timeout.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  connect_job_factory_->set_timeout_duration(
      base::TimeDelta::FromMilliseconds(500));
  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // Verify the backup timer doesn't create a backup job, by making
  // the backup job a pending job instead of a waiting job, so it
  // *would* complete if it were created.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, base::RunLoop::QuitCurrentWhenIdleClosureDeprecated(),
      base::TimeDelta::FromSeconds(1));
  base::RunLoop().Run();
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, PreconnectWithBackupJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup,
             true /* enable_backup_connect_jobs */);

  // Make the ConnectJob hang forever.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  base::RunLoop().RunUntilIdle();

  // Make the backup job be a pending job, so it completes normally.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  // Timer has started, but the backup connect job shouldn't be created yet.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  ASSERT_THAT(callback.WaitForResult(), IsOk());

  // The hung connect job should still be there, but everything else should be
  // complete.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
}

// Tests that a preconnect that starts out with unread data can still be used.
// http://crbug.com/334467
TEST_F(ClientSocketPoolBaseTest, PreconnectWithUnreadData) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockUnreadDataJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // Fail future jobs to be sure that handle receives the preconnected socket
  // rather than closing it and making a new one.
  connect_job_factory_->set_job_type(TestConnectJob::kMockFailingJob);
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(OK, handle.Init(
                    TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                    SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                    callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                    pool_.get(), NetLogWithSource()));

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));

  // Drain the pending read.
  EXPECT_EQ(1, handle.socket()->Read(nullptr, 1, CompletionOnceCallback()));

  TestLoadTimingInfoConnectedReused(handle);
  handle.Reset();

  // The socket should be usable now that it's idle again.
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
}

TEST_F(ClientSocketPoolBaseTest, RequestGetsAssignedJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
}

TEST_F(ClientSocketPoolBaseTest, MultipleRequestsGetAssignedJobs) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));

  // One job completes. The other request should still have its job.
  client_socket_factory_.SignalJob(0);
  EXPECT_THAT(callback1.WaitForResult(), IsOk());

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));
}

TEST_F(ClientSocketPoolBaseTest, PreconnectJobGetsAssignedToRequest) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  pool_->RequestSockets(TestGroupId("a"), params_, base::nullopt, 1,
                        NetLogWithSource());

  ASSERT_TRUE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(1u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
}

TEST_F(ClientSocketPoolBaseTest, HigherPriorityRequestStealsJob) {
  CreatePool(kDefaultMaxSockets, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));

  // Insert a higher priority request
  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, HIGHEST,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // The highest priority request should steal the job from the default priority
  // request.
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                               &handle1));
}

TEST_F(ClientSocketPoolBaseTest, RequestStealsJobFromLowestRequestWithJob) {
  CreatePool(3, 3);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle_lowest;
  TestCompletionCallback callback_lowest;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle_lowest.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                         SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                         callback_lowest.callback(),
                         ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                         NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle_highest;
  TestCompletionCallback callback_highest;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle_highest.Init(TestGroupId("a"), params_, base::nullopt, HIGHEST,
                          SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                          callback_highest.callback(),
                          ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                          NetLogWithSource()));

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle_low;
  TestCompletionCallback callback_low;
  EXPECT_EQ(ERR_IO_PENDING,
            handle_low.Init(
                TestGroupId("a"), params_, base::nullopt, LOW, SocketTag(),
                ClientSocketPool::RespectLimits::ENABLED,
                callback_low.callback(), ClientSocketPool::ProxyAuthCallback(),
                pool_.get(), NetLogWithSource()));

  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle_lowest2;
  TestCompletionCallback callback_lowest2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle_lowest2.Init(TestGroupId("a"), params_, base::nullopt, LOWEST,
                          SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                          callback_lowest2.callback(),
                          ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                          NetLogWithSource()));

  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // The top three requests in the queue should have jobs.
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_highest));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_low));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_lowest));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(
      TestGroupId("a"), &handle_lowest2));

  // Add another request with medium priority. It should steal the job from the
  // lowest priority request with a job.
  ClientSocketHandle handle_medium;
  TestCompletionCallback callback_medium;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle_medium.Init(TestGroupId("a"), params_, base::nullopt, MEDIUM,
                         SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                         callback_medium.callback(),
                         ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                         NetLogWithSource()));

  EXPECT_EQ(3u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_highest));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_medium));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle_low));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                               &handle_lowest));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(
      TestGroupId("a"), &handle_lowest2));
}

TEST_F(ClientSocketPoolBaseTest, ReprioritizeRequestStealsJob) {
  CreatePool(kDefaultMaxSockets, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // The second request doesn't get a job because we are at the limit.
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                               &handle2));

  // Reprioritizing the second request places it above the first, and it steals
  // the job from the first request.
  pool_->SetPriority(TestGroupId("a"), &handle2, HIGHEST);
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                               &handle1));
}

TEST_F(ClientSocketPoolBaseTest, CancelRequestReassignsJob) {
  CreatePool(kDefaultMaxSockets, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // The second request doesn't get a job because we are the limit.
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
  EXPECT_FALSE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                               &handle2));

  // The second request should get a job upon cancelling the first request.
  handle1.Reset();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));
}

TEST_F(ClientSocketPoolBaseTest, JobCompletionReassignsJob) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle1.Init(TestGroupId("a"), params_, base::nullopt, HIGHEST,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback1.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  ClientSocketHandle handle2;
  TestCompletionCallback callback2;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle2.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle1));
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));

  // The lower-priority job completes first. The higher-priority request should
  // get the socket, and the lower-priority request should get the remaining
  // job.
  client_socket_factory_.SignalJob(1);
  EXPECT_THAT(callback1.WaitForResult(), IsOk());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumNeverAssignedConnectJobsInGroupForTesting(
                    TestGroupId("a")));
  EXPECT_EQ(0u,
            pool_->NumUnassignedConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_TRUE(handle1.socket());
  EXPECT_TRUE(pool_->RequestInGroupWithHandleHasJobForTesting(TestGroupId("a"),
                                                              &handle2));
}

class MockLayeredPool : public HigherLayeredPool {
 public:
  MockLayeredPool(TransportClientSocketPool* pool,
                  const ClientSocketPool::GroupId& group_id)
      : pool_(pool), group_id_(group_id), can_release_connection_(true) {
    pool_->AddHigherLayeredPool(this);
  }

  ~MockLayeredPool() override { pool_->RemoveHigherLayeredPool(this); }

  int RequestSocket(TransportClientSocketPool* pool) {
    return handle_.Init(
        group_id_, ClientSocketPool::SocketParams::CreateForHttpForTesting(),
        base::nullopt, DEFAULT_PRIORITY, SocketTag(),
        ClientSocketPool::RespectLimits::ENABLED, callback_.callback(),
        ClientSocketPool::ProxyAuthCallback(), pool, NetLogWithSource());
  }

  int RequestSocketWithoutLimits(TransportClientSocketPool* pool) {
    return handle_.Init(
        group_id_, ClientSocketPool::SocketParams::CreateForHttpForTesting(),
        base::nullopt, MAXIMUM_PRIORITY, SocketTag(),
        ClientSocketPool::RespectLimits::DISABLED, callback_.callback(),
        ClientSocketPool::ProxyAuthCallback(), pool, NetLogWithSource());
  }

  bool ReleaseOneConnection() {
    if (!handle_.is_initialized() || !can_release_connection_) {
      return false;
    }
    handle_.socket()->Disconnect();
    handle_.Reset();
    return true;
  }

  void set_can_release_connection(bool can_release_connection) {
    can_release_connection_ = can_release_connection;
  }

  MOCK_METHOD0(CloseOneIdleConnection, bool());

 private:
  TransportClientSocketPool* const pool_;
  ClientSocketHandle handle_;
  TestCompletionCallback callback_;
  const ClientSocketPool::GroupId group_id_;
  bool can_release_connection_;
};

// Tests the basic case of closing an idle socket in a higher layered pool when
// a new request is issued and the lower layer pool is stalled.
TEST_F(ClientSocketPoolBaseTest, CloseIdleSocketsHeldByLayeredPoolWhenNeeded) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  MockLayeredPool mock_layered_pool(pool_.get(), TestGroupId("foo"));
  EXPECT_THAT(mock_layered_pool.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool, CloseOneIdleConnection())
      .WillOnce(Invoke(&mock_layered_pool,
                       &MockLayeredPool::ReleaseOneConnection));
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());
}

// Tests the case that trying to close an idle socket in a higher layered pool
// fails.
TEST_F(ClientSocketPoolBaseTest,
       CloseIdleSocketsHeldByLayeredPoolWhenNeededFails) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  MockLayeredPool mock_layered_pool(pool_.get(), TestGroupId("foo"));
  mock_layered_pool.set_can_release_connection(false);
  EXPECT_THAT(mock_layered_pool.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool, CloseOneIdleConnection())
      .WillOnce(Invoke(&mock_layered_pool,
                       &MockLayeredPool::ReleaseOneConnection));
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(callback.have_result());
}

// Same as above, but the idle socket is in the same group as the stalled
// socket, and closes the only other request in its group when closing requests
// in higher layered pools.  This generally shouldn't happen, but it may be
// possible if a higher level pool issues a request and the request is
// subsequently cancelled.  Even if it's not possible, best not to crash.
TEST_F(ClientSocketPoolBaseTest,
       CloseIdleSocketsHeldByLayeredPoolWhenNeededSameGroup) {
  CreatePool(2, 2);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  // Need a socket in another group for the pool to be stalled (If a group
  // has the maximum number of connections already, it's not stalled).
  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(OK, handle1.Init(TestGroupId("group1"), params_, base::nullopt,
                             DEFAULT_PRIORITY, SocketTag(),
                             ClientSocketPool::RespectLimits::ENABLED,
                             callback1.callback(),
                             ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                             NetLogWithSource()));

  MockLayeredPool mock_layered_pool(pool_.get(), TestGroupId("group2"));
  EXPECT_THAT(mock_layered_pool.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool, CloseOneIdleConnection())
      .WillOnce(Invoke(&mock_layered_pool,
                       &MockLayeredPool::ReleaseOneConnection));
  ClientSocketHandle handle;
  TestCompletionCallback callback2;
  EXPECT_EQ(ERR_IO_PENDING,
            handle.Init(
                TestGroupId("group2"), params_, base::nullopt, DEFAULT_PRIORITY,
                SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                callback2.callback(), ClientSocketPool::ProxyAuthCallback(),
                pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback2.WaitForResult(), IsOk());
}

// Tests the case when an idle socket can be closed when a new request is
// issued, and the new request belongs to a group that was previously stalled.
TEST_F(ClientSocketPoolBaseTest,
       CloseIdleSocketsHeldByLayeredPoolInSameGroupWhenNeeded) {
  CreatePool(2, 2);
  std::list<TestConnectJob::JobType> job_types;
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  connect_job_factory_->set_job_types(&job_types);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(OK, handle1.Init(TestGroupId("group1"), params_, base::nullopt,
                             DEFAULT_PRIORITY, SocketTag(),
                             ClientSocketPool::RespectLimits::ENABLED,
                             callback1.callback(),
                             ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                             NetLogWithSource()));

  MockLayeredPool mock_layered_pool(pool_.get(), TestGroupId("group2"));
  EXPECT_THAT(mock_layered_pool.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool, CloseOneIdleConnection())
      .WillRepeatedly(Invoke(&mock_layered_pool,
                             &MockLayeredPool::ReleaseOneConnection));
  mock_layered_pool.set_can_release_connection(false);

  // The third request is made when the socket pool is in a stalled state.
  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  EXPECT_EQ(ERR_IO_PENDING,
            handle3.Init(
                TestGroupId("group3"), params_, base::nullopt, DEFAULT_PRIORITY,
                SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                pool_.get(), NetLogWithSource()));

  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(callback3.have_result());

  // The fourth request is made when the pool is no longer stalled.  The third
  // request should be serviced first, since it was issued first and has the
  // same priority.
  mock_layered_pool.set_can_release_connection(true);
  ClientSocketHandle handle4;
  TestCompletionCallback callback4;
  EXPECT_EQ(ERR_IO_PENDING,
            handle4.Init(
                TestGroupId("group3"), params_, base::nullopt, DEFAULT_PRIORITY,
                SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                callback4.callback(), ClientSocketPool::ProxyAuthCallback(),
                pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback3.WaitForResult(), IsOk());
  EXPECT_FALSE(callback4.have_result());

  // Closing a handle should free up another socket slot.
  handle1.Reset();
  EXPECT_THAT(callback4.WaitForResult(), IsOk());
}

// Tests the case when an idle socket can be closed when a new request is
// issued, and the new request belongs to a group that was previously stalled.
//
// The two differences from the above test are that the stalled requests are not
// in the same group as the layered pool's request, and the the fourth request
// has a higher priority than the third one, so gets a socket first.
TEST_F(ClientSocketPoolBaseTest,
       CloseIdleSocketsHeldByLayeredPoolInSameGroupWhenNeeded2) {
  CreatePool(2, 2);
  std::list<TestConnectJob::JobType> job_types;
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  job_types.push_back(TestConnectJob::kMockJob);
  connect_job_factory_->set_job_types(&job_types);

  ClientSocketHandle handle1;
  TestCompletionCallback callback1;
  EXPECT_EQ(OK, handle1.Init(TestGroupId("group1"), params_, base::nullopt,
                             DEFAULT_PRIORITY, SocketTag(),
                             ClientSocketPool::RespectLimits::ENABLED,
                             callback1.callback(),
                             ClientSocketPool::ProxyAuthCallback(), pool_.get(),
                             NetLogWithSource()));

  MockLayeredPool mock_layered_pool(pool_.get(), TestGroupId("group2"));
  EXPECT_THAT(mock_layered_pool.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool, CloseOneIdleConnection())
      .WillRepeatedly(Invoke(&mock_layered_pool,
                             &MockLayeredPool::ReleaseOneConnection));
  mock_layered_pool.set_can_release_connection(false);

  // The third request is made when the socket pool is in a stalled state.
  ClientSocketHandle handle3;
  TestCompletionCallback callback3;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle3.Init(TestGroupId("group3"), params_, base::nullopt, MEDIUM,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback3.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));

  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(callback3.have_result());

  // The fourth request is made when the pool is no longer stalled.  This
  // request has a higher priority than the third request, so is serviced first.
  mock_layered_pool.set_can_release_connection(true);
  ClientSocketHandle handle4;
  TestCompletionCallback callback4;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle4.Init(TestGroupId("group3"), params_, base::nullopt, HIGHEST,
                   SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                   callback4.callback(), ClientSocketPool::ProxyAuthCallback(),
                   pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback4.WaitForResult(), IsOk());
  EXPECT_FALSE(callback3.have_result());

  // Closing a handle should free up another socket slot.
  handle1.Reset();
  EXPECT_THAT(callback3.WaitForResult(), IsOk());
}

TEST_F(ClientSocketPoolBaseTest,
       CloseMultipleIdleSocketsHeldByLayeredPoolWhenNeeded) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  MockLayeredPool mock_layered_pool1(pool_.get(), TestGroupId("foo"));
  EXPECT_THAT(mock_layered_pool1.RequestSocket(pool_.get()), IsOk());
  EXPECT_CALL(mock_layered_pool1, CloseOneIdleConnection())
      .WillRepeatedly(Invoke(&mock_layered_pool1,
                             &MockLayeredPool::ReleaseOneConnection));
  MockLayeredPool mock_layered_pool2(pool_.get(), TestGroupId("bar"));
  EXPECT_THAT(mock_layered_pool2.RequestSocketWithoutLimits(pool_.get()),
              IsOk());
  EXPECT_CALL(mock_layered_pool2, CloseOneIdleConnection())
      .WillRepeatedly(Invoke(&mock_layered_pool2,
                             &MockLayeredPool::ReleaseOneConnection));
  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));
  EXPECT_THAT(callback.WaitForResult(), IsOk());
}

// Test that when a socket pool and group are at their limits, a request
// with RespectLimits::DISABLED triggers creation of a new socket, and gets the
// socket instead of a request with the same priority that was issued earlier,
// but has RespectLimits::ENABLED.
TEST_F(ClientSocketPoolBaseTest, IgnoreLimits) {
  CreatePool(1, 1);

  // Issue a request to reach the socket pool limit.
  EXPECT_EQ(OK, StartRequestWithIgnoreLimits(
                    TestGroupId("a"), MAXIMUM_PRIORITY,
                    ClientSocketPool::RespectLimits::ENABLED));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  EXPECT_EQ(ERR_IO_PENDING, StartRequestWithIgnoreLimits(
                                TestGroupId("a"), MAXIMUM_PRIORITY,
                                ClientSocketPool::RespectLimits::ENABLED));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Issue a request that ignores the limits, so a new ConnectJob is
  // created.
  EXPECT_EQ(ERR_IO_PENDING, StartRequestWithIgnoreLimits(
                                TestGroupId("a"), MAXIMUM_PRIORITY,
                                ClientSocketPool::RespectLimits::DISABLED));
  ASSERT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  EXPECT_THAT(request(2)->WaitForResult(), IsOk());
  EXPECT_FALSE(request(1)->have_result());
}

// Test that when a socket pool and group are at their limits, a ConnectJob
// issued for a request with RespectLimits::DISABLED is not cancelled when a
// request with RespectLimits::ENABLED issued to the same group is cancelled.
TEST_F(ClientSocketPoolBaseTest, IgnoreLimitsCancelOtherJob) {
  CreatePool(1, 1);

  // Issue a request to reach the socket pool limit.
  EXPECT_EQ(OK, StartRequestWithIgnoreLimits(
                    TestGroupId("a"), MAXIMUM_PRIORITY,
                    ClientSocketPool::RespectLimits::ENABLED));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);

  EXPECT_EQ(ERR_IO_PENDING, StartRequestWithIgnoreLimits(
                                TestGroupId("a"), MAXIMUM_PRIORITY,
                                ClientSocketPool::RespectLimits::ENABLED));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Issue a request with RespectLimits::DISABLED, so a new ConnectJob is
  // created.
  EXPECT_EQ(ERR_IO_PENDING, StartRequestWithIgnoreLimits(
                                TestGroupId("a"), MAXIMUM_PRIORITY,
                                ClientSocketPool::RespectLimits::DISABLED));
  ASSERT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Cancel the pending request with RespectLimits::ENABLED. The ConnectJob
  // should not be cancelled.
  request(1)->handle()->Reset();
  ASSERT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  EXPECT_THAT(request(2)->WaitForResult(), IsOk());
  EXPECT_FALSE(request(1)->have_result());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthNoAuthCallback) {
  CreatePool(1, 1);

  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_EQ(
      ERR_IO_PENDING,
      handle.Init(TestGroupId("a"), params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()));

  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  EXPECT_THAT(callback.WaitForResult(), IsError(ERR_PROXY_AUTH_REQUESTED));
  EXPECT_FALSE(handle.is_initialized());
  EXPECT_FALSE(handle.socket());

  // The group should now be empty, and thus be deleted.
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
}

class TestAuthHelper {
 public:
  TestAuthHelper() = default;
  ~TestAuthHelper() = default;

  void InitHandle(
      scoped_refptr<ClientSocketPool::SocketParams> params,
      TransportClientSocketPool* pool,
      RequestPriority priority = DEFAULT_PRIORITY,
      ClientSocketPool::RespectLimits respect_limits =
          ClientSocketPool::RespectLimits::ENABLED,
      const ClientSocketPool::GroupId& group_id_in = TestGroupId("a")) {
    EXPECT_EQ(ERR_IO_PENDING,
              handle_.Init(group_id_in, params, base::nullopt, priority,
                           SocketTag(), respect_limits, callback_.callback(),
                           base::BindRepeating(&TestAuthHelper::AuthCallback,
                                               base::Unretained(this)),
                           pool, NetLogWithSource()));
  }

  void WaitForAuth() {
    run_loop_ = std::make_unique<base::RunLoop>();
    run_loop_->Run();
    run_loop_.reset();
  }

  void WaitForAuthAndRestartSync() {
    restart_sync_ = true;
    WaitForAuth();
    restart_sync_ = false;
  }

  void WaitForAuthAndResetHandleSync() {
    reset_handle_sync_ = true;
    WaitForAuth();
    reset_handle_sync_ = false;
  }

  void RestartWithAuth() {
    DCHECK(restart_with_auth_callback_);
    std::move(restart_with_auth_callback_).Run();
  }

  int WaitForResult() {
    int result = callback_.WaitForResult();
    // There shouldn't be any callback waiting to be invoked once the request is
    // complete.
    EXPECT_FALSE(restart_with_auth_callback_);
    // The socket should only be initialized on success.
    EXPECT_EQ(result == OK, handle_.is_initialized());
    EXPECT_EQ(result == OK, handle_.socket() != nullptr);
    return result;
  }

  ClientSocketHandle* handle() { return &handle_; }
  int auth_count() const { return auth_count_; }
  int have_result() const { return callback_.have_result(); }

 private:
  void AuthCallback(const HttpResponseInfo& response,
                    HttpAuthController* auth_controller,
                    base::OnceClosure restart_with_auth_callback) {
    EXPECT_FALSE(restart_with_auth_callback_);
    EXPECT_TRUE(restart_with_auth_callback);

    // Once there's a result, this method shouldn't be invoked again.
    EXPECT_FALSE(callback_.have_result());

    ++auth_count_;
    run_loop_->Quit();
    if (restart_sync_) {
      std::move(restart_with_auth_callback).Run();
      return;
    }

    restart_with_auth_callback_ = std::move(restart_with_auth_callback);

    if (reset_handle_sync_) {
      handle_.Reset();
      return;
    }
  }

  std::unique_ptr<base::RunLoop> run_loop_;
  base::OnceClosure restart_with_auth_callback_;

  bool restart_sync_ = false;
  bool reset_handle_sync_ = false;

  ClientSocketHandle handle_;
  int auth_count_ = 0;
  TestCompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(TestAuthHelper);
};

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnce) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.WaitForAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.RestartWithAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  EXPECT_THAT(auth_helper.WaitForResult(), IsOk());
  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceSync) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.WaitForAuthAndRestartSync();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  EXPECT_THAT(auth_helper.WaitForResult(), IsOk());
  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceFails) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeOnceFailingJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuth();
  auth_helper.RestartWithAuth();
  EXPECT_THAT(auth_helper.WaitForResult(), IsError(ERR_CONNECTION_FAILED));

  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceSyncFails) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeOnceFailingJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuthAndRestartSync();
  EXPECT_THAT(auth_helper.WaitForResult(), IsError(ERR_CONNECTION_FAILED));

  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceDeleteHandle) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.handle()->Reset();

  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_FALSE(auth_helper.handle()->is_initialized());
  EXPECT_FALSE(auth_helper.handle()->socket());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceDeleteHandleSync) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuthAndResetHandleSync();
  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_FALSE(auth_helper.handle()->is_initialized());
  EXPECT_FALSE(auth_helper.handle()->socket());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthOnceFlushWithError) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuth();

  pool_->FlushWithError(ERR_FAILED);
  base::RunLoop().RunUntilIdle();

  // When flushing the socket pool, bound sockets should delay returning the
  // error until completion.
  EXPECT_FALSE(auth_helper.have_result());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());

  auth_helper.RestartWithAuth();
  // The callback should be called asynchronously.
  EXPECT_FALSE(auth_helper.have_result());

  EXPECT_THAT(auth_helper.WaitForResult(), IsError(ERR_FAILED));
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthTwice) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeTwiceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(LOAD_STATE_CONNECTING,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.WaitForAuth();
  auth_helper.RestartWithAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, auth_helper.auth_count());
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.WaitForAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2, auth_helper.auth_count());
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  auth_helper.RestartWithAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2, auth_helper.auth_count());
  EXPECT_EQ(LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,
            pool_->GetLoadState(TestGroupId("a"), auth_helper.handle()));

  EXPECT_THAT(auth_helper.WaitForResult(), IsOk());
  EXPECT_EQ(2, auth_helper.auth_count());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

TEST_F(ClientSocketPoolBaseTest, ProxyAuthTwiceFails) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeTwiceFailingJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  auth_helper.WaitForAuth();
  auth_helper.RestartWithAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, auth_helper.auth_count());

  auth_helper.WaitForAuth();
  auth_helper.RestartWithAuth();
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(2, auth_helper.auth_count());

  EXPECT_THAT(auth_helper.WaitForResult(), IsError(ERR_CONNECTION_FAILED));
  EXPECT_EQ(2, auth_helper.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->IdleSocketCount());
}

// Makes sure that when a bound request is destroyed, a new ConnectJob is
// created, if needed.
TEST_F(ClientSocketPoolBaseTest,
       ProxyAuthCreateNewConnectJobOnDestroyBoundRequest) {
  CreatePool(1 /* max_sockets */, 1 /* max_sockets_per_group */);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeOnceFailingJob);

  // First request creates a ConnectJob.
  TestAuthHelper auth_helper1;
  auth_helper1.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // A second request come in, but no new ConnectJob is needed, since the limit
  // has been reached.
  TestAuthHelper auth_helper2;
  auth_helper2.InitHandle(params_, pool_.get());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Run until the auth callback for the first request is invoked.
  auth_helper1.WaitForAuth();
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // Make connect jobs succeed, then cancel the first request, which should
  // destroy the bound ConnectJob, and cause a new ConnectJob to start.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  auth_helper1.handle()->Reset();
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // The second ConnectJob should succeed.
  EXPECT_THAT(auth_helper2.WaitForResult(), IsOk());
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
}

// Makes sure that when a bound request is destroyed, a new ConnectJob is
// created for another group, if needed.
TEST_F(ClientSocketPoolBaseTest,
       ProxyAuthCreateNewConnectJobOnDestroyBoundRequestDifferentGroups) {
  CreatePool(1 /* max_sockets */, 1 /* max_sockets_per_group */);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeOnceFailingJob);

  // First request creates a ConnectJob.
  TestAuthHelper auth_helper1;
  auth_helper1.InitHandle(params_, pool_.get(), DEFAULT_PRIORITY);
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // A second request come in, but no new ConnectJob is needed, since the limit
  // has been reached.
  TestAuthHelper auth_helper2;
  auth_helper2.InitHandle(params_, pool_.get(), DEFAULT_PRIORITY,
                          ClientSocketPool::RespectLimits::ENABLED,
                          TestGroupId("b"));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));

  // Run until the auth callback for the first request is invoked.
  auth_helper1.WaitForAuth();
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("b")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("b")));

  // Make connect jobs succeed, then cancel the first request, which should
  // destroy the bound ConnectJob, and cause a new ConnectJob to start for the
  // other group.
  connect_job_factory_->set_job_type(TestConnectJob::kMockPendingJob);
  auth_helper1.handle()->Reset();
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));

  // The second ConnectJob should succeed.
  EXPECT_THAT(auth_helper2.WaitForResult(), IsOk());
  EXPECT_EQ(0, auth_helper2.auth_count());
  EXPECT_FALSE(pool_->HasGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("b")));
}

// Test that once an auth challenge is bound, that's the request that gets all
// subsequent calls and the socket itself.
TEST_F(ClientSocketPoolBaseTest, ProxyAuthStaysBound) {
  CreatePool(1, 1);
  connect_job_factory_->set_job_type(
      TestConnectJob::kMockAuthChallengeTwiceJob);

  // First request creates a ConnectJob.
  TestAuthHelper auth_helper1;
  auth_helper1.InitHandle(params_, pool_.get(), LOWEST);
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // A second, higher priority request is made.
  TestAuthHelper auth_helper2;
  auth_helper2.InitHandle(params_, pool_.get(), LOW);
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Run until the auth callback for the second request is invoked.
  auth_helper2.WaitForAuth();
  EXPECT_EQ(0, auth_helper1.auth_count());
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(TestGroupId("a")));

  // Start a higher priority job. It shouldn't be able to steal |auth_helper2|'s
  // ConnectJob.
  TestAuthHelper auth_helper3;
  auth_helper3.InitHandle(params_, pool_.get(), HIGHEST);
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Start a higher job that ignores limits, creating a hanging socket. It
  // shouldn't be able to steal |auth_helper2|'s ConnectJob.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);
  TestAuthHelper auth_helper4;
  auth_helper4.InitHandle(params_, pool_.get(), HIGHEST,
                          ClientSocketPool::RespectLimits::DISABLED);
  EXPECT_EQ(2u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));

  // Restart with auth, and |auth_helper2|'s auth method should be invoked
  // again.
  auth_helper2.RestartWithAuth();
  auth_helper2.WaitForAuth();
  EXPECT_EQ(0, auth_helper1.auth_count());
  EXPECT_FALSE(auth_helper1.have_result());
  EXPECT_EQ(2, auth_helper2.auth_count());
  EXPECT_FALSE(auth_helper2.have_result());
  EXPECT_EQ(0, auth_helper3.auth_count());
  EXPECT_FALSE(auth_helper3.have_result());
  EXPECT_EQ(0, auth_helper4.auth_count());
  EXPECT_FALSE(auth_helper4.have_result());

  // Advance auth again, and |auth_helper2| should get the socket.
  auth_helper2.RestartWithAuth();
  EXPECT_THAT(auth_helper2.WaitForResult(), IsOk());
  // The hung ConnectJob for the RespectLimits::DISABLED request is still in the
  // socket pool.
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(TestGroupId("a")));
  EXPECT_EQ(0, auth_helper1.auth_count());
  EXPECT_FALSE(auth_helper1.have_result());
  EXPECT_EQ(0, auth_helper3.auth_count());
  EXPECT_FALSE(auth_helper3.have_result());
  EXPECT_EQ(0, auth_helper4.auth_count());
  EXPECT_FALSE(auth_helper4.have_result());

  // If the socket is returned to the socket pool, the RespectLimits::DISABLED
  // socket request should be able to claim it.
  auth_helper2.handle()->Reset();
  EXPECT_THAT(auth_helper4.WaitForResult(), IsOk());
  EXPECT_EQ(0, auth_helper1.auth_count());
  EXPECT_FALSE(auth_helper1.have_result());
  EXPECT_EQ(0, auth_helper3.auth_count());
  EXPECT_FALSE(auth_helper3.have_result());
  EXPECT_EQ(0, auth_helper4.auth_count());
}

TEST_F(ClientSocketPoolBaseTest, RefreshGroupCreatesNewConnectJobs) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");

  // First job will be waiting until it gets aborted.
  connect_job_factory_->set_job_type(TestConnectJob::kMockWaitingJob);

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_THAT(
      handle.Init(kGroupId, params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()),
      IsError(ERR_IO_PENDING));

  // Switch connect job types, so creating a new ConnectJob will result in
  // success.
  connect_job_factory_->set_job_type(TestConnectJob::kMockJob);

  pool_->RefreshGroupForTesting(kGroupId);
  EXPECT_EQ(OK, callback.WaitForResult());
  ASSERT_TRUE(handle.socket());
  EXPECT_EQ(0, pool_->IdleSocketCount());
  ASSERT_TRUE(pool_->HasGroupForTesting(kGroupId));
  EXPECT_EQ(0u, pool_->IdleSocketCountInGroup(kGroupId));
  EXPECT_EQ(0u, pool_->NumConnectJobsInGroupForTesting(kGroupId));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(kGroupId));
}

TEST_F(ClientSocketPoolBaseTest, RefreshGroupClosesIdleConnectJobs) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");

  pool_->RequestSockets(kGroupId, params_, base::nullopt, 2,
                        NetLogWithSource());
  ASSERT_TRUE(pool_->HasGroupForTesting(kGroupId));
  EXPECT_EQ(2, pool_->IdleSocketCount());
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(kGroupId));

  pool_->RefreshGroupForTesting(kGroupId);
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_FALSE(pool_->HasGroupForTesting(kGroupId));
}

TEST_F(ClientSocketPoolBaseTest,
       RefreshGroupDoesNotCloseIdleConnectJobsInOtherGroup) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");
  const ClientSocketPool::GroupId kOtherGroupId = TestGroupId("b");

  pool_->RequestSockets(kOtherGroupId, params_, base::nullopt, 2,
                        NetLogWithSource());
  ASSERT_TRUE(pool_->HasGroupForTesting(kOtherGroupId));
  EXPECT_EQ(2, pool_->IdleSocketCount());
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(kOtherGroupId));

  pool_->RefreshGroupForTesting(kGroupId);
  ASSERT_TRUE(pool_->HasGroupForTesting(kOtherGroupId));
  EXPECT_EQ(2, pool_->IdleSocketCount());
  EXPECT_EQ(2u, pool_->IdleSocketCountInGroup(kOtherGroupId));
}

TEST_F(ClientSocketPoolBaseTest, RefreshGroupPreventsSocketReuse) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_THAT(
      handle.Init(kGroupId, params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()),
      IsOk());
  ASSERT_TRUE(pool_->HasGroupForTesting(kGroupId));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(kGroupId));

  pool_->RefreshGroupForTesting(kGroupId);

  handle.Reset();
  EXPECT_EQ(0, pool_->IdleSocketCount());
  EXPECT_FALSE(pool_->HasGroupForTesting(kGroupId));
}

TEST_F(ClientSocketPoolBaseTest,
       RefreshGroupDoesNotPreventSocketReuseInOtherGroup) {
  CreatePool(kDefaultMaxSockets, kDefaultMaxSocketsPerGroup);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");
  const ClientSocketPool::GroupId kOtherGroupId = TestGroupId("b");

  ClientSocketHandle handle;
  TestCompletionCallback callback;
  EXPECT_THAT(
      handle.Init(kOtherGroupId, params_, base::nullopt, DEFAULT_PRIORITY,
                  SocketTag(), ClientSocketPool::RespectLimits::ENABLED,
                  callback.callback(), ClientSocketPool::ProxyAuthCallback(),
                  pool_.get(), NetLogWithSource()),
      IsOk());
  ASSERT_TRUE(pool_->HasGroupForTesting(kOtherGroupId));
  EXPECT_EQ(1, pool_->NumActiveSocketsInGroupForTesting(kOtherGroupId));

  pool_->RefreshGroupForTesting(kGroupId);

  handle.Reset();
  EXPECT_EQ(1, pool_->IdleSocketCount());
  ASSERT_TRUE(pool_->HasGroupForTesting(kOtherGroupId));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(kOtherGroupId));
}

TEST_F(ClientSocketPoolBaseTest, RefreshGroupReplacesBoundConnectJobOnConnect) {
  CreatePool(1, 1);
  const ClientSocketPool::GroupId kGroupId = TestGroupId("a");
  connect_job_factory_->set_job_type(TestConnectJob::kMockAuthChallengeOnceJob);

  TestAuthHelper auth_helper;
  auth_helper.InitHandle(params_, pool_.get(), DEFAULT_PRIORITY,
                         ClientSocketPool::RespectLimits::ENABLED, kGroupId);
  EXPECT_EQ(1u, pool_->NumConnectJobsInGroupForTesting(kGroupId));

  auth_helper.WaitForAuth();

  // This should update the generation, but not cancel the old ConnectJob - it's
  // not safe to do anything while waiting on the original ConnectJob.
  pool_->RefreshGroupForTesting(kGroupId);

  // Providing auth credentials and restarting the request with them will cause
  // the ConnectJob to complete successfully, but the result will be discarded
  // because of the generation mismatch.
  auth_helper.RestartWithAuth();

  // Despite using ConnectJobs that simulate a single challenge, a second
  // challenge will be seen, due to using a new ConnectJob.
  auth_helper.WaitForAuth();
  auth_helper.RestartWithAuth();

  EXPECT_THAT(auth_helper.WaitForResult(), IsOk());
  EXPECT_TRUE(auth_helper.handle()->socket());
  EXPECT_EQ(2, auth_helper.auth_count());

  // When released, the socket will be returned to the socket pool, and
  // available for reuse.
  auth_helper.handle()->Reset();
  EXPECT_EQ(1, pool_->IdleSocketCount());
  ASSERT_TRUE(pool_->HasGroupForTesting(kGroupId));
  EXPECT_EQ(1u, pool_->IdleSocketCountInGroup(kGroupId));
}

}  // namespace

}  // namespace net
