/**
 * ellipse.cpp
 * Implementation of singleton methods for ELLIPSE
 */

#include "curve.h"

VALUE siren_ellipse_new( const handle<Geom_Curve>* curve)
{
  struct RClass* mod_siren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Ellipse")));
  void* p = rb_malloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_ellipse_type;
  return obj;
}

handle<Geom_Ellipse> siren_ellipse_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_ellipse_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Ellipse> ellipse = handle<Geom_Ellipse>::DownCast(hgc);
  if (ellipse.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Ellipse."); }
  return ellipse;
}

bool siren_ellipse_install( struct RClass* mod_siren)
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_ellipse = rb_define_class_under(mod_siren, "Ellipse", cls_curve);
  MRB_SET_INSTANCE_TT(cls_ellipse, MRB_TT_DATA);
  rb_define_method(cls_ellipse, "initialize", siren_curve_init, MRB_ARGS_NONE());
  return true;
}
