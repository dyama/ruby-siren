/**
 * hyperbola.cpp
 * Implementation of singleton methods for HYPERBOLA
 */

#include "curve.h"

VALUE siren_hyperbola_new( const handle<Geom_Curve>* curve)
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Hyperbola")));
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_hyperbola_type;
  return obj;
}

handle<Geom_Hyperbola> siren_hyperbola_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_hyperbola_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Hyperbola> hyperbola = handle<Geom_Hyperbola>::DownCast(hgc);
  if (hyperbola.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Hyperbola."); }
  return hyperbola;
}

bool siren_hyperbola_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_hyperbola = rb_define_class_under(sr_mSiren, "Hyperbola", cls_curve);
  MRB_SET_INSTANCE_TT(cls_hyperbola, MRB_TT_DATA);
  rb_define_method(cls_hyperbola, "initialize", siren_curve_init, MRB_ARGS_NONE());
  return true;
}
