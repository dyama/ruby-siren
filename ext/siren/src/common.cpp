#include "common.h"

void siren_ary_to_xyz(VALUE ary, Standard_Real& x, Standard_Real& y, Standard_Real& z)
{
  x = 0.0; y = 0.0; z = 0.0;

  Check_Type(ary, T_ARRAY);

  int len = RARRAY_LEN(ary);
  if (len > 0) {
    x = NUM2DBL(RARRAY_AREF(ary, 0));
  }
  if (len > 1) {
    y = NUM2DBL(RARRAY_AREF(ary, 1));
  }
  if (len > 2) {
    z = NUM2DBL(RARRAY_AREF(ary, 2));
  }
  return;
}