#ifndef _CURVE_CIRCLE_H_
#define _CURVE_CIRCLE_H_

#include "siren.h"

#include <Geom_Circle.hxx>

extern VALUE sr_cCircle;

static rb_data_type_t siren_circle_type = { "Circle", siren_curve_final };
bool siren_circle_install();
handle<Geom_Circle> siren_circle_get(VALUE);
VALUE siren_circle_new( const handle<Geom_Curve>* curve);

VALUE siren_circle_radius(int, VALUE*, VALUE);
VALUE siren_circle_radius_set(int, VALUE*, VALUE);
VALUE siren_circle_center(int, VALUE*, VALUE);
VALUE siren_circle_center_set(int, VALUE*, VALUE);
VALUE siren_circle_area(int, VALUE*, VALUE);
VALUE siren_circle_length(int, VALUE*, VALUE);
VALUE siren_circle_normal(int, VALUE*, VALUE);
VALUE siren_circle_normal_set(int, VALUE*, VALUE);
VALUE siren_circle_dir(int, VALUE*, VALUE);
VALUE siren_circle_dir_set(int, VALUE*, VALUE);
VALUE siren_circle_dist(int, VALUE*, VALUE);
VALUE siren_circle_distdist(int, VALUE*, VALUE);
VALUE siren_circle_contain(int, VALUE*, VALUE);

#endif
