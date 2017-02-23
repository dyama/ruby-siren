#include "filler.h"

BRepFill_Filling* siren_filler_get( VALUE obj)
{
  return static_cast<BRepFill_Filling*>(_get_datatype(obj, &siren_filler_type));
}

bool siren_filler_install(VALUE mod_siren)
{
  VALUE cls_filler = rb_define_class_under(mod_siren, "Filler", rb_cObject);
#if 0
  MRB_SET_INSTANCE_TT(cls_filler, MRB_TT_DATA);
#endif
  rb_define_method(cls_filler, "initialize", siren_filler_init,      10);
  rb_define_method(cls_filler, "add_bound",  siren_filler_add_bound, 2);
  rb_define_method(cls_filler, "add",        siren_filler_add,       2);
  rb_define_method(cls_filler, "build",      siren_filler_build,     0);
  rb_define_method(cls_filler, "done?",      siren_filler_is_done,   0);
  rb_define_method(cls_filler, "face",       siren_filler_face,      0);
  rb_define_method(cls_filler, "g0error",    siren_filler_g0error,   1);
  rb_define_method(cls_filler, "g1error",    siren_filler_g1error,   1);
  rb_define_method(cls_filler, "g2error",    siren_filler_g2error,   1);
  return true;
}

VALUE
siren_filler_init(VALUE self,
    VALUE degree, VALUE nbptsoncur, VALUE nbiter, VALUE anisotropie,
    VALUE tol2d, VALUE tol3d, VALUE tolang, VALUE tolcurv, VALUE maxdeg, VALUE maxsegs
    )
{
#if 0
  VALUE degree, nbptsoncur, nbiter;
  VALUE anisotropie;
  VALUE tol2d, tol3d, tolang, tolcurv;
  VALUE maxdeg, maxsegs;
  int argc = rb_get_args("|iiibffffii",
      &degree, &nbptsoncur, &nbiter, &anisotropie,
      &tol2d, &tol3d, &tolang, &tolcurv, &maxdeg, &maxsegs);
#endif

  int argc = 0;
  if (!NIL_P(degree)) argc += 1;
  if (!NIL_P(nbptsoncur)) argc += 1;
  if (!NIL_P(nbiter)) argc += 1;
  if (!NIL_P(anisotropie)) argc += 1;
  if (!NIL_P(tol2d)) argc += 1;
  if (!NIL_P(tol3d)) argc += 1;
  if (!NIL_P(tolang)) argc += 1;
  if (!NIL_P(tolcurv)) argc += 1;
  if (!NIL_P(maxdeg)) argc += 1;
  if (!NIL_P(maxsegs)) argc += 1;

#if 0
  void* p = rb_malloc(sizeof(BRepFill_Filling));
#else
  void* p = ruby_xmalloc(sizeof(BRepFill_Filling));
#endif
  BRepFill_Filling* inner = nullptr;

  switch (argc) {
    case 1:
      inner = new(p) BRepFill_Filling(NUM2INT(degree));
      break;
    case 2:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur));
      break;
    case 3:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter));
      break;
    case 4:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      break;
    case 5:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d));
      break;
    case 6:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d), NUM2DBL(tol3d));
      break;
    case 7:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang));
      break;
    case 8:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv));
      break;
    case 9:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv), NUM2INT(maxdeg));
      break;
    case 10:
      inner = new(p) BRepFill_Filling(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue,
          NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv), NUM2INT(maxdeg), NUM2INT(maxsegs));
      break;
    default:
      inner = new(p) BRepFill_Filling();
      break;
  }

  DATA_PTR(self)  = inner;
  DATA_TYPE(self) = &siren_filler_type;
  return self;
}

void siren_filler_final( void* p)
{
  BRepFill_Filling* pp = static_cast<BRepFill_Filling*>(p);
  ruby_xfree(pp);
}

VALUE siren_filler_add_bound(VALUE self, VALUE edge, VALUE order)
{
#if 0
  VALUE edge;
  VALUE order;
  int argc = rb_get_args("oi", &edge, &order);
#endif
  TopoDS_Shape* s = siren_shape_get(edge);
  TopoDS_Edge e = TopoDS::Edge(*s);
  siren_filler_get(self)->Add(e, (GeomAbs_Shape)NUM2INT(order),
      /* IsBound= */ Standard_True);
  return Qnil;
}

VALUE siren_filler_add(VALUE self, VALUE obj, VALUE order)
{
#if 0
  VALUE obj;
  VALUE order;
  int argc = rb_get_args("o|i", &obj, &order);
  if (argc == 2) {
#endif
  if (order != Qnil) {
    TopoDS_Shape* s = siren_shape_get(obj);
    TopoDS_Edge e = TopoDS::Edge(*s);
    siren_filler_get(self)->Add(e, (GeomAbs_Shape)NUM2INT(order),
        /* IsBound= */ Standard_False);
  }
  else {
    gp_Pnt pnt = siren_ary_to_pnt(obj);
    siren_filler_get(self)->Add(pnt);
  }
  return Qnil;
}

VALUE siren_filler_build( VALUE self)
{
  siren_filler_get(self)->Build();
  return Qnil;
}

VALUE siren_filler_is_done( VALUE self)
{
  return siren_filler_get(self)->IsDone() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_filler_face( VALUE self)
{
  TopoDS_Face f = siren_filler_get(self)->Face();
  return siren_shape_new(f);
}

VALUE siren_filler_g0error(VALUE self, VALUE index)
{
#if 0
  VALUE index;
  int argc = rb_get_args("|i", &index);
#endif
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G0Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G0Error();
  }
  return (value);
}

VALUE siren_filler_g1error(VALUE self, VALUE index)
{
#if 0
  VALUE index;
  int argc = rb_get_args("|i", &index);
#endif
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G1Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G1Error();
  }
  return (value);
}

VALUE siren_filler_g2error(VALUE self, VALUE index)
{
#if 0
  VALUE index;
  int argc = rb_get_args("|i", &index);
#endif
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G2Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G2Error();
  }
  return (value);
}

