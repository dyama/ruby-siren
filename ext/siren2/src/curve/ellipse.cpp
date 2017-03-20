/**
 * ellipse.cpp
 * Implementation of singleton methods for ELLIPSE
 */

#include "curve.h"

VALUE sr_cEllipse;

SR_CURVE_INIT(Ellipse, Ellipse, ellipse)

bool siren_ellipse_install()
{
  SR_CURVE_DEFINE(Ellipse)
  rb_define_method(sr_cEllipse, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
