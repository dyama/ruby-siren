/**
 * offsetcurve.cpp
 * Implementation of singleton methods for OFFSETCURVE
 */

#include "curve.h"

VALUE sr_cOffsetCurve;

SR_CURVE_GET(OffsetCurve, offsetcurve)

bool siren_offsetcurve_install()
{
  sr_cOffsetCurve = rb_define_class_under(sr_mSiren, "OffsetCurve", rb_cObject);
  rb_define_alloc_func(sr_cOffsetCurve, siren_curve_allocate);
  rb_define_method(sr_cOffsetCurve, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
