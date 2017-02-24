#include "io/iges.h"

#ifdef SIREN_ENABLE_IGES

bool siren_iges_install()
{
  // Class method
  rb_define_class_method(sr_mSiren, "save_iges", siren_iges_save, MRB_ARGS_REQ(2));
  rb_define_class_method(sr_mSiren, "load_iges", siren_iges_load, MRB_ARGS_REQ(1));
  // For mix-in
  rb_define_method      (sr_mSiren, "save_iges", siren_iges_save, MRB_ARGS_REQ(2));
  rb_define_method      (sr_mSiren, "load_iges", siren_iges_load, MRB_ARGS_REQ(1));
  return true;
}

VALUE siren_iges_save( VALUE self)
{
  VALUE target;
  VALUE path;
  rb_scan_args(argc, argv, "oS", &target, &path);

  IGESControl_Controller::Init();
//  IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"),
//    Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
  IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"), 1);  // the second argument "1" sets the OCCT => IGES conversion method to "BRep"  

  writer.AddShape(*siren_shape_get(target));
  writer.ComputeModel();

  std::ofstream fst(RSTRING_PTR(path), std::ios_base::out);
  if (writer.Write(fst) == Standard_False) {
    rb_raisef(Qnil, "Failed to save IGES to %S.", path);
  }

  return Qnil;
}

VALUE siren_iges_load( VALUE self)
{
  VALUE path;
  VALUE as_ary = FALSE;
  rb_scan_args(argc, argv, "S|b", &path, &as_ary);

  IGESControl_Reader iges_reader;
  int stat = iges_reader.ReadFile((Standard_CString)RSTRING_PTR(path));
  VALUE result;

  if (stat == IFSelect_RetDone) {
    try {
      iges_reader.TransferRoots();
    }
    catch (...) {
      rb_raise(E_RUNTIME_ERROR, "Failed to TransferRoots() with an IGES.");
    }

    if (as_ary) {
      // Return array
      result = rb_ary_new();
      for (int i=1; i <= iges_reader.NbShapes(); i++) {
        try {
          TopoDS_Shape shape = iges_reader.Shape(i);
          VALUE mrshape = siren_shape_new(shape);
          rb_ary_push(result, mrshape);
        }
        catch(...) {
          rb_warn("Failed to get entitiy at %d.", i);
        }
      }
      if (_ary_len(result) < 1) {
        result = Qnil;
      }
    }
    else {
      // As one shape
      result = siren_shape_new(iges_reader.OneShape());
    }
  }
  else {
    rb_raisef(Qnil, "Failed to load IGES from %S.", path);
  }
  return result;
}

#endif
