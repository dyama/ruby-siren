#include "shape/compound.h"

VALUE siren_compound_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Compound")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_compound_type;
  return obj;
}

TopoDS_Compound siren_compound_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_compound_type));
  TopoDS_Compound compound = TopoDS::Compound(*shape);
  if (compound.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Compound."); }
  return compound;
}

bool siren_compound_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_compound = rb_define_class_under(sr_mSiren, "Compound", cls_shape);
  MRB_SET_INSTANCE_TT(cls_compound, MRB_TT_DATA);
  rb_define_method(cls_compound, "initialize", siren_compound_init,   MRB_ARGS_NONE());
  rb_define_method(cls_compound, "push",       siren_compound_push,   MRB_ARGS_NONE());
  rb_define_method(cls_compound, "<<",         siren_compound_push,   MRB_ARGS_NONE());
  rb_define_method(cls_compound, "delete",     siren_compound_delete, MRB_ARGS_NONE());
  return true;
}

struct RClass* siren_compound_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Compound")));
}

VALUE siren_compound_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Compound"));
}

VALUE siren_compound_init( VALUE self)
{
  VALUE* a;
  VALUE len;
  int argc = rb_scan_args("*", &a, &len);

  TopoDS_Compound comp;
  BRep_Builder B;
  B.MakeCompound(comp);

  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (_array_p(arg)) {
      VALUE subary = rb_funcall(arg, "flatten", 0);
      for (int j = 0; j < rb_ary_len(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(rb_ary_ref(subary, j));
        B.Add(comp, *shape);
      }
    }
    else {
      TopoDS_Shape* shape = siren_shape_get(arg);
      B.Add(comp, *shape);
    }
  }

  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = comp; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(self) = &siren_compound_type;
  return self;
}

VALUE siren_compound_push( VALUE self)
{
  VALUE* a;
  VALUE len;
  int argc = rb_scan_args("*", &a, &len);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (_array_p(arg)) {
      VALUE subary = rb_funcall(arg, "flatten", 0);
      for (int j = 0; j < rb_ary_len(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(rb_ary_ref(subary, j));
        B.Add(comp, *shape);
      }
    }
    else {
      TopoDS_Shape* shape = siren_shape_get(arg);
      B.Add(comp, *shape);
    }
  }
  return self;
}

VALUE siren_compound_delete( VALUE self)
{
  VALUE* a;
  VALUE len;
  int argc = rb_scan_args("*", &a, &len);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (_array_p(arg)) {
      VALUE subary = rb_funcall(arg, "flatten", 0);
      for (int j = 0; j < rb_ary_len(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(rb_ary_ref(subary, j));
        B.Remove(comp, *shape);
      }
    }
    else {
      TopoDS_Shape* shape = siren_shape_get(arg);
      B.Remove(comp, *shape);
    }
  }
  return self;
}

