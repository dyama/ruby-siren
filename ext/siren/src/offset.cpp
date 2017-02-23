#include "offset.h"

bool siren_offset_install( struct RClass* mod_siren)
{
  // Class method
  rb_define_class_method(mod_siren, "sweep_vec",       siren_offset_sweep_vec,       MRB_ARGS_REQ(2));
  rb_define_class_method(mod_siren, "sweep_path",      siren_offset_sweep_path,      MRB_ARGS_REQ(2) | MRB_ARGS_OPT(4));
  rb_define_class_method(mod_siren, "loft",            siren_offset_loft,            MRB_ARGS_REQ(1) | MRB_ARGS_OPT(3));
  rb_define_class_method(mod_siren, "offset_geomsurf", siren_offset_offset_geomsurf, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
  rb_define_class_method(mod_siren, "offset",          siren_offset_offset,          MRB_ARGS_REQ(3) | MRB_ARGS_OPT(5));
  rb_define_class_method(mod_siren, "offset_shape",    siren_offset_offset_shape,    MRB_ARGS_REQ(3) | MRB_ARGS_OPT(4));
  rb_define_class_method(mod_siren, "pipe",            siren_offset_pipe,            MRB_ARGS_REQ(2) | MRB_ARGS_OPT(2));
  // For mix-in
  rb_define_method      (mod_siren, "sweep_vec",       siren_offset_sweep_vec,       MRB_ARGS_REQ(2));
  rb_define_method      (mod_siren, "sweep_path",      siren_offset_sweep_path,      MRB_ARGS_REQ(2) | MRB_ARGS_OPT(4));
  rb_define_method      (mod_siren, "loft",            siren_offset_loft,            MRB_ARGS_REQ(1) | MRB_ARGS_OPT(3));
  rb_define_method      (mod_siren, "offset_geomsurf", siren_offset_offset_geomsurf, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
  rb_define_method      (mod_siren, "offset",          siren_offset_offset,          MRB_ARGS_REQ(3) | MRB_ARGS_OPT(5));
  rb_define_method      (mod_siren, "offset_shape",    siren_offset_offset_shape,    MRB_ARGS_REQ(3) | MRB_ARGS_OPT(4));
  rb_define_method      (mod_siren, "pipe",            siren_offset_pipe,            MRB_ARGS_REQ(2) | MRB_ARGS_OPT(2));
  return true;
}

VALUE siren_offset_sweep_vec( VALUE self)
{
  VALUE target, vec;
  int argc = rb_get_args("oA", &target, &vec);

  TopoDS_Shape* profile = siren_shape_get(target);

  gp_Pnt sp = gp_Pnt(0., 0., 0.).Transformed(profile->Location());
  gp_Pnt tp = siren_ary_to_pnt(vec).Transformed(profile->Location());

  TopoDS_Edge pe = BRepBuilderAPI_MakeEdge(sp, tp);
  TopoDS_Wire path = BRepBuilderAPI_MakeWire(pe);

  BRepOffsetAPI_MakePipe mp(path, *profile);
  mp.Build();

  return siren_shape_new(mp.Shape());
}

VALUE siren_offset_sweep_path( VALUE self)
{
  VALUE target, pathwire;
  rb_bool cont, corr;
  VALUE scale_first, scale_last;
  int argc = rb_get_args("oo|bbff", &target, &pathwire, &cont, &corr, &scale_first, &scale_last);

  TopoDS_Shape* shape_profile = siren_shape_get(target);
  TopoDS_Shape* shape_path = siren_shape_get(pathwire);

  TopoDS_Wire path;

  if (shape_path->ShapeType() == TopAbs_EDGE) {
    path = BRepBuilderAPI_MakeWire(TopoDS::Edge(*shape_path));
  }
  else if (shape_path->ShapeType() == TopAbs_WIRE) {
    path = TopoDS::Wire(*shape_path);
  }
  else {
    rb_raise(E_ARGUMENT_ERROR, "Type of path is not Edge or Wire.");
  }

  VALUE result = Qnil;

  if (argc >= 3 && argc <= 6) {

    Standard_Boolean withContact = (Standard_Boolean)cont;
    Standard_Boolean withCorrection = (Standard_Boolean)corr;

    BRepOffsetAPI_MakePipeShell ps(path);

    // get params
    Standard_Real fparam, lparam;
    {
      BRepAdaptor_CompCurve cc(path);
      fparam = cc.FirstParameter();
      lparam = cc.LastParameter();
    }

    if (argc < 6) {
      scale_last  = 1.0;
      if (argc < 5) {
        scale_first = 1.0;
      }
    }

    //opencascade::handle<Law_Linear> law = new Law_Linear();
    //law->Set(fparam, scale_first, lparam, scale_last);

    opencascade::handle<Law_S> law = new Law_S();
    law->Set(fparam, scale_first, lparam, scale_last);

    //opencascade::handle<Law_Composite> law = new Law_Composite(fparam, lparam, 1.0e-6);

    // get start point
    TopoDS_Vertex pfirst; {
      TopoDS_Vertex plast;
      TopExp::Vertices(path, pfirst, plast);
    }

    ps.SetLaw(
        *shape_profile, // セクションプロファイル
        law,           // 掃引規則
        pfirst,        // 開始点
        withContact,   // translate profile to start point
        withCorrection // Change normal of profile by curveture
        );

    ps.Build();
    result = siren_shape_new(ps.Shape());
  }
  else {
    BRepOffsetAPI_MakePipe mp(path, *shape_profile);
    mp.Build();
    result = siren_shape_new(mp.Shape());
  }
  return result;
}

VALUE siren_offset_loft( VALUE self)
{
  VALUE objs;
  rb_bool smooth, is_solid, is_ruled;
  int argc = rb_get_args("A|bbb", &objs, &smooth,  &is_solid, &is_ruled);
  int lsize = rb_ary_len(objs);

  if (lsize < 2) {
    rb_raise(E_ARGUMENT_ERROR, "Number of shapes must be over 2 wires.");
  }

  Standard_Boolean is_sm, is_s, is_r;
  is_sm = argc < 2 ? Standard_True : (Standard_Boolean)smooth;
  is_s = argc < 3 ? Standard_False : (Standard_Boolean)is_solid;
  is_r = argc < 4 ? Standard_True : (Standard_Boolean)is_ruled;

  BRepOffsetAPI_ThruSections ts(is_s, is_r);

  for (int i=0; i<lsize; i++) {
    VALUE line = rb_ary_ref(objs, i);
    TopoDS_Shape* shape = siren_shape_get(line);
    TopoDS_Wire w = TopoDS::Wire(*shape);
    ts.AddWire(w);
  }

  ts.SetSmoothing(is_sm);
  ts.Build();

  return siren_shape_new(ts.Shape());
}

VALUE siren_offset_offset_geomsurf( VALUE self)
{
  VALUE target;
  VALUE offset, tol;
  int argc = rb_get_args("of|f", &target, &offset, &tol);
  if (argc < 3)
    tol = 1.0;

  TopoDS_Shape* shape = siren_shape_get(target);

  TopoDS_Compound comp;
  BRep_Builder B;
  B.MakeCompound(comp);

  TopExp_Explorer exp(*shape, TopAbs_FACE);

  for (; exp.More(); exp.Next()) {
    TopoDS_Face face = TopoDS::Face(exp.Current());
    opencascade::handle<Geom_Surface> gs = BRep_Tool::Surface(face);
    opencascade::handle<Geom_OffsetSurface> gos = new Geom_OffsetSurface(gs, offset);
    TopoDS_Face newface = BRepBuilderAPI_MakeFace(gos, tol);
    B.Add(comp, newface);
  }

  return siren_shape_new(comp);
}

VALUE siren_offset_offset( VALUE self)
{
  VALUE target;
  VALUE offset, tol;
  rb_int mode = (int)BRepOffset_Skin;
  rb_bool intersect = false, self_intersect = false;
  rb_int join = (int)GeomAbs_Arc;
  rb_bool thickening = false;
  int argc = rb_get_args("off|ibbib", &target, &offset, &tol, &mode,
      &intersect, &self_intersect, &join, &thickening);

  TopoDS_Shape* shape = siren_shape_get(target);

  BRepOffset_MakeOffset api;
  api.Initialize(*shape, offset, tol, (BRepOffset_Mode)mode,
      intersect, self_intersect, (GeomAbs_JoinType)join, thickening);
  api.MakeOffsetShape();
  if (api.IsDone()) {
    return siren_shape_new(api.Shape());
  }
  else {
    switch (api.Error()) {
    default: break;
    }
  }
  return Qnil;
}

VALUE siren_offset_offset_shape( VALUE self)
{
  VALUE target;
  VALUE offset, tol;
  rb_int mode = (int)BRepOffset_Skin;
  rb_bool intersect = false, self_intersect = false;
  rb_int join = (int)GeomAbs_Arc;
  int argc = rb_get_args("off|ibbi", &target, &offset, &tol, &mode,
      &intersect, &self_intersect, &join);

  TopoDS_Shape* shape = siren_shape_get(target);

  TopoDS_Shape result = BRepOffsetAPI_MakeOffsetShape(*shape, offset, tol, (BRepOffset_Mode)mode,
      intersect, self_intersect, (GeomAbs_JoinType)join);

  return siren_shape_new(result);
}

VALUE siren_offset_pipe( VALUE self)
{
  VALUE profile, path;
  rb_int mode;
  rb_bool force_approx_c1;
  int argc = rb_get_args("oo|ib", &profile, &path, &mode, &force_approx_c1);

  TopoDS_Shape* shape_profile = siren_shape_get(profile);
  TopoDS_Shape* shape_path = siren_shape_get(path);

  if (shape_path->ShapeType() != TopAbs_WIRE) {
    rb_raise(E_ARGUMENT_ERROR, "Type of path is not Edge or Wire.");
  }

  TopoDS_Wire wire = TopoDS::Wire(*shape_path);

  if (argc == 2) {
    BRepOffsetAPI_MakePipe mp(wire, *shape_profile);
    mp.Build();
    return siren_shape_new(mp.Shape());
  }
  if (argc == 3) {
    force_approx_c1 = FALSE;
  }

  BRepOffsetAPI_MakePipe mp(wire, *shape_profile, (GeomFill_Trihedron)mode, force_approx_c1);
  mp.Build();
  return siren_shape_new(mp.Shape());
}

