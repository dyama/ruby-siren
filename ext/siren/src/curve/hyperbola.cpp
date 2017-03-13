/**
 * hyperbola.cpp
 * Implementation of singleton methods for HYPERBOLA
 */

#include "curve.h"

VALUE siren_hyperbola_new( const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cHyperbola);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_hyperbola_type;
#endif
  return obj;
}

handle<Geom_Hyperbola> siren_hyperbola_get(VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_hyperbola_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Hyperbola> hyperbola = handle<Geom_Hyperbola>::DownCast(hgc);
  if (hyperbola.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Hyperbola."); }
  return hyperbola;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Hyperbola> bola = handle<Geom_Hyperbola>::DownCast(hgc);
  if (bola.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Hyperbola.");
  }
  return bola;
#endif
}

bool siren_hyperbola_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_hyperbola = rb_define_class_under(sr_mSiren, "Hyperbola", cls_curve);
  MRB_SET_INSTANCE_TT(cls_hyperbola, MRB_TT_DATA);
#endif
  rb_define_method(sr_cHyperbola, "initialize", siren_curve_init, -1);
  return true;
}
