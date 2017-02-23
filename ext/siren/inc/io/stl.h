#ifndef _STL_H_
#define _STL_H_

#include "siren.h"
#include "vec.h"
#include "shape.h"

#include <StlAPI.hxx>

bool siren_stl_install( struct RClass* rclass);
VALUE siren_stl_load( VALUE self);

#endif
