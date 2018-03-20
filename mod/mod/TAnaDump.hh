//
#ifndef __murat_inc_TAnaDump_hh__
#define __murat_inc_TAnaDump_hh__

#include "TObject.h"
#include "TObjArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"

#ifndef __CINT__

#include "art/Framework/Principal/Event.h"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "RecoDataProducts/inc/XYZVec.hh"

#else

namespace art {
  class Event;
}

namespace mu2e {
  class PtrStepPointMCVectorCollection;
}

#endif

class KalRep;

namespace mu2e {
  class StrawHit;
  class StrawHitMCTruth;
  class CaloCluster;
  class CaloProtoCluster;
  class TrkToCaloExtrapol;
  class StepPointMC;
  class GenParticle;
  class SimParticle;
  class CalTimePeak;
  class KalSeed;
  class HelixSeed;
  class ComboHit;
  class TrackClusterMatch;
  class TrkCaloHit;
  class TrkStrawHit;
  class SimParticleTimeOffset;
}

class TAnaDump : public TObject {
public:

  const art::Event*                     fEvent;
  TObjArray*                            fListOfObjects;
  TString                               fFlagBgrHitsModuleLabel;
  mu2e::SimParticleTimeOffset*          fTimeOffsets;
  mu2e::PtrStepPointMCVectorCollection* fListOfMCStrawHits;
  double                                fTmp[100];  // for testing

private:

  TAnaDump(int TimeOffsets = 1);
  ~TAnaDump();
  
  class  Cleaner {
  public: 
    Cleaner();
    ~Cleaner();
  };
  
  friend class Cleaner;

public:

  static TAnaDump* fgInstance;

//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
  static TAnaDump*  Instance(int UseTimeOffsets = 1);


  void   AddObject      (const char* Name, void* Object);
  void*  FindNamedObject(const char* Name);

  void SetEvent(art::Event& Evt) { fEvent = &Evt; }
  void SetFlagBgrHitsModuleLabel(const char* Label) { fFlagBgrHitsModuleLabel = Label; }

  double evalWeight(XYZVec& HitPos   ,
		    XYZVec& StrawDir ,
		    XYZVec& HelCenter, 
		    double             Radius   ,
		    int                WeightMode,
		    fhicl::ParameterSet const& Pset);

  void printEventHeader();

  void printCaloCrystalHits (const char* ModuleLabel, 
			     const char* ProductName = "", 
			     const char* ProcessName = ""); 

  void printCaloDigiCollection(const char* ModuleLabel, 
			       const char* ProductName = "", 
			       const char* ProcessName = ""); 

  void printCaloRecoDigiCollection(const char* ModuleLabel, 
				   const char* ProductName = "", 
				   const char* ProcessName = "");

  void printCaloHits        (const char* ModuleLabel, 
			     const char* ProductName = "", 
			     const char* ProcessName = ""); 

  void printDiskCalorimeter();

  void printCaloCluster          (const mu2e::CaloCluster* Cluster ,
				  const char*              Opt = "");
  
  void printCaloClusterCollection (const char* ModuleLabel, 
				   const char* ProductName,
				   const char* ProcessName);
  
  void printCaloProtoCluster      (const mu2e::CaloProtoCluster* Clu     ,
				   const char*                   Opt = "");
  
  void printCaloProtoClusterCollection (const char* ModuleLabel, 
					const char* ProductName,
					const char* ProcessName);
  
  void printTrackSeed          (const mu2e::KalSeed* TrkSeed, 
				const char* Opt = ""    ,
				const char* ModuleLabelStrawHit="makeSH");

  void printTrackSeedCollection(const char* ModuleLabel             , 
				const char* ProductName         = "", 
				const char* ProcessName         = "",
				int         hitOpt              = 0,
				const char* ModuleLabelStrawHit ="makeSH"); 

  void printHelixSeed          (const mu2e::HelixSeed*         Helix, 
				const char* Opt = ""                ,
				const char* ModuleLabelStrawHit="makeSH" );

  void printHelixSeedCollection(const char* ModuleLabel             , 
				const char* ProductName         = "", 
				const char* ProcessName         = "",
				int         hitOpt              = 0,
				const char* ModuleLabelStrawHit ="makeSH"); 

//   void plotTrackSeed           (int Index                      , 
// 				const char* ModuleLabelTrkSeeds,
// 				const char* ModuleLabelHitPos   );



  void printTrkCaloHit(const KalRep* Krep, mu2e::TrkCaloHit* CaloHit);

  void printKalRep(const KalRep* Krep, const char* Opt = "", const char* Prefix = "");

  void printKalRepCollection(const char* ModuleLabel     , 
			     const char* ProductName = "", 
			     const char* ProcessName = "",
			     int         hitOpt      = 0); 
  
  void printTrkToCaloExtrapol           (const mu2e::TrkToCaloExtrapol*extrk,
					 const char* Opt = "");

  void printTrkToCaloExtrapolCollection (const char* ModuleLabel, 
					 const char* ProductName = "", 
					 const char* ProcessName = "");

  void printCalTimePeak   (const mu2e::CalTimePeak* TimePeak, const char* Opt = "");

  void printCalTimePeakCollection(const char* ModuleLabel     , 
				  const char* ProductName = "", 
				  const char* ProcessName = "",
				  int         hitOpt      = 0);

//   void printCaloCrystalHit(const CaloCrystalHit* Hit, const char* Opt = "");
//   void printCaloHit       (const CaloHit*        Hit, const char* Opt = "");
  void printGenParticle   (const mu2e::GenParticle*    P  , const char* Opt = "");

  void printGenParticleCollections();

  void printSimParticle   (const mu2e::SimParticle*    P  , const char* Opt = "");

  void printSimParticleCollection(const char* ModuleLabel     , 
				  const char* ProductName = "", 
				  const char* ProcessName = "");

  void printStrawHit      (const mu2e::StrawHit*    Hit, 
			   const mu2e::StepPointMC* Step,
			   const char*              Opt   = "", 
			   int                      INit  = -1,
			   int                      Flags = -1);
  
  void printHelixHit      (const mu2e::ComboHit*    HelHit,
			   const mu2e::StrawHit*    Hit, 
			   const mu2e::StepPointMC* Step,
			   const char*              Opt   = "", 
			   int                      INit  = -1,
			   int                      Flags = -1);
  
  void printStrawHitCollection (const char* ModuleLabel, 
				const char* ProductName = "", 
				const char* ProcessName = "",
				double TMin = -1.e6,
				double TMax =  1.e6);

  void printStrawHitMCTruth      (const mu2e::StrawHitMCTruth* Hit, const char* Opt = "");

  void printStrawHitMCTruthCollection (const char* ModuleLabel, 
				       const char* ProductName = "", 
				       const char* ProcessName = "");

  void printStepPointMC(const mu2e::StepPointMC* Step, const char* Opt = "");

  void printStepPointMCCollection (const char* ModuleLabel     , 
				   const char* ProductName = "", 
				   const char* ProcessName = "");

  void printStrawHitPosition(const mu2e::ComboHit* Pos, const char* Opt = "");

  void printStrawHitPositionCollection (const char* ModuleLabel     , 
					const char* ProductName = "", 
					const char* ProcessName = "");

  void printTrackClusterMatch(const mu2e::TrackClusterMatch* TcMatch, const char* Option);

  void printTrackClusterMatchCollection (const char* ModuleLabel     , 
					 const char* ProductName = "", 
					 const char* ProcessName = "");
			      
  void printStepPointMCVectorCollection(const char* ModuleLabel     ,
					const char* ProductName = "", 
					const char* ProcessName = "");
			      
					// refit track dropping hits away > NSig sigma (0.1)
  void  refitTrack(void* Trk, double NSig);

  void  Test_000(const KalRep* Krep, mu2e::TrkStrawHit* Hit);

  ClassDef(TAnaDump,0)
};


#endif
