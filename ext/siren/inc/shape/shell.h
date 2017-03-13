#ifndef _SHAPE_SHELL_H_
#define _SHAPE_SHELL_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>
#include <BRepBuilderAPI_Sewing.hxx>

VALUE sr_cShell;

static rb_data_type_t siren_shell_type = { "Shell", siren_shape_final };
bool siren_shell_install();
TopoDS_Shell siren_shell_get( VALUE self);
VALUE siren_shell_new( const TopoDS_Shape* src);
VALUE siren_shell_obj();
struct RClass* siren_shell_rclass();

VALUE siren_shell_make( VALUE self);

#endif
