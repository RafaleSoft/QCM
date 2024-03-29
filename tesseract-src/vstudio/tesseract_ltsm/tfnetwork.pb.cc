// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: tfnetwork.proto

#include "tfnetwork.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_tensorflow_2fcore_2fframework_2fgraph_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<3> scc_info_GraphDef_tensorflow_2fcore_2fframework_2fgraph_2eproto;
namespace tesseract {
class TFNetworkModelDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<TFNetworkModel> _instance;
} _TFNetworkModel_default_instance_;
}  // namespace tesseract
static void InitDefaultsscc_info_TFNetworkModel_tfnetwork_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::tesseract::_TFNetworkModel_default_instance_;
    new (ptr) ::tesseract::TFNetworkModel();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::tesseract::TFNetworkModel::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_TFNetworkModel_tfnetwork_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_TFNetworkModel_tfnetwork_2eproto}, {
      &scc_info_GraphDef_tensorflow_2fcore_2fframework_2fgraph_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_tfnetwork_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_tfnetwork_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_tfnetwork_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_tfnetwork_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, graph_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, global_step_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, spec_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, depth_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, x_size_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, y_size_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, batch_size_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, num_classes_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, using_ctc_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, image_input_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, image_widths_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, image_heights_),
  PROTOBUF_FIELD_OFFSET(::tesseract::TFNetworkModel, output_layer_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::tesseract::TFNetworkModel)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::tesseract::_TFNetworkModel_default_instance_),
};

const char descriptor_table_protodef_tfnetwork_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017tfnetwork.proto\022\ttesseract\032%tensorflow"
  "/core/framework/graph.proto\"\233\002\n\016TFNetwor"
  "kModel\022#\n\005graph\030\001 \001(\0132\024.tensorflow.Graph"
  "Def\022\023\n\013global_step\030\002 \001(\003\022\014\n\004spec\030\003 \001(\t\022\r"
  "\n\005depth\030\004 \001(\005\022\016\n\006x_size\030\005 \001(\005\022\016\n\006y_size\030"
  "\006 \001(\005\022\022\n\nbatch_size\030\010 \001(\005\022\023\n\013num_classes"
  "\030\t \001(\005\022\021\n\tusing_ctc\030\n \001(\010\022\023\n\013image_input"
  "\030\013 \001(\t\022\024\n\014image_widths\030\014 \001(\t\022\025\n\rimage_he"
  "ights\030\r \001(\t\022\024\n\014output_layer\030\016 \001(\tb\006proto"
  "3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_tfnetwork_2eproto_deps[1] = {
  &::descriptor_table_tensorflow_2fcore_2fframework_2fgraph_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_tfnetwork_2eproto_sccs[1] = {
  &scc_info_TFNetworkModel_tfnetwork_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_tfnetwork_2eproto_once;
static bool descriptor_table_tfnetwork_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_tfnetwork_2eproto = {
  &descriptor_table_tfnetwork_2eproto_initialized, descriptor_table_protodef_tfnetwork_2eproto, "tfnetwork.proto", 361,
  &descriptor_table_tfnetwork_2eproto_once, descriptor_table_tfnetwork_2eproto_sccs, descriptor_table_tfnetwork_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_tfnetwork_2eproto::offsets,
  file_level_metadata_tfnetwork_2eproto, 1, file_level_enum_descriptors_tfnetwork_2eproto, file_level_service_descriptors_tfnetwork_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_tfnetwork_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_tfnetwork_2eproto), true);
namespace tesseract {

// ===================================================================

void TFNetworkModel::InitAsDefaultInstance() {
  ::tesseract::_TFNetworkModel_default_instance_._instance.get_mutable()->graph_ = const_cast< ::tensorflow::GraphDef*>(
      ::tensorflow::GraphDef::internal_default_instance());
}
class TFNetworkModel::_Internal {
 public:
  static const ::tensorflow::GraphDef& graph(const TFNetworkModel* msg);
};

const ::tensorflow::GraphDef&
TFNetworkModel::_Internal::graph(const TFNetworkModel* msg) {
  return *msg->graph_;
}
void TFNetworkModel::clear_graph() {
  if (GetArenaNoVirtual() == nullptr && graph_ != nullptr) {
    delete graph_;
  }
  graph_ = nullptr;
}
TFNetworkModel::TFNetworkModel()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:tesseract.TFNetworkModel)
}
TFNetworkModel::TFNetworkModel(const TFNetworkModel& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  spec_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_spec().empty()) {
    spec_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.spec_);
  }
  image_input_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_image_input().empty()) {
    image_input_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_input_);
  }
  image_widths_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_image_widths().empty()) {
    image_widths_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_widths_);
  }
  image_heights_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_image_heights().empty()) {
    image_heights_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_heights_);
  }
  output_layer_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_output_layer().empty()) {
    output_layer_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.output_layer_);
  }
  if (from._internal_has_graph()) {
    graph_ = new ::tensorflow::GraphDef(*from.graph_);
  } else {
    graph_ = nullptr;
  }
  ::memcpy(&global_step_, &from.global_step_,
    static_cast<size_t>(reinterpret_cast<char*>(&using_ctc_) -
    reinterpret_cast<char*>(&global_step_)) + sizeof(using_ctc_));
  // @@protoc_insertion_point(copy_constructor:tesseract.TFNetworkModel)
}

void TFNetworkModel::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_TFNetworkModel_tfnetwork_2eproto.base);
  spec_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_input_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_widths_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_heights_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  output_layer_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&graph_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&using_ctc_) -
      reinterpret_cast<char*>(&graph_)) + sizeof(using_ctc_));
}

TFNetworkModel::~TFNetworkModel() {
  // @@protoc_insertion_point(destructor:tesseract.TFNetworkModel)
  SharedDtor();
}

void TFNetworkModel::SharedDtor() {
  spec_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_input_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_widths_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_heights_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  output_layer_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete graph_;
}

void TFNetworkModel::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const TFNetworkModel& TFNetworkModel::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_TFNetworkModel_tfnetwork_2eproto.base);
  return *internal_default_instance();
}


void TFNetworkModel::Clear() {
// @@protoc_insertion_point(message_clear_start:tesseract.TFNetworkModel)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  spec_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_input_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_widths_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  image_heights_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  output_layer_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (GetArenaNoVirtual() == nullptr && graph_ != nullptr) {
    delete graph_;
  }
  graph_ = nullptr;
  ::memset(&global_step_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&using_ctc_) -
      reinterpret_cast<char*>(&global_step_)) + sizeof(using_ctc_));
  _internal_metadata_.Clear();
}

const char* TFNetworkModel::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .tensorflow.GraphDef graph = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_graph(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 global_step = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          global_step_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string spec = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_spec(), ptr, ctx, "tesseract.TFNetworkModel.spec");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 depth = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          depth_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 x_size = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          x_size_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 y_size = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          y_size_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 batch_size = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 64)) {
          batch_size_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 num_classes = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 72)) {
          num_classes_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool using_ctc = 10;
      case 10:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 80)) {
          using_ctc_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string image_input = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 90)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_image_input(), ptr, ctx, "tesseract.TFNetworkModel.image_input");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string image_widths = 12;
      case 12:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 98)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_image_widths(), ptr, ctx, "tesseract.TFNetworkModel.image_widths");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string image_heights = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 106)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_image_heights(), ptr, ctx, "tesseract.TFNetworkModel.image_heights");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string output_layer = 14;
      case 14:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 114)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_output_layer(), ptr, ctx, "tesseract.TFNetworkModel.output_layer");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* TFNetworkModel::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:tesseract.TFNetworkModel)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .tensorflow.GraphDef graph = 1;
  if (this->has_graph()) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(
        1, _Internal::graph(this), target, stream);
  }

  // int64 global_step = 2;
  if (this->global_step() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(2, this->_internal_global_step(), target);
  }

  // string spec = 3;
  if (this->spec().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_spec().data(), static_cast<int>(this->_internal_spec().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "tesseract.TFNetworkModel.spec");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_spec(), target);
  }

  // int32 depth = 4;
  if (this->depth() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_depth(), target);
  }

  // int32 x_size = 5;
  if (this->x_size() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(5, this->_internal_x_size(), target);
  }

  // int32 y_size = 6;
  if (this->y_size() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(6, this->_internal_y_size(), target);
  }

  // int32 batch_size = 8;
  if (this->batch_size() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(8, this->_internal_batch_size(), target);
  }

  // int32 num_classes = 9;
  if (this->num_classes() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(9, this->_internal_num_classes(), target);
  }

  // bool using_ctc = 10;
  if (this->using_ctc() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(10, this->_internal_using_ctc(), target);
  }

  // string image_input = 11;
  if (this->image_input().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_image_input().data(), static_cast<int>(this->_internal_image_input().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "tesseract.TFNetworkModel.image_input");
    target = stream->WriteStringMaybeAliased(
        11, this->_internal_image_input(), target);
  }

  // string image_widths = 12;
  if (this->image_widths().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_image_widths().data(), static_cast<int>(this->_internal_image_widths().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "tesseract.TFNetworkModel.image_widths");
    target = stream->WriteStringMaybeAliased(
        12, this->_internal_image_widths(), target);
  }

  // string image_heights = 13;
  if (this->image_heights().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_image_heights().data(), static_cast<int>(this->_internal_image_heights().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "tesseract.TFNetworkModel.image_heights");
    target = stream->WriteStringMaybeAliased(
        13, this->_internal_image_heights(), target);
  }

  // string output_layer = 14;
  if (this->output_layer().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_output_layer().data(), static_cast<int>(this->_internal_output_layer().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "tesseract.TFNetworkModel.output_layer");
    target = stream->WriteStringMaybeAliased(
        14, this->_internal_output_layer(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:tesseract.TFNetworkModel)
  return target;
}

size_t TFNetworkModel::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:tesseract.TFNetworkModel)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string spec = 3;
  if (this->spec().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_spec());
  }

  // string image_input = 11;
  if (this->image_input().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_image_input());
  }

  // string image_widths = 12;
  if (this->image_widths().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_image_widths());
  }

  // string image_heights = 13;
  if (this->image_heights().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_image_heights());
  }

  // string output_layer = 14;
  if (this->output_layer().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_output_layer());
  }

  // .tensorflow.GraphDef graph = 1;
  if (this->has_graph()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *graph_);
  }

  // int64 global_step = 2;
  if (this->global_step() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_global_step());
  }

  // int32 depth = 4;
  if (this->depth() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_depth());
  }

  // int32 x_size = 5;
  if (this->x_size() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_x_size());
  }

  // int32 y_size = 6;
  if (this->y_size() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_y_size());
  }

  // int32 batch_size = 8;
  if (this->batch_size() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_batch_size());
  }

  // int32 num_classes = 9;
  if (this->num_classes() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_num_classes());
  }

  // bool using_ctc = 10;
  if (this->using_ctc() != 0) {
    total_size += 1 + 1;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void TFNetworkModel::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:tesseract.TFNetworkModel)
  GOOGLE_DCHECK_NE(&from, this);
  const TFNetworkModel* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<TFNetworkModel>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:tesseract.TFNetworkModel)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:tesseract.TFNetworkModel)
    MergeFrom(*source);
  }
}

void TFNetworkModel::MergeFrom(const TFNetworkModel& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:tesseract.TFNetworkModel)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.spec().size() > 0) {

    spec_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.spec_);
  }
  if (from.image_input().size() > 0) {

    image_input_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_input_);
  }
  if (from.image_widths().size() > 0) {

    image_widths_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_widths_);
  }
  if (from.image_heights().size() > 0) {

    image_heights_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.image_heights_);
  }
  if (from.output_layer().size() > 0) {

    output_layer_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.output_layer_);
  }
  if (from.has_graph()) {
    _internal_mutable_graph()->::tensorflow::GraphDef::MergeFrom(from._internal_graph());
  }
  if (from.global_step() != 0) {
    _internal_set_global_step(from._internal_global_step());
  }
  if (from.depth() != 0) {
    _internal_set_depth(from._internal_depth());
  }
  if (from.x_size() != 0) {
    _internal_set_x_size(from._internal_x_size());
  }
  if (from.y_size() != 0) {
    _internal_set_y_size(from._internal_y_size());
  }
  if (from.batch_size() != 0) {
    _internal_set_batch_size(from._internal_batch_size());
  }
  if (from.num_classes() != 0) {
    _internal_set_num_classes(from._internal_num_classes());
  }
  if (from.using_ctc() != 0) {
    _internal_set_using_ctc(from._internal_using_ctc());
  }
}

void TFNetworkModel::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:tesseract.TFNetworkModel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TFNetworkModel::CopyFrom(const TFNetworkModel& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:tesseract.TFNetworkModel)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TFNetworkModel::IsInitialized() const {
  return true;
}

void TFNetworkModel::InternalSwap(TFNetworkModel* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  spec_.Swap(&other->spec_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  image_input_.Swap(&other->image_input_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  image_widths_.Swap(&other->image_widths_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  image_heights_.Swap(&other->image_heights_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  output_layer_.Swap(&other->output_layer_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(graph_, other->graph_);
  swap(global_step_, other->global_step_);
  swap(depth_, other->depth_);
  swap(x_size_, other->x_size_);
  swap(y_size_, other->y_size_);
  swap(batch_size_, other->batch_size_);
  swap(num_classes_, other->num_classes_);
  swap(using_ctc_, other->using_ctc_);
}

::PROTOBUF_NAMESPACE_ID::Metadata TFNetworkModel::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace tesseract
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::tesseract::TFNetworkModel* Arena::CreateMaybeMessage< ::tesseract::TFNetworkModel >(Arena* arena) {
  return Arena::CreateInternal< ::tesseract::TFNetworkModel >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
