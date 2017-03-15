#ifdef SR_ENABLE_CHUNK

#include "shape/chunk.h"

VALUE sr_cChunk;

SR_SHAPE_GET(CompSolid, chunk)

bool siren_chunk_install()
{
  SR_SHAPE_INIT(CompSolid)
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
