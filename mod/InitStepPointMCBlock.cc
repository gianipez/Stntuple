///////////////////////////////////////////////////////////////////////////////
// stores one StepPointMCCollection in STNTUPLE data block
///////////////////////////////////////////////////////////////////////////////
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"

#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawHitMCTruthCollection.hh"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "DataProducts/inc/VirtualDetectorId.hh"

#include "Stntuple/obj/TStepPointMCBlock.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"

//-----------------------------------------------------------------------------
int StntupleInitMu2eStepPointMCBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int mode) 
{
  // 
  const char* oname = {"StntupleInitMu2eStepPointMCBlock"};

  static char   module_label  [100], description  [100];

  const mu2e::StepPointMCCollection*       spmc_coll(0);
  //  const mu2e::SimParticle*                 sim(0);
  art::Handle<mu2e::StepPointMCCollection> spmc_handle;

  //  TStepPointMC*      step;
  TStepPointMCBlock* spmc_block = (TStepPointMCBlock*) Block;
  spmc_block->Clear();

  Block->GetModuleLabel("mu2e::StepPointMCCollection",module_label);
  Block->GetDescription("mu2e::StepPointMCCollection",description );

  const mu2e::StepPointMC*                    spmc;
//-----------------------------------------------------------------------------
//  const art::Provenance*                 prov;
//  art::Selector  selector(art::ProductInstanceNameSelector(""));
//  AnEvent->getMany(selector, list_of_sp);
//
//   for (std::vector<art::Handle<mu2e::SimParticleCollection>> ::const_iterator it = list_of_sp.begin();
//        it != list_of_sp.end(); it++) {
//     handle = it.operator -> ();

  art::Selector  selector(art::ProcessNameSelector("")               && 
			  art::ModuleLabelSelector(module_label)     && 
			  art::ProductInstanceNameSelector(description) );

  AnEvent->get(selector,spmc_handle);

  if (spmc_handle.isValid()) {
    spmc_coll = spmc_handle.product();
    //      prov = handle->provenance();
      
    //       printf("moduleLabel = %-20s, producedClassname = %-30s, productInstanceName = %-20s\n",
    // 	     prov->moduleLabel().data(),
    // 	     prov->producedClassName().data(),
    // 	     prov->productInstanceName().data());

    for (mu2e::StepPointMCCollection::const_iterator ip = spmc_coll->begin(); ip != spmc_coll->end(); ip++) {
      spmc = &(*ip);
      
      int   volume_id = spmc->volumeId();

      int   id (-1), gen_index(-1), pdg_code(-1), parent_pdg_code (-1), parent_id(-1);
      int   creation_code(-1), end_process_code(-1);
      float edep_tot (-1.), edep_nio(-1.), time(-1.), step_length(-1.);

      art::Ptr<mu2e::SimParticle> const& simptr = spmc->simParticle();
      const mu2e::SimParticle* sim  = simptr.get();

      if (sim == NULL) {
	printf(">>> ERROR: %s sim == NULL, skip \n",oname);
      }
      else {
	id            = sim->id().asInt();
	gen_index     = sim->generatorIndex();
	pdg_code      = sim->pdgId();
	creation_code = sim->creationCode();

	art::Ptr<mu2e::SimParticle> const& parentptr = sim->parent();

	const mu2e::SimParticle* par = parentptr.get();
	if (par != NULL) {
	  parent_pdg_code = (int) par->pdgId();
	  parent_id       = (int) par->id().asInt();
	}
	
	end_process_code = spmc->endProcessCode();

	edep_tot    = spmc->totalEDep();
	edep_nio    = spmc->nonIonizingEDep();
	time        = spmc->time();
	step_length = spmc->stepLength();
      }

      spmc_block->NewStepPointMC(volume_id, gen_index, 
				 id       , pdg_code       ,
				 parent_id, parent_pdg_code, 
				 creation_code, end_process_code,
				 edep_tot,edep_nio,
				 time, step_length,
				 spmc->position().x(),
				 spmc->position().y(),
				 spmc->position().z(),
				 spmc->momentum().x(),
				 spmc->momentum().y(),
				 spmc->momentum().z());
    }
  }

  return 0;
}


