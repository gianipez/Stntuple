///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "RecoDataProducts/inc/HelixSeed.hh"

#include "Stntuple/print/print/TAnaDump.hh"
#include "Stntuple/print/print/Stntuple_print_functions.hh"
//-----------------------------------------------------------------------------
// print all HelixSeed collections in the event
//-----------------------------------------------------------------------------
void print_helix_seed_colls() {

  printf("--------------------------- Available HelixSeedCollections: \n");

  std::vector<art::Handle<mu2e::HelixSeedCollection>> vcoll;

  const art::Event* event = TAnaDump::Instance()->Event();

  art::Selector  selector(art::ProductInstanceNameSelector(""));
  event->getMany(selector,vcoll);

  for (auto handle = vcoll.begin(); handle != vcoll.end(); handle++) {
    if (handle->isValid()) {
      const art::Provenance* prov = handle->provenance();
      
      printf("moduleLabel: %-20s, productInstanceName: %-20s, processName:= %-30s\n" ,
	     prov->moduleLabel().data(),
	     prov->productInstanceName().data(),
	     prov->processName().data()
	     );
    }
  }
  return;
}
