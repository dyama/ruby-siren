#include "vec.h"

#define rb_array_p(x)  RB_TYPE_P(x, T_ARRAY)
#define rb_fixnum_p(x) FIXNUM_P(x)
#define rb_float_p(x)  RB_FLOAT_TYPE_P(x)

VALUE sr_cVec;

gp_Vec* siren_vec_get( VALUE obj)
{
#if 0
  return static_cast<gp_Vec*>(_get_datatype(obj, &siren_vec_type));
#else
  gp_Vec* m;
  Data_Get_Struct(obj, gp_Vec, m);
  return m;
#endif
}

VALUE siren_vec_new( double x, double y, double z)
{
  VALUE arg = rb_ary_new();
  rb_ary_push(arg, (x));
  rb_ary_push(arg, (y));
  rb_ary_push(arg, (z));
  return rb_class_new_instance(1, &arg, sr_mSiren);
}

VALUE siren_vec_new( const gp_Vec& vec)
{
  return siren_vec_new(vec.X(), vec.Y(), vec.Z());
}

bool siren_vec_install()
{
  sr_cVec = rb_define_class_under(sr_mSiren, "Vec", rb_cObject);
  // MRB_SET_INSTANCE_TT(sr_cVec, MRB_TT_DATA);
  rb_define_method(sr_cVec, "initialize",       RUBY_METHOD_FUNC(siren_vec_init),             -1);
  rb_define_method(sr_cVec, "x",                RUBY_METHOD_FUNC(siren_vec_x),                -1);
  rb_define_method(sr_cVec, "x=",               RUBY_METHOD_FUNC(siren_vec_x_set),            -1);
  rb_define_method(sr_cVec, "y",                RUBY_METHOD_FUNC(siren_vec_y),                -1);
  rb_define_method(sr_cVec, "y=",               RUBY_METHOD_FUNC(siren_vec_y_set),            -1);
  rb_define_method(sr_cVec, "z",                RUBY_METHOD_FUNC(siren_vec_z),                -1);
  rb_define_method(sr_cVec, "z=",               RUBY_METHOD_FUNC(siren_vec_z_set),            -1);
  rb_define_method(sr_cVec, "equal?",           RUBY_METHOD_FUNC(siren_vec_is_equal),         -1);
  rb_define_method(sr_cVec, "parallel?",        RUBY_METHOD_FUNC(siren_vec_is_parallel),      -1);
  rb_define_method(sr_cVec, "normal?",          RUBY_METHOD_FUNC(siren_vec_is_normal),        -1);
  rb_define_method(sr_cVec, "normal",           RUBY_METHOD_FUNC(siren_vec_normal),           -1);
  rb_define_method(sr_cVec, "normal!",          RUBY_METHOD_FUNC(siren_vec_normal_bang),      -1);
  rb_define_method(sr_cVec, "reverse?",         RUBY_METHOD_FUNC(siren_vec_is_reverse),       -1);
  rb_define_method(sr_cVec, "reverse",          RUBY_METHOD_FUNC(siren_vec_reverse),          -1);
  rb_define_method(sr_cVec, "reverse!",         RUBY_METHOD_FUNC(siren_vec_reverse_bang),     -1);
  rb_define_method(sr_cVec, "angle",            RUBY_METHOD_FUNC(siren_vec_angle),            -1);
  rb_define_method(sr_cVec, "angleref",         RUBY_METHOD_FUNC(siren_vec_angleref),         -1);
  rb_define_method(sr_cVec, "magnitude",        RUBY_METHOD_FUNC(siren_vec_magnitude),        -1);
  rb_define_method(sr_cVec, "size",             RUBY_METHOD_FUNC(siren_vec_magnitude),        -1);
  rb_define_method(sr_cVec, "length",           RUBY_METHOD_FUNC(siren_vec_magnitude),        -1);
  rb_define_method(sr_cVec, "cross",            RUBY_METHOD_FUNC(siren_vec_cross),            -1);
  rb_define_method(sr_cVec, "cross!",           RUBY_METHOD_FUNC(siren_vec_cross_bang),       -1);
  rb_define_method(sr_cVec, "dot",              RUBY_METHOD_FUNC(siren_vec_dot),              -1);
  rb_define_method(sr_cVec, "dot_cross",        RUBY_METHOD_FUNC(siren_vec_dot_cross),        -1);
  rb_define_method(sr_cVec, "cross_cross",      RUBY_METHOD_FUNC(siren_vec_cross_cross),      -1);
  rb_define_method(sr_cVec, "cross_cross!",     RUBY_METHOD_FUNC(siren_vec_cross_cross_bang), -1);
  rb_define_method(sr_cVec, "cross_mag",        RUBY_METHOD_FUNC(siren_vec_cross_mag),        -1);
  rb_define_method(sr_cVec, "cross_square_mag", RUBY_METHOD_FUNC(siren_vec_cross_square_mag), -1);
  rb_define_method(sr_cVec, "square_mag",       RUBY_METHOD_FUNC(siren_vec_square_mag),       -1);

  rb_define_method(sr_cVec, "mirror",           RUBY_METHOD_FUNC(siren_vec_mirror),           -1);
  rb_define_method(sr_cVec, "mirror!",          RUBY_METHOD_FUNC(siren_vec_mirror_bang),      -1);
  rb_define_method(sr_cVec, "rotate",           RUBY_METHOD_FUNC(siren_vec_rotate),           -1);
  rb_define_method(sr_cVec, "rotate!",          RUBY_METHOD_FUNC(siren_vec_rotate_bang),      -1);
  rb_define_method(sr_cVec, "scale",            RUBY_METHOD_FUNC(siren_vec_scale),            -1);
  rb_define_method(sr_cVec, "scale!",           RUBY_METHOD_FUNC(siren_vec_scale_bang),       -1);
  rb_define_method(sr_cVec, "transform",        RUBY_METHOD_FUNC(siren_vec_transform),        -1);
  rb_define_method(sr_cVec, "transform!",       RUBY_METHOD_FUNC(siren_vec_transform_bang),   -1);

  rb_define_module_function(sr_cVec, "-@",      RUBY_METHOD_FUNC(siren_vec_negative),         -1);
  rb_define_module_function(sr_cVec, "==",      RUBY_METHOD_FUNC(siren_vec_eq),               -1);
  rb_define_module_function(sr_cVec, "+",       RUBY_METHOD_FUNC(siren_vec_plus),             -1);
  rb_define_module_function(sr_cVec, "-",       RUBY_METHOD_FUNC(siren_vec_minus),            -1);
  rb_define_module_function(sr_cVec, "*",       RUBY_METHOD_FUNC(siren_vec_multiply_scalar),  -1);
  rb_define_module_function(sr_cVec, "/",       RUBY_METHOD_FUNC(siren_vec_devide_scalar),    -1);

  return true;
}

VALUE siren_vec_init(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);

  Standard_Real x = 0.0, y = 0.0, z = 0.0;
  if (len > 0 && rb_array_p(a[0])) {
    gp_Pnt p = siren_ary_to_pnt(a[0]);
    x = p.X(); y = p.Y(); z = p.Z();
  }
  else {
    if (len >= 1) {
      if (rb_fixnum_p(a[0]))
        x = DBL2NUM(a[0]);
      else if (rb_float_p(a[0]))
        x = (a[0]);
    }
    if (len >= 2) {
      if (rb_fixnum_p(a[1]))
        y = DBL2NUM(a[1]);
      else if (rb_float_p(a[1]))
        y = VALUE(a[1]);
    }
    if (len >= 3) {
      if (rb_fixnum_p(a[2]))
        z = DBL2NUM(a[2]);
      else if (rb_float_p(a[2]))
        z = (a[2]);
    }
  }

  void* p = ruby_xmalloc(sizeof(gp_Vec));
  gp_Vec* vec = new(p) gp_Vec(x, y, z);
  DATA_PTR(self) = vec;
  //DATA_TYPE(self) = &siren_vec_type;
  return self;
}

void siren_vec_final( void* p)
{
  gp_Vec* v = static_cast<gp_Vec*>(p);
  ruby_xfree(v);
}

VALUE siren_vec_x(int argc, VALUE* argv, VALUE self)
{
  return (siren_vec_get(self)->X());
}

VALUE siren_vec_x_set(int argc, VALUE* argv, VALUE self)
{
  VALUE val;
  rb_scan_args(argc, argv, "f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetX(val);
  return (vec->X());
}

VALUE siren_vec_y(int argc, VALUE* argv, VALUE self)
{
  return (siren_vec_get(self)->Y());
}

VALUE siren_vec_y_set(int argc, VALUE* argv, VALUE self)
{
  VALUE val;
  rb_scan_args(argc, argv, "f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetY(val);
  return (vec->Y());
}

VALUE siren_vec_z(int argc, VALUE* argv, VALUE self)
{
  return (siren_vec_get(self)->Z());
}

VALUE siren_vec_z_set(int argc, VALUE* argv, VALUE self)
{
  VALUE val;
  rb_scan_args(argc, argv, "f", &val);
  gp_Vec* vec = siren_vec_get(self);
  vec->SetZ(val);
  return (vec->Z());
}

VALUE siren_vec_to_a(int argc, VALUE* argv, VALUE self)
{
  gp_Vec* vec = siren_vec_get(self);
  return siren_vec_to_ary(*vec);
}

VALUE siren_vec_xyz(int argc, VALUE* argv, VALUE self)
{
  return siren_vec_to_a(0, nullptr, self);
}

VALUE siren_vec_is_equal(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  VALUE lintol, angtol;
  rb_scan_args(argc, argv, "off", &other, &lintol, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsEqual(*o, lintol, angtol);
  return res ? Qtrue : Qfalse;
}

VALUE siren_vec_is_normal(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  VALUE angtol;
  rb_scan_args(argc, argv, "of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsNormal(*o, angtol);
  return res ? Qtrue : Qfalse;
}

VALUE siren_vec_is_reverse(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  VALUE angtol;
  rb_scan_args(argc, argv, "of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsOpposite(*o, angtol);
  return res ? Qtrue : Qfalse;
}

VALUE siren_vec_is_parallel(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  VALUE angtol;
  rb_scan_args(argc, argv, "of", &other, &angtol);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Boolean res = me->IsParallel(*o, angtol);
  return res ? Qtrue : Qfalse;
}

VALUE siren_vec_normal(int argc, VALUE* argv, VALUE self)
{
  return siren_vec_new(siren_vec_get(self)->Normalized());
}

VALUE siren_vec_normal_bang(int argc, VALUE* argv, VALUE self)
{
  siren_vec_get(self)->Normalize();
  return self;
}

VALUE siren_vec_reverse(int argc, VALUE* argv, VALUE self)
{
  return siren_vec_new(siren_vec_get(self)->Reversed());
}

VALUE siren_vec_reverse_bang(int argc, VALUE* argv, VALUE self)
{
  siren_vec_get(self)->Reverse();
  return self;
}

VALUE siren_vec_angle(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  Standard_Real res = me->Angle(*o);
  return (res);
}

VALUE siren_vec_angleref(int argc, VALUE* argv, VALUE self)
{
  VALUE other, vref;
  rb_scan_args(argc, argv, "oo", &other, &vref);
  gp_Vec* me = siren_vec_get(self);
  gp_Vec* o = siren_vec_get(other);
  gp_Vec* ref = siren_vec_get(vref);
  Standard_Real res = me->AngleWithRef(*o, *ref);
  return (res);
}

VALUE siren_vec_magnitude(int argc, VALUE* argv, VALUE self)
{
  Standard_Real res = siren_vec_get(self)->Magnitude();
  return (res);
}

VALUE siren_vec_negative(int argc, VALUE* argv, VALUE self)
{
  gp_Vec ans = -(*siren_vec_get(self));
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_eq(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  Standard_Real lintol = 0.0, angtol = 0.0; // to be use the default tolerance value
  Standard_Boolean ans = siren_vec_get(self)->IsEqual(*siren_vec_get(other), lintol, angtol);
  return ans ? Qtrue : Qfalse;
}

VALUE siren_vec_plus(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  gp_Vec ans = *siren_vec_get(self) + *siren_vec_get(other);
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_minus(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  gp_Vec ans = *siren_vec_get(self) - *siren_vec_get(other);
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_multiply_scalar(int argc, VALUE* argv, VALUE self)
{
  VALUE factor;
  rb_scan_args(argc, argv, "f", &factor);
  gp_Vec ans = *siren_vec_get(self) * factor;
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_devide_scalar(int argc, VALUE* argv, VALUE self)
{
  VALUE factor;
  rb_scan_args(argc, argv, "f", &factor);
  gp_Vec ans = *siren_vec_get(self) / factor;
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  gp_Vec ans = siren_vec_get(self)->Crossed(*siren_vec_get(other));
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  siren_vec_get(self)->Cross(*siren_vec_get(other));
  return self;
}

VALUE siren_vec_dot(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  Standard_Real ans = siren_vec_get(self)->Dot(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_dot_cross(int argc, VALUE* argv, VALUE self)
{
  VALUE v1, v2;
  rb_scan_args(argc, argv, "o", &v1, &v2);
  Standard_Real ans = siren_vec_get(self)->DotCross(
      *siren_vec_get(v1),
      *siren_vec_get(v2));
  return (ans);
}

VALUE siren_vec_cross_cross(int argc, VALUE* argv, VALUE self)
{
  VALUE v1, v2;
  rb_scan_args(argc, argv, "o", &v1, &v2);
  gp_Vec ans = siren_vec_get(self)->CrossCrossed(
      *siren_vec_get(v1),
      *siren_vec_get(v2)
      );
  return siren_vec_new(ans.X(), ans.Y(), ans.Z());
}

VALUE siren_vec_cross_cross_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE v1, v2;
  rb_scan_args(argc, argv, "o", &v1, &v2);
  siren_vec_get(self)->CrossCross(
      *siren_vec_get(v1),
      *siren_vec_get(v2)
      );
  return self;
}

VALUE siren_vec_cross_mag(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  Standard_Real ans = siren_vec_get(self)->CrossMagnitude(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_cross_square_mag(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "o", &other);
  Standard_Real ans = siren_vec_get(self)->CrossSquareMagnitude(*siren_vec_get(other));
  return (ans);
}

VALUE siren_vec_square_mag(int argc, VALUE* argv, VALUE self)
{
  Standard_Real res = siren_vec_get(self)->SquareMagnitude();
  return (res);
}

VALUE siren_vec_mirror(int argc, VALUE* argv, VALUE self)
{
  VALUE dir;
  rb_scan_args(argc, argv, "o", &dir);
  gp_Vec res = siren_vec_get(self)->Mirrored(*siren_vec_get(dir));
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_mirror_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE dir;
  rb_scan_args(argc, argv, "o", &dir);
  siren_vec_get(self)->Mirror(*siren_vec_get(dir));
  return self;
}

VALUE siren_vec_rotate(int argc, VALUE* argv, VALUE self)
{
  VALUE dir;
  VALUE angle;
  rb_scan_args(argc, argv, "of", &dir, &angle);
  gp_Vec res = siren_vec_get(self)->Rotated(
      gp_Ax1(gp_Pnt(0.0, 0.0, 0.0), *siren_vec_get(dir)), angle);
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_rotate_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE dir;
  VALUE angle;
  rb_scan_args(argc, argv, "of", &dir, &angle);
  siren_vec_get(self)->Rotate(
      gp_Ax1(gp_Pnt(0.0, 0.0, 0.0), *siren_vec_get(dir)), angle);
  return self;
}

VALUE siren_vec_scale(int argc, VALUE* argv, VALUE self)
{
  VALUE f;
  rb_scan_args(argc, argv, "f", &f);
  gp_Vec res = siren_vec_get(self)->Scaled(f);
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_scale_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE f;
  rb_scan_args(argc, argv, "f", &f);
  siren_vec_get(self)->Scale(f);
  return self;
}

VALUE siren_vec_transform(int argc, VALUE* argv, VALUE self)
{
  VALUE t;
  rb_scan_args(argc, argv, "o", &t);
  gp_Vec res = siren_vec_get(self)->Transformed(*siren_trans_get(t));
  return siren_vec_new(res.X(), res.Y(), res.Z());
}

VALUE siren_vec_transform_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE t;
  rb_scan_args(argc, argv, "o", &t);
  siren_vec_get(self)->Transform(*siren_trans_get(t));
  return self;
}

