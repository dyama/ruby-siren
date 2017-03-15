/**
 * ellipse.cpp
 * Implementation of singleton methods for ELLIPSE
 */

#include "curve.h"

VALUE sr_cEllipse;

SR_CURVE_GET(Ellipse, ellipse)

bool siren_ellipse_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_ellipse = rb_define_class_under(sr_mSiren, "Ellipse", cls_curve);
  MRB_SET_INSTANCE_TT(cls_ellipse, MRB_TT_DATA);
#endif
  rb_define_method(sr_cEllipse, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
