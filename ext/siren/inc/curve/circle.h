#ifndef _CURVE_CIRCLE_H_
#define _CURVE_CIRCLE_H_

#include "siren.h"

#include <Geom_Circle.hxx>

VALUE sr_cCircle;

static rb_data_type_t siren_circle_type = { "Circle", siren_curve_final };
bool siren_circle_install();
handle<Geom_Circle> siren_circle_get(VALUE);
VALUE siren_circle_new( const handle<Geom_Curve>* curve);

VALUE siren_circle_radius(...);
VALUE siren_circle_radius_set(...);
VALUE siren_circle_center(...);
VALUE siren_circle_center_set(...);
VALUE siren_circle_area(...);
VALUE siren_circle_length(...);
VALUE siren_circle_normal(...);
VALUE siren_circle_normal_set(...);
VALUE siren_circle_dir(...);
VALUE siren_circle_dir_set(...);
VALUE siren_circle_dist(...);
VALUE siren_circle_distdist(...);
VALUE siren_circle_contain(...);

#endif
