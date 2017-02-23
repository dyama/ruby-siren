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

bool siren_iges_install( struct RClass* rclass);

VALUE siren_iges_save( VALUE self);
VALUE siren_iges_load( VALUE self);

#endif
