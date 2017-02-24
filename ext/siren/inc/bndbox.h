#ifndef _BNDBOX_H_
#define _BNDBOX_H_

#include "siren.h"
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

VALUE sr_cBndBox;

bool siren_bndbox_install( struct RClass* rclass);
void siren_bndbox_final( void* p);
static rb_data_type_t siren_bndbox_type = { "BndBox", siren_bndbox_final };
VALUE siren_bndbox_new( const TopoDS_Shape& shape);
Bnd_Box* siren_bndbox_get( VALUE obj);
struct RClass* siren_bndbox_rclass();
#define siren_is_bndbox(obj) (DATA_TYPE(obj) == &siren_bndbox_type)

VALUE siren_bndbox_init(...);
VALUE siren_bndbox_to_s(...);
VALUE siren_bndbox_min(...);
VALUE siren_bndbox_max(...);
VALUE siren_bndbox_add(...);
VALUE siren_bndbox_is_out(...);
VALUE siren_bndbox_center(...);
VALUE siren_bndbox_xsize(...);
VALUE siren_bndbox_ysize(...);
VALUE siren_bndbox_zsize(...);
VALUE siren_bndbox_is_void(...);
VALUE siren_bndbox_is_whole(...);
VALUE siren_bndbox_void_bang(...);
VALUE siren_bndbox_whole_bang(...);
VALUE siren_bndbox_is_xthin(...);
VALUE siren_bndbox_is_ythin(...);
VALUE siren_bndbox_is_zthin(...);
VALUE siren_bndbox_is_openxmin(...);
VALUE siren_bndbox_is_openxmax(...);
VALUE siren_bndbox_is_openymin(...);
VALUE siren_bndbox_is_openymax(...);
VALUE siren_bndbox_is_openzmin(...);
VALUE siren_bndbox_is_openzmax(...);
VALUE siren_bndbox_openxmin_bang(...);
VALUE siren_bndbox_openxmax_bang(...);
VALUE siren_bndbox_openymin_bang(...);
VALUE siren_bndbox_openymax_bang(...);
VALUE siren_bndbox_openzmin_bang(...);
VALUE siren_bndbox_openzmax_bang(...);
VALUE siren_bndbox_set_gap(...);
VALUE siren_bndbox_get_gap(...);
VALUE siren_bndbox_dist(...);
VALUE siren_bndbox_square(...);

#endif
