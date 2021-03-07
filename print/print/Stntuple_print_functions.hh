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

#endif
