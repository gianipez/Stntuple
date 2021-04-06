//////////////////////////////////////////////////////////////////////////////
#ifndef __Stntuple_print_functions_hh__
#define __Stntuple_print_functions_hh__

#ifndef __CINT__

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"

#else

namespace art {
  class Event;
}

#endif

void     print_combo_hit_coll  (const char* ComboHitCollTag, const char* StrawHitCollTag = "makeSH");
void     print_combo_hit_coll_2(const char* ComboHitCollTag, const char* StrawHitCollTag = "makeSH");

void     print_ch_colls        ();   // print combo hit colls
void     print_helix_seed_colls();
void     print_kalrep_colls    ();
void     print_kalseed_colls   ();
void     print_sd_colls        ();
void     print_sdmc_colls      ();
void     print_sh_colls        ();   // print straw hit collections
void     print_spmc_colls      ();   // print all StepPointMC collections
void     print_tc_colls        ();   // print time cluster collections

#endif
