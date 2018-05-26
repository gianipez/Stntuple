#ifndef stntuple_ana_scripts_modules_hh
#define stntuple_ana_scripts_modules_hh

// #include "Stntuple/ana/TDFCModule.hh"

class TPhotosAnaModule;
class TEventDisplayModule;
class TValidationModule;
class TDebugModule;
class TLumiMonModule;

namespace stntuple {

  TDebugModule*            m_dbg   = NULL;
  TDFCModule*              m_dfc   = NULL;
  TLumiMonModule*          m_lum   = NULL;
  TEventDisplayModule*     m_evd   = NULL;
  TPhotosAnaModule*        m_pho   = NULL;
  TValidationModule*       m_val   = NULL;
};

#endif
