//-----------------------------------------------------------------------------
//  algorithms working on different STNTUPLE objects
//  Nov 26 2000 P.Murat
//-----------------------------------------------------------------------------

#include <float.h>
#include <iostream>
#include <math.h>
#include "TLorentzVector.h"

#include "Stntuple/alg/TStntuple.hh"

#include "Stntuple/loop/TStnModule.hh"

#include "Stntuple/obj/TStnTrack.hh"

#include <Stntuple/obj/TStnHeaderBlock.hh>
#include <Stntuple/obj/TStnTrackBlock.hh>

#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TStnErrorLogger.hh"

#include "Stntuple/alg/TStntuple.hh"

#include "Stntuple/obj/TStnDBManager.hh"


ClassImp(TStntuple)


TStntuple*        TStntuple::fgInstance       = 0;

Int_t             TStntuple::fgRunNumber = 0;
//_____________________________________________________________________________
TStntuple::TStntuple() {
}

//_____________________________________________________________________________
TStntuple::~TStntuple() {
}

//_____________________________________________________________________________
TStntuple*  TStntuple::Instance() { 
  static Cleaner cleaner;
  return (fgInstance) ? fgInstance : (fgInstance = new TStntuple());
}

//------------------------------------------------------------------------------
TStntuple::Cleaner::Cleaner() {
}

//------------------------------------------------------------------------------
TStntuple::Cleaner::~Cleaner() {
  if (TStntuple::fgInstance) {
    delete TStntuple::fgInstance;
    TStntuple::fgInstance = 0;
  }
}

//_____________________________________________________________________________
Int_t TStntuple::Init(Int_t RunNumber) {

  int rc = 0;
//----------------------------------------------------------------------------
// Did we do this alread?
//----------------------------------------------------------------------------
  if (fgRunNumber == RunNumber) return rc;

//----------------------------------------------------------------------------
// Pointer to the data base manager
//----------------------------------------------------------------------------
//  TStnDBManager* dbm = TStnDBManager::Instance();

//----------------------------------------------------------------------------
// Assign the run number to make sure we do not repeat this unnecessarily
//----------------------------------------------------------------------------
  fgRunNumber = RunNumber;

  return rc;
}

//-----------------------------------------------------------------------------
// parameterization of the DIO spectrum on Al
// from Czarnecki et al, Phys.Rev.D84:013006,2011 (http://www.arxiv.org/abs/1106.4756)
// the weights are normalized to the unit integral, so the histogram used has to 
// be divided by the number of events and, then, scaled to the expected number 
// of protons on target
//-----------------------------------------------------------------------------
double TStntuple::DioWeightAl(double E) {

  double a5(8.6434), a6(1.16874), a7(-1.87828e-2), a8(9.16327e-3);
  double emu(105.194), mAl(25133.);
  
  double de, de5, w;

  de  = emu-E-E*E/(2*mAl);

  de5 = de*de*de*de*de;

  w   = 1.e-17*de5*(a5 + de*(a6+de*(a7+a8*de)));

  if (de < 0) w = 0;

  return w;
}

//-----------------------------------------------------------------------------
// parameterization of the LO DIO spectrum on Al 
// from mu2e-6309 (by R.Szafron)
//-----------------------------------------------------------------------------
double TStntuple::DioWeightAl_LO(double E) {

  double a5(8.99879), a6(1.17169), a7(-1.06599e-2), a8(8.14251e-3);
  double emu(105.194), mAl(25133.);
  
  double de, de5, w;

  de  = emu-E-E*E/(2*mAl);

  de5 = de*de*de*de*de;

  w   = 1.e-17*de5*(a5 + de*(a6+de*(a7+a8*de)));

  if (de < 0) w = 0;

  return w;
}

//-----------------------------------------------------------------------------
// parameterization of the DIO spectrum on Al with LL radiative corrections 
// from mu2e-6309 (by R.Szafron)
// 'emu' - energy of the muon bound in Al nucleus, not the muon mass
//-----------------------------------------------------------------------------
double TStntuple::DioWeightAl_LL(double E) {

  double a5(8.9), a6(1.17169), a7(-1.06599e-2), a8(8.14251e-3);
  double emu(105.194), mAl(25133.), mmu(105.658), me(0.511);
  double alpha(1./137.036) ;  // alpha EM
  
  double de, de5, w;

  de  = emu-E-E*E/(2*mAl);

  de5 = de*de*de*de*de;

  double f = 2.*log((mmu/me)*(1-de/mmu))-2+2.*log(2);

  w   = 1.e-17*pow(de/mmu,(alpha/M_PI)*f)*de5*(a5 + de*(a6+de*(a7+a8*de)));

  if (de < 0) w = 0;

  return w;
}

//-----------------------------------------------------------------------------
// 
double TStntuple::DioWeightTi(double E) {
  printf(">>> ERROR: TStntuple::DioWeightTi not implemented yet\n");
  return -1;
}

