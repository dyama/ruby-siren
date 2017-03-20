/*
 * Document-class: Curve
 *
 */

#include "curve.h"

VALUE sr_cCurve;

const rb_data_type_t siren_curve_type = {
  "Siren::Curve",
  { 0, siren_curve_final, },
  0, 0, 0,
};

VALUE siren_curve_allocate(VALUE klass)
{
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  new(p) handle<Geom_Curve>();
  return Data_Wrap_Struct(klass, NULL, siren_curve_final, p);
}

handle<Geom_Curve>* siren_curve_get(VALUE obj)
{
  handle<Geom_Curve>* m;
  Data_Get_Struct(obj, handle<Geom_Curve>, m);
  return m;
}

VALUE siren_curve_new(handle<Geom_Curve> curve)
{
  VALUE klass;
  GeomAbs_CurveType type = siren_curve_geomtype_native(curve);
  switch (type) {
    case GeomAbs_Line:         klass = sr_cLine; break;
    case GeomAbs_Circle:       klass = sr_cCircle; break;
    case GeomAbs_Ellipse:      klass = sr_cEllipse; break;
    case GeomAbs_Hyperbola:    klass = sr_cHyperbola; break;
    case GeomAbs_Parabola:     klass = sr_cParabola; break;
    case GeomAbs_BezierCurve:  klass = sr_cBzCurve; break;
    case GeomAbs_BSplineCurve: klass = sr_cBSCurve; break;
    case GeomAbs_OffsetCurve:  klass = sr_cOffsetCurve; break;
    default: rb_raise(Qnil, "Failed to make Curve object."); break;
  }
  auto val = siren_curve_allocate(klass);
  auto pc = siren_curve_get(val);
  *pc = curve;
  return val;
}

bool siren_curve_install()
{
  sr_cCurve = rb_define_class_under(sr_mSiren, "Curve", rb_cObject);
  rb_define_alloc_func(sr_cCurve, siren_curve_allocate);
  rb_define_method(sr_cCurve, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
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
  handle<Geom_Curve>* hgcurve = static_cast<handle<Geom_Curve>*>(p);
  if (!(*hgcurve).IsNull()) {
    (*hgcurve).Nullify();
  }
  ruby_xfree(p);
}

