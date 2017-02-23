#ifndef _CURVE_OFFSETCURVE_H_
#define _CURVE_OFFSETCURVE_H_

#include "siren.h"

#include <Geom_OffsetCurve.hxx>

static rb_data_type_t siren_offsetcurve_type = { "OffsetCurve", siren_curve_final };
bool siren_offsetcurve_install();
handle<Geom_OffsetCurve> siren_offsetcurve_get( VALUE self);
VALUE siren_offsetcurve_new( const handle<Geom_Curve>* curve);

#endif
