// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/config/data_source_config.proto

#include "perfetto/config/data_source_config.pb.h"

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

namespace protobuf_perfetto_2fconfig_2fandroid_2fandroid_5flog_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fandroid_2fandroid_5flog_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_AndroidLogConfig;
}  // namespace protobuf_perfetto_2fconfig_2fandroid_2fandroid_5flog_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fchrome_2fchrome_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fchrome_2fchrome_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ChromeConfig;
}  // namespace protobuf_perfetto_2fconfig_2fchrome_2fchrome_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fftrace_2fftrace_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fftrace_2fftrace_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_FtraceConfig;
}  // namespace protobuf_perfetto_2fconfig_2fftrace_2fftrace_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2finode_5ffile_2finode_5ffile_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2finode_5ffile_2finode_5ffile_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<1> scc_info_InodeFileConfig;
}  // namespace protobuf_perfetto_2fconfig_2finode_5ffile_2finode_5ffile_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fpower_2fandroid_5fpower_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fpower_2fandroid_5fpower_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_AndroidPowerConfig;
}  // namespace protobuf_perfetto_2fconfig_2fpower_2fandroid_5fpower_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fprocess_5fstats_2fprocess_5fstats_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fprocess_5fstats_2fprocess_5fstats_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ProcessStatsConfig;
}  // namespace protobuf_perfetto_2fconfig_2fprocess_5fstats_2fprocess_5fstats_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fprofiling_2fheapprofd_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fprofiling_2fheapprofd_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<1> scc_info_HeapprofdConfig;
}  // namespace protobuf_perfetto_2fconfig_2fprofiling_2fheapprofd_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_SysStatsConfig;
}  // namespace protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto
namespace protobuf_perfetto_2fconfig_2ftest_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2ftest_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<1> scc_info_TestConfig;
}  // namespace protobuf_perfetto_2fconfig_2ftest_5fconfig_2eproto
namespace perfetto {
namespace protos {
class DataSourceConfigDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<DataSourceConfig>
      _instance;
} _DataSourceConfig_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace protobuf_perfetto_2fconfig_2fdata_5fsource_5fconfig_2eproto {
static void InitDefaultsDataSourceConfig() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perfetto::protos::_DataSourceConfig_default_instance_;
    new (ptr) ::perfetto::protos::DataSourceConfig();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perfetto::protos::DataSourceConfig::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<9> scc_info_DataSourceConfig =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 9, InitDefaultsDataSourceConfig}, {
      &protobuf_perfetto_2fconfig_2fftrace_2fftrace_5fconfig_2eproto::scc_info_FtraceConfig.base,
      &protobuf_perfetto_2fconfig_2fchrome_2fchrome_5fconfig_2eproto::scc_info_ChromeConfig.base,
      &protobuf_perfetto_2fconfig_2finode_5ffile_2finode_5ffile_5fconfig_2eproto::scc_info_InodeFileConfig.base,
      &protobuf_perfetto_2fconfig_2fprocess_5fstats_2fprocess_5fstats_5fconfig_2eproto::scc_info_ProcessStatsConfig.base,
      &protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto::scc_info_SysStatsConfig.base,
      &protobuf_perfetto_2fconfig_2fprofiling_2fheapprofd_5fconfig_2eproto::scc_info_HeapprofdConfig.base,
      &protobuf_perfetto_2fconfig_2fpower_2fandroid_5fpower_5fconfig_2eproto::scc_info_AndroidPowerConfig.base,
      &protobuf_perfetto_2fconfig_2fandroid_2fandroid_5flog_5fconfig_2eproto::scc_info_AndroidLogConfig.base,
      &protobuf_perfetto_2fconfig_2ftest_5fconfig_2eproto::scc_info_TestConfig.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_DataSourceConfig.base);
}

}  // namespace protobuf_perfetto_2fconfig_2fdata_5fsource_5fconfig_2eproto
namespace perfetto {
namespace protos {

// ===================================================================

void DataSourceConfig::InitAsDefaultInstance() {
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->ftrace_config_ = const_cast< ::perfetto::protos::FtraceConfig*>(
      ::perfetto::protos::FtraceConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->chrome_config_ = const_cast< ::perfetto::protos::ChromeConfig*>(
      ::perfetto::protos::ChromeConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->inode_file_config_ = const_cast< ::perfetto::protos::InodeFileConfig*>(
      ::perfetto::protos::InodeFileConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->process_stats_config_ = const_cast< ::perfetto::protos::ProcessStatsConfig*>(
      ::perfetto::protos::ProcessStatsConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->sys_stats_config_ = const_cast< ::perfetto::protos::SysStatsConfig*>(
      ::perfetto::protos::SysStatsConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->heapprofd_config_ = const_cast< ::perfetto::protos::HeapprofdConfig*>(
      ::perfetto::protos::HeapprofdConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->android_power_config_ = const_cast< ::perfetto::protos::AndroidPowerConfig*>(
      ::perfetto::protos::AndroidPowerConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->android_log_config_ = const_cast< ::perfetto::protos::AndroidLogConfig*>(
      ::perfetto::protos::AndroidLogConfig::internal_default_instance());
  ::perfetto::protos::_DataSourceConfig_default_instance_._instance.get_mutable()->for_testing_ = const_cast< ::perfetto::protos::TestConfig*>(
      ::perfetto::protos::TestConfig::internal_default_instance());
}
void DataSourceConfig::clear_ftrace_config() {
  if (ftrace_config_ != NULL) ftrace_config_->Clear();
  clear_has_ftrace_config();
}
void DataSourceConfig::clear_chrome_config() {
  if (chrome_config_ != NULL) chrome_config_->Clear();
  clear_has_chrome_config();
}
void DataSourceConfig::clear_inode_file_config() {
  if (inode_file_config_ != NULL) inode_file_config_->Clear();
  clear_has_inode_file_config();
}
void DataSourceConfig::clear_process_stats_config() {
  if (process_stats_config_ != NULL) process_stats_config_->Clear();
  clear_has_process_stats_config();
}
void DataSourceConfig::clear_sys_stats_config() {
  if (sys_stats_config_ != NULL) sys_stats_config_->Clear();
  clear_has_sys_stats_config();
}
void DataSourceConfig::clear_heapprofd_config() {
  if (heapprofd_config_ != NULL) heapprofd_config_->Clear();
  clear_has_heapprofd_config();
}
void DataSourceConfig::clear_android_power_config() {
  if (android_power_config_ != NULL) android_power_config_->Clear();
  clear_has_android_power_config();
}
void DataSourceConfig::clear_android_log_config() {
  if (android_log_config_ != NULL) android_log_config_->Clear();
  clear_has_android_log_config();
}
void DataSourceConfig::clear_for_testing() {
  if (for_testing_ != NULL) for_testing_->Clear();
  clear_has_for_testing();
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DataSourceConfig::kNameFieldNumber;
const int DataSourceConfig::kTargetBufferFieldNumber;
const int DataSourceConfig::kTraceDurationMsFieldNumber;
const int DataSourceConfig::kEnableExtraGuardrailsFieldNumber;
const int DataSourceConfig::kTracingSessionIdFieldNumber;
const int DataSourceConfig::kFtraceConfigFieldNumber;
const int DataSourceConfig::kChromeConfigFieldNumber;
const int DataSourceConfig::kInodeFileConfigFieldNumber;
const int DataSourceConfig::kProcessStatsConfigFieldNumber;
const int DataSourceConfig::kSysStatsConfigFieldNumber;
const int DataSourceConfig::kHeapprofdConfigFieldNumber;
const int DataSourceConfig::kAndroidPowerConfigFieldNumber;
const int DataSourceConfig::kAndroidLogConfigFieldNumber;
const int DataSourceConfig::kLegacyConfigFieldNumber;
const int DataSourceConfig::kForTestingFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DataSourceConfig::DataSourceConfig()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_perfetto_2fconfig_2fdata_5fsource_5fconfig_2eproto::scc_info_DataSourceConfig.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:perfetto.protos.DataSourceConfig)
}
DataSourceConfig::DataSourceConfig(const DataSourceConfig& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_name()) {
    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  legacy_config_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_legacy_config()) {
    legacy_config_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.legacy_config_);
  }
  if (from.has_ftrace_config()) {
    ftrace_config_ = new ::perfetto::protos::FtraceConfig(*from.ftrace_config_);
  } else {
    ftrace_config_ = NULL;
  }
  if (from.has_chrome_config()) {
    chrome_config_ = new ::perfetto::protos::ChromeConfig(*from.chrome_config_);
  } else {
    chrome_config_ = NULL;
  }
  if (from.has_inode_file_config()) {
    inode_file_config_ = new ::perfetto::protos::InodeFileConfig(*from.inode_file_config_);
  } else {
    inode_file_config_ = NULL;
  }
  if (from.has_process_stats_config()) {
    process_stats_config_ = new ::perfetto::protos::ProcessStatsConfig(*from.process_stats_config_);
  } else {
    process_stats_config_ = NULL;
  }
  if (from.has_sys_stats_config()) {
    sys_stats_config_ = new ::perfetto::protos::SysStatsConfig(*from.sys_stats_config_);
  } else {
    sys_stats_config_ = NULL;
  }
  if (from.has_heapprofd_config()) {
    heapprofd_config_ = new ::perfetto::protos::HeapprofdConfig(*from.heapprofd_config_);
  } else {
    heapprofd_config_ = NULL;
  }
  if (from.has_android_power_config()) {
    android_power_config_ = new ::perfetto::protos::AndroidPowerConfig(*from.android_power_config_);
  } else {
    android_power_config_ = NULL;
  }
  if (from.has_android_log_config()) {
    android_log_config_ = new ::perfetto::protos::AndroidLogConfig(*from.android_log_config_);
  } else {
    android_log_config_ = NULL;
  }
  if (from.has_for_testing()) {
    for_testing_ = new ::perfetto::protos::TestConfig(*from.for_testing_);
  } else {
    for_testing_ = NULL;
  }
  ::memcpy(&target_buffer_, &from.target_buffer_,
    static_cast<size_t>(reinterpret_cast<char*>(&enable_extra_guardrails_) -
    reinterpret_cast<char*>(&target_buffer_)) + sizeof(enable_extra_guardrails_));
  // @@protoc_insertion_point(copy_constructor:perfetto.protos.DataSourceConfig)
}

void DataSourceConfig::SharedCtor() {
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  legacy_config_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&ftrace_config_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enable_extra_guardrails_) -
      reinterpret_cast<char*>(&ftrace_config_)) + sizeof(enable_extra_guardrails_));
}

DataSourceConfig::~DataSourceConfig() {
  // @@protoc_insertion_point(destructor:perfetto.protos.DataSourceConfig)
  SharedDtor();
}

void DataSourceConfig::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  legacy_config_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete ftrace_config_;
  if (this != internal_default_instance()) delete chrome_config_;
  if (this != internal_default_instance()) delete inode_file_config_;
  if (this != internal_default_instance()) delete process_stats_config_;
  if (this != internal_default_instance()) delete sys_stats_config_;
  if (this != internal_default_instance()) delete heapprofd_config_;
  if (this != internal_default_instance()) delete android_power_config_;
  if (this != internal_default_instance()) delete android_log_config_;
  if (this != internal_default_instance()) delete for_testing_;
}

void DataSourceConfig::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const DataSourceConfig& DataSourceConfig::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_perfetto_2fconfig_2fdata_5fsource_5fconfig_2eproto::scc_info_DataSourceConfig.base);
  return *internal_default_instance();
}


void DataSourceConfig::Clear() {
// @@protoc_insertion_point(message_clear_start:perfetto.protos.DataSourceConfig)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 255u) {
    if (cached_has_bits & 0x00000001u) {
      name_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      legacy_config_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000004u) {
      GOOGLE_DCHECK(ftrace_config_ != NULL);
      ftrace_config_->Clear();
    }
    if (cached_has_bits & 0x00000008u) {
      GOOGLE_DCHECK(chrome_config_ != NULL);
      chrome_config_->Clear();
    }
    if (cached_has_bits & 0x00000010u) {
      GOOGLE_DCHECK(inode_file_config_ != NULL);
      inode_file_config_->Clear();
    }
    if (cached_has_bits & 0x00000020u) {
      GOOGLE_DCHECK(process_stats_config_ != NULL);
      process_stats_config_->Clear();
    }
    if (cached_has_bits & 0x00000040u) {
      GOOGLE_DCHECK(sys_stats_config_ != NULL);
      sys_stats_config_->Clear();
    }
    if (cached_has_bits & 0x00000080u) {
      GOOGLE_DCHECK(heapprofd_config_ != NULL);
      heapprofd_config_->Clear();
    }
  }
  if (cached_has_bits & 1792u) {
    if (cached_has_bits & 0x00000100u) {
      GOOGLE_DCHECK(android_power_config_ != NULL);
      android_power_config_->Clear();
    }
    if (cached_has_bits & 0x00000200u) {
      GOOGLE_DCHECK(android_log_config_ != NULL);
      android_log_config_->Clear();
    }
    if (cached_has_bits & 0x00000400u) {
      GOOGLE_DCHECK(for_testing_ != NULL);
      for_testing_->Clear();
    }
  }
  if (cached_has_bits & 30720u) {
    ::memset(&target_buffer_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&enable_extra_guardrails_) -
        reinterpret_cast<char*>(&target_buffer_)) + sizeof(enable_extra_guardrails_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool DataSourceConfig::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:perfetto.protos.DataSourceConfig)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(2147483645u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string name = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional uint32 target_buffer = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          set_has_target_buffer();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &target_buffer_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional uint32 trace_duration_ms = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {
          set_has_trace_duration_ms();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &trace_duration_ms_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional uint64 tracing_session_id = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(32u /* 32 & 0xFF */)) {
          set_has_tracing_session_id();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &tracing_session_id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool enable_extra_guardrails = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(48u /* 48 & 0xFF */)) {
          set_has_enable_extra_guardrails();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &enable_extra_guardrails_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.FtraceConfig ftrace_config = 100;
      case 100: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 802 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_ftrace_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.ChromeConfig chrome_config = 101;
      case 101: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u /* 810 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_chrome_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.InodeFileConfig inode_file_config = 102;
      case 102: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(50u /* 818 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_inode_file_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.ProcessStatsConfig process_stats_config = 103;
      case 103: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(58u /* 826 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_process_stats_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.SysStatsConfig sys_stats_config = 104;
      case 104: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(66u /* 834 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_sys_stats_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.HeapprofdConfig heapprofd_config = 105;
      case 105: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(74u /* 842 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_heapprofd_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.AndroidPowerConfig android_power_config = 106;
      case 106: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(82u /* 850 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_android_power_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.AndroidLogConfig android_log_config = 107;
      case 107: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(90u /* 858 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_android_log_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional string legacy_config = 1000;
      case 1000: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(66u /* 8002 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_legacy_config()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .perfetto.protos.TestConfig for_testing = 268435455;
      case 268435455: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(250u /* 2147483642 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_for_testing()));
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
  // @@protoc_insertion_point(parse_success:perfetto.protos.DataSourceConfig)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:perfetto.protos.DataSourceConfig)
  return false;
#undef DO_
}

void DataSourceConfig::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:perfetto.protos.DataSourceConfig)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // optional uint32 target_buffer = 2;
  if (cached_has_bits & 0x00000800u) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->target_buffer(), output);
  }

  // optional uint32 trace_duration_ms = 3;
  if (cached_has_bits & 0x00001000u) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->trace_duration_ms(), output);
  }

  // optional uint64 tracing_session_id = 4;
  if (cached_has_bits & 0x00002000u) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(4, this->tracing_session_id(), output);
  }

  // optional bool enable_extra_guardrails = 6;
  if (cached_has_bits & 0x00004000u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(6, this->enable_extra_guardrails(), output);
  }

  // optional .perfetto.protos.FtraceConfig ftrace_config = 100;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      100, this->_internal_ftrace_config(), output);
  }

  // optional .perfetto.protos.ChromeConfig chrome_config = 101;
  if (cached_has_bits & 0x00000008u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      101, this->_internal_chrome_config(), output);
  }

  // optional .perfetto.protos.InodeFileConfig inode_file_config = 102;
  if (cached_has_bits & 0x00000010u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      102, this->_internal_inode_file_config(), output);
  }

  // optional .perfetto.protos.ProcessStatsConfig process_stats_config = 103;
  if (cached_has_bits & 0x00000020u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      103, this->_internal_process_stats_config(), output);
  }

  // optional .perfetto.protos.SysStatsConfig sys_stats_config = 104;
  if (cached_has_bits & 0x00000040u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      104, this->_internal_sys_stats_config(), output);
  }

  // optional .perfetto.protos.HeapprofdConfig heapprofd_config = 105;
  if (cached_has_bits & 0x00000080u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      105, this->_internal_heapprofd_config(), output);
  }

  // optional .perfetto.protos.AndroidPowerConfig android_power_config = 106;
  if (cached_has_bits & 0x00000100u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      106, this->_internal_android_power_config(), output);
  }

  // optional .perfetto.protos.AndroidLogConfig android_log_config = 107;
  if (cached_has_bits & 0x00000200u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      107, this->_internal_android_log_config(), output);
  }

  // optional string legacy_config = 1000;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1000, this->legacy_config(), output);
  }

  // optional .perfetto.protos.TestConfig for_testing = 268435455;
  if (cached_has_bits & 0x00000400u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      268435455, this->_internal_for_testing(), output);
  }

  output->WriteRaw(_internal_metadata_.unknown_fields().data(),
                   static_cast<int>(_internal_metadata_.unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:perfetto.protos.DataSourceConfig)
}

size_t DataSourceConfig::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perfetto.protos.DataSourceConfig)
  size_t total_size = 0;

  total_size += _internal_metadata_.unknown_fields().size();

  if (_has_bits_[0 / 32] & 255u) {
    // optional string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // optional string legacy_config = 1000;
    if (has_legacy_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->legacy_config());
    }

    // optional .perfetto.protos.FtraceConfig ftrace_config = 100;
    if (has_ftrace_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *ftrace_config_);
    }

    // optional .perfetto.protos.ChromeConfig chrome_config = 101;
    if (has_chrome_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *chrome_config_);
    }

    // optional .perfetto.protos.InodeFileConfig inode_file_config = 102;
    if (has_inode_file_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *inode_file_config_);
    }

    // optional .perfetto.protos.ProcessStatsConfig process_stats_config = 103;
    if (has_process_stats_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *process_stats_config_);
    }

    // optional .perfetto.protos.SysStatsConfig sys_stats_config = 104;
    if (has_sys_stats_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *sys_stats_config_);
    }

    // optional .perfetto.protos.HeapprofdConfig heapprofd_config = 105;
    if (has_heapprofd_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *heapprofd_config_);
    }

  }
  if (_has_bits_[8 / 32] & 32512u) {
    // optional .perfetto.protos.AndroidPowerConfig android_power_config = 106;
    if (has_android_power_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *android_power_config_);
    }

    // optional .perfetto.protos.AndroidLogConfig android_log_config = 107;
    if (has_android_log_config()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *android_log_config_);
    }

    // optional .perfetto.protos.TestConfig for_testing = 268435455;
    if (has_for_testing()) {
      total_size += 5 +
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          *for_testing_);
    }

    // optional uint32 target_buffer = 2;
    if (has_target_buffer()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->target_buffer());
    }

    // optional uint32 trace_duration_ms = 3;
    if (has_trace_duration_ms()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->trace_duration_ms());
    }

    // optional uint64 tracing_session_id = 4;
    if (has_tracing_session_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->tracing_session_id());
    }

    // optional bool enable_extra_guardrails = 6;
    if (has_enable_extra_guardrails()) {
      total_size += 1 + 1;
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void DataSourceConfig::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const DataSourceConfig*>(&from));
}

void DataSourceConfig::MergeFrom(const DataSourceConfig& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perfetto.protos.DataSourceConfig)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 255u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_name();
      name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
    }
    if (cached_has_bits & 0x00000002u) {
      set_has_legacy_config();
      legacy_config_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.legacy_config_);
    }
    if (cached_has_bits & 0x00000004u) {
      mutable_ftrace_config()->::perfetto::protos::FtraceConfig::MergeFrom(from.ftrace_config());
    }
    if (cached_has_bits & 0x00000008u) {
      mutable_chrome_config()->::perfetto::protos::ChromeConfig::MergeFrom(from.chrome_config());
    }
    if (cached_has_bits & 0x00000010u) {
      mutable_inode_file_config()->::perfetto::protos::InodeFileConfig::MergeFrom(from.inode_file_config());
    }
    if (cached_has_bits & 0x00000020u) {
      mutable_process_stats_config()->::perfetto::protos::ProcessStatsConfig::MergeFrom(from.process_stats_config());
    }
    if (cached_has_bits & 0x00000040u) {
      mutable_sys_stats_config()->::perfetto::protos::SysStatsConfig::MergeFrom(from.sys_stats_config());
    }
    if (cached_has_bits & 0x00000080u) {
      mutable_heapprofd_config()->::perfetto::protos::HeapprofdConfig::MergeFrom(from.heapprofd_config());
    }
  }
  if (cached_has_bits & 32512u) {
    if (cached_has_bits & 0x00000100u) {
      mutable_android_power_config()->::perfetto::protos::AndroidPowerConfig::MergeFrom(from.android_power_config());
    }
    if (cached_has_bits & 0x00000200u) {
      mutable_android_log_config()->::perfetto::protos::AndroidLogConfig::MergeFrom(from.android_log_config());
    }
    if (cached_has_bits & 0x00000400u) {
      mutable_for_testing()->::perfetto::protos::TestConfig::MergeFrom(from.for_testing());
    }
    if (cached_has_bits & 0x00000800u) {
      target_buffer_ = from.target_buffer_;
    }
    if (cached_has_bits & 0x00001000u) {
      trace_duration_ms_ = from.trace_duration_ms_;
    }
    if (cached_has_bits & 0x00002000u) {
      tracing_session_id_ = from.tracing_session_id_;
    }
    if (cached_has_bits & 0x00004000u) {
      enable_extra_guardrails_ = from.enable_extra_guardrails_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void DataSourceConfig::CopyFrom(const DataSourceConfig& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perfetto.protos.DataSourceConfig)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DataSourceConfig::IsInitialized() const {
  return true;
}

void DataSourceConfig::Swap(DataSourceConfig* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DataSourceConfig::InternalSwap(DataSourceConfig* other) {
  using std::swap;
  name_.Swap(&other->name_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  legacy_config_.Swap(&other->legacy_config_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(ftrace_config_, other->ftrace_config_);
  swap(chrome_config_, other->chrome_config_);
  swap(inode_file_config_, other->inode_file_config_);
  swap(process_stats_config_, other->process_stats_config_);
  swap(sys_stats_config_, other->sys_stats_config_);
  swap(heapprofd_config_, other->heapprofd_config_);
  swap(android_power_config_, other->android_power_config_);
  swap(android_log_config_, other->android_log_config_);
  swap(for_testing_, other->for_testing_);
  swap(target_buffer_, other->target_buffer_);
  swap(trace_duration_ms_, other->trace_duration_ms_);
  swap(tracing_session_id_, other->tracing_session_id_);
  swap(enable_extra_guardrails_, other->enable_extra_guardrails_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string DataSourceConfig::GetTypeName() const {
  return "perfetto.protos.DataSourceConfig";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::perfetto::protos::DataSourceConfig* Arena::CreateMaybeMessage< ::perfetto::protos::DataSourceConfig >(Arena* arena) {
  return Arena::CreateInternal< ::perfetto::protos::DataSourceConfig >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
