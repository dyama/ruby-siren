#include "curve.h"

VALUE siren_circle_new( const handle<Geom_Curve>* curve)
{
  struct RClass* mod_siren = rb_module_get("Siren");
  VALUE obj;
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(mod_siren), VALUEern_lit("Circle")));
  void* p = rb_malloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
  DATA_TYPE(obj) = &siren_circle_type;
  return obj;
}

handle<Geom_Circle> siren_circle_get( VALUE self)
{
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_circle_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Circle> circle = handle<Geom_Circle>::DownCast(hgc);
  if (circle.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Circle."); }
  return circle;
}

bool siren_circle_install( struct RClass* mod_siren)
{
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_circle = rb_define_class_under(mod_siren, "Circle", rb_cObject);
  MRB_SET_INSTANCE_TT(cls_circle, MRB_TT_DATA);
  rb_define_method(cls_circle, "initialize", siren_curve_init, MRB_ARGS_NONE());

  rb_define_method(cls_circle, "radius",   siren_circle_radius,     MRB_ARGS_NONE());
  rb_define_method(cls_circle, "radius=",  siren_circle_radius_set, MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "center",   siren_circle_center,     MRB_ARGS_NONE());
  rb_define_method(cls_circle, "center=",  siren_circle_center_set, MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "area",     siren_circle_area,       MRB_ARGS_NONE());
  rb_define_method(cls_circle, "length",   siren_circle_length,     MRB_ARGS_NONE());
  rb_define_method(cls_circle, "normal",   siren_circle_normal,     MRB_ARGS_NONE());
  rb_define_method(cls_circle, "normal=",  siren_circle_normal_set, MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "dir",      siren_circle_dir,        MRB_ARGS_NONE());
  rb_define_method(cls_circle, "dir=",     siren_circle_dir_set,    MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "dist",     siren_circle_dist,       MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "distdist", siren_circle_distdist,   MRB_ARGS_REQ(1));
  rb_define_method(cls_circle, "contain",  siren_circle_contain,    MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  return true;
}

VALUE siren_circle_radius( VALUE self)
{
  Standard_Real r = siren_circle_get(self)->Radius();
  return (r);
}

VALUE siren_circle_radius_set( VALUE self)
{
  VALUE r;
  int argc = rb_get_args("f", &r);
  siren_circle_get(self)->SetRadius(r);
  return Qnil;
}

VALUE siren_circle_center( VALUE self)
{
  gp_Pnt center = siren_circle_get(self)->Circ().Location();
  return siren_pnt_to_ary(center);
}

VALUE siren_circle_center_set( VALUE self)
{
  VALUE pos;
  int argc = rb_get_args("A", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  circ.SetLocation(p);
  circle->SetCirc(circ);
  return pos;
}

VALUE siren_circle_area( VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  return (circle->Circ().Area());
}

VALUE siren_circle_length( VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  return (circle->Circ().Length());
}

VALUE siren_circle_normal( VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  // Returns the main axis of the circle.
  // It is the axis perpendicular to the plane of the circle,
  // passing through the "Location" point (center) of the circle.
  gp_Ax1 axis = circle->Circ().Axis();
  return siren_dir_to_ary(axis.Direction());
}

VALUE siren_circle_normal_set( VALUE self)
{
  VALUE norm;
  int argc = rb_get_args("A", &norm);
  gp_Dir dir = siren_ary_to_dir(norm);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  gp_Ax1 axis = circ.Axis();
  axis.SetDirection(dir);
  circ.SetAxis(axis);
  circle->SetCirc(circ);
  return norm;
}

VALUE siren_circle_dir( VALUE self)
{
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Ax1 axis = circle->Circ().XAxis();
  return siren_dir_to_ary(axis.Direction());
}

VALUE siren_circle_dir_set( VALUE self)
{
  VALUE val;
  int argc = rb_get_args("A", &val);
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

VALUE siren_circle_dist( VALUE self)
{
  VALUE pos;
  int argc = rb_get_args("A", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return (circ.Distance(p));
}

VALUE siren_circle_distdist( VALUE self)
{
  VALUE pos;
  int argc = rb_get_args("A", &pos);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return (circ.SquareDistance(p));
}

VALUE siren_circle_contain( VALUE self)
{
  VALUE pos;
  VALUE lintol = 1.0e-7;
  int argc = rb_get_args("A|f", &pos, &lintol);
  gp_Pnt p = siren_ary_to_pnt(pos);
  handle<Geom_Circle> circle = siren_circle_get(self);
  gp_Circ circ = circle->Circ();
  return circ.Contains(p, lintol) ? Qtrue : Qfalse;
}
