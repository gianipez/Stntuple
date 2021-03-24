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

  struct BarData_t {
    int     fBarIndex;
    int     fSector;
    int     fSectorType;
    float   fBarX;
    float   fBarY;
    float   fBarZ;
    float   fWidthDir;
    bool    localXY(float&X, float&Y){
      if((fSector >= 0) && (fSector <= 6)){
    	//right side sectors: 0-4, left side sectors: 5-6; dx/dz gives particle direction wrt beam
    	X = fBarX;
    	Y = fBarZ;
	if((fSector >= 0) && (fSector <= 4)){
	  X = -X; // flip right side to make slopes match left and top
	}
	return true;
      }
      else if((fSector >= 7) && (fSector <= 10)){
    	//top side sectors: 7-10; dy/dz gives particle direction wrt beam
    	X = fBarY;
    	Y = fBarZ;
	return true;
      }
      else{
    	//nothing is needed if particle goes through U,D sectors
        X = 0;
	Y = 0;
	return false;
      }
    }
  };

  enum { kNSectors = 22, kNBars = 5504 } ;

  SectorData_t fSector[kNSectors];
  BarData_t    fBar[kNBars];

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
  bool        LocalBarXY   (int I, float&X, float&Y) { return fBar[I].localXY(X,Y); }
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
