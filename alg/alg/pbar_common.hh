// -*- Mode: C++ -*- 
//------------------------------------------------------------------------------
// Oct 28 2019 P.Murat: 
//------------------------------------------------------------------------------
#ifndef __PBAR_COMMON_HH__
#define __PBAR_COMMON_HH__

struct PbarCommon_t {
  int      IFIRST_TIME;        // event number
  float    P2MAX;              // p2max
  float    P2C;                // p2c
};

extern PbarCommon_t*    gPbarCommon;

void InitPbarCommon();
//------------------------------------------------------------------------------
//  end of pbar_common.hh
//------------------------------------------------------------------------------
#endif
