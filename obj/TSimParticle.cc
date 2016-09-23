//-----------------------------------------------------------------------------
// TSimParticle: STNTUPLE description of the generator-level MC particle
// It inherits from ROOT's TParticle and adds more functions to it
// particle number in the original list is saved in TObject::fUniqueID 
// Nov 25 2000 P.Murat (CDF/FNAL)
//----------------------------------------------------------------------------- 
#include <limits.h>	// UINT_MAX
#include "TDatabasePDG.h"
#include "Stntuple/obj/TSimParticle.hh"

ClassImp(TSimParticle)

//-----------------------------------------------------------------------------
void TSimParticle::ReadV1(TBuffer &R__b) {

  struct TSimParticleV01_t {
    int             fParentID;
    int             fPdgCode;
    int             fCreationCode;
    int             fStartVolumeIndex;
    int             fTerminationCode;
    int             fEndVolumeIndex;
    int             fNStrawHits;
    
    float           fMomTargetEnd;
    float           fMomTrackerFront;		// entrance to ST

    TLorentzVector  fStartPos;
    TLorentzVector  fStartMom;
  };

  TSimParticleV01_t data;

  int nwi = ((int*  ) &data.fMomTargetEnd) - &data.fParentID;
  int nwf = ((float*) &data.fStartPos    ) - &data.fMomTargetEnd ;

  TObject::Streamer(R__b);

  R__b.ReadFastArray(&data.fParentID   ,nwi);
  R__b.ReadFastArray(&data.fMomTargetEnd,nwf);

  fParentID         = data.fParentID;
  fPdgCode          = data.fPdgCode;
  fCreationCode     = data.fCreationCode;
  fStartVolumeIndex = data.fStartVolumeIndex;
  fTerminationCode  = data.fTerminationCode;
  fEndVolumeIndex   = data.fEndVolumeIndex;
  fNStrawHits       = data.fNStrawHits;

  fGenpID           = -1;         // ** added in V2 **

  fStartPos.Streamer(R__b);
  fStartMom.Streamer(R__b);
  
}

//-----------------------------------------------------------------------------
// we don't really need to write out TObject part - so far it is not used
//-----------------------------------------------------------------------------
void TSimParticle::Streamer(TBuffer& R__b) {
  int nwi, nwf;

  nwi = ((int*  ) &fMomTargetEnd) - &fParentID;
  nwf = ((float*) &fStartPos    ) - &fMomTargetEnd ;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); 
    if (R__v == 1) ReadV1(R__b);
    else {

      TObject::Streamer(R__b);
      R__b.ReadFastArray(&fParentID   ,nwi);
      R__b.ReadFastArray(&fMomTargetEnd,nwf);

      fStartPos.Streamer(R__b);
      fStartMom.Streamer(R__b);
    }
  }
  else {
    R__b.WriteVersion(TSimParticle::IsA());
    TObject::Streamer(R__b);

    R__b.WriteFastArray(&fParentID    ,nwi);
    R__b.WriteFastArray(&fMomTargetEnd,nwf);

    fStartPos.Streamer(R__b);
    fStartMom.Streamer(R__b);
  }
}

//_____________________________________________________________________________
TSimParticle::TSimParticle() {
  SetUniqueID(UINT_MAX);
  fGenpID   = -1;
}

//_____________________________________________________________________________
TSimParticle::TSimParticle(Int_t ID, Int_t ParentID, Int_t PdgCode, 
			   int CreationCode, int TerminationCode,
			   int StartVolumeIndex, int EndVolumeIndex,
			   int GenpID,
			   Float_t px, Float_t py, Float_t pz, Float_t e,
			   Float_t vx, Float_t vy, Float_t vz, Float_t t):
  TObject(),
  fStartPos(vx,vy,vz,t),
  fStartMom(px,py,pz,e)
  
{
  SetUniqueID(ID);
  fParentID         = ParentID;
  fPdgCode          = PdgCode;
  fCreationCode     = CreationCode;
  fTerminationCode  = TerminationCode;
  fStartVolumeIndex = StartVolumeIndex;
  fEndVolumeIndex   = EndVolumeIndex;
  fNStrawHits       = 0;
  fGenpID           = GenpID;
  fMomTargetEnd     = -1.;
  fMomTrackerFront  = -1.;
}

//-----------------------------------------------------------------------------
TSimParticle::~TSimParticle() {
}

//_____________________________________________________________________________
int  TSimParticle::Init(Int_t ID, Int_t ParentID, Int_t PdgCode, 
			int CreationCode, int TerminationCode,
			int StartVolumeIndex, int EndVolumeIndex,
			int GenpID,
			Float_t px, Float_t py, Float_t pz, Float_t e,
			Float_t vx, Float_t vy, Float_t vz, Float_t t) 
{
  SetUniqueID(ID);
  fParentID         = ParentID;
  fPdgCode          = PdgCode;
  fCreationCode     = CreationCode;
  fTerminationCode  = TerminationCode;
  fStartVolumeIndex = StartVolumeIndex;
  fEndVolumeIndex   = EndVolumeIndex;
  fNStrawHits       = 0;
  fGenpID           = GenpID;
  fMomTargetEnd     = -1.;
  fMomTrackerFront  = -1.;

  fStartPos.SetXYZT(vx,vy,vz,t);
  fStartMom.SetXYZT(px,py,pz,e);

  return 0;
}

//_____________________________________________________________________________
void TSimParticle::Print(Option_t* Opt) const {

  TString opt = Opt;
  if ((opt == "banner") || (opt == "")) {
				// print banner
    printf("   i name                   PDG  GenpID  isthep  im1  im2  id1  id2      px");
    printf("      py       pz       e        vx       vy        vz       t\n");
  }

  TDatabasePDG* db = TDatabasePDG::Instance();

  TParticlePDG* pdg = db->GetParticle(fPdgCode);

  if ((opt == "data") || (opt == "")) {
    printf("%4i",Number());
    if (pdg) printf(" %-19s",pdg->GetName());
    else          printf(" %-19s","*** unknown ***");
    printf("%7i"  ,fPdgCode);
    printf("%8i"  ,fGenpID);
    printf("%9.3f",fStartMom.Px());
    printf("%9.3f",fStartMom.Py());
    printf("%9.3f",fStartMom.Pz());
    printf("%9.3f",fStartMom.Energy());
    printf("%9.3f",fStartPos.X());
    printf("%9.3f",fStartPos.Y());
    printf("%9.3f",fStartPos.X());
    printf("%9.3f",fStartPos.T());
    printf("\n");
  }
}


