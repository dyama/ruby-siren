#include "shape/solid.h"

VALUE sr_cSolid;

VALUE siren_solid_new( const TopoDS_Shape* src)
{
  VALUE obj = rb_instance_alloc(sr_cSolid);
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(obj) = &siren_solid_type;
  return obj;
}

TopoDS_Solid siren_solid_get(VALUE self)
{
#if 0
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_solid_type));
  TopoDS_Solid solid = TopoDS::Solid(*shape);
  if (solid.IsNull()) { rb_raise(Qnil, "The geometry type is not Solid."); }
  return solid;
#endif
}

bool siren_solid_install()
{
#if 0
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_solid = rb_define_class_under(sr_mSiren, "Solid", cls_shape);
  MRB_SET_INSTANCE_TT(cls_solid, MRB_TT_DATA);
#endif
  rb_define_method(sr_cSolid, "initialize", RUBY_METHOD_FUNC(siren_solid_init), -1);
  rb_define_singleton_method(sr_cSolid, "box",        RUBY_METHOD_FUNC(siren_solid_box),        -1);
  rb_define_singleton_method(sr_cSolid, "box2p",      RUBY_METHOD_FUNC(siren_solid_box2p),      -1);
  rb_define_singleton_method(sr_cSolid, "boxax",      RUBY_METHOD_FUNC(siren_solid_boxax),      -1);
  rb_define_singleton_method(sr_cSolid, "sphere",     RUBY_METHOD_FUNC(siren_solid_sphere),     -1);
  rb_define_singleton_method(sr_cSolid, "cylinder",   RUBY_METHOD_FUNC(siren_solid_cylinder),   -1);
  rb_define_singleton_method(sr_cSolid, "cone",       RUBY_METHOD_FUNC(siren_solid_cone),       -1);
  rb_define_singleton_method(sr_cSolid, "torus",      RUBY_METHOD_FUNC(siren_solid_torus),      -1);
  rb_define_singleton_method(sr_cSolid, "halfspace",  RUBY_METHOD_FUNC(siren_solid_halfspace),  -1);
  rb_define_singleton_method(sr_cSolid, "prism",      RUBY_METHOD_FUNC(siren_solid_prism),      -1);
  rb_define_singleton_method(sr_cSolid, "revol",      RUBY_METHOD_FUNC(siren_solid_revol),      -1);
  rb_define_singleton_method(sr_cSolid, "revolution", RUBY_METHOD_FUNC(siren_solid_revolution), -1);
  rb_define_singleton_method(sr_cSolid, "wedge",      RUBY_METHOD_FUNC(siren_solid_wedge),      -1);
  return true;
}

VALUE siren_solid_init(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);
  if (len == 0) {
    rb_raise(Qnil, "No shapes specified.");
  }
  BRepBuilderAPI_MakeSolid solid_maker;
  for (int i = 0; i < len; i++) {
    TopoDS_Shell shell = siren_shell_get(a[i]);
    solid_maker.Add(shell);
  }
  if (!solid_maker.IsDone()) {
    rb_raise(Qnil, "Failed to make a Solid.");
  }
  TopoDS_Shape shape = solid_maker.Shape();
  if (shape.IsNull()) {
    rb_raise(Qnil, "Failed to make a Solid.");
  }
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = shape; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(self) = &siren_solid_type;
  return self;
}

VALUE siren_solid_box(int argc, VALUE* argv, VALUE self)
{
  VALUE size, pos;
  rb_scan_args(argc, argv, "|AA", &size, &pos);

  Standard_Real sx, sy, sz;
  if (argc >= 1) {
    siren_ary_to_xyz(size, sx, sy, sz);
  }
  else {
    sx = 1.0; sy = 1.0; sz = 1.0;
  }

  gp_Pnt op;
  if (argc >= 2) {
    Standard_Real px, py, pz;
    siren_ary_to_xyz(pos, px, py, pz);
    op = gp_Pnt(px, py, pz);
  }
  else {
    op = gp_Pnt(0., 0., 0.);
  }
  if (std::fabs(sx * sy * sz) == 0.0) {
    rb_raise(Qnil,
      "Failed to make solid. Incorrect size specified.");
  }
  BRepPrimAPI_MakeBox api(op, sx, sy, sz);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_box2p(int argc, VALUE* argv, VALUE self)
{
  VALUE p1, p2;
  rb_scan_args(argc, argv, "|AA", &p1, &p2);

  Standard_Real x1 = 0.0, y1 = 0.0, z1 = 0.0;
  Standard_Real x2 = 1.0, y2 = 1.0, z2 = 1.0;

  if (argc >= 1) {
    siren_ary_to_xyz(p1, x1, y1, z1);
  }
  if (argc >= 2) {
    siren_ary_to_xyz(p2, x2, y2, z2);
  }

  gp_Pnt point1(x1, x1, x1);
  gp_Pnt point2(x2, x2, x2);

  BRepPrimAPI_MakeBox api(point1, point2);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_boxax(int argc, VALUE* argv, VALUE self)
{
  VALUE size, pos, dir;
  rb_scan_args(argc, argv, "AAA", &size, &pos, &dir);

  Standard_Real sx, sy, sz;
  siren_ary_to_xyz(size, sx, sy, sz);

  Standard_Real px, py, pz;
  siren_ary_to_xyz(pos, px, py, pz);

  Standard_Real dx, dy, dz;
  siren_ary_to_xyz(dir, dx, dy, dz);

  gp_Ax2 ax(gp_Pnt(px, py, pz), gp_Dir(dx, dy, dz));

  BRepPrimAPI_MakeBox api(ax, sx, sy, sz);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_sphere(int argc, VALUE* argv, VALUE self)
{
  VALUE r = 1.0;
  VALUE pos;
  rb_scan_args(argc, argv, "|fA", &r, &pos);

  gp_Pnt op;
  if (argc == 2) {
    Standard_Real px, py, pz;
    siren_ary_to_xyz(pos, px, py, pz);
    op = gp_Pnt(px, py, pz);
  }
  else {
    op = gp_Pnt(0., 0., 0.);
  }

  if (r < 0) {
    rb_raise(Qnil, "Failed to make solid."
      " Specified radius value below 0.");
  }

  BRepPrimAPI_MakeSphere api(op, r);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_cylinder(int argc, VALUE* argv, VALUE self)
{
  VALUE pos, norm;
  VALUE r, h, a;
  rb_scan_args(argc, argv, "AAfff", &pos, &norm, &r, &h, &a);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeCylinder api(ax, r, h, a);

  return siren_shape_new(api.Shape());
}

VALUE siren_solid_cone(int argc, VALUE* argv, VALUE self)
{
  VALUE pos, norm;
  VALUE r1, r2, h, ang;
  rb_scan_args(argc, argv, "AAffff", &pos, &norm, &r1, &r2, &h, &ang);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeCone api(ax, r1, r2, h, ang);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_torus(int argc, VALUE* argv, VALUE self)
{
  VALUE r1, r2, ang;
  VALUE pos, norm;
  rb_scan_args(argc, argv, "AAfff", &pos, &norm, &r1, &r2, &ang);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeTorus api(ax, r1, r2, ang);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_halfspace(int argc, VALUE* argv, VALUE self)
{
  VALUE surf, refpnt;
  rb_scan_args(argc, argv, "oA", &surf, &refpnt);
  TopoDS_Shape* shape = siren_shape_get(surf);
  if (shape == nullptr || shape->IsNull()) {
    rb_raise(Qnil, "Specified shape is incorrect.");
  }
  TopoDS_Solid solid;
  gp_Pnt pnt = siren_ary_to_pnt(refpnt);
  if (shape->ShapeType() == TopAbs_FACE) {
    solid = BRepPrimAPI_MakeHalfSpace(TopoDS::Face(*shape), pnt);
  }
  else if (shape->ShapeType() == TopAbs_SHELL) {
    solid = BRepPrimAPI_MakeHalfSpace(TopoDS::Shell(*shape), pnt);
  }
  else {
    rb_raise(Qnil, "Specified shape type is not FACE or SHELL.");
  }
  return siren_shape_new(solid);
}

VALUE siren_solid_prism(int argc, VALUE* argv, VALUE self)
{
  rb_raise(Qnil, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_revol(int argc, VALUE* argv, VALUE self)
{
  rb_raise(Qnil, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_revolution(int argc, VALUE* argv, VALUE self)
{
  rb_raise(Qnil, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_wedge(int argc, VALUE* argv, VALUE self)
{
  VALUE dx = 1.0, dy = 1.0, dz = 1.0, x = 0.5, z = 0.5, X = 0.5, Z = 0.5;
  rb_scan_args(argc, argv, "|fffffff", &dx, &dy, &dz, &x, &z, &X, &Z);
  try {
    BRepPrimAPI_MakeWedge api(dx, dy, dz, x, z, X, Z);
    TopoDS_Shape s = api.Shape();
    return siren_shape_new(s);
  }
  catch (...) {
    rb_raise(Qnil, "Failed to make a wedge.");
  }
  return Qnil;
}