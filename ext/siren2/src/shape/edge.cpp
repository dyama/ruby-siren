#include "shape/edge.h"

VALUE sr_cEdge;

SR_SHAPE_GET(Edge, edge)
SR_SHAPE_CHECK(Edge, edge)

bool siren_edge_install()
{
  SR_SHAPE_INIT(Edge)
  rb_define_method(sr_cEdge, "initialize", RUBY_METHOD_FUNC(siren_edge_init),      -1);
  rb_define_method(sr_cEdge, "sp",         RUBY_METHOD_FUNC(siren_edge_sp),        -1);
  rb_define_method(sr_cEdge, "tp",         RUBY_METHOD_FUNC(siren_edge_tp),        -1);
  rb_define_method(sr_cEdge, "to_pts",     RUBY_METHOD_FUNC(siren_edge_to_pts),    -1);
  rb_define_method(sr_cEdge, "param",      RUBY_METHOD_FUNC(siren_edge_param),     -1);
  rb_define_method(sr_cEdge, "to_xyz",     RUBY_METHOD_FUNC(siren_edge_to_xyz),    -1);
  rb_define_method(sr_cEdge, "curvature",  RUBY_METHOD_FUNC(siren_edge_curvature), -1);
  rb_define_method(sr_cEdge, "tangent",    RUBY_METHOD_FUNC(siren_edge_tangent),   -1);
  rb_define_method(sr_cEdge, "extrema",    RUBY_METHOD_FUNC(siren_edge_extrema),   -1);
  rb_define_method(sr_cEdge, "split",      RUBY_METHOD_FUNC(siren_edge_split),     -1);
  rb_define_method(sr_cEdge, "trim",       RUBY_METHOD_FUNC(siren_edge_trim),      -1);
  rb_define_method(sr_cEdge, "terms",      RUBY_METHOD_FUNC(siren_edge_terms),     -1);
  rb_define_method(sr_cEdge, "curve",      RUBY_METHOD_FUNC(siren_edge_curve),     -1);
  return true;
}

VALUE siren_edge_init(int argc, VALUE* argv, VALUE self)
{
  VALUE curve;
  VALUE sp = DBL2NUM(0.0), tp = DBL2NUM(1.0);
  rb_scan_args(argc, argv, "12", &curve, &sp, &tp);
  auto phgcurve = siren_curve_get(curve);
  TopoDS_Shape edge;
  if (argc == 1) {
    edge = BRepBuilderAPI_MakeEdge(*phgcurve);
  }
  else if (argc == 2) {
    rb_raise(Qnil,
        "The start parameter specified without a terminal parameter.");
  }
  else {
    siren_numeric_check(sp);
    siren_numeric_check(tp);
    try {
      edge = BRepBuilderAPI_MakeEdge(*phgcurve, NUM2DBL(sp), NUM2DBL(tp));
      if (edge.IsNull()) {
        rb_raise(Qnil, "Failed to make Edge from the Curve.");
      }
    }
    catch (...) {
      rb_raise(Qnil, "Failed to make Edge from the Curve.");
    }
  }
  auto p = siren_shape_get(self);
  *p = edge;
  return self;
}

VALUE siren_edge_sp(int argc, VALUE* argv, VALUE self)
{
  BRepAdaptor_Curve bracurve(siren_edge_get(self));
  gp_Pnt sp = bracurve.Value(bracurve.FirstParameter());
  return siren_pnt_to_ary(sp);
}

VALUE siren_edge_tp(int argc, VALUE* argv, VALUE self)
{
  BRepAdaptor_Curve bracurve(siren_edge_get(self));
  gp_Pnt tp = bracurve.Value(bracurve.LastParameter());
  return siren_pnt_to_ary(tp);
}

VALUE siren_edge_to_pts(int argc, VALUE* argv, VALUE self)
{
  VALUE deflect;
  VALUE lintol;
  rb_scan_args(argc, argv, "02", &deflect, &lintol);
  if (argc < 1) {
    deflect = DBL2NUM(1.0e-7);
  }
  if (argc < 2) {
    lintol = DBL2NUM(1.0e-7);
  }
  TopoDS_Edge edge = siren_edge_get(self);
  BRepAdaptor_Curve adaptor(edge);
  double first_param, last_param;
  first_param = adaptor.FirstParameter();
  last_param = adaptor.LastParameter();

  VALUE line = rb_ary_new();

  GCPnts_UniformDeflection unidef(adaptor, NUM2DBL(deflect));
  if (unidef.IsDone()) {
    // first point
    gp_Pnt p = adaptor.Value(first_param);
    rb_ary_push(line, siren_pnt_to_ary(p));
    gp_Pnt prev = p;

    for (int i=1; i<=unidef.NbPoints(); i++) {
      p = unidef.Value(i);
      if (prev.IsEqual(p, NUM2DBL(lintol))) {
        continue;
      }
      rb_ary_push(line, siren_pnt_to_ary(p));
      prev = p;
    }
    // last point
    p = adaptor.Value(last_param);
    if (!prev.IsEqual(p, NUM2DBL(lintol))) {
      rb_ary_push(line, siren_pnt_to_ary(p));
    }
  }
  return line;
}

VALUE siren_edge_param(int argc, VALUE* argv, VALUE self)
{
  VALUE xyz;
  VALUE tol = DBL2NUM(1.0e-7);
  rb_scan_args(argc, argv, "11", &xyz, &tol);

  TopoDS_Edge edge = siren_edge_get(self);

  ShapeAnalysis_Curve ana;
  BRepAdaptor_Curve gcurve(edge);
  gp_Pnt p = siren_ary_to_pnt(xyz);
  gp_Pnt pp;
  Standard_Real param;
  Standard_Real distance = ana.Project(gcurve, p, NUM2DBL(tol), pp, param);

  if (fabs(distance) > tol) {
    rb_raise(Qnil, "Specified position is not on the edge.");
  }

  return (param);
}

VALUE siren_edge_to_xyz(int argc, VALUE* argv, VALUE self)
{
  VALUE param;
  rb_scan_args(argc, argv, "1", &param);
  siren_numeric_check(param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(NUM2DBL(param), p, v1, v2);
  return siren_pnt_to_ary(p);
}

VALUE siren_edge_curvature(int argc, VALUE* argv, VALUE self)
{
  VALUE param;
  rb_scan_args(argc, argv, "1", &param);
  siren_numeric_check(param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(NUM2DBL(param), p, v1, v2);
  return siren_vec_new(v2.X(), v2.Y(), v2.Z());
}

VALUE siren_edge_tangent(int argc, VALUE* argv, VALUE self)
{
  VALUE param;
  rb_scan_args(argc, argv, "1", &param);
  siren_numeric_check(param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(NUM2DBL(param), p, v1, v2);
  return siren_vec_new(v1.X(), v1.Y(), v1.Z());
}

VALUE siren_edge_terms(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Edge edge = siren_edge_get(self);
  Standard_Real first, last;
  BRep_Tool::Curve(edge, first, last);
  VALUE res = rb_ary_new();
  rb_ary_push(res, DBL2NUM(first));
  rb_ary_push(res, DBL2NUM(last));
  return res;
}

VALUE siren_edge_curve(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Edge edge = siren_edge_get(self);
  // // set property
  // Standard_Real first, last;
  // handle<Geom_Curve> hgcurve = BRep_Tool::Curve(edge, first, last);
  // rb_obj_iv_set(rb_obj_value(cls_edge), rb_intern_lit("@curve"), siren_curve_new(&hgcurve));
  // // get property
  // return rb_iv_get(self, rb_intern_lit("@curve"));
  Standard_Real first, last;
  handle<Geom_Curve> hgcurve = BRep_Tool::Curve(edge, first, last);
  return siren_curve_new(hgcurve);
}

VALUE siren_edge_extrema(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  siren_edge_check(other);
  TopoDS_Edge e1 = siren_edge_get(self);
  TopoDS_Edge e2 = siren_edge_get(other);
  BRepExtrema_ExtCC ext(e1, e2);
  if (!ext.IsDone()) {
    rb_raise(Qnil, "Failed to get extrema points.");
  }
  else if (ext.IsParallel()) {
    return Qnil;
  }
  VALUE p1s = rb_ary_new();
  VALUE p2s = rb_ary_new();
  for (int i = 1; i <= ext.NbExt(); i++) {
    rb_ary_push(p1s, DBL2NUM(ext.ParameterOnE1(i)));
    rb_ary_push(p2s, DBL2NUM(ext.ParameterOnE2(i)));
  }
  VALUE res[2] = { p1s, p2s };
  return rb_ary_new_from_values(2, res);
}

VALUE siren_edge_split(int argc, VALUE* argv, VALUE self)
{
  VALUE param;
  rb_scan_args(argc, argv, "1", &param);
  siren_numeric_check(param);
  Standard_Real first, last;
  TopoDS_Edge e = siren_edge_get(self);
  handle<Geom_Curve> gc  = BRep_Tool::Curve(e, first, last);
  if (NUM2DBL(param) <= first || NUM2DBL(param) >= last) {
    rb_raise(Qnil, "Specified parameter is out of range of curve parameter.");
  }
  TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(gc, first, NUM2DBL(param));
  TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(gc, NUM2DBL(param), last);
  VALUE res[] = { siren_shape_new(e1), siren_shape_new(e2) };
  return rb_ary_new_from_values(2, res);
}

VALUE siren_edge_trim(int argc, VALUE* argv, VALUE self)
{
  VALUE first2, last2;
  rb_scan_args(argc, argv, "2", &first2, &last2);
  siren_numeric_check(first2);
  siren_numeric_check(last2);
  if (NUM2DBL(first2) == NUM2DBL(last2)) {
    rb_raise(Qnil, "Specified parameter has same value.");
  }
  Standard_Real first, last;
  TopoDS_Edge e = siren_edge_get(self);
  handle<Geom_Curve> gc  = BRep_Tool::Curve(e, first, last);
  TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(gc, NUM2DBL(first2), NUM2DBL(last2));
  return siren_shape_new(edge);
}
