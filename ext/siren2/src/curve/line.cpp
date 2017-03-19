/**
 * line.cpp
 * Implementation of singleton methods for LINE
 */

#include "curve.h"

VALUE sr_cLine;

SR_CURVE_GET(Line, line)

bool siren_line_install()
{
  SR_CURVE_INIT(Line)
  rb_define_method(sr_cLine, "initialize", RUBY_METHOD_FUNC(siren_curve_init), -1);
  rb_define_method(sr_cLine, "dir",        RUBY_METHOD_FUNC(siren_line_dir),   -1);
  return true;
}

VALUE siren_line_dir(int argc, VALUE* argv, VALUE self)
{
  handle<Geom_Line> line = siren_line_get(self);
  return siren_dir_to_ary(line->Lin().Direction());
}
