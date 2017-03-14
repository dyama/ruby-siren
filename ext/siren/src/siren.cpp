#include "siren.h"

#include "vec.h"
// #include "curve.h"
// #include "shape.h"
// #include "topalgo.h"
// #include "brep.h"
// #include "trans.h"
// #include "bndbox.h"
// #include "filler.h"
// 
// #ifdef SIREN_ENABLE_BO
//   #include "bo.h"
// #endif
// 
// #ifdef SIREN_ENABLE_SHHEALING
//   #include "heal.h"
// #endif
// 
// #ifdef SIREN_ENABLE_OFFSET
//   #include "offset.h"
// #endif
// 
// #ifdef SIREN_ENABLE_IGES
//   #include "io/iges.h"
// #endif
// 
// #ifdef SIREN_ENABLE_STL
//   #include "io/stl.h"
// #endif
// 
// #ifdef SIREN_ENABLE_STEP
//   #include "io/step.h"
// #endif

VALUE sr_mSiren;

extern "C" {

  // initializer
  void Init_siren()
  {
    // Siren module
    sr_mSiren  = rb_define_module("Siren");

    // Class
    siren_vec_install    ();
//     siren_bndbox_install ();
//     siren_curve_install  ();
//     siren_shape_install  ();
//     siren_trans_install  ();
// 
//     // API class
//     siren_filler_install ();
// 
//     // Method
//     siren_topalgo_install();
//     siren_brep_install   ();
// #ifdef SIREN_ENABLE_OFFSET
//     siren_offset_install ();
// #endif
// #ifdef SIREN_ENABLE_BO
//     siren_bo_install     ();
// #endif
// #ifdef SIREN_ENABLE_IGES
//     siren_iges_install   ();
// #endif
// #ifdef SIREN_ENABLE_STL
//     siren_stl_install    ();
// #endif
// #ifdef SIREN_ENABLE_STEP
//     siren_step_install   ();
// #endif

    return;
  }

  // finalizer
  void rb_mruby_siren_gem_final()
  {
    return;
  }

} // extern "C" {
