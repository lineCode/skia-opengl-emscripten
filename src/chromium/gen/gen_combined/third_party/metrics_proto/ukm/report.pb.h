// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ukm/report.proto

#ifndef PROTOBUF_INCLUDED_ukm_2freport_2eproto
#define PROTOBUF_INCLUDED_ukm_2freport_2eproto

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
#include "ukm/aggregate.pb.h"
#include "ukm/entry.pb.h"
#include "ukm/source.pb.h"
#include "system_profile.pb.h"
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_ukm_2freport_2eproto 

namespace protobuf_ukm_2freport_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
}  // namespace protobuf_ukm_2freport_2eproto
namespace ukm {
class Report;
class ReportDefaultTypeInternal;
extern ReportDefaultTypeInternal _Report_default_instance_;
class Report_SourceCounts;
class Report_SourceCountsDefaultTypeInternal;
extern Report_SourceCountsDefaultTypeInternal _Report_SourceCounts_default_instance_;
}  // namespace ukm
namespace google {
namespace protobuf {
template<> ::ukm::Report* Arena::CreateMaybeMessage<::ukm::Report>(Arena*);
template<> ::ukm::Report_SourceCounts* Arena::CreateMaybeMessage<::ukm::Report_SourceCounts>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace ukm {

enum Report_LogRotationReason {
  Report_LogRotationReason_UNKNOWN = 0,
  Report_LogRotationReason_SCHEDULED_ROTATION = 1,
  Report_LogRotationReason_BACKGROUNDED = 2,
  Report_LogRotationReason_SHUTDOWN = 3
};
bool Report_LogRotationReason_IsValid(int value);
const Report_LogRotationReason Report_LogRotationReason_LogRotationReason_MIN = Report_LogRotationReason_UNKNOWN;
const Report_LogRotationReason Report_LogRotationReason_LogRotationReason_MAX = Report_LogRotationReason_SHUTDOWN;
const int Report_LogRotationReason_LogRotationReason_ARRAYSIZE = Report_LogRotationReason_LogRotationReason_MAX + 1;

// ===================================================================

class Report_SourceCounts : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:ukm.Report.SourceCounts) */ {
 public:
  Report_SourceCounts();
  virtual ~Report_SourceCounts();

  Report_SourceCounts(const Report_SourceCounts& from);

  inline Report_SourceCounts& operator=(const Report_SourceCounts& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Report_SourceCounts(Report_SourceCounts&& from) noexcept
    : Report_SourceCounts() {
    *this = ::std::move(from);
  }

  inline Report_SourceCounts& operator=(Report_SourceCounts&& from) noexcept {
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

  static const Report_SourceCounts& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Report_SourceCounts* internal_default_instance() {
    return reinterpret_cast<const Report_SourceCounts*>(
               &_Report_SourceCounts_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(Report_SourceCounts* other);
  friend void swap(Report_SourceCounts& a, Report_SourceCounts& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Report_SourceCounts* New() const final {
    return CreateMaybeMessage<Report_SourceCounts>(NULL);
  }

  Report_SourceCounts* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Report_SourceCounts>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const Report_SourceCounts& from);
  void MergeFrom(const Report_SourceCounts& from);
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
  void InternalSwap(Report_SourceCounts* other);
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

  // accessors -------------------------------------------------------

  // optional int32 observed = 1;
  bool has_observed() const;
  void clear_observed();
  static const int kObservedFieldNumber = 1;
  ::google::protobuf::int32 observed() const;
  void set_observed(::google::protobuf::int32 value);

  // optional int32 navigation_sources = 2;
  bool has_navigation_sources() const;
  void clear_navigation_sources();
  static const int kNavigationSourcesFieldNumber = 2;
  ::google::protobuf::int32 navigation_sources() const;
  void set_navigation_sources(::google::protobuf::int32 value);

  // optional int32 unmatched_sources = 3;
  bool has_unmatched_sources() const;
  void clear_unmatched_sources();
  static const int kUnmatchedSourcesFieldNumber = 3;
  ::google::protobuf::int32 unmatched_sources() const;
  void set_unmatched_sources(::google::protobuf::int32 value);

  // optional int32 carryover_sources = 4;
  bool has_carryover_sources() const;
  void clear_carryover_sources();
  static const int kCarryoverSourcesFieldNumber = 4;
  ::google::protobuf::int32 carryover_sources() const;
  void set_carryover_sources(::google::protobuf::int32 value);

  // optional int32 deferred_sources = 5;
  bool has_deferred_sources() const;
  void clear_deferred_sources();
  static const int kDeferredSourcesFieldNumber = 5;
  ::google::protobuf::int32 deferred_sources() const;
  void set_deferred_sources(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ukm.Report.SourceCounts)
 private:
  void set_has_observed();
  void clear_has_observed();
  void set_has_navigation_sources();
  void clear_has_navigation_sources();
  void set_has_unmatched_sources();
  void clear_has_unmatched_sources();
  void set_has_carryover_sources();
  void clear_has_carryover_sources();
  void set_has_deferred_sources();
  void clear_has_deferred_sources();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::int32 observed_;
  ::google::protobuf::int32 navigation_sources_;
  ::google::protobuf::int32 unmatched_sources_;
  ::google::protobuf::int32 carryover_sources_;
  ::google::protobuf::int32 deferred_sources_;
  friend struct ::protobuf_ukm_2freport_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Report : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:ukm.Report) */ {
 public:
  Report();
  virtual ~Report();

  Report(const Report& from);

  inline Report& operator=(const Report& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Report(Report&& from) noexcept
    : Report() {
    *this = ::std::move(from);
  }

  inline Report& operator=(Report&& from) noexcept {
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

  static const Report& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Report* internal_default_instance() {
    return reinterpret_cast<const Report*>(
               &_Report_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(Report* other);
  friend void swap(Report& a, Report& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Report* New() const final {
    return CreateMaybeMessage<Report>(NULL);
  }

  Report* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Report>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const Report& from);
  void MergeFrom(const Report& from);
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
  void InternalSwap(Report* other);
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

  typedef Report_SourceCounts SourceCounts;

  typedef Report_LogRotationReason LogRotationReason;
  static const LogRotationReason UNKNOWN =
    Report_LogRotationReason_UNKNOWN;
  static const LogRotationReason SCHEDULED_ROTATION =
    Report_LogRotationReason_SCHEDULED_ROTATION;
  static const LogRotationReason BACKGROUNDED =
    Report_LogRotationReason_BACKGROUNDED;
  static const LogRotationReason SHUTDOWN =
    Report_LogRotationReason_SHUTDOWN;
  static inline bool LogRotationReason_IsValid(int value) {
    return Report_LogRotationReason_IsValid(value);
  }
  static const LogRotationReason LogRotationReason_MIN =
    Report_LogRotationReason_LogRotationReason_MIN;
  static const LogRotationReason LogRotationReason_MAX =
    Report_LogRotationReason_LogRotationReason_MAX;
  static const int LogRotationReason_ARRAYSIZE =
    Report_LogRotationReason_LogRotationReason_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // repeated .ukm.Source sources = 3;
  int sources_size() const;
  void clear_sources();
  static const int kSourcesFieldNumber = 3;
  ::ukm::Source* mutable_sources(int index);
  ::google::protobuf::RepeatedPtrField< ::ukm::Source >*
      mutable_sources();
  const ::ukm::Source& sources(int index) const;
  ::ukm::Source* add_sources();
  const ::google::protobuf::RepeatedPtrField< ::ukm::Source >&
      sources() const;

  // repeated .ukm.Entry entries = 4;
  int entries_size() const;
  void clear_entries();
  static const int kEntriesFieldNumber = 4;
  ::ukm::Entry* mutable_entries(int index);
  ::google::protobuf::RepeatedPtrField< ::ukm::Entry >*
      mutable_entries();
  const ::ukm::Entry& entries(int index) const;
  ::ukm::Entry* add_entries();
  const ::google::protobuf::RepeatedPtrField< ::ukm::Entry >&
      entries() const;

  // repeated .ukm.Aggregate aggregates = 7;
  int aggregates_size() const;
  void clear_aggregates();
  static const int kAggregatesFieldNumber = 7;
  ::ukm::Aggregate* mutable_aggregates(int index);
  ::google::protobuf::RepeatedPtrField< ::ukm::Aggregate >*
      mutable_aggregates();
  const ::ukm::Aggregate& aggregates(int index) const;
  ::ukm::Aggregate* add_aggregates();
  const ::google::protobuf::RepeatedPtrField< ::ukm::Aggregate >&
      aggregates() const;

  // optional .metrics.SystemProfileProto system_profile = 2;
  bool has_system_profile() const;
  void clear_system_profile();
  static const int kSystemProfileFieldNumber = 2;
  private:
  const ::metrics::SystemProfileProto& _internal_system_profile() const;
  public:
  const ::metrics::SystemProfileProto& system_profile() const;
  ::metrics::SystemProfileProto* release_system_profile();
  ::metrics::SystemProfileProto* mutable_system_profile();
  void set_allocated_system_profile(::metrics::SystemProfileProto* system_profile);

  // optional .ukm.Report.SourceCounts source_counts = 10;
  bool has_source_counts() const;
  void clear_source_counts();
  static const int kSourceCountsFieldNumber = 10;
  private:
  const ::ukm::Report_SourceCounts& _internal_source_counts() const;
  public:
  const ::ukm::Report_SourceCounts& source_counts() const;
  ::ukm::Report_SourceCounts* release_source_counts();
  ::ukm::Report_SourceCounts* mutable_source_counts();
  void set_allocated_source_counts(::ukm::Report_SourceCounts* source_counts);

  // optional fixed64 client_id = 1;
  bool has_client_id() const;
  void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  ::google::protobuf::uint64 client_id() const;
  void set_client_id(::google::protobuf::uint64 value);

  // optional int32 session_id = 5;
  bool has_session_id() const;
  void clear_session_id();
  static const int kSessionIdFieldNumber = 5;
  ::google::protobuf::int32 session_id() const;
  void set_session_id(::google::protobuf::int32 value);

  // optional int32 report_id = 6;
  bool has_report_id() const;
  void clear_report_id();
  static const int kReportIdFieldNumber = 6;
  ::google::protobuf::int32 report_id() const;
  void set_report_id(::google::protobuf::int32 value);

  // optional bool is_continuous = 8;
  bool has_is_continuous() const;
  void clear_is_continuous();
  static const int kIsContinuousFieldNumber = 8;
  bool is_continuous() const;
  void set_is_continuous(bool value);

  // optional .ukm.Report.LogRotationReason log_rotation_reason = 9;
  bool has_log_rotation_reason() const;
  void clear_log_rotation_reason();
  static const int kLogRotationReasonFieldNumber = 9;
  ::ukm::Report_LogRotationReason log_rotation_reason() const;
  void set_log_rotation_reason(::ukm::Report_LogRotationReason value);

  // @@protoc_insertion_point(class_scope:ukm.Report)
 private:
  void set_has_client_id();
  void clear_has_client_id();
  void set_has_session_id();
  void clear_has_session_id();
  void set_has_report_id();
  void clear_has_report_id();
  void set_has_is_continuous();
  void clear_has_is_continuous();
  void set_has_log_rotation_reason();
  void clear_has_log_rotation_reason();
  void set_has_system_profile();
  void clear_has_system_profile();
  void set_has_source_counts();
  void clear_has_source_counts();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::ukm::Source > sources_;
  ::google::protobuf::RepeatedPtrField< ::ukm::Entry > entries_;
  ::google::protobuf::RepeatedPtrField< ::ukm::Aggregate > aggregates_;
  ::metrics::SystemProfileProto* system_profile_;
  ::ukm::Report_SourceCounts* source_counts_;
  ::google::protobuf::uint64 client_id_;
  ::google::protobuf::int32 session_id_;
  ::google::protobuf::int32 report_id_;
  bool is_continuous_;
  int log_rotation_reason_;
  friend struct ::protobuf_ukm_2freport_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Report_SourceCounts

// optional int32 observed = 1;
inline bool Report_SourceCounts::has_observed() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Report_SourceCounts::set_has_observed() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Report_SourceCounts::clear_has_observed() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Report_SourceCounts::clear_observed() {
  observed_ = 0;
  clear_has_observed();
}
inline ::google::protobuf::int32 Report_SourceCounts::observed() const {
  // @@protoc_insertion_point(field_get:ukm.Report.SourceCounts.observed)
  return observed_;
}
inline void Report_SourceCounts::set_observed(::google::protobuf::int32 value) {
  set_has_observed();
  observed_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.SourceCounts.observed)
}

// optional int32 navigation_sources = 2;
inline bool Report_SourceCounts::has_navigation_sources() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Report_SourceCounts::set_has_navigation_sources() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Report_SourceCounts::clear_has_navigation_sources() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Report_SourceCounts::clear_navigation_sources() {
  navigation_sources_ = 0;
  clear_has_navigation_sources();
}
inline ::google::protobuf::int32 Report_SourceCounts::navigation_sources() const {
  // @@protoc_insertion_point(field_get:ukm.Report.SourceCounts.navigation_sources)
  return navigation_sources_;
}
inline void Report_SourceCounts::set_navigation_sources(::google::protobuf::int32 value) {
  set_has_navigation_sources();
  navigation_sources_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.SourceCounts.navigation_sources)
}

// optional int32 unmatched_sources = 3;
inline bool Report_SourceCounts::has_unmatched_sources() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Report_SourceCounts::set_has_unmatched_sources() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Report_SourceCounts::clear_has_unmatched_sources() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Report_SourceCounts::clear_unmatched_sources() {
  unmatched_sources_ = 0;
  clear_has_unmatched_sources();
}
inline ::google::protobuf::int32 Report_SourceCounts::unmatched_sources() const {
  // @@protoc_insertion_point(field_get:ukm.Report.SourceCounts.unmatched_sources)
  return unmatched_sources_;
}
inline void Report_SourceCounts::set_unmatched_sources(::google::protobuf::int32 value) {
  set_has_unmatched_sources();
  unmatched_sources_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.SourceCounts.unmatched_sources)
}

// optional int32 carryover_sources = 4;
inline bool Report_SourceCounts::has_carryover_sources() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Report_SourceCounts::set_has_carryover_sources() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Report_SourceCounts::clear_has_carryover_sources() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Report_SourceCounts::clear_carryover_sources() {
  carryover_sources_ = 0;
  clear_has_carryover_sources();
}
inline ::google::protobuf::int32 Report_SourceCounts::carryover_sources() const {
  // @@protoc_insertion_point(field_get:ukm.Report.SourceCounts.carryover_sources)
  return carryover_sources_;
}
inline void Report_SourceCounts::set_carryover_sources(::google::protobuf::int32 value) {
  set_has_carryover_sources();
  carryover_sources_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.SourceCounts.carryover_sources)
}

// optional int32 deferred_sources = 5;
inline bool Report_SourceCounts::has_deferred_sources() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Report_SourceCounts::set_has_deferred_sources() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Report_SourceCounts::clear_has_deferred_sources() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Report_SourceCounts::clear_deferred_sources() {
  deferred_sources_ = 0;
  clear_has_deferred_sources();
}
inline ::google::protobuf::int32 Report_SourceCounts::deferred_sources() const {
  // @@protoc_insertion_point(field_get:ukm.Report.SourceCounts.deferred_sources)
  return deferred_sources_;
}
inline void Report_SourceCounts::set_deferred_sources(::google::protobuf::int32 value) {
  set_has_deferred_sources();
  deferred_sources_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.SourceCounts.deferred_sources)
}

// -------------------------------------------------------------------

// Report

// optional fixed64 client_id = 1;
inline bool Report::has_client_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Report::set_has_client_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Report::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Report::clear_client_id() {
  client_id_ = GOOGLE_ULONGLONG(0);
  clear_has_client_id();
}
inline ::google::protobuf::uint64 Report::client_id() const {
  // @@protoc_insertion_point(field_get:ukm.Report.client_id)
  return client_id_;
}
inline void Report::set_client_id(::google::protobuf::uint64 value) {
  set_has_client_id();
  client_id_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.client_id)
}

// optional int32 session_id = 5;
inline bool Report::has_session_id() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Report::set_has_session_id() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Report::clear_has_session_id() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Report::clear_session_id() {
  session_id_ = 0;
  clear_has_session_id();
}
inline ::google::protobuf::int32 Report::session_id() const {
  // @@protoc_insertion_point(field_get:ukm.Report.session_id)
  return session_id_;
}
inline void Report::set_session_id(::google::protobuf::int32 value) {
  set_has_session_id();
  session_id_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.session_id)
}

// optional int32 report_id = 6;
inline bool Report::has_report_id() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Report::set_has_report_id() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Report::clear_has_report_id() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Report::clear_report_id() {
  report_id_ = 0;
  clear_has_report_id();
}
inline ::google::protobuf::int32 Report::report_id() const {
  // @@protoc_insertion_point(field_get:ukm.Report.report_id)
  return report_id_;
}
inline void Report::set_report_id(::google::protobuf::int32 value) {
  set_has_report_id();
  report_id_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.report_id)
}

// optional bool is_continuous = 8;
inline bool Report::has_is_continuous() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Report::set_has_is_continuous() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Report::clear_has_is_continuous() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Report::clear_is_continuous() {
  is_continuous_ = false;
  clear_has_is_continuous();
}
inline bool Report::is_continuous() const {
  // @@protoc_insertion_point(field_get:ukm.Report.is_continuous)
  return is_continuous_;
}
inline void Report::set_is_continuous(bool value) {
  set_has_is_continuous();
  is_continuous_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.is_continuous)
}

// optional .ukm.Report.LogRotationReason log_rotation_reason = 9;
inline bool Report::has_log_rotation_reason() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Report::set_has_log_rotation_reason() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Report::clear_has_log_rotation_reason() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Report::clear_log_rotation_reason() {
  log_rotation_reason_ = 0;
  clear_has_log_rotation_reason();
}
inline ::ukm::Report_LogRotationReason Report::log_rotation_reason() const {
  // @@protoc_insertion_point(field_get:ukm.Report.log_rotation_reason)
  return static_cast< ::ukm::Report_LogRotationReason >(log_rotation_reason_);
}
inline void Report::set_log_rotation_reason(::ukm::Report_LogRotationReason value) {
  assert(::ukm::Report_LogRotationReason_IsValid(value));
  set_has_log_rotation_reason();
  log_rotation_reason_ = value;
  // @@protoc_insertion_point(field_set:ukm.Report.log_rotation_reason)
}

// optional .metrics.SystemProfileProto system_profile = 2;
inline bool Report::has_system_profile() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Report::set_has_system_profile() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Report::clear_has_system_profile() {
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::metrics::SystemProfileProto& Report::_internal_system_profile() const {
  return *system_profile_;
}
inline const ::metrics::SystemProfileProto& Report::system_profile() const {
  const ::metrics::SystemProfileProto* p = system_profile_;
  // @@protoc_insertion_point(field_get:ukm.Report.system_profile)
  return p != NULL ? *p : *reinterpret_cast<const ::metrics::SystemProfileProto*>(
      &::metrics::_SystemProfileProto_default_instance_);
}
inline ::metrics::SystemProfileProto* Report::release_system_profile() {
  // @@protoc_insertion_point(field_release:ukm.Report.system_profile)
  clear_has_system_profile();
  ::metrics::SystemProfileProto* temp = system_profile_;
  system_profile_ = NULL;
  return temp;
}
inline ::metrics::SystemProfileProto* Report::mutable_system_profile() {
  set_has_system_profile();
  if (system_profile_ == NULL) {
    auto* p = CreateMaybeMessage<::metrics::SystemProfileProto>(GetArenaNoVirtual());
    system_profile_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ukm.Report.system_profile)
  return system_profile_;
}
inline void Report::set_allocated_system_profile(::metrics::SystemProfileProto* system_profile) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(system_profile_);
  }
  if (system_profile) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      system_profile = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, system_profile, submessage_arena);
    }
    set_has_system_profile();
  } else {
    clear_has_system_profile();
  }
  system_profile_ = system_profile;
  // @@protoc_insertion_point(field_set_allocated:ukm.Report.system_profile)
}

// repeated .ukm.Source sources = 3;
inline int Report::sources_size() const {
  return sources_.size();
}
inline ::ukm::Source* Report::mutable_sources(int index) {
  // @@protoc_insertion_point(field_mutable:ukm.Report.sources)
  return sources_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::ukm::Source >*
Report::mutable_sources() {
  // @@protoc_insertion_point(field_mutable_list:ukm.Report.sources)
  return &sources_;
}
inline const ::ukm::Source& Report::sources(int index) const {
  // @@protoc_insertion_point(field_get:ukm.Report.sources)
  return sources_.Get(index);
}
inline ::ukm::Source* Report::add_sources() {
  // @@protoc_insertion_point(field_add:ukm.Report.sources)
  return sources_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ukm::Source >&
Report::sources() const {
  // @@protoc_insertion_point(field_list:ukm.Report.sources)
  return sources_;
}

// optional .ukm.Report.SourceCounts source_counts = 10;
inline bool Report::has_source_counts() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Report::set_has_source_counts() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Report::clear_has_source_counts() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Report::clear_source_counts() {
  if (source_counts_ != NULL) source_counts_->Clear();
  clear_has_source_counts();
}
inline const ::ukm::Report_SourceCounts& Report::_internal_source_counts() const {
  return *source_counts_;
}
inline const ::ukm::Report_SourceCounts& Report::source_counts() const {
  const ::ukm::Report_SourceCounts* p = source_counts_;
  // @@protoc_insertion_point(field_get:ukm.Report.source_counts)
  return p != NULL ? *p : *reinterpret_cast<const ::ukm::Report_SourceCounts*>(
      &::ukm::_Report_SourceCounts_default_instance_);
}
inline ::ukm::Report_SourceCounts* Report::release_source_counts() {
  // @@protoc_insertion_point(field_release:ukm.Report.source_counts)
  clear_has_source_counts();
  ::ukm::Report_SourceCounts* temp = source_counts_;
  source_counts_ = NULL;
  return temp;
}
inline ::ukm::Report_SourceCounts* Report::mutable_source_counts() {
  set_has_source_counts();
  if (source_counts_ == NULL) {
    auto* p = CreateMaybeMessage<::ukm::Report_SourceCounts>(GetArenaNoVirtual());
    source_counts_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ukm.Report.source_counts)
  return source_counts_;
}
inline void Report::set_allocated_source_counts(::ukm::Report_SourceCounts* source_counts) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete source_counts_;
  }
  if (source_counts) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      source_counts = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, source_counts, submessage_arena);
    }
    set_has_source_counts();
  } else {
    clear_has_source_counts();
  }
  source_counts_ = source_counts;
  // @@protoc_insertion_point(field_set_allocated:ukm.Report.source_counts)
}

// repeated .ukm.Entry entries = 4;
inline int Report::entries_size() const {
  return entries_.size();
}
inline ::ukm::Entry* Report::mutable_entries(int index) {
  // @@protoc_insertion_point(field_mutable:ukm.Report.entries)
  return entries_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::ukm::Entry >*
Report::mutable_entries() {
  // @@protoc_insertion_point(field_mutable_list:ukm.Report.entries)
  return &entries_;
}
inline const ::ukm::Entry& Report::entries(int index) const {
  // @@protoc_insertion_point(field_get:ukm.Report.entries)
  return entries_.Get(index);
}
inline ::ukm::Entry* Report::add_entries() {
  // @@protoc_insertion_point(field_add:ukm.Report.entries)
  return entries_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ukm::Entry >&
Report::entries() const {
  // @@protoc_insertion_point(field_list:ukm.Report.entries)
  return entries_;
}

// repeated .ukm.Aggregate aggregates = 7;
inline int Report::aggregates_size() const {
  return aggregates_.size();
}
inline ::ukm::Aggregate* Report::mutable_aggregates(int index) {
  // @@protoc_insertion_point(field_mutable:ukm.Report.aggregates)
  return aggregates_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::ukm::Aggregate >*
Report::mutable_aggregates() {
  // @@protoc_insertion_point(field_mutable_list:ukm.Report.aggregates)
  return &aggregates_;
}
inline const ::ukm::Aggregate& Report::aggregates(int index) const {
  // @@protoc_insertion_point(field_get:ukm.Report.aggregates)
  return aggregates_.Get(index);
}
inline ::ukm::Aggregate* Report::add_aggregates() {
  // @@protoc_insertion_point(field_add:ukm.Report.aggregates)
  return aggregates_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ukm::Aggregate >&
Report::aggregates() const {
  // @@protoc_insertion_point(field_list:ukm.Report.aggregates)
  return aggregates_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ukm

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ukm::Report_LogRotationReason> : ::std::true_type {};

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_ukm_2freport_2eproto
