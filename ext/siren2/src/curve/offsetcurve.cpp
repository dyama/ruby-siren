/**
 * offsetcurve.cpp
 * Implementation of singleton methods for OFFSETCURVE
 */

#include "curve.h"

VALUE sr_cOffsetCurve;

SR_CURVE_INIT(OffsetCurve, OffsetCurve, offsetcurve)

bool siren_offsetcurve_install()
{
  SR_CURVE_DEFINE(OffsetCurve)
  rb_define_method(sr_cOffsetCurve, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
