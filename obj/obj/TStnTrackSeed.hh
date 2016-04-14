//
// Read the track 
//
// $Id:   $
// $Author:$
// $Date: $
//
// Contact person Pavel Murat
//
#ifndef Stntuple_obj_TStnTrackSeed_hh
#define Stntuple_obj_TStnTrackSeed_hh

// storable objects (data products)
// 

// C++ includes.
#include <iostream>

#include "TString.h"
#include "TFolder.h"
#include "TFile.h"

//namespace murat {

namespace mu2e {
  class TrackSeed;
  class StrawHit;
  class CaloCluster;
}

class TStnTrackSeed : public TObject {

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
  int                       fNLoops;
  int                       fNHits;
//-----------------------------------------------------------------------------
// floats
//-----------------------------------------------------------------------------
  float                     fT0;    
  float                     fT0Err; 
  float                     fD0;    
  float                     fPhi0;  
  float                     fOmega; 
  float                     fZ0;    
  float                     fTanDip;

  float                     fCaloTime;   
  float			    fCaloEnergy; 
  float			    fCaloX;      
  float			    fCaloY;      
  float			    fCaloZ;      

//-----------------------------------------------------------------------------
// transients
//-----------------------------------------------------------------------------
  const mu2e::TrackSeed*    fTrackSeed;  //!
  const mu2e::CaloCluster*  fCluster;    //!
  //  const std::vector<art::Ptr<mu2e::StrawHit> > *fSelectedTrkHits;
//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
  TStnTrackSeed(int i = -1);
  ~TStnTrackSeed();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int     NLoops     () { return  fNLoops; }
  int     NHits      () { return  fNHits;  }

  float   T0         () { return  fT0;    }
  float   T0Err      () { return  fT0Err; }
  float   D0         () { return  fD0;    }
  float   Phi0       () { return  fPhi0;  }
  float   Omega      () { return  fOmega; }
  float   Z0         () { return  fZ0;    }
  float   TanDip     () { return  fTanDip; }

  float   CaloTime   () { return fCaloTime; }
  float   CaloEnergy () { return fCaloEnergy;}
  float   CaloX      () { return fCaloX;     }
  float   CaloY      () { return fCaloY;     }
  float   CaloZ      () { return fCaloZ;     }

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt = "") ;
  virtual void Print(Option_t* Opt = "") const ;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
//  void ReadV1(TBuffer& R__b);

  ClassDef(TStnTrackSeed,1);
};

#endif
