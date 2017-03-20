#ifndef _SR_H_
#define _SR_H_

// Configuration
#define SR_ENABLE_GPROP
#define SR_ENABLE_BO
#define SR_ENABLE_OFFSET
//#define SR_ENABLE_SHHEALING
#define SR_ENABLE_STL
#define SR_ENABLE_IGES
#define SR_ENABLE_STEP
//#define SR_ENABLE_CHUNK

// C++ standard library headers
#include <iostream>
#include <stdlib.h>

// placement new
#include <new>

// Ruby headers
#include "ruby.h"

// OpenCASCADE headers
#include <Standard.hxx>
#include <Standard_TypeDef.hxx>
#include <Standard_Macro.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <gp_Pnt.hxx>

using namespace opencascade;

extern VALUE sr_mSiren;

#include "common.h"

#define SR_CLASS_DATATYPE(U,L) const rb_data_type_t siren_##L##_type \
  = { "Siren::" # U, { 0, siren_##L##_final, }, 0, 0, 0, };

#define SR_CLASS_ALLOCATE(O,L) VALUE siren_##L##_allocate(VALUE klass) \
  { \
    O* p; \
    return TypedData_Make_Struct(klass, O, &siren_##L##_type, p); \
  }

#define SR_CLASS_GET(O,L) O* siren_##L##_get(VALUE obj) \
{ \
  O* p; \
  TypedData_Get_Struct(obj, O, &siren_##L##_type, p); \
  return p; \
}

#define SR_CLASS_INIT(O,U,L) \
  SR_CLASS_DATATYPE(U,L) \
  SR_CLASS_ALLOCATE(O,L) \
  SR_CLASS_GET(O,L)

#endif
