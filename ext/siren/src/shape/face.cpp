#include "shape/face.h"

VALUE siren_face_new( const TopoDS_Shape* src)
{
  VALUE obj = rb_instance_alloc(sr_cFace);
  void* p = ruby_xmalloc(sizeof(TopoDS_Shape));
  TopoDS_Shape* inner = new(p) TopoDS_Shape();
  *inner = *src; // Copy to inner native member
  DATA_PTR(obj)  = const_cast<TopoDS_Shape*>(inner);
//  DATA_TYPE(obj) = &siren_face_type;
  return obj;
}

TopoDS_Face siren_face_get(VALUE self)
{
#if 0
  TopoDS_Shape* shape = static_cast<TopoDS_Shape*>(_get_datatype(self, &siren_face_type));
  TopoDS_Face face = TopoDS::Face(*shape);
  if (face.IsNull()) { rb_raise(Qnil, "The geometry type is not Face."); }
  return face;
#endif
}

bool siren_face_install()
{
#if 0
  struct RClass* cls_shape = siren_shape_rclass();
  struct RClass* cls_face = rb_define_class_under(sr_mSiren, "Face", cls_shape);
  MRB_SET_INSTANCE_TT(cls_face, MRB_TT_DATA);
#endif
  rb_define_method(sr_cFace, "initialize", siren_shape_init,     -1);
  rb_define_method(sr_cFace, "normal",     siren_face_normal,    -1);
  rb_define_method(sr_cFace, "to_bezier",  siren_face_to_bezier, -1);
  rb_define_method(sr_cFace, "split",      siren_face_split,     -1);
  rb_define_method(sr_cFace, "triangle",   siren_face_triangle,  -1);
  rb_define_singleton_method(sr_cFace, "plane",    siren_face_plane,    -1);
  rb_define_singleton_method(sr_cFace, "face",     siren_face_face,     -1);
  rb_define_singleton_method(sr_cFace, "infplane", siren_face_infplane, -1);
  rb_define_singleton_method(sr_cFace, "polygon",  siren_face_polygon,  -1);
  rb_define_singleton_method(sr_cFace, "bzsurf",   siren_face_bzsurf,   -1);
  rb_define_singleton_method(sr_cFace, "bssurf",   siren_face_bssurf,   -1);
  return true;
}

VALUE siren_face_normal(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Face f = siren_face_get(self);
  Standard_Real umin, umax, vmin, vmax;
  BRepTools::UVBounds(f, umin, umax, vmin, vmax);
  handle<Geom_Surface> gsurf = BRep_Tool::Surface(f);
  GeomLProp_SLProps props(gsurf, umin, vmin, 1, 0.01);
  gp_Dir n = props.Normal();
  return siren_vec_new(n.X(), n.Y(), n.Z());
}

VALUE siren_face_to_bezier(int argc, VALUE* argv, VALUE self)
{
  TopoDS_Face face = siren_face_get(self);
  handle<Geom_Surface> gsurf  = BRep_Tool::Surface(face);
  handle<Geom_BSplineSurface> gbssurf = handle<Geom_BSplineSurface>::DownCast(gsurf);
  if (gbssurf.IsNull()) {
    rb_raise(Qnil, "Specified shape is not B-Spline surface.");
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

VALUE siren_face_split(int argc, VALUE* argv, VALUE self)
{
  VALUE obj;
  rb_scan_args(argc, argv, "o", &obj);

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
    rb_raise(Qnil, "Incorrect argument specified.");
  }
  try {
    splitter.Build();
  }
  catch (...) {
    rb_raise(Qnil, "Failed to split the face.");
  }
  if (!splitter.IsDone()) {
    rb_raise(Qnil, "Failed to split the face.");
  }
  return siren_shape_new(splitter.Shape());
}

VALUE siren_face_triangle(int argc, VALUE* argv, VALUE self)
{
  VALUE deflection, angle;
  rb_scan_args(argc, argv, "ff", &deflection, &angle);

  VALUE result = rb_ary_new();

  TopoDS_Face face = siren_face_get(self);
  BRepTools::Update(face);

  BRepMesh_IncrementalMesh imesh(face, deflection, Standard_False, angle);
  imesh.Perform();
  if (!imesh.IsDone()) {
    rb_raise(Qnil, "Failed to incremantal mesh.");
  }

  TopoDS_Face face2 = TopoDS::Face(imesh.Shape());

  TopLoc_Location loc;
  // Do triangulation
  handle<Poly_Triangulation> poly = BRep_Tool::Triangulation(face2, loc);
  if (poly.IsNull()) {
    rb_raise(Qnil, "Failed to triangulation.");
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

VALUE siren_face_plane(int argc, VALUE* argv, VALUE self)
{
  VALUE pos, norm, vx;
  VALUE umin, umax, vmin, vmax;
  rb_scan_args(argc, argv, "AAAffff", &pos, &norm, &vx, &umin, &umax, &vmin, &vmax);
  try {
    gp_Pln _pln(siren_ary_to_ax2(pos, norm, vx));
    BRepBuilderAPI_MakeFace face(_pln, umin, umax, vmin, vmax);
    return siren_shape_new(face.Shape());
  }
  catch (...) {
    rb_raise(Qnil, "Failed to make a plane. "
        "vx has same value with the normal vector.");
    return Qnil;
  }
}

VALUE siren_face_face(int argc, VALUE* argv, VALUE self)
{
  VALUE wire;
  VALUE force_plane;
  rb_scan_args(argc, argv, "ob", &wire, &force_plane);
  TopoDS_Shape* s = siren_shape_get(wire);
  TopoDS_Wire w = TopoDS::Wire(*s);
  if (w.IsNull()) {
    rb_raise(Qnil, "Specified shape type is not wire.");
  }
  TopoDS_Face face = BRepBuilderAPI_MakeFace(w, (Standard_Boolean)force_plane);
  return siren_shape_new(face);
}

VALUE siren_face_infplane(int argc, VALUE* argv, VALUE self)
{
  VALUE orig, dir;
  rb_scan_args(argc, argv, "AA", &orig, &dir);
  gp_Pln pln(siren_ary_to_pnt(orig), siren_ary_to_dir(dir));
  TopoDS_Face face = BRepBuilderAPI_MakeFace(pln);
  return siren_shape_new(face);
}

VALUE siren_face_polygon(int argc, VALUE* argv, VALUE self)
{
  VALUE pts;
  VALUE force_plane = Qfalse;
  rb_scan_args(argc, argv, "A|b", &pts, &force_plane);

  BRepBuilderAPI_MakePolygon mp;

  for (int i=0; i<RARRAY_LEN(pts); i++) {
    mp.Add(siren_ary_to_pnt(RARRAY_AREF(pts, i)));
  }

  mp.Close();
  BRepBuilderAPI_MakeFace mf(mp.Wire(), force_plane);
  mf.Build();

  if (!mf.IsDone()) {
    rb_raise(Qnil, "Failed to make a polygon.");
  }

  return siren_shape_new(mf.Shape());
}

VALUE siren_face_bzsurf(int argc, VALUE* argv, VALUE self)
{
  VALUE ptary, wtary;
  rb_scan_args(argc, argv, "A|A", &ptary, &wtary);

  int rlen = RARRAY_LEN(ptary);
  int clen = RARRAY_LEN(RARRAY_AREF(ptary, 0));

  TColgp_Array2OfPnt poles(0, rlen-1, 0, clen-1);

  for (int r=0; r<rlen; r++) {
    VALUE ar = RARRAY_AREF(ptary, r);
    for (int c=0; c<clen; c++) {
      poles.SetValue(r, c, siren_ary_to_pnt(RARRAY_AREF(ar, c)));
    }
  }

  opencascade::handle<Geom_BezierSurface> s = nullptr;

  if (argc == 2) {
    TColStd_Array2OfReal weights(0, rlen-1, 0, clen-1);
    for (int r=0; r<rlen; r++) {
      VALUE ar = RARRAY_AREF(wtary, r);
      for (int c=0; c<clen; c++) {
        VALUE val = RARRAY_AREF(ar, c);
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

VALUE siren_face_bssurf(int argc, VALUE* argv, VALUE self)
{
  VALUE _udeg, _vdeg;
  VALUE _ar_ukm, _ar_vkm;
  VALUE _pol;
  VALUE _wire;
  rb_scan_args(argc, argv, "iAiAA|o", &_udeg, &_ar_ukm, &_vdeg, &_ar_vkm, &_pol, &_wire);

  bool has_contour = argc == 6;

  Standard_Integer udeg = _udeg;
  Standard_Integer nbuknots = RARRAY_LEN(_ar_ukm);
  Standard_Integer nbuknots_pure = 0;
  TColStd_Array1OfReal uknots(1, nbuknots);
  TColStd_Array1OfInteger umults(1, nbuknots);
  for (int i=1; i<=nbuknots; i++) {
    VALUE item = RARRAY_AREF(_ar_ukm, i - 1);
    VALUE knot = RARRAY_AREF(item, 0);
    VALUE mult = RARRAY_AREF(item, 1);
    uknots(i) = VALUE(knot);
    umults(i) = DBL2NUM(mult);
    nbuknots_pure += umults(i);
  }
  Standard_Integer nbupoles = nbuknots_pure - udeg - 1;

  Standard_Integer vdeg = _vdeg;
  Standard_Integer nbvknots = RARRAY_LEN(_ar_vkm);
  Standard_Integer nbvknots_pure = 0;
  TColStd_Array1OfReal vknots(1, nbvknots);
  TColStd_Array1OfInteger vmults(1, nbvknots);
  for (int i=1; i<=nbvknots; i++) {
    VALUE item = RARRAY_AREF(_ar_vkm, i - 1);
    VALUE knot = RARRAY_AREF(item, 0);
    VALUE mult = RARRAY_AREF(item, 1);
    vknots(i) = VALUE(knot);
    vmults(i) = DBL2NUM(mult);
    nbvknots_pure += vmults(i);
  }
  Standard_Integer nbvpoles = nbvknots_pure - vdeg - 1;

  TColgp_Array2OfPnt   poles  (1, nbupoles, 1, nbvpoles);
  TColStd_Array2OfReal weights(1, nbupoles, 1, nbvpoles);

  for (int v=1; v <= nbvpoles; v++) {
    VALUE vitem = RARRAY_AREF(_pol, v - 1);
    for (int u=1; u <= nbupoles; u++) {
      VALUE uitem = RARRAY_AREF(vitem, u - 1);
      poles.SetValue(u, v, siren_ary_to_pnt(RARRAY_AREF(uitem, 0)));
      weights.SetValue(u, v, VALUE(RARRAY_AREF(uitem, 1)));
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

