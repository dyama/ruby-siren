#ifndef _CURVE_PARABOLA_H_
#define _CURVE_PARABOLA_H_

#include "siren.h"

#include <Geom_Parabola.hxx>

static rb_data_type_t siren_parabola_type = { "Palabora", siren_curve_final };
bool siren_parabola_install( struct RClass* sr_mSiren);
handle<Geom_Parabola> siren_parabola_get( VALUE self);
VALUE siren_parabola_new( const handle<Geom_Curve>* curve);

#endif
