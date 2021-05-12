///////////////////////////////////////////////////////////////////////////////
// May 04 2013 P.Murat
// 
// in 'XY' mode draw calorimeter clusters as circles with different colors 
// in 'Cal' mode draw every detail...
///////////////////////////////////////////////////////////////////////////////
#include "TVirtualX.h"
#include "TPad.h"
#include "TStyle.h"
#include "TVector3.h"
#include "TLine.h"
#include "TArc.h"
#include "TArrow.h"
#include "TMath.h"
#include "TBox.h"
#include "TEllipse.h"
#include "TObjArray.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "art/Framework/Principal/Handle.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "TrackerGeom/inc/Straw.hh"
//#include "TrackerConditions/inc/Types.hh"

#include "Stntuple/gui/TEvdStraw.hh"
#include "Stntuple/gui/TEvdComboHit.hh"
#include "Stntuple/gui/TStnVisManager.hh"

ClassImp(stntuple::TEvdComboHit)

namespace stntuple {
//-----------------------------------------------------------------------------
TEvdComboHit::TEvdComboHit() {
}

//-----------------------------------------------------------------------------
TEvdComboHit::TEvdComboHit(const mu2e::ComboHit*      Hit,
			   double X, double Y, double Z, 
			   double                     Wx,
			   double                     Wy,
			   double                     SigW,
			   double                     SigR,
			   int                        Mask, 
			   int                        Color): 
  TObject(),
  fHit(Hit),
  fPos(X,Y,Z),
  fDir(Wx,Wy) // ,
  //   fEllipse()
 {
   // printf("TEvdComboHit::TEvdComboHit: StrawDidiMC::driftDistance disabled. ask Dave Brown\n");

  fSigW  = SigW;
  fSigR  = SigR;
  fMask  = Mask;
  fColor = Color;
//-----------------------------------------------------------------------------
// define lines for XY view
//-----------------------------------------------------------------------------
  fLineW.SetX1(fPos.X()-fDir.X()*fSigW);
  fLineW.SetY1(fPos.Y()-fDir.Y()*fSigW);
  fLineW.SetX2(fPos.X()+fDir.X()*fSigW);
  fLineW.SetY2(fPos.Y()+fDir.Y()*fSigW);
  fLineW.SetLineColor(Color);

  fLineR.SetX1(fPos.X()+fDir.Y()*fSigR);
  fLineR.SetY1(fPos.Y()-fDir.X()*fSigR);
  fLineR.SetX2(fPos.X()-fDir.Y()*fSigR);
  fLineR.SetY2(fPos.Y()+fDir.X()*fSigR);
  fLineR.SetLineColor(Color);

  //  const CLHEP::Hep3Vector* mid_point = &fStraw->GetStraw()->getMidPoint();
  
  // double rdrift(2.);
//-----------------------------------------------------------------------------
// RZ view - for straw hits
//-----------------------------------------------------------------------------
  // fEllipse.SetX1(mid_point->z());
  // fEllipse.SetY1(mid_point->perp());
  // fEllipse.SetR1(rdrift);
  // fEllipse.SetR2(rdrift);
  // fEllipse.SetFillStyle(3003);
  // fEllipse.SetFillColor(kBlue+2);
  // fEllipse.SetLineColor(kBlue+2);

  // fXYMarker.SetX(fX);
  // fXYMarker.SetY(fY);
  // fXYMarker.SetMarkerStyle(style);
  // fXYMarker.SetMarkerSize (size );
  // fXYMarker.SetMarkerColor(color);
//-----------------------------------------------------------------------------
// TZ view
//-----------------------------------------------------------------------------
  fTZMarker.SetX(Z);
  fTZMarker.SetY(Hit->time());
  fTZMarker.SetMarkerStyle(20);
  fTZMarker.SetMarkerSize (1 );
  fTZMarker.SetMarkerColor(Color);
}

//-----------------------------------------------------------------------------
TEvdComboHit::~TEvdComboHit() {
}

//-----------------------------------------------------------------------------
void TEvdComboHit::Paint(Option_t* Option) {
  const char oname[] = "TEvdComboHit::Paint";

  //  int   iv;

  int view = TVisManager::Instance()->GetCurrentView()->Type();

  if      (view == TStnView::kXY ) PaintXY (Option);
  else if (view == TStnView::kTZ ) PaintTZ (Option);
  else if (view == TStnView::kCal) PaintCal(Option);
  else {
    printf("[%s] >>> ERROR: unknown view: %i, DO NOTHING\n",oname,view);
  }

  gPad->Modified();
}

//_____________________________________________________________________________
void TEvdComboHit::PaintXY(Option_t* Option) {
  fLineW.Paint(Option);
  fLineR.Paint(Option);
}

//_____________________________________________________________________________
void TEvdComboHit::PaintTZ(Option_t* Option) {
  fTZMarker.Paint(Option);
}

// //-----------------------------------------------------------------------------
// void TEvdComboHit::PaintRZ(Option_t* Option) {
//   fEllipse.SetFillColor(kBlue+2);
//   fEllipse.SetFillStyle(3001);
//   fEllipse.Paint(Option);
// }

//-----------------------------------------------------------------------------
void TEvdComboHit::PaintCal(Option_t* option) {
  // nothing to draw...
}


// //_____________________________________________________________________________
// Int_t TEvdComboHit::DistancetoPrimitive(Int_t px, Int_t py) {
//   return 9999;
// }

//_____________________________________________________________________________
Int_t TEvdComboHit::DistancetoPrimitiveXY(Int_t px, Int_t py) {

  Int_t dist = 9999;

  static TVector3 global;
//   static TVector3 local;

//   Double_t    dx1, dx2, dy1, dy2, dx_min, dy_min, dr;

  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  return dist;
}

//_____________________________________________________________________________
Int_t TEvdComboHit::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  return 9999;
}

}
