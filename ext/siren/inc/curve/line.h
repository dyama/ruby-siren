#ifndef _CURVE_LINE_H_
#define _CURVE_LINE_H_

#include "siren.h"

#include <Geom_Line.hxx>

static rb_data_type_t siren_line_type = { "Line", siren_curve_final };
bool siren_line_install( struct RClass* mod_siren);
handle<Geom_Line> siren_line_get( VALUE self);
VALUE siren_line_new( const handle<Geom_Curve>* curve);

VALUE siren_line_dir( VALUE self);

#endif
