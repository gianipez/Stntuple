#include "Stntuple/alg/RMCSpectra.hh"

//Evaluate the spectrum for a given photon/daughter energy
double RMCSpectra::Weight(double energy) {
  double wt(1.);
  if(internal_ == 0) {
    if(!fSpectrum_) {
      std::cout << "RMCSpectra::" << __func__ << ": Spectrum not initialized!\n";
      return wt;
    }
    wt = fSpectrum_->Eval(energy); //external uses function spectrum
  } else if(internal_ >  0) {
    if(!hSpectrum_) {
      std::cout << "RMCSpectra::" << __func__ << ": Spectrum not initialized!\n";
      return wt;
    }
    wt = hSpectrum_->Interpolate(energy); //internal uses histogram spectrum
  }
  if(verbose_ > 1) std::cout << "RMCSpectra::" << __func__ << ": E = " << energy
			     << " wt = " << wt << std::endl;
  return wt;
}

void RMCSpectra::InitializeSpectrum() {
  if(external_version_ == 0) { //closure approximation
    fSpectrum_ = new TF1("closure", "20.*(1-2*x/[0]+2*(x/[0])^2)*x/[0]*(1-x/[0])^2", 0., kmax_cl_);
    fSpectrum_->FixParameter(0, kmax_cl_);
  }
  if(internal_)
    //convolve external photon spectrum with internal conversion to get e+/e-
    ConvolveInternal();
}

void RMCSpectra::ConvolveInternal() {
  if(verbose_ > 0) Print();
  if(verbose_ > 0) std::cout << "RMCSpectra: Beginning internal spectrum convolution!\n";
  if(internal_version_ == 0 || internal_version_ == 1) { //Kroll+Wada+Joseph spectrum
    kwj_int_.use_proton_values_ = (internal_version_ == 0); //Offline version uses values from capture on proton
    if(verbose_ > 0) kwj_int_.Print();
    //Current Mu2e Offline implementation assumes rho is not a function of photon energy --> remove dependence
    //fixed rho(E_gamma) := rho if version = 0
    InitializeKrollWadaHistogram(internal_version_ == 0);
  } else if(internal_version_ == 2) { //Plestid+Hill spectrum
    if(verbose_ > 0) pl_hill_int_.Print();
    InitializePlestidHillHistogram();
  }
}

void RMCSpectra::InitializePlestidHillHistogram() {
  const int nentries = 1e8; //entries to make convolution
    
  hSpectrum_ = new TH1D("int_conv", "int_conv", 1000, 0., (external_version_ == 0) ? kmax_cl_ : kmax_kn_);
  for(int entry = 0; entry < nentries; ++entry) {
    double photon_energy(0.), positron_energy(1.), prob(1.);
    if(external_version_ == 0) { //closure approximation
      photon_energy = 2.*pl_hill_int_.me_ + (kmax_cl_ - 2.*pl_hill_int_.me_)*rand_->Uniform(); //needs at least 2*electron mass
      prob = fSpectrum_->Eval(photon_energy) * (kmax_cl_ - 2.*pl_hill_int_.me_); //external PDF / (flat PDF)
    }
    if(internal_version_ == 1) {
      positron_energy = pl_hill_int_.me_ + (photon_energy - 2.*pl_hill_int_.me_)*rand_->Uniform(); //needs at least 1 mass, and leave 1 mass
      prob *= pl_hill_int_.Probability(positron_energy, photon_energy) * (photon_energy - 2.*pl_hill_int_.me_); //internal PDF / (flat PDF)
    }
    prob /= nentries;
    hSpectrum_->Fill(positron_energy, prob);	
  }
}

//Get the branching fraction of Kroll+Wada as a function of photon energy
TH1D* RMCSpectra::GetRhoVsEHist(int entries) {
  double emax = (external_version_ == 0) ? kmax_cl_ : kmax_kn_;
  TH1D* h = new TH1D("hRhoVsE", "hRhoVsE", 1000, 0., emax);
  if(verbose_ > 0) std::cout << " initializing Kroll+Wada Rho vs Energy histogram\n";
  const double me = kwj_int_.me_;
  for(int i = 0; i < entries; ++i) {
    const double e_g = 2.*me + (emax-2.*me)*rand_->Uniform();
    const double x   = 2.*me + (e_g-2.*me)*rand_->Uniform();
    const double mxy = sqrt(1.-(2.*me/x)*(2.*me/x));
    const double y   = 2.*mxy*(rand_->Uniform()-0.5);

    double kw  = kwj_int_.Probability(x,y,e_g)*2.*mxy*(e_g-2.*me)*(emax-2.*me);
    if(kw <= 0) {
      if(verbose_ > 1) std::cout << "RMCSpectra::" << __func__ << ": < 0 internal probability! e_g = "
				 << e_g << " x = " << x << " y = " << y << " mxy = " << mxy << std::endl;
      kw = 0.;
    }

    h->Fill(e_g, kw/entries);
  }
  return h;
}

//returns a normalized histogram to get the probability distribution
//of producing an internal electron/positron of energy E given kMax
//using closure approximation + Kroll+Wada
void RMCSpectra::InitializeKrollWadaHistogram(bool removeRho) {
  if(verbose_ > 0) std::cout << " Using removeRho = " << removeRho << std::endl;
  kwj_int_.verbose_ = (verbose_ > 0) ? verbose_ - 1 : 0;
  int entries = 1e8;
  double emax = (external_version_ == 0) ? kmax_cl_ : kmax_kn_;
  hSpectrum_ = new TH1D("int_conv", "int_conv", 1000, 0., emax);
  TH1D* hRhoVsEg = (removeRho) ? GetRhoVsEHist(2e8) : 0;
  TH1D* h = (verbose_ > 1) ? new TH1D("hRhoVsEDebug", "hRhoVsEDebug", 1000, 0., emax) : 0; //for debugging
  TH1D* h2 = (verbose_ > 1) ? new TH1D("hExt", "hExt", 1000, 0., emax) : 0; //for debugging
  if(verbose_ > 0) std::cout << " initializing Kroll+Wada Energy histogram\n";
  const double me = kwj_int_.me_;
  for(int i = 0; i < entries; ++i) {
    //photon energy
    const double e_g = 2.*me + (emax-2.*me)*rand_->Uniform();
    //pair parameters
    const double x   = 2.*me + (e_g-2.*me)*rand_->Uniform();
    const double mxy = sqrt(1.-(2.*me/x)*(2.*me/x));//maximum y
    const double y   = 2.*mxy*(rand_->Uniform()-0.5);
    
    //internal conversion weight
    double kw  = kwj_int_.Probability(x,y,e_g)*2.*mxy*(e_g-2.*me);
    if(kw <= 0) {
      if(verbose_ > 1) std::cout << "RMCSpectra::" << __func__ << ": < 0 internal probability! e_g = "
				 << e_g << " x = " << x << " y = " << y << " mxy = " << mxy << std::endl;
      kw = 0.;
    }

    //real photon weight
    const double cl  = fSpectrum_->Eval(e_g)*(emax-2.*me);
    double prob = cl*kw/entries;

    double energy = 0.;
    if(prob <= 0.) {
      if(verbose_ > 1) std::cout << "RMCSpectra::" << __func__ << ": < 0 probability! e_g = "
				 << e_g << " x = " << x << " y = " << y << " mxy = " << mxy << std::endl;
      prob = 0.;
    }
    else {
      if(removeRho)
	prob /= hRhoVsEg->Interpolate(e_g);
      //get a daughter energy from parameters
      energy = 0.5*(e_g + y*sqrt(e_g*e_g - x*x));
    }
    hSpectrum_->Fill(energy, prob);
    if(h)  h ->Fill(e_g, prob/cl*(emax-2.*me));
    if(h2) h2->Fill(e_g, cl/entries/h2->GetBinWidth(1));
  }
  hSpectrum_->Scale(1./hSpectrum_->Integral()/hSpectrum_->GetBinWidth(1)); //Force unit norm
  if(!h)
    delete hRhoVsEg;
  else {hRhoVsEg->Draw(); h->SetLineColor(kRed); h->Draw("sames"); h2->SetLineColor(kGreen); h2->Draw("sames");fSpectrum_->Draw("sames");}
}
