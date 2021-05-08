///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name validation_005("val_stn");

void val_stn(int PdgCode = 11, int GeneratorCode = 2) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  stntuple::m_val = (stntuple::TValidationModule*) g.x->AddModule("stntuple::TValidationModule",0);  
  stntuple::m_val->SetPdgCode      (PdgCode);
  stntuple::m_val->SetGeneratorCode(GeneratorCode);
}

