// Autogenerated by the ProtoZero compiler plugin. DO NOT EDIT.

#ifndef PERFETTO_PROTOS_PERFETTO_COMMON_DESCRIPTOR_PROTO_H_
#define PERFETTO_PROTOS_PERFETTO_COMMON_DESCRIPTOR_PROTO_H_

#include <stddef.h>
#include <stdint.h>

#include "perfetto/base/export.h"
#include "perfetto/protozero/proto_decoder.h"
#include "perfetto/protozero/message.h"

namespace perfetto {
namespace protos {
namespace pbzero {

class DescriptorProto;
class DescriptorProto_ReservedRange;
class EnumDescriptorProto;
class EnumValueDescriptorProto;
class FieldDescriptorProto;
class FileDescriptorProto;
class OneofDescriptorProto;
class OneofOptions;
enum FieldDescriptorProto_Label : int32_t;
enum FieldDescriptorProto_Type : int32_t;

enum FieldDescriptorProto_Type : int32_t {
  FieldDescriptorProto_Type_TYPE_DOUBLE = 1,
  FieldDescriptorProto_Type_TYPE_FLOAT = 2,
  FieldDescriptorProto_Type_TYPE_INT64 = 3,
  FieldDescriptorProto_Type_TYPE_UINT64 = 4,
  FieldDescriptorProto_Type_TYPE_INT32 = 5,
  FieldDescriptorProto_Type_TYPE_FIXED64 = 6,
  FieldDescriptorProto_Type_TYPE_FIXED32 = 7,
  FieldDescriptorProto_Type_TYPE_BOOL = 8,
  FieldDescriptorProto_Type_TYPE_STRING = 9,
  FieldDescriptorProto_Type_TYPE_GROUP = 10,
  FieldDescriptorProto_Type_TYPE_MESSAGE = 11,
  FieldDescriptorProto_Type_TYPE_BYTES = 12,
  FieldDescriptorProto_Type_TYPE_UINT32 = 13,
  FieldDescriptorProto_Type_TYPE_ENUM = 14,
  FieldDescriptorProto_Type_TYPE_SFIXED32 = 15,
  FieldDescriptorProto_Type_TYPE_SFIXED64 = 16,
  FieldDescriptorProto_Type_TYPE_SINT32 = 17,
  FieldDescriptorProto_Type_TYPE_SINT64 = 18,
};

enum FieldDescriptorProto_Label : int32_t {
  FieldDescriptorProto_Label_LABEL_OPTIONAL = 1,
  FieldDescriptorProto_Label_LABEL_REQUIRED = 2,
  FieldDescriptorProto_Label_LABEL_REPEATED = 3,
};

class PERFETTO_EXPORT OneofOptions : public ::protozero::Message {
 public:
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/0, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
  };
};

class PERFETTO_EXPORT EnumValueDescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kNumberFieldNumber = 2,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/2, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_number() const { return at<2>().valid(); }
    int32_t number() const { return at<2>().as_int32(); }
  };
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  void set_number(int32_t value) {
    AppendVarInt(2, value);
  }
};

class PERFETTO_EXPORT EnumDescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kValueFieldNumber = 2,
    kReservedNameFieldNumber = 5,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/5, /*HAS_REPEATED_FIELDS=*/true> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_value() const { return at<2>().valid(); }
    ::protozero::RepeatedFieldIterator value() const { return GetRepeated(2); }
    bool has_reserved_name() const { return at<5>().valid(); }
    ::protozero::RepeatedFieldIterator reserved_name() const { return GetRepeated(5); }
  };
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  template <typename T = EnumValueDescriptorProto> T* add_value() {
    return BeginNestedMessage<T>(2);
  }

  void add_reserved_name(const char* value) {
    AppendString(5, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void add_reserved_name(const char* value, size_t size) {
    AppendBytes(5, value, size);
  }
};

class PERFETTO_EXPORT OneofDescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kOptionsFieldNumber = 2,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/2, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_options() const { return at<2>().valid(); }
    ::protozero::ConstBytes options() const { return at<2>().as_bytes(); }
  };
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  template <typename T = OneofOptions> T* set_options() {
    return BeginNestedMessage<T>(2);
  }

};

class PERFETTO_EXPORT FieldDescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kNumberFieldNumber = 3,
    kLabelFieldNumber = 4,
    kTypeFieldNumber = 5,
    kTypeNameFieldNumber = 6,
    kDefaultValueFieldNumber = 7,
    kOneofIndexFieldNumber = 9,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/9, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_number() const { return at<3>().valid(); }
    int32_t number() const { return at<3>().as_int32(); }
    bool has_label() const { return at<4>().valid(); }
    int32_t label() const { return at<4>().as_int32(); }
    bool has_type() const { return at<5>().valid(); }
    int32_t type() const { return at<5>().as_int32(); }
    bool has_type_name() const { return at<6>().valid(); }
    ::protozero::ConstChars type_name() const { return at<6>().as_string(); }
    bool has_default_value() const { return at<7>().valid(); }
    ::protozero::ConstChars default_value() const { return at<7>().as_string(); }
    bool has_oneof_index() const { return at<9>().valid(); }
    int32_t oneof_index() const { return at<9>().as_int32(); }
  };
  using Type = ::perfetto::protos::pbzero::FieldDescriptorProto_Type;
  using Label = ::perfetto::protos::pbzero::FieldDescriptorProto_Label;
  static const Type TYPE_DOUBLE = FieldDescriptorProto_Type_TYPE_DOUBLE;
  static const Type TYPE_FLOAT = FieldDescriptorProto_Type_TYPE_FLOAT;
  static const Type TYPE_INT64 = FieldDescriptorProto_Type_TYPE_INT64;
  static const Type TYPE_UINT64 = FieldDescriptorProto_Type_TYPE_UINT64;
  static const Type TYPE_INT32 = FieldDescriptorProto_Type_TYPE_INT32;
  static const Type TYPE_FIXED64 = FieldDescriptorProto_Type_TYPE_FIXED64;
  static const Type TYPE_FIXED32 = FieldDescriptorProto_Type_TYPE_FIXED32;
  static const Type TYPE_BOOL = FieldDescriptorProto_Type_TYPE_BOOL;
  static const Type TYPE_STRING = FieldDescriptorProto_Type_TYPE_STRING;
  static const Type TYPE_GROUP = FieldDescriptorProto_Type_TYPE_GROUP;
  static const Type TYPE_MESSAGE = FieldDescriptorProto_Type_TYPE_MESSAGE;
  static const Type TYPE_BYTES = FieldDescriptorProto_Type_TYPE_BYTES;
  static const Type TYPE_UINT32 = FieldDescriptorProto_Type_TYPE_UINT32;
  static const Type TYPE_ENUM = FieldDescriptorProto_Type_TYPE_ENUM;
  static const Type TYPE_SFIXED32 = FieldDescriptorProto_Type_TYPE_SFIXED32;
  static const Type TYPE_SFIXED64 = FieldDescriptorProto_Type_TYPE_SFIXED64;
  static const Type TYPE_SINT32 = FieldDescriptorProto_Type_TYPE_SINT32;
  static const Type TYPE_SINT64 = FieldDescriptorProto_Type_TYPE_SINT64;
  static const Label LABEL_OPTIONAL = FieldDescriptorProto_Label_LABEL_OPTIONAL;
  static const Label LABEL_REQUIRED = FieldDescriptorProto_Label_LABEL_REQUIRED;
  static const Label LABEL_REPEATED = FieldDescriptorProto_Label_LABEL_REPEATED;
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  void set_number(int32_t value) {
    AppendVarInt(3, value);
  }
  void set_label(::perfetto::protos::pbzero::FieldDescriptorProto_Label value) {
    AppendTinyVarInt(4, value);
  }
  void set_type(::perfetto::protos::pbzero::FieldDescriptorProto_Type value) {
    AppendTinyVarInt(5, value);
  }
  void set_type_name(const char* value) {
    AppendString(6, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_type_name(const char* value, size_t size) {
    AppendBytes(6, value, size);
  }
  void set_default_value(const char* value) {
    AppendString(7, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_default_value(const char* value, size_t size) {
    AppendBytes(7, value, size);
  }
  void set_oneof_index(int32_t value) {
    AppendVarInt(9, value);
  }
};

class PERFETTO_EXPORT DescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kFieldFieldNumber = 2,
    kExtensionFieldNumber = 6,
    kNestedTypeFieldNumber = 3,
    kEnumTypeFieldNumber = 4,
    kOneofDeclFieldNumber = 8,
    kReservedRangeFieldNumber = 9,
    kReservedNameFieldNumber = 10,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/10, /*HAS_REPEATED_FIELDS=*/true> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_field() const { return at<2>().valid(); }
    ::protozero::RepeatedFieldIterator field() const { return GetRepeated(2); }
    bool has_extension() const { return at<6>().valid(); }
    ::protozero::RepeatedFieldIterator extension() const { return GetRepeated(6); }
    bool has_nested_type() const { return at<3>().valid(); }
    ::protozero::RepeatedFieldIterator nested_type() const { return GetRepeated(3); }
    bool has_enum_type() const { return at<4>().valid(); }
    ::protozero::RepeatedFieldIterator enum_type() const { return GetRepeated(4); }
    bool has_oneof_decl() const { return at<8>().valid(); }
    ::protozero::RepeatedFieldIterator oneof_decl() const { return GetRepeated(8); }
    bool has_reserved_range() const { return at<9>().valid(); }
    ::protozero::RepeatedFieldIterator reserved_range() const { return GetRepeated(9); }
    bool has_reserved_name() const { return at<10>().valid(); }
    ::protozero::RepeatedFieldIterator reserved_name() const { return GetRepeated(10); }
  };
  using ReservedRange = ::perfetto::protos::pbzero::DescriptorProto_ReservedRange;
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  template <typename T = FieldDescriptorProto> T* add_field() {
    return BeginNestedMessage<T>(2);
  }

  template <typename T = FieldDescriptorProto> T* add_extension() {
    return BeginNestedMessage<T>(6);
  }

  template <typename T = DescriptorProto> T* add_nested_type() {
    return BeginNestedMessage<T>(3);
  }

  template <typename T = EnumDescriptorProto> T* add_enum_type() {
    return BeginNestedMessage<T>(4);
  }

  template <typename T = OneofDescriptorProto> T* add_oneof_decl() {
    return BeginNestedMessage<T>(8);
  }

  template <typename T = DescriptorProto_ReservedRange> T* add_reserved_range() {
    return BeginNestedMessage<T>(9);
  }

  void add_reserved_name(const char* value) {
    AppendString(10, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void add_reserved_name(const char* value, size_t size) {
    AppendBytes(10, value, size);
  }
};

class PERFETTO_EXPORT DescriptorProto_ReservedRange : public ::protozero::Message {
 public:
  enum : int32_t {
    kStartFieldNumber = 1,
    kEndFieldNumber = 2,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/2, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_start() const { return at<1>().valid(); }
    int32_t start() const { return at<1>().as_int32(); }
    bool has_end() const { return at<2>().valid(); }
    int32_t end() const { return at<2>().as_int32(); }
  };
  void set_start(int32_t value) {
    AppendVarInt(1, value);
  }
  void set_end(int32_t value) {
    AppendVarInt(2, value);
  }
};

class PERFETTO_EXPORT FileDescriptorProto : public ::protozero::Message {
 public:
  enum : int32_t {
    kNameFieldNumber = 1,
    kPackageFieldNumber = 2,
    kDependencyFieldNumber = 3,
    kPublicDependencyFieldNumber = 10,
    kWeakDependencyFieldNumber = 11,
    kMessageTypeFieldNumber = 4,
    kEnumTypeFieldNumber = 5,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/11, /*HAS_REPEATED_FIELDS=*/true> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_name() const { return at<1>().valid(); }
    ::protozero::ConstChars name() const { return at<1>().as_string(); }
    bool has_package() const { return at<2>().valid(); }
    ::protozero::ConstChars package() const { return at<2>().as_string(); }
    bool has_dependency() const { return at<3>().valid(); }
    ::protozero::RepeatedFieldIterator dependency() const { return GetRepeated(3); }
    bool has_public_dependency() const { return at<10>().valid(); }
    ::protozero::RepeatedFieldIterator public_dependency() const { return GetRepeated(10); }
    bool has_weak_dependency() const { return at<11>().valid(); }
    ::protozero::RepeatedFieldIterator weak_dependency() const { return GetRepeated(11); }
    bool has_message_type() const { return at<4>().valid(); }
    ::protozero::RepeatedFieldIterator message_type() const { return GetRepeated(4); }
    bool has_enum_type() const { return at<5>().valid(); }
    ::protozero::RepeatedFieldIterator enum_type() const { return GetRepeated(5); }
  };
  void set_name(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_name(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
  void set_package(const char* value) {
    AppendString(2, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_package(const char* value, size_t size) {
    AppendBytes(2, value, size);
  }
  void add_dependency(const char* value) {
    AppendString(3, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void add_dependency(const char* value, size_t size) {
    AppendBytes(3, value, size);
  }
  void add_public_dependency(int32_t value) {
    AppendVarInt(10, value);
  }
  void add_weak_dependency(int32_t value) {
    AppendVarInt(11, value);
  }
  template <typename T = DescriptorProto> T* add_message_type() {
    return BeginNestedMessage<T>(4);
  }

  template <typename T = EnumDescriptorProto> T* add_enum_type() {
    return BeginNestedMessage<T>(5);
  }

};

class PERFETTO_EXPORT FileDescriptorSet : public ::protozero::Message {
 public:
  enum : int32_t {
    kFileFieldNumber = 1,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/1, /*HAS_REPEATED_FIELDS=*/true> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_file() const { return at<1>().valid(); }
    ::protozero::RepeatedFieldIterator file() const { return GetRepeated(1); }
  };
  template <typename T = FileDescriptorProto> T* add_file() {
    return BeginNestedMessage<T>(1);
  }

};

} // Namespace.
} // Namespace.
} // Namespace.
#endif  // Include guard.
