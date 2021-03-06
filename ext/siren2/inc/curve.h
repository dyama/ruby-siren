#ifndef _CURVE_H_
#define _CURVE_H_

#include "siren.h"

#include <Geom_Curve.hxx>
#include <GeomAbs_CurveType.hxx>

extern VALUE sr_cCurve;
extern const rb_data_type_t siren_curve_type;

VALUE siren_curve_allocate(VALUE);
VALUE siren_curve_new(handle<Geom_Curve> curve);

void siren_curve_final(void* p);

#include "curve/line.h"
#include "curve/circle.h"
#include "curve/ellipse.h"
#include "curve/hyperbola.h"
#include "curve/parabola.h"
#include "curve/bzcurve.h"
#include "curve/bscurve.h"
#include "curve/offsetcurve.h"

inline GeomAbs_CurveType siren_curve_geomtype_native(handle<Geom_Curve> hgc)
{
  handle<Standard_Type> type = hgc->DynamicType();
  /* Geom_BoundedCurve */
  if (STANDARD_TYPE(Geom_BezierCurve) == type)  { return GeomAbs_BezierCurve; }
  if (STANDARD_TYPE(Geom_BSplineCurve) == type) { return GeomAbs_BSplineCurve; }
  /* Geom_Conic */
  if (STANDARD_TYPE(Geom_Circle) == type)       { return GeomAbs_Circle; }
  if (STANDARD_TYPE(Geom_Ellipse) == type)      { return GeomAbs_Ellipse; }
  if (STANDARD_TYPE(Geom_Hyperbola) == type)    { return GeomAbs_Hyperbola; }
  if (STANDARD_TYPE(Geom_Parabola) == type)     { return GeomAbs_Parabola; }
  /* Line */
  if (STANDARD_TYPE(Geom_Line) == type)         { return GeomAbs_Line; }
  return GeomAbs_OtherCurve;
}

bool siren_curve_install();
handle<Geom_Curve>* siren_curve_get(VALUE obj);
struct RClass* siren_curve_rclass();

VALUE siren_curve_new(handle<Geom_Curve> curve);
VALUE siren_curve_init(int, VALUE*, VALUE);

#define SR_CURVE_GET(OCCT,SRT) \
  handle<Geom_##OCCT> siren_##SRT##_get(VALUE self) \
  { \
    auto curve = siren_curve_get(self); \
    auto res = handle<Geom_##OCCT>::DownCast(*curve); \
    if (res.IsNull()) { \
      rb_raise(Qnil, "The geometry type is not " #SRT "."); \
    } \
    return res; \
  }

#define SR_CURVE_CHECK(CLASSU, CLASSL) bool siren_##CLASSL##_p(const VALUE& target) \
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

#define SR_CURVE_DEFINE(CLASS) \
  sr_c##CLASS = rb_define_class_under(sr_mSiren, #CLASS, sr_cCurve); \
  rb_define_alloc_func(sr_c##CLASS, siren_curve_allocate);

#define SR_CURVE_INIT(O,U,L) \
  SR_CURVE_GET(O,L) \
  SR_CURVE_CHECK(U, L)

#endif
