//# Copyright 2020  Adrián Irles IJCLab (CNRS/IN2P3)

#include "TROOT.h"
#include "TFile.h"
#include "DecodedSLBAnalysis.cc"

int Noise(TString filename_in, TString output="", int gain=1, int sca=15){


  filename_in=filename_in+".root";
  cout<<" Display of file: "<<filename_in<<endl;
  DecodedSLBAnalysis ss(filename_in,"siwecaldecoded");

  cout<<"Start NSlabsNAalysis"<<endl;
  int result=ss.NSlabsAnalysisNoise(output,1,12,gain,sca);
  return result;

}