#include "shape/wire.h"

VALUE siren_wire_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Wire")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_wire_type;
  return obj;
}

TopoDS_Wire siren_wire_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_wire_type));
  TopoDS_Wire wire = TopoDS::Wire(*shape);
  if (wire.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Wire."); }
  return wire;
}

bool siren_wire_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_wire = rb_define_class_under(sr_mSiren, "Wire", cls_shape);
  MRB_SET_INSTANCE_TT(cls_wire, MRB_TT_DATA);
  rb_define_method(cls_wire, "initialize",    siren_shape_init,         MRB_ARGS_NONE());
  rb_define_method(cls_wire, "ordered_edges", siren_wire_ordered_edges, MRB_ARGS_NONE());
  rb_define_method(cls_wire, "curves",        siren_wire_curves,        MRB_ARGS_NONE());

  auto obj_wire = rb_obj_ptr(siren_wire_obj());
  rb_define_singleton_method(obj_wire, "make", siren_wire_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_singleton_method(obj_wire, "join", siren_wire_make, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  return true;
}

struct RClass* siren_wire_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Wire")));
}

VALUE siren_wire_ordered_edges( VALUE self)
{
  TopoDS_Wire wire = siren_wire_get(self);
  VALUE res = rb_ary_new();
  for (BRepTools_WireExplorer exp(wire); exp.More(); exp.Next()) {
    TopoDS_Edge e = exp.Current();
    rb_ary_push(res, siren_shape_new(e));
  }
  return res;
}

VALUE siren_wire_curves( VALUE self)
{
  VALUE res = rb_ary_new();
  VALUE edges = rb_funcall(self, "edges", 0);
  for (int i = 0; i < rb_ary_len(edges); i++) {
    VALUE edge = rb_ary_ref(edges, i);
    VALUE curve = rb_funcall(edge, "curve", 0);
    rb_ary_push(res, curve);
  }
  return res;
}

VALUE siren_wire_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Wire"));
}

VALUE siren_wire_make( VALUE self)
{
  VALUE objs;
  VALUE tol;
  int argc = rb_scan_args("A|f", &objs, &tol);
  BRepBuilderAPI_MakeWire api;
#ifdef USE_WIRE_FIX
  ShapeFix_Wire sfw;
  handle<ShapeExtend_WireData> wd = new ShapeExtend_WireData();
  ShapeFix_ShapeTolerance FTol;
  int osize = rb_ary_len(objs);
  for (int i = 0; i < osize ; i++) {
    VALUE obj = rb_ary_ref(objs, i);
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
  for (int i = 0; i < rb_ary_len(objs); i++) {
    TopoDS_Shape* shape = siren_shape_get(rb_ary_ref(objs, i));
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
        rb_raise(E_ARGUMENT_ERROR, "Failed to make a wire. (Empty wire)");
        break;
      case BRepBuilderAPI_DisconnectedWire:
        rb_raise(E_ARGUMENT_ERROR, "Failed to make a wire. (Disconnected wire)");
        break;
      case BRepBuilderAPI_NonManifoldWire:
        rb_raise(E_ARGUMENT_ERROR, "Failed to make a wire. (Non manifold wire)");
        break;
      default: break;
    }
  }
#endif
  return siren_wire_new(&api.Wire());
}
