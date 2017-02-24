#include "io/stl.h"

bool siren_stl_install()
{
  // Class method
  rb_define_class_method(sr_mSiren, "load_stl", siren_stl_load, MRB_ARGS_REQ(1));
  // For mix-in
  rb_define_method      (sr_mSiren, "load_stl", siren_stl_load, MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_stl_load( VALUE self)
{
  VALUE path;
  rb_scan_args(argc, argv, "S", &path);

  TopoDS_Shape shape;
  StlAPI::Read(shape, (Standard_CString)RSTRING_PTR(path));

  if (shape.IsNull()) {
    rb_raisef(Qnil, "Failed to load STL from %S.", path);
  }

  return siren_shape_new(shape);
}

