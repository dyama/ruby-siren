/**
 * bzcurve.cpp
 */

#include "curve.h"

VALUE siren_bzcurve_new( const handle<Geom_Curve>* curve)
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("BzCurve")));
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_bzcurve_type;
  return obj;
}

handle<Geom_BezierCurve> siren_bzcurve_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_bzcurve_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_BezierCurve> bzcurve = handle<Geom_BezierCurve>::DownCast(hgc);
  if (bzcurve.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not BzCurve."); }
  return bzcurve;
}

bool siren_bzcurve_install()
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_bzcurve = rb_define_class_under(sr_mSiren, "BzCurve", cls_curve);
  MRB_SET_INSTANCE_TT(cls_bzcurve, MRB_TT_DATA);
  rb_define_method(cls_bzcurve, "initialize", siren_bzcurve_init,   MRB_ARGS_NONE());
  // rb_define_method(cls_bzcurve, "degree",     siren_bzcurve_degree, MRB_ARGS_NONE());
  return true;
}

VALUE siren_bzcurve_init( VALUE self)
{
  VALUE ps, ws;
  rb_scan_args(argc, argv, "A|A", &ps, &ws);
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
  DATA_TYPE(self) = &siren_bzcurve_type;
  return self;
}

