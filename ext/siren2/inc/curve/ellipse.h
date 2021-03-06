#ifndef _CURVE_ELLIPSE_H_
#define _CURVE_ELLIPSE_H_

#include "siren.h"

#include <Geom_Ellipse.hxx>

extern VALUE sr_cEllipse;

static rb_data_type_t siren_ellipse_type = { "Ellipse", siren_curve_final };
bool siren_ellipse_install();
handle<Geom_Ellipse> siren_ellipse_get(VALUE self);
VALUE siren_ellipse_new( const handle<Geom_Curve>* curve);

#endif
