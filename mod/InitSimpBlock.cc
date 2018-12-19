///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <vector>

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"
#include "canvas/Utilities/InputTag.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "GeometryService/inc/VirtualDetector.hh"

#include "DataProducts/inc/VirtualDetectorId.hh"

#include "MCDataProducts/inc/GenParticle.hh"
#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawDigiMCCollection.hh"

#include "RecoDataProducts/inc/StrawHitCollection.hh"

#include "Stntuple/obj/TSimpBlock.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"


#include "Stntuple/mod/THistModule.hh"
#include "Stntuple/base/TNamedHandle.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

//-----------------------------------------------------------------------------
// fill SimParticle's data block
//-----------------------------------------------------------------------------
int StntupleInitMu2eSimpBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int mode) 
{
  const char* oname = {"StntupleInitMu2eSimpBlock"};

  static int    initialized(0), primary_only(0);
  static char   strh_module_label[100], strh_description[100];
  static char   sdmc_module_label[100], sdmc_description[100];
  static char   g4_module_label  [100], g4_description  [100];

  static char   store_primary_only[10];  // "0" or not

  static double _min_energy;

  std::vector<art::Handle<mu2e::SimParticleCollection>> list_of_sp;

  const mu2e::SimParticleCollection*       simp_coll(0);
  const mu2e::SimParticle*                 sim(0);
  const mu2e::StrawHitCollection*          list_of_straw_hits(0);

  double        px, py, pz, energy;
  int           id, parent_id, generator_id, n_straw_hits(0), nhits;
  int           pdg_code, start_vol_id, end_vol_id, creation_code, termination_code;
  TSimParticle* simp;

  TSimpBlock* simp_block = (TSimpBlock*) Block;
  simp_block->Clear();

  char   module_name[100], h_name[100];

  if (initialized == 0) {
    initialized = 1;

    Block->GetModuleLabel("MinSimpEnergyHandle",module_name);
    Block->GetDescription("MinSimpEnergyHandle",h_name     );

    THistModule*  m;
    TNamedHandle* nh;

    m    = static_cast<THistModule*>  (THistModule::GetListOfModules()->FindObject(module_name));
    nh   = static_cast<TNamedHandle*> (m->GetFolder()->FindObject(h_name));
    _min_energy = *(static_cast<double*> (nh->Object()));
  }

  Block->GetModuleLabel("mu2e::StrawHitCollection",strh_module_label);
  Block->GetDescription("mu2e::StrawHitCollection",strh_description );

  Block->GetModuleLabel("mu2e::StepPointMCCollection",g4_module_label);
  Block->GetDescription("mu2e::StepPointMCCollection",g4_description );

  Block->GetModuleLabel("StorePrimaryOnly",store_primary_only);
  if (store_primary_only[0] == '0') primary_only = 0;
  else                              primary_only = 1;

  art::Handle<mu2e::StrawHitCollection> shHandle;
  if (strh_module_label[0] != 0) {
    if (strh_description[0] == 0) AnEvent->getByLabel(strh_module_label,shHandle);
    else                          AnEvent->getByLabel(strh_module_label,strh_description,shHandle);
    if (shHandle.isValid()) {
      list_of_straw_hits = shHandle.product();
      n_straw_hits      = list_of_straw_hits->size();
    }
  }

  Block->GetModuleLabel("mu2e::StrawDigiMCCollection",sdmc_module_label);
  Block->GetDescription("mu2e::StrawDigiMCCollection",sdmc_description );

  char sdmc_tag[100];
  strcpy(sdmc_tag,sdmc_module_label);
  if (sdmc_description[0] != 0) {
    strcat(sdmc_tag,":");
    strcat(sdmc_tag,sdmc_description);
  }
    
  auto mcdH = AnEvent->getValidHandle<mu2e::StrawDigiMCCollection>(sdmc_tag);
  const mu2e::StrawDigiMCCollection* mcdigis = mcdH.product();

  mu2e::GeomHandle<mu2e::VirtualDetector> vdg;

  char simp_coll_tag[100];
  Block->GetCollTag("mu2e::SimParticleCollection",simp_coll_tag);
  auto simp_handle = AnEvent->getValidHandle<mu2e::SimParticleCollection>(simp_coll_tag);
//-----------------------------------------------------------------------------
// figure out how many straw hits each particle has produced
//-----------------------------------------------------------------------------
  std::vector<int> vid, vin;
  int np_with_hits(0);   // number of particles with straw hits

  if (n_straw_hits > 0) {

    vid.reserve(n_straw_hits);
    vin.reserve(n_straw_hits);

    for (int i=0; i<n_straw_hits; i++) vin[i] = 0;

    const mu2e::StepPointMC* step;

    for (int i=0; i<n_straw_hits; i++) {
      const mu2e::StrawDigiMC* mcdigi = &mcdigis->at(i);
      if (mcdigi->wireEndTime(mu2e::StrawEnd::cal) < mcdigi->wireEndTime(mu2e::StrawEnd::hv)) {
	step = mcdigi->stepPointMC(mu2e::StrawEnd::cal).get();
      }
      else {
	step = mcdigi->stepPointMC(mu2e::StrawEnd::hv ).get();
      }
    
      art::Ptr<mu2e::SimParticle> const& simptr = step->simParticle(); 
      art::Ptr<mu2e::SimParticle> mother        = simptr;
      while(mother->hasParent())  mother        = mother->parent();
      const mu2e::SimParticle*    sim           = mother.get();
	  
      int sim_id = sim->id().asInt();

      int found  = 0;
      for (int ip=0; ip<np_with_hits; ip++) {
	if (sim_id == vid[ip]) {
	  vin[ip] += 1;
	  found    = 1;
	  break;
	}
      }

      if (found == 0) {
	vid[np_with_hits] = sim_id;
	vin[np_with_hits] = 1;
      }
    }
  }

  if (simp_handle.isValid()) {
    simp_coll = simp_handle.product();

    for (mu2e::SimParticleCollection::const_iterator ip = simp_coll->begin(); ip != simp_coll->end(); ip++) {
      sim      = &ip->second;
      const mu2e::GenParticle* genp = sim->genParticle().get();

      if (primary_only && (genp == nullptr))                continue;
      id        = sim->id().asInt();
      parent_id = -1;
      if (sim->parent()) parent_id = sim->parent()->id().asInt();

      pdg_code         = (int) sim->pdgId();

      if (genp) generator_id = genp->generatorId().id(); // ID of the MC generator
      else      generator_id = -1;

      creation_code    = sim->creationCode();
      termination_code = sim->stoppingCode();

      start_vol_id     = sim->startVolumeIndex();
      end_vol_id       = sim->endVolumeIndex();
      
      px     = sim->startMomentum().x();
      py     = sim->startMomentum().y();
      pz     = sim->startMomentum().z();
      energy = sim->startMomentum().e();

      if (energy < _min_energy)                             continue;
      simp   = simp_block->NewParticle(id, parent_id, pdg_code, 
				       creation_code, termination_code,
				       start_vol_id, end_vol_id,
				       generator_id);
      simp->SetStartMom(px, py, pz, energy);
      const CLHEP::Hep3Vector* sp = &sim->startPosition();
      simp->SetStartPos(sp->x(),sp->y(),sp->z(),sim->startGlobalTime());
      simp->SetEndMom  (sim->endMomentum().x(),
			sim->endMomentum().y(),
			sim->endMomentum().z(),
			sim->endMomentum().e());
      const CLHEP::Hep3Vector* ep = &sim->endPosition();
      simp->SetEndPos(ep->x(),ep->y(),ep->z(),sim->endGlobalTime());
//-----------------------------------------------------------------------------
// particle parameters at virtual detectors
//-----------------------------------------------------------------------------
      if (vdg->nDet() > 0) {
	art::Handle<mu2e::StepPointMCCollection> vdhits;
	AnEvent->getByLabel(g4_module_label,g4_description,vdhits);
	if (!vdhits.isValid()) {
	  char warning[100];
	  sprintf(warning,"StepPointMCCollection %s:%s NOT FOUND\n",g4_module_label,g4_description);
	  mf::LogWarning(oname) << warning;
	}
	else {
	  int nvdhits = vdhits->size();
	  for (int i=0; i<nvdhits; i++) {
	    const mu2e::StepPointMC* hit = &(*vdhits)[i];
	    
	    mu2e::VirtualDetectorId vdid(hit->volumeId());
	    
	    if (vdid.id() == mu2e::VirtualDetectorId::ST_Out) {
	      
	      const mu2e::SimParticle* sim = hit->simParticle().get();
	      
	      if (sim == NULL) {
		printf(">>> ERROR: %s sim == NULL\n",oname);
	      }
	      int sim_id = sim->id().asInt();
	      if (sim_id == id) {
		simp->SetMomTargetEnd(hit->momentum().mag());
	      }
	    }
	    else if (vdid.isTrackerFront()) {
	      art::Ptr<mu2e::SimParticle> const& simptr = hit->simParticle();
	      const mu2e::SimParticle* sim = simptr.get();
	      
	      if (sim == NULL) {
		printf("[%s] ERROR: sim == NULL. CONTINUE.\n",oname);
	      }
	      else {
		int sim_id = sim->id().asInt();
		if (sim_id == id) {
		  simp->SetMomTrackerFront(hit->momentum().mag());
		}
	      }
	    }
	    
	  }
	}
      }
//------------------------------------------------------------------------------
// count number of straw hits produced by the particle
//-----------------------------------------------------------------------------
      nhits = 0;
      
      for (int i=0; i<np_with_hits; i++) {
	if (vid[i] == id) {
	  nhits = vin[i];
	  break;
	}
      }
      simp->SetNStrawHits(nhits);
    }
  }
  else {
//-----------------------------------------------------------------------------
// no SIMP collection
//-----------------------------------------------------------------------------
    mf::LogWarning(oname) << " WARNING in " << oname << ": SimParticleCollection " 
			  << g4_module_label << " not found, BAIL OUT\n";
    return -1;
  }

  return 0;
}


