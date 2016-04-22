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
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNLoops;
  nwf      = ((float*) &fTrackSeed     ) - &fT0;

  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
					// current version: V1
    R__b.ReadFastArray(&fNLoops, nwi);
    R__b.ReadFastArray(&fT0    , nwf);
   }
  else {
    R__b.WriteVersion(TStnTrackSeed::IsA());

    R__b.WriteFastArray(&fNLoops, nwi);
    R__b.WriteFastArray(&fT0    , nwf);
    
    
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

  fClusterTime   = 0;  
  fClusterEnergy = 0;
  fClusterX      = 0;     
  fClusterY      = 0;     
  fClusterZ      = 0;     

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




