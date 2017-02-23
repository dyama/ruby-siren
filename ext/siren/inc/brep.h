#ifndef _BREP_H_
#define _BREP_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

bool siren_brep_install( struct RClass* rclass);

VALUE siren_brep_save( VALUE self);
VALUE siren_brep_load( VALUE self);
VALUE siren_brep_dump( VALUE self);

#endif
