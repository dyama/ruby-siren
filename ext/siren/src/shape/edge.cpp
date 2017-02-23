#include "shape/edge.h"

VALUE siren_edge_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Edge")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_edge_type;
  return obj;
}

TopoDS_Edge siren_edge_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_edge_type));
  TopoDS_Edge edge = TopoDS::Edge(*shape);
  if (edge.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Edge."); }
  return edge;
}

bool siren_edge_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_edge = rb_define_class_under(sr_mSiren, "Edge", cls_shape);
  MRB_SET_INSTANCE_TT(cls_edge, MRB_TT_DATA);
  rb_define_method(cls_edge, "initialize", siren_edge_init,      MRB_ARGS_NONE());
  rb_define_method(cls_edge, "sp",         siren_edge_sp,        MRB_ARGS_NONE());
  rb_define_method(cls_edge, "tp",         siren_edge_tp,        MRB_ARGS_NONE());
  rb_define_method(cls_edge, "to_pts",     siren_edge_to_pts,    MRB_ARGS_OPT(2));
  rb_define_method(cls_edge, "param",      siren_edge_param,     MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method(cls_edge, "to_xyz",     siren_edge_to_xyz,    MRB_ARGS_REQ(1));
  rb_define_method(cls_edge, "curvature",  siren_edge_curvature, MRB_ARGS_REQ(1));
  rb_define_method(cls_edge, "tangent",    siren_edge_tangent,   MRB_ARGS_REQ(1));
  rb_define_method(cls_edge, "extrema",    siren_edge_extrema,   MRB_ARGS_REQ(1));
  rb_define_method(cls_edge, "split",      siren_edge_split,     MRB_ARGS_REQ(1));
  rb_define_method(cls_edge, "trim",       siren_edge_trim,      MRB_ARGS_REQ(2));
  rb_define_method(cls_edge, "terms",      siren_edge_terms,     MRB_ARGS_NONE());
  rb_define_method(cls_edge, "length",     siren_edge_length,    MRB_ARGS_NONE());
  rb_define_method(cls_edge, "curve",      siren_edge_curve,     MRB_ARGS_NONE());
  return true;
}

VALUE siren_edge_init( VALUE self)
{
  VALUE curve;
  VALUE sp = 0.0, tp = 1.0;
  int argc = rb_scan_args("o|ff", &curve, &sp, &tp);
  opencascade::handle<Geom_Curve>* phgcurve
    = static_cast<opencascade::handle<Geom_Curve>*>(DATA_PTR(curve));
  TopoDS_Shape edge;
  if (argc == 1) {
    edge = BRepBuilderAPI_MakeEdge(*phgcurve);
  }
  else if (argc == 2) {
    rb_raise(E_ARGUMENT_ERROR,
        "The start parameter specified without a terminal parameter.");
  }
  else {
    try {
      edge = BRepBuilderAPI_MakeEdge(*phgcurve, sp, tp);
      if (edge.IsNull()) {
        rb_raise(E_RUNTIME_ERROR, "Failed to make Edge from the Curve.");
      }
    }
    catch (...) {
      rb_raise(E_RUNTIME_ERROR, "Failed to make Edge from the Curve.");
    }
  }
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = edge; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(self) = &siren_edge_type;
  return self;
}

struct RClass* siren_edge_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Edge")));
}

VALUE siren_edge_sp( VALUE self)
{
  BRepAdaptor_Curve bracurve(siren_edge_get(self));
  gp_Pnt sp = bracurve.Value(bracurve.FirstParameter());
  return siren_pnt_to_ary(sp);
}

VALUE siren_edge_tp( VALUE self)
{
  BRepAdaptor_Curve bracurve(siren_edge_get(self));
  gp_Pnt tp = bracurve.Value(bracurve.LastParameter());
  return siren_pnt_to_ary(tp);
}

VALUE siren_edge_to_pts( VALUE self)
{
  VALUE deflect = 1.0e-7;
  VALUE lintol = 1.0e-7;
  int argc = rb_scan_args("|ff", &deflect, &lintol);
  TopoDS_Edge edge = siren_edge_get(self);
  BRepAdaptor_Curve adaptor(edge);
  double first_param, last_param;
  first_param = adaptor.FirstParameter();
  last_param = adaptor.LastParameter();

  VALUE line = rb_ary_new();

  GCPnts_UniformDeflection unidef(adaptor, deflect);
  if (unidef.IsDone()) {
    // first point
    gp_Pnt p = adaptor.Value(first_param);
    rb_ary_push(line, siren_pnt_to_ary(p));
    gp_Pnt prev = p;

    for (int i=1; i<=unidef.NbPoints(); i++) {
      p = unidef.Value(i);
      if (prev.IsEqual(p, lintol)) {
        continue;
      }
      rb_ary_push(line, siren_pnt_to_ary(p));
      prev = p;
    }
    // last point
    p = adaptor.Value(last_param);
    if (!prev.IsEqual(p, lintol)) {
      rb_ary_push(line, siren_pnt_to_ary(p));
    }
  }
  return line;
}

VALUE siren_edge_param( VALUE self)
{
  VALUE xyz;
  VALUE tol = 1.0e-7;
  int argc = rb_scan_args("A|f", &xyz, &tol);

  TopoDS_Edge edge = siren_edge_get(self);

  ShapeAnalysis_Curve ana;
  BRepAdaptor_Curve gcurve(edge);
  gp_Pnt p = siren_ary_to_pnt(xyz);
  gp_Pnt pp;
  Standard_Real param;
  Standard_Real distance = ana.Project(gcurve, p, tol, pp, param);

  if (fabs(distance) > tol) {
    rb_raise(E_ARGUMENT_ERROR, "Specified position is not on the edge.");
  }

  return (param);
}

VALUE siren_edge_to_xyz( VALUE self)
{
  VALUE param;
  int argc = rb_scan_args("f", &param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(param, p, v1, v2);
  return siren_pnt_to_ary(p);
}

VALUE siren_edge_curvature( VALUE self)
{
  VALUE param;
  int argc = rb_scan_args("f", &param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(param, p, v1, v2);
  return siren_vec_new(v2.X(), v2.Y(), v2.Z());
}

VALUE siren_edge_tangent( VALUE self)
{
  VALUE param;
  int argc = rb_scan_args("f", &param);
  BRepAdaptor_Curve C(siren_edge_get(self));
  gp_Pnt p;
  gp_Vec v1, v2;
  C.D2(param, p, v1, v2);
  return siren_vec_new(v1.X(), v1.Y(), v1.Z());
}

VALUE siren_edge_terms( VALUE self)
{
  TopoDS_Edge edge = siren_edge_get(self);
  Standard_Real first, last;
  BRep_Tool::Curve(edge, first, last);
  VALUE res = rb_ary_new();
  rb_ary_push(res, (first));
  rb_ary_push(res, (last));
  return res;
}

VALUE siren_edge_curve( VALUE self)
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
  return siren_curve_new(&hgcurve);
}

VALUE siren_edge_extrema( VALUE self)
{
  VALUE other;
  int argc = rb_scan_args("o", &other);
  TopoDS_Edge e2 = siren_edge_get(other);
  if (e2.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Specified shape type is not edge.");
  }
  TopoDS_Edge e1 = siren_edge_get(self);
  BRepExtrema_ExtCC ext(e1, e2);
  if (!ext.IsDone()) {
    rb_raise(E_RUNTIME_ERROR, "Failed to get extrema points.");
  }
  else if (ext.IsParallel()) {
    return Qnil;
  }
  VALUE p1s = rb_ary_new();
  VALUE p2s = rb_ary_new();
  for (int i = 1; i <= ext.NbExt(); i++) {
    rb_ary_push(p1s, (ext.ParameterOnE1(i)));
    rb_ary_push(p2s, (ext.ParameterOnE2(i)));
  }
  VALUE res[2] = { p1s, p2s };
  return rb_ary_new_from_values(2, res);
}

VALUE siren_edge_split( VALUE self)
{
  VALUE param;
  int argc = rb_scan_args("f", &param);
  Standard_Real first, last;
  TopoDS_Edge e = siren_edge_get(self);
  handle<Geom_Curve> gc  = BRep_Tool::Curve(e, first, last);
  if (param <= first || param >= last) {
    rb_raise(E_ARGUMENT_ERROR, "Specified parameter is out of range of curve parameter.");
  }
  TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(gc, first, param);
  TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(gc, param, last);
  VALUE res[] = { siren_shape_new(e1), siren_shape_new(e2) };
  return rb_ary_new_from_values(2, res);
}

VALUE siren_edge_trim( VALUE self)
{
  VALUE first2, last2;
  int argc = rb_scan_args("ff", &first2, &last2);
  if (first2 == last2) {
    rb_raise(E_ARGUMENT_ERROR, "Specified parameter has same value.");
  }
  Standard_Real first, last;
  TopoDS_Edge e = siren_edge_get(self);
  handle<Geom_Curve> gc  = BRep_Tool::Curve(e, first, last);
  TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(gc, first2, last2);
  return siren_shape_new(edge);
}

VALUE siren_edge_length( VALUE self)
{
  Standard_Real res = 0.0;
  VALUE ary = siren_edge_to_pts(self);
  gp_Pnt prev;
  for (int i=0; i < rb_ary_len(ary); i++) {
    if (i == 0) {
      prev = siren_ary_to_pnt(rb_ary_ref(ary, i));
    }
    else {
      gp_Pnt curr = siren_ary_to_pnt(rb_ary_ref(ary, i));
      res += curr.Distance(prev);
      prev = curr;
    }
  }
  return (res);
}

VALUE siren_edge_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Edge"));
}
