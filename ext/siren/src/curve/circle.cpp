#include "curve.h"

VALUE sr_cCircle;

VALUE siren_circle_new(const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cCircle);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_circle_type;
#endif
  return obj;
}

handle<Geom_Circle> siren_circle_get(VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_circle_type));
  if (hgc.IsNull()) {
    rb_raisef(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Circle> circle = handle<Geom_Circle>::DownCast(hgc);
  if (circle.IsNull()) {
    rb_raisef(Qnil, "The geometry type is not Circle.");
  }
  return circle;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Circle> circle = handle<Geom_Circle>::DownCast(hgc);
  if (circle.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Circle.");
  }
  return circle;
#endif
}

bool siren_circle_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_circle = rb_define_class_under(sr_mSiren, "Circle", rb_cObject);
  MRB_SET_INSTANCE_TT(cls_circle, MRB_TT_DATA);
#endif

  rb_define_method(sr_cCircle, "initialize", siren_curve_init,        -1);
  rb_define_method(sr_cCircle, "radius",     siren_circle_radius,     -1);
  rb_define_method(sr_cCircle, "radius=",    siren_circle_radius_set, -1);
  rb_define_method(sr_cCircle, "center",     siren_circle_center,     -1);
  rb_define_method(sr_cCircle, "center=",    siren_circle_center_set, -1);
  rb_define_method(sr_cCircle, "area",       siren_circle_area,       -1);
  rb_define_method(sr_cCircle, "length",     siren_circle_length,     -1);
  rb_define_method(sr_cCircle, "normal",     siren_circle_normal,     -1);
  rb_define_method(sr_cCircle, "normal=",    siren_circle_normal_set, -1);
  rb_define_method(sr_cCircle, "dir",        siren_circle_dir,        -1);
  rb_define_method(sr_cCircle, "dir=",       siren_circle_dir_set,    -1);
  rb_define_method(sr_cCircle, "dist",       siren_circle_dist,       -1);
  rb_define_method(sr_cCircle, "distdist",   siren_circle_distdist,   -1);
  rb_define_method(sr_cCircle, "contain",    siren_circle_contain,    -1);
  return true;
}

VALUE siren_circle_radius(int argc, VALUE* argv, VALUE self)
{
  Standard_Real r = siren_circle_get(self)->Radius();
  return (r);
}

VALUE siren_circle_radius_set(int argc, VALUE* argv, VALUE self)
{
  VALUE r;
  rb_scan_args(argc, argv, "1", &r);
  siren_circle_get(self)->SetRadius(r);
  return Qnil;
}

VALUE siren_circle_center(int argc, VALUE* argv, VALUE self)
{
  gp_Pnt center = siren_circle_get(self)->Circ().Location();
  return siren_pnt_to_ary(center);
}

VALUE siren_circle_center_set(int argc, VALUE* argv, VALUE self)
{
  VALUE pos;
  rb_scan_args(argc, argv, "1", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  circ.SetLocation(p);
  circle->SetCirc(circ);
  return pos;
}

VALUE siren_circle_area(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  return (circle->Circ().Area());
}

VALUE siren_circle_length(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  return (circle->Circ().Length());
}

VALUE siren_circle_normal(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  // Returns the main axis of the circle.
  // It is the axis perpendicular to the plane of the circle,
  // passing through the "Location" point (center) of the circle.
  gp_Ax1 axis = circle->Circ().Axis();
  return siren_dir_to_ary(axis.Direction());
}

VALUE siren_circle_normal_set(int argc, VALUE* argv, VALUE self)
{
  VALUE norm;
  rb_scan_args(argc, argv, "1", &norm);
  gp_Dir dir = siren_ary_to_dir(norm);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  gp_Ax1 axis = circ.Axis();
  axis.SetDirection(dir);
  circ.SetAxis(axis);
  circle->SetCirc(circ);
  return norm;
}

VALUE siren_circle_dir(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Ax1 axis = circle->Circ().XAxis();
  return siren_dir_to_ary(axis.Direction());
}

VALUE siren_circle_dir_set(int argc, VALUE* argv, VALUE self)
{
  VALUE val;
  rb_scan_args(argc, argv, "1", &val);
  gp_Dir dir = siren_ary_to_dir(val);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  gp_Ax2 axis;
  axis.SetAxis(circ.Axis());
  axis.SetXDirection(dir);
  circ.SetPosition(axis);
  circle->SetCirc(circ);
  return val;
}

VALUE siren_circle_dist(int argc, VALUE* argv, VALUE self)
{
  VALUE pos;
  rb_scan_args(argc, argv, "1", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return (circ.Distance(p));
}

VALUE siren_circle_distdist(int argc, VALUE* argv, VALUE self)
{
  VALUE pos;
  rb_scan_args(argc, argv, "1", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return (circ.SquareDistance(p));
}

VALUE siren_circle_contain(int argc, VALUE* argv, VALUE self)
{
  VALUE pos;
  VALUE lintol = 1.0e-7;
  rb_scan_args(argc, argv, "11", &pos, &lintol);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return circ.Contains(p, lintol) ? Qtrue : Qfalse;
}
