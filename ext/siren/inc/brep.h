#ifndef _BREP_H_
#define _BREP_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

bool siren_brep_install();

VALUE siren_brep_save(int, VALUE*, VALUE);
VALUE siren_brep_load(int, VALUE*, VALUE);
VALUE siren_brep_dump(int, VALUE*, VALUE);

#endif
