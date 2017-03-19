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

VALUE siren_trans_init(int, VALUE*, VALUE);
VALUE siren_trans_to_s(int, VALUE*, VALUE);
VALUE siren_trans_to_a(int, VALUE*, VALUE);
VALUE siren_trans_matrix(int, VALUE*, VALUE);
VALUE siren_trans_set_matrix(int, VALUE*, VALUE);
VALUE siren_trans_multiply(int, VALUE*, VALUE);
VALUE siren_trans_multiply_bang(int, VALUE*, VALUE);
VALUE siren_trans_power(int, VALUE*, VALUE);
VALUE siren_trans_power_bang(int, VALUE*, VALUE);
VALUE siren_trans_invert(int, VALUE*, VALUE);
VALUE siren_trans_invert_bang(int, VALUE*, VALUE);
VALUE siren_trans_is_negative(int, VALUE*, VALUE);
VALUE siren_trans_translate_bang(int, VALUE*, VALUE);
VALUE siren_trans_translatef(int, VALUE*, VALUE);
VALUE siren_trans_set_translatef(int, VALUE*, VALUE);
VALUE siren_trans_mirror(int, VALUE*, VALUE);
VALUE siren_trans_mirror_bang(int, VALUE*, VALUE);
VALUE siren_trans_rotate_bang(int, VALUE*, VALUE);
//VALUE siren_trans_rotatef(int, VALUE*, VALUE);
//VALUE siren_trans_set_rotatef(int, VALUE*, VALUE);
VALUE siren_trans_scale_bang(int, VALUE*, VALUE);
VALUE siren_trans_scalef(int, VALUE*, VALUE);
VALUE siren_trans_set_scalef(int, VALUE*, VALUE);
VALUE siren_trans_transform_bang(int, VALUE*, VALUE);
VALUE siren_trans_move_point(int, VALUE*, VALUE);

#endif
