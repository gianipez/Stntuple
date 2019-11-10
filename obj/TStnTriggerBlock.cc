///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "obj/TStnTriggerTable.hh"
#include "obj/TStnTriggerBlock.hh"
// #include "obj/TStnMcCalibrations.hh"
#include "obj/TStnEvent.hh"
#include "obj/TStnNode.hh"
#include "obj/TStnHeaderBlock.hh"
#include "TMath.h"

ClassImp(TStnTriggerBlock)

//______________________________________________________________________________
void TStnTriggerBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class TStnTriggerBlock.
  
  int   nwi = (Short_t*)&fPaths-&fNPaths;
  
  if (R__b.IsReading()) {
    /* int ver = */ R__b.ReadVersion(); 
    R__b.ReadFastArray(&fNPaths,nwi);
    fPaths.Streamer(R__b);
  } 
  else {
    R__b.WriteVersion(TStnTriggerBlock::IsA());

					// write fixed part 
    R__b.WriteFastArray(&fNPaths,nwi);
    fPaths.Streamer(R__b);
  }
}

//_____________________________________________________________________________
TStnTriggerBlock::TStnTriggerBlock() {
//  for(int i=0; i<4; i++) Beam[i] = 0.0;
}

//_____________________________________________________________________________
TStnTriggerBlock::~TStnTriggerBlock() {
}


//_____________________________________________________________________________
Int_t TStnTriggerBlock::GetListOfPassedTriggers(const TStnTriggerTable* Table,
						const char*             Name, 
						Int_t                   Level,
						TObjArray*              List) 
{
  // assume that trigger table has been already initialized
  // if Name=0 include all passed triggers

  if (List == 0) return -1;

  List->Clear();

//-----------------------------------------------------------------------------
//  
//-----------------------------------------------------------------------------
  for (int i=0; i<fNPaths; i++) {
    const TStnTrigger* trig = Table->GetTrigger(i);
    if ((Name == 0) || (strstr(trig->GetName(),Name) != 0)) {
      List->Add((TObject*)trig);
    }
    else {
      Warning("GetListOfTriggers","doesn't exist!\n");
    }
  }

  return List->GetEntriesFast();
}



//_____________________________________________________________________________
void TStnTriggerBlock::Clear(Option_t* opt) {
  fNPaths   = -1;
  fNModSeq  = -1;
  fNModules = -1;
  fPaths.Clear();
}

//_____________________________________________________________________________
void TStnTriggerBlock::Print(Option_t* opt) const {
  // print contents of the trigger block
					// TSID
 //  printf(" ---------------------------------- TSID ------------------------------\n");
 //  printf(" part mess_id bc bheader turn_counter B0 BC TI AM Myron L2B ev_id \n");
 //  printf(" ----------------------------------------------------------------------\n");
 //  printf(" %4i",fTsid.PartitionNumber());
 //  printf(" %7i",fTsid.MessageId());
 //  printf(" %2i",fTsid.BunchCounter());
 //  printf(" %6x",fTsid.BoardHeader());
 //  printf(" %10i",fTsid.TurnCounter());
 //  printf(" %2i",fTsid.B0Marker());
 //  printf(" %2i",fTsid.BCMarker());
 //  printf(" %2i",fTsid.TIMarker());
 //  printf(" %2i",fTsid.AbortMarker());
 //  printf(" %2i",fTsid.MyronBucket());
 //  printf(" %1i",fTsid.L2Buffer());
 //  printf(" %10i",fTsid.EventId());
 //  printf("\n");

 // if (strstr(opt,"tl1d") != 0 || strstr(opt,"TL2D") != 0) {
 //   fTl1d.Print();
 // }

 // if (strstr(opt,"tl2d") == 0 || strstr(opt,"TL2D") != 0) {
 //   fTl2d.Print();
 // }
}
