/**
 * parabola.cpp
 * Implementation of singleton methods for PARABOLA
 */

#include "curve.h"

VALUE sr_cParabola;

SR_CURVE_GET(Parabola, parabola)

bool siren_parabola_install()
{
  sr_cParabola = rb_define_class_under(sr_mSiren, "Parabola", rb_cObject);
  rb_define_alloc_func(sr_cParabola, siren_curve_allocate);
  rb_define_method(sr_cParabola, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
