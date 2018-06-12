//
// Read the track 
//
// $Id:   $
// $Author:$
// $Date: $
//
// Contact person Pavel Murat
//
#ifndef Stntuple_obj_TStnHelix_hh
#define Stntuple_obj_TStnHelix_hh

// storable objects (data products)
// 

// C++ includes.
#include <iostream>

#include "TString.h"
#include "TFolder.h"
#include "TFile.h"
#include "TBuffer.h"
#include "TLorentzVector.h"

//namespace murat {

namespace mu2e {
  class HelixSeed;
  class StrawHit;
  class CaloCluster;
}

class TStnHelix : public TObject {

  enum {
    kNFreeIntsV2   = 10,		// V2
    kNFreeFloatsV2 = 10,  		// V2

    kNFreeInts     = 4,		        // v3: added the indices of the two SimParticles contributing 
                                        //     the most and their fraction of hits within the helix, 
    kNFreeFloats   = 10			//     also added the p and pT of the SimParticles.
  };

public:
  TLorentzVector            fMom1;  
  TLorentzVector            fOrigin1;
  TLorentzVector            fMom2;  
  TLorentzVector            fOrigin2;

  
//-----------------------------------------------------------------------------
// integers
//-----------------------------------------------------------------------------
  int                       fNHits;	     
  int                       fAlgorithmID;     // bit-packed : (alg_mask << 16 ) | best
  int                       fTimeClusterIndex;
  int                       fTrackSeedIndex; 
  int                       fNComboHits;     
  int                       fSimpPDG1;          // added in v3
  int                       fSimpPDGM1;         // added in v3
  int                       fSimpId1Hits;       // added in v3
  int                       fSimpPDG2;          // added in v3
  int                       fSimpPDGM2;         // added in v3
  int                       fSimpId2Hits;       // added in v3
  int                       fInt[kNFreeInts]; // provision for future I/O expansion
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
  float			    fClusterEnergy; 
  float			    fClusterX;      
  float			    fClusterY;      
  float			    fClusterZ;      
  float                     fFloat[kNFreeFloats]; // provision for future I/O expansion

//-----------------------------------------------------------------------------
// transients
//-----------------------------------------------------------------------------
  const mu2e::HelixSeed*    fHelix;  //!
//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
  TStnHelix(int i = -1);
  ~TStnHelix();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int     NHits           () { return fNHits;  }
  int     NComboHits      () { return fNComboHits; }
  int     AlgorithmID     () { return fAlgorithmID; }
  int     AlgMask         () { return (fAlgorithmID >> 16) & 0xffff; }
  int     BestAlg         () { return fAlgorithmID & 0xffff; }
  int     TimeClusterIndex() { return fTimeClusterIndex; }
  int     TrackSeedIndex  () { return fTrackSeedIndex;   }
  int     PDG1            () { return fSimpPDG1; }
  int     PDGMother1      () { return fSimpPDGM1; }
  int     ComboHitsFrom1  () { return fSimpId1Hits; }
  int     PDG2            () { return fSimpPDG2; }
  int     PDGMother2      () { return fSimpPDGM2; }
  int     ComboHitsFrom2  () { return fSimpId2Hits; }

  float   T0         () { return  fT0;     }
  float   T0Err      () { return  fT0Err;  }

  float   RCent      () { return  fRCent;  } 
  float   FCent      () { return  fFCent;  } 
  float   Radius     () { return  fRadius; }
  float   Lambda     () { return  fLambda; }
  float   FZ0        () { return  fFZ0;    }   
  float   CenterX    () { return  fRCent*cos(fFCent);}
  float   CenterY    () { return  fRCent*sin(fFCent);}
  float   D0         () { return  fD0;    }   

  float   Chi2XY     () { return  fChi2XYNDof;}
  float   Chi2ZPhi   () { return  fChi2PhiZNDof;}

  float   ClusterTime   () { return fClusterTime;  }
  float   ClusterEnergy () { return fClusterEnergy;}
  float   ClusterX      () { return fClusterX;     }
  float   ClusterY      () { return fClusterY;     }
  float   ClusterZ      () { return fClusterZ;     }

  TLorentzVector  Mom1     () { return fMom1; }
  TLorentzVector  Origin1  () { return fOrigin1; }
  TLorentzVector  Mom2     () { return fMom2; }
  TLorentzVector  Origin2  () { return fOrigin2; }

//----------------------------------------------------------------------------
// setters
//----------------------------------------------------------------------------
  void    SetTimeClusterIndex(int I) { fTimeClusterIndex = I; }
  void    SetTrackSeedIndex  (int I) { fTrackSeedIndex   = I; }

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt = "") ;
  virtual void Print(Option_t* Opt = "") const ;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void ReadV1(TBuffer& R__b);
  void ReadV2(TBuffer& R__b);

  ClassDef(TStnHelix,3);
};

#endif
