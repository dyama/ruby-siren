#ifndef _SHAPE_FACE_H_
#define _SHAPE_FACE_H_

#include "siren.h"
#include "shape.h"
#include "vec.h"

#include <gp_Ax3.hxx>
#include <gp_Pln.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>

#include <BRepTools.hxx>
#include <GeomLProp_SLProps.hxx>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <Geom_BezierSurface.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>

#include <Geom_BSplineSurface.hxx>
#include <ShapeFix_Shape.hxx>

// to_bezier
#include <TopoDS_Compound.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <GeomConvert_BSplineSurfaceToBezierSurface.hxx>
#include <TColGeom_Array2OfBezierSurface.hxx>

// split
#include <BRepFeat_SplitShape.hxx>

// triangle
#include <BRepMesh_IncrementalMesh.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Triangle.hxx>
#include <Poly_Array1OfTriangle.hxx>

extern VALUE sr_cFace;

static rb_data_type_t siren_face_type = { "Face", siren_shape_final };
bool siren_face_install();
TopoDS_Face siren_face_get(VALUE self);
VALUE siren_face_new( const TopoDS_Shape* src);

VALUE siren_face_plane(...);
VALUE siren_face_face(...);
VALUE siren_face_infplane(...);
VALUE siren_face_polygon(...);
VALUE siren_face_bzsurf(...);
VALUE siren_face_bssurf(...);

VALUE siren_face_normal(...);
VALUE siren_face_to_bezier(...);
VALUE siren_face_split(...);
VALUE siren_face_triangle(...);

#endif
