#include <iostream>
#include <iomanip>

#include "obj/TStnTimePeakBlock.hh"
#include "obj/TStnTimePeak.hh"

ClassImp(TStnTimePeakBlock)

//_____________________________________________________________________________
void TStnTimePeakBlock::Streamer(TBuffer &R__b) {
  // Stream an object of class TStnTrackBlock.

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    R__b >> fNTimePeaks;
    fListOfTimePeaks->Streamer(R__b);
    // for (int i=0; i<fNTracks; i++) {
    //   Track(i)->SetNumber(i);
    // }
  } 
  else {
    R__b.WriteVersion(TStnTimePeakBlock::IsA());
    R__b << fNTimePeaks;
    fListOfTimePeaks->Streamer(R__b);
  }
}
TStnTimePeakBlock::TStnTimePeakBlock() {
  fNTimePeaks      = 0;
  fListOfTimePeaks = new TClonesArray("TStnTimePeak",100);
  fListOfTimePeaks->BypassStreamer(kFALSE);
  fCollName  = "default";
}


//_____________________________________________________________________________
TStnTimePeakBlock::~TStnTimePeakBlock() {
  fListOfTimePeaks->Delete();
  delete fListOfTimePeaks;
}


//_____________________________________________________________________________
void TStnTimePeakBlock::Clear(Option_t* opt) {
  fNTimePeaks = 0;
  fListOfTimePeaks->Clear(opt);
}

//------------------------------------------------------------------------------
void TStnTimePeakBlock::Print(Option_t* opt) const {

  int banner_printed = 0;
  for (int i=0; i<fNTimePeaks; i++) {
    TStnTimePeak* t = ((TStnTimePeakBlock*) this)->TimePeak(i);
    if (! banner_printed) {
      t->Print("banner");
      banner_printed = 1;
    }
    t->Print("data");
  }
}
