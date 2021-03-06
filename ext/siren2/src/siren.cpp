#include "siren.h"

#include "vec.h"
#include "trans.h"
#include "bndbox.h"
#include "shape.h"
#include "brep.h"
#include "topalgo.h"
#include "curve.h"
#include "filler.h"

#ifdef SR_ENABLE_BO
  #include "bo.h"
#endif

#ifdef SR_ENABLE_SHHEALING
  #include "heal.h"
#endif

#ifdef SR_ENABLE_OFFSET
  #include "offset.h"
#endif

#ifdef SR_ENABLE_IGES
  #include "io/iges.h"
#endif

#ifdef SR_ENABLE_STL
  #include "io/stl.h"
#endif

#ifdef SR_ENABLE_STEP
  #include "io/step.h"
#endif

VALUE sr_mSiren;

extern "C" {

  // initializer
  void Init_siren2()
  {
    // Siren module
    sr_mSiren  = rb_define_module("Siren");

    // Class
    siren_vec_install    ();
    siren_trans_install  ();
    siren_bndbox_install ();
    siren_shape_install  ();
    siren_curve_install  ();
    siren_topalgo_install();
    siren_brep_install   ();
    siren_filler_install ();
#ifdef SR_ENABLE_OFFSET
    siren_offset_install ();
#endif
#ifdef SR_ENABLE_BO
    siren_bo_install     ();
#endif
#ifdef SR_ENABLE_IGES
    siren_iges_install   ();
#endif
#ifdef SR_ENABLE_STL
    siren_stl_install    ();
#endif
#ifdef SR_ENABLE_STEP
    siren_step_install   ();
#endif

    return;
  }

  // finalizer
  void rb_mruby_siren_gem_final()
  {
    return;
  }

} // extern "C" {
