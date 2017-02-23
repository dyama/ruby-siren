#ifndef _SHAPE_EDGE_H_
#define _SHAPE_EDGE_H_

#include "siren.h"
#include "curve.h"
#include "shape.h"

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>

#include <TopExp_Explorer.hxx>
#include <BRepAdaptor_Curve.hxx>

// to_pts
#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_UniformDeflection.hxx>

// param
#include <ShapeAnalysis_Curve.hxx>
#include <BRepAdaptor_Curve.hxx>

#include <BRepExtrema_ExtCC.hxx>

static rb_data_type_t siren_edge_type = { "Edge", siren_shape_final };
bool siren_edge_install();
TopoDS_Edge siren_edge_get( VALUE self);
VALUE siren_edge_new( const TopoDS_Shape* src);
VALUE siren_edge_obj();
struct RClass* siren_edge_rclass();

VALUE siren_edge_init( VALUE self);
VALUE siren_edge_sp( VALUE self);
VALUE siren_edge_tp( VALUE self);
VALUE siren_edge_to_pts( VALUE self);
VALUE siren_edge_param( VALUE self);
VALUE siren_edge_to_xyz( VALUE self);
VALUE siren_edge_curvature( VALUE self);
VALUE siren_edge_tangent( VALUE self);

VALUE siren_edge_terms( VALUE self);
VALUE siren_edge_curve( VALUE self);

VALUE siren_edge_split( VALUE self);
VALUE siren_edge_trim( VALUE self);
VALUE siren_edge_extrema( VALUE self);

VALUE siren_edge_length( VALUE self);

#endif
