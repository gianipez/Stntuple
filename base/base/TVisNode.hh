#ifndef Stntuple_base_TVisVode_hh
#define Stntuple_base_TVisVode_hh

#include "TObject.h"
#include "TString.h"

class TVisNode: public TObject {
protected:
  TString    fName;
  int        fDebugLevel;
  TObject*   fClosestObject;
  int        fDist;
public:
					// ****** constructors and destructor
  TVisNode(const char* name = "TVisNode");
  virtual ~TVisNode();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  TObject*            GetClosestObject() { return fClosestObject; }
  
  virtual const char* GetName() const    { return fName.Data(); }

  int                 DebugLevel()       { return fDebugLevel; }

					// called by TEvdManager::DisplayEvent
  virtual int         InitEvent() = 0;

  void                SetDebugLevel(int Level) { fDebugLevel = Level; }

  void                SetClosestObject(TObject* Obj, int Dist) {
    fClosestObject = Obj;
    fDist          = Dist;
  }

  ClassDef(TVisNode,0)
};

#endif
