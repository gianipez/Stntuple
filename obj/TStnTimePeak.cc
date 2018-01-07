//
#include <iostream>

#include "TMatrix.h"
#include "obj/TStnTimePeak.hh"

// #include "RecoDataProducts/inc/TimePeak.hh"

namespace {
  //  const double BF = 1.4116 ;  // CDF case
  const double BF = 1.0 ;         // MU2E case
}

//namespace murat {

ClassImp(TStnTimePeak)

void TStnTimePeak::Streamer(TBuffer& R__b) {
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNHits;
  nwf      = ((float*) &fTimeCluster         ) - &fT0;

  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
					// current version: V1
    R__b.ReadFastArray(&fNHits, nwi);
    R__b.ReadFastArray(&fT0    , nwf);
   }
  else {
    R__b.WriteVersion(TStnTimePeak::IsA());

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0    , nwf);
    
    
  }
}

TStnTimePeak::TStnTimePeak(int i) {

  fNHits  = 0;

  fT0     = 0;    
  fT0Err  = 0; 

  fPosX   = 0;
  fPosY   = 0;
  fPosZ   = 0;

  fClusterTime   = 0;  
  fClusterEnergy = 0;
  fClusterX      = 0;     
  fClusterY      = 0;     
  fClusterZ      = 0;     

}

//_____________________________________________________________________________
TStnTimePeak::~TStnTimePeak() {
}


//-----------------------------------------------------------------------------
void TStnTimePeak::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnTimePeak::Print(Option_t* opt) const {
  Error("Print", "Not implemented yet");
}

// } // end namespace




