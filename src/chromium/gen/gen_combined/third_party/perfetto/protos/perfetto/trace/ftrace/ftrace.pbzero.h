// Autogenerated by the ProtoZero compiler plugin. DO NOT EDIT.

#ifndef PERFETTO_PROTOS_PERFETTO_TRACE_FTRACE_FTRACE_PROTO_H_
#define PERFETTO_PROTOS_PERFETTO_TRACE_FTRACE_FTRACE_PROTO_H_

#include <stddef.h>
#include <stdint.h>

#include "perfetto/base/export.h"
#include "perfetto/protozero/proto_decoder.h"
#include "perfetto/protozero/message.h"

namespace perfetto {
namespace protos {
namespace pbzero {


class PERFETTO_EXPORT PrintFtraceEvent : public ::protozero::Message {
 public:
  enum : int32_t {
    kIpFieldNumber = 1,
    kBufFieldNumber = 2,
  };
  class Decoder : public ::protozero::TypedProtoDecoder</*MAX_FIELD_ID=*/2, /*HAS_REPEATED_FIELDS=*/false> {
   public:
    Decoder(const uint8_t* data, size_t len) : TypedProtoDecoder(data, len) {}
    bool has_ip() const { return at<1>().valid(); }
    uint64_t ip() const { return at<1>().as_uint64(); }
    bool has_buf() const { return at<2>().valid(); }
    ::protozero::ConstChars buf() const { return at<2>().as_string(); }
  };
  void set_ip(uint64_t value) {
    AppendVarInt(1, value);
  }
  void set_buf(const char* value) {
    AppendString(2, value);
  }
  // Doesn't check for null terminator.
  // Expects |value| to be at least |size| long.
  void set_buf(const char* value, size_t size) {
    AppendBytes(2, value, size);
  }
};

} // Namespace.
} // Namespace.
} // Namespace.
#endif  // Include guard.
