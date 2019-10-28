//-----------------------------------------------------------------------
//  revision history:
//  -----------------
// *0001 Aug 13 1998 P.Murat: reorganize InitFortranCommonBlocks
//-----------------------------------------------------------------------

#include "Stntuple/alg/pbar_common.hh"

PbarCommon_t*    gPbarCommon;

extern "C" void*  pbar_common_address_();

static int PbarCommonInitialized = 0;

void InitPbarCommon() {

  if (PbarCommonInitialized) return;
  PbarCommonInitialized = 1;

  gPbarCommon = (PbarCommon_t*) pbar_common_address_();
}





