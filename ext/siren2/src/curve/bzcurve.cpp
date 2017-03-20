/**
 * bzcurve.cpp
 */

#include "curve.h"

VALUE sr_cBzCurve;

SR_CURVE_INIT(BezierCurve, BzCurve, bzcurve)

bool siren_bzcurve_install()
{
  SR_CURVE_DEFINE(BzCurve)
  rb_define_method(sr_cBzCurve, "initialize", RUBY_METHOD_FUNC(siren_bzcurve_init), -1);
  return true;
}

VALUE siren_bzcurve_init(int argc, VALUE* argv, VALUE self)
{
  VALUE ps, ws;
  rb_scan_args(argc, argv, "11", &ps, &ws);
  bool has_weight = argc == 2;
  int plen = RARRAY_LEN(ps);
  TColgp_Array1OfPnt poles(1, plen);
  TColStd_Array1OfReal weights(1, plen);
  // Start index of weights must be 1. Crash construction of Geom_BezierCurve
  // if another index specified.
  for (int i = 0; i < plen; i++) {
    poles.SetValue(i + 1, siren_ary_to_pnt(RARRAY_AREF(ps, i)));
    if (has_weight) {
      VALUE w = RARRAY_AREF(ws, i);
      weights.SetValue(i + 1, VALUE(w));
    }
  }
  handle<Geom_Curve> curve = nullptr;
  try {
    if (has_weight) {
      curve = new Geom_BezierCurve(poles, weights);
    }
    else {
      curve = new Geom_BezierCurve(poles);
    }
  }
  catch (...) {
    rb_raise(Qnil, "Failed to make a BzCurve.");
  }
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = curve;
  DATA_PTR(self) = hgcurve;
#if 0
  DATA_TYPE(self) = &siren_bzcurve_type;
#endif
  return self;
}
