#ifndef _SHAPE_VERTEX_H_
#define _SHAPE_VERTEX_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

extern VALUE sr_cVertex;

static rb_data_type_t siren_vertex_type = { "Vertex", siren_shape_final };
bool siren_vertex_install();
TopoDS_Vertex siren_vertex_get(VALUE self);
VALUE siren_vertex_new( const TopoDS_Shape* src);

VALUE siren_vertex_init(int, VALUE*, VALUE);
VALUE siren_vertex_xyz(int, VALUE*, VALUE);
VALUE siren_vertex_to_v(int, VALUE*, VALUE);

#endif
