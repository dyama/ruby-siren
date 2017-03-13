#ifndef _VEC_H_
#define _VEC_H_

#include "siren.h"

extern VALUE sr_cVec;

bool siren_vec_install();
void siren_vec_final(void* p);
static rb_data_type_t siren_vec_type = { "Vec", siren_vec_final };
gp_Vec* siren_vec_get(VALUE obj);
struct RClass* siren_vec_rclass();
VALUE siren_vec_new( double x, double y, double z);
VALUE siren_vec_new( const gp_Vec& vec);

VALUE siren_vec_init(...);
VALUE siren_vec_x(...);
VALUE siren_vec_x_set(...);
VALUE siren_vec_y(...);
VALUE siren_vec_y_set(...);
VALUE siren_vec_z(...);
VALUE siren_vec_z_set(...);

VALUE siren_vec_is_equal(...);
VALUE siren_vec_is_normal(...);
VALUE siren_vec_is_reverse(...);
VALUE siren_vec_is_parallel(...);
VALUE siren_vec_angle(...);
VALUE siren_vec_angleref(...);
VALUE siren_vec_magnitude(...);
VALUE siren_vec_square_mag(...);
VALUE siren_vec_cross(...);
VALUE siren_vec_cross_bang(...);
VALUE siren_vec_cross_mag(...);
VALUE siren_vec_cross_square_mag(...);
VALUE siren_vec_cross_cross(...);
VALUE siren_vec_cross_cross_bang(...);
VALUE siren_vec_dot(...);
VALUE siren_vec_dot_cross(...);
VALUE siren_vec_normal(...);
VALUE siren_vec_normal_bang(...);
VALUE siren_vec_reverse(...);
VALUE siren_vec_reverse_bang(...);
 
// VALUE siren_vec_coord(...);
// VALUE siren_vec_coord_set(...);
 
VALUE siren_vec_mirror(...);
VALUE siren_vec_mirror_bang(...);
VALUE siren_vec_rotate(...);
VALUE siren_vec_rotate_bang(...);
VALUE siren_vec_scale(...);
VALUE siren_vec_scale_bang(...);
VALUE siren_vec_transform(...);
VALUE siren_vec_transform_bang(...);

VALUE siren_vec_negative(...);

VALUE siren_vec_eq(...);
VALUE siren_vec_plus(...);
VALUE siren_vec_minus(...);
VALUE siren_vec_multiply_scalar(...);
VALUE siren_vec_devide_scalar(...);

#endif
