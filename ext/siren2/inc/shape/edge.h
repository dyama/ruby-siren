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

bool siren_edge_install();
TopoDS_Edge siren_edge_get(VALUE self);
bool siren_edge_p(const VALUE&);
void siren_edge_check(const VALUE&);
VALUE siren_edge_new( const TopoDS_Shape* src);

VALUE siren_edge_init(int, VALUE*, VALUE);
VALUE siren_edge_sp(int, VALUE*, VALUE);
VALUE siren_edge_tp(int, VALUE*, VALUE);
VALUE siren_edge_to_pts(int, VALUE*, VALUE);
VALUE siren_edge_param(int, VALUE*, VALUE);
VALUE siren_edge_to_xyz(int, VALUE*, VALUE);
VALUE siren_edge_curvature(int, VALUE*, VALUE);
VALUE siren_edge_tangent(int, VALUE*, VALUE);

VALUE siren_edge_terms(int, VALUE*, VALUE);
VALUE siren_edge_curve(int, VALUE*, VALUE);

VALUE siren_edge_split(int, VALUE*, VALUE);
VALUE siren_edge_trim(int, VALUE*, VALUE);
VALUE siren_edge_extrema(int, VALUE*, VALUE);

#endif
