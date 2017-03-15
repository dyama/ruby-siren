#include "shape/compound.h"

#define rb_array_p(x) RB_TYPE_P(x, T_ARRAY)

VALUE sr_cCompound;

TopoDS_Compound siren_compound_get(VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopoDS_Compound res = TopoDS::Compound(*shape);
  if (res.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Compound.");
  }
  return res;
}

bool siren_compound_install()
{
  sr_cCompound = rb_define_class_under(sr_mSiren, "Compound", rb_cObject);
  rb_define_alloc_func(sr_cCompound, siren_shape_allocate);
  rb_define_method(sr_cCompound, "initialize", RUBY_METHOD_FUNC(siren_compound_init),   -1);
  rb_define_method(sr_cCompound, "push",       RUBY_METHOD_FUNC(siren_compound_push),   -1);
  rb_define_method(sr_cCompound, "<<",         RUBY_METHOD_FUNC(siren_compound_push),   -1);
  rb_define_method(sr_cCompound, "delete",     RUBY_METHOD_FUNC(siren_compound_delete), -1);
  return true;
}

VALUE siren_compound_init(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);

  TopoDS_Compound comp;
  BRep_Builder B;
  B.MakeCompound(comp);

  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (rb_array_p(arg)) {
      VALUE subary = rb_funcall(arg, rb_intern("flatten"), 0);
      for (int j = 0; j < RARRAY_LEN(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(subary, j));
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
//  DATA_TYPE(self) = &siren_compound_type;
  return self;
}

VALUE siren_compound_push(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (rb_array_p(arg)) {
      VALUE subary = rb_funcall(arg, rb_intern("flatten"), 0);
      for (int j = 0; j < RARRAY_LEN(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(subary, j));
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

VALUE siren_compound_delete(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  for (int i = 0; i < len; i++) {
    VALUE arg = *(a + i);
    if (rb_array_p(arg)) {
      VALUE subary = rb_funcall(arg, rb_intern("flatten"), 0);
      for (int j = 0; j < RARRAY_LEN(subary); j++) {
        TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(subary, j));
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
