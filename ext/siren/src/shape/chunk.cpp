#ifdef SIREN_ENABLE_CHUNK

#include "shape/chunk.h"

VALUE siren_chunk_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* mod_siren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Chunk")));
  void* p = rb_malloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_chunk_type;
  return obj;
}

TopoDS_CompSolid siren_chunk_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_chunk_type));
  TopoDS_CompSolid chunk = TopoDS::CompSolid(*shape);
  if (chunk.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Chunk."); }
  return chunk;
}

bool siren_chunk_install( struct RClass* mod_siren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_chunk = rb_define_class_under(mod_siren, "Chunk", cls_shape);
  MRB_SET_INSTANCE_TT(cls_chunk, MRB_TT_DATA);
  rb_define_method(cls_chunk, "initialize", siren_chunk_init,   MRB_ARGS_NONE());
  rb_define_method(cls_chunk, "to_solid",  siren_chunk_to_solid, MRB_ARGS_NONE());
  return true;
}

struct RClass* siren_chunk_rclass()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(mod_siren), VALUEern_lit("Chunk")));
}

VALUE siren_chunk_obj()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Chunk"));
}

VALUE siren_chunk_init( VALUE self)
{
  VALUE* a;
  VALUE len;
  int argc = rb_get_args("*", &a, &len);

  TopoDS_CompSolid cs;
  TopoDS_Builder builder;
  builder.MakeCompSolid(cs);

  for (int i = 0; i < len; i++) {
    if (_array_p(a[i])) {
      for (int j = 0; j < rb_ary_len(a[i]); j++) {
        auto solid = siren_solid_get(rb_ary_ref(a[i], j));
        builder.Add(cs, solid);
      }
    }
    else {
      auto solid = siren_solid_get(a[i]);
      builder.Add(cs, solid);
    }
  }

  void* p = rb_malloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = cs; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(self) = &siren_chunk_type;
  return self;
}

VALUE siren_chunk_to_solid( VALUE self)
{
  auto cs = siren_chunk_get(self);
  auto solid = BRepBuilderAPI_MakeSolid(cs);
  return siren_shape_new(solid);
}

#endif
