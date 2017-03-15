#include "io/step.h"

#ifdef SR_ENABLE_STEP

bool siren_step_install()
{
#if 0
  // Class method
  rb_define_class_method(sr_mSiren, "save_step",  RUBY_METHOD_FUNC(siren_step_save), -1);
  rb_define_class_method(sr_mSiren, "load_step",  RUBY_METHOD_FUNC(siren_step_load), -1);
#endif
  // For mix-in
  rb_define_method      (sr_mSiren, "save_step",  RUBY_METHOD_FUNC(siren_step_save), -1);
  rb_define_method      (sr_mSiren, "load_step",  RUBY_METHOD_FUNC(siren_step_load), -1);
  return true;
}

VALUE siren_step_save(int argc, VALUE* argv, VALUE self)
{
  VALUE s;
  VALUE path;
  rb_scan_args(argc, argv, "2", &s, &path);
  TopoDS_Shape* shape = siren_shape_get(s);
  STEPControl_Writer writer;
  if (writer.Transfer(*shape, STEPControl_AsIs, Standard_True) != IFSelect_RetDone) {
    rb_raise(Qnil, "Failed to transfer Shape to STEP entity.");
  }
  if (writer.Write(RSTRING_PTR(path)) != IFSelect_RetDone) {
    rb_raise(Qnil, "Failed to write STEP to %S.", path);
  }
  return Qnil;
}

VALUE siren_step_load(int argc, VALUE* argv, VALUE self)
{
  VALUE path;
  rb_scan_args(argc, argv, "1", &path);
  STEPControl_Reader reader;
  if (!reader.ReadFile((Standard_CString)RSTRING_PTR(path))) {
     rb_raise(Qnil, "Failed to load STEP from %S.", path);
  }
  reader.NbRootsForTransfer();
  reader.TransferRoots();
  return siren_shape_new(reader.OneShape());
}

#endif
