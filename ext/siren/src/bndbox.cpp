#include "bndbox.h"

VALUE siren_bndbox_new( const TopoDS_Shape& shape)
{
  VALUE obj;
  obj = rb_instance_alloc(rb_obj_value(siren_bndbox_rclass()));
  void* p = ruby_xmalloc(sizeof(Bnd_Box));
  Bnd_Box* inner = new(p) Bnd_Box();
  BRepBndLib::Add(shape, *inner);
  DATA_PTR(obj)  = inner;
  DATA_TYPE(obj) = &siren_bndbox_type;
  return obj;
}

Bnd_Box* siren_bndbox_get( VALUE obj)
{
  return static_cast<Bnd_Box*>(_get_datatype(obj, &siren_bndbox_type));
}

struct RClass* siren_bndbox_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("BndBox")));
}

bool siren_bndbox_install()
{
  struct RClass* cls_bndbox = rb_define_class_under(sr_mSiren, "BndBox", rb_cObject);
  MRB_SET_INSTANCE_TT(cls_bndbox, MRB_TT_DATA);
  rb_define_method(cls_bndbox, "initialize", siren_bndbox_init,          MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "inspect",    siren_bndbox_to_s,          MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "to_s",       siren_bndbox_to_s,          MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "min",        siren_bndbox_min,           MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "max",        siren_bndbox_max,           MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "add",        siren_bndbox_add,           MRB_ARGS_REQ(1));
  rb_define_method(cls_bndbox, "out?",       siren_bndbox_is_out,        MRB_ARGS_REQ(1));
  rb_define_method(cls_bndbox, "center",     siren_bndbox_center,        MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "xsize",      siren_bndbox_xsize,         MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "ysize",      siren_bndbox_ysize,         MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "zsize",      siren_bndbox_zsize,         MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "void?",      siren_bndbox_is_void,       MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "whole?",     siren_bndbox_is_whole,      MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "void!",      siren_bndbox_void_bang,     MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "whole!",     siren_bndbox_whole_bang,    MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "xthin?",     siren_bndbox_is_xthin,      MRB_ARGS_OPT(1));
  rb_define_method(cls_bndbox, "ythin?",     siren_bndbox_is_ythin,      MRB_ARGS_OPT(1));
  rb_define_method(cls_bndbox, "zthin?",     siren_bndbox_is_zthin,      MRB_ARGS_OPT(1));
  rb_define_method(cls_bndbox, "openxmin?",  siren_bndbox_is_openxmin,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openxmax?",  siren_bndbox_is_openxmax,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openymin?",  siren_bndbox_is_openymin,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openymax?",  siren_bndbox_is_openymax,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openzmin?",  siren_bndbox_is_openzmin,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openzmax?",  siren_bndbox_is_openzmax,   MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openxmin!",  siren_bndbox_openxmin_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openxmax!",  siren_bndbox_openxmax_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openymin!",  siren_bndbox_openymin_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openymax!",  siren_bndbox_openymax_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openzmin!",  siren_bndbox_openzmin_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "openzmax!",  siren_bndbox_openzmax_bang, MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "gap",        siren_bndbox_get_gap,       MRB_ARGS_NONE());
  rb_define_method(cls_bndbox, "gap=",       siren_bndbox_set_gap,       MRB_ARGS_REQ(1));
  rb_define_method(cls_bndbox, "dist",       siren_bndbox_dist,          MRB_ARGS_REQ(1));
  rb_define_method(cls_bndbox, "square",     siren_bndbox_square,        MRB_ARGS_NONE());
  return true;
}

VALUE siren_bndbox_init( VALUE self)
{
  void* p = ruby_xmalloc(sizeof(Bnd_Box));
  Bnd_Box* bndbox = new(p) Bnd_Box();
  DATA_PTR(self) = bndbox;
  DATA_TYPE(self) = &siren_bndbox_type;
  return self;
}

void siren_bndbox_final( void* p)
{
  Bnd_Box* pp = static_cast<Bnd_Box*>(p);
  ruby_xfree(pp);
}

VALUE siren_bndbox_to_s( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  char str[128];
  if (b->IsVoid()) {
    snprintf(str, sizeof(str), "#<BndBox:0x%x (void)>", (unsigned int)(uintptr_t)rb_cptr(self));
  }
  else {
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
    const int s = 16;
    char sxmin[s]; char sxmax[s];
    char symin[s]; char symax[s];
    char szmin[s]; char szmax[s];
    b->IsOpenXmin() ? snprintf(sxmin, s, "%s", "inf") : snprintf(sxmin, s, "%f", xmin);
    b->IsOpenXmax() ? snprintf(sxmax, s, "%s", "inf") : snprintf(sxmax, s, "%f", xmax);
    b->IsOpenYmin() ? snprintf(symin, s, "%s", "inf") : snprintf(symin, s, "%f", ymin);
    b->IsOpenYmax() ? snprintf(symax, s, "%s", "inf") : snprintf(symax, s, "%f", ymax);
    b->IsOpenZmin() ? snprintf(szmin, s, "%s", "inf") : snprintf(szmin, s, "%f", zmin);
    b->IsOpenZmax() ? snprintf(szmax, s, "%s", "inf") : snprintf(szmax, s, "%f", zmax);
    snprintf(str, sizeof(str),
        "#<BndBox:0x%x xmin=%s, ymin=%s, zmin=%s, xmax=%s, ymax=%s, zmax=%s>",
        (unsigned int)(uintptr_t)rb_cptr(self), sxmin, symin, szmin, sxmax, symax, szmax);
  }
  return rb_str_new_cstr(str);
}

VALUE siren_bndbox_min( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  return siren_pnt_to_ary(b->CornerMin());
}

VALUE siren_bndbox_max( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  return siren_pnt_to_ary(b->CornerMax());
}

VALUE siren_bndbox_add( VALUE self)
{
  VALUE obj;
  int argc = rb_scan_args("o", &obj);

  Bnd_Box* b = siren_bndbox_get(self);

  if (siren_is_shape(obj)) {
    TopoDS_Shape* s = siren_shape_get(obj);
    BRepBndLib::Add(*s, *b);
  }
  else if (siren_is_bndbox(obj)) {
    Bnd_Box* bb = siren_bndbox_get(obj);
    b->Add(*bb);
  }
  else if (_array_p(obj)) {
    b->Add(siren_ary_to_pnt(obj));
  }

  return Qnil;
}

VALUE siren_bndbox_is_out( VALUE self)
{
  VALUE other;
  int argc = rb_scan_args("o", &other);
  Bnd_Box* b = siren_bndbox_get(self);
  // return b->IsOut(siren_pnt_get(other)) == Standard_True ? Qtrue : Qfalse;
  return b->IsOut(*siren_bndbox_get(other)) == Standard_True ? Qtrue : Qfalse;
}

VALUE siren_bndbox_center( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return siren_pnt_new((xmax - xmin) / 2.0, (ymax - ymin) / 2.0, (zmax - zmin) / 2.0);
}

VALUE siren_bndbox_xsize( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return (xmax - xmin);
}

VALUE siren_bndbox_ysize( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return (ymax - ymin);
}

VALUE siren_bndbox_zsize( VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return (zmax - zmin);
}

VALUE siren_bndbox_is_void( VALUE self)
{
  return siren_bndbox_get(self)->IsVoid() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_whole( VALUE self)
{
  return siren_bndbox_get(self)->IsWhole() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_void_bang( VALUE self)
{
  siren_bndbox_get(self)->SetVoid();
  return Qnil;
}

VALUE siren_bndbox_whole_bang( VALUE self)
{
  siren_bndbox_get(self)->SetWhole();
  return Qnil;
}

VALUE siren_bndbox_is_xthin( VALUE self)
{
  VALUE tol;
  int argc = rb_scan_args("|f", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsXThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_ythin( VALUE self)
{
  VALUE tol;
  int argc = rb_scan_args("|f", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsYThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_zthin( VALUE self)
{
  VALUE tol;
  int argc = rb_scan_args("|f", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsZThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openxmin( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenXmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openxmax( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenXmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openymin( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenYmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openymax( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenYmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openzmin( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenZmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openzmax( VALUE self)
{
  return siren_bndbox_get(self)->IsOpenZmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_openxmin_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenXmin();
  return Qnil;
}

VALUE siren_bndbox_openxmax_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenXmax();
  return Qnil;
}

VALUE siren_bndbox_openymin_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenYmin();
  return Qnil;
}

VALUE siren_bndbox_openymax_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenYmax();
  return Qnil;
}

VALUE siren_bndbox_openzmin_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenZmin();
  return Qnil;
}

VALUE siren_bndbox_openzmax_bang( VALUE self)
{
  siren_bndbox_get(self)->OpenZmax();
  return Qnil;
}

VALUE siren_bndbox_set_gap( VALUE self)
{
  VALUE tol;
  int argc = rb_scan_args("f", &tol);
  siren_bndbox_get(self)->SetGap(tol);
  return Qnil;
}

VALUE siren_bndbox_get_gap( VALUE self)
{
  Standard_Real tol = siren_bndbox_get(self)->GetGap();
  return (tol);
}

VALUE siren_bndbox_dist( VALUE self)
{
  VALUE other;
  int argc = rb_scan_args("o", &other);
  Bnd_Box* b = siren_bndbox_get(self);
  Bnd_Box* bb= siren_bndbox_get(other);
  if (b->IsVoid() || bb->IsVoid()) {
    return Qnil;
  }
  Standard_Real value = b->Distance(*bb);
  return (value);
}

VALUE siren_bndbox_square( VALUE self)
{
  Standard_Real value = siren_bndbox_get(self)->SquareExtent();
  return (value);
}

