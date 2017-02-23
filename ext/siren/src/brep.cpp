#include "brep.h"

bool siren_brep_install( struct RClass* mod_siren)
{
  // Class method
  mrb_define_class_method(mrb, mod_siren, "save_brep", siren_brep_save, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, mod_siren, "load_brep", siren_brep_load, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, mod_siren, "dump",      siren_brep_dump, MRB_ARGS_REQ(1));
  // For mix-in
  mrb_define_method      (mrb, mod_siren, "save_brep", siren_brep_save, MRB_ARGS_REQ(2));
  mrb_define_method      (mrb, mod_siren, "load_brep", siren_brep_load, MRB_ARGS_REQ(1));
  mrb_define_method      (mrb, mod_siren, "dump",      siren_brep_dump, MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_brep_save( VALUE self)
{
  VALUE target;
  VALUE path;
  int argc = mrb_get_args(mrb, "oS", &target, &path);
  TopoDS_Shape* shape = siren_shape_get(mrb, target);
  try {
    std::ofstream fst(RSTRING_PTR(path), std::ios_base::out);
    BRepTools::Write(*shape, fst);
  }
  catch (...) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Failed to save BRep to %S.", path);
  }
  return mrb_nil_value();
}

VALUE siren_brep_load( VALUE self)
{
  VALUE path;
  int argc = mrb_get_args(mrb, "S", &path);
  BRep_Builder B;
  TopoDS_Shape shape;
  try {
    std::ifstream fst(RSTRING_PTR(path), std::ios_base::in);
    BRepTools::Read(shape, fst, B);
  }
  catch (...) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Failed to load BRep from %S.", path);
  }
  return siren_shape_new(mrb, shape);
}

VALUE siren_brep_dump( VALUE self)
{
  VALUE target;
  int argc = mrb_get_args(mrb, "o", &target);
  TopoDS_Shape* shape = siren_shape_get(mrb, target);
  BRepTools::Dump(*shape, std::cout);
  return mrb_nil_value();
}

