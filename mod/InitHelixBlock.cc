//-----------------------------------------------------------------------------
//  Apr 2016 G. Pezzullo: initialization of the MU2E STNTUPLE Helix block
//
//-----------------------------------------------------------------------------
#include <cstdio>
#include "TROOT.h"
#include "TFolder.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"


#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnEvent.hh"

#include "Stntuple/obj/TStnTimeCluster.hh"
#include "Stntuple/obj/TStnTimeClusterBlock.hh"

#include "Stntuple/obj/TStnHelix.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"

#include "Stntuple/obj/TStnTrackSeed.hh"
#include "Stntuple/obj/TStnTrackSeedBlock.hh"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"

#include "TTrackerGeom/inc/TTracker.hh"
#include "CalorimeterGeom/inc/Calorimeter.hh"

#include "RecoDataProducts/inc/XYZVec.hh"
#include "RecoDataProducts/inc/TimeCluster.hh"
#include "RecoDataProducts/inc/HelixSeed.hh"
#include "RecoDataProducts/inc/HelixHit.hh"
#include "RecoDataProducts/inc/KalSeed.hh"

#include "RecoDataProducts/inc/CaloCluster.hh"

#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "MCDataProducts/inc/StrawDigiMC.hh"
#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawDigiMCCollection.hh"
#include "TrkDiag/inc/TrkMCTools.hh"

#include "CalPatRec/inc/AlgorithmIDCollection.hh"
#include "CalPatRec/inc/LsqSums4.hh"



namespace {
  double evalWeight(const mu2e::HelixHit*  Hit   ,
		    XYZVec& StrawDir ,
		    XYZVec& HelCenter, 
		    double             Radius   ,
		    int                WeightMode,
		    fhicl::ParameterSet const& Pset) {//WeightMode = 1 is for XY chi2 , WeightMode = 0 is for Phi-z chi2
  
    //    double    rs(2.5);   // straw radius, mm
    double    transErr = 5./sqrt(12.);
    if (Hit->nStrawHits() > 1) transErr *= 1.5;
    double    transErr2 = transErr*transErr;
    // double    ew(30.0);  // assumed resolution along the wire, mm

    double x  = Hit->pos().x();
    double y  = Hit->pos().y();
    double dx = x-HelCenter.x();
    double dy = y-HelCenter.y();
  
    double costh  = (dx*StrawDir.x()+dy*StrawDir.y())/sqrt(dx*dx+dy*dy);
    double sinth2 = 1-costh*costh;
  
    double wt(0), wtXY(1), wtPhiZ(1);

    //  fhicl::ParameterSet const& pset = helix_handle.provenance()->parameterSet();
    std::string                module     = Pset.get<std::string>("module_type");
  
    if ( module == "CalHelixFinder"){
      fhicl::ParameterSet const& psetHelFit = Pset.get<fhicl::ParameterSet>("HelixFinderAlg", fhicl::ParameterSet());
      wtXY   = psetHelFit.get<double>("weightXY");
      wtPhiZ = psetHelFit.get<double>("weightZPhi");
    }
    //scale the weight for having chi2/ndof distribution peaking at 1
    if ( WeightMode == 1){//XY-Fit
      double e2     = Hit->wireErr2()*sinth2+transErr2*costh*costh; //ew*ew*sinth2+rs*rs*costh*costh;
      wt  = 1./e2;
      wt *= wtXY;
    } else if (WeightMode ==0 ){//Phi-Z Fit
      double e2     = Hit->wireErr2()*costh*costh+transErr2*sinth2; //ew*ew*costh*costh+rs*rs*sinth2;
      wt     = Radius*Radius/e2;
      wt    *= wtPhiZ;
    }
  
    return wt;
  }
}

//-----------------------------------------------------------------------------
// assume that the collection name is set, so we could grab it from the event
//-----------------------------------------------------------------------------
int  StntupleInitMu2eHelixBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode) {

  const mu2e::HelixSeedCollection*               list_of_helices(0);
  mu2e::AlgorithmIDCollection*             list_of_algs   (0);

  char                 helix_module_label[100], helix_description[100]; 
  char                 algs_module_label [100], algs_description [100];

  TStnHelixBlock*         cb = (TStnHelixBlock*) Block;
  TStnHelix*              helix;


  cb->Clear();

  cb->GetModuleLabel("mu2e::AlgorithmIDCollection",algs_module_label);
  cb->GetDescription("mu2e::AlgorithmIDCollection",algs_description );

  cb->GetModuleLabel("mu2e::HelixSeedCollection", helix_module_label);
  cb->GetDescription("mu2e::HelixSeedCollection", helix_description );


  art::Handle<mu2e::AlgorithmIDCollection> algsHandle;
  strcpy(algs_module_label, helix_module_label);
  strcpy(algs_description , helix_description);
  if (algs_module_label[0] != 0) {
    if (algs_description[0] == 0) Evt->getByLabel(algs_module_label,algsHandle);
    else                          Evt->getByLabel(algs_module_label,algs_description, algsHandle);
    if (algsHandle.isValid()) list_of_algs = (mu2e::AlgorithmIDCollection*) algsHandle.product();
  }


  art::Handle<mu2e::HelixSeedCollection>               helix_handle;
  if (helix_module_label[0] != 0){
    Evt->getByLabel(helix_module_label, helix_handle);
    if (helix_handle.isValid()) list_of_helices = helix_handle.product();//(mu2e::HelixSeedCollection*) &(*helix_handle);
  }

  // art::Handle<mu2e::StrawHitCollection>         shcHandle;
  // const mu2e::StrawHitCollection*               shcol;

  // const char* ProductName = "";
  // const char* ProcessName = "";

  // //now get the strawhitcollection
  // if (ProductName[0] != 0) {
  //   art::Selector  selector(art::ProductInstanceNameSelector(ProductName) &&
  // 			    art::ProcessNameSelector(ProcessName)         && 
  // 			    art::ModuleLabelSelector("makeSH")           );
  //   fEvent->get(selector, shcHandle);
  // }
  // else {
  //   art::Selector  selector(art::ProcessNameSelector(ProcessName)         && 
  // 			    art::ModuleLabelSelector("makeSH")           );
  //   fEvent->get(selector, shcHandle);
  // }
    
  // shcol = shcHandle.product();

  // art::Handle<mu2e::PtrStepPointMCVectorCollection> mcptrHandleStraw;
  // Evt->getByLabel("makeSD","",mcptrHandleStraw);
  // mu2e::PtrStepPointMCVectorCollection const* hits_mcptrStraw = mcptrHandleStraw.product();

  const mu2e::StrawDigiMCCollection* mcdigis(0);
  art::Handle<mu2e::StrawDigiMCCollection> mcdH;
  Evt->getByLabel("makeSD", mcdH);
  mcdigis = mcdH.product();
  
  const mu2e::HelixSeed     *tmpHel(0);
  int                        nhelices(0);
  const mu2e::RobustHelix   *robustHel(0);
  const mu2e::CaloCluster   *cluster(0);
 
  const mu2e::AlgorithmID*   alg_id(0);
  int                        mask(-1);

  //  int xxx(0);

  if (list_of_helices) nhelices = list_of_helices->size();
  
  std::vector<int>     hits_simp_id, hits_simp_index;

  TParticlePDG* part;
  TDatabasePDG* pdg_db = TDatabasePDG::Instance();
 
  for (int i=0; i<nhelices; i++) {
    //clear vector content
    hits_simp_id.clear();
    hits_simp_index.clear();

    helix                  = cb->NewHelix();
    tmpHel                 = &list_of_helices->at(i);
    cluster                = tmpHel->caloCluster().get();
    robustHel              = &tmpHel->helix();
    if (cluster != 0){
      mu2e::GeomHandle<mu2e::Calorimeter> ch;
      const mu2e::Calorimeter* _calorimeter = ch.get();      
      
      helix->fClusterTime    = cluster->time();
      helix->fClusterEnergy  = cluster->energyDep();
      CLHEP::Hep3Vector         gpos = _calorimeter->geomUtil().diskToMu2e(cluster->diskId(),cluster->cog3Vector());
      CLHEP::Hep3Vector         tpos = _calorimeter->geomUtil().mu2eToTracker(gpos);
      helix->fClusterX       = tpos.x();
      helix->fClusterY       = tpos.y();
      helix->fClusterZ       = tpos.z();
    }else {
      helix->fClusterTime    = 0; 
      helix->fClusterEnergy  = 0; 
      helix->fClusterX       = 0; 
      helix->fClusterY       = 0; 
      helix->fClusterZ       = 0; 
    }
    
    helix->fHelix        = tmpHel;
    helix->fT0           = tmpHel->t0()._t0;
    helix->fT0Err        = tmpHel->t0()._t0err;     
    helix->fRCent        = robustHel->rcent  ();
    helix->fFCent        = robustHel->fcent  ();     
    helix->fRadius       = robustHel->radius ();
    helix->fLambda       = robustHel->lambda ();     
    helix->fFZ0          = robustHel->fz0    ();
    helix->fD0           = robustHel->rcent  () - robustHel->radius ();
    
    //2017-02-23: gianipez - calculate the chi2
    const mu2e::HelixHitCollection* hits      = &tmpHel->hits();
    const mu2e::ComboHit*           hit(0);
    // CLHEP::Hep3Vector         pos(0), /*helix_pos(0),*/ wdir(0), sdir(0), helix_center(0);
    XYZVec                    pos(0,0,0), /*helix_pos(0),*/ wdir(0,0,0), sdir(0,0,0), helix_center(0,0,0);
    double                    phi(0), helix_phi(0);
    double                    radius    = robustHel->radius();
    helix_center = robustHel->center();

    //    double                    chi2xy(0), chi2phiz(0);
    int                       nhits(hits->size());

    LsqSums4 sxy;
    //add the stopping target center as in CalHeliFinderAlg.cc
    sxy.addPoint(0., 0., 1./900.);

    LsqSums4 srphi;
    // static const CLHEP::Hep3Vector zdir(0.0,0.0,1.0);
    static const XYZVec zdir(0.0,0.0,1.0);

    fhicl::ParameterSet const& pset = helix_handle.provenance()->parameterSet();

    int      nStrawHits(0);
    for (int j=0; j<nhits; ++j){      //this loop is made over the ComboHits
      hit       = &hits->at(j);
      //get the MC truth info
      //      int  hitIndex                = tmpHel->indices().at(j);//hit->index();
      
      std::vector<StrawDigiIndex> shids;
      tmpHel->hits().fillStrawDigiIndices(*(Evt),j,shids);

      mu2e::StrawDigiMC const&     mcdigi = mcdigis->at(shids[0]);
      art::Ptr<mu2e::StepPointMC>  spmcp;
      mu2e::TrkMCTools::stepPoint(spmcp,mcdigi);
      const mu2e::StepPointMC* Step = spmcp.get();
      
      if (Step) {
	art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
	int sim_id        = simptr->id().asInt();

	hits_simp_id.push_back   (sim_id); 
	hits_simp_index.push_back(shids[0]);
      }

      pos       = hit->pos();
      wdir      = hit->wdir();
      sdir      = zdir.Cross(wdir);
      phi       = hit->phi();
      helix_phi = helix->fFZ0 + pos.z()/helix->fLambda;
      double    weightXY   = evalWeight(hit, sdir, helix_center, radius, 1, pset);

      sxy.addPoint(pos.x(), pos.y(), weightXY);

      double    dPhi     = helix_phi - phi- M_PI/2.;
      while (dPhi > M_PI){
	phi    += 2*M_PI;
        dPhi   = helix_phi - phi;
      }
      while (dPhi < -M_PI){
	phi   -= 2*M_PI; 
	dPhi  = helix_phi - phi;
      }
      double weight    = evalWeight(hit, sdir, helix_center, radius, 0, pset);
      srphi.addPoint(pos.z(), phi, weight);

      //increase the counter of the StrawHits
      nStrawHits += hit->nStrawHits();
    } 

    //find the simparticle that created the majority of the hits
    int     max(0), mostvalueindex(-1), mostvalue= hits_simp_id[0];
    for (int k=0; k<nhits; ++k){
      int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), hits_simp_id[k]);
      if ( co>max) {
	max            = co;
	mostvalue      = hits_simp_id[k];
	mostvalueindex = hits_simp_index[k];
      }
    }

    //    helix->fSimpId1     = mostvalue;
    helix->fSimpId1Hits = max;
    //set defaults
    // helix->fSimpId2     = -1;
    helix->fSimpId2Hits = -1;

    mu2e::StrawDigiMC const&     mcdigi = mcdigis->at(mostvalueindex);
    art::Ptr<mu2e::StepPointMC>  spmcp;
    mu2e::TrkMCTools::stepPoint(spmcp,mcdigi);
    const mu2e::StepPointMC* Step = spmcp.get();
    // mu2e::PtrStepPointMCVector const& mcptr(hits_mcptrStraw->at(mostvalueindex) );
    // const mu2e::StepPointMC* Step = mcptr[0].get();
    const mu2e::SimParticle * sim (0);

    if (Step) {
      art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
      helix->fSimpPDG1    = simptr->pdgId();
      art::Ptr<mu2e::SimParticle> mother = simptr;
      part   = pdg_db->GetParticle(helix->fSimpPDG1);

      while(mother->hasParent()) mother = mother->parent();
      sim = mother.operator ->();

      helix->fSimpPDGM1   = sim->pdgId();
      
      double   px = Step->momentum().x();//simptr->startMomentum().x();
      double   py = Step->momentum().y();//simptr->startMomentum().y();
      double   pz = Step->momentum().z();//simptr->startMomentum().z();
      double   mass(-1.);//  = part->Mass();
      double   energy(-1.);// = sqrt(px*px+py*py+pz*pz+mass*mass);
      if (part) {
	mass   = part->Mass();
	energy = sqrt(px*px+py*py+pz*pz+mass*mass);
      }
      helix->fMom1.SetPxPyPzE(px,py,pz,energy);

      const CLHEP::Hep3Vector* sp = &simptr->startPosition();
      helix->fOrigin1.SetXYZT(sp->x(),sp->y(),sp->z(),simptr->startGlobalTime());
  
      // helix->fSimp1P      = Step->momentum().mag();
      // helix->fSimp1Pt     = sqrt(pow(Step->momentum().x(),2.) + pow(Step->momentum().y(),2.));
    }
    
    //look for the second most frequent hit
    if (max != nhits){
      int   secondmostvalueindex(-1);
      max = 0;//reset max

      for (int k=0; k<nhits; ++k){
	int value = hits_simp_id[k];
	int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), value);
	if ( (co>max) && (value != mostvalue)) {
	  max                  = co;
	  // secondmostvalue      = value;
	  secondmostvalueindex = hits_simp_index[k];
	}
      }
      //      helix->fSimpId2     = secondmostvalue;
      helix->fSimpId2Hits = max;


      mu2e::StrawDigiMC const&     mcdigi = mcdigis->at(secondmostvalueindex);
      art::Ptr<mu2e::StepPointMC>  spmcp;
      mu2e::TrkMCTools::stepPoint(spmcp,mcdigi);
      const mu2e::StepPointMC* Step = spmcp.get();
      const mu2e::SimParticle * sim (0);

      if (Step) {
	art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
	helix->fSimpPDG2    = simptr->pdgId();
	art::Ptr<mu2e::SimParticle> mother = simptr;
	part   = pdg_db->GetParticle(helix->fSimpPDG2);

	while(mother->hasParent()) mother = mother->parent();
	sim = mother.operator ->();

	helix->fSimpPDGM2   = sim->pdgId();
      
	double   px = simptr->startMomentum().x();
	double   py = simptr->startMomentum().y();
	double   pz = simptr->startMomentum().z();
	double   mass(-1.);//  = part->Mass();
	double   energy(-1.);// = sqrt(px*px+py*py+pz*pz+mass*mass);
	if (part) {
	  mass   = part->Mass();
	  energy = sqrt(px*px+py*py+pz*pz+mass*mass);
	}
	helix->fMom2.SetPxPyPzE(px,py,pz,energy);

	const CLHEP::Hep3Vector* sp = &simptr->startPosition();
	helix->fOrigin2.SetXYZT(sp->x(),sp->y(),sp->z(),simptr->startGlobalTime());
  
	// helix->fSimp2P      = Step->momentum().mag();
	// helix->fSimp2Pt     = sqrt(pow(Step->momentum().x(),2.) + pow(Step->momentum().y(),2.));
      }      
      
    }
    
    helix->fNComboHits = tmpHel->hits().size();
    helix->fNHits      = nStrawHits;

    if (cluster != 0){
      double     weight_cl_xy = 1./100.;//FIX ME!
      pos       = XYZVec(helix->fClusterX, helix->fClusterY, helix->fClusterZ);
      sxy.addPoint(pos.x(), pos.y(), weight_cl_xy);
      
      phi       = XYZVec(pos - helix_center).phi();
      phi       = TVector2::Phi_0_2pi(phi);
      helix_phi = helix->fFZ0 + pos.z()/helix->fLambda;
      double     dPhi        = helix_phi - phi;
      while (dPhi > M_PI){
	phi    += 2*M_PI;
        dPhi   = helix_phi - phi;
      }
      while (dPhi < -M_PI){
	phi   -= 2*M_PI; 
	dPhi  = helix_phi - phi;
      }

      double     weight_cl_phiz = 10.;//1./(err_cl*err_cl);
      srphi.addPoint(pos.z(), phi, weight_cl_phiz);
    }
    

    helix->fChi2XYNDof   = sxy.chi2DofCircle();
    helix->fChi2PhiZNDof = srphi.chi2DofLine();
    
    mask = (0x0001 << 16) | 0x0000;

    if (list_of_algs) {
      std::string         module_type = pset.get<std::string>("module_type");
      
      alg_id = &list_of_algs->at(i);
      mask   = alg_id->BestID() | (alg_id->AlgMask() << 16);
    }
    
    helix->fAlgorithmID = mask;
  }

  return 0;
}

//_____________________________________________________________________________
Int_t StntupleInitMu2eHelixBlockLinks(TStnDataBlock* Block, AbsEvent* AnEvent, int Mode) 
{
  // Mu2e version, do nothing

  Int_t  ev_number, rn_number;

  ev_number = AnEvent->event();
  rn_number = AnEvent->run();

  if (! Block->Initialized(ev_number,rn_number)) return -1;

					// do not do initialize links 2nd time

  if (Block->LinksInitialized()) return 0;

  TStnEvent*           ev;
  TStnHelixBlock*      hb;
  TStnHelix*           helix;
  TStnTrackSeedBlock*  tsb;
  TStnTrackSeed*       trkseed;
  TStnTimeClusterBlock*   tpb;
  TStnTimeCluster*        tp;

  const mu2e::HelixSeed*   khelix, *fkhelix;
  const mu2e::KalSeed*     kseed;
  const mu2e::TimeCluster* ktimepeak, *fktimepeak;

  char                     short_helix_block_name[100], timepeak_block_name[100];

  ev     = Block->GetEvent();
  hb     = (TStnHelixBlock*) Block;
  
  hb->GetModuleLabel("mu2e::KalSeedCollection"    , short_helix_block_name);
  hb->GetModuleLabel("mu2e::TimeClusterCollection", timepeak_block_name   );

  tsb    = (TStnTrackSeedBlock*) ev->GetDataBlock(short_helix_block_name);
  tpb    = (TStnTimeClusterBlock* ) ev->GetDataBlock(timepeak_block_name   );

  int    nhelix   = hb ->NHelices();
  int    ntrkseed = tsb->NTrackSeeds();
  int    ntpeak   = tpb->NTimeClusters();

  for (int i=0; i<nhelix; ++i){
    helix  = hb   ->Helix(i);
    khelix = helix->fHelix;
    int      trackseedIndex(-1);
    for (int j=0; j<ntrkseed; ++j){
      trkseed = tsb->TrackSeed(j);
      kseed   = trkseed->fTrackSeed;
      fkhelix = kseed->helix().get();
      if (fkhelix == khelix) {
	trackseedIndex = j;
	break;
      }
    }
    
    if (trackseedIndex < 0) {
      printf(">>> ERROR: CalHelixFinder helix %i -> no TrackSeed associated\n", i);//FIXME!
	  continue;
    }
    helix->SetTrackSeedIndex(trackseedIndex);

    ktimepeak = khelix->timeCluster().get();
    int      timepeakIndex(-1);
    for (int j=0; j<ntpeak;++j){
      tp         = tpb->TimeCluster(j);
      fktimepeak = tp->fTimeCluster;
      if (fktimepeak == ktimepeak){
	timepeakIndex = j;
	break;
      }
    }

    if (timepeakIndex < 0) {
      printf(">>> ERROR: CalHelixFinder helix %i -> no TimeCluster associated\n", i);//FIXME!
	  continue;
    }

    helix->SetTimeClusterIndex(timepeakIndex);

    
  }

//-----------------------------------------------------------------------------
// mark links as initialized
//-----------------------------------------------------------------------------
  hb->fLinksInitialized = 1;


  return 0;
}

