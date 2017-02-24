/*
 * Document-class: Curve
 *
 */

#include "curve.h"

VALUE siren_curve_new( const opencascade::handle<Geom_Curve>* curve)
{
  GeomAbs_CurveType type = siren_curve_geomtype_native(*curve);
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
  VALUE obj = rb_instance_alloc(sr_cCurve);
  void* p = ruby_xmalloc(sizeof(opencascade::handle<Geom_Curve>));
  opencascade::handle<Geom_Curve>* hgcurve = new(p) opencascade::handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  // DATA_TYPE(obj) = &siren_curve_type;
  return obj;
}

bool siren_curve_install()
{
  sr_cCurve = rb_define_class_under(sr_mSiren, "Curve", rb_cObject);
  // MRB_SET_INSTANCE_TT(cls_curve, MRB_TT_DATA);
  rb_define_method(sr_cCurve, "initialize", siren_curve_init, -1);

  // Define derived classes for Siren::Curve
  siren_line_install();
  siren_circle_install();
  siren_ellipse_install();
  siren_hyperbola_install();
  siren_parabola_install();
  siren_bzcurve_install();
  siren_bscurve_install();
  siren_offsetcurve_install();

  return true;
}

VALUE siren_curve_init(int argc, VALUE* argv, VALUE self)
{
  rb_raise(Qnil, "private method `new' called for Curve:Class");
  return Qnil;
}

void siren_curve_final(void* p)
{
  opencascade::handle<Geom_Curve>* hgcurve = static_cast<opencascade::handle<Geom_Curve>*>(p);
  if (!(*hgcurve).IsNull()) {
    (*hgcurve).Nullify();
  }
  ruby_xfree(p);
}

opencascade::handle<Geom_Curve>* siren_curve_get(VALUE obj)
{
#if 0
  return static_cast<opencascade::handle<Geom_Curve>*>(_get_datatype(obj, &siren_curve_type));
#else
  opencascade::handle<Geom_Curve>* m;
  Data_Get_Struct(obj, opencascade::handle<Geom_Curve>, m);
  return m;
#endif
}

