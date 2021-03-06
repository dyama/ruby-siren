#ifndef _SHAPE_COMPOUND_H_
#define _SHAPE_COMPOUND_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>

extern VALUE sr_cCompound;

bool siren_compound_install();
TopoDS_Compound siren_compound_get(VALUE self);
bool siren_compound_p(const VALUE&);
void siren_compound_check(const VALUE&);
VALUE siren_compound_new( const TopoDS_Shape* src);
VALUE siren_compound_init(int, VALUE*, VALUE);
VALUE siren_compound_push(int, VALUE*, VALUE);
VALUE siren_compound_delete(int, VALUE*, VALUE);

#endif
