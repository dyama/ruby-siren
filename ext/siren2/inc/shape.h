#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "siren.h"
#include "vec.h"
#include "bndbox.h"
#include "trans.h"

#ifdef SR_ENABLE_SHHEALING
  #include "heal.h"
#endif

extern VALUE sr_cShape;
extern const rb_data_type_t siren_shape_type;

VALUE siren_shape_allocate(VALUE);
void siren_shape_final(void* p);

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

bool siren_shape_p(const VALUE&);
void siren_shape_check(const VALUE&);

VALUE siren_shape_init(int, VALUE*, VALUE);
VALUE siren_shape_is_null(int, VALUE*, VALUE);
VALUE siren_shape_pos(int, VALUE*, VALUE);
VALUE siren_shape_trans(int, VALUE*, VALUE);
VALUE siren_shape_set_trans(int, VALUE*, VALUE);

VALUE siren_shape_bndbox(int, VALUE*, VALUE);

VALUE siren_shape_translate_bang(int, VALUE*, VALUE);
VALUE siren_shape_rotate_bang(int, VALUE*, VALUE);
VALUE siren_shape_scale_bang(int, VALUE*, VALUE);
VALUE siren_shape_mirror_bang(int, VALUE*, VALUE);
VALUE siren_shape_move_bang(int, VALUE*, VALUE);
VALUE siren_shape_translate(int, VALUE*, VALUE);
VALUE siren_shape_rotate(int, VALUE*, VALUE);
VALUE siren_shape_scale(int, VALUE*, VALUE);
VALUE siren_shape_mirror(int, VALUE*, VALUE);
VALUE siren_shape_move(int, VALUE*, VALUE);

VALUE siren_shape_hashcode(int, VALUE*, VALUE);

VALUE siren_shape_is_partner(int, VALUE*, VALUE);
VALUE siren_shape_is_same(int, VALUE*, VALUE);
VALUE siren_shape_is_equal(int, VALUE*, VALUE);

VALUE siren_shape_explore(int, VALUE*, VALUE);
VALUE siren_shape_subshapes(int, VALUE*, VALUE);

VALUE siren_shape_section(int, VALUE*, VALUE);

VALUE siren_shape_reverse(int, VALUE*, VALUE);
VALUE siren_shape_reverse_bang(int, VALUE*, VALUE);

/* from BRepTools */
VALUE siren_shape_update_bang(int, VALUE*, VALUE);
VALUE siren_shape_clean_bang(int, VALUE*, VALUE);

/* flag accesors */
VALUE siren_shape_is_lock(int, VALUE*, VALUE);
VALUE siren_shape_set_lock(int, VALUE*, VALUE);
VALUE siren_shape_is_modify(int, VALUE*, VALUE);
VALUE siren_shape_set_modify(int, VALUE*, VALUE);
VALUE siren_shape_is_check(int, VALUE*, VALUE);
VALUE siren_shape_set_check(int, VALUE*, VALUE);
VALUE siren_shape_is_orientable(int, VALUE*, VALUE);
VALUE siren_shape_set_orientable(int, VALUE*, VALUE);
VALUE siren_shape_is_close(int, VALUE*, VALUE);
VALUE siren_shape_set_close(int, VALUE*, VALUE);
VALUE siren_shape_is_infinite(int, VALUE*, VALUE);
VALUE siren_shape_set_infinite(int, VALUE*, VALUE);
VALUE siren_shape_is_convex(int, VALUE*, VALUE);
VALUE siren_shape_set_convex(int, VALUE*, VALUE);

VALUE siren_shape_next_trans(int, VALUE*, VALUE);
VALUE siren_shape_first_datum(int, VALUE*, VALUE);

// VALUE siren_shape_complement(int, VALUE*, VALUE);
// VALUE siren_shape_complement_bang(int, VALUE*, VALUE);
// VALUE siren_shape_compose(int, VALUE*, VALUE);
// VALUE siren_shape_compose_bang(int, VALUE*, VALUE);

#define SR_SHAPE_GET(OCCT,SRT) \
  TopoDS_##OCCT siren_##SRT##_get(VALUE self) \
  { \
    TopoDS_Shape* shape = siren_shape_get(self); \
    TopoDS_##OCCT res = TopoDS::OCCT(*shape); \
    if (res.IsNull()) { \
      rb_raise(Qnil, "The geometry type is not " #SRT "."); \
    } \
    return res; \
  }

#define SR_SHAPE_DEFINE(CLASS) \
  sr_c##CLASS = rb_define_class_under(sr_mSiren, #CLASS, sr_cShape); \
  rb_define_alloc_func(sr_c##CLASS, siren_shape_allocate);

#define SR_SHAPE_CHECK(CLASSU, CLASSL) bool siren_##CLASSL##_p(const VALUE& target) \
  { \
    return rb_funcall(target, rb_intern("is_a?"), 1, sr_c##CLASSU) == Qtrue; \
  } \
  void siren_##CLASSL##_check(const VALUE& target) \
  { \
    if (!siren_##CLASSL##_p(target)) { \
      VALUE type = rb_funcall(target, rb_intern("class"), 0); \
      VALUE type_str = rb_funcall(type, rb_intern("to_s"), 0); \
      VALUE etype_str = rb_funcall(sr_c##CLASSU, rb_intern("to_s"), 0); \
      rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)", \
          RSTRING_PTR(type_str), RSTRING_PTR(etype_str)); \
    } \
  }

#define SR_SHAPE_INIT(O,L) \
  SR_SHAPE_GET(O,L) \
  SR_SHAPE_CHECK(O, L)

#endif
