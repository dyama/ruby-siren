#include "common.h"

/* this function copied from class.c */
VALUE rb_instance_alloc( VALUE cv)
{
  struct RClass *c = rb_class_ptr(cv);
  struct RObject *o;
  enum rb_vtype ttype = MRB_INSTANCE_TT(c);

  if (c->tt == MRB_TT_SCLASS)
    rb_raise(E_TYPE_ERROR, "can't create instance of singleton class");

  if (ttype == 0) ttype = MRB_TT_OBJECT;
  o = (struct RObject*)rb_obj_alloc(ttype, c);
  return rb_obj_value(o);
}
/* end of function */

void siren_ary_to_xyz( VALUE ary, Standard_Real& x, Standard_Real& y, Standard_Real& z)
{
  x = 0.0; y = 0.0; z = 0.0;
  int len = RARRAY_LEN(ary);
  if (len > 0) {
    VALUE val = RARRAY_AREF(ary, 0);
    if (RB_FLOAT_TYPE_P(val)) {
      x = VALUE(val);
    }
    else if (_fixnum_p(val)) {
      x = rb_fixnum(val);
    }
  }
  if (len > 1) {
    VALUE val = RARRAY_AREF(ary, 1);
    if (RB_FLOAT_TYPE_P(val)) {
      y = VALUE(val);
    }
    else if (_fixnum_p(val)) {
      y = rb_fixnum(val);
    }
  }
  if (len > 2) {
    VALUE val = RARRAY_AREF(ary, 2);
    if (RB_FLOAT_TYPE_P(val)) {
      z = VALUE(val);
    }
    else if (_fixnum_p(val)) {
      z = rb_fixnum(val);
    }
  }
  return;
}

