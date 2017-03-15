#include "filler.h"

VALUE sr_cFiller;

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
  rb_scan_args(argc, argv, "19",
      &degree, &nbptsoncur, &nbiter, &anisotropie,
      &tol2d, &tol3d, &tolang, &tolcurv, &maxdeg, &maxsegs);

#if 0
  void* p = ruby_xmalloc(sizeof(BRepFill_Filling));
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
  // DATA_TYPE(self) = &siren_filler_type;
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
  rb_scan_args(argc, argv, "oi", &edge, &order);
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
  rb_scan_args(argc, argv, "o|i", &obj, &order);
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
  rb_scan_args(argc, argv, "|i", &index);
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
  rb_scan_args(argc, argv, "|i", &index);
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
  rb_scan_args(argc, argv, "|i", &index);
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
