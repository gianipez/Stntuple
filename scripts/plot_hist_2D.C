///////////////////////////////////////////////////////////////////////////////
// this file is to be included, can't use standalone mode
// it contains two plot_hist_1D functions with different call signatures
//
// overlay two histograms with the same ModuleName/HistName from two files
// defined by Ds1 and Ds2, HistName like "spmc_1/mom"
///////////////////////////////////////////////////////////////////////////////
#ifndef __plot_hist_2D__
#define __plot_hist_2D__

#include "TH1.h"
#include "TH2.h"

#include "Stntuple/val/stntuple_val_functions.hh"
#include "Stntuple/scripts/plot_utilities.hh"
#include "Stntuple/scripts/hist_data.hh"
//-----------------------------------------------------------------------------
// plot one hist
//-----------------------------------------------------------------------------
void plot_hist_2D(hist_data_t* Hd, int Print = 0) {
  
  char figure_name[200];
//-----------------------------------------------------------------------------
// figure out clone histogram names
//-----------------------------------------------------------------------------
  TString h1name(Hd->fName);

  if (Hd->fNewName == "") h1name.ReplaceAll("/","_");
  else                    h1name = Hd->fNewName;

  hist_file_t* hf1 = Hd->fFile;

  if (hf1 == nullptr) {
    printf("plot_hist_2D in trouble, exit\n");
    return ;
  }
  
  TH2F* hpx1 = (TH2F*) gh2(hf1->fName,Hd->fModule,Hd->fName)->Clone(h1name);
  if (Hd->fRebin > 0) hpx1->Rebin(Hd->fRebin);
//-----------------------------------------------------------------------------
// scale, if requested
//-----------------------------------------------------------------------------
  if (Hd->fScale > 0) hpx1->Scale(Hd->fScale);
  
  stn_dataset_t* ds1 = hf1->fDataset;
//-----------------------------------------------------------------------------
// create a canvas
//-----------------------------------------------------------------------------
  TString canvas_name(Hd->fModule);
  canvas_name += "_";
  canvas_name += h1name;

  if (Hd->fCanvasName != "") canvas_name = Hd->fCanvasName;

  int cx = (Hd->fCanvasSizeX < 0) ? 1200 : Hd->fCanvasSizeX;
  int cy = (Hd->fCanvasSizeY < 0) ?  800 : Hd->fCanvasSizeY;

  TCanvas* c = new TCanvas(canvas_name.Data(),canvas_name.Data(),cx,cy);
  c->SetLogy(Hd->fYLogScale);
//-----------------------------------------------------------------------------
// the two histograms correspond to slightly different NPOT
//-----------------------------------------------------------------------------
  hpx1->SetLineColor(kRed-3);
  hpx1->SetLineWidth(2);
  hpx1->SetTitle("");
  if (Hd->fXMin < Hd->fXMax) hpx1->GetXaxis()->SetRangeUser(Hd->fXMin,Hd->fXMax);
  if (Hd->fYMin < Hd->fYMax) hpx1->GetYaxis()->SetRangeUser(Hd->fYMin,Hd->fYMax);
  
  if (Hd->fXAxisTitle != ""   ) hpx1->GetXaxis()->SetTitle(Hd->fXAxisTitle.Data());

  if (Hd->fMarkerStyle > 0) hpx1->SetMarkerStyle(Hd->fMarkerStyle);
  hpx1->Draw();
//-----------------------------------------------------------------------------
// position statbox - need to update the canvas first
//-----------------------------------------------------------------------------
  c->Modified();
  c->Update();
  plot_stat_box(hpx1,Hd->fStatBoxXMin,Hd->fStatBoxYMin,Hd->fStatBoxXMax,Hd->fStatBoxYMax);
//-----------------------------------------------------------------------------
// add legend - not sure what legend is needed
//-----------------------------------------------------------------------------
  float xmin{0.65}, ymin{0.25}, xmax{0.90}, ymax{0.35};
  if (Hd->fLegendXMin > 0) { // redefine the legend position, normalized coords
    xmin = Hd->fLegendXMin;
    ymin = Hd->fLegendYMin;
    xmax = Hd->fLegendXMax;
    ymax = Hd->fLegendYMax;
  }
  
  // TLegend* leg = new TLegend(xmin,ymin,xmax,ymax);
  // leg->AddEntry(hpx1,Hd->fLabel.Data(),"pl");  // "pl"
  // leg->SetBorderSize(0);
  // leg->Draw();
//-----------------------------------------------------------------------------
// write DS names inside the plot
//-----------------------------------------------------------------------------
  TString label;

  if (Hd->fPlotLabel != "") label = Hd->fPlotLabel;
  else                      label = Hd->fName;

  draw_label_ndc(label.Data(),0.15,0.86,0.03,52); // lower left corner

  c->Modified(); c->Update();
//-----------------------------------------------------------------------------
// do we need to add something else? Print = -1 serves that purpose
//-----------------------------------------------------------------------------
  printf(" Hd->fPlotName = %s\n",Hd->fPlotName.Data());
  
  if (Hd->fPlotName == "") {
    TString hn1(Hd->fName);

    TString fol1 = hn1(0,hn1.Index('/'));
    TString nam1 = hn1(hn1.Index('/')+1,hn1.Length());

    printf("fol1=:%s  nam1:%s Hd->fName:%s\n",fol1.Data(),nam1.Data(),Hd->fName.Data());

    if (Hd->fYLogScale == 1) Hd->fPlotName = Form("%s_%s_log",fol1.Data(),nam1.Data());
    else                     Hd->fPlotName = Form("%s_%s_lin",fol1.Data(),nam1.Data());
  }
  
  if (Print == -1) {
    Hd->fCanvas   = c;
    Hd->fHist     = hpx1;
    Hd->fOutputFn = Form("%s/eps/%s.eps",FiguresDir,Hd->fPlotName.Data());
  }
  else { 
//-----------------------------------------------------------------------------
// .png files are written into /png/ subdirectory
//-----------------------------------------------------------------------------
    if (Print == 1) {
      c->Print(Form("%s/eps/%s.eps",FiguresDir,Hd->fPlotName.Data())) ;
    }
  }
  return;
}


//-----------------------------------------------------------------------------
// compare two histograms with different ModuleName/HistName's from two different files
// HistName like "spmc_1/mom"
//-----------------------------------------------------------------------------
void plot_hist_2D(hist_data_t* Hist1,  hist_data_t*  Hist2, int Print = 0) {
  
  char figure_name[200];

//-----------------------------------------------------------------------------
// figure out clone histogram names
//-----------------------------------------------------------------------------
  TString h1name(Hist1->fName), h2name(Hist2->fName);

  if (Hist1->fNewName == "") h1name.ReplaceAll("/","_");
  else                       h1name = Hist1->fNewName;

  if (Hist2->fNewName == "") h2name.ReplaceAll("/","_");
  else                       h2name = Hist2->fNewName;

  hist_file_t* hf1 = Hist1->fFile;
  hist_file_t* hf2 = Hist2->fFile;
  
  TH1F* hpx1 = (TH1F*) gh1(hf1->fName,Hist1->fModule,Hist1->fName)->Clone(h1name);
  if (Hist1->fRebin > 0) hpx1->Rebin(Hist1->fRebin);
  
  TH1F* hpx2 = (TH1F*) gh1(hf2->fName,Hist2->fModule,Hist2->fName)->Clone(h2name);
  if (Hist2->fRebin > 0) hpx2->Rebin(Hist2->fRebin);

  stn_dataset_t* ds1 = hf1->fDataset;
  stn_dataset_t* ds2 = hf2->fDataset;

  if (Hist2->fScale == 1) {
    double scale = (ds1->fNGenEvents+0.)/(ds2->fNGenEvents+0.);
    hpx2->Scale(scale);
  }
  
  if (Hist2->fScale == 2) {
    double scale = hpx1->Integral()/hpx2->Integral();
    hpx2->Scale(scale);
  }
//-----------------------------------------------------------------------------
// create a canvas
//-----------------------------------------------------------------------------
  TString canvas_name(Hist1->fModule);
  canvas_name += "_";
  canvas_name += h1name;
  canvas_name += "_vs_";
  canvas_name += Hist2->fModule;
  canvas_name += "_";
  canvas_name += h2name;

  int cx = (Hist1->fCanvasSizeX < 0) ? 1200 : Hist1->fCanvasSizeX;
  int cy = (Hist1->fCanvasSizeY < 0) ?  800 : Hist1->fCanvasSizeY;

  TCanvas* c = new TCanvas(canvas_name.Data(),canvas_name.Data(),cx,cy);
  c->SetLogy(Hist1->fYLogScale);
//-----------------------------------------------------------------------------
// the two histograms correspond to slightly different NPOT
//-----------------------------------------------------------------------------
  hpx1->SetLineColor(kRed-3);
  hpx1->SetLineWidth(2);
  hpx1->SetTitle("");
  if (Hist1->fXMin < Hist1->fXMax) hpx1->GetXaxis()->SetRangeUser(Hist1->fXMin,Hist1->fXMax);
  if (Hist1->fXAxisTitle != ""   ) hpx1->GetXaxis()->SetTitle(Hist1->fXAxisTitle.Data());

  hpx1->Draw();

  hpx2->SetLineWidth(1);

  if (Hist2->fLineColor >=0) hpx2->SetLineColor(Hist2->fLineColor);
  if (Hist2->fFillStyle >=0) hpx2->SetFillStyle(Hist2->fFillStyle);
  if (Hist2->fFillColor >=0) hpx2->SetFillColor(Hist2->fFillColor);

  if (Hist2->fMarkerStyle >=0) hpx2->SetMarkerStyle(Hist2->fMarkerStyle);
  if (Hist2->fMarkerColor >=0) hpx2->SetMarkerColor(Hist2->fMarkerColor);
  if (Hist2->fMarkerSize  >=0) hpx2->SetMarkerSize (Hist2->fMarkerSize );

  printf("ds2 marker: %i\n",Hist2->fMarkerStyle);
  if (Hist2->fMarkerStyle >=0) hpx2->Draw("ep,sames");
  else                         hpx2->Draw("sames");
//-----------------------------------------------------------------------------
// position statboxes - need to update the canvas first
//-----------------------------------------------------------------------------
  c->Modified();
  c->Update();
  plot_stat_box(hpx1,0.65,0.65,0.9,0.9);
  plot_stat_box(hpx2,0.65,0.40,0.9,0.65);
//-----------------------------------------------------------------------------
// add legend
//-----------------------------------------------------------------------------
  float xmin{0.65}, ymin{0.25}, xmax{0.90}, ymax{0.35};
  if (Hist1->fLegendXMin > 0) { // redefine the legend position, normalized coords
    xmin = Hist1->fLegendXMin;
    ymin = Hist1->fLegendYMin;
    xmax = Hist1->fLegendXMax;
    ymax = Hist1->fLegendYMax;
  }
  
  TLegend* leg = new TLegend(xmin,ymin,xmax,ymax);
  leg->AddEntry(hpx1,Hist1->fLabel.Data(),"pl");  // "pl"
  leg->AddEntry(hpx2,Hist2->fLabel.Data(),"f");
  leg->SetBorderSize(0);
  leg->Draw();
//-----------------------------------------------------------------------------
// write DS names inside the plot
//-----------------------------------------------------------------------------
  TString label;

  if (Hist1->fPlotLabel != "") {
    label = Hist1->fPlotLabel;
  }
  else {
    label = Hist1->fName;
    if (Hist1->fName != Hist2->fName) {
      label += " vs ";
      label += Hist2->fName;
    }
  }

  draw_label_ndc(label.Data(),0.15,0.86,0.03,52); // lower left corner

  c->Modified(); c->Update();
//-----------------------------------------------------------------------------
// do we need to add something else? Print = -1 serves that purpose
//-----------------------------------------------------------------------------
  if (Hist1->fPlotName == "") {
    TString hn1(Hist1->fName), hn2(Hist2->fName);

    TString fol1 = hn1(0,hn1.Index('/'));
    TString nam1 = hn1(hn1.Index('/')+1,hn1.Length());

    TString fol2 = hn2(0,hn2.Index('/'));
    TString nam2 = hn2(hn2.Index('/')+1,hn2.Length());

    if (Hist1->fYLogScale == 1) {
      Hist1->fPlotName = Form("%s_%s_vs_%s_%s_log",
			      Hist1->fName.Data(),fol1.Data(),fol2.Data(),nam1.Data());
    }
    else {
      Hist1->fPlotName = Form("%s_%s_vs_%s_%s_lin",
			      Hist1->fName.Data(),fol1.Data(),fol2.Data(),nam1.Data());
    }
  }
  
  if (Print == -1) {
    Hist1->fCanvas   = c;
    Hist1->fHist     = hpx1;
    Hist1->fOutputFn = Form("%s/eps/%s.eps",FiguresDir,Hist1->fPlotName.Data());
  }
  else { 
//-----------------------------------------------------------------------------
// .png files are written into /png/ subdirectory
//-----------------------------------------------------------------------------
    if (Print == 1) {
      c->Print(Form("%s/eps/%s.eps",FiguresDir,Hist1->fPlotName.Data())) ;
    }
  }
  return;
}

#endif
