#ifndef _BNDBOX_H_
#define _BNDBOX_H_

#include "siren.h"
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

extern VALUE sr_cBndBox;

bool siren_bndbox_install();
void siren_bndbox_final( void* p);
static rb_data_type_t siren_bndbox_type = { "BndBox", siren_bndbox_final };
VALUE siren_bndbox_new( const TopoDS_Shape& shape);
Bnd_Box* siren_bndbox_get(VALUE obj);
#define siren_is_bndbox(obj) (DATA_TYPE(obj) == &siren_bndbox_type)

VALUE siren_bndbox_init(int, VALUE*, VALUE);
VALUE siren_bndbox_to_s(int, VALUE*, VALUE);
VALUE siren_bndbox_min(int, VALUE*, VALUE);
VALUE siren_bndbox_max(int, VALUE*, VALUE);
VALUE siren_bndbox_add(int, VALUE*, VALUE);
VALUE siren_bndbox_is_out(int, VALUE*, VALUE);
VALUE siren_bndbox_center(int, VALUE*, VALUE);
VALUE siren_bndbox_xsize(int, VALUE*, VALUE);
VALUE siren_bndbox_ysize(int, VALUE*, VALUE);
VALUE siren_bndbox_zsize(int, VALUE*, VALUE);
VALUE siren_bndbox_is_void(int, VALUE*, VALUE);
VALUE siren_bndbox_is_whole(int, VALUE*, VALUE);
VALUE siren_bndbox_void_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_whole_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_is_xthin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_ythin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_zthin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openxmin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openxmax(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openymin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openymax(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openzmin(int, VALUE*, VALUE);
VALUE siren_bndbox_is_openzmax(int, VALUE*, VALUE);
VALUE siren_bndbox_openxmin_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_openxmax_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_openymin_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_openymax_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_openzmin_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_openzmax_bang(int, VALUE*, VALUE);
VALUE siren_bndbox_set_gap(int, VALUE*, VALUE);
VALUE siren_bndbox_get_gap(int, VALUE*, VALUE);
VALUE siren_bndbox_dist(int, VALUE*, VALUE);
VALUE siren_bndbox_square(int, VALUE*, VALUE);

#endif
