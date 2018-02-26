//-----------------------------------------------------------------------------
//  Apr 2016 G. Pezzullo: initialization of the MU2E STNTUPLE TrackSeed block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"

#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnEvent.hh"

#include "Stntuple/obj/TStnHelix.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"

#include "Stntuple/obj/TStnTrackSeed.hh"
#include "Stntuple/obj/TStnTrackSeedBlock.hh"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "TTrackerGeom/inc/TTracker.hh"
// #include "TrkReco/inc/TrkStrawHit.hh"

#include "RecoDataProducts/inc/KalSeed.hh"

#include "RecoDataProducts/inc/CaloCluster.hh"

// #include "CalPatRec/inc/THackData.hh"

//-----------------------------------------------------------------------------
// assume that the collection name is set, so we could grab it from the event
//-----------------------------------------------------------------------------
int  StntupleInitMu2eTrackSeedBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode) {

  mu2e::KalSeedCollection*  list_of_trackSeeds;

  static char                 trkSeed_module_label[100], trkSeed_description[100]; 

  TStnTrackSeedBlock*         cb = (TStnTrackSeedBlock*) Block;
  TStnTrackSeed*              trackSeed;


  cb->Clear();

  cb->GetModuleLabel("mu2e::KalSeedCollection", trkSeed_module_label);
  cb->GetDescription("mu2e::KalSeedCollection", trkSeed_description );

  if (trkSeed_module_label[0] == 0) {
    cb->GetModuleLabel("ShortTrackSeedBlockName", trkSeed_module_label);
  }
  art::Handle<mu2e::KalSeedCollection>               trackSeed_handle;
  if (trkSeed_description[0] == 0) Evt->getByLabel(trkSeed_module_label, trackSeed_handle);
  else                          Evt->getByLabel(trkSeed_module_label, trkSeed_description, trackSeed_handle);
  list_of_trackSeeds = (mu2e::KalSeedCollection*) &(*trackSeed_handle);

  const mu2e::KalSeed         *trkSeed(0);
  int                           ntrkseeds(0);
  
  const mu2e::CaloCluster       *cluster(0);
 
  ntrkseeds = list_of_trackSeeds->size();
  

  
  for (int i=0; i<ntrkseeds; i++) {
    trackSeed                  = cb->NewTrackSeed();
    trkSeed                    = &list_of_trackSeeds->at(i);
    cluster                    = trkSeed->caloCluster().get();

    if (cluster != 0){
      trackSeed->fClusterTime    = cluster->time();
      trackSeed->fClusterEnergy  = cluster->energyDep();
      trackSeed->fClusterX       = cluster->cog3Vector().x();
      trackSeed->fClusterY       = cluster->cog3Vector().y();
      trackSeed->fClusterZ       = cluster->cog3Vector().z();
    }else {
      trackSeed->fClusterTime    = 0; 
      trackSeed->fClusterEnergy  = 0; 
      trackSeed->fClusterX       = 0; 
      trackSeed->fClusterY       = 0; 
      trackSeed->fClusterZ       = 0; 
    }
    
    mu2e::KalSegment kalSeg  = trkSeed->segments().at(0);//take the KalSegment closer to the entrance of the tracker
    trackSeed->fTrackSeed    = trkSeed;
    trackSeed->fNHits        = trkSeed->hits().size();//_timeCluster._strawHitIdxs.size();
    trackSeed->fT0           = trkSeed->t0()._t0;
    trackSeed->fT0Err        = trkSeed->t0()._t0err;     
    trackSeed->fD0           = kalSeg.helix().d0();
    trackSeed->fFlt0         = trkSeed->flt0();     
    trackSeed->fTanDip       = kalSeg.helix().tanDip();
    trackSeed->fP            = kalSeg.mom();
    trackSeed->fPt           = trackSeed->fP*std::cos( std::atan(trackSeed->fTanDip));
    trackSeed->fZ0           = kalSeg.helix().z0();

    trackSeed->fChi2         = trkSeed->chisquared();
    trackSeed->fFitCons      = trkSeed->fitConsistency();

    
  }

  //------------------------------------------------------------------------------------------
  // 2016-07-11 G. Pezzullo: how do we want to propagate information of each strawhit 
  //  present in the track candidate?
  //------------------------------------------------------------------------------------------
  // int    ev_number,  rn_number;
  // ev_number = Evt->event();
  // rn_number = Evt->run();
  // cb->f_RunNumber   = rn_number;
  // cb->f_EventNumber = ev_number;



  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2eTrackSeedBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, do nothing

  // Int_t  ev_number, rn_number;

  // ev_number = AnEvent->event();
  // rn_number = AnEvent->run();

  //  if (!Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnTrackSeedBlock* tsb, *ftsb;
  TStnTrackSeed*      trkseed, *ftrkseed;
  TStnHelixBlock*     hb;//, *fhb;
  TStnHelix          *helix;//, *fhelix;
  TStnEvent*          ev;

  const mu2e::HelixSeed* khelix, *fkhelix;
  const mu2e::KalSeed*   kseed, *fkseed;

  tsb    = (TStnTrackSeedBlock*) Block;

  char                kseed_module_label[100], helix_block_name[100], short_helix_block_name[100], short_seed_block_name[100];
  tsb->GetModuleLabel("mu2e::KalSeedCollection"  , kseed_module_label);
  tsb->GetModuleLabel("HelixBlockName"           , helix_block_name  );
  tsb->GetModuleLabel("ShortHelixBlockName"      , short_helix_block_name  );
  tsb->GetModuleLabel("ShortTrackSeedBlockName"  , short_seed_block_name);

  ev     = Block->GetEvent();
  hb     = (TStnHelixBlock*)     ev->GetDataBlock(helix_block_name      );
  //  fhb    = (TStnHelixBlock*)     ev->GetDataBlock(short_helix_block_name);
  ftsb   = (TStnTrackSeedBlock*) ev->GetDataBlock(short_seed_block_name );
  
  int    ntrkseed = tsb ->NTrackSeeds();
  int    ntrk     = ftsb->NTrackSeeds();
  int    nhelix   = hb  ->NHelices();
  
  for (int i=0; i<ntrkseed; i++) {
    trkseed = tsb->TrackSeed(i);
    kseed   = trkseed->fTrackSeed;

    //    fhelix   =  fhb  ->Helix(i);
    fkhelix  = kseed->helix().get();//fhelix->fHelix;
    int  helixIndex(-1);
    for (int j=0; j<nhelix; ++j){
      helix  = hb->Helix(j);
      khelix = helix->fHelix;
      if ( khelix == fkhelix){
	helixIndex = j;
	break;
      }    
    }
    
    if (helixIndex < 0) {
      printf(">>> ERROR: %s trackseed %i -> no HelixSeed associated\n", kseed_module_label, i);
      continue;
    }
    trkseed->SetHelixIndex(helixIndex);
    
    int      trkIndex(-1);
    for (int k=0; k<ntrk; ++k){
      ftrkseed = ftsb->TrackSeed(k);
      fkseed   = ftrkseed->fTrackSeed;
      fkseed   = fkseed->kalSeed().get();
      if (kseed == fkseed) {
	trkIndex = k;
	break;
      }
    }
    
    if (trkIndex < 0) {
      printf(">>> ERROR: %s trackseed %i -> no KalRep associated\n", kseed_module_label, i);
      continue;
    }
    trkseed->SetTrackIndex(trkIndex);
   
  }
//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  Block->SetLinksInitialized();
  //  tsb->fLinksInitialized = 1;


  return 0;
}

