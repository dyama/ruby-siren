/**
 * parabola.cpp
 * Implementation of singleton methods for PARABOLA
 */

#include "curve.h"

VALUE sr_cParabola;

SR_CURVE_GET(Parabola, parabola)

bool siren_parabola_install()
{
  SR_CURVE_INIT(Parabola)
  rb_define_method(sr_cParabola, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
