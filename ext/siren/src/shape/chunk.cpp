#ifdef SIREN_ENABLE_CHUNK

#include "shape/chunk.h"

VALUE sr_cChunk;

VALUE siren_chunk_new( const TopoDS_Shape* src)
{
  VALUE obj = rb_instance_alloc(sr_cChunk);
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(obj) = &siren_chunk_type;
  return obj;
}

TopoDS_CompSolid siren_chunk_get(VALUE self)
{
#if 0
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_chunk_type));
  TopoDS_CompSolid chunk = TopoDS::CompSolid(*shape);
  if (chunk.IsNull()) { rb_raise(Qnil, "The geometry type is not Chunk."); }
  return chunk;
#endif
}

bool siren_chunk_install()
{
#if 0
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_chunk = rb_define_class_under(sr_mSiren, "Chunk", cls_shape);
  MRB_SET_INSTANCE_TT(cls_chunk, MRB_TT_DATA);
#endif
  rb_define_method(sr_cChunk, "initialize", RUBY_METHOD_FUNC(siren_chunk_init),   -1);
  rb_define_method(sr_cChunk, "to_solid",  RUBY_METHOD_FUNC(siren_chunk_to_solid), -1);
  return true;
}

struct RClass* siren_chunk_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Chunk")));
}

VALUE siren_chunk_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Chunk"));
}

VALUE siren_chunk_init(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);

  TopoDS_CompSolid cs;
  TopoDS_Builder builder;
  builder.MakeCompSolid(cs);

  for (int i = 0; i < len; i++) {
    if (_array_p(a[i])) {
      for (int j = 0; j < RARRAY_LEN(a[i]); j++) {
        auto solid = siren_solid_get(RARRAY_AREF(a[i], j));
        builder.Add(cs, solid);
      }
    }
    else {
      auto solid = siren_solid_get(a[i]);
      builder.Add(cs, solid);
    }
  }

  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = cs; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(self) = &siren_chunk_type;
  return self;
}

VALUE siren_chunk_to_solid(int argc, VALUE* argv, VALUE self)
{
  auto cs = siren_chunk_get(self);
  auto solid = BRepBuilderAPI_MakeSolid(cs);
  return siren_shape_new(solid);
}

#endif