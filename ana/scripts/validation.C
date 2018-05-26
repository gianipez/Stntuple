///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name validation_005("val_stn");
def_name validation_006("evd");

void val_stn(int PdgCode = 11, int GeneratorCode = 2) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  m_val = (TValidationModule*) g.x->AddModule("TValidationModule",0);  
  m_val->SetPdgCode      (PdgCode);
  m_val->SetGeneratorCode(GeneratorCode);
}


//-----------------------------------------------------------------------------
void  evd(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure analysis module to write TMVA training trees
//-----------------------------------------------------------------------------
  m_evd = (TEventDisplayModule*) g.x->AddModule("TEventDisplayModule",0);
  m_evd->SetDisplayBeamMuMinus(1);
  m_evd->SetDisplayBeamMuPlus (1);

  if (DebugBit >= 0) m_evd->SetDebugBit(DebugBit,1);
}

