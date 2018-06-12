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
#include "TLorentzVector.h"

//namespace murat {

namespace mu2e {
  class KalSeed;
  class StrawHit;
  class CaloCluster;
}

class TStnTrackSeed : public TObject {

  enum {
    kNFreeInts     = 10,		// V2
    kNFreeFloats   = 10			// V2
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
  int                       fHelixIndex;
  int                       fTrackIndex;
  int                       fSimpPDG1;          // added in v2
  int                       fSimpPDGM1;         // added in v2
  int                       fSimpId1Hits;       // added in v2
  int                       fSimpPDG2;          // added in v2
  int                       fSimpPDGM2;         // added in v2
  int                       fSimpId2Hits;       // added in v2
  int                       fInt[kNFreeInts]; // provision for future I/O expansion
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
  float                     fFloat[kNFreeFloats]; // provision for future I/O expansion
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
  int     PDG1       () { return  fSimpPDG1; }
  int     PDGMother1 () { return  fSimpPDGM1; }
  int     NHitsFrom1 () { return  fSimpId1Hits; }
  int     PDG2       () { return  fSimpPDG2; }
  int     PDGMother2 () { return  fSimpPDGM2; }
  int     NHitsFrom2 () { return  fSimpId2Hits; }

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

  TLorentzVector  Mom1     () { return fMom1; }
  TLorentzVector  Origin1  () { return fOrigin1; }
  TLorentzVector  Mom2     () { return fMom2; }
  TLorentzVector  Origin2  () { return fOrigin2; }


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
  void ReadV1(TBuffer& R__b);

  ClassDef(TStnTrackSeed,2);
};

#endif
