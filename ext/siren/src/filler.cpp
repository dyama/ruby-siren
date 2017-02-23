#include "filler.h"

BRepFill_Filling* siren_filler_get( VALUE obj)
{
  return static_cast<BRepFill_Filling*>(_get_datatype(obj, &siren_filler_type));
}

bool siren_filler_install( struct RClass* mod_siren)
{
  struct RClass* cls_filler = rb_define_class_under(mod_siren, "Filler", mrb->object_class);
  MRB_SET_INSTANCE_TT(cls_filler, MRB_TT_DATA);
  rb_define_method(cls_filler, "initialize", siren_filler_init,      MRB_ARGS_OPT(10));
  rb_define_method(cls_filler, "add_bound",  siren_filler_add_bound, MRB_ARGS_REQ(2));
  rb_define_method(cls_filler, "add",        siren_filler_add,       MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method(cls_filler, "build",      siren_filler_build,     MRB_ARGS_NONE());
  rb_define_method(cls_filler, "done?",      siren_filler_is_done,   MRB_ARGS_NONE());
  rb_define_method(cls_filler, "face",       siren_filler_face,      MRB_ARGS_NONE());
  rb_define_method(cls_filler, "g0error",    siren_filler_g0error,   MRB_ARGS_OPT(1));
  rb_define_method(cls_filler, "g1error",    siren_filler_g1error,   MRB_ARGS_OPT(1));
  rb_define_method(cls_filler, "g2error",    siren_filler_g2error,   MRB_ARGS_OPT(1));
  return true;
}

VALUE siren_filler_init( VALUE self)
{
  rb_int degree, nbptsoncur, nbiter;
  rb_bool anisotropie;
  VALUE tol2d, tol3d, tolang, tolcurv;
  rb_int maxdeg, maxsegs;
  int argc = rb_get_args("|iiibffffii",
      &degree, &nbptsoncur, &nbiter, &anisotropie,
      &tol2d, &tol3d, &tolang, &tolcurv, &maxdeg, &maxsegs);

  void* p = rb_malloc(sizeof(BRepFill_Filling));
  BRepFill_Filling* inner = nullptr;

  switch (argc) {
    case 1:
      inner = new(p) BRepFill_Filling(degree);
      break;
    case 2:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur);
      break;
    case 3:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter);
      break;
    case 4:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie);
      break;
    case 5:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d);
      break;
    case 6:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d, tol3d);
      break;
    case 7:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d, tol3d, tolang);
      break;
    case 8:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d, tol3d, tolang, tolcurv);
      break;
    case 9:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d, tol3d, tolang, tolcurv, maxdeg);
      break;
    case 10:
      inner = new(p) BRepFill_Filling(degree, nbptsoncur, nbiter, anisotropie,
          tol2d, tol3d, tolang, tolcurv, maxdeg, maxsegs);
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
  rb_free(pp);
}

VALUE siren_filler_add_bound( VALUE self)
{
  VALUE edge;
  rb_int order;
  int argc = rb_get_args("oi", &edge, &order);
  TopoDS_Shape* s = siren_shape_get(edge);
  TopoDS_Edge e = TopoDS::Edge(*s);
  siren_filler_get(self)->Add(e, (GeomAbs_Shape)order,
      /* IsBound= */ Standard_True);
  return Qnil;
}

VALUE siren_filler_add( VALUE self)
{
  VALUE obj;
  rb_int order;
  int argc = rb_get_args("o|i", &obj, &order);
  if (argc == 2) {
    TopoDS_Shape* s = siren_shape_get(obj);
    TopoDS_Edge e = TopoDS::Edge(*s);
    siren_filler_get(self)->Add(e, (GeomAbs_Shape)order,
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
    rb_true_value() : rb_false_value();
}

VALUE siren_filler_face( VALUE self)
{
  TopoDS_Face f = siren_filler_get(self)->Face();
  return siren_shape_new(f);
}

VALUE siren_filler_g0error( VALUE self)
{
  rb_int index;
  int argc = rb_get_args("|i", &index);
  Standard_Real value;
  if (argc) {
    value = siren_filler_get(self)->G0Error(index);
  }
  else {
    value = siren_filler_get(self)->G0Error();
  }
  return (value);
}

VALUE siren_filler_g1error( VALUE self)
{
  rb_int index;
  int argc = rb_get_args("|i", &index);
  Standard_Real value;
  if (argc) {
    value = siren_filler_get(self)->G1Error(index);
  }
  else {
    value = siren_filler_get(self)->G1Error();
  }
  return (value);
}

VALUE siren_filler_g2error( VALUE self)
{
  rb_int index;
  int argc = rb_get_args("|i", &index);
  Standard_Real value;
  if (argc) {
    value = siren_filler_get(self)->G2Error(index);
  }
  else {
    value = siren_filler_get(self)->G2Error();
  }
  return (value);
}

