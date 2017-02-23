/**
 * bscurve.cpp
 */

#include "curve.h"

VALUE siren_bscurve_new( const handle<Geom_Curve>* curve)
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("BSCurve")));
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_bscurve_type;
  return obj;
}

handle<Geom_BSplineCurve> siren_bscurve_get( VALUE self)
{
  handle<Geom_Curve> hgc
    = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_bscurve_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_BSplineCurve> bscurve = handle<Geom_BSplineCurve>::DownCast(hgc);
  if (bscurve.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not BSCurve."); }
  return bscurve;
}

bool siren_bscurve_install()
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_bscurve = rb_define_class_under(sr_mSiren, "BSCurve", cls_curve);
  MRB_SET_INSTANCE_TT(cls_bscurve, MRB_TT_DATA);
  rb_define_method(cls_bscurve, "initialize", siren_bscurve_init,    MRB_ARGS_REQ(4) | MRB_ARGS_OPT(1));
  rb_define_method(cls_bscurve, "degree",     siren_bscurve_degree,  MRB_ARGS_NONE());
  rb_define_method(cls_bscurve, "knots",      siren_bscurve_knots,   MRB_ARGS_NONE());
  rb_define_method(cls_bscurve, "mults",      siren_bscurve_mults,   MRB_ARGS_NONE());
  rb_define_method(cls_bscurve, "poles",      siren_bscurve_poles,   MRB_ARGS_NONE());
  rb_define_method(cls_bscurve, "weights",    siren_bscurve_weights, MRB_ARGS_NONE());
  return true;
}

VALUE siren_bscurve_init( VALUE self)
{
  VALUE d;
  VALUE ks, ms, ps, ws;
  int argc = rb_scan_args("iAAA|A", &d, &ks, &ms, &ps, &ws);

  int plen = rb_ary_len(ps);

  TColgp_Array1OfPnt poles(0, plen - 1);
  TColStd_Array1OfReal weights(0, plen - 1);
  for (int i=0; i < plen; i++) {
    poles.SetValue(i, siren_ary_to_pnt(rb_ary_ref(ps, i)));
    if (argc >= 5) {
      VALUE w = rb_ary_ref(ws, i);
      weights.SetValue(i, VALUE(w));
    }
    else {
      weights.SetValue(i, 1.0);
    }
  }

  int klen = rb_ary_len(ks);
  TColStd_Array1OfReal knots(0, klen - 1);
  TColStd_Array1OfInteger mults(0, klen - 1);

  for (int i=0; i < klen; i++) {
    VALUE knot = rb_ary_ref(ks, i);
    knots.SetValue(i, VALUE(knot));
    VALUE mult = rb_ary_ref(ms, i);
    mults.SetValue(i, (Standard_Integer)rb_fixnum(mult));
  }

  handle<Geom_Curve> curve
    = new Geom_BSplineCurve(poles, weights, knots, mults, d, Standard_False);

  // initialize において self は既に rb_instance_alloc されているので、
  // DATA_PTR と DATA_TYPE のみを設定する
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = curve;
  DATA_PTR(self) = hgcurve;
  DATA_TYPE(self) = &siren_bscurve_type;
  return self;
}

VALUE siren_bscurve_degree( VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  return rb_fixnum_value((int)curve->Degree());
}

VALUE siren_bscurve_knots( VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE knots = rb_ary_new();
  for (int i = 1; i <= curve->NbKnots(); i++) {
    rb_ary_push(knots, (curve->Knot(i)));
  }
  return knots;
}

VALUE siren_bscurve_mults( VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE mults = rb_ary_new();
  for (int i = 1; i <= curve->NbKnots(); i++) {
    rb_ary_push(mults, rb_fixnum_value(curve->Multiplicity(i)));
  }
  return mults;
}

VALUE siren_bscurve_poles( VALUE self)
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

VALUE siren_bscurve_weights( VALUE self)
{
  handle<Geom_BSplineCurve> curve = siren_bscurve_get(self);
  VALUE weights = rb_ary_new();
  for (int i = 1; i <= curve->NbPoles(); i++) {
    rb_ary_push(weights, (curve->Weight(i)));
  }
  return weights;
}

