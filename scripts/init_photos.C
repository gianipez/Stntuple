//

#include "mc/photos/TPhotos.hh"

#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"
#include "Stntuple/ana/TStnGeneratorModule.hh"

void init_photos() {
  TG3Generator* phts  = TPhotos::Instance();
  m_stg = new TStnGeneratorModule();
  m_stg->AddGenerator(phts);
  g.x->SetInputModule(m_stg);
}
