#include "heal.h"

VALUE siren_heal_outerwire(VALUE self, VALUE tol)
{
#if 0
  VALUE tol = 1.0e-1;
  rb_scan_args(argc, argv, "|f", &tol);
#else
  if (tol == Qnil) {
    tol = 1.0e-1;
  }
#endif

  TopoDS_Shape* shape = siren_shape_get(self);

  VALUE res = Qnil;

  if (shape->ShapeType() == TopAbs_FACE) {
    TopoDS_Face face = TopoDS::Face(*shape);
    TopoDS_Wire wire = ShapeAnalysis::OuterWire(face);
    // ShapeAnalysis_FreeBounds
    // ::ConnectWiresToWires
    res = siren_shape_new(wire);
  }
  else {
#if 0
    ShapeAnalysis_FreeBounds safb(*shape, tol);
#else
    ShapeAnalysis_FreeBounds safb(*shape, NUM2DBL(tol));
#endif
    TopoDS_Compound comp = safb.GetClosedWires();
    res = siren_shape_new(comp);
  }
  return res;
}

VALUE siren_heal_fix( VALUE self)
{
  TopoDS_Shape* shape = siren_shape_get(self);
  VALUE res = Qnil;

  opencascade::handle<ShapeFix_Shape> sfs = new ShapeFix_Shape();
  sfs->Init(*shape);
  sfs->SetPrecision(1.0);
  sfs->SetMinTolerance(1.0e-6);
  sfs->SetMaxTolerance(1.0e-1);

  switch (shape->ShapeType()) {
    case TopAbs_SOLID:
      sfs->FixSolidTool()->FixShellMode() = 1;
    case TopAbs_SHELL:
      sfs->FixShellTool()->FixFaceMode() = 1;
      sfs->FixShellTool()->FixOrientationMode() = 1;
    case TopAbs_FACE:
      sfs->FixFaceTool()->FixAddNaturalBoundMode() = 1;
      sfs->FixFaceTool()->FixIntersectingWiresMode() = 1;
      sfs->FixFaceTool()->FixLoopWiresMode() = 1;
      sfs->FixFaceTool()->FixOrientationMode() = 1;
      sfs->FixFaceTool()->FixPeriodicDegeneratedMode() = 1;
      sfs->FixFaceTool()->FixSmallAreaWireMode() = 1;
      sfs->FixFaceTool()->FixSplitFaceMode() = 1;
      sfs->FixFaceTool()->FixWireMode() = 1;
    case TopAbs_WIRE:
      //sfs->FixWireTool()->FixAddCurve3dMode() = 1;
      //sfs->FixWireTool()->FixAddPCurveMode() = 1;
      sfs->FixWireTool()->FixConnectedMode() = 1;
      sfs->FixWireTool()->FixDegeneratedMode() = 1;
      sfs->FixWireTool()->FixEdgeCurvesMode() = 1;
      //sfs->FixWireTool()->FixGaps2dMode() = 1;
      sfs->FixWireTool()->FixGaps3dMode() = 1;
      sfs->FixWireTool()->FixGapsByRangesMode() = 1;
      sfs->FixWireTool()->FixIntersectingEdgesMode() = 1;
      sfs->FixWireTool()->FixLackingMode() = 1;
      sfs->FixWireTool()->FixNonAdjacentIntersectingEdgesMode() = 1;
      sfs->FixWireTool()->FixNotchedEdgesMode() = 1;
      //sfs->FixWireTool()->FixRemoveCurve3dMode() = 1;
      //sfs->FixWireTool()->FixRemovePCurveMode() = 1;
      sfs->FixWireTool()->FixReorderMode() = 1;
      //sfs->FixWireTool()->FixReversed2dMode() = 1;
      sfs->FixWireTool()->FixSameParameterMode() = 1;
      sfs->FixWireTool()->FixSeamMode() = 1;
      sfs->FixWireTool()->FixSelfIntersectingEdgeMode() = 1;
      sfs->FixWireTool()->FixSelfIntersectionMode() = 1;
      sfs->FixWireTool()->FixShiftedMode() = 1;
      sfs->FixWireTool()->FixSmallMode() = 1;
      sfs->FixWireTool()->FixVertexToleranceMode() = 1;
    default:
      sfs->FixVertexPositionMode() = 1;
  }

  sfs->Perform();

  TopoDS_Shape fixedshape = sfs->Shape();
  if (!fixedshape.IsNull()) {
    res = siren_shape_new(fixedshape);
  }

  return res;
}

