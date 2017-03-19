#ifndef _CURVE_BEZIERCURVE_H_
#define _CURVE_BEZIERCURVE_H_

#include "siren.h"

#include <Geom_BezierCurve.hxx>

extern VALUE sr_cBzCurve;

static rb_data_type_t siren_bzcurve_type = { "BzCurve", siren_curve_final };
bool siren_bzcurve_install();
handle<Geom_BezierCurve> siren_bzcurve_get(VALUE self);
VALUE siren_bzcurve_new( const handle<Geom_Curve>* curve);

VALUE siren_bzcurve_init(int, VALUE*, VALUE);

#endif
