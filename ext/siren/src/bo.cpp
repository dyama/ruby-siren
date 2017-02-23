#include "bo.h"

bool siren_bo_install( struct RClass* mod_siren)
{
  struct RClass* cls_shape = siren_shape_rclass(mrb);
  mrb_define_method(mrb, cls_shape, "common",     siren_bo_common,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_shape, "fuse",       siren_bo_fuse,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_shape, "cut",        siren_bo_cut,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls_shape, "projwire",   siren_bo_projwire,    MRB_ARGS_REQ(2));
  return true;
}

VALUE siren_bo_common( VALUE self)
{
  VALUE target;
  int argc = mrb_get_args(mrb, "o", &target);
  TopoDS_Shape* S1 = siren_shape_get(mrb, self);
  TopoDS_Shape* S2 = siren_shape_get(mrb, target);
  BRepAlgoAPI_Common api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return mrb_nil_value();
  }
  return siren_shape_new(mrb, api.Shape());
}

VALUE siren_bo_fuse( VALUE self)
{
  VALUE target;
  int argc = mrb_get_args(mrb, "o", &target);
  TopoDS_Shape* S1 = siren_shape_get(mrb, self);
  TopoDS_Shape* S2 = siren_shape_get(mrb, target);
  BRepAlgoAPI_Fuse api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return mrb_nil_value();
  }
  return siren_shape_new(mrb, api.Shape());
}

VALUE siren_bo_cut( VALUE self)
{
  VALUE target;
  int argc = mrb_get_args(mrb, "o", &target);
  TopoDS_Shape* S1 = siren_shape_get(mrb, self);
  TopoDS_Shape* S2 = siren_shape_get(mrb, target);
  BRepAlgoAPI_Cut api(*S1, *S2);
  api.Build();
  if (api.ErrorStatus()) {
    return mrb_nil_value();
  }
  return siren_shape_new(mrb, api.Shape());
}

VALUE siren_bo_projwire( VALUE self)
{
  VALUE w, v;
  int argc = mrb_get_args(mrb, "oo", &w, &v);
  TopoDS_Shape* wire = siren_shape_get(mrb, w);
  TopoDS_Shape* shape = siren_shape_get(mrb, self);
  gp_Vec* vec = siren_vec_get(mrb, v);
  BRepProj_Projection api(*wire, *shape, *vec);
  if (!api.IsDone()) {
    return mrb_nil_value();
  }
  return siren_shape_new(mrb, api.Shape());
}

