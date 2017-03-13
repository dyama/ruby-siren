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

extern VALUE sr_cEdge;

static rb_data_type_t siren_edge_type = { "Edge", siren_shape_final };
bool siren_edge_install();
TopoDS_Edge siren_edge_get(VALUE self);
VALUE siren_edge_new( const TopoDS_Shape* src);

VALUE siren_edge_init(...);
VALUE siren_edge_sp(...);
VALUE siren_edge_tp(...);
VALUE siren_edge_to_pts(...);
VALUE siren_edge_param(...);
VALUE siren_edge_to_xyz(...);
VALUE siren_edge_curvature(...);
VALUE siren_edge_tangent(...);

VALUE siren_edge_terms(...);
VALUE siren_edge_curve(...);

VALUE siren_edge_split(...);
VALUE siren_edge_trim(...);
VALUE siren_edge_extrema(...);

VALUE siren_edge_length(...);

#endif
