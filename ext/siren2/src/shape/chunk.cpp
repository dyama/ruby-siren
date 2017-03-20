#ifdef SR_ENABLE_CHUNK

#include "shape/chunk.h"

VALUE sr_cChunk;

SR_SHAPE_GET(CompSolid, chunk)
SR_SHAPE_CHECK(CompSolid, chunk)

bool siren_chunk_install()
{
  SR_SHAPE_DEFINE(CompSolid)
  rb_define_method(sr_cChunk, "initialize", RUBY_METHOD_FUNC(siren_chunk_init),   -1);
  rb_define_method(sr_cChunk, "to_solid",  RUBY_METHOD_FUNC(siren_chunk_to_solid), -1);
  return true;
}

VALUE siren_chunk_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Chunk"));
}

VALUE siren_chunk_init(int argc, VALUE* argv, VALUE self)
{
  VALUE a;
  rb_scan_args(argc, argv, "*", &a);
  TopoDS_CompSolid cs;
  TopoDS_Builder builder;
  builder.MakeCompSolid(cs);
  auto b = rb_funcall(a, rb_intern("flatten"), 0);
  for (int i = 0; i < RARRAY_LEN(b); i++) {
    auto item = RARRAY_AREF(b, i);
    if (siren_solid_p(item)) {
      auto solid = siren_solid_get(item);
      builder.Add(cs, solid);
    }
  }
  auto p = siren_shape_get(self);
  *p = cs;
  return self;
}

VALUE siren_chunk_to_solid(int argc, VALUE* argv, VALUE self)
{
  auto cs = siren_chunk_get(self);
  auto solid = BRepBuilderAPI_MakeSolid(cs);
  return siren_shape_new(solid);
}

#endif
