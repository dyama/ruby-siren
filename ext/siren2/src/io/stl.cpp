#include "io/stl.h"

bool siren_stl_install()
{
  rb_define_method(sr_mSiren, "load_stl", RUBY_METHOD_FUNC(siren_stl_load), -1);
  return true;
}

VALUE siren_stl_load(int argc, VALUE* argv, VALUE self)
{
  VALUE path;
  rb_scan_args(argc, argv, "1", &path);

  TopoDS_Shape shape;
  StlAPI::Read(shape, (Standard_CString)RSTRING_PTR(path));

  if (shape.IsNull()) {
    rb_raise(Qnil, "Failed to load STL from %S.", (wchar_t*)RSTRING_PTR(path));
  }

  return siren_shape_new(shape);
}
