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

bool siren_offset_install( struct RClass* rclass);

VALUE siren_offset_sweep_vec( VALUE self);
VALUE siren_offset_sweep_path( VALUE self);
VALUE siren_offset_loft( VALUE self);
VALUE siren_offset_offset_geomsurf( VALUE self);
VALUE siren_offset_offset( VALUE self);
VALUE siren_offset_offset_shape( VALUE self);
VALUE siren_offset_pipe( VALUE self);

#endif
