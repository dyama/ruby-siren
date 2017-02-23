#include "shape/vertex.h"

VALUE siren_vertex_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Vertex")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_vertex_type;
  return obj;
}

TopoDS_Vertex siren_vertex_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_vertex_type));
  TopoDS_Vertex vertex = TopoDS::Vertex(*shape);
  if (vertex.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Vertex."); }
  return vertex;
}

bool siren_vertex_install()
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_vertex = rb_define_class_under(sr_mSiren, "Vertex", cls_shape);
  MRB_SET_INSTANCE_TT(cls_vertex, MRB_TT_DATA);
  rb_define_method(cls_vertex, "initialize", siren_vertex_init, MRB_ARGS_NONE());
  rb_define_method(cls_vertex, "xyz",        siren_vertex_xyz,  MRB_ARGS_NONE());
  rb_define_method(cls_vertex, "to_a",       siren_vertex_xyz,  MRB_ARGS_NONE());
  rb_define_method(cls_vertex, "to_v",       siren_vertex_to_v, MRB_ARGS_NONE());
  return true;
}

struct RClass* siren_vertex_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Vertex")));
}

VALUE siren_vertex_init( VALUE self)
{
  VALUE* a;
  VALUE len;
  int argc = rb_scan_args("*", &a, &len);

  Standard_Real x = 0.0, y = 0.0, z = 0.0;
  if (len > 0 && rb_array_p(a[0])) {
    gp_Pnt p = siren_ary_to_pnt(a[0]);
    x = p.X(); y = p.Y(); z = p.Z();
  }
  else {
    if (len >= 1) {
      if (_fixnum_p(a[0]))
        x = rb_fixnum(a[0]);
      else if VALUE_p(a[0])
        x = VALUE(a[0]);
    }
    if (len >= 2) {
      if (_fixnum_p(a[1]))
        y = rb_fixnum(a[1]);
      else if (_float_p(a[1]))
        y = VALUE(a[1]);
    }
    if (len >= 3) {
      if (_fixnum_p(a[2]))
        z = rb_fixnum(a[2]);
      else if (_float_p(a[2]))
        z = VALUE(a[2]);
    }
  }
  TopoDS_Vertex v = BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, z));

  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = v; // Copy to inner native member
  DATA_PTR(self)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(self) = &siren_vertex_type;
  return self;
}

VALUE siren_vertex_xyz( VALUE self)
{
  TopoDS_Vertex vertex = siren_vertex_get(self);
  return siren_pnt_to_ary(BRep_Tool::Pnt(vertex));
}

VALUE siren_vertex_to_v( VALUE self)
{
  TopoDS_Vertex vertex = siren_vertex_get(self);
  gp_Pnt p = BRep_Tool::Pnt(vertex);
  return siren_vec_new(p.X(), p.Y(), p.Z());
}

VALUE siren_vertex_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Vertex"));
}

