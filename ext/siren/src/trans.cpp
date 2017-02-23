#include "trans.h"

gp_Trsf* siren_trans_get( VALUE obj)
{
  return static_cast<gp_Trsf*>(mrb_get_datatype(mrb, obj, &siren_trans_type));
}

struct RClass* siren_trans_rclass()
{
  struct RClass* mod_siren = mrb_module_get(mrb, "Siren");
  return mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(mod_siren), mrb_intern_lit(mrb, "Trans")));
}

VALUE siren_trans_new( const gp_Trsf& src)
{
  VALUE res;
  res = mrb_instance_alloc(mrb, mrb_obj_value(siren_trans_rclass(mrb)));
  void* p = mrb_malloc(mrb, sizeof(gp_Trsf));
  gp_Trsf* trans = new(p) gp_Trsf();
  *trans = src;
  DATA_PTR(res) = trans;
  DATA_TYPE(res) = &siren_trans_type;
  return res;
}

bool siren_trans_install( struct RClass* mod_siren)
{
  struct RClass* cls_trans = mrb_define_class_under(mrb, mod_siren, "Trans", mrb->object_class);
  MRB_SET_INSTANCE_TT(cls_trans, MRB_TT_DATA);
  mrb_define_method(mrb, cls_trans, "initialize"     , siren_trans_init               , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "inspect"        , siren_trans_to_s               , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "to_s"           , siren_trans_to_s               , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "to_a"           , siren_trans_matrix             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "to_ary"         , siren_trans_matrix             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "matrix"         , siren_trans_matrix             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "matrix="        , siren_trans_set_matrix         , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "multiply"       , siren_trans_multiply           , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "multiply!"      , siren_trans_multiply_bang      , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "power"          , siren_trans_power              , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "power!"         , siren_trans_power_bang         , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "translate!"     , siren_trans_translate_bang     , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "translatef"     , siren_trans_translatef         , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "translatef="    , siren_trans_set_translatef     , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "mirror!"        , siren_trans_mirror_bang        , MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, cls_trans, "rotate!"        , siren_trans_rotate_bang        , MRB_ARGS_REQ(3));
//  mrb_define_method(mrb, cls_trans, "rotatef"        , siren_trans_rotatef            , MRB_ARGS_NONE());
//  mrb_define_method(mrb, cls_trans, "rotatef="       , siren_trans_set_rotatef        , MRB_ARGS_REQ(3));
  mrb_define_method(mrb, cls_trans, "scale!"         , siren_trans_scale_bang         , MRB_ARGS_REQ(2));
  mrb_define_method(mrb, cls_trans, "scalef"         , siren_trans_scalef             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "scalef="        , siren_trans_set_scalef         , MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_trans, "invert"         , siren_trans_invert             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "invert!"        , siren_trans_invert_bang        , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "reverse"        , siren_trans_invert             , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "reverse!"       , siren_trans_invert_bang        , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "negative?"      , siren_trans_is_negative        , MRB_ARGS_NONE());
  mrb_define_method(mrb, cls_trans, "transform!"     , siren_trans_transform_bang     , MRB_ARGS_REQ(6));

  mrb_define_method(mrb, cls_trans, "move_point"     , siren_trans_move_point         , MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_trans_init( VALUE self)
{
  void* p = mrb_malloc(mrb, sizeof(gp_Trsf));
  gp_Trsf* trans = new(p) gp_Trsf();
  DATA_PTR(self) = trans;
  DATA_TYPE(self) = &siren_trans_type;
  return self;
}

void siren_trans_final( void* p)
{
  gp_Trsf* t = static_cast<gp_Trsf*>(p);
  mrb_free(mrb, t);
}

VALUE siren_trans_to_s( VALUE self)
{
  char str[256];
  Standard_Real a11 = siren_trans_get(mrb, self)->Value(1, 1);
  Standard_Real a12 = siren_trans_get(mrb, self)->Value(1, 2);
  Standard_Real a13 = siren_trans_get(mrb, self)->Value(1, 3);
  Standard_Real a14 = siren_trans_get(mrb, self)->Value(1, 4);
  Standard_Real a21 = siren_trans_get(mrb, self)->Value(2, 1);
  Standard_Real a22 = siren_trans_get(mrb, self)->Value(2, 2);
  Standard_Real a23 = siren_trans_get(mrb, self)->Value(2, 3);
  Standard_Real a24 = siren_trans_get(mrb, self)->Value(2, 4);
  Standard_Real a31 = siren_trans_get(mrb, self)->Value(3, 1);
  Standard_Real a32 = siren_trans_get(mrb, self)->Value(3, 2);
  Standard_Real a33 = siren_trans_get(mrb, self)->Value(3, 3);
  Standard_Real a34 = siren_trans_get(mrb, self)->Value(3, 4);
  snprintf(str, sizeof(str),
      "#<Trans:0x%x\n"
      "    X = % 2.6f, % 2.6f, % 2.6f\n"
      "    Y = % 2.6f, % 2.6f, % 2.6f\n"
      "    Z = % 2.6f, % 2.6f, % 2.6f\n"
      "    T = % 2.6f, % 2.6f, % 2.6f>",
      (unsigned int)(uintptr_t)mrb_cptr(self),
      a11, a21, a31,
      a12, a22, a32,
      a13, a23, a33,
      a14, a24, a34
      );
  return mrb_str_new_cstr(mrb, str);
}

VALUE siren_trans_translate_bang( VALUE self)
{
  VALUE v;
  int argc = mrb_get_args(mrb, "A", &v);
  gp_Vec vec = siren_ary_to_vec(mrb, v); 
  gp_Trsf* trans = siren_trans_get(mrb, self);
  trans->SetTranslation(vec);
  return self;
}

VALUE siren_trans_rotate_bang( VALUE self)
{
  VALUE op, norm;
  mrb_float angle;
  int argc = mrb_get_args(mrb, "AAf", &op, &norm, &angle);
  gp_Trsf* trans = siren_trans_get(mrb, self);
  trans->SetRotation(siren_ary_to_ax1(mrb, op, norm), angle);
  return self;
}

/*
VALUE siren_trans_rotatef( VALUE self)
{
  gp_XYZ axis;
  Standard_Real angle;
  if (siren_trans_get(mrb, self)->GetRotation(axis, angle) == Standard_False) {
    return mrb_nil_value();
  }
  VALUE res[] = {
    siren_pnt_new(mrb, axis.X(), axis.Y(), axis.Z()),
    mrb_float_value(mrb, angle)
  };
  return mrb_ary_new_from_values(mrb, 2, res);
}

VALUE siren_trans_set_rotatef( VALUE self)
{
  return mrb_nil_value();
}
*/

VALUE siren_trans_scale_bang( VALUE self)
{
  VALUE op;
  mrb_float factor;
  int argc = mrb_get_args(mrb, "Af", &op, &factor);
  gp_Trsf* trans  = siren_trans_get(mrb, self);
  trans->SetScale(siren_ary_to_pnt(mrb, op), factor);
  return self;
}

VALUE siren_trans_scalef( VALUE self)
{
  gp_Trsf* trans  = siren_trans_get(mrb, self);
  Standard_Real f = trans->ScaleFactor();
  return mrb_float_value(mrb, f);
}

VALUE siren_trans_set_scalef( VALUE self)
{
  mrb_float f;
  int argc = mrb_get_args(mrb, "f", &f);
  gp_Trsf* trans  = siren_trans_get(mrb, self);
  trans->SetScaleFactor(f);
  return mrb_float_value(mrb, f);
}

VALUE siren_trans_mirror_bang( VALUE self)
{
  VALUE op, norm, vx;
  int argc = mrb_get_args(mrb, "A|AA", &op, &norm, &vx);
  switch (argc) {
  case 1:
    siren_trans_get(mrb, self)->SetMirror(siren_ary_to_pnt(mrb, op));
    break;
  case 2:
    siren_trans_get(mrb, self)->SetMirror(siren_ary_to_ax1(mrb, op, norm));
    break;
  case 3:
    siren_trans_get(mrb, self)->SetMirror(siren_ary_to_ax2(mrb, op, norm, vx));
    break;
  }
  return self;
}

VALUE siren_trans_multiply( VALUE self)
{
  VALUE other;
  int argc = mrb_get_args(mrb, "o", &other);
  gp_Trsf* trans_me = siren_trans_get(mrb, self);
  gp_Trsf* trans_other = siren_trans_get(mrb, other);
  gp_Trsf t = trans_me->Multiplied(*trans_other);
  return siren_trans_new(mrb, t);
}

VALUE siren_trans_multiply_bang( VALUE self)
{
  VALUE other;
  int argc = mrb_get_args(mrb, "o", &other);
  gp_Trsf* trans_me = siren_trans_get(mrb, self);
  gp_Trsf* trans_other = siren_trans_get(mrb, other);
  trans_me->Multiply(*trans_other);
  return self;
}

VALUE siren_trans_power( VALUE self)
{
  mrb_int n;
  int argc = mrb_get_args(mrb, "i", &n);
  return siren_trans_new(mrb, siren_trans_get(mrb, self)->Powered(n));
}

VALUE siren_trans_power_bang( VALUE self)
{
  mrb_int n;
  int argc = mrb_get_args(mrb, "i", &n);
  siren_trans_get(mrb, self)->Power(n);
  return self;
}

VALUE siren_trans_invert( VALUE self)
{
  return siren_trans_new(mrb, siren_trans_get(mrb, self)->Inverted());
}

VALUE siren_trans_invert_bang( VALUE self)
{
  siren_trans_get(mrb, self)->Invert();
  return self;
}

VALUE siren_trans_is_negative( VALUE self)
{
  return siren_trans_get(mrb, self)->IsNegative() ? mrb_true_value() : mrb_false_value();
}

VALUE siren_trans_transform_bang( VALUE self)
{
  VALUE pos1, norm1, vdir1;
  VALUE pos2, norm2, vdir2;
  int argc = mrb_get_args(mrb, "AAA|AAA", &pos1, &norm1, &vdir1, &pos2, &norm2, &vdir2);
  gp_Trsf* trans = siren_trans_get(mrb, self);
  if (argc == 3) {
    trans->SetTransformation(siren_ary_to_ax3(mrb, pos1, norm1, vdir1));
  }
  else if (argc == 6) {
    trans->SetTransformation(siren_ary_to_ax3(mrb, pos1, norm1, vdir1), siren_ary_to_ax3(mrb, pos2, norm2, vdir2));
  }
  else {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Number of arguments is wrong.");
  }
  return self;
}

VALUE siren_trans_matrix( VALUE self)
{
  VALUE result[4];
  for (int row = 1; row <= 4; row++) {
    result[row - 1] = mrb_ary_new(mrb);
    for (int col = 1; col <= 3; col++) {
      mrb_ary_push(mrb, result[row - 1], 
          mrb_float_value(mrb, siren_trans_get(mrb, self)->Value(col, row)));
    }
  }
  return mrb_ary_new_from_values(mrb, 4, result);
}

VALUE siren_trans_set_matrix( VALUE self)
{
  VALUE ary;
  int argc = mrb_get_args(mrb, "A", &ary);
  if (mrb_ary_len(mrb, ary) != 4) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Number of items of specified array is wrong.");
  }
  gp_Vec x = siren_ary_to_vec(mrb, mrb_ary_ref(mrb, ary, 0));
  gp_Vec y = siren_ary_to_vec(mrb, mrb_ary_ref(mrb, ary, 1));
  gp_Vec z = siren_ary_to_vec(mrb, mrb_ary_ref(mrb, ary, 2));
  gp_Vec t = siren_ary_to_vec(mrb, mrb_ary_ref(mrb, ary, 3));
  gp_Trsf* trans = siren_trans_get(mrb, self);
  trans->SetValues(
      x.X(), y.X(), z.X(), t.X(), 
      x.Y(), y.Y(), z.Y(), t.Y(), 
      x.Z(), y.Z(), z.Z(), t.Z());
  return self;
}

VALUE siren_trans_translatef( VALUE self)
{
  gp_Trsf* trans = siren_trans_get(mrb, self);
  gp_XYZ xyz = trans->TranslationPart();
  return siren_pnt_new(mrb, xyz.X(), xyz.Y(), xyz.Z());
}

VALUE siren_trans_set_translatef( VALUE self)
{
  VALUE v;
  int argc = mrb_get_args(mrb, "o", &v);
  siren_trans_get(mrb, self)->SetTranslationPart(siren_ary_to_vec(mrb, v));
  return mrb_nil_value();
}

VALUE siren_trans_move_point( VALUE self)
{
  VALUE ary;
  int argc = mrb_get_args(mrb, "A", &ary);
  gp_Pnt point = siren_ary_to_pnt(mrb, ary);
  gp_Trsf* trans = siren_trans_get(mrb, self);
  point.Transform(*trans);
  return siren_pnt_to_ary(mrb, point);
}

