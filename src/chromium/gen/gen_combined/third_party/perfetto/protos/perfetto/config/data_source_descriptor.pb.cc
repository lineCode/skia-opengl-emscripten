// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: perfetto/config/data_source_descriptor.proto

#include "perfetto/config/data_source_descriptor.pb.h"

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

namespace perfetto {
namespace protos {
class DataSourceDescriptorDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<DataSourceDescriptor>
      _instance;
} _DataSourceDescriptor_default_instance_;
}  // namespace protos
}  // namespace perfetto
namespace protobuf_perfetto_2fconfig_2fdata_5fsource_5fdescriptor_2eproto {
static void InitDefaultsDataSourceDescriptor() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perfetto::protos::_DataSourceDescriptor_default_instance_;
    new (ptr) ::perfetto::protos::DataSourceDescriptor();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perfetto::protos::DataSourceDescriptor::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_DataSourceDescriptor =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsDataSourceDescriptor}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_DataSourceDescriptor.base);
}

}  // namespace protobuf_perfetto_2fconfig_2fdata_5fsource_5fdescriptor_2eproto
namespace perfetto {
namespace protos {

// ===================================================================

void DataSourceDescriptor::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DataSourceDescriptor::kNameFieldNumber;
const int DataSourceDescriptor::kWillNotifyOnStopFieldNumber;
const int DataSourceDescriptor::kWillNotifyOnStartFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DataSourceDescriptor::DataSourceDescriptor()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_perfetto_2fconfig_2fdata_5fsource_5fdescriptor_2eproto::scc_info_DataSourceDescriptor.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:perfetto.protos.DataSourceDescriptor)
}
DataSourceDescriptor::DataSourceDescriptor(const DataSourceDescriptor& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_name()) {
    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  ::memcpy(&will_notify_on_stop_, &from.will_notify_on_stop_,
    static_cast<size_t>(reinterpret_cast<char*>(&will_notify_on_start_) -
    reinterpret_cast<char*>(&will_notify_on_stop_)) + sizeof(will_notify_on_start_));
  // @@protoc_insertion_point(copy_constructor:perfetto.protos.DataSourceDescriptor)
}

void DataSourceDescriptor::SharedCtor() {
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&will_notify_on_stop_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&will_notify_on_start_) -
      reinterpret_cast<char*>(&will_notify_on_stop_)) + sizeof(will_notify_on_start_));
}

DataSourceDescriptor::~DataSourceDescriptor() {
  // @@protoc_insertion_point(destructor:perfetto.protos.DataSourceDescriptor)
  SharedDtor();
}

void DataSourceDescriptor::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void DataSourceDescriptor::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const DataSourceDescriptor& DataSourceDescriptor::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_perfetto_2fconfig_2fdata_5fsource_5fdescriptor_2eproto::scc_info_DataSourceDescriptor.base);
  return *internal_default_instance();
}


void DataSourceDescriptor::Clear() {
// @@protoc_insertion_point(message_clear_start:perfetto.protos.DataSourceDescriptor)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    name_.ClearNonDefaultToEmptyNoArena();
  }
  ::memset(&will_notify_on_stop_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&will_notify_on_start_) -
      reinterpret_cast<char*>(&will_notify_on_stop_)) + sizeof(will_notify_on_start_));
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool DataSourceDescriptor::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:perfetto.protos.DataSourceDescriptor)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
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

      // optional bool will_notify_on_stop = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          set_has_will_notify_on_stop();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &will_notify_on_stop_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool will_notify_on_start = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {
          set_has_will_notify_on_start();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &will_notify_on_start_)));
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
  // @@protoc_insertion_point(parse_success:perfetto.protos.DataSourceDescriptor)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:perfetto.protos.DataSourceDescriptor)
  return false;
#undef DO_
}

void DataSourceDescriptor::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:perfetto.protos.DataSourceDescriptor)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // optional bool will_notify_on_stop = 2;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->will_notify_on_stop(), output);
  }

  // optional bool will_notify_on_start = 3;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->will_notify_on_start(), output);
  }

  output->WriteRaw(_internal_metadata_.unknown_fields().data(),
                   static_cast<int>(_internal_metadata_.unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:perfetto.protos.DataSourceDescriptor)
}

size_t DataSourceDescriptor::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perfetto.protos.DataSourceDescriptor)
  size_t total_size = 0;

  total_size += _internal_metadata_.unknown_fields().size();

  if (_has_bits_[0 / 32] & 7u) {
    // optional string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // optional bool will_notify_on_stop = 2;
    if (has_will_notify_on_stop()) {
      total_size += 1 + 1;
    }

    // optional bool will_notify_on_start = 3;
    if (has_will_notify_on_start()) {
      total_size += 1 + 1;
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void DataSourceDescriptor::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const DataSourceDescriptor*>(&from));
}

void DataSourceDescriptor::MergeFrom(const DataSourceDescriptor& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perfetto.protos.DataSourceDescriptor)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 7u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_name();
      name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
    }
    if (cached_has_bits & 0x00000002u) {
      will_notify_on_stop_ = from.will_notify_on_stop_;
    }
    if (cached_has_bits & 0x00000004u) {
      will_notify_on_start_ = from.will_notify_on_start_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void DataSourceDescriptor::CopyFrom(const DataSourceDescriptor& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perfetto.protos.DataSourceDescriptor)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DataSourceDescriptor::IsInitialized() const {
  return true;
}

void DataSourceDescriptor::Swap(DataSourceDescriptor* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DataSourceDescriptor::InternalSwap(DataSourceDescriptor* other) {
  using std::swap;
  name_.Swap(&other->name_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(will_notify_on_stop_, other->will_notify_on_stop_);
  swap(will_notify_on_start_, other->will_notify_on_start_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string DataSourceDescriptor::GetTypeName() const {
  return "perfetto.protos.DataSourceDescriptor";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protos
}  // namespace perfetto
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::perfetto::protos::DataSourceDescriptor* Arena::CreateMaybeMessage< ::perfetto::protos::DataSourceDescriptor >(Arena* arena) {
  return Arena::CreateInternal< ::perfetto::protos::DataSourceDescriptor >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
