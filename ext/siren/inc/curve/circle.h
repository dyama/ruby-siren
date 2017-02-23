#ifndef _CURVE_CIRCLE_H_
#define _CURVE_CIRCLE_H_

#include "siren.h"

#include <Geom_Circle.hxx>

static rb_data_type_t siren_circle_type = { "Circle", siren_curve_final };
bool siren_circle_install( struct RClass* mod_siren);
handle<Geom_Circle> siren_circle_get( VALUE self);
VALUE siren_circle_new( const handle<Geom_Curve>* curve);

VALUE siren_circle_radius( VALUE self);
VALUE siren_circle_radius_set( VALUE self);
VALUE siren_circle_center( VALUE self);
VALUE siren_circle_center_set( VALUE self);
VALUE siren_circle_area( VALUE self);
VALUE siren_circle_length( VALUE self);
VALUE siren_circle_normal( VALUE self);
VALUE siren_circle_normal_set( VALUE self);
VALUE siren_circle_dir( VALUE self);
VALUE siren_circle_dir_set( VALUE self);
VALUE siren_circle_dist( VALUE self);
VALUE siren_circle_distdist( VALUE self);
VALUE siren_circle_contain( VALUE self);

#endif
