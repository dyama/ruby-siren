#ifndef _STEP_H_
#define _STEP_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>

bool siren_step_install( struct RClass* rclass);

VALUE siren_step_save( VALUE self);
VALUE siren_step_load( VALUE self);

#endif
