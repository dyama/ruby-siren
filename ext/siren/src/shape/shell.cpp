#include "shape/shell.h"

VALUE sr_cShell;

TopoDS_Shell siren_shell_get(VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopoDS_Shell res = TopoDS::Shell(*shape);
  if (res.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Shell.");
  }
  return res;
}

bool siren_shell_install()
{
  sr_cShell = rb_define_class_under(sr_mSiren, "Shell", rb_cObject);
  rb_define_alloc_func(sr_cShell, siren_shape_allocate);
  rb_define_method(sr_cShell, "initialize", RUBY_METHOD_FUNC(siren_shape_init),  -1);
  rb_define_singleton_method(sr_cShell, "make", RUBY_METHOD_FUNC(siren_shell_make), -1);
  rb_define_singleton_method(sr_cShell, "sew",  RUBY_METHOD_FUNC(siren_shell_make), -1);
  return true;
}

VALUE siren_shell_make(int argc, VALUE* argv, VALUE self)
{
  VALUE ary;
  VALUE tol;
  rb_scan_args(argc, argv, "A|f", &ary, &tol);
  BRepBuilderAPI_Sewing sewer;
  sewer.Init();
  if (argc == 2 && tol >= 0) {
    sewer.SetTolerance(tol);
  }
  int len = RARRAY_LEN(ary);
  for (int i=0; i < len; i++) {
    VALUE item = RARRAY_AREF(ary, i);
    TopoDS_Shape* shape = siren_shape_get(item);
    if (shape->IsNull()) {
      continue;
    }
    TopExp_Explorer ex(*shape, TopAbs_FACE);
    for (; ex.More(); ex.Next()) {
      sewer.Add(ex.Current());
    }
  }
  sewer.Perform();
  return siren_shape_new(sewer.SewedShape());
}
