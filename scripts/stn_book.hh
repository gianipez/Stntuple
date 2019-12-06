//-----------------------------------------------------------------------------
// a 'dataset' here is a histogram file plus data fields for plotting attributes
//-----------------------------------------------------------------------------
#ifndef __Stntuple_scripts_stn_book_hh__
#define __Stntuple_scripts_stn_book_hh__

#include "Stntuple/scripts/stn_dataset.hh"
#include "Stntuple/scripts/hist_file.hh"
//-----------------------------------------------------------------------------
// book is a list of datasets, list owns its pointers
//-----------------------------------------------------------------------------
class stn_book  {
public:
  TString                fName;
  TString                fHistDir;
  vector<stn_dataset_t*> fListOfDatasets;
  vector<hist_file_t*>   fListOfHistFiles;

  stn_book(const char* Name = "", const char* HistDir = "") {
    fName    = Name;
    fHistDir = HistDir;
  }
  
  ~stn_book() {
    int nd = fListOfDatasets.size();
    for (int i = 0; i<nd; i++) {
      delete (fListOfDatasets)[i];
    }

    int nf = fListOfHistFiles.size();
    for (int i = 0; i<nf; i++) {
      delete (fListOfHistFiles)[i];
    }
  }

  const char* Name   () { return fName.Data(); }
  const char* HistDir() { return fHistDir.Data(); }

  int AddDataset(stn_dataset_t* Ds) {
    int rc = fListOfDatasets.size();
    fListOfDatasets.push_back(Ds);
    return rc;
  }

  stn_dataset_t* NewDataset(const char* DsID, const char* Name,
			    long int NEvents, long int NGenEvents) {
    
    stn_dataset_t* d = new stn_dataset_t(DsID);
    TString name(Name);
    if (name == "") d->fName = DsID;
    else            d->fName = Name;

    d->fNEvents    = NEvents;
    d->fNGenEvents = NGenEvents;
    
    fListOfDatasets.push_back(d);
    return d;
  }

  stn_dataset_t* Dataset(int I) { return fListOfDatasets.at(I); }

  stn_dataset_t* FindDataset(const char* DsID) {
    stn_dataset_t* ds(nullptr);

    for(int i=0; i<fListOfDatasets.size(); i++) {
      stn_dataset_t* d = fListOfDatasets[i];
      if (d->fID == DsID) {
	ds = d;
	break;
      }
    }
    if (ds == nullptr) {
      printf(" get_dataset ERROR: cant find dsid=%s in book=%s, return NULL\n",
	     DsID,fName.Data());
    }
    return ds;
  }

  
  hist_file_t* NewHistFile(const char* DsID, const char* ProductionJob, const char* JobName) {

    hist_file_t* hf(nullptr);
    
    stn_dataset_t* ds = FindDataset(DsID);

    if (ds) {
      hf = new hist_file_t(Form("%s/%s.%s.%s.hist",fHistDir.Data(),DsID,ProductionJob,JobName),
			   ds,ProductionJob,JobName);
      fListOfHistFiles.push_back(hf);
    }
    return hf;
  }

};

#endif
