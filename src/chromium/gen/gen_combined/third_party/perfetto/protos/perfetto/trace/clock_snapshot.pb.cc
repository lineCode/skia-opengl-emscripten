// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/trace/clock_snapshot.proto

#include "perfetto/trace/clock_snapshot.pb.h"

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

namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ClockSnapshot_Clock;
}  // namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto
namespace perfetto {
namespace protos {
class ClockSnapshot_ClockDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ClockSnapshot_Clock>
      _instance;
} _ClockSnapshot_Clock_default_instance_;
class ClockSnapshotDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ClockSnapshot>
      _instance;
} _ClockSnapshot_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto {
static void InitDefaultsClockSnapshot_Clock() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perfetto::protos::_ClockSnapshot_Clock_default_instance_;
    new (ptr) ::perfetto::protos::ClockSnapshot_Clock();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perfetto::protos::ClockSnapshot_Clock::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_ClockSnapshot_Clock =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsClockSnapshot_Clock}, {}};

static void InitDefaultsClockSnapshot() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perfetto::protos::_ClockSnapshot_default_instance_;
    new (ptr) ::perfetto::protos::ClockSnapshot();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perfetto::protos::ClockSnapshot::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_ClockSnapshot =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsClockSnapshot}, {
      &protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot_Clock.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_ClockSnapshot_Clock.base);
  ::google::protobuf::internal::InitSCC(&scc_info_ClockSnapshot.base);
}

}  // namespace protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto
namespace perfetto {
namespace protos {
bool ClockSnapshot_Clock_Type_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::UNKNOWN;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::REALTIME;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::REALTIME_COARSE;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::MONOTONIC;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::MONOTONIC_COARSE;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::MONOTONIC_RAW;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::BOOTTIME;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::PROCESS_CPUTIME;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::THREAD_CPUTIME;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::Type_MIN;
const ClockSnapshot_Clock_Type ClockSnapshot_Clock::Type_MAX;
const int ClockSnapshot_Clock::Type_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void ClockSnapshot_Clock::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ClockSnapshot_Clock::kTypeFieldNumber;
const int ClockSnapshot_Clock::kTimestampFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ClockSnapshot_Clock::ClockSnapshot_Clock()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot_Clock.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:perfetto.protos.ClockSnapshot.Clock)
}
ClockSnapshot_Clock::ClockSnapshot_Clock(const ClockSnapshot_Clock& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&timestamp_, &from.timestamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&type_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(type_));
  // @@protoc_insertion_point(copy_constructor:perfetto.protos.ClockSnapshot.Clock)
}

void ClockSnapshot_Clock::SharedCtor() {
  ::memset(&timestamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&type_) -
      reinterpret_cast<char*>(&timestamp_)) + sizeof(type_));
}

ClockSnapshot_Clock::~ClockSnapshot_Clock() {
  // @@protoc_insertion_point(destructor:perfetto.protos.ClockSnapshot.Clock)
  SharedDtor();
}

void ClockSnapshot_Clock::SharedDtor() {
}

void ClockSnapshot_Clock::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ClockSnapshot_Clock& ClockSnapshot_Clock::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot_Clock.base);
  return *internal_default_instance();
}


void ClockSnapshot_Clock::Clear() {
// @@protoc_insertion_point(message_clear_start:perfetto.protos.ClockSnapshot.Clock)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 3u) {
    ::memset(&timestamp_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&type_) -
        reinterpret_cast<char*>(&timestamp_)) + sizeof(type_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool ClockSnapshot_Clock::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:perfetto.protos.ClockSnapshot.Clock)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .perfetto.protos.ClockSnapshot.Clock.Type type = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::perfetto::protos::ClockSnapshot_Clock_Type_IsValid(value)) {
            set_type(static_cast< ::perfetto::protos::ClockSnapshot_Clock_Type >(value));
          } else {
            unknown_fields_stream.WriteVarint32(8u);
            unknown_fields_stream.WriteVarint32(
                static_cast< ::google::protobuf::uint32>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional uint64 timestamp = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          set_has_timestamp();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &timestamp_)));
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
  // @@protoc_insertion_point(parse_success:perfetto.protos.ClockSnapshot.Clock)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:perfetto.protos.ClockSnapshot.Clock)
  return false;
#undef DO_
}

void ClockSnapshot_Clock::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:perfetto.protos.ClockSnapshot.Clock)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .perfetto.protos.ClockSnapshot.Clock.Type type = 1;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional uint64 timestamp = 2;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->timestamp(), output);
  }

  output->WriteRaw(_internal_metadata_.unknown_fields().data(),
                   static_cast<int>(_internal_metadata_.unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:perfetto.protos.ClockSnapshot.Clock)
}

size_t ClockSnapshot_Clock::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perfetto.protos.ClockSnapshot.Clock)
  size_t total_size = 0;

  total_size += _internal_metadata_.unknown_fields().size();

  if (_has_bits_[0 / 32] & 3u) {
    // optional uint64 timestamp = 2;
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->timestamp());
    }

    // optional .perfetto.protos.ClockSnapshot.Clock.Type type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ClockSnapshot_Clock::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ClockSnapshot_Clock*>(&from));
}

void ClockSnapshot_Clock::MergeFrom(const ClockSnapshot_Clock& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perfetto.protos.ClockSnapshot.Clock)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 3u) {
    if (cached_has_bits & 0x00000001u) {
      timestamp_ = from.timestamp_;
    }
    if (cached_has_bits & 0x00000002u) {
      type_ = from.type_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void ClockSnapshot_Clock::CopyFrom(const ClockSnapshot_Clock& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perfetto.protos.ClockSnapshot.Clock)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ClockSnapshot_Clock::IsInitialized() const {
  return true;
}

void ClockSnapshot_Clock::Swap(ClockSnapshot_Clock* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ClockSnapshot_Clock::InternalSwap(ClockSnapshot_Clock* other) {
  using std::swap;
  swap(timestamp_, other->timestamp_);
  swap(type_, other->type_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ClockSnapshot_Clock::GetTypeName() const {
  return "perfetto.protos.ClockSnapshot.Clock";
}


// ===================================================================

void ClockSnapshot::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ClockSnapshot::kClocksFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ClockSnapshot::ClockSnapshot()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:perfetto.protos.ClockSnapshot)
}
ClockSnapshot::ClockSnapshot(const ClockSnapshot& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      clocks_(from.clocks_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:perfetto.protos.ClockSnapshot)
}

void ClockSnapshot::SharedCtor() {
}

ClockSnapshot::~ClockSnapshot() {
  // @@protoc_insertion_point(destructor:perfetto.protos.ClockSnapshot)
  SharedDtor();
}

void ClockSnapshot::SharedDtor() {
}

void ClockSnapshot::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ClockSnapshot& ClockSnapshot::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_perfetto_2ftrace_2fclock_5fsnapshot_2eproto::scc_info_ClockSnapshot.base);
  return *internal_default_instance();
}


void ClockSnapshot::Clear() {
// @@protoc_insertion_point(message_clear_start:perfetto.protos.ClockSnapshot)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  clocks_.Clear();
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool ClockSnapshot::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:perfetto.protos.ClockSnapshot)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .perfetto.protos.ClockSnapshot.Clock clocks = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
                input, add_clocks()));
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
  // @@protoc_insertion_point(parse_success:perfetto.protos.ClockSnapshot)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:perfetto.protos.ClockSnapshot)
  return false;
#undef DO_
}

void ClockSnapshot::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:perfetto.protos.ClockSnapshot)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .perfetto.protos.ClockSnapshot.Clock clocks = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->clocks_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1,
      this->clocks(static_cast<int>(i)),
      output);
  }

  output->WriteRaw(_internal_metadata_.unknown_fields().data(),
                   static_cast<int>(_internal_metadata_.unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:perfetto.protos.ClockSnapshot)
}

size_t ClockSnapshot::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perfetto.protos.ClockSnapshot)
  size_t total_size = 0;

  total_size += _internal_metadata_.unknown_fields().size();

  // repeated .perfetto.protos.ClockSnapshot.Clock clocks = 1;
  {
    unsigned int count = static_cast<unsigned int>(this->clocks_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->clocks(static_cast<int>(i)));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ClockSnapshot::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ClockSnapshot*>(&from));
}

void ClockSnapshot::MergeFrom(const ClockSnapshot& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perfetto.protos.ClockSnapshot)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  clocks_.MergeFrom(from.clocks_);
}

void ClockSnapshot::CopyFrom(const ClockSnapshot& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perfetto.protos.ClockSnapshot)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ClockSnapshot::IsInitialized() const {
  return true;
}

void ClockSnapshot::Swap(ClockSnapshot* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ClockSnapshot::InternalSwap(ClockSnapshot* other) {
  using std::swap;
  CastToBase(&clocks_)->InternalSwap(CastToBase(&other->clocks_));
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ClockSnapshot::GetTypeName() const {
  return "perfetto.protos.ClockSnapshot";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::perfetto::protos::ClockSnapshot_Clock* Arena::CreateMaybeMessage< ::perfetto::protos::ClockSnapshot_Clock >(Arena* arena) {
  return Arena::CreateInternal< ::perfetto::protos::ClockSnapshot_Clock >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::perfetto::protos::ClockSnapshot* Arena::CreateMaybeMessage< ::perfetto::protos::ClockSnapshot >(Arena* arena) {
  return Arena::CreateInternal< ::perfetto::protos::ClockSnapshot >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
