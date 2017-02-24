//
#include <iostream>

#include "TMatrix.h"
#include "obj/TStnHelix.hh"

// #include "RecoDataProducts/inc/Helix.hh"

namespace {
  //  const double BF = 1.4116 ;  // CDF case
  const double BF = 1.0 ;         // MU2E case
}

//namespace murat {

ClassImp(TStnHelix)

void TStnHelix::Streamer(TBuffer& R__b) {
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNHits;
  nwf      = ((float*) &fHelix         ) - &fT0;

  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
					// current version: V1
    R__b.ReadFastArray(&fNHits, nwi);
    R__b.ReadFastArray(&fT0    , nwf);
   }
  else {
    R__b.WriteVersion(TStnHelix::IsA());

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0    , nwf);
    
    
  }
}

TStnHelix::TStnHelix(int i) {

  fNHits  = 0;

  fAlgorithmID = -1;

  fT0     = 0;    
  fT0Err  = 0; 
  fRCent  = 0;    
  fFCent  = 0;  
  fRadius = 0; 
  fLambda = 0;    
  fFZ0    = 0;
  fD0     = 0;

  fChi2XYNDof   = 0;
  fChi2PhiZNDof = 0;

  fClusterTime   = 0;  
  fClusterEnergy = 0;
  fClusterX      = 0;     
  fClusterY      = 0;     
  fClusterZ      = 0;     

}

//_____________________________________________________________________________
TStnHelix::~TStnHelix() {
}


//-----------------------------------------------------------------------------
void TStnHelix::Clear(Option_t* opt) {
  Error("Print", "Not implemented yet");
}

//-----------------------------------------------------------------------------
void TStnHelix::Print(Option_t* opt) const {
  Error("Print", "Not implemented yet");
}

// } // end namespace




