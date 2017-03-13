/**
 * parabola.cpp
 * Implementation of singleton methods for PARABOLA
 */

#include "curve.h"

VALUE sr_cParabola;

VALUE siren_parabola_new( const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cParabola);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_parabola_type;
#endif
  return obj;
}

handle<Geom_Parabola> siren_parabola_get(VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_parabola_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Parabola> parabola = handle<Geom_Parabola>::DownCast(hgc);
  if (parabola.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Parabola."); }
  return parabola;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Parabola> parabola = handle<Geom_Parabola>::DownCast(hgc);
  if (parabola.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Parabola.");
  }
  return parabola;
#endif
}

bool siren_parabola_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_parabola = rb_define_class_under(sr_mSiren, "Parabola", cls_curve);
  MRB_SET_INSTANCE_TT(cls_parabola, MRB_TT_DATA);
#endif
  rb_define_method(sr_cParabola, "initialize", siren_curve_init, -1);
  return true;
}
