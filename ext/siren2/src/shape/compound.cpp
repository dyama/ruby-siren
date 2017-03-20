#include "shape/compound.h"

#define rb_array_p(x) RB_TYPE_P(x, T_ARRAY)

VALUE sr_cCompound;

SR_SHAPE_GET(Compound, compound)
SR_SHAPE_CHECK(Compound, compound)

bool siren_compound_install()
{
  SR_SHAPE_DEFINE(Compound)
  rb_define_method(sr_cCompound, "initialize", RUBY_METHOD_FUNC(siren_compound_init),   -1);
  rb_define_method(sr_cCompound, "push",       RUBY_METHOD_FUNC(siren_compound_push),   -1);
  rb_define_method(sr_cCompound, "<<",         RUBY_METHOD_FUNC(siren_compound_push),   -1);
  rb_define_method(sr_cCompound, "delete",     RUBY_METHOD_FUNC(siren_compound_delete), -1);
  return true;
}

VALUE siren_compound_init(int argc, VALUE* argv, VALUE self)
{
  VALUE a;
  rb_scan_args(argc, argv, "*", &a);
  TopoDS_Compound comp;
  BRep_Builder B;
  B.MakeCompound(comp);
  VALUE b = rb_funcall(a, rb_intern("flatten"), 0);
  for (int i = 0; i < RARRAY_LEN(b); i++) {
    TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(b, i));
    B.Add(comp, *shape);
  }
  auto p = siren_shape_get(self);
  *p = comp;
  return self;
}

VALUE siren_compound_push(int argc, VALUE* argv, VALUE self)
{
  VALUE a;
  rb_scan_args(argc, argv, "*", &a);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  VALUE b = rb_funcall(a, rb_intern("flatten"), 0);
  for (int i = 0; i < RARRAY_LEN(b); i++) {
    TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(b, i));
    B.Add(comp, *shape);
  }
  return self;
}

VALUE siren_compound_delete(int argc, VALUE* argv, VALUE self)
{
  VALUE a;
  rb_scan_args(argc, argv, "*", &a);
  TopoDS_Compound comp = siren_compound_get(self);
  BRep_Builder B;
  VALUE b = rb_funcall(a, rb_intern("flatten"), 0);
  for (int i = 0; i < RARRAY_LEN(b); i++) {
    TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(b, i));
    B.Remove(comp, *shape);
  }
  return self;
}
