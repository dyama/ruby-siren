#include "topalgo.h"

bool siren_topalgo_install()
{
  // Class method
#if 0
  rb_define_class_method(sr_mSiren, "copy",       siren_topalgo_copy,        -1);
  rb_define_class_method(sr_mSiren, "line",       siren_topalgo_line,        -1);
  rb_define_class_method(sr_mSiren, "infline",    siren_topalgo_infline,     -1);
  rb_define_class_method(sr_mSiren, "polyline",   siren_topalgo_polyline,    -1);
  rb_define_class_method(sr_mSiren, "interpolate",siren_topalgo_interpolate, -1);
  rb_define_class_method(sr_mSiren, "arc",        siren_topalgo_arc,         -1);
  rb_define_class_method(sr_mSiren, "arc3p",      siren_topalgo_arc3p,       -1);
  rb_define_class_method(sr_mSiren, "circle",     siren_topalgo_circle,      -1);
  rb_define_class_method(sr_mSiren, "circle3p",   siren_topalgo_circle3p,    -1);
#endif
  // For mix-in
  rb_define_method(sr_mSiren, "copy",        RUBY_METHOD_FUNC(siren_topalgo_copy),        -1);
  rb_define_method(sr_mSiren, "line",        RUBY_METHOD_FUNC(siren_topalgo_line),        -1);
  rb_define_method(sr_mSiren, "infline",     RUBY_METHOD_FUNC(siren_topalgo_infline),     -1);
  rb_define_method(sr_mSiren, "polyline",    RUBY_METHOD_FUNC(siren_topalgo_polyline),    -1);
  rb_define_method(sr_mSiren, "interpolate", RUBY_METHOD_FUNC(siren_topalgo_interpolate), -1);
  rb_define_method(sr_mSiren, "arc",         RUBY_METHOD_FUNC(siren_topalgo_arc),         -1);
  rb_define_method(sr_mSiren, "arc3p",       RUBY_METHOD_FUNC(siren_topalgo_arc3p),       -1);
  rb_define_method(sr_mSiren, "circle",      RUBY_METHOD_FUNC(siren_topalgo_circle),      -1);
  rb_define_method(sr_mSiren, "circle3p",    RUBY_METHOD_FUNC(siren_topalgo_circle3p),    -1);
  rb_define_method(sr_cShape, "cog",         RUBY_METHOD_FUNC(siren_topalgo_cog),         -1);
  rb_define_method(sr_cShape, "area",        RUBY_METHOD_FUNC(siren_topalgo_area),        -1);
  rb_define_method(sr_cShape, "volume",      RUBY_METHOD_FUNC(siren_topalgo_volume),      -1);

  return true;
}

VALUE siren_topalgo_copy(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  VALUE copy_geom = Standard_True;

  rb_scan_args(argc, argv, "11", &target, &copy_geom);

  TopoDS_Shape* src = siren_shape_get(target);
  TopoDS_Shape res = BRepBuilderAPI_Copy(*src, (Standard_Boolean)copy_geom);
  return siren_shape_new(res);
}

VALUE siren_topalgo_line(int argc, VALUE* argv, VALUE self)
{
  VALUE sp, tp;
  rb_scan_args(argc, argv, "02", &sp, &tp);
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
    rb_raise(Qnil,
        "Specified terminal point same as the start point.");
  }
  TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(S, T);
  return siren_shape_new(shape);
}

VALUE siren_topalgo_infline(int argc, VALUE* argv, VALUE self)
{
  VALUE orig, dir;
  rb_scan_args(argc, argv, "|AA", &orig, &dir);
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

VALUE siren_topalgo_polyline(int argc, VALUE* argv, VALUE self)
{
  VALUE ary;
  rb_scan_args(argc, argv, "A", &ary);
  BRepBuilderAPI_MakePolygon poly;
  for (int i = 0; i < RARRAY_LEN(ary); i++) {
    poly.Add(siren_ary_to_pnt(RARRAY_AREF(ary, i)));
  }
  TopoDS_Shape shape = poly.Wire();
  return siren_shape_new(shape);
}

VALUE siren_topalgo_interpolate(int argc, VALUE* argv, VALUE self)
{
  VALUE pts, vecs;
  rb_scan_args(argc, argv, "A|A", &pts, &vecs);

  int psize = RARRAY_LEN(pts);
  opencascade::handle<TColgp_HArray1OfPnt> pary = new TColgp_HArray1OfPnt(1, psize);
  for (int i=0; i<psize; i++) {
    pary->SetValue(i+1, siren_ary_to_pnt(RARRAY_AREF(pts, i)));
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
        VALUE avec = RARRAY_AREF(vecs, i);
        if (NIL_P(avec)) {
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
      rb_raise(Qnil, "Failed to make a curve.");
    }

    opencascade::handle<Geom_BSplineCurve> geSpl = intp.Curve();
    if (geSpl.IsNull()) {
      rb_raise(Qnil, "Failed to make a curve.");
    }

    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(geSpl);
    return siren_shape_new(shape);
  }
  catch (...) {
    rb_raise(Qnil, "Failed to make a curve. Incorrect points specified.");
  }
  return Qnil;
}

VALUE siren_topalgo_arc(int argc, VALUE* argv, VALUE self)
{
  VALUE orig, dir, vx;
  VALUE r, start_ang, term_ang;
  rb_scan_args(argc, argv, "AAAfff", &orig, &dir, &vx, &r, &start_ang, &term_ang);
  gp_Circ circle = gp_Circ(gp_Ax2(siren_ary_to_pnt(orig), siren_ary_to_dir(dir), siren_ary_to_dir(vx)), r);
  opencascade::handle<Geom_TrimmedCurve> gc = GC_MakeArcOfCircle(circle, start_ang, term_ang, Standard_True);
  if (gc.IsNull()) {
    rb_raise(Qnil, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_arc3p(int argc, VALUE* argv, VALUE self)
{
  VALUE p1, p2, p3;
  rb_scan_args(argc, argv, "AAA", &p1, &p2, &p3);
  opencascade::handle<Geom_TrimmedCurve> gc = GC_MakeArcOfCircle(
      siren_ary_to_pnt(p1),
      siren_ary_to_pnt(p2),
      siren_ary_to_pnt(p3));
  if (gc.IsNull()) {
    rb_raise(Qnil, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_circle(int argc, VALUE* argv, VALUE self)
{
  VALUE orig, dir;
  VALUE r;
  rb_scan_args(argc, argv, "AAf", &orig, &dir, &r);
  opencascade::handle<Geom_Circle> gc = GC_MakeCircle(
      siren_ary_to_pnt(orig),
      siren_ary_to_dir(dir),
      r);
  if (gc.IsNull()) {
    rb_raise(Qnil, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_circle3p(int argc, VALUE* argv, VALUE self)
{
  VALUE p1, p2, p3;
  rb_scan_args(argc, argv, "AAA", &p1, &p2, &p3);
  opencascade::handle<Geom_Circle> gc = GC_MakeCircle(
      siren_ary_to_pnt(p1),
      siren_ary_to_pnt(p2),
      siren_ary_to_pnt(p3));
  if (gc.IsNull()) {
    rb_raise(Qnil, "Failed to make a curve.");
    return Qnil;
  }
  else {
    TopoDS_Edge E = BRepBuilderAPI_MakeEdge(gc);
    return siren_shape_new(E);
  }
}

VALUE siren_topalgo_volume(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(*shape, gprops);
  Standard_Real vol = gprops.Mass();
  return (vol);
}

VALUE siren_topalgo_cog(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(*shape, gprops);
  gp_Pnt cog = gprops.CentreOfMass();
  return siren_pnt_to_ary(cog);
}

VALUE siren_topalgo_area(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  GProp_GProps gprops;
  BRepGProp::SurfaceProperties(*shape, gprops);
  Standard_Real area = gprops.Mass();
  return (area);
}

