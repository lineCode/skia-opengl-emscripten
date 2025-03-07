// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/config/sys_stats/sys_stats_config.proto

#ifndef PROTOBUF_INCLUDED_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto
#define PROTOBUF_INCLUDED_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_util.h>
#include "perfetto/common/sys_stats_counters.pb.h"
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto 

namespace protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
}  // namespace protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto
namespace perfetto {
namespace protos {
class SysStatsConfig;
class SysStatsConfigDefaultTypeInternal;
extern SysStatsConfigDefaultTypeInternal _SysStatsConfig_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> ::perfetto::protos::SysStatsConfig* Arena::CreateMaybeMessage<::perfetto::protos::SysStatsConfig>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace perfetto {
namespace protos {

enum SysStatsConfig_StatCounters {
  SysStatsConfig_StatCounters_STAT_UNSPECIFIED = 0,
  SysStatsConfig_StatCounters_STAT_CPU_TIMES = 1,
  SysStatsConfig_StatCounters_STAT_IRQ_COUNTS = 2,
  SysStatsConfig_StatCounters_STAT_SOFTIRQ_COUNTS = 3,
  SysStatsConfig_StatCounters_STAT_FORK_COUNT = 4
};
bool SysStatsConfig_StatCounters_IsValid(int value);
const SysStatsConfig_StatCounters SysStatsConfig_StatCounters_StatCounters_MIN = SysStatsConfig_StatCounters_STAT_UNSPECIFIED;
const SysStatsConfig_StatCounters SysStatsConfig_StatCounters_StatCounters_MAX = SysStatsConfig_StatCounters_STAT_FORK_COUNT;
const int SysStatsConfig_StatCounters_StatCounters_ARRAYSIZE = SysStatsConfig_StatCounters_StatCounters_MAX + 1;

// ===================================================================

class SysStatsConfig : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:perfetto.protos.SysStatsConfig) */ {
 public:
  SysStatsConfig();
  virtual ~SysStatsConfig();

  SysStatsConfig(const SysStatsConfig& from);

  inline SysStatsConfig& operator=(const SysStatsConfig& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  SysStatsConfig(SysStatsConfig&& from) noexcept
    : SysStatsConfig() {
    *this = ::std::move(from);
  }

  inline SysStatsConfig& operator=(SysStatsConfig&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const SysStatsConfig& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SysStatsConfig* internal_default_instance() {
    return reinterpret_cast<const SysStatsConfig*>(
               &_SysStatsConfig_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(SysStatsConfig* other);
  friend void swap(SysStatsConfig& a, SysStatsConfig& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline SysStatsConfig* New() const final {
    return CreateMaybeMessage<SysStatsConfig>(NULL);
  }

  SysStatsConfig* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<SysStatsConfig>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const SysStatsConfig& from);
  void MergeFrom(const SysStatsConfig& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  void DiscardUnknownFields();
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SysStatsConfig* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  typedef SysStatsConfig_StatCounters StatCounters;
  static const StatCounters STAT_UNSPECIFIED =
    SysStatsConfig_StatCounters_STAT_UNSPECIFIED;
  static const StatCounters STAT_CPU_TIMES =
    SysStatsConfig_StatCounters_STAT_CPU_TIMES;
  static const StatCounters STAT_IRQ_COUNTS =
    SysStatsConfig_StatCounters_STAT_IRQ_COUNTS;
  static const StatCounters STAT_SOFTIRQ_COUNTS =
    SysStatsConfig_StatCounters_STAT_SOFTIRQ_COUNTS;
  static const StatCounters STAT_FORK_COUNT =
    SysStatsConfig_StatCounters_STAT_FORK_COUNT;
  static inline bool StatCounters_IsValid(int value) {
    return SysStatsConfig_StatCounters_IsValid(value);
  }
  static const StatCounters StatCounters_MIN =
    SysStatsConfig_StatCounters_StatCounters_MIN;
  static const StatCounters StatCounters_MAX =
    SysStatsConfig_StatCounters_StatCounters_MAX;
  static const int StatCounters_ARRAYSIZE =
    SysStatsConfig_StatCounters_StatCounters_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // repeated .perfetto.protos.MeminfoCounters meminfo_counters = 2;
  int meminfo_counters_size() const;
  void clear_meminfo_counters();
  static const int kMeminfoCountersFieldNumber = 2;
  ::perfetto::protos::MeminfoCounters meminfo_counters(int index) const;
  void set_meminfo_counters(int index, ::perfetto::protos::MeminfoCounters value);
  void add_meminfo_counters(::perfetto::protos::MeminfoCounters value);
  const ::google::protobuf::RepeatedField<int>& meminfo_counters() const;
  ::google::protobuf::RepeatedField<int>* mutable_meminfo_counters();

  // repeated .perfetto.protos.VmstatCounters vmstat_counters = 4;
  int vmstat_counters_size() const;
  void clear_vmstat_counters();
  static const int kVmstatCountersFieldNumber = 4;
  ::perfetto::protos::VmstatCounters vmstat_counters(int index) const;
  void set_vmstat_counters(int index, ::perfetto::protos::VmstatCounters value);
  void add_vmstat_counters(::perfetto::protos::VmstatCounters value);
  const ::google::protobuf::RepeatedField<int>& vmstat_counters() const;
  ::google::protobuf::RepeatedField<int>* mutable_vmstat_counters();

  // repeated .perfetto.protos.SysStatsConfig.StatCounters stat_counters = 6;
  int stat_counters_size() const;
  void clear_stat_counters();
  static const int kStatCountersFieldNumber = 6;
  ::perfetto::protos::SysStatsConfig_StatCounters stat_counters(int index) const;
  void set_stat_counters(int index, ::perfetto::protos::SysStatsConfig_StatCounters value);
  void add_stat_counters(::perfetto::protos::SysStatsConfig_StatCounters value);
  const ::google::protobuf::RepeatedField<int>& stat_counters() const;
  ::google::protobuf::RepeatedField<int>* mutable_stat_counters();

  // optional uint32 meminfo_period_ms = 1;
  bool has_meminfo_period_ms() const;
  void clear_meminfo_period_ms();
  static const int kMeminfoPeriodMsFieldNumber = 1;
  ::google::protobuf::uint32 meminfo_period_ms() const;
  void set_meminfo_period_ms(::google::protobuf::uint32 value);

  // optional uint32 vmstat_period_ms = 3;
  bool has_vmstat_period_ms() const;
  void clear_vmstat_period_ms();
  static const int kVmstatPeriodMsFieldNumber = 3;
  ::google::protobuf::uint32 vmstat_period_ms() const;
  void set_vmstat_period_ms(::google::protobuf::uint32 value);

  // optional uint32 stat_period_ms = 5;
  bool has_stat_period_ms() const;
  void clear_stat_period_ms();
  static const int kStatPeriodMsFieldNumber = 5;
  ::google::protobuf::uint32 stat_period_ms() const;
  void set_stat_period_ms(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:perfetto.protos.SysStatsConfig)
 private:
  void set_has_meminfo_period_ms();
  void clear_has_meminfo_period_ms();
  void set_has_vmstat_period_ms();
  void clear_has_vmstat_period_ms();
  void set_has_stat_period_ms();
  void clear_has_stat_period_ms();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedField<int> meminfo_counters_;
  ::google::protobuf::RepeatedField<int> vmstat_counters_;
  ::google::protobuf::RepeatedField<int> stat_counters_;
  ::google::protobuf::uint32 meminfo_period_ms_;
  ::google::protobuf::uint32 vmstat_period_ms_;
  ::google::protobuf::uint32 stat_period_ms_;
  friend struct ::protobuf_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SysStatsConfig

// optional uint32 meminfo_period_ms = 1;
inline bool SysStatsConfig::has_meminfo_period_ms() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SysStatsConfig::set_has_meminfo_period_ms() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SysStatsConfig::clear_has_meminfo_period_ms() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SysStatsConfig::clear_meminfo_period_ms() {
  meminfo_period_ms_ = 0u;
  clear_has_meminfo_period_ms();
}
inline ::google::protobuf::uint32 SysStatsConfig::meminfo_period_ms() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.meminfo_period_ms)
  return meminfo_period_ms_;
}
inline void SysStatsConfig::set_meminfo_period_ms(::google::protobuf::uint32 value) {
  set_has_meminfo_period_ms();
  meminfo_period_ms_ = value;
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.meminfo_period_ms)
}

// repeated .perfetto.protos.MeminfoCounters meminfo_counters = 2;
inline int SysStatsConfig::meminfo_counters_size() const {
  return meminfo_counters_.size();
}
inline void SysStatsConfig::clear_meminfo_counters() {
  meminfo_counters_.Clear();
}
inline ::perfetto::protos::MeminfoCounters SysStatsConfig::meminfo_counters(int index) const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.meminfo_counters)
  return static_cast< ::perfetto::protos::MeminfoCounters >(meminfo_counters_.Get(index));
}
inline void SysStatsConfig::set_meminfo_counters(int index, ::perfetto::protos::MeminfoCounters value) {
  assert(::perfetto::protos::MeminfoCounters_IsValid(value));
  meminfo_counters_.Set(index, value);
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.meminfo_counters)
}
inline void SysStatsConfig::add_meminfo_counters(::perfetto::protos::MeminfoCounters value) {
  assert(::perfetto::protos::MeminfoCounters_IsValid(value));
  meminfo_counters_.Add(value);
  // @@protoc_insertion_point(field_add:perfetto.protos.SysStatsConfig.meminfo_counters)
}
inline const ::google::protobuf::RepeatedField<int>&
SysStatsConfig::meminfo_counters() const {
  // @@protoc_insertion_point(field_list:perfetto.protos.SysStatsConfig.meminfo_counters)
  return meminfo_counters_;
}
inline ::google::protobuf::RepeatedField<int>*
SysStatsConfig::mutable_meminfo_counters() {
  // @@protoc_insertion_point(field_mutable_list:perfetto.protos.SysStatsConfig.meminfo_counters)
  return &meminfo_counters_;
}

// optional uint32 vmstat_period_ms = 3;
inline bool SysStatsConfig::has_vmstat_period_ms() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SysStatsConfig::set_has_vmstat_period_ms() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SysStatsConfig::clear_has_vmstat_period_ms() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SysStatsConfig::clear_vmstat_period_ms() {
  vmstat_period_ms_ = 0u;
  clear_has_vmstat_period_ms();
}
inline ::google::protobuf::uint32 SysStatsConfig::vmstat_period_ms() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.vmstat_period_ms)
  return vmstat_period_ms_;
}
inline void SysStatsConfig::set_vmstat_period_ms(::google::protobuf::uint32 value) {
  set_has_vmstat_period_ms();
  vmstat_period_ms_ = value;
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.vmstat_period_ms)
}

// repeated .perfetto.protos.VmstatCounters vmstat_counters = 4;
inline int SysStatsConfig::vmstat_counters_size() const {
  return vmstat_counters_.size();
}
inline void SysStatsConfig::clear_vmstat_counters() {
  vmstat_counters_.Clear();
}
inline ::perfetto::protos::VmstatCounters SysStatsConfig::vmstat_counters(int index) const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.vmstat_counters)
  return static_cast< ::perfetto::protos::VmstatCounters >(vmstat_counters_.Get(index));
}
inline void SysStatsConfig::set_vmstat_counters(int index, ::perfetto::protos::VmstatCounters value) {
  assert(::perfetto::protos::VmstatCounters_IsValid(value));
  vmstat_counters_.Set(index, value);
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.vmstat_counters)
}
inline void SysStatsConfig::add_vmstat_counters(::perfetto::protos::VmstatCounters value) {
  assert(::perfetto::protos::VmstatCounters_IsValid(value));
  vmstat_counters_.Add(value);
  // @@protoc_insertion_point(field_add:perfetto.protos.SysStatsConfig.vmstat_counters)
}
inline const ::google::protobuf::RepeatedField<int>&
SysStatsConfig::vmstat_counters() const {
  // @@protoc_insertion_point(field_list:perfetto.protos.SysStatsConfig.vmstat_counters)
  return vmstat_counters_;
}
inline ::google::protobuf::RepeatedField<int>*
SysStatsConfig::mutable_vmstat_counters() {
  // @@protoc_insertion_point(field_mutable_list:perfetto.protos.SysStatsConfig.vmstat_counters)
  return &vmstat_counters_;
}

// optional uint32 stat_period_ms = 5;
inline bool SysStatsConfig::has_stat_period_ms() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SysStatsConfig::set_has_stat_period_ms() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SysStatsConfig::clear_has_stat_period_ms() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SysStatsConfig::clear_stat_period_ms() {
  stat_period_ms_ = 0u;
  clear_has_stat_period_ms();
}
inline ::google::protobuf::uint32 SysStatsConfig::stat_period_ms() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.stat_period_ms)
  return stat_period_ms_;
}
inline void SysStatsConfig::set_stat_period_ms(::google::protobuf::uint32 value) {
  set_has_stat_period_ms();
  stat_period_ms_ = value;
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.stat_period_ms)
}

// repeated .perfetto.protos.SysStatsConfig.StatCounters stat_counters = 6;
inline int SysStatsConfig::stat_counters_size() const {
  return stat_counters_.size();
}
inline void SysStatsConfig::clear_stat_counters() {
  stat_counters_.Clear();
}
inline ::perfetto::protos::SysStatsConfig_StatCounters SysStatsConfig::stat_counters(int index) const {
  // @@protoc_insertion_point(field_get:perfetto.protos.SysStatsConfig.stat_counters)
  return static_cast< ::perfetto::protos::SysStatsConfig_StatCounters >(stat_counters_.Get(index));
}
inline void SysStatsConfig::set_stat_counters(int index, ::perfetto::protos::SysStatsConfig_StatCounters value) {
  assert(::perfetto::protos::SysStatsConfig_StatCounters_IsValid(value));
  stat_counters_.Set(index, value);
  // @@protoc_insertion_point(field_set:perfetto.protos.SysStatsConfig.stat_counters)
}
inline void SysStatsConfig::add_stat_counters(::perfetto::protos::SysStatsConfig_StatCounters value) {
  assert(::perfetto::protos::SysStatsConfig_StatCounters_IsValid(value));
  stat_counters_.Add(value);
  // @@protoc_insertion_point(field_add:perfetto.protos.SysStatsConfig.stat_counters)
}
inline const ::google::protobuf::RepeatedField<int>&
SysStatsConfig::stat_counters() const {
  // @@protoc_insertion_point(field_list:perfetto.protos.SysStatsConfig.stat_counters)
  return stat_counters_;
}
inline ::google::protobuf::RepeatedField<int>*
SysStatsConfig::mutable_stat_counters() {
  // @@protoc_insertion_point(field_mutable_list:perfetto.protos.SysStatsConfig.stat_counters)
  return &stat_counters_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace protos
}  // namespace perfetto

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::perfetto::protos::SysStatsConfig_StatCounters> : ::std::true_type {};

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_perfetto_2fconfig_2fsys_5fstats_2fsys_5fstats_5fconfig_2eproto
