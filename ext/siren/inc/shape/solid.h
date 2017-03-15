#ifndef _SHAPE_SOLID_H_
#define _SHAPE_SOLID_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>

extern VALUE sr_cSolid;

static rb_data_type_t siren_solid_type = { "Solid", siren_shape_final };
bool siren_solid_install();
TopoDS_Solid siren_solid_get(VALUE self);
VALUE siren_solid_new( const TopoDS_Shape* src);

VALUE siren_solid_init(int, VALUE*, VALUE);

VALUE siren_solid_box        (int, VALUE*, VALUE);
VALUE siren_solid_box2p      (int, VALUE*, VALUE);
VALUE siren_solid_boxax      (int, VALUE*, VALUE);
VALUE siren_solid_sphere     (int, VALUE*, VALUE);
VALUE siren_solid_cylinder   (int, VALUE*, VALUE);
VALUE siren_solid_cone       (int, VALUE*, VALUE);
VALUE siren_solid_torus      (int, VALUE*, VALUE);
VALUE siren_solid_halfspace  (int, VALUE*, VALUE);
VALUE siren_solid_prism      (int, VALUE*, VALUE);
VALUE siren_solid_revol      (int, VALUE*, VALUE);
VALUE siren_solid_revolution (int, VALUE*, VALUE);
VALUE siren_solid_wedge      (int, VALUE*, VALUE);

#endif
