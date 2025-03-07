// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client_variations.proto

#ifndef PROTOBUF_INCLUDED_client_5fvariations_2eproto
#define PROTOBUF_INCLUDED_client_5fvariations_2eproto

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
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_client_5fvariations_2eproto 

namespace protobuf_client_5fvariations_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
}  // namespace protobuf_client_5fvariations_2eproto
namespace variations {
class ClientVariations;
class ClientVariationsDefaultTypeInternal;
extern ClientVariationsDefaultTypeInternal _ClientVariations_default_instance_;
}  // namespace variations
namespace google {
namespace protobuf {
template<> ::variations::ClientVariations* Arena::CreateMaybeMessage<::variations::ClientVariations>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace variations {

// ===================================================================

class ClientVariations : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:variations.ClientVariations) */ {
 public:
  ClientVariations();
  virtual ~ClientVariations();

  ClientVariations(const ClientVariations& from);

  inline ClientVariations& operator=(const ClientVariations& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ClientVariations(ClientVariations&& from) noexcept
    : ClientVariations() {
    *this = ::std::move(from);
  }

  inline ClientVariations& operator=(ClientVariations&& from) noexcept {
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

  static const ClientVariations& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ClientVariations* internal_default_instance() {
    return reinterpret_cast<const ClientVariations*>(
               &_ClientVariations_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  GOOGLE_ATTRIBUTE_NOINLINE void Swap(ClientVariations* other);
  friend void swap(ClientVariations& a, ClientVariations& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ClientVariations* New() const final {
    return CreateMaybeMessage<ClientVariations>(NULL);
  }

  ClientVariations* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ClientVariations>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    final;
  void CopyFrom(const ClientVariations& from);
  void MergeFrom(const ClientVariations& from);
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
  void InternalSwap(ClientVariations* other);
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

  // repeated int32 variation_id = 1;
  int variation_id_size() const;
  void clear_variation_id();
  static const int kVariationIdFieldNumber = 1;
  ::google::protobuf::int32 variation_id(int index) const;
  void set_variation_id(int index, ::google::protobuf::int32 value);
  void add_variation_id(::google::protobuf::int32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      variation_id() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_variation_id();

  // repeated int32 trigger_variation_id = 3;
  int trigger_variation_id_size() const;
  void clear_trigger_variation_id();
  static const int kTriggerVariationIdFieldNumber = 3;
  ::google::protobuf::int32 trigger_variation_id(int index) const;
  void set_trigger_variation_id(int index, ::google::protobuf::int32 value);
  void add_trigger_variation_id(::google::protobuf::int32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      trigger_variation_id() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_trigger_variation_id();

  // @@protoc_insertion_point(class_scope:variations.ClientVariations)
 private:

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > variation_id_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > trigger_variation_id_;
  friend struct ::protobuf_client_5fvariations_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ClientVariations

// repeated int32 variation_id = 1;
inline int ClientVariations::variation_id_size() const {
  return variation_id_.size();
}
inline void ClientVariations::clear_variation_id() {
  variation_id_.Clear();
}
inline ::google::protobuf::int32 ClientVariations::variation_id(int index) const {
  // @@protoc_insertion_point(field_get:variations.ClientVariations.variation_id)
  return variation_id_.Get(index);
}
inline void ClientVariations::set_variation_id(int index, ::google::protobuf::int32 value) {
  variation_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:variations.ClientVariations.variation_id)
}
inline void ClientVariations::add_variation_id(::google::protobuf::int32 value) {
  variation_id_.Add(value);
  // @@protoc_insertion_point(field_add:variations.ClientVariations.variation_id)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
ClientVariations::variation_id() const {
  // @@protoc_insertion_point(field_list:variations.ClientVariations.variation_id)
  return variation_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
ClientVariations::mutable_variation_id() {
  // @@protoc_insertion_point(field_mutable_list:variations.ClientVariations.variation_id)
  return &variation_id_;
}

// repeated int32 trigger_variation_id = 3;
inline int ClientVariations::trigger_variation_id_size() const {
  return trigger_variation_id_.size();
}
inline void ClientVariations::clear_trigger_variation_id() {
  trigger_variation_id_.Clear();
}
inline ::google::protobuf::int32 ClientVariations::trigger_variation_id(int index) const {
  // @@protoc_insertion_point(field_get:variations.ClientVariations.trigger_variation_id)
  return trigger_variation_id_.Get(index);
}
inline void ClientVariations::set_trigger_variation_id(int index, ::google::protobuf::int32 value) {
  trigger_variation_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:variations.ClientVariations.trigger_variation_id)
}
inline void ClientVariations::add_trigger_variation_id(::google::protobuf::int32 value) {
  trigger_variation_id_.Add(value);
  // @@protoc_insertion_point(field_add:variations.ClientVariations.trigger_variation_id)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
ClientVariations::trigger_variation_id() const {
  // @@protoc_insertion_point(field_list:variations.ClientVariations.trigger_variation_id)
  return trigger_variation_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
ClientVariations::mutable_trigger_variation_id() {
  // @@protoc_insertion_point(field_mutable_list:variations.ClientVariations.trigger_variation_id)
  return &trigger_variation_id_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace variations

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_client_5fvariations_2eproto
