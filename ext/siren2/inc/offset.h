#ifndef _OFFSET_H_
#define _OFFSET_H_

#include "siren.h"
#include "shape.h"
#include "topalgo.h"

// sweep_vec
#include <BRepOffsetAPI_MakePipe.hxx>

// sweep_path
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <TopExp.hxx>
#include <Law_S.hxx>

// loft
#include <BRepOffsetAPI_ThruSections.hxx>

// offset
#include <Geom_OffsetSurface.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>

bool siren_offset_install();

VALUE siren_offset_sweep_vec(int, VALUE*, VALUE);
VALUE siren_offset_sweep_path(int, VALUE*, VALUE);
VALUE siren_offset_loft(int, VALUE*, VALUE);
VALUE siren_offset_offset_geomsurf(int, VALUE*, VALUE);
VALUE siren_offset_offset(int, VALUE*, VALUE);
VALUE siren_offset_offset_shape(int, VALUE*, VALUE);
VALUE siren_offset_pipe(int, VALUE*, VALUE);

#endif
