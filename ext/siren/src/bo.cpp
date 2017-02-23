#include "bo.h"

bool siren_bo_install()
{
  struct RClass* cls_shape = siren_shape_rclass();
  rb_define_method(cls_shape, "common",     siren_bo_common,      MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "fuse",       siren_bo_fuse,        MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "cut",        siren_bo_cut,         MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "projwire",   siren_bo_projwire,    MRB_ARGS_REQ(2));
  return true;
}

VALUE siren_bo_common( VALUE self)
{
  VALUE target;
  int argc = rb_scan_args("o", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Common api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_fuse( VALUE self)
{
  VALUE target;
  int argc = rb_scan_args("o", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Fuse api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_cut( VALUE self)
{
  VALUE target;
  int argc = rb_scan_args("o", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Cut api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_projwire( VALUE self)
{
  VALUE w, v;
  int argc = rb_scan_args("oo", &w, &v);
  TopoDS_Shape* wire = siren_shape_get(w);
  TopoDS_Shape* shape = siren_shape_get(self);
  gp_Vec* vec = siren_vec_get(v);
  BRepProj_Projection api(*wire, *shape, *vec);
  if (!api.IsDone()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

