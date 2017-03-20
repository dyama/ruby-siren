#include "trans.h"

VALUE sr_cTrans;

SR_CLASS_INIT(gp_Trsf, Trans, trans)

VALUE siren_trans_new(const gp_Trsf& src)
{
  VALUE t = siren_trans_allocate(sr_cTrans);
  gp_Trsf* pt = siren_trans_get(t);
  *pt = src;
  return t;
}

bool siren_trans_install()
{
  sr_cTrans = rb_define_class_under(sr_mSiren, "Trans", rb_cObject);
  rb_define_alloc_func(sr_cTrans, siren_trans_allocate);
  rb_define_method(sr_cTrans, "initialize"     , RUBY_METHOD_FUNC(siren_trans_init              ), -1);
  rb_define_method(sr_cTrans, "to_s"           , RUBY_METHOD_FUNC(siren_trans_to_s              ), -1);
  rb_define_method(sr_cTrans, "to_a"           , RUBY_METHOD_FUNC(siren_trans_matrix            ), -1);
  rb_define_method(sr_cTrans, "matrix"         , RUBY_METHOD_FUNC(siren_trans_matrix            ), -1);
  rb_define_method(sr_cTrans, "matrix="        , RUBY_METHOD_FUNC(siren_trans_set_matrix        ), -1);
  rb_define_method(sr_cTrans, "multiply"       , RUBY_METHOD_FUNC(siren_trans_multiply          ), -1);
  rb_define_method(sr_cTrans, "multiply!"      , RUBY_METHOD_FUNC(siren_trans_multiply_bang     ), -1);
  rb_define_method(sr_cTrans, "power"          , RUBY_METHOD_FUNC(siren_trans_power             ), -1);
  rb_define_method(sr_cTrans, "power!"         , RUBY_METHOD_FUNC(siren_trans_power_bang        ), -1);
  rb_define_method(sr_cTrans, "translate!"     , RUBY_METHOD_FUNC(siren_trans_translate_bang    ), -1);
  rb_define_method(sr_cTrans, "translatef"     , RUBY_METHOD_FUNC(siren_trans_translatef        ), -1);
  rb_define_method(sr_cTrans, "translatef="    , RUBY_METHOD_FUNC(siren_trans_set_translatef    ), -1);
  rb_define_method(sr_cTrans, "mirror"         , RUBY_METHOD_FUNC(siren_trans_mirror            ), -1);
  rb_define_method(sr_cTrans, "mirror!"        , RUBY_METHOD_FUNC(siren_trans_mirror_bang       ), -1);
  rb_define_method(sr_cTrans, "rotate!"        , RUBY_METHOD_FUNC(siren_trans_rotate_bang       ), -1);
  rb_define_method(sr_cTrans, "scale!"         , RUBY_METHOD_FUNC(siren_trans_scale_bang        ), -1);
  rb_define_method(sr_cTrans, "scalef"         , RUBY_METHOD_FUNC(siren_trans_scalef            ), -1);
  rb_define_method(sr_cTrans, "scalef="        , RUBY_METHOD_FUNC(siren_trans_set_scalef        ), -1);
  rb_define_method(sr_cTrans, "invert"         , RUBY_METHOD_FUNC(siren_trans_invert            ), -1);
  rb_define_method(sr_cTrans, "invert!"        , RUBY_METHOD_FUNC(siren_trans_invert_bang       ), -1);
  rb_define_method(sr_cTrans, "reverse"        , RUBY_METHOD_FUNC(siren_trans_invert            ), -1);
  rb_define_method(sr_cTrans, "reverse!"       , RUBY_METHOD_FUNC(siren_trans_invert_bang       ), -1);
  rb_define_method(sr_cTrans, "negative?"      , RUBY_METHOD_FUNC(siren_trans_is_negative       ), -1);
  rb_define_method(sr_cTrans, "transform!"     , RUBY_METHOD_FUNC(siren_trans_transform_bang    ), -1);
  rb_define_method(sr_cTrans, "move_point"     , RUBY_METHOD_FUNC(siren_trans_move_point        ), -1);
//  rb_define_method(sr_cTrans, "rotatef"        , siren_trans_rotatef            , -1);
//  rb_define_method(sr_cTrans, "rotatef="       , siren_trans_set_rotatef        , -1);
  return true;
}

VALUE siren_trans_init(int argc, VALUE* argv, VALUE self)
{
  return self;
}

void siren_trans_final(void* p)
{
  gp_Trsf* t = static_cast<gp_Trsf*>(p);
  ruby_xfree(t);
}

VALUE siren_trans_to_s(int argc, VALUE* argv, VALUE self)
{
  char str[256];
  Standard_Real a11 = siren_trans_get(self)->Value(1, 1);
  Standard_Real a12 = siren_trans_get(self)->Value(1, 2);
  Standard_Real a13 = siren_trans_get(self)->Value(1, 3);
  Standard_Real a14 = siren_trans_get(self)->Value(1, 4);
  Standard_Real a21 = siren_trans_get(self)->Value(2, 1);
  Standard_Real a22 = siren_trans_get(self)->Value(2, 2);
  Standard_Real a23 = siren_trans_get(self)->Value(2, 3);
  Standard_Real a24 = siren_trans_get(self)->Value(2, 4);
  Standard_Real a31 = siren_trans_get(self)->Value(3, 1);
  Standard_Real a32 = siren_trans_get(self)->Value(3, 2);
  Standard_Real a33 = siren_trans_get(self)->Value(3, 3);
  Standard_Real a34 = siren_trans_get(self)->Value(3, 4);
  snprintf(str, sizeof(str),
      "#<Trans:0x%x\n"
      "    X = % 2.6f, % 2.6f, % 2.6f\n"
      "    Y = % 2.6f, % 2.6f, % 2.6f\n"
      "    Z = % 2.6f, % 2.6f, % 2.6f\n"
      "    T = % 2.6f, % 2.6f, % 2.6f>",
      -1, // (unsigned int)(uintptr_t)rb_cptr(self),
      a11, a21, a31,
      a12, a22, a32,
      a13, a23, a33,
      a14, a24, a34
      );
  return rb_str_new_cstr(str);
}

VALUE siren_trans_translate_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE v;
  rb_scan_args(argc, argv, "1", &v);
  gp_Vec vec = siren_ary_to_vec(v); 
  gp_Trsf* trans = siren_trans_get(self);
  trans->SetTranslation(vec);
  return self;
}

VALUE siren_trans_rotate_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm;
  VALUE angle;
  rb_scan_args(argc, argv, "3", &op, &norm, &angle);
  gp_Trsf* trans = siren_trans_get(self);
  trans->SetRotation(siren_ary_to_ax1(op, norm), angle);
  return self;
}

/*
VALUE siren_trans_rotatef(int argc, VALUE* argv, VALUE self)
{
  gp_XYZ axis;
  Standard_Real angle;
  if (siren_trans_get(self)->GetRotation(axis, angle) == Standard_False) {
    return Qnil;
  }
  VALUE res[] = {
    siren_pnt_new(axis.X(), axis.Y(), axis.Z()),
    (angle)
  };
  return rb_ary_new_from_values(2, res);
}

VALUE siren_trans_set_rotatef(int argc, VALUE* argv, VALUE self)
{
  return Qnil;
}
*/

VALUE siren_trans_scale_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op;
  VALUE factor;
  rb_scan_args(argc, argv, "2", &op, &factor);
  gp_Trsf* trans  = siren_trans_get(self);
  trans->SetScale(siren_ary_to_pnt(op), factor);
  return self;
}

VALUE siren_trans_scalef(int argc, VALUE* argv, VALUE self)
{
  gp_Trsf* trans  = siren_trans_get(self);
  Standard_Real f = trans->ScaleFactor();
  return (f);
}

VALUE siren_trans_set_scalef(int argc, VALUE* argv, VALUE self)
{
  VALUE f;
  rb_scan_args(argc, argv, "1", &f);
  gp_Trsf* trans  = siren_trans_get(self);
  trans->SetScaleFactor(f);
  return (f);
}

VALUE siren_trans_mirror(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm, vx;
  rb_scan_args(argc, argv, "12", &op, &norm, &vx);
  auto res = siren_trans_new(*siren_trans_get(self));
  auto p = siren_trans_get(res);
  switch (argc) {
  case 1:
    p->SetMirror(siren_ary_to_pnt(op));
    break;
  case 2:
    p->SetMirror(siren_ary_to_ax1(op, norm));
    break;
  case 3:
    p->SetMirror(siren_ary_to_ax2(op, norm, vx));
    break;
  }
  return res;
}

VALUE siren_trans_mirror_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm, vx;
  rb_scan_args(argc, argv, "12", &op, &norm, &vx);
  switch (argc) {
  case 1:
    siren_trans_get(self)->SetMirror(siren_ary_to_pnt(op));
    break;
  case 2:
    siren_trans_get(self)->SetMirror(siren_ary_to_ax1(op, norm));
    break;
  case 3:
    siren_trans_get(self)->SetMirror(siren_ary_to_ax2(op, norm, vx));
    break;
  }
  return self;
}

VALUE siren_trans_multiply(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  gp_Trsf* trans_me = siren_trans_get(self);
  gp_Trsf* trans_other = siren_trans_get(other);
  gp_Trsf t = trans_me->Multiplied(*trans_other);
  return siren_trans_new(t);
}

VALUE siren_trans_multiply_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  gp_Trsf* trans_me = siren_trans_get(self);
  gp_Trsf* trans_other = siren_trans_get(other);
  trans_me->Multiply(*trans_other);
  return self;
}

VALUE siren_trans_power(int argc, VALUE* argv, VALUE self)
{
  VALUE n;
  rb_scan_args(argc, argv, "1", &n);
  return siren_trans_new(siren_trans_get(self)->Powered(n));
}

VALUE siren_trans_power_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE n;
  rb_scan_args(argc, argv, "1", &n);
  siren_trans_get(self)->Power(n);
  return self;
}

VALUE siren_trans_invert(int argc, VALUE* argv, VALUE self)
{
  return siren_trans_new(siren_trans_get(self)->Inverted());
}

VALUE siren_trans_invert_bang(int argc, VALUE* argv, VALUE self)
{
  siren_trans_get(self)->Invert();
  return self;
}

VALUE siren_trans_is_negative(int argc, VALUE* argv, VALUE self)
{
  return siren_trans_get(self)->IsNegative() ? Qtrue : Qfalse;
}

VALUE siren_trans_transform_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE pos1, norm1, vdir1;
  VALUE pos2, norm2, vdir2;
  rb_scan_args(argc, argv, "33", &pos1, &norm1, &vdir1, &pos2, &norm2, &vdir2);
  gp_Trsf* trans = siren_trans_get(self);
  if (argc == 3) {
    trans->SetTransformation(siren_ary_to_ax3(pos1, norm1, vdir1));
  }
  else if (argc == 6) {
    trans->SetTransformation(siren_ary_to_ax3(pos1, norm1, vdir1), siren_ary_to_ax3(pos2, norm2, vdir2));
  }
  else {
    rb_raise(Qnil, "Number of arguments is wrong.");
  }
  return self;
}

VALUE siren_trans_matrix(int argc, VALUE* argv, VALUE self)
{
  VALUE result[4];
  for (int row = 1; row <= 4; row++) {
    result[row - 1] = rb_ary_new();
    for (int col = 1; col <= 3; col++) {
      rb_ary_push(result[row - 1], DBL2NUM(siren_trans_get(self)->Value(col, row)));
    }
  }
  return rb_ary_new_from_values(4, result);
}

VALUE siren_trans_set_matrix(int argc, VALUE* argv, VALUE self)
{
  VALUE ary;
  rb_scan_args(argc, argv, "1", &ary);
  if (RARRAY_LEN(ary) != 4) {
    rb_raise(Qnil, "Number of items of specified array is wrong.");
  }
  gp_Vec x = siren_ary_to_vec(RARRAY_AREF(ary, 0));
  gp_Vec y = siren_ary_to_vec(RARRAY_AREF(ary, 1));
  gp_Vec z = siren_ary_to_vec(RARRAY_AREF(ary, 2));
  gp_Vec t = siren_ary_to_vec(RARRAY_AREF(ary, 3));
  gp_Trsf* trans = siren_trans_get(self);
  trans->SetValues(
      x.X(), y.X(), z.X(), t.X(), 
      x.Y(), y.Y(), z.Y(), t.Y(), 
      x.Z(), y.Z(), z.Z(), t.Z());
  return self;
}

VALUE siren_trans_translatef(int argc, VALUE* argv, VALUE self)
{
  gp_Trsf* trans = siren_trans_get(self);
  gp_XYZ xyz = trans->TranslationPart();
  return siren_pnt_new(xyz.X(), xyz.Y(), xyz.Z());
}

VALUE siren_trans_set_translatef(int argc, VALUE* argv, VALUE self)
{
  VALUE v;
  rb_scan_args(argc, argv, "1", &v);
  siren_trans_get(self)->SetTranslationPart(siren_ary_to_vec(v));
  return Qnil;
}

VALUE siren_trans_move_point(int argc, VALUE* argv, VALUE self)
{
  VALUE ary;
  rb_scan_args(argc, argv, "1", &ary);
  gp_Pnt point = siren_ary_to_pnt(ary);
  gp_Trsf* trans = siren_trans_get(self);
  point.Transform(*trans);
  return siren_pnt_to_ary(point);
}
