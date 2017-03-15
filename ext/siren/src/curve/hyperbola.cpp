/**
 * hyperbola.cpp
 * Implementation of singleton methods for HYPERBOLA
 */

#include "curve.h"

VALUE sr_cHyperbola;

SR_CURVE_GET(Hyperbola, hyperbola)

bool siren_hyperbola_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_hyperbola = rb_define_class_under(sr_mSiren, "Hyperbola", cls_curve);
  MRB_SET_INSTANCE_TT(cls_hyperbola, MRB_TT_DATA);
#endif
  rb_define_method(sr_cHyperbola, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  return true;
}
