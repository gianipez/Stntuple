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
  class KalSeed;
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
  int                       fNHits;
  int                       fHelixIndex;
  int                       fTrackIndex;
//-----------------------------------------------------------------------------
// floats
//-----------------------------------------------------------------------------
  float                     fT0;    
  float                     fT0Err; 
  float                     fD0;    
  float                     fZ0;    
  float                     fP;  
  float                     fPt; 
  float                     fTanDip;
  float                     fFlt0;// flight distance where the track crosses the tracker midplane (z=0)
  float                     fChi2;
  float                     fFitCons;

  float                     fClusterTime;   
  float			    fClusterEnergy; 
  float			    fClusterX;      
  float			    fClusterY;      
  float			    fClusterZ;      

//-----------------------------------------------------------------------------
// transients
//-----------------------------------------------------------------------------
  const mu2e::KalSeed*      fTrackSeed;  //!
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
  int     NHits      () { return  fNHits;      }
  int     HelixIndex () { return  fHelixIndex; }
  int     TrackIndex () { return  fTrackIndex; }

  float   T0         () { return  fT0;    }
  float   T0Err      () { return  fT0Err; }
  float   D0         () { return  fD0;    }
  float   P          () { return  fP;  }
  float   Pt         () { return  fPt; }
  float   Z0         () { return  fZ0;    }
  float   TanDip     () { return  fTanDip; }
  float   Flt0       () { return  fFlt0; }

  float   Chi2       () { return  fChi2;}
  float   FitCons    () { return  fFitCons;}

  float   ClusterTime   () { return fClusterTime; }
  float   ClusterEnergy () { return fClusterEnergy;}
  float   ClusterX      () { return fClusterX;     }
  float   ClusterY      () { return fClusterY;     }
  float   ClusterZ      () { return fClusterZ;     }

//-----------------------------------------------------------------------------
// setters
//-----------------------------------------------------------------------------
  void    SetHelixIndex(int I) { fHelixIndex = I; }
  void    SetTrackIndex(int I) { fTrackIndex = I; }

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
