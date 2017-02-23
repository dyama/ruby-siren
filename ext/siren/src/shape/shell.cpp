#include "shape/shell.h"

VALUE siren_shell_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass(mrb);
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  obj = mrb_instance_alloc(mrb, mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Shell")));
  void* p = mrb_malloc(mrb, sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_shell_type;
  return obj;
}

TopoDS_Shell siren_shell_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(mrb_get_datatype(mrb, self, &siren_shell_type));
  TopoDS_Shell shell = TopoDS::Shell(*shape);
  if (shell.IsNull()) { mrb_raise(mrb, E_RUNTIME_ERROR, "The geometry type is not Shell."); }
  return shell;
}

bool siren_shell_install( struct RClass* mod_siren)
{
  struct RClass* cls_shape = siren_shape_rclass(mrb);
  struct RClass* cls_shell = mrb_define_class_under(mrb, mod_siren, "Shell", cls_shape);
  MRB_SET_INSTANCE_TT(cls_shell, MRB_TT_DATA);
  mrb_define_method(mrb, cls_shell, "initialize", siren_shape_init,  MRB_ARGS_NONE());

  auto obj_shell = mrb_obj_ptr(siren_shell_obj(mrb));
  mrb_define_singleton_method(mrb, obj_shell, "make", siren_shell_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  mrb_define_singleton_method(mrb, obj_shell, "sew",  siren_shell_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  return true;
}

struct RClass* siren_shell_rclass()
{
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  return mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Shell")));
}

VALUE siren_shell_obj()
{
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  return mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Shell"));
}

VALUE siren_shell_make( VALUE self)
{
  VALUE ary;
  mrb_float tol;
  int argc = mrb_get_args(mrb, "A|f", &ary, &tol);
  BRepBuilderAPI_Sewing sewer;
  sewer.Init();
  if (argc == 2 && tol >= 0) {
    sewer.SetTolerance(tol);
  }
  int len = mrb_ary_len(mrb, ary);
  for (int i=0; i < len; i++) {
    VALUE item = mrb_ary_ref(mrb, ary, i);
    TopoDS_Shape* shape = siren_shape_get(mrb, item);
    if (shape->IsNull()) {
      continue;
    }
    TopExp_Explorer ex(*shape, TopAbs_FACE);
    for (; ex.More(); ex.Next()) {
      sewer.Add(ex.Current());
    }
  }
  sewer.Perform();
  return siren_shape_new(mrb, sewer.SewedShape());
}

