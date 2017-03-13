#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "siren.h"
#include "vec.h"
#include "bndbox.h"
#include "trans.h"

#ifdef SIREN_ENABLE_SHHEALING
  #include "heal.h"
#endif

extern VALUE sr_cShape;

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

bool siren_shape_install();
TopoDS_Shape* siren_shape_get( VALUE obj);
VALUE siren_shape_new( const TopoDS_Shape& shape);
struct RClass* siren_shape_rclass();
#define siren_is_shape(obj) (DATA_TYPE(obj) == &siren_shape_type)

VALUE siren_shape_init(...);
VALUE siren_shape_is_null(...);
VALUE siren_shape_pos(...);
VALUE siren_shape_trans(...);
VALUE siren_shape_set_trans(...);

VALUE siren_shape_bndbox(...);

VALUE siren_shape_translate_bang(...);
VALUE siren_shape_rotate_bang(...);
VALUE siren_shape_scale_bang(...);
VALUE siren_shape_mirror_bang(...);
VALUE siren_shape_move_bang(...);
VALUE siren_shape_translate(...);
VALUE siren_shape_rotate(...);
VALUE siren_shape_scale(...);
VALUE siren_shape_mirror(...);
VALUE siren_shape_move(...);

VALUE siren_shape_hashcode(...);

VALUE siren_shape_is_partner(...);
VALUE siren_shape_is_same(...);
VALUE siren_shape_is_equal(...);

VALUE siren_shape_explore(...);
VALUE siren_shape_subshapes(...);

VALUE siren_shape_section(...);

VALUE siren_shape_reverse(...);
VALUE siren_shape_reverse_bang(...);

/* from BRepTools */
VALUE siren_shape_update_bang(...);
VALUE siren_shape_clean_bang(...);

/* flag accesors */
VALUE siren_shape_is_lock(...);
VALUE siren_shape_set_lock(...);
VALUE siren_shape_is_modify(...);
VALUE siren_shape_set_modify(...);
VALUE siren_shape_is_check(...);
VALUE siren_shape_set_check(...);
VALUE siren_shape_is_orientable(...);
VALUE siren_shape_set_orientable(...);
VALUE siren_shape_is_close(...);
VALUE siren_shape_set_close(...);
VALUE siren_shape_is_infinite(...);
VALUE siren_shape_set_infinite(...);
VALUE siren_shape_is_convex(...);
VALUE siren_shape_set_convex(...);

VALUE siren_shape_next_trans(...);
VALUE siren_shape_first_datum(...);

// VALUE siren_shape_complement(...);
// VALUE siren_shape_complement_bang(...);
// VALUE siren_shape_compose(...);
// VALUE siren_shape_compose_bang(...);
#endif
