#include "shape/vertex.h"

#define rb_array_p(x) RB_TYPE_P(x, T_ARRAY)

VALUE sr_cVertex;

VALUE siren_vertex_new( const TopoDS_Shape* src)
{
  VALUE obj = rb_instance_alloc(sr_cVertex);
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(obj) = &siren_vertex_type;
  return obj;
}

TopoDS_Vertex siren_vertex_get(VALUE self)
{
#if 0
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_vertex_type));
  TopoDS_Vertex vertex = TopoDS::Vertex(*shape);
  if (vertex.IsNull()) { rb_raise(Qnil, "The geometry type is not Vertex."); }
  return vertex;
#endif
}

bool siren_vertex_install()
{
#if 0
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_vertex = rb_define_class_under(sr_mSiren, "Vertex", cls_shape);
  MRB_SET_INSTANCE_TT(cls_vertex, MRB_TT_DATA);
#endif
  rb_define_method(sr_cVertex, "initialize", RUBY_METHOD_FUNC(siren_vertex_init), -1);
  rb_define_method(sr_cVertex, "xyz",        RUBY_METHOD_FUNC(siren_vertex_xyz),  -1);
  rb_define_method(sr_cVertex, "to_a",       RUBY_METHOD_FUNC(siren_vertex_xyz),  -1);
  rb_define_method(sr_cVertex, "to_v",       RUBY_METHOD_FUNC(siren_vertex_to_v), -1);
  return true;
}

VALUE siren_vertex_init(int argc, VALUE* argv, VALUE self)
{
  VALUE* a;
  VALUE len;
  rb_scan_args(argc, argv, "*", &a, &len);

  Standard_Real x = 0.0, y = 0.0, z = 0.0;
  if (len > 0 && rb_array_p(a[0])) {
    gp_Pnt p = siren_ary_to_pnt(a[0]);
    x = p.X(); y = p.Y(); z = p.Z();
  }
  else {
    if (len >= 1) {
      if (FIXNUM_P(a[0]))
        x = DBL2NUM(a[0]);
      else if (RB_FLOAT_TYPE_P(a[0]))
        x = VALUE(a[0]);
    }
    if (len >= 2) {
      if (FIXNUM_P(a[1]))
        y = DBL2NUM(a[1]);
      else if (RB_FLOAT_TYPE_P(a[1]))
        y = VALUE(a[1]);
    }
    if (len >= 3) {
      if (FIXNUM_P(a[2]))
        z = DBL2NUM(a[2]);
      else if (RB_FLOAT_TYPE_P(a[2]))
        z = VALUE(a[2]);
    }
  }
  TopoDS_Vertex v = BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, z));

  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = v; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(self) = &siren_vertex_type;
  return self;
}

VALUE siren_vertex_xyz(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Vertex vertex = siren_vertex_get(self);
  return siren_pnt_to_ary(BRep_Tool::Pnt(vertex));
}

VALUE siren_vertex_to_v(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Vertex vertex = siren_vertex_get(self);
  gp_Pnt p = BRep_Tool::Pnt(vertex);
  return siren_vec_new(p.X(), p.Y(), p.Z());
}