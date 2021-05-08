//
// - Serves as the link between ROOT "events" (e.g. mouse-clicks) and the ART
//   event display service by providing a receiver slot for signals generated
//   by the ROOT events.  A ROOT dictionary needs to be generated for this.
//

#include "TROOT.h"

#include "Stntuple/ana/TEventDisplayUtils.hh"
#include <string>

ClassImp(stntuple::TEventDisplayUtils)

namespace stntuple {

//-----------------------------------------------------------------------------
// these things seem childish and useless.... leave them in for a while
//------------------------------------------------------------------------------
enum nav_states_ {
  kNEXT_EVENT  ,
  kPREV_EVENT  ,
  kRELOAD_EVENT,
  kGOTO_EVENT
};

static int gsNavState    = 0;
static int gsTargetRun   = 0;
static int gsTargetEvent = 0;

int NavState_Which() { return gsNavState; }

int NavState_TargetRun() {
  return gsTargetRun;
}

int NavState_TargetEvent() {
  return gsTargetEvent;
}

void NavState_Set(int which) {
  gsNavState = which;
  gROOT->GetApplication()->Terminate();
}

void NavState_SetTarget(int run, int event) {
  gsTargetRun   = run;
  gsTargetEvent = event;
}

//-----------------------------------------------------------------------------
TEventDisplayUtils::TEventDisplayUtils(): TObject() {
  fTbRun = 0;
  fTbEvt = 0;
}

//-----------------------------------------------------------------------------
TEventDisplayUtils::~TEventDisplayUtils() {
}

void TEventDisplayUtils::PrevEvent(){
  NavState_Set(kPREV_EVENT);
}

void TEventDisplayUtils::NextEvent(){
  NavState_Set(kNEXT_EVENT);
}

void TEventDisplayUtils::GotoEvent(){
  int run   = std::stoi(fTbRun->GetString());
  int event = std::stoi(fTbEvt->GetString());
  
  NavState_SetTarget(run, event);
  NavState_Set(kGOTO_EVENT);
}
}
