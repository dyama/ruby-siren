#include "common.h"

void siren_ary_to_xyz(VALUE ary, Standard_Real& x, Standard_Real& y, Standard_Real& z)
{
  x = 0.0; y = 0.0; z = 0.0;

  int len = RARRAY_LEN(ary);
  if (len > 0) {
    auto xx = RARRAY_AREF(ary, 0);
    if (siren_numeric_p(xx)) {
      x = NUM2DBL(xx);
    }
  }
  if (len > 1) {
    auto yy = RARRAY_AREF(ary, 1);
    if (siren_numeric_p(yy)) {
      y = NUM2DBL(yy);
    }
  }
  if (len > 2) {
    auto zz = RARRAY_AREF(ary, 2);
    if (siren_numeric_p(zz)) {
      z = NUM2DBL(zz);
    }
  }
  return;
}

bool siren_numeric_p(const VALUE& target)
{
  return rb_obj_is_kind_of(target, rb_cNumeric) == Qtrue;
}

void siren_numeric_check(const VALUE& target)
{
  if (!siren_numeric_p(target)) {
    VALUE type = rb_funcall(target, rb_intern("class"), 0);
    VALUE type_str = rb_funcall(type, rb_intern("to_s"), 0);
    VALUE etype_str = rb_funcall(rb_cNumeric, rb_intern("to_s"), 0);
    rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)",
        RSTRING_PTR(type_str), RSTRING_PTR(etype_str));
  }
}

