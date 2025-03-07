// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/trace/chrome/chrome_trace_packet.proto

#include "perfetto/trace/chrome/chrome_trace_packet.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace protobuf_perfetto_2fcommon_2ftrace_5fstats_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fcommon_2ftrace_5fstats_2eproto ::google::protobuf::internal::SCCInfo<1> scc_info_TraceStats;
}  // namespace protobuf_perfetto_2fcommon_2ftrace_5fstats_2eproto
namespace protobuf_perfetto_2fconfig_2ftrace_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2ftrace_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<7> scc_info_TraceConfig;
}  // namespace protobuf_perfetto_2fconfig_2ftrace_5fconfig_2eproto
namespace protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fevent_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fevent_2eproto ::google::protobuf::internal::SCCInfo<4> scc_info_ChromeEventBundle;
}  // namespace protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fevent_2eproto
namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto ::google::protobuf::internal::SCCInfo<1> scc_info_ClockSnapshot;
}  // namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto
namespace protobuf_perfetto_2ftrace_2finterned_5fdata_2finterned_5fdata_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2finterned_5fdata_2finterned_5fdata_2eproto ::google::protobuf::internal::SCCInfo<4> scc_info_InternedData;
}  // namespace protobuf_perfetto_2ftrace_2finterned_5fdata_2finterned_5fdata_2eproto
namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2fprocess_5fdescriptor_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2ftrack_5fevent_2fprocess_5fdescriptor_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ProcessDescriptor;
}  // namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2fprocess_5fdescriptor_2eproto
namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2fthread_5fdescriptor_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2ftrack_5fevent_2fthread_5fdescriptor_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ThreadDescriptor;
}  // namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2fthread_5fdescriptor_2eproto
namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2ftrack_5fevent_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2ftrack_5fevent_2ftrack_5fevent_2eproto ::google::protobuf::internal::SCCInfo<3> scc_info_TrackEvent;
}  // namespace protobuf_perfetto_2ftrace_2ftrack_5fevent_2ftrack_5fevent_2eproto
namespace perfetto {
namespace protos {
class ChromeTracePacketDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ChromeTracePacket>
      _instance;
} _ChromeTracePacket_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fpacket_2eproto {
static void InitDefaultsChromeTracePacket() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perfetto::protos::_ChromeTracePacket_default_instance_;
    new (ptr) ::perfetto::protos::ChromeTracePacket();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perfetto::protos::ChromeTracePacket::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<8> scc_info_ChromeTracePacket =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 8, InitDefaultsChromeTracePacket}, {
      &protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fevent_2eproto::scc_info_ChromeEventBundle.base,
      &protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot.base,
      &protobuf_perfetto_2ftrace_2ftrack_5fevent_2ftrack_5fevent_2eproto::scc_info_TrackEvent.base,
      &protobuf_perfetto_2ftrace_2ftrack_5fevent_2fprocess_5fdescriptor_2eproto::scc_info_ProcessDescriptor.base,
      &protobuf_perfetto_2ftrace_2ftrack_5fevent_2fthread_5fdescriptor_2eproto::scc_info_ThreadDescriptor.base,
      &protobuf_perfetto_2fconfig_2ftrace_5fconfig_2eproto::scc_info_TraceConfig.base,
      &protobuf_perfetto_2fcommon_2ftrace_5fstats_2eproto::scc_info_TraceStats.base,
      &protobuf_perfetto_2ftrace_2finterned_5fdata_2finterned_5fdata_2eproto::scc_info_InternedData.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_ChromeTracePacket.base);
}

}  // namespace protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fpacket_2eproto
namespace perfetto {
namespace protos {

// ===================================================================

void ChromeTracePacket::InitAsDefaultInstance() {
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->chrome_events_ = const_cast< ::perfetto::protos::ChromeEventBundle*>(
      ::perfetto::protos::ChromeEventBundle::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->clock_snapshot_ = const_cast< ::perfetto::protos::ClockSnapshot*>(
      ::perfetto::protos::ClockSnapshot::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->track_event_ = const_cast< ::perfetto::protos::TrackEvent*>(
      ::perfetto::protos::TrackEvent::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->process_descriptor_ = const_cast< ::perfetto::protos::ProcessDescriptor*>(
      ::perfetto::protos::ProcessDescriptor::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->thread_descriptor_ = const_cast< ::perfetto::protos::ThreadDescriptor*>(
      ::perfetto::protos::ThreadDescriptor::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->trace_config_ = const_cast< ::perfetto::protos::TraceConfig*>(
      ::perfetto::protos::TraceConfig::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->trace_stats_ = const_cast< ::perfetto::protos::TraceStats*>(
      ::perfetto::protos::TraceStats::internal_default_instance());
  ::perfetto::protos::_ChromeTracePacket_default_instance_._instance.get_mutable()->interned_data_ = const_cast< ::perfetto::protos::InternedData*>(
      ::perfetto::protos::InternedData::internal_default_instance());
}
void ChromeTracePacket::clear_chrome_events() {
  if (GetArenaNoVirtual() == NULL && chrome_events_ != NULL) {
    delete chrome_events_;
  }
  chrome_events_ = NULL;
}
void ChromeTracePacket::clear_clock_snapshot() {
  if (GetArenaNoVirtual() == NULL && clock_snapshot_ != NULL) {
    delete clock_snapshot_;
  }
  clock_snapshot_ = NULL;
}
void ChromeTracePacket::clear_track_event() {
  if (GetArenaNoVirtual() == NULL && track_event_ != NULL) {
    delete track_event_;
  }
  track_event_ = NULL;
}
void ChromeTracePacket::clear_process_descriptor() {
  if (GetArenaNoVirtual() == NULL && process_descriptor_ != NULL) {
    delete process_descriptor_;
  }
  process_descriptor_ = NULL;
}
void ChromeTracePacket::clear_thread_descriptor() {
  if (GetArenaNoVirtual() == NULL && thread_descriptor_ != NULL) {
    delete thread_descriptor_;
  }
  thread_descriptor_ = NULL;
}
void ChromeTracePacket::clear_trace_config() {
  if (GetArenaNoVirtual() == NULL && trace_config_ != NULL) {
    delete trace_config_;
  }
  trace_config_ = NULL;
}
void ChromeTracePacket::clear_trace_stats() {
  if (GetArenaNoVirtual() == NULL && trace_stats_ != NULL) {
    delete trace_stats_;
  }
  trace_stats_ = NULL;
}
void ChromeTracePacket::clear_interned_data() {
  if (GetArenaNoVirtual() == NULL && interned_data_ != NULL) {
    delete interned_data_;
  }
  interned_data_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ChromeTracePacket::kChromeEventsFieldNumber;
const int ChromeTracePacket::kClockSnapshotFieldNumber;
const int ChromeTracePacket::kTrackEventFieldNumber;
const int ChromeTracePacket::kProcessDescriptorFieldNumber;
const int ChromeTracePacket::kThreadDescriptorFieldNumber;
const int ChromeTracePacket::kTraceConfigFieldNumber;
const int ChromeTracePacket::kTraceStatsFieldNumber;
const int ChromeTracePacket::kTrustedPacketSequenceIdFieldNumber;
const int ChromeTracePacket::kInternedDataFieldNumber;
const int ChromeTracePacket::kIncrementalStateClearedFieldNumber;
const int ChromeTracePacket::kPreviousPacketDroppedFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ChromeTracePacket::ChromeTracePacket()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fpacket_2eproto::scc_info_ChromeTracePacket.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:perfetto.protos.ChromeTracePacket)
}
ChromeTracePacket::ChromeTracePacket(const ChromeTracePacket& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_chrome_events()) {
    chrome_events_ = new ::perfetto::protos::ChromeEventBundle(*from.chrome_events_);
  } else {
    chrome_events_ = NULL;
  }
  if (from.has_clock_snapshot()) {
    clock_snapshot_ = new ::perfetto::protos::ClockSnapshot(*from.clock_snapshot_);
  } else {
    clock_snapshot_ = NULL;
  }
  if (from.has_track_event()) {
    track_event_ = new ::perfetto::protos::TrackEvent(*from.track_event_);
  } else {
    track_event_ = NULL;
  }
  if (from.has_interned_data()) {
    interned_data_ = new ::perfetto::protos::InternedData(*from.interned_data_);
  } else {
    interned_data_ = NULL;
  }
  if (from.has_trace_config()) {
    trace_config_ = new ::perfetto::protos::TraceConfig(*from.trace_config_);
  } else {
    trace_config_ = NULL;
  }
  if (from.has_trace_stats()) {
    trace_stats_ = new ::perfetto::protos::TraceStats(*from.trace_stats_);
  } else {
    trace_stats_ = NULL;
  }
  if (from.has_process_descriptor()) {
    process_descriptor_ = new ::perfetto::protos::ProcessDescriptor(*from.process_descriptor_);
  } else {
    process_descriptor_ = NULL;
  }
  if (from.has_thread_descriptor()) {
    thread_descriptor_ = new ::perfetto::protos::ThreadDescriptor(*from.thread_descriptor_);
  } else {
    thread_descriptor_ = NULL;
  }
  ::memcpy(&trusted_packet_sequence_id_, &from.trusted_packet_sequence_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&previous_packet_dropped_) -
    reinterpret_cast<char*>(&trusted_packet_sequence_id_)) + sizeof(previous_packet_dropped_));
  // @@protoc_insertion_point(copy_constructor:perfetto.protos.ChromeTracePacket)
}

void ChromeTracePacket::SharedCtor() {
  ::memset(&chrome_events_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&previous_packet_dropped_) -
      reinterpret_cast<char*>(&chrome_events_)) + sizeof(previous_packet_dropped_));
}

ChromeTracePacket::~ChromeTracePacket() {
  // @@protoc_insertion_point(destructor:perfetto.protos.ChromeTracePacket)
  SharedDtor();
}

void ChromeTracePacket::SharedDtor() {
  if (this != internal_default_instance()) delete chrome_events_;
  if (this != internal_default_instance()) delete clock_snapshot_;
  if (this != internal_default_instance()) delete track_event_;
  if (this != internal_default_instance()) delete interned_data_;
  if (this != internal_default_instance()) delete trace_config_;
  if (this != internal_default_instance()) delete trace_stats_;
  if (this != internal_default_instance()) delete process_descriptor_;
  if (this != internal_default_instance()) delete thread_descriptor_;
}

void ChromeTracePacket::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ChromeTracePacket& ChromeTracePacket::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_perfetto_2ftrace_2fchrome_2fchrome_5ftrace_5fpacket_2eproto::scc_info_ChromeTracePacket.base);
  return *internal_default_instance();
}


void ChromeTracePacket::Clear() {
// @@protoc_insertion_point(message_clear_start:perfetto.protos.ChromeTracePacket)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaNoVirtual() == NULL && chrome_events_ != NULL) {
    delete chrome_events_;
  }
  chrome_events_ = NULL;
  if (GetArenaNoVirtual() == NULL && clock_snapshot_ != NULL) {
    delete clock_snapshot_;
  }
  clock_snapshot_ = NULL;
  if (GetArenaNoVirtual() == NULL && track_event_ != NULL) {
    delete track_event_;
  }
  track_event_ = NULL;
  if (GetArenaNoVirtual() == NULL && interned_data_ != NULL) {
    delete interned_data_;
  }
  interned_data_ = NULL;
  if (GetArenaNoVirtual() == NULL && trace_config_ != NULL) {
    delete trace_config_;
  }
  trace_config_ = NULL;
  if (GetArenaNoVirtual() == NULL && trace_stats_ != NULL) {
    delete trace_stats_;
  }
  trace_stats_ = NULL;
  if (GetArenaNoVirtual() == NULL && process_descriptor_ != NULL) {
    delete process_descriptor_;
  }
  process_descriptor_ = NULL;
  if (GetArenaNoVirtual() == NULL && thread_descriptor_ != NULL) {
    delete thread_descriptor_;
  }
  thread_descriptor_ = NULL;
  ::memset(&trusted_packet_sequence_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&previous_packet_dropped_) -
      reinterpret_cast<char*>(&trusted_packet_sequence_id_)) + sizeof(previous_packet_dropped_));
  _internal_metadata_.Clear();
}

bool ChromeTracePacket::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:perfetto.protos.ChromeTracePacket)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(16383u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // .perfetto.protos.ChromeEventBundle chrome_events = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u /* 42 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_chrome_events()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.ClockSnapshot clock_snapshot = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(50u /* 50 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_clock_snapshot()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 trusted_packet_sequence_id = 10;
      case 10: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(80u /* 80 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &trusted_packet_sequence_id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.TrackEvent track_event = 11;
      case 11: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(90u /* 90 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_track_event()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.InternedData interned_data = 12;
      case 12: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(98u /* 98 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_interned_data()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.TraceConfig trace_config = 33;
      case 33: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 266 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_trace_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.TraceStats trace_stats = 35;
      case 35: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u /* 282 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_trace_stats()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool incremental_state_cleared = 41;
      case 41: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(72u /* 328 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &incremental_state_cleared_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool previous_packet_dropped = 42;
      case 42: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(80u /* 336 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &previous_packet_dropped_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.ProcessDescriptor process_descriptor = 43;
      case 43: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(90u /* 346 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_process_descriptor()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .perfetto.protos.ThreadDescriptor thread_descriptor = 44;
      case 44: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(98u /* 354 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_thread_descriptor()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:perfetto.protos.ChromeTracePacket)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:perfetto.protos.ChromeTracePacket)
  return false;
#undef DO_
}

void ChromeTracePacket::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:perfetto.protos.ChromeTracePacket)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .perfetto.protos.ChromeEventBundle chrome_events = 5;
  if (this->has_chrome_events()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      5, this->_internal_chrome_events(), output);
  }

  // .perfetto.protos.ClockSnapshot clock_snapshot = 6;
  if (this->has_clock_snapshot()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      6, this->_internal_clock_snapshot(), output);
  }

  // uint32 trusted_packet_sequence_id = 10;
  if (this->trusted_packet_sequence_id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(10, this->trusted_packet_sequence_id(), output);
  }

  // .perfetto.protos.TrackEvent track_event = 11;
  if (this->has_track_event()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      11, this->_internal_track_event(), output);
  }

  // .perfetto.protos.InternedData interned_data = 12;
  if (this->has_interned_data()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      12, this->_internal_interned_data(), output);
  }

  // .perfetto.protos.TraceConfig trace_config = 33;
  if (this->has_trace_config()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      33, this->_internal_trace_config(), output);
  }

  // .perfetto.protos.TraceStats trace_stats = 35;
  if (this->has_trace_stats()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      35, this->_internal_trace_stats(), output);
  }

  // bool incremental_state_cleared = 41;
  if (this->incremental_state_cleared() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(41, this->incremental_state_cleared(), output);
  }

  // bool previous_packet_dropped = 42;
  if (this->previous_packet_dropped() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(42, this->previous_packet_dropped(), output);
  }

  // .perfetto.protos.ProcessDescriptor process_descriptor = 43;
  if (this->has_process_descriptor()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      43, this->_internal_process_descriptor(), output);
  }

  // .perfetto.protos.ThreadDescriptor thread_descriptor = 44;
  if (this->has_thread_descriptor()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      44, this->_internal_thread_descriptor(), output);
  }

  output->WriteRaw((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).data(),
                   static_cast<int>((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size()));
  // @@protoc_insertion_point(serialize_end:perfetto.protos.ChromeTracePacket)
}

size_t ChromeTracePacket::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perfetto.protos.ChromeTracePacket)
  size_t total_size = 0;

  total_size += (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size();

  // .perfetto.protos.ChromeEventBundle chrome_events = 5;
  if (this->has_chrome_events()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *chrome_events_);
  }

  // .perfetto.protos.ClockSnapshot clock_snapshot = 6;
  if (this->has_clock_snapshot()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *clock_snapshot_);
  }

  // .perfetto.protos.TrackEvent track_event = 11;
  if (this->has_track_event()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *track_event_);
  }

  // .perfetto.protos.InternedData interned_data = 12;
  if (this->has_interned_data()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *interned_data_);
  }

  // .perfetto.protos.TraceConfig trace_config = 33;
  if (this->has_trace_config()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *trace_config_);
  }

  // .perfetto.protos.TraceStats trace_stats = 35;
  if (this->has_trace_stats()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *trace_stats_);
  }

  // .perfetto.protos.ProcessDescriptor process_descriptor = 43;
  if (this->has_process_descriptor()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *process_descriptor_);
  }

  // .perfetto.protos.ThreadDescriptor thread_descriptor = 44;
  if (this->has_thread_descriptor()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *thread_descriptor_);
  }

  // uint32 trusted_packet_sequence_id = 10;
  if (this->trusted_packet_sequence_id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->trusted_packet_sequence_id());
  }

  // bool incremental_state_cleared = 41;
  if (this->incremental_state_cleared() != 0) {
    total_size += 2 + 1;
  }

  // bool previous_packet_dropped = 42;
  if (this->previous_packet_dropped() != 0) {
    total_size += 2 + 1;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ChromeTracePacket::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ChromeTracePacket*>(&from));
}

void ChromeTracePacket::MergeFrom(const ChromeTracePacket& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perfetto.protos.ChromeTracePacket)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_chrome_events()) {
    mutable_chrome_events()->::perfetto::protos::ChromeEventBundle::MergeFrom(from.chrome_events());
  }
  if (from.has_clock_snapshot()) {
    mutable_clock_snapshot()->::perfetto::protos::ClockSnapshot::MergeFrom(from.clock_snapshot());
  }
  if (from.has_track_event()) {
    mutable_track_event()->::perfetto::protos::TrackEvent::MergeFrom(from.track_event());
  }
  if (from.has_interned_data()) {
    mutable_interned_data()->::perfetto::protos::InternedData::MergeFrom(from.interned_data());
  }
  if (from.has_trace_config()) {
    mutable_trace_config()->::perfetto::protos::TraceConfig::MergeFrom(from.trace_config());
  }
  if (from.has_trace_stats()) {
    mutable_trace_stats()->::perfetto::protos::TraceStats::MergeFrom(from.trace_stats());
  }
  if (from.has_process_descriptor()) {
    mutable_process_descriptor()->::perfetto::protos::ProcessDescriptor::MergeFrom(from.process_descriptor());
  }
  if (from.has_thread_descriptor()) {
    mutable_thread_descriptor()->::perfetto::protos::ThreadDescriptor::MergeFrom(from.thread_descriptor());
  }
  if (from.trusted_packet_sequence_id() != 0) {
    set_trusted_packet_sequence_id(from.trusted_packet_sequence_id());
  }
  if (from.incremental_state_cleared() != 0) {
    set_incremental_state_cleared(from.incremental_state_cleared());
  }
  if (from.previous_packet_dropped() != 0) {
    set_previous_packet_dropped(from.previous_packet_dropped());
  }
}

void ChromeTracePacket::CopyFrom(const ChromeTracePacket& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perfetto.protos.ChromeTracePacket)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ChromeTracePacket::IsInitialized() const {
  return true;
}

void ChromeTracePacket::Swap(ChromeTracePacket* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ChromeTracePacket::InternalSwap(ChromeTracePacket* other) {
  using std::swap;
  swap(chrome_events_, other->chrome_events_);
  swap(clock_snapshot_, other->clock_snapshot_);
  swap(track_event_, other->track_event_);
  swap(interned_data_, other->interned_data_);
  swap(trace_config_, other->trace_config_);
  swap(trace_stats_, other->trace_stats_);
  swap(process_descriptor_, other->process_descriptor_);
  swap(thread_descriptor_, other->thread_descriptor_);
  swap(trusted_packet_sequence_id_, other->trusted_packet_sequence_id_);
  swap(incremental_state_cleared_, other->incremental_state_cleared_);
  swap(previous_packet_dropped_, other->previous_packet_dropped_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ChromeTracePacket::GetTypeName() const {
  return "perfetto.protos.ChromeTracePacket";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::perfetto::protos::ChromeTracePacket* Arena::CreateMaybeMessage< ::perfetto::protos::ChromeTracePacket >(Arena* arena) {
  return Arena::CreateInternal< ::perfetto::protos::ChromeTracePacket >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
