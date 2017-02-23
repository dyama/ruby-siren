/**
 * parabola.cpp
 * Implementation of singleton methods for PARABOLA
 */

#include "curve.h"

VALUE siren_parabola_new( const handle<Geom_Curve>* curve)
{
  struct RClass* mod_siren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(mod_siren), rb_intern_lit("Parabola")));
  void* p = rb_malloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_parabola_type;
  return obj;
}

handle<Geom_Parabola> siren_parabola_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_parabola_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Parabola> parabola = handle<Geom_Parabola>::DownCast(hgc);
  if (parabola.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Parabola."); }
  return parabola;
}

bool siren_parabola_install( struct RClass* mod_siren)
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_parabola = rb_define_class_under(mod_siren, "Parabola", cls_curve);
  MRB_SET_INSTANCE_TT(cls_parabola, MRB_TT_DATA);
  rb_define_method(cls_parabola, "initialize", siren_curve_init, MRB_ARGS_NONE());
  return true;
}
