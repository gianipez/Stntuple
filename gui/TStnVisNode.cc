///////////////////////////////////////////////////////////////////////////////
// real base class for Stntuple VisNodes 
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/gui/TStnVisNode.hh"
#include "Stntuple/gui/TStnVisManager.hh"

ClassImp(TStnVisNode)


//-----------------------------------------------------------------------------
TStnVisNode::TStnVisNode(const char* name): fName(name) {
  fClosestObject = NULL;
  fDebugLevel    = 0;
}


//-----------------------------------------------------------------------------
TStnVisNode::~TStnVisNode() {
}

//-----------------------------------------------------------------------------
void TStnVisNode::Paint   (Option_t* Option) {
  
  int   iv;
				// parse option list

  const char* view = TStnVisManager::Instance()->GetCurrentView();

  if      (strstr(view,"xy" ) != 0) PaintXY(Option);
  else if (strstr(view,"xz" ) != 0) PaintRZ(Option);
  else if (strstr(view,"tz" ) != 0) PaintTZ(Option);
  else if (strstr(view,"cal") != 0) {
    sscanf(view,"cal,%i",&iv);
    if (iv == fDiskID) {
      PaintCal(Option);
    }
  }
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
int  TStnVisNode::DistancetoPrimitive(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
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

  return 10000000;
}

//-----------------------------------------------------------------------------
int  TStnVisNode::DistancetoPrimitiveCal(Int_t px, Int_t py) {
  // by default, return a large number
  // decide how to deal with 3D views later

  return 10000000;
}

//-----------------------------------------------------------------------------
int TStnVisNode::InitEvent() {
  return 0;
}
