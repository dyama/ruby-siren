#ifndef _VEC_H_
#define _VEC_H_

#include "siren.h"

extern VALUE sr_cVec;

bool siren_vec_install();
void siren_vec_final(void* p);
static rb_data_type_t siren_vec_type = { "Siren::Vec", siren_vec_final };
gp_Vec* siren_vec_get(VALUE obj);
struct RClass* siren_vec_rclass();
VALUE siren_vec_new(double x, double y, double z);
VALUE siren_vec_new(const gp_Vec& vec);

VALUE siren_vec_init(int, VALUE*, VALUE);
VALUE siren_vec_x(int, VALUE*, VALUE);
VALUE siren_vec_x_set(int, VALUE*, VALUE);
VALUE siren_vec_y(int, VALUE*, VALUE);
VALUE siren_vec_y_set(int, VALUE*, VALUE);
VALUE siren_vec_z(int, VALUE*, VALUE);
VALUE siren_vec_z_set(int, VALUE*, VALUE);

VALUE siren_vec_is_equal(int, VALUE*, VALUE);
VALUE siren_vec_is_normal(int, VALUE*, VALUE);
VALUE siren_vec_is_reverse(int, VALUE*, VALUE);
VALUE siren_vec_is_parallel(int, VALUE*, VALUE);
VALUE siren_vec_angle(int, VALUE*, VALUE);
VALUE siren_vec_angleref(int, VALUE*, VALUE);
VALUE siren_vec_magnitude(int, VALUE*, VALUE);
VALUE siren_vec_square_mag(int, VALUE*, VALUE);
VALUE siren_vec_cross(int, VALUE*, VALUE);
VALUE siren_vec_cross_bang(int, VALUE*, VALUE);
VALUE siren_vec_cross_mag(int, VALUE*, VALUE);
VALUE siren_vec_cross_square_mag(int, VALUE*, VALUE);
VALUE siren_vec_cross_cross(int, VALUE*, VALUE);
VALUE siren_vec_cross_cross_bang(int, VALUE*, VALUE);
VALUE siren_vec_dot(int, VALUE*, VALUE);
VALUE siren_vec_dot_cross(int, VALUE*, VALUE);
VALUE siren_vec_normal(int, VALUE*, VALUE);
VALUE siren_vec_normal_bang(int, VALUE*, VALUE);
VALUE siren_vec_reverse(int, VALUE*, VALUE);
VALUE siren_vec_reverse_bang(int, VALUE*, VALUE);
 
// VALUE siren_vec_coord(int, VALUE*, VALUE);
// VALUE siren_vec_coord_set(int, VALUE*, VALUE);
 
VALUE siren_vec_mirror(int argc, VALUE* argv, VALUE self);
VALUE siren_vec_mirror_bang(int, VALUE*, VALUE);
VALUE siren_vec_rotate(int, VALUE*, VALUE);
VALUE siren_vec_rotate_bang(int, VALUE*, VALUE);
VALUE siren_vec_scale(int, VALUE*, VALUE);
VALUE siren_vec_scale_bang(int, VALUE*, VALUE);
VALUE siren_vec_transform(int, VALUE*, VALUE);
VALUE siren_vec_transform_bang(int, VALUE*, VALUE);

VALUE siren_vec_negative(int, VALUE*, VALUE);

VALUE siren_vec_eq(int, VALUE*, VALUE);
VALUE siren_vec_plus(int, VALUE*, VALUE);
VALUE siren_vec_minus(int, VALUE*, VALUE);
VALUE siren_vec_multiply_scalar(int, VALUE*, VALUE);
VALUE siren_vec_devide_scalar(int, VALUE*, VALUE);

#endif
