#ifndef _CURVE_BSPLINECURVE_H_
#define _CURVE_BSPLINECURVE_H_

#include "siren.h"

#include <Geom_BSplineCurve.hxx>

extern VALUE sr_cBSCurve;

static rb_data_type_t siren_bscurve_type = { "BSCurve", siren_curve_final };
bool siren_bscurve_install();
handle<Geom_BSplineCurve> siren_bscurve_get(VALUE self);
VALUE siren_bscurve_new( const handle<Geom_Curve>* curve);

VALUE siren_bscurve_init(...);
VALUE siren_bscurve_degree(...);
VALUE siren_bscurve_knots(...);
VALUE siren_bscurve_mults(...);
VALUE siren_bscurve_poles(...);
VALUE siren_bscurve_weights(...);

#endif
