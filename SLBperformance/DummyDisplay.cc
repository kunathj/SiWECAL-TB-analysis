//# Copyright 2020  Adrián Irles IJCLab (CNRS/IN2P3)

#include "TROOT.h"
#include "TFile.h"
#include "DecodedSLBAnalysis.cc"

void DummyDisplay(TString filename_in, TString output="", int ncoinc=7, TString cob_positions_str="") {


  filename_in=filename_in+".root";
  cout<<" Display of file: "<<filename_in<<endl;
  DecodedSLBAnalysis ss(filename_in);


  int nslabs=15;
  // Process the cob positions string.
  std::vector<int> cob_positions;
  if ( cob_positions_str != "" ) {
    if (!cob_positions_str.IsDigit()) {
      throw std::invalid_argument( "Invalid COB positions string: " + cob_positions_str);
    }
    TString tok;
    Ssiz_t from = 0;
    while (cob_positions_str.Tokenize(tok, from)) cob_positions.push_back(tok.Atoi());
    std::set<int> cob_positions_set(cob_positions.begin(), cob_positions.end());
    if ((cob_positions_set.size() != cob_positions.size()) ||
        (*std::max_element(cob_positions.begin(), cob_positions.end()) >= nslabs) ||
        (*std::min_element(cob_positions.begin(), cob_positions.end()) < 0)) {
          throw std::invalid_argument( "Invalid COB positions string: " + cob_positions_str);
    }
  }
  
  for(int islab=0; islab<nslabs; islab++) {
    TString map_name="../mapping/fev10_chip_channel_x_y_mapping.txt";
    if ( std::find(cob_positions.begin(), cob_positions.end(), islab) != cob_positions.end() ) {
        map_name = "../mapping/fev11_cob_chip_channel_x_y_mapping.txt";
    }
    //  if(i_slboard==8 || i_slboard==12) map="../mapping/fev11_cob_chip_channel_x_y_mapping.txt";
    /// else map="../mapping/fev10_chip_channel_x_y_mapping.txt"; 
    ss.ReadMap(map,i_slboard);
  }

  //  ss.n_slboards=3;
  ss.QuickDisplay(TString::Format("%s_%i",output.Data(),ncoinc),ncoinc);/// using coincidences from 7 layers
    
  
  if(ncoinc==7) {
    ss.QuickDisplay(TString::Format("%s_%i",output.Data(),ncoinc),ncoinc);/// using coincidences from 7 layers
    ss.SlowControlMonitoring(output);
    ss.Monitoring(output,10,false);
    ss.HitMapsSimpleTracks(output,ncoinc);//
    ss.SynchronizationStudies(TString::Format("%s_%i",output.Data(),ncoinc),ncoinc,false);
  }
  gSystem->Exit(0);

}
