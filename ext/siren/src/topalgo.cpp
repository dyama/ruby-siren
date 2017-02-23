#include "topalgo.h"

bool siren_topalgo_install( struct RClass* mod_siren)
{
  // Class method
  rb_define_class_method(mod_siren, "copy",       siren_topalgo_copy,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_class_method(mod_siren, "line",       siren_topalgo_line,       MRB_ARGS_OPT(2));
  rb_define_class_method(mod_siren, "infline",    siren_topalgo_infline,    MRB_ARGS_OPT(2));
  rb_define_class_method(mod_siren, "polyline",   siren_topalgo_polyline,   MRB_ARGS_REQ(1));
  rb_define_class_method(mod_siren, "interpolate",siren_topalgo_interpolate,MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_class_method(mod_siren, "arc",        siren_topalgo_arc,        MRB_ARGS_REQ(6));
  rb_define_class_method(mod_siren, "arc3p",      siren_topalgo_arc3p,      MRB_ARGS_REQ(3));
  rb_define_class_method(mod_siren, "circle",     siren_topalgo_circle,     MRB_ARGS_REQ(3));
  rb_define_class_method(mod_siren, "circle3p",   siren_topalgo_circle3p,   MRB_ARGS_REQ(3));
  // For mix-in
  rb_define_method      (mod_siren, "copy",       siren_topalgo_copy,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method      (mod_siren, "line",       siren_topalgo_line,       MRB_ARGS_REQ(2));
  rb_define_method      (mod_siren, "infline",    siren_topalgo_infline,    MRB_ARGS_REQ(2));
  rb_define_method      (mod_siren, "polyline",   siren_topalgo_polyline,   MRB_ARGS_REQ(1));
  rb_define_method      (mod_siren, "interpolate",siren_topalgo_interpolate,MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method      (mod_siren, "arc",        siren_topalgo_arc,        MRB_ARGS_REQ(6));
  rb_define_method      (mod_siren, "arc3p",      siren_topalgo_arc3p,      MRB_ARGS_REQ(3));
  rb_define_method      (mod_siren, "circle",     siren_topalgo_circle,     MRB_ARGS_REQ(3));
  rb_define_method      (mod_siren, "circle3p",   siren_topalgo_circle3p,   MRB_ARGS_REQ(3));

  struct RClass* cls_shape = siren_shape_rclass();
  rb_define_method(cls_shape, "cog",    siren_topalgo_cog,    MRB_ARGS_NONE());
  rb_define_method(cls_shape, "area",   siren_topalgo_area,   MRB_ARGS_NONE());
  rb_define_method(cls_shape, "volume", siren_topalgo_volume, MRB_ARGS_NONE());
  return true;
}

VALUE siren_topalgo_copy( VALUE self)
{
  VALUE target;
  VALUE copy_geom = (_bool)Standard_True;
  int argc = rb_get_args("o|b", &target, &copy_geom);
  TopoDS_Shape* src = siren_shape_get(target);
  TopoDS_Shape res = BRepBuilderAPI_Copy(*src, (Standard_Boolean)copy_geom);
  return siren_shape_new(res);
}

VALUE siren_topalgo_line( VALUE self)
{
  VALUE sp, tp;
  int argc = rb_get_args("|AA", &sp, &tp);
  gp_Pnt S(0., 0., 0.);
  gp_Pnt T(1., 1., 1.);
  if (argc > 0) {
    S = siren_ary_to_pnt(sp);
  }
  if (argc > 1) {
    T = siren_ary_to_pnt(tp);
  }
  Standard_Real linear_tolerance = 1.0e-7;
  if (S.IsEqual(T, linear_tolerance)) {
    rb_raise(E_ARGUMENT_ERROR,
        "Specified terminal point same as the start point.");
  }
  TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(S, T);
  return siren_shape_new(shape);
}

VALUE siren_topalgo_infline( VALUE self)
{
  VALUE orig, dir;
  int argc = rb_get_args("|AA", &orig, &dir);
  gp_Pnt p(0., 0., 0.);
  gp_Dir d(1., 0., 0.);
  if (argc > 0) {
    p = siren_ary_to_pnt(orig);
  }
  if (argc > 1) {
    d = siren_ary_to_dir(dir);
  }
  TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(gp_Lin(p, d));
  return siren_shape_new(shape);
}

VALUE siren_topalgo_polyline( VALUE self)
{
  VALUE ary;
  int argc = rb_get_args("A", &ary);
  BRepBuilderAPI_MakePolygon poly;
  for (int i = 0; i < rb_ary_len(ary); i++) {
    poly.Add(siren_ary_to_pnt(rb_ary_ref(ary, i)));
  }
  TopoDS_Shape shape = poly.Wire();
  return siren_shape_new(shape);
}

VALUE siren_topalgo_interpolate( VALUE self)
{
  VALUE pts, vecs;
  int argc = rb_get_args("A|A", &pts, &vecs);

  int psize = rb_ary_len(pts);
  opencascade::handle<TColgp_HArray1OfPnt> pary = new TColgp_HArray1OfPnt(1, psize);
  for (int i=0; i<psize; i++) {
    pary->SetValue(i+1, siren_ary_to_pnt(rb_ary_ref(pts, i)));
  }

  try {
    Standard_Real tolerance = 1.0e-7;
    Standard_Boolean periodic_flag = Standard_False;

    // Raise exception when got pary has same points.
    GeomAPI_Interpolate intp(pary, periodic_flag, tolerance);

    if (argc == 2) {
      TColgp_Array1OfVec vec(1, psize);
      opencascade::handle<TColStd_HArray1OfBoolean> use = new TColStd_HArray1OfBoolean(1, psize);

      for (int i=0; i<psize; i++) {
        VALUE avec = rb_ary_ref(vecs, i);
        if (_nil_p(avec)) {
          use->SetValue(i+1, Standard_False);
        }
        else {
          vec.SetValue(i+1, siren_ary_to_vec(avec));
          use->SetValue(i+1, Standard_True);
        }
      }
      intp.Load(vec, use, Standard_True);
    }

    intp.Perform();
    if (!intp.IsDone()) {
      rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    }

    opencascade::handle<Geom_BSplineCurve> geSpl = intp.Curve();
    if (geSpl.IsNull()) {
      rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    }

    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(geSpl);
    return siren_shape_new(shape);
  }
  catch (...) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve. Incorrect points specified.");
  }
  return Qnil;
}

VALUE siren_topalgo_arc( VALUE self)
{
  VALUE orig, dir, vx;
  VALUE r, start_ang, term_ang;
  int argc = rb_get_args("AAAfff", &orig, &dir, &vx, &r, &start_ang, &term_ang);
  gp_Circ circle = gp_Circ(gp_Ax2(siren_ary_to_pnt(orig), siren_ary_to_dir(dir), siren_ary_to_dir(vx)), r);
  opencascade::handle<Geom_TrimmedCurve> gc = GC_MakeArcOfCircle(circle, start_ang, term_ang, Standard_True);
  if (gc.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_arc3p( VALUE self)
{
  VALUE p1, p2, p3;
  int argc = rb_get_args("AAA", &p1, &p2, &p3);
  opencascade::handle<Geom_TrimmedCurve> gc = GC_MakeArcOfCircle(
      siren_ary_to_pnt(p1),
      siren_ary_to_pnt(p2),
      siren_ary_to_pnt(p3));
  if (gc.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_circle( VALUE self)
{
  VALUE orig, dir;
  VALUE r;
  int argc = rb_get_args("AAf", &orig, &dir, &r);
  opencascade::handle<Geom_Circle> gc = GC_MakeCircle(
      siren_ary_to_pnt(orig),
      siren_ary_to_dir(dir),
      r);
  if (gc.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_circle3p( VALUE self)
{
  VALUE p1, p2, p3;
  int argc = rb_get_args("AAA", &p1, &p2, &p3);
  opencascade::handle<Geom_Circle> gc = GC_MakeCircle(
      siren_ary_to_pnt(p1),
      siren_ary_to_pnt(p2),
      siren_ary_to_pnt(p3));
  if (gc.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_volume( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(*shape, gprops);
  Standard_Real vol = gprops.Mass();
  return ((_float)vol);
}

VALUE siren_topalgo_cog( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(*shape, gprops);
  gp_Pnt cog = gprops.CentreOfMass();
  return siren_pnt_to_ary(cog);
}

VALUE siren_topalgo_area( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::SurfaceProperties(*shape, gprops);
  Standard_Real area = gprops.Mass();
  return (area);
}

