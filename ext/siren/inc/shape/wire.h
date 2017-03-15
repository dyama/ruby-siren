#ifndef _SHAPE_WIRE_H_
#define _SHAPE_WIRE_H_

#include "siren.h"
#include "shape.h"
#include "topalgo.h"
#include "vec.h"

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>

// #include <ShapeFix_Wire.hxx>
// #include <ShapeExtend_WireData.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeFix_ShapeTolerance.hxx>
#include <BRepTools_WireExplorer.hxx>

extern VALUE sr_cWire;

static rb_data_type_t siren_wire_type = { "Wire", siren_shape_final };
bool siren_wire_install();
TopoDS_Wire siren_wire_get(VALUE self);
VALUE siren_wire_new( const TopoDS_Shape* src);

VALUE siren_wire_make(int, VALUE*, VALUE);
VALUE siren_wire_ordered_edges(int, VALUE*, VALUE);
VALUE siren_wire_curves(int, VALUE*, VALUE);

#endif
