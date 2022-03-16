//# Copyright 2020 Adrián Irles IJCLab (CNRS/IN2P3)

#ifndef SLBdecoded2ROOT_CC
#define SLBdecoded2ROOT_CC

#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <sstream>
#include <string>


// .x SLBdecoded2ROOT.C+

using std::cout;
using std::endl;

class SLBdecoded2ROOT {

public:
  SLBdecoded2ROOT(){
  };
  ~SLBdecoded2ROOT(){

  };
  
  void ReadFile(TString inputFileName, bool overwrite=false, TString outFileName = "default",bool zerosupression=false);

protected:

  enum sizes{
    SLBDEPTH=15,
    MEMDEPTH=15,
    NCHANNELS=64,
    NCHIP=16,
    NEGDATA_THR=11, //event with data below are tagged badbcid+=32
    BCIDTHRES=4
  };

  int R2Rstate;

  void Initialisation();
  void treeInit(bool);
  //  int  GetTree(TString rootfilename);
  void GetBadBCID();

  TFile* fout;
  TTree* tree;

  TFile *finroot;
  TTree* slboardread;

  int bcid[SLBDEPTH][NCHIP][MEMDEPTH];
  int corrected_bcid[SLBDEPTH][NCHIP][MEMDEPTH];
  int badbcid[SLBDEPTH][NCHIP][MEMDEPTH];
  int nhits[SLBDEPTH][NCHIP][MEMDEPTH];
  int charge_low[SLBDEPTH][NCHIP][MEMDEPTH][NCHANNELS];
  int charge_high[SLBDEPTH][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_low[SLBDEPTH][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_high[SLBDEPTH][NCHIP][MEMDEPTH][NCHANNELS];
  int event;
  //int numbcid;
  int numCol[SLBDEPTH][NCHIP];
  int chipID[SLBDEPTH][NCHIP];
  int slot[SLBDEPTH];
  int slboard_id[SLBDEPTH];
  int n_slboards;
  int acqNumber;

  //slowcontrol
  double startACQ[SLBDEPTH];
  int rawTSD[SLBDEPTH];
  float TSD[SLBDEPTH];
  int rawAVDD0[SLBDEPTH];
  int rawAVDD1[SLBDEPTH];
  float AVDD0[SLBDEPTH];
  float AVDD1[SLBDEPTH];


  //  InfoChip * info;
};

//******************************************************************************************************************

void SLBdecoded2ROOT::Initialisation() {
  fout->cd(); R2Rstate=-1;

  tree = new TTree("siwecaldecoded","siwecaldecoded");

  tree->Branch("event",&event,"event/I");
  tree->Branch("acqNumber",&acqNumber,"acqNumber/I");
  tree->Branch("n_slboards",&n_slboards,"n_slboards/I");

  TString name;
  name= TString::Format("slot[%i]/I",SLBDEPTH);
  tree->Branch("slot",slot,name);
  
  name= TString::Format("slboard_id[%i]/I",SLBDEPTH);
  tree->Branch("slboard_id",slboard_id,name);
  
  name= TString::Format("chipid[%i][%i]/I",SLBDEPTH,NCHIP);
  tree->Branch("chipid",chipID,name);

  name= TString::Format("nColumns[%i][%i]/I",SLBDEPTH,NCHIP);
  tree->Branch("nColumns",numCol,name);

  name= TString::Format("startACQ[%i]/F",SLBDEPTH);
  tree->Branch("startACQ",startACQ,name);

  name= TString::Format("rawTSD[%i]/I",SLBDEPTH);
  tree->Branch("rawTSD",rawTSD,name);

  name= TString::Format("TSD[%i]/F",SLBDEPTH);
  tree->Branch("TSD",TSD,name);
  
  name= TString::Format("rawAVDD0[%i]/I",SLBDEPTH);
  tree->Branch("rawAVDD0",rawAVDD0,name);

  name= TString::Format("rawAVDD1[%i]/I",SLBDEPTH);
  tree->Branch("rawAVDD1",rawAVDD1,name);

  name= TString::Format("AVDD0[%i]/F",SLBDEPTH);
  tree->Branch("AVDD0",AVDD0,name);

  name= TString::Format("AVDD1[%i]/F",SLBDEPTH);
  tree->Branch("AVDD1",AVDD1,name);
  
  name= TString::Format("bcid[%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH);
  tree->Branch("bcid",bcid,name);

  name= TString::Format("corrected_bcid[%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH);
  tree->Branch("corrected_bcid",corrected_bcid,name);

  name= TString::Format("badbcid[%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH);
  tree->Branch("badbcid",badbcid,name);

  name= TString::Format("nhits[%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH);
  tree->Branch("nhits",nhits,name);

  name= TString::Format("lowGain[%i][%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH,NCHANNELS);
  tree->Branch("charge_lowGain",charge_low,name);

  name= TString::Format("highGain[%i][%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH,NCHANNELS);
  tree->Branch("charge_hiGain",charge_high,name);

  name= TString::Format("gain_hit_low[%i][%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH,NCHANNELS);
  tree->Branch("gain_hit_low",gain_hit_low,name);

  name= TString::Format("gain_hit_high[%i][%i][%i][%i]/I",SLBDEPTH,NCHIP,MEMDEPTH,NCHANNELS);
  tree->Branch("gain_hit_high",gain_hit_high,name);

  return;
}






//******************************************************************************************************************

void SLBdecoded2ROOT::treeInit(bool zerosupression=false) { //init data for a single SPILL ?

  for (int isl=0; isl<SLBDEPTH; isl++) {
    for (int k=0; k<NCHIP; k++) {
      for (int i=0; i<MEMDEPTH; i++) {
	bcid[isl][k][i]=-999;
	badbcid[isl][k][i]=-999;
	corrected_bcid[isl][k][i]=-999;
	nhits[isl][k][i]=-999;
	for (int j=0; j<NCHANNELS; j++) {
	  charge_low[isl][k][i][j]=-999;
	  charge_high[isl][k][i][j]=-999;
	  gain_hit_low[isl][k][i][j]=-999;
	  gain_hit_high[isl][k][i][j]=-999;
	}
      }
      
      chipID[isl][k]=-999;
      numCol[isl][k]=0;
      startACQ[isl]=-1;
      rawTSD[isl]=-1;
      TSD[isl]=-1;
      rawAVDD0[isl]=-1;
      rawAVDD1[isl]=-1;
      AVDD0[isl]=-1;
      AVDD1[isl]=-1;
    }
    slot[isl]=-1;
    slboard_id[isl]=-1;
  }
  n_slboards=-1;
  acqNumber=-1;

  return;
}


//******************************************************************************************************************


//******************************************************************************************************************

void SLBdecoded2ROOT::ReadFile(TString inputFileName, bool overwrite, TString outFileName, bool zerosupression) {

  event=0;
  acqNumber=0;
  
  if(outFileName == "default"){
    outFileName = TString::Format("%s.root",inputFileName.Data());
    cout<<outFileName<<endl;
  }
  
  if(!overwrite){
    fout = new TFile(outFileName,"create");
    if(!fout->IsOpen()){
      return;
    }
  }
  else {
    fout = new TFile(outFileName,"recreate");
  }
  

  std::ifstream reading_file(inputFileName);
 
  if(!reading_file){
    cout<<" ERROR  ----------------------------------- No file: "<<inputFileName<<endl;
    return 0;
  } else {
    cout<<" Read File "<<inputFileName<<endl;
    //cout<<" slb= " <<slboard_index<<endl;
  }



  //FTDI
  int cycleID=-1;
  int tmp_n_slboards=0;
  TString tmpst;

  std::string strheader;
  std::getline(reading_file, strheader);
  std::cout << "header 1: " << strheader << std::endl;
  std::getline(reading_file, strheader);
  std::cout << "header 2: " << strheader << std::endl;
  std::cout << "FTDI Interface? " << strheader.find("SL_DIRECT_INTERFACE") << std::endl;
  std::cout << "COREMODULE " << strheader.find("COREMODULE") << std::endl;
  std::cout << "std::string::npos " << std::string::npos   << std::endl;
  TString readout_type="COREMODULE";
  //CRP 24/11/20 Correct analysis of std::string  
  //FTDI readout DIRECT Interface, only one slab
  std::size_t found = strheader.find("SL_DIRECT_INTERFACE");
  if (found!=std::string::npos) {
//if(strheader.find("SL_DIRECT_INTERFACE")!=0) {
    std::cout << "FTDI found" << std::endl;
    readout_type="FTDI";
    tmp_n_slboards=1;
    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);
  } else {
    reading_file >> tmpst >> tmpst >> tmpst  >> tmpst >>  tmpst >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst  >> tmpst >> tmp_n_slboards >> tmpst;
    cout<<" NB OF CONNECTED SLABs = " << tmp_n_slboards <<endl;
    std::getline(reading_file, strheader);

    for(int islboard=0; islboard<tmp_n_slboards; islboard++) {
      std::getline(reading_file, strheader);
      std::cout<<islboard<<" "<<strheader<< std::endl;
    }

    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);
    std::getline(reading_file, strheader);

  }

  
  // Initialisation((const int)tmp_n_slboards);
  Initialisation();

  
  // int bcid_cycle[20][16][15];

 

  while (reading_file) {
    // output the line
    TString tmpst1;
    TString tmpst;
    int size=0;
    int chip=-1;
    int slabidx=-1;
    int slabadd=-1;
    double start_acq=-1;
    int raw_tsd=-1;
    float _tsd=-1;
    int raw_avdd0=-1;
    int raw_avdd1=-1;
    float _avdd0=-1;
    float _avdd1=-1;

    reading_file >> tmpst1 >> tmpst >> size >> tmpst >> chip >> tmpst >>  tmpst >> tmpst >> slabidx >> tmpst >> slabadd >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst  >> tmpst >> tmpst >> cycleID >> tmpst >> start_acq >> tmpst >> raw_tsd >> tmpst >> raw_avdd0 >>  tmpst >> raw_avdd1 >> tmpst >> _tsd >> tmpst >> _avdd0 >>  tmpst >> _avdd1;
    //    if(slabidx>20)
    //std::cout << " -----" <<tmpst1<<" "<<size<< " "<<chip<<" "<<slabidx<<" "<<slabadd<<" "<<cycleID <<" "<<acqNumber<<" "<<start_acq<<" "<<tmpst1<<std::endl;
    //  if(slabidx<0) break;
    if( readout_type=="COREMODULE" && slabidx==-1) break;//ftdi
    if( readout_type=="FTDI"  && slabidx!=-1) break;
    if( readout_type=="FTDI") {
      slabidx=0;
      slabadd=0;
    }

    if(acqNumber==0) treeInit(zerosupression);
     if(acqNumber>0 && acqNumber!=cycleID) {
      GetBadBCID();
      tree->Fill();
      treeInit(zerosupression);
    }

    

    //save variables
    acqNumber=cycleID;
    n_slboards=tmp_n_slboards;
    event++;
    int previousBCID=-1000;
    int loopBCID=0;

    startACQ[slabadd]=start_acq;
    rawTSD[slabadd]=raw_tsd;
    TSD[slabadd]=_tsd;
    rawAVDD0[slabadd]=raw_avdd0;
    rawAVDD1[slabadd]=raw_avdd1;
    AVDD0[slabadd]=raw_avdd0;
    AVDD1[slabadd]=raw_avdd1;

    slot[slabadd]=-1;//we don't know this information... the DQ only provides addresses.
    slboard_id[slabadd]=slabadd;

    
    for(int i=0; i<size; i++) {
      //##0 BCID 1627 SCA 1 #Hits 49
      int bcid_tmp=-1;
      int sca=-1;
      int nhits_tmp=-1;
      reading_file >> tmpst >> tmpst >> bcid_tmp >> tmpst >> sca >>  tmpst  >>  nhits_tmp ;
      //cout<<bcid_tmp<<" "<<sca<<" "<<nhits_tmp<<" "<<endl;
      sca=size-(sca+1);
      bcid[slabadd][chip][sca]=bcid_tmp;
      nhits[slabadd][chip][sca]=nhits_tmp;
      numCol[slabadd][chip]++;
      
      if(bcid[slabadd][chip][sca] > 0 && bcid[slabadd][chip][sca]-previousBCID < 0) loopBCID++;
      if(bcid[slabadd][chip][sca] > 0 ) corrected_bcid[slabadd][chip][sca] = bcid[slabadd][chip][sca]+loopBCID*4096;
      previousBCID=bcid_tmp;
      
      if(chip>-1 && chip<16) {
	chipID[slabadd][chip]=chip;
      } else {
	cout<<"Wrong chipID = "<<chip<<endl;
	break;
      }
      //      }
      int nchn=0;
      if(zerosupression==false) nchn=NCHANNELS;
      else nchn = nhits_tmp;
      //Ch 0 LG 322 1 0 HG 408 1 0
      for(int ichn=0; ichn<nchn; ichn++) {
	int chn=-1;
	int lg=-1;
	int hg=-1;
	int lg_bit=-1;
	int hg_bit=-1;
	reading_file >> tmpst >> chn >> tmpst >> lg >>  lg_bit >> tmpst >>  tmpst >> hg >> hg_bit >> tmpst;
	//cout << chn << " "<< hg <<" " <<hg_bit<<endl;
	//	if(slabid==slboard_index) {
	  charge_low[slabadd][chip][sca][chn]=lg;
	  gain_hit_low[slabadd][chip][sca][chn]=lg_bit;
	  charge_high[slabadd][chip][sca][chn]=hg;
	  gain_hit_high[slabadd][chip][sca][chn]=hg_bit;
	  //}
      }//end channel  
    }//end events of the chi

  }
  
  fout->cd(); 
  fout->Write(0);
//  for(int ichip=0; ichip<16; ichip++) {
//   bcid_diff[ichip]->Write();
//    bcid_correl[ichip]->Write();
//  }
  fout->Close();
    
  return;
}

void SLBdecoded2ROOT::GetBadBCID() {

  //add tags
  //  int count_negdata[SLBDEPTH];
  //  for(int i=0; i<SLBDEPTH; i++) count_negdata[i]=0;

  for(int i=0; i<SLBDEPTH; i++) {
  for (int k=0; k<NCHIP; k++) {
    //only for valid chips in this spill
    if (chipID[i][k]>=0) {
      for (int ibc=0; ibc<numCol[i][k]; ibc++) {

	// if sca+1 is filled with consec bcid, but sca+2 not, then badbcid[sca]==1 && badbcid[sca+1]==2 (bcid+1 issue, events are not bad, just the next sca trigger is empty)
	// if sca+1 is filled with consec bcid, and sca+2 also, then badbcid[sca]==3 && badbcid[sca+1]==3 (retriggering)
	// if sca+1 is not filled with consec bcid,  badbcid==0

	if(ibc==0) {
	  badbcid[i][k][ibc]=0;
	  int corr_bcid=corrected_bcid[i][k][ibc];
	  int corr_bcid1=0;
	  int corr_bcid2=0;

	  if(corrected_bcid[i][k][ibc+1]>0 && corrected_bcid[i][k][ibc]>0 && (corrected_bcid[i][k][ibc+1]-corrected_bcid[i][k][ibc])>0) 
	    corr_bcid1=corrected_bcid[i][k][ibc+1];

	  if(corrected_bcid[i][k][ibc+2]>0 && (corrected_bcid[i][k][ibc+2]-corrected_bcid[i][k][ibc+1])>0) 
	    corr_bcid2=corrected_bcid[i][k][ibc+2];

	  if(corr_bcid2>0) {
	    //empty events
	    if( ( corr_bcid2-corr_bcid1) >(BCIDTHRES - 1) && (corr_bcid1-corr_bcid) ==1) {
	      badbcid[i][k][ibc]=1;
	      badbcid[i][k][ibc+1]=2;
	    }
	    // pure retriggers
	    if( ( corr_bcid2-corr_bcid1) < BCIDTHRES && (corr_bcid1-corr_bcid) < BCIDTHRES) {
	      badbcid[i][k][ibc]=3;
	      badbcid[i][k][ibc+1]=3;
	      badbcid[i][k][ibc+2]=3;
	    }
    	  }

	  if( corr_bcid1 > 0 && (corr_bcid1-corr_bcid) > 1 && (corr_bcid1-corr_bcid) <BCIDTHRES) {
	    badbcid[i][k][ibc]=3;
	    badbcid[i][k][ibc+1]=3;
	  }  
	} //ibc==0 if 

	if(ibc>0 && badbcid[i][k][ibc]<0 && corrected_bcid[i][k][ibc] >0 &&  (corrected_bcid[i][k][ibc]-corrected_bcid[i][k][ibc-1])>0 ) {
	  badbcid[i][k][ibc]=0;
	  int corr_bcid=corrected_bcid[i][k][ibc];
	  int corr_bcidminus=corrected_bcid[i][k][ibc-1];

	  if(corrected_bcid[i][k][ibc+1]>0 && (corrected_bcid[i][k][ibc+1]-corrected_bcid[i][k][ibc])>0) {
	    int corr_bcid1=corrected_bcid[i][k][ibc+1];

	    if(corrected_bcid[i][k][ibc+2]>0 && (corrected_bcid[i][k][ibc+2]-corrected_bcid[i][k][ibc+1])>0) {
	      int corr_bcid2=corrected_bcid[i][k][ibc+2];
	      if( ( corr_bcid2-corr_bcid1) < BCIDTHRES && (corr_bcid1-corr_bcid) < BCIDTHRES) {
		badbcid[i][k][ibc]=3;
		badbcid[i][k][ibc+1]=3;
		badbcid[i][k][ibc+2]=3;
	      }
	      if( (corr_bcid1-corr_bcid) < BCIDTHRES && (corr_bcid-corr_bcidminus) < BCIDTHRES ) badbcid[i][k][ibc]=3;

	      if( badbcid[i][k][ibc]!=3 && ( corr_bcid2-corr_bcid1) >(BCIDTHRES - 1) && (corr_bcid1-corr_bcid) ==1) {
		badbcid[i][k][ibc]=1;
		badbcid[i][k][ibc+1]=2;
	      }
	      if( badbcid[i][k][ibc]!=3 && ( corr_bcid2-corr_bcid1) >(BCIDTHRES - 1) && (corr_bcid1-corr_bcid) > 1 && (corr_bcid1-corr_bcid) <BCIDTHRES) {
		badbcid[i][k][ibc]=3;
		badbcid[i][k][ibc+1]=3;
	      }
	      if( (corr_bcid-corr_bcidminus) < BCIDTHRES ) badbcid[i][k][ibc]=3;

	      //if( badbcid[i][k][ibc-1]==1 && (corr_bcid1-corr_bcid) > (BCIDTHRES - 1)) badbcid[i][k][ibc]=2;
	    } else {
	      if( (corr_bcid1-corr_bcid) < BCIDTHRES ) badbcid[i][k][ibc]=3;
	      if( (corr_bcid-corr_bcidminus) < BCIDTHRES ) badbcid[i][k][ibc]=3;
	    } //ibc+2 if
	  } else {
	    if( (corr_bcid-corr_bcidminus) < BCIDTHRES ) badbcid[i][k][ibc]=3;
	  }//ibc+1 if
	} //ibc>0 if 

      
	//tag zero (under/over flow) data
	//	count_negdata=0;
	
	//	for (int ichan=0; ichan<NCHANNELS; ichan++) {
	//  if  (charge_high[i][k][ibc][ichan] < NEGDATA_THR) count_negdata++;
	//}//ichan

	//if (count_negdata>0) {badbcid[i][k][ibc]+=32;}

      }//ibc

    }//chipID
  }//k
  }//i   slboard
  
}

//******************************************************************************************************************

////////////////////////  GETTREE   ////////////////////////
/*int SLBdecoded2ROOT::GetTree (TString rootfilename) { //from raw2root 1st pass
  //open pre-precessed data
  finroot = new TFile(rootfilename,"read");
  if (! finroot ) {return 0;}
  slboardread = (TTree*)finroot->Get("siwecaldecoded"); //rawdata

  //get data from file
  slboardread->SetBranchAddress("gain_hit_high",gain_hit_high );
  slboardread->SetBranchAddress("gain_hit_low",gain_hit_low );
  slboardread->SetBranchAddress("charge_hiGain",charge_high );
  slboardread->SetBranchAddress("charge_lowGain",charge_low );
  slboardread->SetBranchAddress("bcid", bcid);
  slboardread->SetBranchAddress("badbcid", badbcid);
  slboardread->SetBranchAddress("nhits", nhits);
  slboardread->SetBranchAddress("event", &event);
  slboardread->SetBranchAddress("acqNumber", &acqNumber);
  slboardread->SetBranchAddress("corrected_bcid",corrected_bcid);
  slboardread->SetBranchAddress("nColumns",numCol);
  slboardread->SetBranchAddress("chipid",chipID);
  slboardread->SetBranchAddress("slot",slot);
  slboardread->SetBranchAddress("slboard_id",slboard_id);
  slboardread->SetBranchAddress("n_slboards",&n_slboards);
  R2Rstate=1;
  return 1;

}//method GetTree
*/

#endif

