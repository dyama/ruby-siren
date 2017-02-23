#include "siren.h"

#include "vec.h"
#include "curve.h"
#include "shape.h"
#include "topalgo.h"
#include "brep.h"
#include "trans.h"
#include "bndbox.h"
#include "filler.h"

#ifdef SIREN_ENABLE_BO
  #include "bo.h"
#endif

#ifdef SIREN_ENABLE_SHHEALING
  #include "heal.h"
#endif

#ifdef SIREN_ENABLE_OFFSET
  #include "offset.h"
#endif

#ifdef SIREN_ENABLE_IGES
  #include "io/iges.h"
#endif

#ifdef SIREN_ENABLE_STL
  #include "io/stl.h"
#endif

#ifdef SIREN_ENABLE_STEP
  #include "io/step.h"
#endif

extern "C" {

  // initializer
  void rb_mruby_siren_gem_init()
  {
    // Siren module
    sr_mSiren  = rb_define_module("Siren");

    // Class
    siren_bndbox_install (sr_mSiren);
    siren_curve_install  (sr_mSiren);
    siren_shape_install  (sr_mSiren);
    siren_trans_install  (sr_mSiren);
    siren_vec_install    (sr_mSiren);

    // API class
    siren_filler_install (sr_mSiren);

    // Method
    siren_topalgo_install(sr_mSiren);
    siren_brep_install   (sr_mSiren);
#ifdef SIREN_ENABLE_OFFSET
    siren_offset_install (sr_mSiren);
#endif
#ifdef SIREN_ENABLE_BO
    siren_bo_install     (sr_mSiren);
#endif
#ifdef SIREN_ENABLE_IGES
    siren_iges_install   (sr_mSiren);
#endif
#ifdef SIREN_ENABLE_STL
    siren_stl_install    (sr_mSiren);
#endif
#ifdef SIREN_ENABLE_STEP
    siren_step_install   (sr_mSiren);
#endif

    return;
  }

  // finalizer
  void rb_mruby_siren_gem_final()
  {
    return;
  }

MRB_END_DECL
