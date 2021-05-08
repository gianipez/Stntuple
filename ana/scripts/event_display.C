///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"

def_name stn_event_display_001("stn_evd");

//-----------------------------------------------------------------------------
void  stn_evd(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure 
//-----------------------------------------------------------------------------
  stntuple::m_evd = (stntuple::TEventDisplayModule*) g.x->AddModule("stntuple::TEventDisplayModule",0);
  stntuple::m_evd->SetDisplayBeamMuMinus(1);
  stntuple::m_evd->SetDisplayBeamMuPlus (1);

  if (DebugBit >= 0) stntuple::m_evd->SetDebugBit(DebugBit,1);
}

