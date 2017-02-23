/**
 * offsetcurve.cpp
 * Implementation of singleton methods for OFFSETCURVE
 */

#include "curve.h"

VALUE siren_offsetcurve_new( const handle<Geom_Curve>* curve)
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("OffsetCurve")));
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_offsetcurve_type;
  return obj;
}

handle<Geom_OffsetCurve> siren_offsetcurve_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_offsetcurve_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_OffsetCurve> offsetcurve = handle<Geom_OffsetCurve>::DownCast(hgc);
  if (offsetcurve.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not OffsetCurve."); }
  return offsetcurve;
}

bool siren_offsetcurve_install()
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_offsetcurve = rb_define_class_under(sr_mSiren, "offsetcurve", cls_curve);
  MRB_SET_INSTANCE_TT(cls_offsetcurve, MRB_TT_DATA);
  rb_define_method(cls_offsetcurve, "initialize", siren_curve_init, MRB_ARGS_NONE());
  return true;
}
