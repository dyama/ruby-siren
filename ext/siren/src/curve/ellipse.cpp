/**
 * ellipse.cpp
 * Implementation of singleton methods for ELLIPSE
 */

#include "curve.h"

VALUE siren_ellipse_new( const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cEllipse);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_ellipse_type;
#endif
  return obj;
}

handle<Geom_Ellipse> siren_ellipse_get( VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_ellipse_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Ellipse> ellipse = handle<Geom_Ellipse>::DownCast(hgc);
  if (ellipse.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Ellipse."); }
  return ellipse;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Ellipse> e = handle<Geom_Ellipse>::DownCast(hgc);
  if (e.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Ellipse.");
  }
  return e;
#endif
}

bool siren_ellipse_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_ellipse = rb_define_class_under(sr_mSiren, "Ellipse", cls_curve);
  MRB_SET_INSTANCE_TT(cls_ellipse, MRB_TT_DATA);
#endif
  rb_define_method(sr_cEllipse, "initialize", siren_curve_init, -1);
  return true;
}
