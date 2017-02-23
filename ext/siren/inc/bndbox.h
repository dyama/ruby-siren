#ifndef _BNDBOX_H_
#define _BNDBOX_H_

#include "siren.h"
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

bool siren_bndbox_install( struct RClass* rclass);
void siren_bndbox_final( void* p);
static rb_data_type_t siren_bndbox_type = { "BndBox", siren_bndbox_final };
VALUE siren_bndbox_new( const TopoDS_Shape& shape);
Bnd_Box* siren_bndbox_get( VALUE obj);
struct RClass* siren_bndbox_rclass();
#define siren_is_bndbox(obj) (DATA_TYPE(obj) == &siren_bndbox_type)

VALUE siren_bndbox_init( VALUE self);
VALUE siren_bndbox_to_s( VALUE self);
VALUE siren_bndbox_min( VALUE self);
VALUE siren_bndbox_max( VALUE self);
VALUE siren_bndbox_add( VALUE self);
VALUE siren_bndbox_is_out( VALUE self);
VALUE siren_bndbox_center( VALUE self);
VALUE siren_bndbox_xsize( VALUE self);
VALUE siren_bndbox_ysize( VALUE self);
VALUE siren_bndbox_zsize( VALUE self);
VALUE siren_bndbox_is_void( VALUE self);
VALUE siren_bndbox_is_whole( VALUE self);
VALUE siren_bndbox_void_bang( VALUE self);
VALUE siren_bndbox_whole_bang( VALUE self);
VALUE siren_bndbox_is_xthin( VALUE self);
VALUE siren_bndbox_is_ythin( VALUE self);
VALUE siren_bndbox_is_zthin( VALUE self);
VALUE siren_bndbox_is_openxmin( VALUE self);
VALUE siren_bndbox_is_openxmax( VALUE self);
VALUE siren_bndbox_is_openymin( VALUE self);
VALUE siren_bndbox_is_openymax( VALUE self);
VALUE siren_bndbox_is_openzmin( VALUE self);
VALUE siren_bndbox_is_openzmax( VALUE self);
VALUE siren_bndbox_openxmin_bang( VALUE self);
VALUE siren_bndbox_openxmax_bang( VALUE self);
VALUE siren_bndbox_openymin_bang( VALUE self);
VALUE siren_bndbox_openymax_bang( VALUE self);
VALUE siren_bndbox_openzmin_bang( VALUE self);
VALUE siren_bndbox_openzmax_bang( VALUE self);
VALUE siren_bndbox_set_gap( VALUE self);
VALUE siren_bndbox_get_gap( VALUE self);
VALUE siren_bndbox_dist( VALUE self);
VALUE siren_bndbox_square( VALUE self);

#endif
