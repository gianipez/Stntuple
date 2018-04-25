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
void TStnHelix::ReadV1(TBuffer &R__b) {

  struct TStnHelixDataV1_t {
    int                       fNHits;
    int                       fAlgorithmID;     // bit-packed : (alg_mask << 16 ) | best
    int                       fTimeClusterIndex;
    int                       fTrackSeedIndex;
    //-----------------------------------------------------------------------------
    // floats
    //-----------------------------------------------------------------------------
    float                     fT0;    
    float                     fT0Err; 

    float                     fRCent;  // radius of circle center
    float                     fFCent;  // azimuth of circle center
    float                     fRadius; // transverse radius of the helix (mm).  Always positive
    float                     fLambda; // dz/dphi (mm/radian)
    float                     fFZ0;    // azimuth (phi) at the center z position (radians)
    float                     fD0;     // impact paramter 
    float                     fChi2XYNDof;
    float                     fChi2PhiZNDof;

    float                     fClusterTime;   
    float		      fClusterEnergy; 
    float                     fClusterX;      
    float		      fClusterY;      
    float		      fClusterZ;      

    const mu2e::HelixSeed*    fHelix;  //!
 };

  TStnHelixDataV1_t data;

  int            nwi, nwf;
  
  nwi      = ((int*  ) &data.fT0            ) - &data.fNHits;
  nwf      = ((float*) &data.fHelix         ) - &data.fT0;
    
  R__b.ReadFastArray(&data.fNHits ,nwi);
  R__b.ReadFastArray(&data.fT0    ,nwf);

  fNHits             = data.fNHits;	     
  fAlgorithmID       = data.fAlgorithmID;    
  fTimeClusterIndex  = data.fTimeClusterIndex;
  fTrackSeedIndex    = data.fTrackSeedIndex; 

  fT0                = data.fT0;    	  
  fT0Err             = data.fT0Err; 	  
                
  fRCent             = data.fRCent;
  fFCent             = data.fFCent;
  fRadius            = data.fRadius;
  fLambda            = data.fLambda;
  fFZ0               = data.fFZ0;   
  fD0                = data.fD0;    
  fChi2XYNDof        = data.fChi2XYNDof;  
  fChi2PhiZNDof      = data.fChi2PhiZNDof;
                
  fClusterTime       = data.fClusterTime; 
  fClusterEnergy     = data.fClusterEnergy; 
  fClusterX          = data.fClusterX;      
  fClusterY          = data.fClusterY;    
  fClusterZ          = data.fClusterZ;    
}


void TStnHelix::Streamer(TBuffer& R__b) {
  int   nwi, nwf;
  
  nwi      = ((int*  ) &fT0            ) - &fNHits;
  nwf      = ((float*) &fHelix         ) - &fT0;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); 

    if (R__v == 1) ReadV1 (R__b);
    else {
					// current version: V2
      R__b.ReadFastArray(&fNHits, nwi);
      R__b.ReadFastArray(&fT0    , nwf);
    }
  }
  else {
    R__b.WriteVersion(TStnHelix::IsA());

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0   , nwf);
    
    
  }
}

TStnHelix::TStnHelix(int i) {

  fNHits  = 0;

  fNComboHits = -1;

  fAlgorithmID = -1;

  fTimeClusterIndex = -1;
  fTrackSeedIndex   = -1;

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




