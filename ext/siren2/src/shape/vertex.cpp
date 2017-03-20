#include "shape/vertex.h"

#define rb_array_p(x) RB_TYPE_P(x, T_ARRAY)

VALUE sr_cVertex;

SR_SHAPE_INIT(Vertex, vertex)

bool siren_vertex_install()
{
  SR_SHAPE_DEFINE(Vertex)
  rb_define_method(sr_cVertex, "initialize", RUBY_METHOD_FUNC(siren_vertex_init), -1);
  rb_define_method(sr_cVertex, "xyz",        RUBY_METHOD_FUNC(siren_vertex_xyz),  -1);
  rb_define_method(sr_cVertex, "to_a",       RUBY_METHOD_FUNC(siren_vertex_xyz),  -1);
  rb_define_method(sr_cVertex, "to_v",       RUBY_METHOD_FUNC(siren_vertex_to_v), -1);
  return true;
}

VALUE siren_vertex_init(int argc, VALUE* argv, VALUE self)
{
  VALUE a, y, z;
  rb_scan_args(argc, argv, "12", &a, &y, &z);
  gp_Pnt pnt;
  if (argc == 1) {
    pnt = siren_ary_to_pnt(a);
  }
  else if (argc == 3) {
    siren_numeric_check(a);
    siren_numeric_check(y);
    siren_numeric_check(z);
    pnt = gp_Pnt(NUM2DBL(a), NUM2DBL(y), NUM2DBL(z));
  }
  else {
    rb_raise(Qnil, "wrong number of arguments specified.");
  }
  TopoDS_Vertex v = BRepBuilderAPI_MakeVertex(pnt);
  auto p = siren_shape_get(self);
  *p = v;
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
