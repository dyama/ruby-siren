#ifndef _FILLER_H_
#define _FILLER_H_

#include "siren.h"

#include <BRepFill_Filling.hxx>

extern VALUE sr_cFiller;

bool siren_filler_install();
void siren_filler_final( void* p);
static rb_data_type_t siren_filler_type = { "Filler", siren_filler_final };

BRepFill_Filling* siren_filler_get( VALUE obj);

VALUE siren_filler_init(int, VALUE*, VALUE);
VALUE siren_filler_add_bound(int, VALUE*, VALUE);
VALUE siren_filler_add(int, VALUE*, VALUE);
VALUE siren_filler_build(int, VALUE*, VALUE);
VALUE siren_filler_is_done(int, VALUE*, VALUE);
VALUE siren_filler_face(int, VALUE*, VALUE);
VALUE siren_filler_g0error(int, VALUE*, VALUE);
VALUE siren_filler_g1error(int, VALUE*, VALUE);
VALUE siren_filler_g2error(int, VALUE*, VALUE);

#endif
