#ifndef _VEC_H_
#define _VEC_H_

#include "siren.h"

bool siren_vec_install( struct RClass* rclass);
void siren_vec_final( void* p);
static rb_data_type_t siren_vec_type = { "Vec", siren_vec_final };
gp_Vec* siren_vec_get( VALUE obj);
struct RClass* siren_vec_rclass();
VALUE siren_vec_new( double x, double y, double z);
VALUE siren_vec_new( const gp_Vec& vec);

VALUE siren_vec_init( VALUE self);
VALUE siren_vec_x( VALUE self);
VALUE siren_vec_x_set( VALUE self);
VALUE siren_vec_y( VALUE self);
VALUE siren_vec_y_set( VALUE self);
VALUE siren_vec_z( VALUE self);
VALUE siren_vec_z_set( VALUE self);

VALUE siren_vec_is_equal( VALUE self);
VALUE siren_vec_is_normal( VALUE self);
VALUE siren_vec_is_reverse( VALUE self);
VALUE siren_vec_is_parallel( VALUE self);
VALUE siren_vec_angle( VALUE self);
VALUE siren_vec_angleref( VALUE self);
VALUE siren_vec_magnitude( VALUE self);
VALUE siren_vec_square_mag( VALUE self);
VALUE siren_vec_cross( VALUE self);
VALUE siren_vec_cross_bang( VALUE self);
VALUE siren_vec_cross_mag( VALUE self);
VALUE siren_vec_cross_square_mag( VALUE self);
VALUE siren_vec_cross_cross( VALUE self);
VALUE siren_vec_cross_cross_bang( VALUE self);
VALUE siren_vec_dot( VALUE self);
VALUE siren_vec_dot_cross( VALUE self);
VALUE siren_vec_normal( VALUE self);
VALUE siren_vec_normal_bang( VALUE self);
VALUE siren_vec_reverse( VALUE self);
VALUE siren_vec_reverse_bang( VALUE self);
 
// VALUE siren_vec_coord( VALUE self);
// VALUE siren_vec_coord_set( VALUE self);
 
VALUE siren_vec_mirror( VALUE self);
VALUE siren_vec_mirror_bang( VALUE self);
VALUE siren_vec_rotate( VALUE self);
VALUE siren_vec_rotate_bang( VALUE self);
VALUE siren_vec_scale( VALUE self);
VALUE siren_vec_scale_bang( VALUE self);
VALUE siren_vec_transform( VALUE self);
VALUE siren_vec_transform_bang( VALUE self);

VALUE siren_vec_negative( VALUE self);

VALUE siren_vec_eq( VALUE self);
VALUE siren_vec_plus( VALUE self);
VALUE siren_vec_minus( VALUE self);
VALUE siren_vec_multiply_scalar( VALUE self);
VALUE siren_vec_devide_scalar( VALUE self);

#endif
