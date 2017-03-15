#ifndef _STEP_H_
#define _STEP_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>

bool siren_step_install();

VALUE siren_step_save(int, VALUE*, VALUE);
VALUE siren_step_load(int, VALUE*, VALUE);

#endif
