#ifndef _BO_H_
#define _BO_H_

#include "siren.h"
#include "shape.h"

#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

#include <BRepProj_Projection.hxx>

bool siren_bo_install();
VALUE siren_bo_common   (...);
VALUE siren_bo_fuse     (...);
VALUE siren_bo_cut      (...);
VALUE siren_bo_projwire (...);

#endif
