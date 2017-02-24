#include "brep.h"

bool siren_brep_install()
{
#if 0
  // Class method
  rb_define_class_method(sr_mSiren, "save_brep", siren_brep_save, MRB_ARGS_REQ(2));
  rb_define_class_method(sr_mSiren, "load_brep", siren_brep_load, MRB_ARGS_REQ(1));
  rb_define_class_method(sr_mSiren, "dump",      siren_brep_dump, MRB_ARGS_REQ(1));
#endif
  // For mix-in
  rb_define_method(sr_mSiren, "save_brep", siren_brep_save, -1);
  rb_define_method(sr_mSiren, "load_brep", siren_brep_load, -1);
  rb_define_method(sr_mSiren, "dump",      siren_brep_dump, -1);
  return true;
}

VALUE siren_brep_save(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  VALUE path;
  rb_scan_args(argc, argv, "2", &target, &path);
  TopoDS_Shape* shape = siren_shape_get(target);
  try {
    std::ofstream fst(RSTRING_PTR(path), std::ios_base::out);
    BRepTools::Write(*shape, fst);
  }
  catch (...) {
    rb_raise(rb_eSystemCallError, "Failed to save BRep to %S.", (wchar_t*)StringValuePtr(path));
  }
  return Qnil;
}

VALUE siren_brep_load(int argc, VALUE* argv, VALUE self)
{
  VALUE path;
  rb_scan_args(argc, argv, "1", &path);
  BRep_Builder B;
  TopoDS_Shape shape;
  try {
    std::ifstream fst(RSTRING_PTR(path), std::ios_base::in);
    BRepTools::Read(shape, fst, B);
  }
  catch (...) {
    rb_raise(rb_eSystemCallError, "Failed to load BRep from %S.", (wchar_t*)StringValuePtr(path));
  }
  return siren_shape_new(shape);
}

VALUE siren_brep_dump(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  rb_scan_args(argc, argv, "1", &target);
  TopoDS_Shape* shape = siren_shape_get(target);
  BRepTools::Dump(*shape, std::cout);
  return Qnil;
}

