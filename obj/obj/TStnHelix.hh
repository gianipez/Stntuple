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

//namespace murat {

namespace mu2e {
  class HelixSeed;
  class StrawHit;
  class CaloCluster;
}

class TStnHelix : public TObject {

  // enum {
  //   kNFreeIntsV1   = 10,		// V1
  //   kNFreeFloatsV1 = 10,		// V1

  //   kNFreeInts     = 10,		// V2
  //   kNFreeFloats   =  3			// V2
  // };

public:
//-----------------------------------------------------------------------------
// integers
//-----------------------------------------------------------------------------
  int                       fNHits;
  int                       fAlgorithmID;     // bit-packed : (alg_mask << 16 ) | best
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
  int     NHits      () { return  fNHits;  }
  int     AlgorithmID() { return fAlgorithmID; }
  int     AlgMask    () { return (fAlgorithmID >> 16) & 0xffff; }

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

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt = "") ;
  virtual void Print(Option_t* Opt = "") const ;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
//  void ReadV1(TBuffer& R__b);

  ClassDef(TStnHelix,1);
};

#endif
