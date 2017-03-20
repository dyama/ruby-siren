#include "shape/shell.h"

VALUE sr_cShell;

SR_SHAPE_GET(Shell, shell)
SR_SHAPE_CHECK(Shell, shell)

bool siren_shell_install()
{
  SR_SHAPE_DEFINE(Shell)
  rb_define_method(sr_cShell, "initialize", RUBY_METHOD_FUNC(siren_shape_init),  -1);
  rb_define_singleton_method(sr_cShell, "make", RUBY_METHOD_FUNC(siren_shell_make), -1);
  rb_define_singleton_method(sr_cShell, "sew",  RUBY_METHOD_FUNC(siren_shell_make), -1);
  return true;
}

VALUE siren_shell_make(int argc, VALUE* argv, VALUE self)
{
  VALUE ary;
  VALUE tol;
  rb_scan_args(argc, argv, "11", &ary, &tol);
  BRepBuilderAPI_Sewing sewer;
  sewer.Init();
  if (argc == 2 && NUM2DBL(tol) >= 0.0) {
    sewer.SetTolerance(NUM2DBL(tol));
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
