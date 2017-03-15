#ifndef _IGES_H_
#define _IGES_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <IGESControl_Controller.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <Interface_Static.hxx>
//#include <IFSelect_ReturnStatus.hxx>

bool siren_iges_install();

VALUE siren_iges_save(int, VALUE*, VALUE);
VALUE siren_iges_load(int, VALUE*, VALUE);

#endif
