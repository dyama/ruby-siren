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

static rb_data_type_t siren_solid_type = { "Solid", siren_shape_final };
bool siren_solid_install();
TopoDS_Solid siren_solid_get( VALUE self);
VALUE siren_solid_new( const TopoDS_Shape* src);
VALUE siren_solid_obj();
struct RClass* siren_solid_rclass();

VALUE siren_solid_init( VALUE self);

VALUE siren_solid_box        ( VALUE self);
VALUE siren_solid_box2p      ( VALUE self);
VALUE siren_solid_boxax      ( VALUE self);
VALUE siren_solid_sphere     ( VALUE self);
VALUE siren_solid_cylinder   ( VALUE self);
VALUE siren_solid_cone       ( VALUE self);
VALUE siren_solid_torus      ( VALUE self);
VALUE siren_solid_halfspace  ( VALUE self);
VALUE siren_solid_prism      ( VALUE self);
VALUE siren_solid_revol      ( VALUE self);
VALUE siren_solid_revolution ( VALUE self);
VALUE siren_solid_wedge      ( VALUE self);

#endif
