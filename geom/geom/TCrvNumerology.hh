///////////////////////////////////////////////////////////////////////////////
// CRV numerology
///////////////////////////////////////////////////////////////////////////////
#ifndef Stntuple_inc_TCrvNumerology_hh
#define Stntuple_inc_TCrvNumerology_hh

#include "TObject.h"
#include "TString.h"

class TCrvNumerology : public TObject {
public:

  struct SectorData_t {
    int     fNumber;
    TString fName;
    int     fNModules;
    int     fNLayers;
    int     fNBarsPerLayer;
    int     fFirstIndex;
    float   fBarLength;     //bar length in meters
  };

  enum { kNSectors = 22 } ;

  SectorData_t fSector[kNSectors];

  static  TCrvNumerology* fgInstance;

  class  Cleaner {
  public:
    Cleaner ();
    ~Cleaner();
  };
  friend class Cleaner;
//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
  TCrvNumerology();
  ~TCrvNumerology();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int         NSectors          () { return kNSectors;                 }
  const char* Name         (int I) { return fSector[I].fName.Data();   }
  int         NModules     (int I) { return fSector[I].fNModules;      }
  int         NBarsPerLayer(int I) { return fSector[I].fNBarsPerLayer; }
  int         FirstIndex   (int I) { return fSector[I].fFirstIndex;    }
  float       BarLength    (int I) { return fSector[I].fBarLength;     }

//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  int        GetBarInfo(int BarIndex, int& Sector, int& Module, int& Layer, int& Bar);

  static TCrvNumerology*  Instance();
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void    Clear(Option_t* Opt = "") ;
  void    Print(Option_t* Opt = "") const ;

  ClassDef(TCrvNumerology,0)

};

//}  // end namespace

#endif
