//# Copyright 2020  Adrián Irles IJCLab (CNRS/IN2P3)

#include "TROOT.h"
#include "TFile.h"
#include "DecodedSLBAnalysis.cc"

void Monitoring(TString filename_in, TString output="", int freq=1, 
    bool shifter=false, TString cob_positions_str="") {
  filename_in=filename_in+".root";
  cout<<" Monitoring of file: "<<filename_in<<endl;
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
    
    ss.ReadMap(map,i_layer);
  }

  //  ss.n_layers=3;
  ss.Monitoring(output,freq,shifter);
  //ss.SynchronizationStudies(output,freq,shifter); 
  if(shifter==false) gSystem->Exit(0);

}
