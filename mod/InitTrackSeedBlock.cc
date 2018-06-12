//-----------------------------------------------------------------------------
//  Apr 2016 G. Pezzullo: initialization of the MU2E STNTUPLE TrackSeed block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"


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
#include "RecoDataProducts/inc/StrawHitCollection.hh"
#include "RecoDataProducts/inc/StrawHit.hh"
//#include "RecoDataProducts/inc/TrkStrawHitSeed.hh"


#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"


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

  art::Handle<mu2e::PtrStepPointMCVectorCollection> mcptrHandleStraw;
  Evt->getByLabel("makeSD","",mcptrHandleStraw);
  mu2e::PtrStepPointMCVectorCollection const* hits_mcptrStraw = mcptrHandleStraw.product();

  std::vector<int>     hits_simp_id, hits_simp_index;
  TParticlePDG*        part;
  TDatabasePDG*        pdg_db = TDatabasePDG::Instance();
 
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

    //now loop over the hits to search the particle that generated the track
    int nsh = trkSeed->hits().size();
    
    for (int j=0; j<nsh; ++j){
      int  hitIndex  = int(trkSeed->hits().at(j).index());

      mu2e::PtrStepPointMCVector const& mcptr(hits_mcptrStraw->at(hitIndex));
      const mu2e::StepPointMC* Step = mcptr[0].get();
      if (Step) {
	art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
	int sim_id        = simptr->id().asInt();

	hits_simp_id.push_back   (sim_id); 
	hits_simp_index.push_back(hitIndex);
      }
    }//end loop over the hits
    

 //find the simparticle that created the majority of the hits
    int     max(0), mostvalueindex(-1), mostvalue= hits_simp_id[0];
    for (int k=0; k<nsh; ++k){
      int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), hits_simp_id[k]);
      if ( co>max) {
	max            = co;
	mostvalue      = hits_simp_id[k];
	mostvalueindex = hits_simp_index[k];
      }
    }

    //    trackSeed->fSimpId1     = mostvalue;
    trackSeed->fSimpId1Hits = max;
    //set defaults
    // trackSeed->fSimpId2     = -1;
    trackSeed->fSimpId2Hits = -1;

    mu2e::PtrStepPointMCVector const& mcptr(hits_mcptrStraw->at(mostvalueindex) );
    const mu2e::StepPointMC* Step = mcptr[0].get();
    const mu2e::SimParticle * sim (0);

    if (Step) {
      art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
      trackSeed->fSimpPDG1    = simptr->pdgId();
      art::Ptr<mu2e::SimParticle> mother = simptr;
      part   = pdg_db->GetParticle(trackSeed->fSimpPDG1);

      while(mother->hasParent()) mother = mother->parent();
      sim = mother.operator ->();

      trackSeed->fSimpPDGM1   = sim->pdgId();
      
      double   px = simptr->startMomentum().x();
      double   py = simptr->startMomentum().y();
      double   pz = simptr->startMomentum().z();
      double   mass(-1.);//  = part->Mass();
      double   energy(-1.);// = sqrt(px*px+py*py+pz*pz+mass*mass);
      if (part) {
	mass   = part->Mass();
	energy = sqrt(px*px+py*py+pz*pz+mass*mass);
      }
      trackSeed->fMom1.SetPxPyPzE(px,py,pz,energy);

      const CLHEP::Hep3Vector* sp = &simptr->startPosition();
      trackSeed->fOrigin1.SetXYZT(sp->x(),sp->y(),sp->z(),simptr->startGlobalTime());
  
      // trackSeed->fSimp1P      = Step->momentum().mag();
      // trackSeed->fSimp1Pt     = sqrt(pow(Step->momentum().x(),2.) + pow(Step->momentum().y(),2.));
    }
    
    //look for the second most frequent hit
    if (max != nsh && max > 0){
      int   secondmostvalueindex(-1);
      max = 0;//reset max

      for (int k=0; k<nsh; ++k){
	int value = hits_simp_id[k];
	int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), value);
	if ( (co>max) && (value != mostvalue)) {
	  max                  = co;
	  // secondmostvalue      = value;
	  secondmostvalueindex = hits_simp_index[k];
	}
      }
      //      trackSeed->fSimpId2     = secondmostvalue;
      trackSeed->fSimpId2Hits = max;

      if (secondmostvalueindex >=0){
      mu2e::PtrStepPointMCVector const& mcptr(hits_mcptrStraw->at(secondmostvalueindex) );
      const mu2e::StepPointMC* Step = mcptr[0].get();
      const mu2e::SimParticle * sim (0);

      if (Step) {
	art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
	trackSeed->fSimpPDG2    = simptr->pdgId();
	art::Ptr<mu2e::SimParticle> mother = simptr;
	part   = pdg_db->GetParticle(trackSeed->fSimpPDG2);

	while(mother->hasParent()) mother = mother->parent();
	sim = mother.operator ->();

	trackSeed->fSimpPDGM2   = sim->pdgId();
      
	double   px = simptr->startMomentum().x();
	double   py = simptr->startMomentum().y();
	double   pz = simptr->startMomentum().z();
	double   mass(-1.);//  = part->Mass();
	double   energy(-1.);// = sqrt(px*px+py*py+pz*pz+mass*mass);
	if (part) {
	  mass   = part->Mass();
	  energy = sqrt(px*px+py*py+pz*pz+mass*mass);
	}
	trackSeed->fMom2.SetPxPyPzE(px,py,pz,energy);

	const CLHEP::Hep3Vector* sp = &simptr->startPosition();
	trackSeed->fOrigin2.SetXYZT(sp->x(),sp->y(),sp->z(),simptr->startGlobalTime());
  
	// trackSeed->fSimp2P      = Step->momentum().mag();
	// trackSeed->fSimp2Pt     = sqrt(pow(Step->momentum().x(),2.) + pow(Step->momentum().y(),2.));
      }      
    }
    }
    
    
  }//end loop over the trackSeeds

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

