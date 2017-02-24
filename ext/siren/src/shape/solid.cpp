#include "shape/solid.h"

VALUE siren_solid_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Solid")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_solid_type;
  return obj;
}

TopoDS_Solid siren_solid_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_solid_type));
  TopoDS_Solid solid = TopoDS::Solid(*shape);
  if (solid.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Solid."); }
  return solid;
}

bool siren_solid_install()
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_solid = rb_define_class_under(sr_mSiren, "Solid", cls_shape);
  MRB_SET_INSTANCE_TT(cls_solid, MRB_TT_DATA);
  rb_define_method(cls_solid, "initialize", siren_solid_init, MRB_ARGS_NONE());

  auto obj_solid = rb_obj_ptr(siren_solid_obj());
  rb_define_singleton_method(obj_solid, "box",        siren_solid_box,        MRB_ARGS_OPT(2));
  rb_define_singleton_method(obj_solid, "box2p",      siren_solid_box2p,      MRB_ARGS_OPT(2));
  rb_define_singleton_method(obj_solid, "boxax",      siren_solid_boxax,      MRB_ARGS_REQ(3));
  rb_define_singleton_method(obj_solid, "sphere",     siren_solid_sphere,     MRB_ARGS_OPT(2));
  rb_define_singleton_method(obj_solid, "cylinder",   siren_solid_cylinder,   MRB_ARGS_REQ(5));
  rb_define_singleton_method(obj_solid, "cone",       siren_solid_cone,       MRB_ARGS_REQ(6));
  rb_define_singleton_method(obj_solid, "torus",      siren_solid_torus,      MRB_ARGS_REQ(5));
  rb_define_singleton_method(obj_solid, "halfspace",  siren_solid_halfspace,  MRB_ARGS_REQ(2));
  rb_define_singleton_method(obj_solid, "prism",      siren_solid_prism,      MRB_ARGS_NONE());
  rb_define_singleton_method(obj_solid, "revol",      siren_solid_revol,      MRB_ARGS_NONE());
  rb_define_singleton_method(obj_solid, "revolution", siren_solid_revolution, MRB_ARGS_NONE());
  rb_define_singleton_method(obj_solid, "wedge",      siren_solid_wedge,      MRB_ARGS_OPT(7));
  return true;
}

struct RClass* siren_solid_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Solid")));
}

VALUE siren_solid_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Solid"));
}

VALUE siren_solid_init( VALUE self)
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
    rb_raise(E_RUNTIME_ERROR, "Failed to make a Solid.");
  }
  TopoDS_Shape shape = solid_maker.Shape();
  if (shape.IsNull()) {
    rb_raise(E_RUNTIME_ERROR, "Failed to make a Solid.");
  }
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = shape; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(self) = &siren_solid_type;
  return self;
}

VALUE siren_solid_box( VALUE self)
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

VALUE siren_solid_box2p( VALUE self)
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

VALUE siren_solid_boxax( VALUE self)
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

VALUE siren_solid_sphere( VALUE self)
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

VALUE siren_solid_cylinder( VALUE self)
{
  VALUE pos, norm;
  VALUE r, h, a;
  rb_scan_args(argc, argv, "AAfff", &pos, &norm, &r, &h, &a);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeCylinder api(ax, r, h, a);

  return siren_shape_new(api.Shape());
}

VALUE siren_solid_cone( VALUE self)
{
  VALUE pos, norm;
  VALUE r1, r2, h, ang;
  rb_scan_args(argc, argv, "AAffff", &pos, &norm, &r1, &r2, &h, &ang);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeCone api(ax, r1, r2, h, ang);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_torus( VALUE self)
{
  VALUE r1, r2, ang;
  VALUE pos, norm;
  rb_scan_args(argc, argv, "AAfff", &pos, &norm, &r1, &r2, &ang);

  gp_Ax2 ax = siren_ary_to_ax2(pos, norm);

  BRepPrimAPI_MakeTorus api(ax, r1, r2, ang);
  return siren_shape_new(api.Shape());
}

VALUE siren_solid_halfspace( VALUE self)
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

VALUE siren_solid_prism( VALUE self)
{
  rb_raise(E_NOTIMP_ERROR, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_revol( VALUE self)
{
  rb_raise(E_NOTIMP_ERROR, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_revolution( VALUE self)
{
  rb_raise(E_NOTIMP_ERROR, "Not implemented.");
  return Qnil;
}

VALUE siren_solid_wedge( VALUE self)
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

