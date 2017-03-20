/**
 * hyperbola.cpp
 * Implementation of singleton methods for HYPERBOLA
 */

#include "curve.h"

VALUE sr_cHyperbola;

SR_CURVE_INIT(Hyperbola, Hyperbola, hyperbola)

bool siren_hyperbola_install()
{
  SR_CURVE_DEFINE(Hyperbola)
  rb_define_method(sr_cHyperbola, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
