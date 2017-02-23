#include "vec.h"

gp_Vec* siren_vec_get( VALUE obj)
{
  return static_cast<gp_Vec*>(_get_datatype(obj, &siren_vec_type));
}

struct RClass* siren_vec_rclass()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(mod_siren), rb_intern_lit("Vec")));
}

VALUE siren_vec_new( double x, double y, double z)
{
  VALUE arg = rb_ary_new();
  rb_ary_push(arg, (x));
  rb_ary_push(arg, (y));
  rb_ary_push(arg, (z));
  return rb_class_new_instance(1, &arg, siren_vec_rclass());
}

VALUE siren_vec_new( const gp_Vec& vec)
{
  return siren_vec_new(vec.X(), vec.Y(), vec.Z());
}

bool siren_vec_install( struct RClass* mod_siren)
{
  struct RClass* cls_vec = rb_define_class_under(mod_siren, "Vec", mrb->object_class);
  MRB_SET_INSTANCE_TT(cls_vec, MRB_TT_DATA);
  rb_define_method(cls_vec, "initialize",       siren_vec_init,             MRB_ARGS_NONE() | MRB_ARGS_OPT(1));
  rb_define_method(cls_vec, "x",                siren_vec_x,                MRB_ARGS_NONE());
  rb_define_method(cls_vec, "x=",               siren_vec_x_set,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "y",                siren_vec_y,                MRB_ARGS_NONE());
  rb_define_method(cls_vec, "y=",               siren_vec_y_set,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "z",                siren_vec_z,                MRB_ARGS_NONE());
  rb_define_method(cls_vec, "z=",               siren_vec_z_set,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "equal?",           siren_vec_is_equal,         MRB_ARGS_REQ(3));
  rb_define_method(cls_vec, "parallel?",        siren_vec_is_parallel,      MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "normal?",          siren_vec_is_normal,        MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "normal",           siren_vec_normal,           MRB_ARGS_NONE());
  rb_define_method(cls_vec, "normal!",          siren_vec_normal_bang,      MRB_ARGS_NONE());
  rb_define_method(cls_vec, "reverse?",         siren_vec_is_reverse,       MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "reverse",          siren_vec_reverse,          MRB_ARGS_NONE());
  rb_define_method(cls_vec, "reverse!",         siren_vec_reverse_bang,     MRB_ARGS_NONE());
  rb_define_method(cls_vec, "angle",            siren_vec_angle,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "angleref",         siren_vec_angleref,         MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "magnitude",        siren_vec_magnitude,        MRB_ARGS_NONE());
  rb_define_method(cls_vec, "size",             siren_vec_magnitude,        MRB_ARGS_NONE());
  rb_define_method(cls_vec, "length",           siren_vec_magnitude,        MRB_ARGS_NONE());
  rb_define_method(cls_vec, "cross",            siren_vec_cross,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "cross!",           siren_vec_cross_bang,       MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "dot",              siren_vec_dot,              MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "dot_cross",        siren_vec_dot_cross,        MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "cross_cross",      siren_vec_cross_cross,      MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "cross_cross!",     siren_vec_cross_cross_bang, MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "cross_mag",        siren_vec_cross_mag,        MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "cross_square_mag", siren_vec_cross_square_mag, MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "square_mag",       siren_vec_square_mag,       MRB_ARGS_NONE());

  rb_define_method(cls_vec, "mirror",           siren_vec_mirror,           MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "mirror!",          siren_vec_mirror_bang,      MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "rotate",           siren_vec_rotate,           MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "rotate!",          siren_vec_rotate_bang,      MRB_ARGS_REQ(2));
  rb_define_method(cls_vec, "scale",            siren_vec_scale,            MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "scale!",           siren_vec_scale_bang,       MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "transform",        siren_vec_transform,        MRB_ARGS_REQ(1));
  rb_define_method(cls_vec, "transform!",       siren_vec_transform_bang,   MRB_ARGS_REQ(1));

  rb_define_module_function(cls_vec, "-@",  siren_vec_negative,         MRB_ARGS_NONE());
  rb_define_module_function(cls_vec, "==",  siren_vec_eq,               MRB_ARGS_REQ(1));
  rb_define_module_function(cls_vec, "+",   siren_vec_plus,             MRB_ARGS_REQ(1));
  rb_define_module_function(cls_vec, "-",   siren_vec_minus,            MRB_ARGS_REQ(1));
  rb_define_module_function(cls_vec, "*",   siren_vec_multiply_scalar,  MRB_ARGS_REQ(1));
  rb_define_module_function(cls_vec, "/",   siren_vec_devide_scalar,    MRB_ARGS_REQ(1));

  return true;
}

VALUE siren_vec_init( VALUE self)
{
  VALUE* a;
  rb_int len;
  int argc = rb_get_args("*", &a, &len);

  Standard_Real x = 0.0, y = 0.0, z = 0.0;
  if (len > 0 && rb_array_p(a[0])) {
    gp_Pnt p = siren_ary_to_pnt(a[0]);
    x = p.X(); y = p.Y(); z = p.Z();
  }
  else {
    if (len >= 1) {
      if (_fixnum_p(a[0]))
        x = rb_fixnum(a[0]);
      else if VALUE_p(a[0])
        x = VALUE(a[0]);
    }
    if (len >= 2) {
      if (_fixnum_p(a[1]))
        y = rb_fixnum(a[1]);
      else if (_float_p(a[1]))
        y = VALUE(a[1]);
    }
    if (len >= 3) {
      if (_fixnum_p(a[2]))
        z = rb_fixnum(a[2]);
      else if (_float_p(a[2]))
        z = VALUE(a[2]);
    }
  }

  void* p = rb_malloc(sizeof(gp_Vec));
  gp_Vec* vec = new(p) gp_Vec(x, y, z);
  DATA_PTR(self) = vec;
  DATA_TYPE(self) = &siren_vec_type;
  return self;
}

void siren_vec_final( void* p)
{
  gp_Vec* v = static_cast<gp_Vec*>(p);
  rb_free(v);
}

VALUE siren_vec_x( VALUE self)
{
  return (siren_vec_get(self)->X());
}

VALUE siren_vec_x_set( VALUE self)
{
  VALUE val;
  int argc = rb_get_args("f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetX(val);
  return (vec->X());
}

VALUE siren_vec_y( VALUE self)
{
  return (siren_vec_get(self)->Y());
}

VALUE siren_vec_y_set( VALUE self)
{
  VALUE val;
  int argc = rb_get_args("f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetY(val);
  return (vec->Y());
}

VALUE siren_vec_z( VALUE self)
{
  return (siren_vec_get(self)->Z());
}

VALUE siren_vec_z_set( VALUE self)
{
  VALUE val;
  int argc = rb_get_args("f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetZ(val);
  return (vec->Z());
}

VALUE siren_vec_to_a( VALUE self)
{
  gp_Vec* vec = siren_vec_get(self);
  return siren_vec_to_ary(*vec);
}

VALUE siren_vec_xyz( VALUE self)
{
  return siren_vec_to_a(self);
}

VALUE siren_vec_is_equal( VALUE self)
{
  VALUE other;
  VALUE lintol, angtol;
  int argc = rb_get_args("off", &other, &lintol, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsEqual(*o, lintol, angtol);
  return res ? rb_true_value() : rb_false_value();
}

VALUE siren_vec_is_normal( VALUE self)
{
  VALUE other;
  VALUE angtol;
  int argc = rb_get_args("of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsNormal(*o, angtol);
  return res ? rb_true_value() : rb_false_value();
}

VALUE siren_vec_is_reverse( VALUE self)
{
  VALUE other;
  VALUE angtol;
  int argc = rb_get_args("of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsOpposite(*o, angtol);
  return res ? rb_true_value() : rb_false_value();
}

VALUE siren_vec_is_parallel( VALUE self)
{
  VALUE other;
  VALUE angtol;
  int argc = rb_get_args("of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsParallel(*o, angtol);
  return res ? rb_true_value() : rb_false_value();
}

VALUE siren_vec_normal( VALUE self)
{
  return siren_vec_new(siren_vec_get(self)->Normalized());
}

VALUE siren_vec_normal_bang( VALUE self)
{
  siren_vec_get(self)->Normalize();
  return self;
}

VALUE siren_vec_reverse( VALUE self)
{
  return siren_vec_new(siren_vec_get(self)->Reversed());
}

VALUE siren_vec_reverse_bang( VALUE self)
{
  siren_vec_get(self)->Reverse();
  return self;
}

VALUE siren_vec_angle( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Real res = me->Angle(*o);
  return (res);
}

VALUE siren_vec_angleref( VALUE self)
{
  VALUE other, vref;
  int argc = rb_get_args("oo", &other, &vref);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  gp_Vec* ref = siren_vec_get(vref);
  Standard_Real res = me->AngleWithRef(*o, *ref);
  return (res);
}

VALUE siren_vec_magnitude( VALUE self)
{
  Standard_Real res = siren_vec_get(self)->Magnitude();
  return (res);
}

VALUE siren_vec_negative( VALUE self)
{
  gp_Vec ans = -(*siren_vec_get(self));
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_eq( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  Standard_Real lintol = 0.0, angtol = 0.0; // to be use the default tolerance value
  Standard_Boolean ans = siren_vec_get(self)->IsEqual(*siren_vec_get(other), lintol, angtol);
  return ans ? rb_true_value() : rb_false_value();
}

VALUE siren_vec_plus( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  gp_Vec ans = *siren_vec_get(self) + *siren_vec_get(other);
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_minus( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  gp_Vec ans = *siren_vec_get(self) - *siren_vec_get(other);
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_multiply_scalar( VALUE self)
{
  VALUE factor;
  int argc = rb_get_args("f", &factor);
  gp_Vec ans = *siren_vec_get(self) * factor;
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_devide_scalar( VALUE self)
{
  VALUE factor;
  int argc = rb_get_args("f", &factor);
  gp_Vec ans = *siren_vec_get(self) / factor;
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  gp_Vec ans = siren_vec_get(self)->Crossed(*siren_vec_get(other));
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross_bang( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  siren_vec_get(self)->Cross(*siren_vec_get(other));
  return self;
}

VALUE siren_vec_dot( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  Standard_Real ans = siren_vec_get(self)->Dot(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_dot_cross( VALUE self)
{
  VALUE v1, v2;
  int argc = rb_get_args("o", &v1, &v2);
  Standard_Real ans = siren_vec_get(self)->DotCross(
      *siren_vec_get(v1),
      *siren_vec_get(v2));
  return (ans);
}

VALUE siren_vec_cross_cross( VALUE self)
{
  VALUE v1, v2;
  int argc = rb_get_args("o", &v1, &v2);
  gp_Vec ans = siren_vec_get(self)->CrossCrossed(
      *siren_vec_get(v1),
      *siren_vec_get(v2)
      );
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross_cross_bang( VALUE self)
{
  VALUE v1, v2;
  int argc = rb_get_args("o", &v1, &v2);
  siren_vec_get(self)->CrossCross(
      *siren_vec_get(v1),
      *siren_vec_get(v2)
      );
  return self;
}

VALUE siren_vec_cross_mag( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  Standard_Real ans = siren_vec_get(self)->CrossMagnitude(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_cross_square_mag( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  Standard_Real ans = siren_vec_get(self)->CrossSquareMagnitude(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_square_mag( VALUE self)
{
  Standard_Real res = siren_vec_get(self)->SquareMagnitude();
  return (res);
}

VALUE siren_vec_mirror( VALUE self)
{
  VALUE dir;
  int argc = rb_get_args("o", &dir);
  gp_Vec res = siren_vec_get(self)->Mirrored(*siren_vec_get(dir));
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_mirror_bang( VALUE self)
{
  VALUE dir;
  int argc = rb_get_args("o", &dir);
  siren_vec_get(self)->Mirror(*siren_vec_get(dir));
  return self;
}

VALUE siren_vec_rotate( VALUE self)
{
  VALUE dir;
  VALUE angle;
  int argc = rb_get_args("of", &dir, &angle);
  gp_Vec res = siren_vec_get(self)->Rotated(
      gp_Ax1(gp_Pnt(0.0, 0.0, 0.0), *siren_vec_get(dir)), angle);
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_rotate_bang( VALUE self)
{
  VALUE dir;
  VALUE angle;
  int argc = rb_get_args("of", &dir, &angle);
  siren_vec_get(self)->Rotate(
      gp_Ax1(gp_Pnt(0.0, 0.0, 0.0), *siren_vec_get(dir)), angle);
  return self;
}

VALUE siren_vec_scale( VALUE self)
{
  VALUE f;
  int argc = rb_get_args("f", &f);
  gp_Vec res = siren_vec_get(self)->Scaled(f);
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_scale_bang( VALUE self)
{
  VALUE f;
  int argc = rb_get_args("f", &f);
  siren_vec_get(self)->Scale(f);
  return self;
}

VALUE siren_vec_transform( VALUE self)
{
  VALUE t;
  int argc = rb_get_args("o", &t);
  gp_Vec res = siren_vec_get(self)->Transformed(*siren_trans_get(t));
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_transform_bang( VALUE self)
{
  VALUE t;
  int argc = rb_get_args("o", &t);
  siren_vec_get(self)->Transform(*siren_trans_get(t));
  return self;
}

