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

  fMom1.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin1.SetXYZT(0.,0.,0.,0.);

  fMom2.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin2.SetXYZT(0.,0.,0.,0.);


  fNHits             = data.fNHits;	     
  fAlgorithmID       = data.fAlgorithmID;    
  fTimeClusterIndex  = data.fTimeClusterIndex;
  fTrackSeedIndex    = data.fTrackSeedIndex; 
  fSimpPDG1          = -1;         // added in v3
  fSimpPDGM1         = -1;         // added in v3
  fSimpId1Hits       = -1;         // added in v3
  fSimpPDG2          = -1;         // added in v3
  fSimpPDGM2         = -1;         // added in v3
  fSimpId2Hits       = -1;         // added in v3


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

  // fSimp1P            = -1.;         // added in v3
  // fSimp1Pt           = -1.;         // added in v3
  // fSimp2P            = -1.;         // added in v3
  // fSimp2Pt           = -1.;         // added in v3
}


void TStnHelix::ReadV2(TBuffer &R__b) {

  struct TStnHelixDataV2_t {
    int                       fNHits;
    int                       fAlgorithmID;     // bit-packed : (alg_mask << 16 ) | best
    int                       fTimeClusterIndex;
    int                       fTrackSeedIndex;
    int                       fInt[kNFreeIntsV2];
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
    float                     fFloat[kNFreeFloatsV2]; // provision for future I/O expansion

    const mu2e::HelixSeed*    fHelix;  //!
 };

  TStnHelixDataV2_t data;

  int            nwi, nwf;
  
  nwi      = ((int*  ) &data.fT0            ) - &data.fNHits;
  nwf      = ((float*) &data.fHelix         ) - &data.fT0;
    
  R__b.ReadFastArray(&data.fNHits ,nwi);
  R__b.ReadFastArray(&data.fT0    ,nwf);

  fMom1.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin1.SetXYZT(0.,0.,0.,0.);

  fMom2.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin2.SetXYZT(0.,0.,0.,0.);

  fNHits             = data.fNHits;	     
  fAlgorithmID       = data.fAlgorithmID;    
  fTimeClusterIndex  = data.fTimeClusterIndex;
  fTrackSeedIndex    = data.fTrackSeedIndex; 
  fSimpPDG1          = -1;         // added in v3
  fSimpPDGM1         = -1;         // added in v3
  fSimpId1Hits       = -1;         // added in v3
  fSimpPDG2          = -1;         // added in v3
  fSimpPDGM2         = -1;         // added in v3
  fSimpId2Hits       = -1;         // added in v3

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
    else if (R__v == 2) ReadV2 (R__b);
    else {
      fMom1.Streamer(R__b);
      fOrigin1.Streamer(R__b);
      fMom2.Streamer(R__b);
      fOrigin2.Streamer(R__b);
					// current version: V3
      R__b.ReadFastArray(&fNHits, nwi);
      R__b.ReadFastArray(&fT0   , nwf);
    }
  }
  else {
    R__b.WriteVersion(TStnHelix::IsA());
    fMom1.Streamer(R__b);
    fOrigin1.Streamer(R__b);
    fMom2.Streamer(R__b);
    fOrigin2.Streamer(R__b);

    R__b.WriteFastArray(&fNHits, nwi);
    R__b.WriteFastArray(&fT0   , nwf);
    
    
  }
}

TStnHelix::TStnHelix(int i) {
  fMom1.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin1.SetXYZT(0.,0.,0.,0.);
  
  fMom2.SetPxPyPzE(0.,0.,0.,0.);
  fOrigin2.SetXYZT(0.,0.,0.,0.);
  
  fNHits  = 0;

  fNComboHits = -1;

  fAlgorithmID = -1;

  fTimeClusterIndex = -1;
  fTrackSeedIndex   = -1;

  fSimpPDG1     = 0;    
  fSimpPDGM1    = 0;    
  fSimpId1Hits  = 0;
  fSimpPDG2     = 0; 
  fSimpPDGM2    = 0; 
  fSimpId2Hits  = 0;

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

  // fSimp1P      = 0; 
  // fSimp1Pt     = 0;
  // fSimp2P      = 0; 
  // fSimp2Pt     = 0;


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




