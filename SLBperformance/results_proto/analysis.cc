#include "analysis.h"


 
void analysis(TString run, int slboard, bool is_cob=false) {

  gROOT->Reset();
  //SetIrlesStyle();
  //  gROOT->LoadMacro("Labels.C");
  gStyle->SetOptFit(0); 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.2);
  gStyle->SetTitleY(1.0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetMarkerSize(1.2);

  //  if(slboard==8 || slboard==12) is_cob=1;
  triggers(run, slboard, is_cob);

  TFile *file = new TFile(TString::Format("plots/layer_%i_%s.root",slboard,run.Data()) , "RECREATE");  
  mipanalysis(file,run, slboard,is_cob);
  file->Close();

}
