// tree created outside
#include "TTree.h"

//-----------------------------------------------------------------------------
// the tree should be created prior to calling
//----------------------------------------------------------------------------- 
int read_combo_hits(const char* Fn, TTree* Tree) { 
  
  TString format = "i/I:nsh/I:sid/I:flags/C:pln/I:pnl/I:lay/I:str/I:x/F:y/F:z/F:time/F:";
  format        += "edep/F:end/I:drtime/F:prtime/F:tres/F:wdist/F:wres/F:pdg/I:pdgm/I:gen/I:id/I:p/F:pz/F";

  return Tree->ReadFile(Fn,format.Data(),' ');
}
