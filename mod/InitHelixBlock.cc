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

#include "RecoDataProducts/inc/AlgorithmIDCollection.hh"
#include "RecoDataProducts/inc/CaloCluster.hh"
#include "RecoDataProducts/inc/StrawHitPositionCollection.hh"

#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "MCDataProducts/inc/StrawDigiMC.hh"
#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StepPointMC.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawDigiMCCollection.hh"
#include "TrkDiag/inc/TrkMCTools.hh"

#include "Mu2eUtilities/inc/LsqSums4.hh"
#include "Math/VectorUtil.h"

using namespace ROOT::Math::VectorUtil;


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
  char                 makeSD_module_label[100];

  TStnHelixBlock*         cb = (TStnHelixBlock*) Block;
  TStnHelix*              helix(0);


  cb->Clear();
  cb->GetModuleLabel("mu2e::StrawDigiMCCollection",makeSD_module_label);

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

  const mu2e::StrawDigiMCCollection* mcdigis(0);
  art::Handle<mu2e::StrawDigiMCCollection> mcdH;
  Evt->getByLabel(makeSD_module_label/*"makeSD"*/, mcdH);
  mcdigis = mcdH.product();
  
  const mu2e::HelixSeed     *tmpHel(0);
  int                        nhelices(0);
  const mu2e::RobustHelix   *robustHel(0);
  const mu2e::CaloCluster   *cluster(0);
 
  const mu2e::AlgorithmID*   alg_id(0);
  int                        mask(-1);

  //std::vector<int>     hits_simp_id, hits_simp_index, hits_simp_z;

  //  int xxx(0);

  if (list_of_helices) nhelices = list_of_helices->size();
  
  TParticlePDG* part(0);
  TDatabasePDG* pdg_db = TDatabasePDG::Instance();
  static XYZVec zaxis(0.0,0.0,1.0); // unit in z direction

  for (int i=0; i<nhelices; i++) {
    // //clear vector content
    // hits_simp_id.clear();
    // hits_simp_index.clear();
    // hits_simp_z.clear();
    std::vector<int>     hits_simp_id, hits_simp_index, hits_simp_z;

    
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
 
    LsqSums4 srphi;
    // static const CLHEP::Hep3Vector zdir(0.0,0.0,1.0);
    static const XYZVec zdir(0.0,0.0,1.0);
    float          rpullScaleF(0.);
    //    float          minrerr(0.);
    float          cradres(0.);
    float          cperpres(0.);

    fhicl::ParameterSet const& pset = helix_handle.provenance()->parameterSet();
    std::string    module     = pset.get<std::string>("module_type");
  
    if ( module == "RobustHelixFinder"){
      rpullScaleF  = pset.get<float>("RPullScaleF", 1.0);//0.895);
      //      minrerr      = pset.get<float>("MinRadiusErr", 20.0);
      cradres      = pset.get<float>("CenterRadialResolution", 20.0);
      cperpres     = pset.get<float>("CenterPerpResolution"  , 20.0);
    }

    if ( module == "CalHelixFinder" ){
      fhicl::ParameterSet const& psetHelFit = pset.get<fhicl::ParameterSet>("HelixFinderAlg", fhicl::ParameterSet());
      int addST  = psetHelFit.get<int>("targetconsistent");//0.895);

      if (addST == 1) { //add the stopping target center as in CalHeliFinderAlg.cc
	sxy.addPoint(0., 0., 1./900.);
      }
    }

    int      nStrawHits(0);
    float    chi2TprZPhi(0.), chi2TprXY(0.);

    for (int j=0; j<nhits; ++j){      //this loop is made over the ComboHits
      hit       = &hits->at(j);
      //get the MC truth info
      //      int  hitIndex                = tmpHel->indices().at(j);//hit->index();
      if (hit->_flag.hasAnyProperty(mu2e::StrawHitFlag::outlier))         continue;

      std::vector<StrawDigiIndex> shids;
      tmpHel->hits().fillStrawDigiIndices(*(Evt),j,shids);

      for (size_t k=0; k<shids.size(); ++k) {
	mu2e::StrawDigiMC const&     mcdigi = mcdigis->at(shids[k]);
	art::Ptr<mu2e::StepPointMC>  spmcp;
	mu2e::TrkMCTools::stepPoint(spmcp,mcdigi);
	const mu2e::StepPointMC* Step = spmcp.get();
      
	if (Step) {
	  art::Ptr<mu2e::SimParticle> const& simptr = Step->simParticle(); 
	  int sim_id        = simptr->id().asInt();
	  float   dz        = Step->position().z();// - trackerZ0;
	  hits_simp_id.push_back   (sim_id); 
	  hits_simp_index.push_back(shids[k]);
	  hits_simp_z.push_back(dz);
	  break;
	}
      }

      pos       = hit->pos();
      wdir      = hit->wdir();
      sdir      = zdir.Cross(wdir);
      phi       = hit->helixPhi();
      //      helix_phi = robustHel->circleAzimuth(pos.z());//helix->fFZ0 + pos.z()/helix->fLambda;
      double    weightXY   = hit->_xyWeight;
      if (weightXY<1e-6) weightXY = evalWeight(hit, sdir, helix_center, radius, 1, pset);

      sxy.addPoint(pos.x(), pos.y(), weightXY);

      double weight    = hit->_zphiWeight;
      if (weight < 1e-6) weight = evalWeight(hit, sdir, helix_center, radius, 0, pset);

      if ( module == "CalHelixFinder"){
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
      }
      srphi.addPoint(pos.z(), phi, weight);

      //increase the counter of the StrawHits
      nStrawHits += hit->nStrawHits();


      //For RobustHelixFinder: calulate sum of the residuals 
      //RobustHelix: X-Y part
      XYZVec cvec  = PerpVector(hit->pos() - robustHel->center(),Geom::ZDir()); // direction from the circle center to the hit
      XYZVec cdir  = cvec.Unit(); // direction from the circle center to the hit
      float  rwdot = wdir.Dot(cdir); // compare directions of radius and wire
      float  dr    = sqrtf(cvec.mag2()) - robustHel->radius();

      float rwdot2 = rwdot*rwdot;
      // compute radial difference and pull
      float werr   = hit->posRes(mu2e::StrawHitPosition::wire);
      float terr   = hit->posRes(mu2e::StrawHitPosition::trans);
      // the resolution is dominated the resolution along the wire
      //      float rres   = std::max(sqrtf(werr*werr*rwdot2 + terr*terr*(1.0-rwdot2)),minrerr);
      float rres   = sqrtf(werr*werr*rwdot2 + terr*terr*(1.0-rwdot2));
      float rpull  = fabs(dr/rres)*rpullScaleF;

      chi2TprXY += rpull*rpull;

      //RobustHelix: Z-Phi 
      XYZVec wtdir = zaxis.Cross(wdir);   // transverse direction to the wire
      // XYZVec cvec = PerpVector(hit->pos() - helix.center(),Geom::ZDir()); // direction from the circle center to the hit
      // XYZVec cdir = cvec.Unit();          // direction from the circle center to the hit
      XYZVec cperp = zaxis.Cross(cdir);   // direction perp to the radius

      XYZVec hpos = hit->pos(); // this sets the z position to the hit z
      robustHel->position(hpos);                // this computes the helix expectation at that z
      XYZVec dh = hit->pos() - hpos;   // this is the vector between them
      float dtrans = fabs(dh.Dot(wtdir)); // transverse projection
      float dwire = fabs(dh.Dot(wdir));   // projection along wire direction

      // compute the total resolution including hit and helix parameters first along the wire
      float wres2 = std::pow(hit->posRes(mu2e::StrawHitPosition::wire),(int)2) +
	std::pow(cradres*cdir.Dot(wdir),(int)2) +
	std::pow(cperpres*cperp.Dot(wdir),(int)2);
      // transverse to the wires
      float wtres2 = std::pow(hit->posRes(mu2e::StrawHitPosition::trans),(int)2) +
	std::pow(cradres*cdir.Dot(wtdir),(int)2) +
	std::pow(cperpres*cperp.Dot(wtdir),(int)2);

      chi2TprZPhi += dwire*dwire/wres2 + dtrans*dtrans/wtres2;

    } 

    //find the simparticle that created the majority of the hits
    int     max(0), mostvalueindex(-1), mostvalue(-1), id_max(0);
    float   dz_most(1e4);
    if (hits_simp_id.size()>0) mostvalue = hits_simp_id[0];
    
    for (int  k=0; k<(int)hits_simp_id.size(); ++k){
      int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), hits_simp_id[k]);
      if ( (co>0) && (co>=max)) {
	float  dz      = std::fabs(hits_simp_z[k]);
	if (dz < dz_most){
	  dz_most        = dz;
	  max            = co;
	  id_max         = k;
	  mostvalue      = hits_simp_id[k];
	  mostvalueindex = hits_simp_index[k];
	}
      }
    }

    //    helix->fSimpId1     = mostvalue;
    helix->fSimpId1Hits = max;
    helix->fSimpId2Hits = -1;
    helix->fSimpPDGM1   = -1;
    helix->fSimpPDGM2   = -1;
    
    if (hits_simp_id.size()>0) {
      if ( (mostvalueindex != hits_simp_index[id_max]) ){
	printf(">>> ERROR: event %i %s helix %i no MC found. MostValueindex = %i hits_simp_index[id_max] =%i \n", 
	       Evt->event(), module.c_str(), i, mostvalueindex, hits_simp_index[id_max]);
      }
    }
    
    if ( (mostvalueindex<0) || (mostvalueindex >= (int)mcdigis->size()))        {
      printf(">>> ERROR: event %i %s helix %i no MC found. MostValueindex = %i hits_simp_index[id_max] = %i mcdigis_size =%li \n", 
	     Evt->event(), module.c_str(), i, mostvalueindex, hits_simp_index[id_max], mcdigis->size());
      continue;
    }
    
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
    if (max != int(hits_simp_id.size()) ){  //nhits){
      int   secondmostvalueindex(-1);
      max     = 0;//reset max
      dz_most = 1e4;

      for (int k=0; k<(int)hits_simp_id.size(); ++k){
	int value = hits_simp_id[k];
	int co = (int)std::count(hits_simp_id.begin(), hits_simp_id.end(), value);
	if ( (co>0) && (co>=max) && (value != mostvalue)) {
	  float  dz      = std::fabs(hits_simp_z[k]);
	  if (dz < dz_most){
	    max                  = co;
	    dz_most              = dz;
	    secondmostvalueindex = hits_simp_index[k];
	  }
	}
      }
      //      helix->fSimpId2     = secondmostvalue;
      helix->fSimpId2Hits = max;

      if (secondmostvalueindex >=0){

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
    }
    
    helix->fNComboHits = tmpHel->hits().size();
    helix->fNHits      = nStrawHits;

    if ( module == "CalHelixFinder"){
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

      double     weight_cl_phiz = 784.;//10.;//1./(err_cl*err_cl);
      srphi.addPoint(pos.z(), phi, weight_cl_phiz);
    }
    
    //Gainipez, 20180911
    //commented for making a test
    if ( module == "CalHelixFinder"){
      helix->fChi2XYNDof   = sxy.chi2DofCircle();
      helix->fChi2PhiZNDof = srphi.chi2DofLine();
    } else {
      helix->fChi2XYNDof   = chi2TprXY/sxy.qn();
      helix->fChi2PhiZNDof = chi2TprZPhi/srphi.qn();    
    }
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

