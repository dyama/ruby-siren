#ifndef _FILLER_H_
#define _FILLER_H_

#include "siren.h"

#include <BRepFill_Filling.hxx>

VALUE sr_cFiller;

bool siren_filler_install( struct RClass* rclass);
void siren_filler_final( void* p);
static rb_data_type_t siren_filler_type = { "Filler", siren_filler_final };

BRepFill_Filling* siren_filler_get( VALUE obj);

VALUE siren_filler_init(...);
VALUE siren_filler_add_bound(...);
VALUE siren_filler_add(...);
VALUE siren_filler_build(...);
VALUE siren_filler_is_done(...);
VALUE siren_filler_face(...);
VALUE siren_filler_g0error(...);
VALUE siren_filler_g1error(...);
VALUE siren_filler_g2error(...);

#endif
