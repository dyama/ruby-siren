#ifndef _HEAL_H_
#define _HEAL_H_

#include "siren.h"
#include "shape.h"

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>

#include <ShapeFix_Shape.hxx>

#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>

VALUE siren_heal_outerwire( VALUE self);
VALUE siren_heal_fix( VALUE self);

#endif
