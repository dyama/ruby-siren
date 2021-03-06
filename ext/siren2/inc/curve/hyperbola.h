#ifndef _CURVE_HYPERBOLA_H_
#define _CURVE_HYPERBOLA_H_

#include "siren.h"

#include <Geom_Hyperbola.hxx>

extern VALUE sr_cHyperbola;

static rb_data_type_t siren_hyperbola_type = { "Hyperbola", siren_curve_final };
bool siren_hyperbola_install();
handle<Geom_Hyperbola> siren_hyperbola_get(VALUE self);
VALUE siren_hyperbola_new( const handle<Geom_Curve>* curve);

#endif
