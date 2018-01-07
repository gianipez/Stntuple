//
// Read the track 
//
// $Id:   $
// $Author:$
// $Date: $
//
// Contact person Pavel Murat
//
#ifndef Stntuple_obj_TStnTimePeak_hh
#define Stntuple_obj_TStnTimePeak_hh

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
  class TimeCluster;
}

class TStnTimePeak : public TObject {

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
  int                       fHelixSeedIndex;
//-----------------------------------------------------------------------------
// floats
//-----------------------------------------------------------------------------
  float                     fT0;    
  float                     fT0Err; 

  float                     fPosX;
  float                     fPosY;
  float                     fPosZ;
 
  float                     fClusterTime;   
  float			    fClusterEnergy; 
  float			    fClusterX;      
  float			    fClusterY;      
  float			    fClusterZ;      

//-----------------------------------------------------------------------------
// transients
//-----------------------------------------------------------------------------
  const mu2e::TimeCluster*    fTimeCluster;  //!
//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
  TStnTimePeak(int i = -1);
  ~TStnTimePeak();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int     NHits         () { return  fNHits;  }
  int     HelixSeedIndex() { return  fHelixSeedIndex; }

  float   T0            () { return  fT0;     }
  float   T0Err         () { return  fT0Err;  }

  float   posX          () { return  fPosX;}
  float   posY          () { return  fPosY;}
  float   posZ          () { return  fPosZ;}

  float   ClusterTime   () { return fClusterTime;  }
  float   ClusterEnergy () { return fClusterEnergy;}
  float   ClusterX      () { return fClusterX;     }
  float   ClusterY      () { return fClusterY;     }
  float   ClusterZ      () { return fClusterZ;     }

//----------------------------------------------------------------------------
// setters
//----------------------------------------------------------------------------
  void    SetHelixSeedIndex(int I) { fHelixSeedIndex = I; }

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt = "") ;
  virtual void Print(Option_t* Opt = "") const ;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
//  void ReadV1(TBuffer& R__b);

  ClassDef(TStnTimePeak,1)
};

#endif
