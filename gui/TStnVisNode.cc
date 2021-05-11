///////////////////////////////////////////////////////////////////////////////
// real base class for Stntuple VisNodes 
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/gui/TStnVisNode.hh"
#include "Stntuple/gui/TStnVisManager.hh"

ClassImp(TStnVisNode)


//-----------------------------------------------------------------------------
TStnVisNode::TStnVisNode(const char* Name): TVisNode(Name) {
}


//-----------------------------------------------------------------------------
TStnVisNode::~TStnVisNode() {
}

//-----------------------------------------------------------------------------
void TStnVisNode::Paint   (Option_t* Option) {
				// parse option list

  int view_type = TStnVisManager::Instance()->GetCurrentView()->Type();

  if      (view_type == TStnView::kXY ) PaintXY (Option);
  else if (view_type == TStnView::kRZ ) PaintRZ (Option);
  else if (view_type == TStnView::kTZ ) PaintTZ (Option);
  else if (view_type == TStnView::kCal) PaintCal(Option);
  else if (view_type == TStnView::kCrv) PaintCrv(Option);
  else {
    // what is the default?
    //    Warning("Paint",Form("Unknown option %s",option));
  }
  
  gPad->Modified();
}

//-----------------------------------------------------------------------------
void TStnVisNode::PaintXY (Option_t* Option) {
}

//-----------------------------------------------------------------------------
void TStnVisNode::PaintRZ (Option_t* Option) {
}

//-----------------------------------------------------------------------------
void TStnVisNode::PaintTZ (Option_t* Option) {
}

//-----------------------------------------------------------------------------
void TStnVisNode::PaintCal(Option_t* Option) {
}

//-----------------------------------------------------------------------------
void TStnVisNode::PaintCrv(Option_t* Option) {
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitive(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  int dist(9999);

  int view  = TStnVisManager::Instance()->GetCurrentView()->Type();
  //  int index = TStnVisManager::Instance()->GetCurrentView()->Index();
 
  if      (view == TStnView::kXY ) dist = DistancetoPrimitiveXY (px,py);
  else if (view == TStnView::kRZ ) dist = DistancetoPrimitiveRZ (px,py);
  else if (view == TStnView::kTZ ) dist = DistancetoPrimitiveTZ (px,py);
  else if (view == TStnView::kCal) dist = DistancetoPrimitiveCal(px,py);
  else if (view == TStnView::kCrv) dist = DistancetoPrimitiveCrv(px,py);
  else {
    // what is the default?
    //    Warning("Paint",Form("Unknown option %s",option));
  }

  return dist;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveXY(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveRZ(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveTZ(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveCal(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveCrv(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
}

//-----------------------------------------------------------------------------
int TStnVisNode::InitEvent() {
  return 0;
}
