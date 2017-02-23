#ifndef _TRANS_H_
#define _TRANS_H_

#include "common.h"
#include "siren.h"
#include "vec.h"

#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>

bool siren_trans_install( struct RClass* rclass);
void siren_trans_final( void* p);
static rb_data_type_t siren_trans_type = { "Trans", siren_trans_final };
gp_Trsf* siren_trans_get( VALUE obj);
struct RClass* siren_trans_rclass();
VALUE siren_trans_new( const gp_Trsf& src);

VALUE siren_trans_init( VALUE self);
VALUE siren_trans_to_s( VALUE self);
VALUE siren_trans_to_a( VALUE self);
VALUE siren_trans_matrix( VALUE self);
VALUE siren_trans_set_matrix( VALUE self);
VALUE siren_trans_multiply( VALUE self);
VALUE siren_trans_multiply_bang( VALUE self);
VALUE siren_trans_power( VALUE self);
VALUE siren_trans_power_bang( VALUE self);
VALUE siren_trans_invert( VALUE self);
VALUE siren_trans_invert_bang( VALUE self);
VALUE siren_trans_is_negative( VALUE self);
VALUE siren_trans_translate_bang( VALUE self);
VALUE siren_trans_translatef( VALUE self);
VALUE siren_trans_set_translatef( VALUE self);
VALUE siren_trans_mirror_bang( VALUE self);
VALUE siren_trans_rotate_bang( VALUE self);
//VALUE siren_trans_rotatef( VALUE self);
//VALUE siren_trans_set_rotatef( VALUE self);
VALUE siren_trans_scale_bang( VALUE self);
VALUE siren_trans_scalef( VALUE self);
VALUE siren_trans_set_scalef( VALUE self);
VALUE siren_trans_transform_bang( VALUE self);
VALUE siren_trans_move_point( VALUE self);

#endif
