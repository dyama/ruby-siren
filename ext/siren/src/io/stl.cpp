#include "io/stl.h"

bool siren_stl_install( struct RClass* mod_siren)
{
  // Class method
  rb_define_class_method(mod_siren, "load_stl", siren_stl_load, MRB_ARGS_REQ(1));
  // For mix-in
  rb_define_method      (mod_siren, "load_stl", siren_stl_load, MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_stl_load( VALUE self)
{
  VALUE path;
  int argc = rb_get_args("S", &path);

  TopoDS_Shape shape;
  StlAPI::Read(shape, (Standard_CString)RSTRING_PTR(path));

  if (shape.IsNull()) {
    rb_raisef(E_ARGUMENT_ERROR, "Failed to load STL from %S.", path);
  }

  return siren_shape_new(shape);
}

