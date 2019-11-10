///////////////////////////////////////////////////////////////////////////////
//  description of a trig DB entry to be stored in the STNTUPLE DB subdirectory
//
//  Author:    P.Murat (CDF/FNAL)
//  Date:      Nov 09 2001
//             Nov 09 2019 : adopt for Mu2e
///////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "obj/TStnTrigger.hh"


ClassImp(TStnTrigger)

//-----------------------------------------------------------------------------
void TStnTrigger::Streamer(TBuffer& R__b) {

  int nwi;

  nwi      = ((int*  ) &fName) - &fBit;

  if (R__b.IsReading()) {
//-----------------------------------------------------------------------------
// read TStnTrigger, Mu2e:V1
//-----------------------------------------------------------------------------
    /* Version_t R__v = */ R__b.ReadVersion(); 
    R__b.ReadFastArray(&fBit,nwi);
    fName.Streamer(R__b);
  }
  else {
    R__b.WriteVersion(TStnTrigger::IsA());
    
    R__b.WriteFastArray(&fBit, nwi);
    fName.Streamer(R__b);
  }
}

//_____________________________________________________________________________
TStnTrigger::TStnTrigger() {
  fBit   = -1;
  fTag   = -1;
  fId    = -1;
}

//_____________________________________________________________________________
TStnTrigger::TStnTrigger(int Bit, int Id, const char* Name, int Tag) {
  fBit   = Bit;
  fTag   = Tag;
  fId    = Id;
  fName  = Name;
}

//_____________________________________________________________________________
TStnTrigger::~TStnTrigger() {
}

//_____________________________________________________________________________
void TStnTrigger::Print(Option_t* Opt) const {
  if ((strcmp(Opt,"banner") == 0) || (strcmp(Opt,"") == 0)) {
    printf("----------------------------------------------------------------\n");
    printf(" Bit       Id    Name   \n");
    printf("----------------------------------------------------------------\n");
  }

  if ((strcmp(Opt,"data") == 0) || (strcmp(Opt,"") == 0)) {
    printf(" %3i   %6i  %s_v-%i\n",fBit,fId,fName.Data(),fTag);
  }
}

