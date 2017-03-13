#ifndef _TRANS_H_
#define _TRANS_H_

#include "common.h"
#include "siren.h"
#include "vec.h"

#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>

extern VALUE sr_cTrans;

bool siren_trans_install();
void siren_trans_final( void* p);
static rb_data_type_t siren_trans_type = { "Trans", siren_trans_final };
gp_Trsf* siren_trans_get( VALUE obj);
struct RClass* siren_trans_rclass();
VALUE siren_trans_new( const gp_Trsf& src);

VALUE siren_trans_init(...);
VALUE siren_trans_to_s(...);
VALUE siren_trans_to_a(...);
VALUE siren_trans_matrix(...);
VALUE siren_trans_set_matrix(...);
VALUE siren_trans_multiply(...);
VALUE siren_trans_multiply_bang(...);
VALUE siren_trans_power(...);
VALUE siren_trans_power_bang(...);
VALUE siren_trans_invert(...);
VALUE siren_trans_invert_bang(...);
VALUE siren_trans_is_negative(...);
VALUE siren_trans_translate_bang(...);
VALUE siren_trans_translatef(...);
VALUE siren_trans_set_translatef(...);
VALUE siren_trans_mirror_bang(...);
VALUE siren_trans_rotate_bang(...);
//VALUE siren_trans_rotatef(...);
//VALUE siren_trans_set_rotatef(...);
VALUE siren_trans_scale_bang(...);
VALUE siren_trans_scalef(...);
VALUE siren_trans_set_scalef(...);
VALUE siren_trans_transform_bang(...);
VALUE siren_trans_move_point(...);

#endif
