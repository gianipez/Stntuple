//-----------------------------------------------------------------------------
//  Apr 2016 G. Pezzullo: initialization of the MU2E STNTUPLE TrackSeed block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"

#include "Stntuple/obj/TStnDataBlock.hh"

#include "Stntuple/obj/TStnTrackSeed.hh"
#include "Stntuple/obj/TStnTrackSeedBlock.hh"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "TTrackerGeom/inc/TTracker.hh"
// #include "TrkReco/inc/TrkStrawHit.hh"

#include "RecoDataProducts/inc/TrackSeed.hh"
#include "RecoDataProducts/inc/TrackSeedCollection.hh"

//#include "RecoDataProducts/inc/TrackSeed.hh"

//-----------------------------------------------------------------------------
// assume that the collection name is set, so we could grab it from the event
//-----------------------------------------------------------------------------
int  StntupleInitMu2eTrackSeedBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode) {

  mu2e::TrackSeedCollection*  list_of_trackSeeds;

  static char                 trkSeed_module_label[100], trkSeed_description[100]; 

  TStnTrackSeedBlock*         cb = (TStnTrackSeedBlock*) Block;
  TStnTrackSeed*              trackSeed;


  cb->Clear();

  cb->GetModuleLabel("mu2e::TrackSeedCollection", trkSeed_module_label);
  cb->GetDescription("mu2e::TrackSeedCollection", trkSeed_description );

  art::Handle<mu2e::TrackSeedCollection>               trackSeed_handle;
  if (trkSeed_description[0] == 0) Evt->getByLabel(trkSeed_module_label, trackSeed_handle);
  else                          Evt->getByLabel(trkSeed_module_label, trkSeed_description, trackSeed_handle);
  list_of_trackSeeds = (mu2e::TrackSeedCollection*) &(*trackSeed_handle);

  const mu2e::TrackSeed         *trkSeed(0);
  int                           ntrkseeds(0);
  
  const mu2e::CaloCluster       *cluster(0);
 
  ntrkseeds = list_of_trackSeeds->size();
  

  
  for (int i=0; i<ntrkseeds; i++) {
    trackSeed                  = cb->NewTrackSeed();
    trkSeed                    = &list_of_trackSeeds->at(i);
    cluster                    = trkSeed->_caloCluster.get();

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
    
    trackSeed->fTrackSeed   = trkSeed;
    trackSeed->fNLoops      = trkSeed->nLoops();
    trackSeed->fNHits       = trkSeed->_selectedTrackerHits.size();
    trackSeed->fT0          = trkSeed->t0    ();
    trackSeed->fT0Err       = trkSeed->errt0 ();     
    trackSeed->fD0          = trkSeed->d0    ();
    trackSeed->fPhi0        = trkSeed->phi0  ();     
    trackSeed->fOmega       = trkSeed->omega ();
    trackSeed->fZ0          = trkSeed->z0    ();     
    trackSeed->fTanDip      = trkSeed->tanDip();
    
  }

  //------------------------------------------------------------------------------------------
  // 2016-04-11 G. Pezzullo: how do we want to propagate information of each strawhit 
  //  present in the track candidate?
  //------------------------------------------------------------------------------------------


  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2eTrackSeedBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, do nothing

  Int_t  ev_number, rn_number;

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (! Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnTrackSeedBlock* tsb = (TStnTrackSeedBlock*) Block;
  // TStnClusterBlock*   clb = (TStnClusterBlock*  ) Block->GetEvent()->GetDataBlock("ClusterBlock");

  // int nclusters = clb->NClusters();

  // if (! tdata) return -1;


  // //  TStnEvent* ev   = header->GetEvent();
  // for (int i=0; i<ntrkseeds; i++) {
  //   trkSeed  = tsb->TrackSeed(i);
  //   b1      = trkSeed->fCaloCluster;

  //   for (icl=0; icl<ncl; icl++) {
  //     TStnCluster* cl = clb->Cluster(icl);
  //     b2 = cl->fCaloCluster;

  //     if (b1 == b2) {
  //     }
  //   }


  //   //trackerHitTime = trkSeed->_relatedTimeCluster.operator ->();
  // }
//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  tsb->fLinksInitialized = 1;


  return 0;
}

