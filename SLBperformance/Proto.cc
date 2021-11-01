//# Copyright 2020  Adrián Irles IJCLab (CNRS/IN2P3)

#include "TROOT.h"
#include "TFile.h"
#include "DecodedSLBAnalysis.cc"

int Proto(TString filename_in, TString output="", TString type="pedestal",
          TString pedestal_file="", TString cob_positions_str="") {

  filename_in=filename_in+".root";
  cout<<" Display of file: "<<filename_in<<endl;
  DecodedSLBAnalysis ss(filename_in);


  // layer 0 is the slab with slboard 17, etc... setup July 2021
  ss.slboard_array_mapping[0]=17;
  ss.slboard_array_mapping[1]=8;
  ss.slboard_array_mapping[2]=10;
  ss.slboard_array_mapping[3]=5;
  ss.slboard_array_mapping[4]=1;
  ss.slboard_array_mapping[5]=13;
  ss.slboard_array_mapping[6]=11;
  ss.slboard_array_mapping[7]=7;
  ss.slboard_array_mapping[8]=14;
  ss.slboard_array_mapping[9]=3;
  ss.slboard_array_mapping[11]=6;
  ss.slboard_array_mapping[12]=9;
  ss.slboard_array_mapping[13]=2;
  ss.slboard_array_mapping[14]=0;
  
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
  
  for(int i_slboard = 0; i_slboard < nslabs; i_slboard++) {
    TString map_name="../mapping/fev10_chip_channel_x_y_mapping.txt";
    if ( std::find(cob_positions.begin(), cob_positions.end(), i_slboard) != cob_positions.end() ) {
        map_name = "../mapping/fev11_cob_chip_channel_x_y_mapping.txt";
    }
    cout<<filename_in<<" "<<output<<" islboard " <<i_slboard<<endl;
    ss.ReadMap(map_name, i_slboard);
    if(type=="retriggers") ss.Retriggers(i_slboard,output,4);
  }
  
  cout<<"Start NSlabsNAalysis"<<endl;
  if(type=="pedestal") int result=ss.NSlabsAnalysis(output,"pedestal");
  if(type=="mip")   int result=ss.NSlabsAnalysis(output,"mip",5,pedestal_file);

  //for(int i_slboard=0; i_slboard<1; i_slboard++)  ss.Retriggers(i_slboard,output,4);
  gSystem->Exit(0);

  return 0;

}
