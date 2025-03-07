// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/common/observable_events.proto

#ifndef PROTOBUF_INCLUDED_perfetto_2fcommon_2fobservable_5fevents_2eproto
#define PROTOBUF_INCLUDED_perfetto_2fcommon_2fobservable_5fevents_2eproto

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
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2fcommon_2fobservable_5fevents_2eproto 

namespace protobuf_perfetto_2fcommon_2fobservable_5fevents_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
}  // namespace protobuf_perfetto_2fcommon_2fobservable_5fevents_2eproto
namespace perfetto {
namespace protos {
class ObservableEvents;
class ObservableEventsDefaultTypeInternal;
extern ObservableEventsDefaultTypeInternal _ObservableEvents_default_instance_;
class ObservableEvents_DataSourceInstanceStateChange;
class ObservableEvents_DataSourceInstanceStateChangeDefaultTypeInternal;
extern ObservableEvents_DataSourceInstanceStateChangeDefaultTypeInternal _ObservableEvents_DataSourceInstanceStateChange_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> ::perfetto::protos::ObservableEvents* Arena::CreateMaybeMessage<::perfetto::protos::ObservableEvents>(Arena*);
template<> ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange* Arena::CreateMaybeMessage<::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace perfetto {
namespace protos {

enum ObservableEvents_Type {
  ObservableEvents_Type_TYPE_UNSPECIFIED = 0,
  ObservableEvents_Type_TYPE_DATA_SOURCES_INSTANCES = 1
};
bool ObservableEvents_Type_IsValid(int value);
const ObservableEvents_Type ObservableEvents_Type_Type_MIN = ObservableEvents_Type_TYPE_UNSPECIFIED;
const ObservableEvents_Type ObservableEvents_Type_Type_MAX = ObservableEvents_Type_TYPE_DATA_SOURCES_INSTANCES;
const int ObservableEvents_Type_Type_ARRAYSIZE = ObservableEvents_Type_Type_MAX + 1;

enum ObservableEvents_DataSourceInstanceState {
  ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STOPPED = 1,
  ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STARTED = 2
};
bool ObservableEvents_DataSourceInstanceState_IsValid(int value);
const ObservableEvents_DataSourceInstanceState ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_MIN = ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STOPPED;
const ObservableEvents_DataSourceInstanceState ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_MAX = ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STARTED;
const int ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_ARRAYSIZE = ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_MAX + 1;

// ===================================================================

class ObservableEvents_DataSourceInstanceStateChange : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange) */ {
 public:
  ObservableEvents_DataSourceInstanceStateChange();
  virtual ~ObservableEvents_DataSourceInstanceStateChange();

  ObservableEvents_DataSourceInstanceStateChange(const ObservableEvents_DataSourceInstanceStateChange& from);

  inline ObservableEvents_DataSourceInstanceStateChange& operator=(const ObservableEvents_DataSourceInstanceStateChange& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ObservableEvents_DataSourceInstanceStateChange(ObservableEvents_DataSourceInstanceStateChange&& from) noexcept
    : ObservableEvents_DataSourceInstanceStateChange() {
    *this = ::std::move(from);
  }

  inline ObservableEvents_DataSourceInstanceStateChange& operator=(ObservableEvents_DataSourceInstanceStateChange&& from) noexcept {
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

  static const ObservableEvents_DataSourceInstanceStateChange& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ObservableEvents_DataSourceInstanceStateChange* internal_default_instance() {
    return reinterpret_cast<const ObservableEvents_DataSourceInstanceStateChange*>(
               &_ObservableEvents_DataSourceInstanceStateChange_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(ObservableEvents_DataSourceInstanceStateChange* other);
  friend void swap(ObservableEvents_DataSourceInstanceStateChange& a, ObservableEvents_DataSourceInstanceStateChange& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ObservableEvents_DataSourceInstanceStateChange* New() const final {
    return CreateMaybeMessage<ObservableEvents_DataSourceInstanceStateChange>(NULL);
  }

  ObservableEvents_DataSourceInstanceStateChange* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ObservableEvents_DataSourceInstanceStateChange>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const ObservableEvents_DataSourceInstanceStateChange& from);
  void MergeFrom(const ObservableEvents_DataSourceInstanceStateChange& from);
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
  void InternalSwap(ObservableEvents_DataSourceInstanceStateChange* other);
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

  // optional string producer_name = 1;
  bool has_producer_name() const;
  void clear_producer_name();
  static const int kProducerNameFieldNumber = 1;
  const ::std::string& producer_name() const;
  void set_producer_name(const ::std::string& value);
  #if LANG_CXX11
  void set_producer_name(::std::string&& value);
  #endif
  void set_producer_name(const char* value);
  void set_producer_name(const char* value, size_t size);
  ::std::string* mutable_producer_name();
  ::std::string* release_producer_name();
  void set_allocated_producer_name(::std::string* producer_name);

  // optional string data_source_name = 2;
  bool has_data_source_name() const;
  void clear_data_source_name();
  static const int kDataSourceNameFieldNumber = 2;
  const ::std::string& data_source_name() const;
  void set_data_source_name(const ::std::string& value);
  #if LANG_CXX11
  void set_data_source_name(::std::string&& value);
  #endif
  void set_data_source_name(const char* value);
  void set_data_source_name(const char* value, size_t size);
  ::std::string* mutable_data_source_name();
  ::std::string* release_data_source_name();
  void set_allocated_data_source_name(::std::string* data_source_name);

  // optional .perfetto.protos.ObservableEvents.DataSourceInstanceState state = 3;
  bool has_state() const;
  void clear_state();
  static const int kStateFieldNumber = 3;
  ::perfetto::protos::ObservableEvents_DataSourceInstanceState state() const;
  void set_state(::perfetto::protos::ObservableEvents_DataSourceInstanceState value);

  // @@protoc_insertion_point(class_scope:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange)
 private:
  void set_has_producer_name();
  void clear_has_producer_name();
  void set_has_data_source_name();
  void clear_has_data_source_name();
  void set_has_state();
  void clear_has_state();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr producer_name_;
  ::google::protobuf::internal::ArenaStringPtr data_source_name_;
  int state_;
  friend struct ::protobuf_perfetto_2fcommon_2fobservable_5fevents_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class ObservableEvents : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:perfetto.protos.ObservableEvents) */ {
 public:
  ObservableEvents();
  virtual ~ObservableEvents();

  ObservableEvents(const ObservableEvents& from);

  inline ObservableEvents& operator=(const ObservableEvents& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ObservableEvents(ObservableEvents&& from) noexcept
    : ObservableEvents() {
    *this = ::std::move(from);
  }

  inline ObservableEvents& operator=(ObservableEvents&& from) noexcept {
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

  static const ObservableEvents& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ObservableEvents* internal_default_instance() {
    return reinterpret_cast<const ObservableEvents*>(
               &_ObservableEvents_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(ObservableEvents* other);
  friend void swap(ObservableEvents& a, ObservableEvents& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ObservableEvents* New() const final {
    return CreateMaybeMessage<ObservableEvents>(NULL);
  }

  ObservableEvents* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ObservableEvents>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const ObservableEvents& from);
  void MergeFrom(const ObservableEvents& from);
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
  void InternalSwap(ObservableEvents* other);
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

  typedef ObservableEvents_DataSourceInstanceStateChange DataSourceInstanceStateChange;

  typedef ObservableEvents_Type Type;
  static const Type TYPE_UNSPECIFIED =
    ObservableEvents_Type_TYPE_UNSPECIFIED;
  static const Type TYPE_DATA_SOURCES_INSTANCES =
    ObservableEvents_Type_TYPE_DATA_SOURCES_INSTANCES;
  static inline bool Type_IsValid(int value) {
    return ObservableEvents_Type_IsValid(value);
  }
  static const Type Type_MIN =
    ObservableEvents_Type_Type_MIN;
  static const Type Type_MAX =
    ObservableEvents_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    ObservableEvents_Type_Type_ARRAYSIZE;

  typedef ObservableEvents_DataSourceInstanceState DataSourceInstanceState;
  static const DataSourceInstanceState DATA_SOURCE_INSTANCE_STATE_STOPPED =
    ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STOPPED;
  static const DataSourceInstanceState DATA_SOURCE_INSTANCE_STATE_STARTED =
    ObservableEvents_DataSourceInstanceState_DATA_SOURCE_INSTANCE_STATE_STARTED;
  static inline bool DataSourceInstanceState_IsValid(int value) {
    return ObservableEvents_DataSourceInstanceState_IsValid(value);
  }
  static const DataSourceInstanceState DataSourceInstanceState_MIN =
    ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_MIN;
  static const DataSourceInstanceState DataSourceInstanceState_MAX =
    ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_MAX;
  static const int DataSourceInstanceState_ARRAYSIZE =
    ObservableEvents_DataSourceInstanceState_DataSourceInstanceState_ARRAYSIZE;

  // accessors -------------------------------------------------------

  // repeated .perfetto.protos.ObservableEvents.DataSourceInstanceStateChange instance_state_changes = 1;
  int instance_state_changes_size() const;
  void clear_instance_state_changes();
  static const int kInstanceStateChangesFieldNumber = 1;
  ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange* mutable_instance_state_changes(int index);
  ::google::protobuf::RepeatedPtrField< ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange >*
      mutable_instance_state_changes();
  const ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange& instance_state_changes(int index) const;
  ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange* add_instance_state_changes();
  const ::google::protobuf::RepeatedPtrField< ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange >&
      instance_state_changes() const;

  // @@protoc_insertion_point(class_scope:perfetto.protos.ObservableEvents)
 private:

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange > instance_state_changes_;
  friend struct ::protobuf_perfetto_2fcommon_2fobservable_5fevents_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ObservableEvents_DataSourceInstanceStateChange

// optional string producer_name = 1;
inline bool ObservableEvents_DataSourceInstanceStateChange::has_producer_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_has_producer_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_has_producer_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_producer_name() {
  producer_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_producer_name();
}
inline const ::std::string& ObservableEvents_DataSourceInstanceStateChange::producer_name() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
  return producer_name_.GetNoArena();
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_producer_name(const ::std::string& value) {
  set_has_producer_name();
  producer_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
}
#if LANG_CXX11
inline void ObservableEvents_DataSourceInstanceStateChange::set_producer_name(::std::string&& value) {
  set_has_producer_name();
  producer_name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
}
#endif
inline void ObservableEvents_DataSourceInstanceStateChange::set_producer_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_producer_name();
  producer_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_producer_name(const char* value, size_t size) {
  set_has_producer_name();
  producer_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
}
inline ::std::string* ObservableEvents_DataSourceInstanceStateChange::mutable_producer_name() {
  set_has_producer_name();
  // @@protoc_insertion_point(field_mutable:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
  return producer_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ObservableEvents_DataSourceInstanceStateChange::release_producer_name() {
  // @@protoc_insertion_point(field_release:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
  if (!has_producer_name()) {
    return NULL;
  }
  clear_has_producer_name();
  return producer_name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_allocated_producer_name(::std::string* producer_name) {
  if (producer_name != NULL) {
    set_has_producer_name();
  } else {
    clear_has_producer_name();
  }
  producer_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), producer_name);
  // @@protoc_insertion_point(field_set_allocated:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.producer_name)
}

// optional string data_source_name = 2;
inline bool ObservableEvents_DataSourceInstanceStateChange::has_data_source_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_has_data_source_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_has_data_source_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_data_source_name() {
  data_source_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_data_source_name();
}
inline const ::std::string& ObservableEvents_DataSourceInstanceStateChange::data_source_name() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
  return data_source_name_.GetNoArena();
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_data_source_name(const ::std::string& value) {
  set_has_data_source_name();
  data_source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
}
#if LANG_CXX11
inline void ObservableEvents_DataSourceInstanceStateChange::set_data_source_name(::std::string&& value) {
  set_has_data_source_name();
  data_source_name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
}
#endif
inline void ObservableEvents_DataSourceInstanceStateChange::set_data_source_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_data_source_name();
  data_source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_data_source_name(const char* value, size_t size) {
  set_has_data_source_name();
  data_source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
}
inline ::std::string* ObservableEvents_DataSourceInstanceStateChange::mutable_data_source_name() {
  set_has_data_source_name();
  // @@protoc_insertion_point(field_mutable:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
  return data_source_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ObservableEvents_DataSourceInstanceStateChange::release_data_source_name() {
  // @@protoc_insertion_point(field_release:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
  if (!has_data_source_name()) {
    return NULL;
  }
  clear_has_data_source_name();
  return data_source_name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_allocated_data_source_name(::std::string* data_source_name) {
  if (data_source_name != NULL) {
    set_has_data_source_name();
  } else {
    clear_has_data_source_name();
  }
  data_source_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), data_source_name);
  // @@protoc_insertion_point(field_set_allocated:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.data_source_name)
}

// optional .perfetto.protos.ObservableEvents.DataSourceInstanceState state = 3;
inline bool ObservableEvents_DataSourceInstanceStateChange::has_state() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_has_state() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_has_state() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ObservableEvents_DataSourceInstanceStateChange::clear_state() {
  state_ = 1;
  clear_has_state();
}
inline ::perfetto::protos::ObservableEvents_DataSourceInstanceState ObservableEvents_DataSourceInstanceStateChange::state() const {
  // @@protoc_insertion_point(field_get:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.state)
  return static_cast< ::perfetto::protos::ObservableEvents_DataSourceInstanceState >(state_);
}
inline void ObservableEvents_DataSourceInstanceStateChange::set_state(::perfetto::protos::ObservableEvents_DataSourceInstanceState value) {
  assert(::perfetto::protos::ObservableEvents_DataSourceInstanceState_IsValid(value));
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:perfetto.protos.ObservableEvents.DataSourceInstanceStateChange.state)
}

// -------------------------------------------------------------------

// ObservableEvents

// repeated .perfetto.protos.ObservableEvents.DataSourceInstanceStateChange instance_state_changes = 1;
inline int ObservableEvents::instance_state_changes_size() const {
  return instance_state_changes_.size();
}
inline void ObservableEvents::clear_instance_state_changes() {
  instance_state_changes_.Clear();
}
inline ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange* ObservableEvents::mutable_instance_state_changes(int index) {
  // @@protoc_insertion_point(field_mutable:perfetto.protos.ObservableEvents.instance_state_changes)
  return instance_state_changes_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange >*
ObservableEvents::mutable_instance_state_changes() {
  // @@protoc_insertion_point(field_mutable_list:perfetto.protos.ObservableEvents.instance_state_changes)
  return &instance_state_changes_;
}
inline const ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange& ObservableEvents::instance_state_changes(int index) const {
  // @@protoc_insertion_point(field_get:perfetto.protos.ObservableEvents.instance_state_changes)
  return instance_state_changes_.Get(index);
}
inline ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange* ObservableEvents::add_instance_state_changes() {
  // @@protoc_insertion_point(field_add:perfetto.protos.ObservableEvents.instance_state_changes)
  return instance_state_changes_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::perfetto::protos::ObservableEvents_DataSourceInstanceStateChange >&
ObservableEvents::instance_state_changes() const {
  // @@protoc_insertion_point(field_list:perfetto.protos.ObservableEvents.instance_state_changes)
  return instance_state_changes_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protos
}  // namespace perfetto

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::perfetto::protos::ObservableEvents_Type> : ::std::true_type {};
template <> struct is_proto_enum< ::perfetto::protos::ObservableEvents_DataSourceInstanceState> : ::std::true_type {};

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_perfetto_2fcommon_2fobservable_5fevents_2eproto
