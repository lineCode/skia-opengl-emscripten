// Autogenerated by the ProtoZero compiler plugin. DO NOT EDIT.

#ifndef PERFETTO_PROTOS_PERFETTO_TRACE_FTRACE_IPI_PROTO_H_
#define PERFETTO_PROTOS_PERFETTO_TRACE_FTRACE_IPI_PROTO_H_

#include <stddef.h>
#include <stdint.h>

#include "perfetto/base/export.h"
#include "perfetto/protozero/proto_decoder.h"
#include "perfetto/protozero/message.h"

namespace perfetto {
namespace protos {
namespace pbzero {


class PERFETTO_EXPORT IpiRaiseFtraceEvent : public ::protozero::Message {
 public:
  enum : int32_t {
    kTargetCpusFieldNumber = 1,
    kReasonFieldNumber = 2,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/2, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_target_cpus() const { return at<1>().valid(); }
    uint32_t target_cpus() const { return at<1>().as_uint32(); }
    bool has_reason() const { return at<2>().valid(); }
    ::protozero::ConstChars reason() const { return at<2>().as_string(); }
  };
  void set_target_cpus(uint32_t value) {
    AppendVarInt(1, value);
  }
  void set_reason(const char* value) {
    AppendString(2, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_reason(const char* value, size_t size) {
    AppendBytes(2, value, size);
  }
};

class PERFETTO_EXPORT IpiExitFtraceEvent : public ::protozero::Message {
 public:
  enum : int32_t {
    kReasonFieldNumber = 1,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/1, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_reason() const { return at<1>().valid(); }
    ::protozero::ConstChars reason() const { return at<1>().as_string(); }
  };
  void set_reason(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_reason(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
};

class PERFETTO_EXPORT IpiEntryFtraceEvent : public ::protozero::Message {
 public:
  enum : int32_t {
    kReasonFieldNumber = 1,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/1, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_reason() const { return at<1>().valid(); }
    ::protozero::ConstChars reason() const { return at<1>().as_string(); }
  };
  void set_reason(const char* value) {
    AppendString(1, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_reason(const char* value, size_t size) {
    AppendBytes(1, value, size);
  }
};

} // Namespace.
} // Namespace.
} // Namespace.
#endif  // Include guard.
