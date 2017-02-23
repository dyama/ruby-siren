#ifndef _FILLER_H_
#define _FILLER_H_

#include "siren.h"

#include <BRepFill_Filling.hxx>

bool siren_filler_install( struct RClass* rclass);
void siren_filler_final( void* p);
static rb_data_type_t siren_filler_type = { "Filler", siren_filler_final };

BRepFill_Filling* siren_filler_get( VALUE obj);

VALUE siren_filler_init( VALUE self);
VALUE siren_filler_add_bound( VALUE self);
VALUE siren_filler_add( VALUE self);
VALUE siren_filler_build( VALUE self);
VALUE siren_filler_is_done( VALUE self);
VALUE siren_filler_face( VALUE self);
VALUE siren_filler_g0error( VALUE self);
VALUE siren_filler_g1error( VALUE self);
VALUE siren_filler_g2error( VALUE self);

#endif
