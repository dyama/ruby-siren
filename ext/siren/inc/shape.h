#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "siren.h"
#include "vec.h"
#include "bndbox.h"
#include "trans.h"

#ifdef SIREN_ENABLE_SHHEALING
  #include "heal.h"
#endif

VALUE sr_cShape;

void siren_shape_final( void* p);
static rb_data_type_t siren_shape_type = { "Shape", siren_shape_final };

#include "shape/vertex.h"
#include "shape/edge.h"
#include "shape/wire.h"
#include "shape/face.h"
#include "shape/shell.h"
#include "shape/solid.h"
#include "shape/chunk.h"
#include "shape/compound.h"

#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

#include <TopExp_Explorer.hxx>

#include <TopLoc_Datum3D.hxx>

#include <BRepAlgoAPI_Section.hxx>

bool siren_shape_install( struct RClass* rclass);
TopoDS_Shape* siren_shape_get( VALUE obj);
VALUE siren_shape_new( const TopoDS_Shape& shape);
struct RClass* siren_shape_rclass();
#define siren_is_shape(obj) (DATA_TYPE(obj) == &siren_shape_type)

VALUE siren_shape_init( VALUE self);
VALUE siren_shape_is_null( VALUE self);
VALUE siren_shape_pos( VALUE self);
VALUE siren_shape_trans( VALUE self);
VALUE siren_shape_set_trans( VALUE self);

VALUE siren_shape_bndbox( VALUE self);

VALUE siren_shape_translate_bang( VALUE self);
VALUE siren_shape_rotate_bang( VALUE self);
VALUE siren_shape_scale_bang( VALUE self);
VALUE siren_shape_mirror_bang( VALUE self);
VALUE siren_shape_move_bang( VALUE self);
VALUE siren_shape_translate( VALUE self);
VALUE siren_shape_rotate( VALUE self);
VALUE siren_shape_scale( VALUE self);
VALUE siren_shape_mirror( VALUE self);
VALUE siren_shape_move( VALUE self);

VALUE siren_shape_hashcode( VALUE self);

VALUE siren_shape_is_partner( VALUE self);
VALUE siren_shape_is_same( VALUE self);
VALUE siren_shape_is_equal( VALUE self);

VALUE siren_shape_explore( VALUE self);
VALUE siren_shape_subshapes( VALUE self);

VALUE siren_shape_section( VALUE self);

VALUE siren_shape_reverse( VALUE self);
VALUE siren_shape_reverse_bang( VALUE self);

/* from BRepTools */
VALUE siren_shape_update_bang( VALUE self);
VALUE siren_shape_clean_bang( VALUE self);

/* flag accesors */
VALUE siren_shape_is_lock( VALUE self);
VALUE siren_shape_set_lock( VALUE self);
VALUE siren_shape_is_modify( VALUE self);
VALUE siren_shape_set_modify( VALUE self);
VALUE siren_shape_is_check( VALUE self);
VALUE siren_shape_set_check( VALUE self);
VALUE siren_shape_is_orientable( VALUE self);
VALUE siren_shape_set_orientable( VALUE self);
VALUE siren_shape_is_close( VALUE self);
VALUE siren_shape_set_close( VALUE self);
VALUE siren_shape_is_infinite( VALUE self);
VALUE siren_shape_set_infinite( VALUE self);
VALUE siren_shape_is_convex( VALUE self);
VALUE siren_shape_set_convex( VALUE self);

VALUE siren_shape_next_trans( VALUE self);
VALUE siren_shape_first_datum( VALUE self);

// VALUE siren_shape_complement( VALUE self);
// VALUE siren_shape_complement_bang( VALUE self);
// VALUE siren_shape_compose( VALUE self);
// VALUE siren_shape_compose_bang( VALUE self);
#endif
