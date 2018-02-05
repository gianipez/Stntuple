#ifndef STNTUPLE_TStnTimePeakBlock
#define STNTUPLE_TStnTimePeakBlock
//-----------------------------------------------------------------------------
//  definition of the cluster block for MU2E analysis
//  Author:    G. Pezzullo
//  Date:      April 11 2016
//-----------------------------------------------------------------------------
#include "TClonesArray.h"

#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnTimePeak.hh"
#include "TBuffer.h"

class TStnTimePeakBlock: public TStnDataBlock {

  friend Int_t StntupleInitMu2eTimeClusterBlock     (TStnDataBlock*, AbsEvent* , int);
  friend Int_t StntupleInitMu2eTimeClusterBlockLinks(TStnDataBlock*, AbsEvent* , int);
public:
//----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
  Int_t          fNTimePeaks;
  TClonesArray*  fListOfTimePeaks;
//----------------------------------------------------------------------------
//  functions
//----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TStnTimePeakBlock();
  virtual ~TStnTimePeakBlock();

  TStnTimePeak* NewTimePeak() {
    TStnTimePeak* trackSeed = new ((*fListOfTimePeaks)[fNTimePeaks]) TStnTimePeak();
    fNTimePeaks++;
    return trackSeed;
  }
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Int_t              NTimePeaks     () { return fNTimePeaks;   }
  TClonesArray*      ListOfTimePeaks() { return fListOfTimePeaks; }

  TStnTimePeak*   TimePeak(int I) {
    return (TStnTimePeak*) fListOfTimePeaks->UncheckedAt(I); 
  }

//-----------------------------------------------------------------------------
// overloaded functions of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* opt="");
  void Print(Option_t* opt="") const;

  ClassDef(TStnTimePeakBlock,1)
};

#endif
