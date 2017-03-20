#ifndef _SHAPE_SHELL_H_
#define _SHAPE_SHELL_H_

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>
#include <BRepBuilderAPI_Sewing.hxx>

extern VALUE sr_cShell;

bool siren_shell_install();
TopoDS_Shell siren_shell_get(VALUE self);
bool siren_shell_p(const VALUE&);
void siren_shell_check(const VALUE&);
VALUE siren_shell_new( const TopoDS_Shape* src);

VALUE siren_shell_make(int, VALUE*, VALUE);

#endif
