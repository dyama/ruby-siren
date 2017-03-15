#include "shape/shell.h"

VALUE sr_cShell;

VALUE siren_shell_new( const TopoDS_Shape* src)
{
  VALUE obj = rb_instance_alloc(sr_cShell);
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(obj) = &siren_shell_type;
  return obj;
}

TopoDS_Shell siren_shell_get(VALUE self)
{
#if 0
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_shell_type));
  TopoDS_Shell shell = TopoDS::Shell(*shape);
  if (shell.IsNull()) { rb_raise(Qnil, "The geometry type is not Shell."); }
  return shell;
#endif
}

bool siren_shell_install()
{
#if 0
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_shell = rb_define_class_under(sr_mSiren, "Shell", cls_shape);
  MRB_SET_INSTANCE_TT(cls_shell, MRB_TT_DATA);
#endif
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