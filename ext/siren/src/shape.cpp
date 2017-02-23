/*
 * Document-class: Shape
 *
 */

#include "shape.h"

TopoDS_Shape* siren_shape_get( VALUE obj)
{
  // return static_cast<TopoDS_Shape*>(_get_datatype(obj, &siren_shape_type));
  // Get ptr without type checking.
  return static_cast<TopoDS_Shape*>(DATA_PTR(obj));
}

VALUE siren_shape_new( const TopoDS_Shape& shape)
{
  struct RClass* cls_shape = siren_shape_rclass();
  switch (shape.ShapeType()) {
    case TopAbs_VERTEX:    return siren_vertex_new(&shape);   break;
    case TopAbs_EDGE:      return siren_edge_new(&shape);     break;
    case TopAbs_WIRE:      return siren_wire_new(&shape);     break;
    case TopAbs_FACE:      return siren_face_new(&shape);     break;
    case TopAbs_SHELL:     return siren_shell_new(&shape);    break;
    case TopAbs_SOLID:     return siren_solid_new(&shape);    break;
#ifdef SIREN_ENABLE_CHUNK
    case TopAbs_COMPSOLID: return siren_chunk_new(&shape);    break;
#endif
    case TopAbs_COMPOUND:  return siren_compound_new(&shape); break;
    default: break;
  }
  rb_raise(E_ARGUMENT_ERROR, "Failed to make Shape object.");
  return Qnil;
}

struct RClass* siren_shape_rclass()
{
  struct RClass* mod_siren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(mod_siren), VALUEern_lit("Shape")));
}

bool siren_shape_install( struct RClass* mod_siren)
{
  struct RClass* cls_shape = rb_define_class_under(mod_siren, "Shape", rb_cObject);
  MRB_SET_INSTANCE_TT(cls_shape, MRB_TT_DATA);
  rb_define_method(cls_shape, "initialize", siren_shape_init,       MRB_ARGS_NONE());
  rb_define_method(cls_shape, "null?",      siren_shape_is_null,    MRB_ARGS_NONE());
  rb_define_method(cls_shape, "pos",        siren_shape_pos,        MRB_ARGS_NONE());
  rb_define_method(cls_shape, "trans",      siren_shape_trans,      MRB_ARGS_NONE());
  rb_define_method(cls_shape, "trans=",     siren_shape_set_trans,  MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "bndbox",     siren_shape_bndbox,     MRB_ARGS_NONE());

  rb_define_method(cls_shape, "translate!", siren_shape_translate_bang, MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "rotate!",    siren_shape_rotate_bang,    MRB_ARGS_REQ(3));
  rb_define_method(cls_shape, "scale!",     siren_shape_scale_bang,     MRB_ARGS_REQ(2));
  rb_define_method(cls_shape, "mirror!",    siren_shape_mirror_bang,    MRB_ARGS_REQ(2));
  rb_define_method(cls_shape, "move!",      siren_shape_move_bang,      MRB_ARGS_REQ(1));

  rb_define_method(cls_shape, "translate",  siren_shape_translate,  MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "rotate",     siren_shape_rotate,     MRB_ARGS_REQ(3));
  rb_define_method(cls_shape, "scale",      siren_shape_scale,      MRB_ARGS_REQ(2));
  rb_define_method(cls_shape, "mirror",     siren_shape_mirror,     MRB_ARGS_REQ(2));
  rb_define_method(cls_shape, "move",       siren_shape_move,       MRB_ARGS_REQ(1));

  rb_define_method(cls_shape, "hashcode",   siren_shape_hashcode,   MRB_ARGS_NONE());

  rb_define_method(cls_shape, "partner?",   siren_shape_is_partner, MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "same?",      siren_shape_is_same,    MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "equal?",     siren_shape_is_equal,   MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "<=>",        siren_shape_is_equal,   MRB_ARGS_REQ(1));

  rb_define_method(cls_shape, "explore",    siren_shape_explore,    MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method(cls_shape, "subshapes",  siren_shape_subshapes,  MRB_ARGS_OPT(2));

  rb_define_method(cls_shape, "section",    siren_shape_section,    MRB_ARGS_REQ(1));

  rb_define_method(cls_shape, "reverse",    siren_shape_reverse,     MRB_ARGS_NONE());
  rb_define_method(cls_shape, "reverse!",   siren_shape_reverse_bang,MRB_ARGS_NONE());

#ifdef SIREN_ENABLE_SHHEALING
  rb_define_method(cls_shape, "outerwire",  siren_heal_outerwire, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_method(cls_shape, "fix",        siren_heal_fix, MRB_ARGS_REQ(1));
#endif

  /* from BRepTools */
  rb_define_method(cls_shape, "update!", siren_shape_update_bang, MRB_ARGS_NONE());
  rb_define_method(cls_shape, "clean!",  siren_shape_clean_bang,  MRB_ARGS_NONE());

  /* frag accessors */
  rb_define_method(cls_shape, "lock?",       siren_shape_is_lock,        MRB_ARGS_NONE());
  rb_define_method(cls_shape, "lock",        siren_shape_is_lock,        MRB_ARGS_NONE());
  rb_define_method(cls_shape, "lock=",       siren_shape_set_lock,       MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "modify?",     siren_shape_is_modify,      MRB_ARGS_NONE());
  rb_define_method(cls_shape, "modify",      siren_shape_is_modify,      MRB_ARGS_NONE());
  rb_define_method(cls_shape, "modify=",     siren_shape_set_modify,     MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "check?",      siren_shape_is_check,       MRB_ARGS_NONE());
  rb_define_method(cls_shape, "check",       siren_shape_is_check,       MRB_ARGS_NONE());
  rb_define_method(cls_shape, "check=",      siren_shape_set_check,      MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "orientable?", siren_shape_is_orientable,  MRB_ARGS_NONE());
  rb_define_method(cls_shape, "orientable",  siren_shape_is_orientable,  MRB_ARGS_NONE());
  rb_define_method(cls_shape, "orientable=", siren_shape_set_orientable, MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "close?",      siren_shape_is_close,       MRB_ARGS_NONE());
  rb_define_method(cls_shape, "close",       siren_shape_is_close,       MRB_ARGS_NONE());
  rb_define_method(cls_shape, "close=",      siren_shape_set_close,      MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "infinite?",   siren_shape_is_infinite,    MRB_ARGS_NONE());
  rb_define_method(cls_shape, "infinite",    siren_shape_is_infinite,    MRB_ARGS_NONE());
  rb_define_method(cls_shape, "infinite=",   siren_shape_set_infinite,   MRB_ARGS_REQ(1));
  rb_define_method(cls_shape, "convex?",     siren_shape_is_convex,      MRB_ARGS_NONE());
  rb_define_method(cls_shape, "convex",      siren_shape_is_convex,      MRB_ARGS_NONE());
  rb_define_method(cls_shape, "convex=",     siren_shape_set_convex,     MRB_ARGS_REQ(1));

  rb_define_method(cls_shape, "next_trans",  siren_shape_next_trans,  MRB_ARGS_NONE());
  rb_define_method(cls_shape, "first_datum", siren_shape_first_datum,  MRB_ARGS_NONE());

  siren_vertex_install(mod_siren);
  siren_edge_install(mod_siren);
  siren_wire_install(mod_siren);
  siren_face_install(mod_siren);
  siren_shell_install(mod_siren);
  siren_solid_install(mod_siren);
#ifdef SIREN_ENABLE_CHUNK
  siren_chunk_install(mod_siren);
#endif
  siren_compound_install(mod_siren);

  return true;
}

VALUE siren_shape_init( VALUE self)
{
  rb_raise(E_NOMETHOD_ERROR, "private method `new' called for Shape:Class");
  return Qnil;
}

void siren_shape_final( void* p)
{
  TopoDS_Shape* s = static_cast<TopoDS_Shape*>(p);
  s->Nullify();
  rb_free(s);
}

VALUE siren_shape_is_null( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return shape->IsNull() ? Qtrue : Qfalse;
}

VALUE siren_shape_pos( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  gp_XYZ pos = shape->Location().Transformation().TranslationPart();
  return siren_pnt_to_ary(gp_Pnt(pos.X(), pos.Y(), pos.Z()));
}

VALUE siren_shape_trans( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location();
  return siren_trans_new(loc.Transformation());
}

VALUE siren_shape_next_trans( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location().NextLocation();
  return siren_trans_new(loc.Transformation());
}

VALUE siren_shape_first_datum( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location();
  opencascade::handle<TopLoc_Datum3D> datum = loc.FirstDatum();
  gp_Trsf t = datum->Transformation();
  return siren_trans_new(t);
}

VALUE siren_shape_set_trans( VALUE self)
{
  VALUE obj;
  int argc = rb_get_args("o", &obj);
  gp_Trsf* trans = siren_trans_get(obj);
  TopLoc_Location loc(*trans);
  siren_shape_get(self)->Location(loc);
  return self;
}

VALUE siren_shape_bndbox( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return siren_bndbox_new(*shape);
}

VALUE siren_shape_translate_bang( VALUE self)
{
  VALUE vec;
  int argc = rb_get_args("A", &vec);
  gp_Trsf trsf;
  trsf.SetTranslation(siren_ary_to_vec(vec));
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_rotate_bang( VALUE self)
{
  VALUE op, norm;
  VALUE ang;
  int argc = rb_get_args("AAf", &op, &norm, &ang);
  gp_Trsf trsf;
  trsf.SetRotation(siren_ary_to_ax1(op, norm), ang);
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_scale_bang( VALUE self)
{
  VALUE op;
  VALUE factor;
  int argc = rb_get_args("Af", &op, &factor);
  gp_Trsf trsf;
  trsf.SetScale(siren_ary_to_pnt(op), factor);
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_mirror_bang( VALUE self)
{
  VALUE op, norm;
  int argc = rb_get_args("AA", &op, &norm);
  gp_Trsf trsf;
  trsf.SetMirror(siren_ary_to_ax2(op, norm));
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_move_bang( VALUE self)
{
  VALUE trans;
  int argc = rb_get_args("o", &trans);
  gp_Trsf* t = siren_trans_get(trans);
  siren_shape_get(self)->Move(*t);
  return self;
}

VALUE siren_shape_translate( VALUE self)
{
  VALUE vec;
  int argc = rb_get_args("A", &vec);
  gp_Trsf trsf;
  trsf.SetTranslation(siren_ary_to_vec(vec));
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_rotate( VALUE self)
{
  VALUE op, norm;
  VALUE ang;
  int argc = rb_get_args("AAf", &op, &norm, &ang);
  gp_Trsf trsf;
  trsf.SetRotation(siren_ary_to_ax1(op, norm), ang);
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_scale( VALUE self)
{
  VALUE op;
  VALUE factor;
  int argc = rb_get_args("Af", &op, &factor);
  gp_Trsf trsf;
  trsf.SetScale(siren_ary_to_pnt(op), factor);
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_mirror( VALUE self)
{
  VALUE op, norm;
  int argc = rb_get_args("AA", &op, &norm);
  gp_Trsf trsf;
  trsf.SetMirror(siren_ary_to_ax2(op, norm));
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_move( VALUE self)
{
  VALUE trans;
  int argc = rb_get_args("o", &trans);
  gp_Trsf* t = siren_trans_get(trans);
  return siren_shape_new(siren_shape_get(self)->Moved(*t));
}

VALUE siren_shape_hashcode( VALUE self)
{
  VALUE upper;
  int argc = rb_get_args("i", &upper);
  TopoDS_Shape* shape = siren_shape_get(self);
  return rb_fixnum_value(shape->HashCode(upper));
}

VALUE siren_shape_is_partner( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsPartner(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_is_same( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsSame(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_is_equal( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsEqual(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_explore( VALUE self)
{
  VALUE klass;
  VALUE klassf;
  VALUE block;
  int argc = rb_get_args("C|C&", &klass, &klassf, &block);

  VALUE mtype;
  TopAbs_ShapeEnum type = TopAbs_COMPOUND;
  if (_bool(_funcall(klass, "==", 1, siren_compound_obj()))) {
    type = TopAbs_COMPOUND;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_solid_obj()))) {
    type = TopAbs_SOLID;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_shell_obj()))) {
    type = TopAbs_SHELL;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_face_obj()))) {
    type = TopAbs_FACE;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_wire_obj()))) {
    type = TopAbs_WIRE;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_edge_obj()))) {
    type = TopAbs_EDGE;
  }
  else if (_bool(_funcall(klass, "==", 1, siren_vertex_obj()))) {
    type = TopAbs_VERTEX;
  }
  else {
    rb_raise(E_ARGUMENT_ERROR, "Type error.");
  }

  TopExp_Explorer ex;
  if (argc == 1) {
    ex.Init(*siren_shape_get(self), type);
  }
  else {
    VALUE mfilter;
    TopAbs_ShapeEnum avoid = TopAbs_COMPOUND;
    if (_bool(_funcall(klassf, "==", 1, siren_compound_obj()))) {
      avoid = TopAbs_COMPOUND;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_solid_obj()))) {
      avoid = TopAbs_SOLID;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_shell_obj()))) {
      avoid = TopAbs_SHELL;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_face_obj()))) {
      avoid = TopAbs_FACE;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_wire_obj()))) {
      avoid = TopAbs_WIRE;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_edge_obj()))) {
      avoid = TopAbs_EDGE;
    }
    else if (_bool(_funcall(klassf, "==", 1, siren_vertex_obj()))) {
      avoid = TopAbs_VERTEX;
    }
    else {
      rb_raise(E_ARGUMENT_ERROR, "Type error.");
    }
    ex.Init(*siren_shape_get(self), type, avoid);
  }

  if (!rb_nil_p(block)) {
    for (; ex.More(); ex.Next()) {
      VALUE argv[2];
      argv[0] = siren_shape_new(ex.Current());
      argv[1] = rb_fixnum_value(ex.Depth());
      rb_yield_argv(block, 2, argv);
    }
    return self;
  }
  VALUE ar = rb_ary_new();
  for (; ex.More(); ex.Next()) {
    VALUE ai = rb_gc_arena_save();
    rb_ary_push(ar, siren_shape_new(ex.Current()));
    rb_gc_arena_restore(ai);
  }
  return ar;
}

VALUE siren_shape_subshapes( VALUE self)
{
  VALUE ori, loc;
  int argc = rb_get_args("|bb", &ori, &loc);
  if (argc == 0) {
    ori = TRUE;
    loc = TRUE;
  }
  if (argc == 1) {
    loc = TRUE;
  }
  VALUE ar = rb_ary_new();
  TopoDS_Shape* shape = siren_shape_get(self);
  if (!shape || shape->IsNull()) {
    return ar;
  }
  TopoDS_Iterator it(*shape, (Standard_Boolean)ori, (Standard_Boolean)loc);
  for (; it.More(); it.Next()) {
    VALUE ai = rb_gc_arena_save();
    rb_ary_push(ar, siren_shape_new(it.Value()));
    rb_gc_arena_restore(ai);
  }
  return ar;
}

VALUE siren_shape_section( VALUE self)
{
  VALUE other;
  int argc = rb_get_args("o", &other);

  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(other);

  BRepAlgoAPI_Section api(*S1, *S2, Standard_False);
  // api.ComputePCurveOn1(Standard_True);
  api.Approximation(Standard_True);
  api.Build();

  if (!api.IsDone()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to intersection.");
  }

  return siren_shape_new(api.Shape());
}

VALUE siren_shape_reverse( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return siren_shape_new(shape->Reversed());
}

VALUE siren_shape_reverse_bang( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  shape->Reverse();
  return Qnil;
}

VALUE siren_shape_update_bang( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  BRepTools::Update(*shape);
  return self;
}

VALUE siren_shape_clean_bang( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  // Removes all the triangulations of the faces of <shape>
  // and removes all polygons on triangulations of the edges.
  BRepTools::Clean(*shape);
  return self;
}

VALUE siren_shape_is_lock( VALUE self)
{
  return siren_shape_get(self)->Locked() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_lock( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Locked((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_modify( VALUE self)
{
  return siren_shape_get(self)->Modified() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_modify( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Modified((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_check( VALUE self)
{
  return siren_shape_get(self)->Checked() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_check( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Checked((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_orientable( VALUE self)
{
  return siren_shape_get(self)->Orientable() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_orientable( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Orientable((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_close( VALUE self)
{
  return siren_shape_get(self)->Closed() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_close( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Closed((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_infinite( VALUE self)
{
  return siren_shape_get(self)->Infinite() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_infinite( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Infinite((Standard_Boolean)flag);
  return self;
}

VALUE siren_shape_is_convex( VALUE self)
{
  return siren_shape_get(self)->Convex() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_convex( VALUE self)
{
  VALUE flag;
  int argc = rb_get_args("b", &flag);
  siren_shape_get(self)->Convex((Standard_Boolean)flag);
  return self;
}

