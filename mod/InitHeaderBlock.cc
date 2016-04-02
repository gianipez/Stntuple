///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TROOT.h"
#include "TFolder.h"
#include "TNamed.h"

#include <Stntuple/obj/TStnEvent.hh>
#include <Stntuple/obj/TStnHeaderBlock.hh>
#include <Stntuple/mod/InitStntupleDataBlocks.hh>
#include <Stntuple/mod/StntupleUtilities.hh>

#include "MCDataProducts/inc/ProtonBunchIntensity.hh"

void stntuple_get_version(char* ver, char* test);

//_____________________________________________________________________________
Int_t StntupleInitMu2eHeaderBlock(TStnDataBlock* block, AbsEvent* AnEvent, int mode) 
{
  // Run II version, section number is defined

  static TFolder*         fol              = NULL;
  //  static TNamed*          processName      = NULL;

  if (! fol) {
//-----------------------------------------------------------------------------
//  initialize local static variables
//-----------------------------------------------------------------------------
    fol     = (TFolder*) gROOT->GetRootFolder()->FindObject("Stntuple");
    //    processName      = (TNamed*) fol->FindObject("ProcessName");
  }

  TStnHeaderBlock* header = (TStnHeaderBlock*) block;

  header->fEventNumber   = AnEvent->event();
  header->fRunNumber     = AnEvent->run();
  header->fSectionNumber = AnEvent->subRun();
  header->fMcFlag        = 1.; // gblEnv->monteFlag();

  header->fNTracks       = -1;
//-----------------------------------------------------------------------------
// instantaneous luminosity
//-----------------------------------------------------------------------------
  header->fInstLum       = -1.;

  static char pbi_module_label[] = "protonBunchIntensity";
  static char pbi_description [] = "";

  art::Handle<mu2e::ProtonBunchIntensity>  pbiHandle;
  const mu2e::ProtonBunchIntensity*        pbi;

  if (pbi_module_label[0] != 0) {
    AnEvent->getByLabel(pbi_module_label,pbi_description,pbiHandle);
    if (pbiHandle.isValid()) {
      pbi = pbiHandle.product();
      header->fInstLum = pbi->intensity();
    }
  }

//-----------------------------------------------------------------------------
//  STNTUPLE version
//-----------------------------------------------------------------------------
  char  ver[200], text[200];
  stntuple_get_version(ver,text);
  header->fStnVersion = ver;

  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2eHeaderBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, section number is defined

  Int_t  ev_number, rn_number;

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (! Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnHeaderBlock* header = (TStnHeaderBlock*) Block;
//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  header->fLinksInitialized = 1;

  return 0;
}


