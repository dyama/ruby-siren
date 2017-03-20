/*
 * Document-class: Shape
 *
 */

#include "shape.h"

VALUE sr_cShape;

SR_CLASS_INIT(TopoDS_Shape, Shape, shape)

VALUE siren_shape_new(const TopoDS_Shape& shape)
{
  VALUE klass;
  switch (shape.ShapeType()) {
    case TopAbs_VERTEX:    klass = sr_cVertex;    break;
    case TopAbs_EDGE:      klass = sr_cEdge;      break;
    case TopAbs_WIRE:      klass = sr_cWire;      break;
    case TopAbs_FACE:      klass = sr_cFace;      break;
    case TopAbs_SHELL:     klass = sr_cShell;     break;
    case TopAbs_SOLID:     klass = sr_cSolid;     break;
#ifdef SR_ENABLE_CHUNK
    case TopAbs_COMPSOLID: klass = sr_cCompSolid; break;
#endif
    case TopAbs_COMPOUND:  klass = sr_cCompound;  break;
    default:
      rb_raise(Qnil, "Failed to make Shape object.");
      break;
  }
  auto val = siren_shape_allocate(klass);
  auto ps = siren_shape_get(val);
  *ps = shape;
  return val;
}

bool siren_shape_install()
{
  sr_cShape = rb_define_class_under(sr_mSiren, "Shape", rb_cObject);
  rb_define_alloc_func(sr_cShape, siren_shape_allocate);
  rb_define_method(sr_cShape, "initialize", RUBY_METHOD_FUNC(siren_shape_init),           -1);
  rb_define_method(sr_cShape, "null?",      RUBY_METHOD_FUNC(siren_shape_is_null),        -1);
  rb_define_method(sr_cShape, "pos",        RUBY_METHOD_FUNC(siren_shape_pos),            -1);
  rb_define_method(sr_cShape, "trans",      RUBY_METHOD_FUNC(siren_shape_trans),          -1);
  rb_define_method(sr_cShape, "trans=",     RUBY_METHOD_FUNC(siren_shape_set_trans),      -1);
  rb_define_method(sr_cShape, "bndbox",     RUBY_METHOD_FUNC(siren_shape_bndbox),         -1);
  rb_define_method(sr_cShape, "translate!", RUBY_METHOD_FUNC(siren_shape_translate_bang), -1);
  rb_define_method(sr_cShape, "rotate!",    RUBY_METHOD_FUNC(siren_shape_rotate_bang),    -1);
  rb_define_method(sr_cShape, "scale!",     RUBY_METHOD_FUNC(siren_shape_scale_bang),     -1);
  rb_define_method(sr_cShape, "mirror!",    RUBY_METHOD_FUNC(siren_shape_mirror_bang),    -1);
  rb_define_method(sr_cShape, "move!",      RUBY_METHOD_FUNC(siren_shape_move_bang),      -1);
  rb_define_method(sr_cShape, "translate",  RUBY_METHOD_FUNC(siren_shape_translate),      -1);
  rb_define_method(sr_cShape, "rotate",     RUBY_METHOD_FUNC(siren_shape_rotate),         -1);
  rb_define_method(sr_cShape, "scale",      RUBY_METHOD_FUNC(siren_shape_scale),          -1);
  rb_define_method(sr_cShape, "mirror",     RUBY_METHOD_FUNC(siren_shape_mirror),         -1);
  rb_define_method(sr_cShape, "move",       RUBY_METHOD_FUNC(siren_shape_move),           -1);
  rb_define_method(sr_cShape, "hashcode",   RUBY_METHOD_FUNC(siren_shape_hashcode),       -1);
  rb_define_method(sr_cShape, "partner?",   RUBY_METHOD_FUNC(siren_shape_is_partner),     -1);
  rb_define_method(sr_cShape, "same?",      RUBY_METHOD_FUNC(siren_shape_is_same),        -1);
  rb_define_method(sr_cShape, "equal?",     RUBY_METHOD_FUNC(siren_shape_is_equal),       -1);
  rb_define_method(sr_cShape, "<=>",        RUBY_METHOD_FUNC(siren_shape_is_equal),       -1);
  rb_define_method(sr_cShape, "explore",    RUBY_METHOD_FUNC(siren_shape_explore),        -1);
  rb_define_method(sr_cShape, "subshapes",  RUBY_METHOD_FUNC(siren_shape_subshapes),      -1);
  rb_define_method(sr_cShape, "section",    RUBY_METHOD_FUNC(siren_shape_section),        -1);
  rb_define_method(sr_cShape, "reverse",    RUBY_METHOD_FUNC(siren_shape_reverse),        -1);
  rb_define_method(sr_cShape, "reverse!",   RUBY_METHOD_FUNC(siren_shape_reverse_bang),   -1);
#ifdef SR_ENABLE_SHHEALING
  rb_define_method(sr_cShape, "outerwire",  RUBY_METHOD_FUNC(siren_heal_outerwire),       -1);
  rb_define_method(sr_cShape, "fix",        RUBY_METHOD_FUNC(siren_heal_fix),             -1);
#endif
  /* from BRepTools */
  rb_define_method(sr_cShape, "update!",    RUBY_METHOD_FUNC(siren_shape_update_bang),     -1);
  rb_define_method(sr_cShape, "clean!",     RUBY_METHOD_FUNC(siren_shape_clean_bang),      -1);
  /* frag accessors */
  rb_define_method(sr_cShape, "lock?",       RUBY_METHOD_FUNC(siren_shape_is_lock),        -1);
  rb_define_method(sr_cShape, "lock",        RUBY_METHOD_FUNC(siren_shape_is_lock),        -1);
  rb_define_method(sr_cShape, "lock=",       RUBY_METHOD_FUNC(siren_shape_set_lock),       -1);
  rb_define_method(sr_cShape, "modify?",     RUBY_METHOD_FUNC(siren_shape_is_modify),      -1);
  rb_define_method(sr_cShape, "modify",      RUBY_METHOD_FUNC(siren_shape_is_modify),      -1);
  rb_define_method(sr_cShape, "modify=",     RUBY_METHOD_FUNC(siren_shape_set_modify),     -1);
  rb_define_method(sr_cShape, "check?",      RUBY_METHOD_FUNC(siren_shape_is_check),       -1);
  rb_define_method(sr_cShape, "check",       RUBY_METHOD_FUNC(siren_shape_is_check),       -1);
  rb_define_method(sr_cShape, "check=",      RUBY_METHOD_FUNC(siren_shape_set_check),      -1);
  rb_define_method(sr_cShape, "orientable?", RUBY_METHOD_FUNC(siren_shape_is_orientable),  -1);
  rb_define_method(sr_cShape, "orientable",  RUBY_METHOD_FUNC(siren_shape_is_orientable),  -1);
  rb_define_method(sr_cShape, "orientable=", RUBY_METHOD_FUNC(siren_shape_set_orientable), -1);
  rb_define_method(sr_cShape, "close?",      RUBY_METHOD_FUNC(siren_shape_is_close),       -1);
  rb_define_method(sr_cShape, "close",       RUBY_METHOD_FUNC(siren_shape_is_close),       -1);
  rb_define_method(sr_cShape, "close=",      RUBY_METHOD_FUNC(siren_shape_set_close),      -1);
  rb_define_method(sr_cShape, "infinite?",   RUBY_METHOD_FUNC(siren_shape_is_infinite),    -1);
  rb_define_method(sr_cShape, "infinite",    RUBY_METHOD_FUNC(siren_shape_is_infinite),    -1);
  rb_define_method(sr_cShape, "infinite=",   RUBY_METHOD_FUNC(siren_shape_set_infinite),   -1);
  rb_define_method(sr_cShape, "convex?",     RUBY_METHOD_FUNC(siren_shape_is_convex),      -1);
  rb_define_method(sr_cShape, "convex",      RUBY_METHOD_FUNC(siren_shape_is_convex),      -1);
  rb_define_method(sr_cShape, "convex=",     RUBY_METHOD_FUNC(siren_shape_set_convex),     -1);
  rb_define_method(sr_cShape, "next_trans",  RUBY_METHOD_FUNC(siren_shape_next_trans),     -1);
  rb_define_method(sr_cShape, "first_datum", RUBY_METHOD_FUNC(siren_shape_first_datum),    -1);

  siren_vertex_install();
  siren_edge_install();
  siren_wire_install();
  siren_face_install();
  siren_shell_install();
  siren_solid_install();
#ifdef SR_ENABLE_CHUNK
  siren_chunk_install();
#endif
  siren_compound_install();

  return true;
}

VALUE siren_shape_init(int argc, VALUE* argv, VALUE self)
{
  rb_raise(rb_eNoMethodError, "private method `new' called for Shape:Class");
  return Qnil;
}

void siren_shape_final( void* p)
{
  TopoDS_Shape* s = static_cast<TopoDS_Shape*>(p);
  s->Nullify();
  ruby_xfree(s);
}

VALUE siren_shape_is_null(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return shape->IsNull() ? Qtrue : Qfalse;
}

VALUE siren_shape_pos(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  gp_XYZ pos = shape->Location().Transformation().TranslationPart();
  return siren_pnt_to_ary(gp_Pnt(pos.X(), pos.Y(), pos.Z()));
}

VALUE siren_shape_trans(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location();
  return siren_trans_new(loc.Transformation());
}

VALUE siren_shape_next_trans(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location().NextLocation();
  return siren_trans_new(loc.Transformation());
}

VALUE siren_shape_first_datum(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  TopLoc_Location loc = shape->Location();
  opencascade::handle<TopLoc_Datum3D> datum = loc.FirstDatum();
  gp_Trsf t = datum->Transformation();
  return siren_trans_new(t);
}

VALUE siren_shape_set_trans(int argc, VALUE* argv, VALUE self)
{
  VALUE obj;
  rb_scan_args(argc, argv, "1", &obj);
  gp_Trsf* trans = siren_trans_get(obj);
  TopLoc_Location loc(*trans);
  siren_shape_get(self)->Location(loc);
  return self;
}

VALUE siren_shape_bndbox(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return siren_bndbox_new(*shape);
}

VALUE siren_shape_translate_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE vec;
  rb_scan_args(argc, argv, "1", &vec);
  gp_Trsf trsf;
  trsf.SetTranslation(siren_ary_to_vec(vec));
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_rotate_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm;
  VALUE ang;
  rb_scan_args(argc, argv, "3", &op, &norm, &ang);
  gp_Trsf trsf;
  trsf.SetRotation(siren_ary_to_ax1(op, norm), ang);
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_scale_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op;
  VALUE factor;
  rb_scan_args(argc, argv, "2", &op, &factor);
  gp_Trsf trsf;
  trsf.SetScale(siren_ary_to_pnt(op), factor);
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_mirror_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm;
  rb_scan_args(argc, argv, "2", &op, &norm);
  gp_Trsf trsf;
  trsf.SetMirror(siren_ary_to_ax2(op, norm));
  siren_shape_get(self)->Move(trsf);
  return self;
}

VALUE siren_shape_move_bang(int argc, VALUE* argv, VALUE self)
{
  VALUE trans;
  rb_scan_args(argc, argv, "1", &trans);
  gp_Trsf* t = siren_trans_get(trans);
  siren_shape_get(self)->Move(*t);
  return self;
}

VALUE siren_shape_translate(int argc, VALUE* argv, VALUE self)
{
  VALUE vec;
  rb_scan_args(argc, argv, "1", &vec);
  gp_Trsf trsf;
  trsf.SetTranslation(siren_ary_to_vec(vec));
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_rotate(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm;
  VALUE ang;
  rb_scan_args(argc, argv, "3", &op, &norm, &ang);
  gp_Trsf trsf;
  trsf.SetRotation(siren_ary_to_ax1(op, norm), ang);
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_scale(int argc, VALUE* argv, VALUE self)
{
  VALUE op;
  VALUE factor;
  rb_scan_args(argc, argv, "2", &op, &factor);
  gp_Trsf trsf;
  trsf.SetScale(siren_ary_to_pnt(op), factor);
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_mirror(int argc, VALUE* argv, VALUE self)
{
  VALUE op, norm;
  rb_scan_args(argc, argv, "2", &op, &norm);
  gp_Trsf trsf;
  trsf.SetMirror(siren_ary_to_ax2(op, norm));
  return siren_shape_new(siren_shape_get(self)->Moved(trsf));
}

VALUE siren_shape_move(int argc, VALUE* argv, VALUE self)
{
  VALUE trans;
  rb_scan_args(argc, argv, "1", &trans);
  gp_Trsf* t = siren_trans_get(trans);
  return siren_shape_new(siren_shape_get(self)->Moved(*t));
}

VALUE siren_shape_hashcode(int argc, VALUE* argv, VALUE self)
{
  VALUE upper;
  rb_scan_args(argc, argv, "1", &upper);
  TopoDS_Shape* shape = siren_shape_get(self);
  return INT2FIX(shape->HashCode(upper));
}

VALUE siren_shape_is_partner(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsPartner(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_is_same(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsSame(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_is_equal(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);
  TopoDS_Shape* shape_self  = siren_shape_get(self);
  TopoDS_Shape* shape_other = siren_shape_get(other);
  return shape_self->IsEqual(*shape_other) ? Qtrue : Qfalse;
}

VALUE siren_shape_explore(int argc, VALUE* argv, VALUE self)
{
  VALUE klass;
  VALUE klassf;
  VALUE block;
  rb_scan_args(argc, argv, "12", &klass, &klassf, &block);

  VALUE mtype;
  TopAbs_ShapeEnum type = TopAbs_COMPOUND;
  if (rb_funcall(klass, rb_intern("=="), 1, sr_cCompound) == Qtrue) {
    type = TopAbs_COMPOUND;
  }
#ifdef SR_ENABLE_CHUNK
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cChunk) == Qtrue) {
    type = TopAbs_COMPSOLID;
  }
#endif
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cSolid) == Qtrue) {
    type = TopAbs_SOLID;
  }
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cShell) == Qtrue) {
    type = TopAbs_SHELL;
  }
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cFace) == Qtrue) {
    type = TopAbs_FACE;
  }
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cWire) == Qtrue) {
    type = TopAbs_WIRE;
  }
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cEdge) == Qtrue) {
    type = TopAbs_EDGE;
  }
  else if (rb_funcall(klass, rb_intern("=="), 1, sr_cVertex) == Qtrue) {
    type = TopAbs_VERTEX;
  }
  else {
    rb_raise(Qnil, "Type error.");
  }

  TopExp_Explorer ex;
  if (argc == 1) {
    ex.Init(*siren_shape_get(self), type);
  }
  else {
    VALUE mfilter;
    TopAbs_ShapeEnum avoid = TopAbs_COMPOUND;
    if (rb_funcall(klassf, rb_intern("=="), 1, sr_cCompound) == Qtrue) {
      avoid = TopAbs_COMPOUND;
    }
#ifdef SR_ENABLE_CHUNK
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cChunk) == Qtrue) {
      avoid = TopAbs_COMPSOLID;
    }
#endif
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cSolid) == Qtrue) {
      avoid = TopAbs_SOLID;
    }
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cShell) == Qtrue) {
      avoid = TopAbs_SHELL;
    }
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cFace) == Qtrue) {
      avoid = TopAbs_FACE;
    }
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cWire) == Qtrue) {
      avoid = TopAbs_WIRE;
    }
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cEdge) == Qtrue) {
      avoid = TopAbs_EDGE;
    }
    else if (rb_funcall(klassf, rb_intern("=="), 1, sr_cVertex) == Qtrue) {
      avoid = TopAbs_VERTEX;
    }
    else {
      rb_raise(Qnil, "Type error.");
    }
    ex.Init(*siren_shape_get(self), type, avoid);
  }

  if (!NIL_P(block)) {
    for (; ex.More(); ex.Next()) {
      VALUE argv[2];
      argv[0] = siren_shape_new(ex.Current());
      argv[1] = INT2FIX(ex.Depth());
#if 0
      rb_yield_argv(block, 2, argv);
#else
      // where is a block?
      rb_yield_values2(2, argv);
#endif
    }
    return self;
  }
  VALUE ar = rb_ary_new();
  for (; ex.More(); ex.Next()) {
#if 0
    VALUE ai = rb_gc_arena_save();
#endif
    rb_ary_push(ar, siren_shape_new(ex.Current()));
#if 0
    rb_gc_arena_restore(ai);
#endif
  }
  return ar;
}

VALUE siren_shape_subshapes(int argc, VALUE* argv, VALUE self)
{
  VALUE ori, loc;
  rb_scan_args(argc, argv, "02", &ori, &loc);
  if (argc == 0) {
    ori = Qtrue;
    loc = Qtrue;
  }
  if (argc == 1) {
    loc = Qtrue;
  }
  VALUE ar = rb_ary_new();
  TopoDS_Shape* shape = siren_shape_get(self);
  if (!shape || shape->IsNull()) {
    return ar;
  }
  TopoDS_Iterator it(*shape, ori == Qtrue, loc == Qtrue);
  for (; it.More(); it.Next()) {
#if 0
    VALUE ai = rb_gc_arena_save();
#endif
    rb_ary_push(ar, siren_shape_new(it.Value()));
#if 0
    rb_gc_arena_restore(ai);
#endif
  }
  return ar;
}

VALUE siren_shape_section(int argc, VALUE* argv, VALUE self)
{
  VALUE other;
  rb_scan_args(argc, argv, "1", &other);

  TopoDS_Shape* S1 = siren_shape_get(self);
  TopoDS_Shape* S2 = siren_shape_get(other);

  BRepAlgoAPI_Section api(*S1, *S2, Standard_False);
  // api.ComputePCurveOn1(Standard_True);
  api.Approximation(Standard_True);
  api.Build();

  if (!api.IsDone()) {
    rb_raise(Qnil, "Failed to intersection.");
  }

  return siren_shape_new(api.Shape());
}

VALUE siren_shape_reverse(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  return siren_shape_new(shape->Reversed());
}

VALUE siren_shape_reverse_bang(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  shape->Reverse();
  return Qnil;
}

VALUE siren_shape_update_bang(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  BRepTools::Update(*shape);
  return self;
}

VALUE siren_shape_clean_bang(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  // Removes all the triangulations of the faces of <shape>
  // and removes all polygons on triangulations of the edges.
  BRepTools::Clean(*shape);
  return self;
}

VALUE siren_shape_is_lock(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Locked() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_lock(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Locked((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_modify(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Modified() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_modify(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Modified((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_check(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Checked() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_check(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Checked((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_orientable(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Orientable() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_orientable(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Orientable((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_close(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Closed() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_close(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Closed((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_infinite(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Infinite() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_infinite(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Infinite((Standard_Boolean)(flag == Qtrue));
  return self;
}

VALUE siren_shape_is_convex(int argc, VALUE* argv, VALUE self)
{
  return siren_shape_get(self)->Convex() == Standard_True ?
    Qtrue : Qfalse;
}

VALUE siren_shape_set_convex(int argc, VALUE* argv, VALUE self)
{
  VALUE flag;
  rb_scan_args(argc, argv, "1", &flag);
  siren_shape_get(self)->Convex((Standard_Boolean)(flag == Qtrue));
  return self;
}

bool siren_shape_p(const VALUE& target)
{
  return rb_funcall(target, rb_intern("is_a?"), 1, sr_cShape) == Qtrue;
}

void siren_shape_check(const VALUE& target)
{
  if (!siren_shape_p(target)) {
    VALUE type = rb_funcall(target, rb_intern("class"), 0);
    VALUE type_str = rb_funcall(type, rb_intern("to_s"), 0);
    VALUE etype_str = rb_funcall(sr_cShape, rb_intern("to_s"), 0);
    rb_raise(rb_eTypeError, "wrong argument type %s (expected %s)",
        RSTRING_PTR(type_str), RSTRING_PTR(etype_str));
  }
}

