/**
 * offsetcurve.cpp
 * Implementation of singleton methods for OFFSETCURVE
 */

#include "curve.h"

VALUE siren_offsetcurve_new( const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cOffsetCurve);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_offsetcurve_type;
#endif
  return obj;
}

handle<Geom_OffsetCurve> siren_offsetcurve_get(VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_offsetcurve_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_OffsetCurve> offsetcurve = handle<Geom_OffsetCurve>::DownCast(hgc);
  if (offsetcurve.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not OffsetCurve."); }
  return offsetcurve;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_OffsetCurve> oc = handle<Geom_OffsetCurve>::DownCast(hgc);
  if (oc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Offset Curve.");
  }
  return oc;
#endif
}

bool siren_offsetcurve_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_offsetcurve = rb_define_class_under(sr_mSiren, "offsetcurve", cls_curve);
  MRB_SET_INSTANCE_TT(cls_offsetcurve, MRB_TT_DATA);
#endif
  rb_define_method(sr_cOffsetCurve, "initialize", siren_curve_init, -1);
  return true;
}
