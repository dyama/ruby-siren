#ifndef _SIREN_H_
#define _SIREN_H_

// Configuration
#define SIREN_ENABLE_GPROP
#define SIREN_ENABLE_BO
#define SIREN_ENABLE_OFFSET
#define SIREN_ENABLE_SHHEALING
#define SIREN_ENABLE_STL
#define SIREN_ENABLE_IGES
#define SIREN_ENABLE_STEP
//#define SIREN_ENABLE_CHUNK

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

#define SR_CLASS_INIT(U,L) \
  sr_c##U = rb_define_class_under(sr_mSiren, #U, rb_cObject); \
  rb_define_alloc_func(sr_c##U, siren_##L##_allocate);

#endif
