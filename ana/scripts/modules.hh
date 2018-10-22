#ifndef stntuple_ana_scripts_modules_hh
#define stntuple_ana_scripts_modules_hh

// #include "Stntuple/ana/TDFCModule.hh"

class TStnPhotosAnaModule;
class TStnEventDisplayModule;
class TStnValidationModule;
class TStnDebugModule;
class TStnLumiMonModule;

namespace stntuple {

  TStnDebugModule*            m_dbg   = NULL;
  TDFCModule*                 m_dfc   = NULL;
  TStnLumiMonModule*          m_lum   = NULL;
  TStnEventDisplayModule*     m_evd   = NULL;
  TStnPhotosAnaModule*        m_pho   = NULL;
  TStnValidationModule*       m_val   = NULL;
};

#endif
