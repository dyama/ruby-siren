#include "brep.h"

bool siren_brep_install()
{
#if 0
  // Class method
  rb_define_class_method(sr_mSiren, "save_brep", RUBY_METHOD_FUNC(siren_brep_save), MRB_ARGS_REQ(2));
  rb_define_class_method(sr_mSiren, "load_brep", RUBY_METHOD_FUNC(siren_brep_load), MRB_ARGS_REQ(1));
  rb_define_class_method(sr_mSiren, "dump",      RUBY_METHOD_FUNC(siren_brep_dump), MRB_ARGS_REQ(1));
#endif
  // For mix-in
  rb_define_method(sr_mSiren, "save_brep", RUBY_METHOD_FUNC(siren_brep_save), -1);
  rb_define_method(sr_mSiren, "load_brep", RUBY_METHOD_FUNC(siren_brep_load), -1);
  rb_define_method(sr_cShape, "dump",      RUBY_METHOD_FUNC(siren_brep_dump), -1);
  return true;
}

VALUE siren_brep_save(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  VALUE path;
  rb_scan_args(argc, argv, "2", &target, &path);
  siren_shape_check(target);
  Check_Type(path, T_STRING);
  if (NUM2INT(rb_funcall(path, rb_intern("size"), 0)) == 0) {
    rb_raise(rb_eArgError, "Specified path has no charactors.");
  }
  TopoDS_Shape* shape = siren_shape_get(target);
  try {
    std::ofstream fst(RSTRING_PTR(path), std::ios_base::out);
    if (fst.good()) {
      BRepTools::Write(*shape, fst);
    }
  }
  catch (...) {
    rb_raise(rb_eIOError, "Failed to save BRep to %s.", RSTRING_PTR(path));
  }
  return Qnil;
}

VALUE siren_brep_load(int argc, VALUE* argv, VALUE self)
{
  VALUE path;
  rb_scan_args(argc, argv, "1", &path);
  Check_Type(path, T_STRING);
  if (rb_funcall(rb_cFile, rb_intern("exist?"), 1, path) == Qfalse) {
    rb_raise(rb_eIOError, "No such file at %s.", RSTRING_PTR(path));
  }
  BRep_Builder B;
  TopoDS_Shape shape;
  try {
    std::ifstream fst(RSTRING_PTR(path), std::ios_base::in);
    if (fst.good()) {
      BRepTools::Read(shape, fst, B);
    }
  }
  catch (...) {
    rb_raise(rb_eIOError, "Failed to load BRep from %s.", RSTRING_PTR(path));
  }
  return siren_shape_new(shape);
}

VALUE siren_brep_dump(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  TopoDS_Shape* shape = siren_shape_get(self);
  BRepTools::Dump(*shape, std::cout);
  return Qnil;
}
