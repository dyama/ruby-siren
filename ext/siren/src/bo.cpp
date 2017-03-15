#include "bo.h"

bool siren_bo_install()
{
  rb_define_method(sr_cShape, "common",   RUBY_METHOD_FUNC(siren_bo_common),   -1);
  rb_define_method(sr_cShape, "fuse",     RUBY_METHOD_FUNC(siren_bo_fuse),     -1);
  rb_define_method(sr_cShape, "cut",      RUBY_METHOD_FUNC(siren_bo_cut),      -1);
  rb_define_method(sr_cShape, "projwire", RUBY_METHOD_FUNC(siren_bo_projwire), -2);
  return true;
}

VALUE siren_bo_common(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  rb_scan_args(argc, argv, "1", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Common api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_fuse(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  rb_scan_args(argc, argv, "1", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Fuse api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_cut(int argc, VALUE* argv, VALUE self)
{
  VALUE target;
  rb_scan_args(argc, argv, "1", &target);
  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(target);
  BRepAlgoAPI_Cut api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}

VALUE siren_bo_projwire(int argc, VALUE* argv, VALUE self)
{
  VALUE w, v;
  rb_scan_args(argc, argv, "2", &w, &v);
  TopoDS_Shape* wire = siren_shape_get(w);
  TopoDS_Shape* shape = siren_shape_get(self);
  gp_Vec* vec = siren_vec_get(v);
  BRepProj_Projection api(*wire, *shape, *vec);
  if (!api.IsDone()) {
    return Qnil;
  }
  return siren_shape_new(api.Shape());
}