#include "filler.h"

VALUE sr_cFiller;

VALUE siren_filler_allocate(VALUE klass)
{
  void* p = ruby_xmalloc(sizeof(BRepFill_Filling));
  new(p) BRepFill_Filling();
  return Data_Wrap_Struct(klass, NULL, siren_filler_final, p);
}

BRepFill_Filling* siren_filler_get(VALUE obj)
{
  BRepFill_Filling* m;
  Data_Get_Struct(obj, BRepFill_Filling, m);
  return m;
}

bool siren_filler_install()
{
  sr_cFiller = rb_define_class_under(sr_mSiren, "Filler", rb_cObject);
  rb_define_alloc_func(sr_cFiller, siren_filler_allocate);

  rb_define_method(sr_cFiller, "initialize", RUBY_METHOD_FUNC(siren_filler_init),      -1);
  rb_define_method(sr_cFiller, "add_bound",  RUBY_METHOD_FUNC(siren_filler_add_bound), 2);
  rb_define_method(sr_cFiller, "add",        RUBY_METHOD_FUNC(siren_filler_add),       2);
  rb_define_method(sr_cFiller, "build",      RUBY_METHOD_FUNC(siren_filler_build),     0);
  rb_define_method(sr_cFiller, "done?",      RUBY_METHOD_FUNC(siren_filler_is_done),   0);
  rb_define_method(sr_cFiller, "face",       RUBY_METHOD_FUNC(siren_filler_face),      0);
  rb_define_method(sr_cFiller, "g0error",    RUBY_METHOD_FUNC(siren_filler_g0error),   1);
  rb_define_method(sr_cFiller, "g1error",    RUBY_METHOD_FUNC(siren_filler_g1error),   1);
  rb_define_method(sr_cFiller, "g2error",    RUBY_METHOD_FUNC(siren_filler_g2error),   1);
  return true;
}

VALUE
siren_filler_init(int argc, VALUE* argv, VALUE self)
{
  VALUE degree, nbptsoncur, nbiter;
  VALUE anisotropie;
  VALUE tol2d, tol3d, tolang, tolcurv;
  VALUE maxdeg, maxsegs;
  // rb_scan_args(argc, argv, "|iiibffffii",
  //     &degree, &nbptsoncur, &nbiter, &anisotropie,
  //     &tol2d, &tol3d, &tolang, &tolcurv, &maxdeg, &maxsegs);
  // issue: unsupports over 10 args?
  rb_scan_args(argc, argv, "19",
      &degree, &nbptsoncur, &nbiter, &anisotropie,
      &tol2d, &tol3d, &tolang, &tolcurv, &maxdeg, &maxsegs);

  auto p = siren_filler_get(self);

  switch (argc) {
    case 1:
      p->SetResolParam(NUM2INT(degree));
      break;
    case 2:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur));
      break;
    case 3:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter));
      break;
    case 4:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      break;
/*
    case 5:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d));
      break;
    case 6:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d), NUM2DBL(tol3d));
      break;
    case 7:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang));
      break;
    case 8:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv));
      break;
    case 9:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv), NUM2INT(maxdeg));
      break;
    case 10:
      p->SetResolParam(NUM2INT(degree), NUM2INT(nbptsoncur), NUM2INT(nbiter), anisotropie == Qtrue);
      p->SetConstrParam(NUM2DBL(tol2d), NUM2DBL(tol3d), NUM2DBL(tolang), NUM2DBL(tolcurv), NUM2INT(maxdeg), NUM2INT(maxsegs));
      break;
*/
    default:
      break;
  }
  return self;
}

void siren_filler_final( void* p)
{
  BRepFill_Filling* pp = static_cast<BRepFill_Filling*>(p);
  ruby_xfree(pp);
}

VALUE siren_filler_add_bound(int argc, VALUE* argv, VALUE self)
{
  VALUE edge;
  VALUE order;
  rb_scan_args(argc, argv, "2", &edge, &order);

  auto e = siren_edge_get(edge);
  siren_filler_get(self)->Add(e, (GeomAbs_Shape)NUM2INT(order),
      /* IsBound= */ Standard_True);
  return Qnil;
}

VALUE siren_filler_add(int argc, VALUE* argv, VALUE self)
{
  VALUE obj;
  VALUE order;
  rb_scan_args(argc, argv, "11", &obj, &order);
  if (order != Qnil) {
    TopoDS_Edge e = siren_edge_get(obj);
    siren_filler_get(self)->Add(e, (GeomAbs_Shape)NUM2INT(order),
        /* IsBound= */ Standard_False);
  }
  else {
    gp_Pnt pnt = siren_ary_to_pnt(obj);
    siren_filler_get(self)->Add(pnt);
  }
  return Qnil;
}

VALUE siren_filler_build(int argc, VALUE* argv, VALUE self)
{
  siren_filler_get(self)->Build();
  return Qnil;
}

VALUE siren_filler_is_done(int argc, VALUE* argv, VALUE self)
{
  return siren_filler_get(self)->IsDone() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_filler_face(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Face f = siren_filler_get(self)->Face();
  return siren_shape_new(f);
}

VALUE siren_filler_g0error(int argc, VALUE* argv, VALUE self)
{
  VALUE index;
  rb_scan_args(argc, argv, "01", &index);
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G0Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G0Error();
  }
  return (value);
}

VALUE siren_filler_g1error(int argc, VALUE* argv, VALUE self)
{
  VALUE index;
  rb_scan_args(argc, argv, "01", &index);
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G1Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G1Error();
  }
  return (value);
}

VALUE siren_filler_g2error(int argc, VALUE* argv, VALUE self)
{
  VALUE index;
  rb_scan_args(argc, argv, "01", &index);
  Standard_Real value;
  if (index != Qnil) {
    value = siren_filler_get(self)->G2Error(NUM2INT(index));
  }
  else {
    value = siren_filler_get(self)->G2Error();
  }
  return (value);
}
