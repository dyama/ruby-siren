/**
 * bscurve.cpp
 */

#include "curve.h"

VALUE sr_cBSCurve;

SR_CURVE_INIT(BSplineCurve, BSCurve, bscurve)

bool siren_bscurve_install()
{
  SR_CURVE_DEFINE(BSCurve)
  rb_define_method(sr_cBSCurve, "initialize", RUBY_METHOD_FUNC(siren_bscurve_init),    -1);
  rb_define_method(sr_cBSCurve, "degree",     RUBY_METHOD_FUNC(siren_bscurve_degree),  -1);
  rb_define_method(sr_cBSCurve, "knots",      RUBY_METHOD_FUNC(siren_bscurve_knots),   -1);
  rb_define_method(sr_cBSCurve, "mults",      RUBY_METHOD_FUNC(siren_bscurve_mults),   -1);
  rb_define_method(sr_cBSCurve, "poles",      RUBY_METHOD_FUNC(siren_bscurve_poles),   -1);
  rb_define_method(sr_cBSCurve, "weights",    RUBY_METHOD_FUNC(siren_bscurve_weights), -1);
  return true;
}

VALUE siren_bscurve_init(int argc, VALUE* argv, VALUE self)
{
  VALUE d;
  VALUE ks, ms, ps, ws;
  rb_scan_args(argc, argv, "41", &d, &ks, &ms, &ps, &ws);

  int plen = RARRAY_LEN(ps);

  TColgp_Array1OfPnt poles(0, plen - 1);
  TColStd_Array1OfReal weights(0, plen - 1);
  for (int i=0; i < plen; i++) {
    poles.SetValue(i, siren_ary_to_pnt(RARRAY_AREF(ps, i)));
    if (argc >= 5) {
      VALUE w = RARRAY_AREF(ws, i);
      weights.SetValue(i, VALUE(w));
    }
    else {
      weights.SetValue(i, 1.0);
    }
  }

  int klen = RARRAY_LEN(ks);
  TColStd_Array1OfReal knots(0, klen - 1);
  TColStd_Array1OfInteger mults(0, klen - 1);

  for (int i=0; i < klen; i++) {
    VALUE knot = RARRAY_AREF(ks, i);
    knots.SetValue(i, VALUE(knot));
    VALUE mult = RARRAY_AREF(ms, i);
    mults.SetValue(i, (Standard_Integer)DBL2NUM(mult));
  }

  handle<Geom_Curve> curve
    = new Geom_BSplineCurve(poles, weights, knots, mults, d, Standard_False);

  // initialize において self は既に rb_instance_alloc されているので、
  // DATA_PTR と DATA_TYPE のみを設定する
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = curve;
  DATA_PTR(self) = hgcurve;
#if 0
  DATA_TYPE(self) = &siren_bscurve_type;
#endif
  return self;
}

VALUE siren_bscurve_degree(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  return INT2FIX((int)curve->Degree());
}

VALUE siren_bscurve_knots(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE knots = rb_ary_new();
  for (int i = 1; i <= curve->NbKnots(); i++) {
    rb_ary_push(knots, (curve->Knot(i)));
  }
  return knots;
}

VALUE siren_bscurve_mults(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE mults = rb_ary_new();
  for (int i = 1; i <= curve->NbKnots(); i++) {
    rb_ary_push(mults, INT2FIX(curve->Multiplicity(i)));
  }
  return mults;
}

VALUE siren_bscurve_poles(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE poles = rb_ary_new();
  for (int i = 1; i <= curve->NbPoles(); i++) {
    VALUE item = rb_ary_new();
    rb_ary_push(item, (curve->Pole(i).X()));
    rb_ary_push(item, (curve->Pole(i).Y()));
    rb_ary_push(item, (curve->Pole(i).Z()));
    rb_ary_push(poles, item);
  }
  return poles;
}

VALUE siren_bscurve_weights(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE weights = rb_ary_new();
  for (int i = 1; i <= curve->NbPoles(); i++) {
    rb_ary_push(weights, (curve->Weight(i)));
  }
  return weights;
}
