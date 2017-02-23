/**
 * line.cpp
 * Implementation of singleton methods for LINE
 */

#include "curve.h"

VALUE siren_line_new( const handle<Geom_Curve>* curve)
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Line")));
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_line_type;
  return obj;
}

handle<Geom_Line> siren_line_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_line_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Line> line = handle<Geom_Line>::DownCast(hgc);
  if (line.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Line."); }
  return line;
}

bool siren_line_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_line = rb_define_class_under(sr_mSiren, "Line", cls_curve);
  MRB_SET_INSTANCE_TT(cls_line, MRB_TT_DATA);
  rb_define_method(cls_line, "initialize", siren_curve_init, MRB_ARGS_NONE());

  rb_define_method(cls_line, "dir", siren_line_dir, MRB_ARGS_NONE());
  return true;
}

struct RClass* siren_line_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Line")));
}

VALUE siren_line_dir( VALUE self)
{
  handle<Geom_Line> line = siren_line_get(self);
  return siren_dir_to_ary(line->Lin().Direction());
}

