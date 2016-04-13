//
#include <iostream>

#include "TMatrix.h"
#include "obj/TStnTrackSeed.hh"

// #include "RecoDataProducts/inc/TrackSeed.hh"

namespace {
  //  const double BF = 1.4116 ;  // CDF case
  const double BF = 1.0 ;         // MU2E case
}

//namespace murat {

ClassImp(TStnTrackSeed)

void TStnTrackSeed::Streamer(TBuffer& R__b) {
 
  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
					// current version: V1
   }
  else {
    R__b.WriteVersion(TStnTrackSeed::IsA());

    }
}

TStnTrackSeed::TStnTrackSeed(int i) {

  fNLoops = 0;
  fNHits  = 0;
  fT0     = 0;    
  fT0Err  = 0; 
  fD0     = 0;    
  fPhi0   = 0;  
  fOmega  = 0; 
  fZ0     = 0;    
  fTanDip = 0;

  fCaloTime   = 0;  
  fCaloEnergy = 0;
  fCaloX      = 0;     
  fCaloY      = 0;     
  fCaloZ      = 0;     

}

//_____________________________________________________________________________
TStnTrackSeed::~TStnTrackSeed() {
}


//-----------------------------------------------------------------------------
void TStnTrackSeed::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnTrackSeed::Print(Option_t* opt) const {
  Error("Print", "Not implemented yet");
}

// } // end namespace




