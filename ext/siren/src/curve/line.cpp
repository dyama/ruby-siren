/**
 * line.cpp
 * Implementation of singleton methods for LINE
 */

#include "curve.h"

VALUE siren_line_new( const handle<Geom_Curve>* curve)
{
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  VALUE obj;
  obj = mrb_instance_alloc(mrb, mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Line")));
  void* p = mrb_malloc(mrb, sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_line_type;
  return obj;
}

handle<Geom_Line> siren_line_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(mrb_get_datatype(mrb, self, &siren_line_type));
  if (hgc.IsNull()) { mrb_raise(mrb, E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Line> line = handle<Geom_Line>::DownCast(hgc);
  if (line.IsNull()) { mrb_raise(mrb, E_RUNTIME_ERROR, "The geometry type is not Line."); }
  return line;
}

bool siren_line_install( struct RClass* mod_siren)
{
  struct RClass* cls_curve = siren_curve_rclass(mrb);
  struct RClass* cls_line = mrb_define_class_under(mrb, mod_siren, "Line", cls_curve);
  MRB_SET_INSTANCE_TT(cls_line, MRB_TT_DATA);
  mrb_define_method(mrb, cls_line, "initialize", siren_curve_init, MRB_ARGS_NONE());

  mrb_define_method(mrb, cls_line, "dir", siren_line_dir, MRB_ARGS_NONE());
  return true;
}

struct RClass* siren_line_rclass()
{
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  return mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Line")));
}

VALUE siren_line_dir( VALUE self)
{
  handle<Geom_Line> line = siren_line_get(mrb, self);
  return siren_dir_to_ary(mrb, line->Lin().Direction());
}

