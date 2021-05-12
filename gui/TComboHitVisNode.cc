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
#include "TBox.h"
// #include "art/Framework/Principal/Event.h"
// #include "art/Framework/Principal/Handle.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "ConditionsService/inc/ConditionsHandle.hh"
#include "TrackerConditions/inc/StrawResponse.hh"

#include "Stntuple/gui/TComboHitVisNode.hh"
#include "Stntuple/gui/TEvdComboHit.hh"

#include "Stntuple/gui/TStnVisManager.hh"

ClassImp(TComboHitVisNode)

//-----------------------------------------------------------------------------
TComboHitVisNode::TComboHitVisNode() : TVisNode("TComboHitVisNode") {
}

//_____________________________________________________________________________
TComboHitVisNode::TComboHitVisNode(const char* Name): TVisNode(Name) {
  fListOfHits = new TObjArray();
  fListOfHits->SetOwner(1);
}

//-----------------------------------------------------------------------------
TComboHitVisNode::~TComboHitVisNode() {
  
  delete fListOfHits;
}

//-----------------------------------------------------------------------------
int TComboHitVisNode::InitEvent() {

  //  const char* oname = "TComboHitVisNode::InitEvent";

  // mu2e::GeomHandle<mu2e::Tracker> ttHandle;
  // const mu2e::Tracker* tracker = ttHandle.get();

  // Tracker calibration object.
  //  mu2e::ConditionsHandle<mu2e::StrawResponse> srep = mu2e::ConditionsHandle<mu2e::StrawResponse>("ignored");

  fListOfHits->Delete();

  const mu2e::ComboHit              *hit;
  //  const mu2e::StrawDigiMC           *hit_digi_mc;

  const CLHEP::Hep3Vector           /**mid,*/ *w; 
  const mu2e::Straw                 *straw(nullptr); 

  int                               color(0); // display_hit, nl, ns; // , ipeak, ihit;
  //  bool                              isFromConversion; // , intime;
  double                            sigw(0), /*vnorm, v,*/ sigr(0); 
  CLHEP::Hep3Vector                 vx0, vx1, vx2;
//-----------------------------------------------------------------------------
// display hits corresponding to a given time peak, or all hits, 
// if the time peak is not found
//-----------------------------------------------------------------------------
  // stntuple::TEvdStraw* evd_straw;

  int nhits = (*fHitColl)->size();
  for (int ihit=0; ihit<nhits; ihit++ ) {

    hit         = &(*fHitColl)->at(ihit);
//-----------------------------------------------------------------------------
// deal with MC information - later
//-----------------------------------------------------------------------------
    w     = &straw->getDirection(); // let it crash

    // isFromConversion = false;
//-----------------------------------------------------------------------------
// add a pointer to the hit to the straw 
//-----------------------------------------------------------------------------
    int mask = 0;
    stntuple::TEvdComboHit* evd_ch;

    evd_ch = new stntuple::TEvdComboHit(hit,
					hit->pos().x(),hit->pos().y(),hit->pos().z(),
					w->x(),w->y(),sigw,sigr,mask,color);
    fListOfHits->Add(evd_ch);
  }

  return 0;
}

//-----------------------------------------------------------------------------
// draw reconstructed tracks and straw hits
//-----------------------------------------------------------------------------
void TComboHitVisNode::PaintXY(Option_t* Option) {

  //  double        time;
  // int           station, ntrk(0);
  stntuple::TEvdComboHit  *hit;

  // const mu2e::ComboHit   *combo_hit;
  // const mu2e::Straw      *straw; 

  //  const char* view = TVisManager::Instance()->GetCurrentView();

  //  TStnVisManager* vm = TStnVisManager::Instance();

  //  int ipeak = vm->TimeCluster();

  // if (ipeak >= 0) {
  //   if ((*fTimeClusterColl) != NULL) {
  //     int ntp = (*fTimeClusterColl)->size();
  //     if (ipeak < ntp) fTimeCluster = &(*fTimeClusterColl)->at(ipeak);
  //     else             fTimeCluster = NULL;
  //   }
  // }

  // double tmin(0), tmax(2000.);

  // if (fTimeCluster) {
  //   tmin = fTimeCluster->t0().t0() - 30; // FIXME!
  //   tmax = fTimeCluster->t0().t0() + 20; // FIXME!
  // }
  
  int nhits = fListOfHits->GetEntries();
  for (int i=0; i<nhits; i++) {
    hit       = (stntuple::TEvdComboHit*) fListOfHits->At(i);
    //    time      = hit->time();

    // if ((station >= vm->MinStation()) && (station <= vm->MaxStation())) { 
    //   if ((time >= tmin) && (time <= tmax)) {
    hit->Paint(Option);
    //   }
    // }
  }

  gPad->Modified();
}

//-----------------------------------------------------------------------------
// draw reconstructed tracks and straw hits
//-----------------------------------------------------------------------------
void TComboHitVisNode::PaintTZ(Option_t* Option) {

  int nhits = fListOfHits->GetEntries();
  for (int i=0; i<nhits; i++) {
    stntuple::TEvdComboHit* hit = (stntuple::TEvdComboHit*) fListOfHits->At(i);

    // float time  = hit->Time();

    //    if ((time >= tmin) && (time <= tmax)) {
    hit->Paint(Option);
    // }
  }

  gPad->Modified();
}

// //-----------------------------------------------------------------------------
// void TComboHitVisNode::PaintRZ(Option_t* Option) {
//   int                       ntrk(0), nhits;
//   stntuple::TEvdTrack*      evd_trk;

//   //  TStnVisManager* vm = TStnVisManager::Instance();

//   fTracker->PaintRZ(Option);
// //-----------------------------------------------------------------------------
// // do not draw all straw hits - just redraw straws in different color instead
// //-----------------------------------------------------------------------------
// //   int nhits = fListOfStrawHits->GetEntries();
// //   for (int i=0; i<nhits; i++) {
// //     hit       = (TEvdStrawHit*) fListOfStrawHits->At(i);

// //     if ((station >= vm->MinStation()) && (station <= vm->MaxStation())) continue;
// //     if ((time    <  vm->TMin()      ) || (time     > vm->TMax()      )) continue; 

// //     hit->Paint(Option);
// //   }
// //-----------------------------------------------------------------------------
// // display tracks and track hits
// //-----------------------------------------------------------------------------

//   gPad->Modified();
// }

//-----------------------------------------------------------------------------
int TComboHitVisNode::DistancetoPrimitiveXY(Int_t px, Int_t py) {
  static TVector3 global;
  global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  TObject* closest(nullptr);

  int  x1, y1, dx1, dy1, min_dist(9999), dist;

  int nhits = fListOfHits->GetEntries();
  for (int i=0; i<nhits; i++) {
    stntuple::TEvdComboHit* hit = (stntuple::TEvdComboHit*) fListOfHits->At(i);

    x1  = gPad->XtoAbsPixel(hit->Pos()->X());
    y1  = gPad->YtoAbsPixel(hit->Pos()->Y());
    dx1 = px-x1;
    dy1 = py-y1;

    dist  = (int) sqrt(dx1*dx1+dy1*dy1);
    if (dist < min_dist) {
      min_dist = dist;
      closest  = hit;
    }
  }

  SetClosestObject(closest,min_dist);

  return min_dist;
}

//-----------------------------------------------------------------------------
Int_t TComboHitVisNode::DistancetoPrimitiveTZ(Int_t px, Int_t py) {

  // static TVector3 global;
  // global.SetXYZ(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py),0);

  TObject* closest(nullptr);

  int  x1, y1, dx1, dy1, min_dist(9999), dist;

  int nhits = fListOfHits->GetEntries();
  for (int i=0; i<nhits; i++) {
    stntuple::TEvdComboHit* hit = (stntuple::TEvdComboHit*) fListOfHits->At(i);
    x1  = gPad->XtoAbsPixel(hit->Z());
    y1  = gPad->YtoAbsPixel(hit->T());
    dx1 = px-x1;
    dy1 = py-y1;

    dist  = (int) sqrt(dx1*dx1+dy1*dy1);
    if (dist < min_dist) {
      min_dist = dist;
      closest  = hit;
    }
  }

  SetClosestObject(closest,min_dist);

  return min_dist;
}

// //-----------------------------------------------------------------------------
// void TComboHitVisNode::Print(Option_t* Option) const {
//   //TComboHitVisNode* node = (TComboHitVisNode*) this;

//   printf("TComboHitVisNode name: %s\n",GetName());
//   printf("nhits: %5i\n",fListOfHits->GetEntriesFast());
//   for (int i=0; i<18; i++) {
//     printf(" %3i",fHitData->fStation[i].fHits.GetEntries());
//   }
//   printf("\n");
// }

