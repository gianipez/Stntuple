///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "RecoDataProducts/inc/TimeCluster.hh"
#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "Stntuple/print/print/TAnaDump.hh"
#include "Stntuple/print/print/Stntuple_print_functions.hh"
//-----------------------------------------------------------------------------
// print all TimeClusterCollection's in the event
//-----------------------------------------------------------------------------
void print_tc_colls() {

  printf("Available TimeClusterCollections: \n");

  std::vector<art::Handle<mu2e::TimeClusterCollection>> vcoll;

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
