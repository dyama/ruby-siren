/**
 * line.cpp
 * Implementation of singleton methods for LINE
 */

#include "curve.h"

VALUE sr_cLine;

VALUE siren_line_new( const handle<Geom_Curve>* curve)
{
  VALUE obj;
  obj = rb_instance_alloc(sr_cLine);
  void* p = ruby_xmalloc(sizeof(handle<Geom_Curve>));
  handle<Geom_Curve>* hgcurve = new(p) handle<Geom_Curve>();
  *hgcurve = *curve;
  DATA_PTR(obj) = hgcurve;
#if 0
  DATA_TYPE(obj) = &siren_line_type;
#endif
  return obj;
}

handle<Geom_Line> siren_line_get(VALUE self)
{
#if 0
  handle<Geom_Curve> hgc = *static_cast<handle<Geom_Curve>*>(_get_datatype(self, &siren_line_type));
  if (hgc.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Curve."); }
  handle<Geom_Line> line = handle<Geom_Line>::DownCast(hgc);
  if (line.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Line."); }
  return line;
#else
  handle<Geom_Curve> hgc;
  Data_Get_Struct(self, Geom_Curve, hgc);
  if (hgc.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Curve.");
  }
  handle<Geom_Line> line = handle<Geom_Line>::DownCast(hgc);
  if (line.IsNull()) {
    rb_raise(Qnil, "The geometry type is not Line.");
  }
  return line;
#endif
}

bool siren_line_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_line = rb_define_class_under(sr_mSiren, "Line", cls_curve);
  MRB_SET_INSTANCE_TT(cls_line, MRB_TT_DATA);
#endif
  rb_define_method(sr_cLine, "initialize", siren_curve_init, -1);
  rb_define_method(sr_cLine, "dir",        siren_line_dir,   -1);
  return true;
}

VALUE siren_line_dir(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Line> line = siren_line_get(self);
  return siren_dir_to_ary(line->Lin().Direction());
}

