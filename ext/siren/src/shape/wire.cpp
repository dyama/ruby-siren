#include "shape/wire.h"
#include "shape.h"

VALUE sr_cWire;

SR_SHAPE_GET(Wire, wire)
SR_SHAPE_CHECK(Wire, wire)

bool siren_wire_install()
{
  SR_SHAPE_INIT(Wire)
  rb_define_method(sr_cWire, "initialize",    RUBY_METHOD_FUNC(siren_shape_init),         -1);
  rb_define_method(sr_cWire, "ordered_edges", RUBY_METHOD_FUNC(siren_wire_ordered_edges), -1);
  rb_define_method(sr_cWire, "curves",        RUBY_METHOD_FUNC(siren_wire_curves),        -1);
  rb_define_singleton_method(sr_cWire, "make", RUBY_METHOD_FUNC(siren_wire_make), -1);
  rb_define_singleton_method(sr_cWire, "join", RUBY_METHOD_FUNC(siren_wire_make), -1);
  return true;
}

VALUE siren_wire_ordered_edges(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Wire wire = siren_wire_get(self);
  VALUE res = rb_ary_new();
  for (BRepTools_WireExplorer exp(wire); exp.More(); exp.Next()) {
    TopoDS_Edge e = exp.Current();
    rb_ary_push(res, siren_shape_new(e));
  }
  return res;
}

VALUE siren_wire_curves(int argc, VALUE* argv, VALUE self)
{
  VALUE res = rb_ary_new();
  VALUE edges = rb_funcall(self, rb_intern("edges"), 0);
  for (int i = 0; i < RARRAY_LEN(edges); i++) {
    VALUE edge = RARRAY_AREF(edges, i);
    VALUE curve = rb_funcall(edge, rb_intern("curve"), 0);
    rb_ary_push(res, curve);
  }
  return res;
}

VALUE siren_wire_make(int argc, VALUE* argv, VALUE self)
{
  VALUE objs;
  VALUE tol;
  rb_scan_args(argc, argv, "A|f", &objs, &tol);
  BRepBuilderAPI_MakeWire api;
#ifdef USE_WIRE_FIX
  ShapeFix_Wire sfw;
  handle<ShapeExtend_WireData> wd = new ShapeExtend_WireData();
  ShapeFix_ShapeTolerance FTol;
  int osize = RARRAY_LEN(objs);
  for (int i = 0; i < osize ; i++) {
    VALUE obj = RARRAY_AREF(objs, i);
    TopoDS_Shape* s = siren_shape_get(obj);
    if (s->IsNull()) {
      continue;
    }
    TopExp_Explorer exp(*s, TopAbs_EDGE);
    for (; exp.More(); exp.Next()) {
      wd->Add(TopoDS::Edge(exp.Current()));
    }
  }
  if (wd->NbEdges() == 0) {
    return Qnil;
  }
  sfw.Load(wd);
  sfw.Perform();
  for (int i = 1; i <= sfw.NbEdges(); i ++) {
    TopoDS_Edge e = sfw.WireData()->Edge(i);
    FTol.SetTolerance(e, argc == 1 ? 0.01 : tol, TopAbs_VERTEX);
    api.Add(e);
  }
  return siren_shape_new(api.Shape());
#else
  ShapeFix_ShapeTolerance fixtol;
  bool has_tol = argc == 2;
  for (int i = 0; i < RARRAY_LEN(objs); i++) {
    TopoDS_Shape* shape = siren_shape_get(RARRAY_AREF(objs, i));
    if (shape->ShapeType() == TopAbs_EDGE) {
      if (has_tol) {
        fixtol.SetTolerance(*shape, tol, TopAbs_VERTEX);
      }
      api.Add(TopoDS::Edge(*shape));
    }
    else if (shape->ShapeType() == TopAbs_WIRE) {
      if (has_tol) {
        fixtol.SetTolerance(*shape, tol, TopAbs_VERTEX);
        fixtol.SetTolerance(*shape, tol, TopAbs_EDGE);
      }
      api.Add(TopoDS::Wire(*shape));
    }
  }
  api.Build();
  if (!api.IsDone()) {
    switch (api.Error()) {
      case BRepBuilderAPI_EmptyWire:
        rb_raise(Qnil, "Failed to make a wire. (Empty wire)");
        break;
      case BRepBuilderAPI_DisconnectedWire:
        rb_raise(Qnil, "Failed to make a wire. (Disconnected wire)");
        break;
      case BRepBuilderAPI_NonManifoldWire:
        rb_raise(Qnil, "Failed to make a wire. (Non manifold wire)");
        break;
      default: break;
    }
  }
#endif
  return siren_shape_new(api.Wire());
}

