/**
 * line.cpp
 * Implementation of singleton methods for LINE
 */

#include "curve.h"

VALUE sr_cLine;

SR_CURVE_GET(Line, line)

bool siren_line_install()
{
#if 0
  struct RClass* cls_curve = siren_curve_rclass();
  struct RClass* cls_line = rb_define_class_under(sr_mSiren, "Line", cls_curve);
  MRB_SET_INSTANCE_TT(cls_line, MRB_TT_DATA);
#endif
  rb_define_method(sr_cLine, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  rb_define_method(sr_cLine, "dir",        RUBY_METHOD_FUNC(siren_line_dir),   -1);
  return true;
}

VALUE siren_line_dir(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Line> line = siren_line_get(self);
  return siren_dir_to_ary(line->Lin().Direction());
}
