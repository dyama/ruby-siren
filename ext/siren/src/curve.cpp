/*
 * Document-class: Curve
 *
 */

#include "curve.h"

VALUE siren_curve_new( const opencascade::handle<Geom_Curve>* curve)
{
  GeomAbs_CurveType type = siren_curve_geomtype_native(*curve);
  struct RClass* mod_siren = rb_module_get("Siren");
  switch (type) {
    case GeomAbs_Line:         return siren_line_new(curve); break;
    case GeomAbs_Circle:       return siren_circle_new(curve); break;
    case GeomAbs_Ellipse:      return siren_ellipse_new(curve); break;
    case GeomAbs_Hyperbola:    return siren_hyperbola_new(curve); break;
    case GeomAbs_Parabola:     return siren_parabola_new(curve); break;
    case GeomAbs_BezierCurve:  return siren_bzcurve_new(curve); break;
    case GeomAbs_BSplineCurve: return siren_bscurve_new(curve); break;
    case GeomAbs_OffsetCurve:  return siren_offsetcurve_new(curve); break;
    default: break;
  }
  // rIght?
  VALUE obj = rb_instance_alloc(rb_obj_value(siren_curve_rclass()));
  void* p = rb_malloc(sizeof(opencascade::handle<Geom_Curve>));
  opencascade::handle<Geom_Curve>* hgcurve = new(p) opencascade::handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_curve_type;
  return obj;
}

bool siren_curve_install( struct RClass* mod_siren)
{
  struct RClass* cls_curve = rb_define_class_under(mod_siren, "Curve", mrb->object_class);
  MRB_SET_INSTANCE_TT(cls_curve, MRB_TT_DATA);
  rb_define_method(cls_curve, "initialize", siren_curve_init,     MRB_ARGS_NONE());

  // Define derived classes for Siren::Curve
  siren_line_install(mod_siren);
  siren_circle_install(mod_siren);
  siren_ellipse_install(mod_siren);
  siren_hyperbola_install(mod_siren);
  siren_parabola_install(mod_siren);
  siren_bzcurve_install(mod_siren);
  siren_bscurve_install(mod_siren);
  siren_offsetcurve_install(mod_siren);

  return true;
}

VALUE siren_curve_init( VALUE self)
{
  rb_raise(E_NOMETHOD_ERROR, "private method `new' called for Curve:Class");
  return Qnil;
}

void siren_curve_final( void* p)
{
  opencascade::handle<Geom_Curve>* hgcurve = static_cast<opencascade::handle<Geom_Curve>*>(p);
  if (!(*hgcurve).IsNull()) {
    (*hgcurve).Nullify();
  }
  rb_free(p);
}

opencascade::handle<Geom_Curve>* siren_curve_get( VALUE obj)
{
  return static_cast<opencascade::handle<Geom_Curve>*>(_get_datatype(obj, &siren_curve_type));
}

struct RClass* siren_curve_rclass()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(mod_siren), rb_intern_lit("Curve")));
}

