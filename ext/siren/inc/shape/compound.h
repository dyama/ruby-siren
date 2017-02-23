#ifndef _SHAPE_COMPOUND_H_
#define _SHAPE_COMPOUND_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>

static rb_data_type_t siren_compound_type = { "Compound", siren_shape_final };
bool siren_compound_install( struct RClass* sr_mSiren);
TopoDS_Compound siren_compound_get( VALUE self);
VALUE siren_compound_new( const TopoDS_Shape* src);
VALUE siren_compound_obj();
struct RClass* siren_compound_rclass();

VALUE siren_compound_init( VALUE self);
VALUE siren_compound_push( VALUE self);
VALUE siren_compound_delete( VALUE self);

#endif
