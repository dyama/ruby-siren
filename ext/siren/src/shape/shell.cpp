#include "shape/shell.h"

VALUE siren_shell_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* mod_siren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Shell")));
  void* p = rb_malloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_shell_type;
  return obj;
}

TopoDS_Shell siren_shell_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_shell_type));
  TopoDS_Shell shell = TopoDS::Shell(*shape);
  if (shell.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Shell."); }
  return shell;
}

bool siren_shell_install( struct RClass* mod_siren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_shell = rb_define_class_under(mod_siren, "Shell", cls_shape);
  MRB_SET_INSTANCE_TT(cls_shell, MRB_TT_DATA);
  rb_define_method(cls_shell, "initialize", siren_shape_init,  MRB_ARGS_NONE());

  auto obj_shell = rb_obj_ptr(siren_shell_obj());
  rb_define_singleton_method(obj_shell, "make", siren_shell_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_singleton_method(obj_shell, "sew",  siren_shell_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  return true;
}

struct RClass* siren_shell_rclass()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(mod_siren), VALUEern_lit("Shell")));
}

VALUE siren_shell_obj()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Shell"));
}

VALUE siren_shell_make( VALUE self)
{
  VALUE ary;
  VALUE tol;
  int argc = rb_get_args("A|f", &ary, &tol);
  BRepBuilderAPI_Sewing sewer;
  sewer.Init();
  if (argc == 2 && tol >= 0) {
    sewer.SetTolerance(tol);
  }
  int len = rb_ary_len(ary);
  for (int i=0; i < len; i++) {
    VALUE item = rb_ary_ref(ary, i);
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

