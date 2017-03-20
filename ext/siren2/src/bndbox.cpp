#include "bndbox.h"

VALUE sr_cBndBox;

SR_CLASS_INIT(Bnd_Box, BndBox, bndbox)

bool siren_bndbox_install()
{
  sr_cBndBox = rb_define_class_under(sr_mSiren, "BndBox", rb_cObject);
  rb_define_alloc_func(sr_cBndBox, siren_bndbox_allocate);
  rb_define_method(sr_cBndBox, "initialize", RUBY_METHOD_FUNC(siren_bndbox_init),          -1);
  rb_define_method(sr_cBndBox, "inspect",    RUBY_METHOD_FUNC(siren_bndbox_to_s),          -1);
  rb_define_method(sr_cBndBox, "to_s",       RUBY_METHOD_FUNC(siren_bndbox_to_s),          -1);
  rb_define_method(sr_cBndBox, "min",        RUBY_METHOD_FUNC(siren_bndbox_min),           -1);
  rb_define_method(sr_cBndBox, "max",        RUBY_METHOD_FUNC(siren_bndbox_max),           -1);
  rb_define_method(sr_cBndBox, "add",        RUBY_METHOD_FUNC(siren_bndbox_add),           -1);
  rb_define_method(sr_cBndBox, "out?",       RUBY_METHOD_FUNC(siren_bndbox_is_out),        -1);
  rb_define_method(sr_cBndBox, "center",     RUBY_METHOD_FUNC(siren_bndbox_center),        -1);
  rb_define_method(sr_cBndBox, "xsize",      RUBY_METHOD_FUNC(siren_bndbox_xsize),         -1);
  rb_define_method(sr_cBndBox, "ysize",      RUBY_METHOD_FUNC(siren_bndbox_ysize),         -1);
  rb_define_method(sr_cBndBox, "zsize",      RUBY_METHOD_FUNC(siren_bndbox_zsize),         -1);
  rb_define_method(sr_cBndBox, "void?",      RUBY_METHOD_FUNC(siren_bndbox_is_void),       -1);
  rb_define_method(sr_cBndBox, "whole?",     RUBY_METHOD_FUNC(siren_bndbox_is_whole),      -1);
  rb_define_method(sr_cBndBox, "void!",      RUBY_METHOD_FUNC(siren_bndbox_void_bang),     -1);
  rb_define_method(sr_cBndBox, "whole!",     RUBY_METHOD_FUNC(siren_bndbox_whole_bang),    -1);
  rb_define_method(sr_cBndBox, "xthin?",     RUBY_METHOD_FUNC(siren_bndbox_is_xthin),      -1);
  rb_define_method(sr_cBndBox, "ythin?",     RUBY_METHOD_FUNC(siren_bndbox_is_ythin),      -1);
  rb_define_method(sr_cBndBox, "zthin?",     RUBY_METHOD_FUNC(siren_bndbox_is_zthin),      -1);
  rb_define_method(sr_cBndBox, "openxmin?",  RUBY_METHOD_FUNC(siren_bndbox_is_openxmin),   -1);
  rb_define_method(sr_cBndBox, "openxmax?",  RUBY_METHOD_FUNC(siren_bndbox_is_openxmax),   -1);
  rb_define_method(sr_cBndBox, "openymin?",  RUBY_METHOD_FUNC(siren_bndbox_is_openymin),   -1);
  rb_define_method(sr_cBndBox, "openymax?",  RUBY_METHOD_FUNC(siren_bndbox_is_openymax),   -1);
  rb_define_method(sr_cBndBox, "openzmin?",  RUBY_METHOD_FUNC(siren_bndbox_is_openzmin),   -1);
  rb_define_method(sr_cBndBox, "openzmax?",  RUBY_METHOD_FUNC(siren_bndbox_is_openzmax),   -1);
  rb_define_method(sr_cBndBox, "openxmin!",  RUBY_METHOD_FUNC(siren_bndbox_openxmin_bang), -1);
  rb_define_method(sr_cBndBox, "openxmax!",  RUBY_METHOD_FUNC(siren_bndbox_openxmax_bang), -1);
  rb_define_method(sr_cBndBox, "openymin!",  RUBY_METHOD_FUNC(siren_bndbox_openymin_bang), -1);
  rb_define_method(sr_cBndBox, "openymax!",  RUBY_METHOD_FUNC(siren_bndbox_openymax_bang), -1);
  rb_define_method(sr_cBndBox, "openzmin!",  RUBY_METHOD_FUNC(siren_bndbox_openzmin_bang), -1);
  rb_define_method(sr_cBndBox, "openzmax!",  RUBY_METHOD_FUNC(siren_bndbox_openzmax_bang), -1);
  rb_define_method(sr_cBndBox, "gap",        RUBY_METHOD_FUNC(siren_bndbox_get_gap),       -1);
  rb_define_method(sr_cBndBox, "gap=",       RUBY_METHOD_FUNC(siren_bndbox_set_gap),       -1);
  rb_define_method(sr_cBndBox, "dist",       RUBY_METHOD_FUNC(siren_bndbox_dist),          -1);
  rb_define_method(sr_cBndBox, "square",     RUBY_METHOD_FUNC(siren_bndbox_square),        -1);
  return true;
}

VALUE siren_bndbox_init(int argc, VALUE* argv, VALUE self)
{
  return self;
}

void siren_bndbox_final(void* p)
{
  Bnd_Box* pp = static_cast<Bnd_Box*>(p);
  ruby_xfree(pp);
}

VALUE siren_bndbox_to_s(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  char str[128];
  if (b->IsVoid()) {
    snprintf(str, sizeof(str), "#<BndBox:0x%x (void)>", -1 /* (unsigned int)(uintptr_t)rb_cptr(self) */);
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
        -1,
        // (unsigned int)(uintptr_t)rb_cptr(self),
        sxmin, symin, szmin, sxmax, symax, szmax);
  }
  return rb_str_new_cstr(str);
}

VALUE siren_bndbox_min(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  return siren_pnt_to_ary(b->CornerMin());
}

VALUE siren_bndbox_max(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  return siren_pnt_to_ary(b->CornerMax());
}

VALUE siren_bndbox_add(int argc, VALUE* argv, VALUE self)
{
  VALUE obj;
  rb_scan_args(argc, argv, "1", &obj);

  Bnd_Box* b = siren_bndbox_get(self);

  /*
  if (siren_is_shape(obj)) {
    TopoDS_Shape* s = siren_shape_get(obj);
    BRepBndLib::Add(*s, *b);
  }
  else if (siren_is_bndbox(obj)) {
 */
    Bnd_Box* bb = siren_bndbox_get(obj);
    b->Add(*bb);
  /*
  }
  else if (RB_TYPE_P(obj, T_ARRAY)) {
    b->Add(siren_ary_to_pnt(obj));
  }
  */

  return Qnil;
}

VALUE siren_bndbox_is_out(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsOut(*siren_bndbox_get(other)) == Standard_True ? Qtrue : Qfalse;
}

VALUE siren_bndbox_center(int argc, VALUE* argv, VALUE self)
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

VALUE siren_bndbox_xsize(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return DBL2NUM(xmax - xmin);
}

VALUE siren_bndbox_ysize(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return DBL2NUM(ymax - ymin);
}

VALUE siren_bndbox_zsize(int argc, VALUE* argv, VALUE self)
{
  Bnd_Box* b = siren_bndbox_get(self);
  if (b->IsVoid()) {
    return Qnil;
  }
  Standard_Real xmin, ymin, zmin;
  Standard_Real xmax, ymax, zmax;
  b->Get(xmin, ymin, zmin, xmax, ymax, zmax);
  return DBL2NUM(zmax - zmin);
}

VALUE siren_bndbox_is_void(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsVoid() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_whole(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsWhole() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_void_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->SetVoid();
  return Qnil;
}

VALUE siren_bndbox_whole_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->SetWhole();
  return Qnil;
}

VALUE siren_bndbox_is_xthin(int argc, VALUE* argv, VALUE self)
{
  VALUE tol;
  rb_scan_args(argc, argv, "01", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsXThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_ythin(int argc, VALUE* argv, VALUE self)
{
  VALUE tol;
  rb_scan_args(argc, argv, "01", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsYThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_zthin(int argc, VALUE* argv, VALUE self)
{
  VALUE tol;
  rb_scan_args(argc, argv, "01", &tol);
  Bnd_Box* b = siren_bndbox_get(self);
  return b->IsZThin(argc ? tol : 0.0) ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openxmin(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenXmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openxmax(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenXmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openymin(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenYmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openymax(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenYmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openzmin(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenZmin() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_is_openzmax(int argc, VALUE* argv, VALUE self)
{
  return siren_bndbox_get(self)->IsOpenZmax() ? Qtrue : Qfalse;
}

VALUE siren_bndbox_openxmin_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenXmin();
  return Qnil;
}

VALUE siren_bndbox_openxmax_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenXmax();
  return Qnil;
}

VALUE siren_bndbox_openymin_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenYmin();
  return Qnil;
}

VALUE siren_bndbox_openymax_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenYmax();
  return Qnil;
}

VALUE siren_bndbox_openzmin_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenZmin();
  return Qnil;
}

VALUE siren_bndbox_openzmax_bang(int argc, VALUE* argv, VALUE self)
{
  siren_bndbox_get(self)->OpenZmax();
  return Qnil;
}

VALUE siren_bndbox_set_gap(int argc, VALUE* argv, VALUE self)
{
  VALUE tol;
  rb_scan_args(argc, argv, "1", &tol);
  siren_bndbox_get(self)->SetGap(tol);
  return Qnil;
}

VALUE siren_bndbox_get_gap(int argc, VALUE* argv, VALUE self)
{
  Standard_Real tol = siren_bndbox_get(self)->GetGap();
  return DBL2NUM(tol);
}

VALUE siren_bndbox_dist(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  Bnd_Box* b = siren_bndbox_get(self);
  Bnd_Box* bb= siren_bndbox_get(other);
  if (b->IsVoid() || bb->IsVoid()) {
    return Qnil;
  }
  Standard_Real value = b->Distance(*bb);
  return DBL2NUM(value);
}

VALUE siren_bndbox_square(int argc, VALUE* argv, VALUE self)
{
  Standard_Real value = siren_bndbox_get(self)->SquareExtent();
  return DBL2NUM(value);
}
