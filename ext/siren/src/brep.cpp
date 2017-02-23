#include "brep.h"

bool siren_brep_install( struct RClass* sr_mSiren)
{
  // Class method
  rb_define_class_method(sr_mSiren, "save_brep", siren_brep_save, MRB_ARGS_REQ(2));
  rb_define_class_method(sr_mSiren, "load_brep", siren_brep_load, MRB_ARGS_REQ(1));
  rb_define_class_method(sr_mSiren, "dump",      siren_brep_dump, MRB_ARGS_REQ(1));
  // For mix-in
  rb_define_method      (sr_mSiren, "save_brep", siren_brep_save, MRB_ARGS_REQ(2));
  rb_define_method      (sr_mSiren, "load_brep", siren_brep_load, MRB_ARGS_REQ(1));
  rb_define_method      (sr_mSiren, "dump",      siren_brep_dump, MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_brep_save( VALUE self)
{
  VALUE target;
  VALUE path;
  int argc = rb_scan_args("oS", &target, &path);
  TopoDS_Shape* shape = siren_shape_get(target);
  try {
    std::ofstream fst(RSTRING_PTR(path), std::ios_base::out);
    BRepTools::Write(*shape, fst);
  }
  catch (...) {
    rb_raisef(E_ARGUMENT_ERROR, "Failed to save BRep to %S.", path);
  }
  return Qnil;
}

VALUE siren_brep_load( VALUE self)
{
  VALUE path;
  int argc = rb_scan_args("S", &path);
  BRep_Builder B;
  TopoDS_Shape shape;
  try {
    std::ifstream fst(RSTRING_PTR(path), std::ios_base::in);
    BRepTools::Read(shape, fst, B);
  }
  catch (...) {
    rb_raisef(E_ARGUMENT_ERROR, "Failed to load BRep from %S.", path);
  }
  return siren_shape_new(shape);
}

VALUE siren_brep_dump( VALUE self)
{
  VALUE target;
  int argc = rb_scan_args("o", &target);
  TopoDS_Shape* shape = siren_shape_get(target);
  BRepTools::Dump(*shape, std::cout);
  return Qnil;
}

