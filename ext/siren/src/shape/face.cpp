#include "shape/face.h"

VALUE siren_face_new( const TopoDS_Shape* src)
{
  VALUE obj;
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* sr_mSiren = rb_module_get("Siren");
  obj = rb_instance_alloc(rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Face")));
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
  DATA_TYPE(obj) = &siren_face_type;
  return obj;
}

TopoDS_Face siren_face_get( VALUE self)
{
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_face_type));
  TopoDS_Face face = TopoDS::Face(*shape);
  if (face.IsNull()) { rb_raise(E_RUNTIME_ERROR, "The geometry type is not Face."); }
  return face;
}

bool siren_face_install( struct RClass* sr_mSiren)
{
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_face = rb_define_class_under(sr_mSiren, "Face", cls_shape);
  MRB_SET_INSTANCE_TT(cls_face, MRB_TT_DATA);
  rb_define_method(cls_face, "initialize", siren_shape_init,     MRB_ARGS_NONE());
  rb_define_method(cls_face, "normal",     siren_face_normal,    MRB_ARGS_NONE());
  rb_define_method(cls_face, "to_bezier",  siren_face_to_bezier, MRB_ARGS_NONE());
  rb_define_method(cls_face, "split",      siren_face_split,     MRB_ARGS_REQ(1));
  rb_define_method(cls_face, "triangle",   siren_face_triangle,  MRB_ARGS_REQ(2));

  auto obj_face = rb_obj_ptr(siren_face_obj());
  rb_define_singleton_method(obj_face, "plane",    siren_face_plane,    MRB_ARGS_REQ(7));
  rb_define_singleton_method(obj_face, "face",     siren_face_face,     MRB_ARGS_REQ(2));
  rb_define_singleton_method(obj_face, "infplane", siren_face_infplane, MRB_ARGS_REQ(2));
  rb_define_singleton_method(obj_face, "polygon",  siren_face_polygon,  MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_singleton_method(obj_face, "bzsurf",   siren_face_bzsurf,   MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
  rb_define_singleton_method(obj_face, "bssurf",   siren_face_bssurf,   MRB_ARGS_REQ(5) | MRB_ARGS_OPT(1));
  return true;
}

struct RClass* siren_face_rclass()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_class_ptr(_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Face")));
}

VALUE siren_face_normal( VALUE self)
{
  TopoDS_Face f = siren_face_get(self);
  Standard_Real umin, umax, vmin, vmax;
  BRepTools::UVBounds(f, umin, umax, vmin, vmax);
  handle<Geom_Surface> gsurf = BRep_Tool::Surface(f);
  GeomLProp_SLProps props(gsurf, umin, vmin, 1, 0.01);
  gp_Dir n = props.Normal();
  return siren_vec_new(n.X(), n.Y(), n.Z());
}

VALUE siren_face_to_bezier( VALUE self)
{
  TopoDS_Face face = siren_face_get(self);
  handle<Geom_Surface> gsurf  = BRep_Tool::Surface(face);
  handle<Geom_BSplineSurface> gbssurf = handle<Geom_BSplineSurface>::DownCast(gsurf);
  if (gbssurf.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Specified shape is not B-Spline surface.");
  }

  TopoDS_Compound comp;
  BRep_Builder B;
  B.MakeCompound(comp);

  GeomConvert_BSplineSurfaceToBezierSurface converter(gbssurf);

  TColGeom_Array2OfBezierSurface ary(1, converter.NbUPatches(), 1, converter.NbVPatches());
  converter.Patches(ary);

  for (int r = ary.LowerRow(); r <= ary.UpperRow(); r++) {
    for (int c = ary.LowerCol(); c <= ary.UpperCol(); c++) {
      handle<Geom_BezierSurface> gbzsurf = ary.Value(r, c);
      TopoDS_Face patch = BRepBuilderAPI_MakeFace(gbzsurf, 1.0e-1);
      B.Add(comp, patch);
    }
  }

  return siren_shape_new(comp);
}

VALUE siren_face_split( VALUE self)
{
  VALUE obj;
  int argc = rb_scan_args("o", &obj);

  TopoDS_Face face = siren_face_get(self);
  BRepFeat_SplitShape splitter(face);

  TopoDS_Shape shape = *siren_shape_get(obj);
  switch (shape.ShapeType()) {
  case TopAbs_WIRE:
    splitter.Add(TopoDS::Wire(shape), face);
    break;
  case TopAbs_EDGE:
    splitter.Add(TopoDS::Edge(shape), face);
    break;
  case TopAbs_COMPOUND:
    splitter.Add(TopoDS::Compound(shape), face);
    break;
  default:
    rb_raise(E_ARGUMENT_ERROR, "Incorrect argument specified.");
  }
  try {
    splitter.Build();
  }
  catch (...) {
    rb_raise(E_RUNTIME_ERROR, "Failed to split the face.");
  }
  if (!splitter.IsDone()) {
    rb_raise(E_RUNTIME_ERROR, "Failed to split the face.");
  }
  return siren_shape_new(splitter.Shape());
}

VALUE siren_face_triangle( VALUE self)
{
  VALUE deflection, angle;
  int argc = rb_scan_args("ff", &deflection, &angle);

  VALUE result = rb_ary_new();

  TopoDS_Face face = siren_face_get(self);
  BRepTools::Update(face);

  BRepMesh_IncrementalMesh imesh(face, deflection, Standard_False, angle);
  imesh.Perform();
  if (!imesh.IsDone()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to incremantal mesh.");
  }

  TopoDS_Face face2 = TopoDS::Face(imesh.Shape());

  TopLoc_Location loc;
  // Do triangulation
  handle<Poly_Triangulation> poly = BRep_Tool::Triangulation(face2, loc);
  if (poly.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to triangulation.");
  }

  const Poly_Array1OfTriangle& tris = poly->Triangles();

  for (Standard_Integer i = tris.Lower(); i <= tris.Upper(); i++) {

    const Poly_Triangle& tri = tris.Value(i);

    // Node indexes
    Standard_Integer n1, n2, n3;
    if (face2.Orientation() != TopAbs_REVERSED) {
      tri.Get(n1, n2, n3);
    }
    else {
      tri.Get(n3, n2, n1);
    }

    gp_Pnt p1 = poly->Nodes().Value(n1);
    gp_Pnt p2 = poly->Nodes().Value(n2);
    gp_Pnt p3 = poly->Nodes().Value(n3);

    p1.Transform(loc);
    p2.Transform(loc);
    p3.Transform(loc);

    gp_Vec u = gp_Vec(p2.XYZ() - p1.XYZ());
    gp_Vec v = gp_Vec(p3.XYZ() - p1.XYZ());

    gp_Vec norm(
        u.Y() * v.Z() - u.Z() * v.Y(),
        u.Z() * v.X() - u.X() * v.Z(),
        u.X() * v.Y() - u.Y() * v.X());
    if (norm.Magnitude() <= 0) {
      continue;
    }
    norm.Normalize();

    VALUE trimesh = rb_ary_new();
    rb_ary_push(trimesh, siren_pnt_to_ary(p1));
    rb_ary_push(trimesh, siren_pnt_to_ary(p2));
    rb_ary_push(trimesh, siren_pnt_to_ary(p3));
    rb_ary_push(trimesh, siren_vec_to_ary(u));
    rb_ary_push(trimesh, siren_vec_to_ary(v));
    rb_ary_push(trimesh, siren_vec_to_ary(norm));

    rb_ary_push(result, trimesh);
  }

  return result;
}

VALUE siren_face_obj()
{
  struct RClass* sr_mSiren = rb_module_get("Siren");
  return rb_const_get(rb_obj_value(sr_mSiren), rb_intern_lit("Face"));
}

VALUE siren_face_plane( VALUE self)
{
  VALUE pos, norm, vx;
  VALUE umin, umax, vmin, vmax;
  int argc = rb_scan_args("AAAffff", &pos, &norm, &vx, &umin, &umax, &vmin, &vmax);
  try {
    gp_Pln _pln(siren_ary_to_ax2(pos, norm, vx));
    BRepBuilderAPI_MakeFace face(_pln, umin, umax, vmin, vmax);
    return siren_shape_new(face.Shape());
  }
  catch (...) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a plane. "
        "vx has same value with the normal vector.");
    return Qnil;
  }
}

VALUE siren_face_face( VALUE self)
{
  VALUE wire;
  VALUE force_plane;
  int argc = rb_scan_args("ob", &wire, &force_plane);
  TopoDS_Shape* s = siren_shape_get(wire);
  TopoDS_Wire w = TopoDS::Wire(*s);
  if (w.IsNull()) {
    rb_raise(E_ARGUMENT_ERROR, "Specified shape type is not wire.");
  }
  TopoDS_Face face = BRepBuilderAPI_MakeFace(w, (Standard_Boolean)force_plane);
  return siren_shape_new(face);
}

VALUE siren_face_infplane( VALUE self)
{
  VALUE orig, dir;
  int argc = rb_scan_args("AA", &orig, &dir);
  gp_Pln pln(siren_ary_to_pnt(orig), siren_ary_to_dir(dir));
  TopoDS_Face face = BRepBuilderAPI_MakeFace(pln);
  return siren_shape_new(face);
}

VALUE siren_face_polygon( VALUE self)
{
  VALUE pts;
  VALUE force_plane = (_bool)Standard_True;
  int argc = rb_scan_args("A|b", &pts, &force_plane);

  BRepBuilderAPI_MakePolygon mp;

  for (int i=0; i<rb_ary_len(pts); i++) {
    mp.Add(siren_ary_to_pnt(rb_ary_ref(pts, i)));
  }

  mp.Close();
  BRepBuilderAPI_MakeFace mf(mp.Wire(), force_plane);
  mf.Build();

  if (!mf.IsDone()) {
    rb_raise(E_ARGUMENT_ERROR, "Failed to make a polygon.");
  }

  return siren_shape_new(mf.Shape());
}

VALUE siren_face_bzsurf( VALUE self)
{
  VALUE ptary, wtary;
  int argc = rb_scan_args("A|A", &ptary, &wtary);

  int rlen = rb_ary_len(ptary);
  int clen = rb_ary_len(rb_ary_ref(ptary, 0));

  TColgp_Array2OfPnt poles(0, rlen-1, 0, clen-1);

  for (int r=0; r<rlen; r++) {
    VALUE ar = rb_ary_ref(ptary, r);
    for (int c=0; c<clen; c++) {
      poles.SetValue(r, c, siren_ary_to_pnt(rb_ary_ref(ar, c)));
    }
  }

  opencascade::handle<Geom_BezierSurface> s = nullptr;

  if (argc == 2) {
    TColStd_Array2OfReal weights(0, rlen-1, 0, clen-1);
    for (int r=0; r<rlen; r++) {
      VALUE ar = rb_ary_ref(wtary, r);
      for (int c=0; c<clen; c++) {
        VALUE val = rb_ary_ref(ar, c);
        weights.SetValue(r, c, VALUE(val));
      }
    }
    s = new Geom_BezierSurface(poles, weights);
  }
  else {
    s = new Geom_BezierSurface(poles);
  }

  return siren_shape_new(BRepBuilderAPI_MakeFace(s, 1.0e-7));
}

VALUE siren_face_bssurf( VALUE self)
{
  VALUE _udeg, _vdeg;
  VALUE _ar_ukm, _ar_vkm;
  VALUE _pol;
  VALUE _wire;
  int argc = rb_scan_args("iAiAA|o", &_udeg, &_ar_ukm, &_vdeg, &_ar_vkm, &_pol, &_wire);

  bool has_contour = argc == 6;

  Standard_Integer udeg = _udeg;
  Standard_Integer nbuknots = rb_ary_len(_ar_ukm);
  Standard_Integer nbuknots_pure = 0;
  TColStd_Array1OfReal uknots(1, nbuknots);
  TColStd_Array1OfInteger umults(1, nbuknots);
  for (int i=1; i<=nbuknots; i++) {
    VALUE item = rb_ary_ref(_ar_ukm, i - 1);
    VALUE knot = rb_ary_ref(item, 0);
    VALUE mult = rb_ary_ref(item, 1);
    uknots(i) = VALUE(knot);
    umults(i) = rb_fixnum(mult);
    nbuknots_pure += umults(i);
  }
  Standard_Integer nbupoles = nbuknots_pure - udeg - 1;

  Standard_Integer vdeg = _vdeg;
  Standard_Integer nbvknots = rb_ary_len(_ar_vkm);
  Standard_Integer nbvknots_pure = 0;
  TColStd_Array1OfReal vknots(1, nbvknots);
  TColStd_Array1OfInteger vmults(1, nbvknots);
  for (int i=1; i<=nbvknots; i++) {
    VALUE item = rb_ary_ref(_ar_vkm, i - 1);
    VALUE knot = rb_ary_ref(item, 0);
    VALUE mult = rb_ary_ref(item, 1);
    vknots(i) = VALUE(knot);
    vmults(i) = rb_fixnum(mult);
    nbvknots_pure += vmults(i);
  }
  Standard_Integer nbvpoles = nbvknots_pure - vdeg - 1;

  TColgp_Array2OfPnt   poles  (1, nbupoles, 1, nbvpoles);
  TColStd_Array2OfReal weights(1, nbupoles, 1, nbvpoles);

  for (int v=1; v <= nbvpoles; v++) {
    VALUE vitem = rb_ary_ref(_pol, v - 1);
    for (int u=1; u <= nbupoles; u++) {
      VALUE uitem = rb_ary_ref(vitem, u - 1);
      poles.SetValue(u, v, siren_ary_to_pnt(rb_ary_ref(uitem, 0)));
      weights.SetValue(u, v, VALUE(_ary_ref(uitem, 1)));
    }
  }

  opencascade::handle<Geom_BSplineSurface> hg_bssurf = new Geom_BSplineSurface(poles, weights, uknots, vknots, umults, vmults, udeg, vdeg);
  TopoDS_Shape shape;
  if (has_contour) {
    TopoDS_Shape* s = siren_shape_get(_wire);
    TopoDS_Wire w = TopoDS::Wire(*s);
    shape = BRepBuilderAPI_MakeFace(hg_bssurf, w, Standard_True);
    // Fix a face
    opencascade::handle<ShapeFix_Shape> sfs = new ShapeFix_Shape();
    sfs->Init(shape);
    sfs->FixFaceTool()->FixAddNaturalBoundMode() = 1;
    sfs->FixFaceTool()->FixIntersectingWiresMode() = 1;
    sfs->FixFaceTool()->FixLoopWiresMode() = 1;
    sfs->FixFaceTool()->FixOrientationMode() = 1;
    sfs->FixFaceTool()->FixPeriodicDegeneratedMode() = 1;
    sfs->FixFaceTool()->FixSmallAreaWireMode() = 1;
    sfs->FixFaceTool()->FixSplitFaceMode() = 1;
    sfs->FixFaceTool()->FixWireMode() = 1;
    sfs->SetPrecision(1.0);
    sfs->SetMinTolerance(1.0e-1);
    sfs->SetMaxTolerance(1.0);
    sfs->Perform();
    shape = sfs->Shape();
    // End of fix
  }
  else {
    Standard_Real toldegen = 1.0e-1;
    shape = BRepBuilderAPI_MakeFace(hg_bssurf, toldegen);
  }

  return siren_shape_new(shape);
}

