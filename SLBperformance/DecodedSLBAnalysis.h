//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr 18 11:06:32 2017 by ROOT version 5.34/34
// from TTree slboard/slboard
// found on file: cosmics_dif_1_1_commissioning.raw.root
//////////////////////////////////////////////////////////

#ifndef DecodedSLBAnalysis_h
#define DecodedSLBAnalysis_h
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TSystemFile.h"
//#include "TIter.h"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class DecodedSLBAnalysis {
public :

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TString         datadir;

   // Declaration of leaf types
      Int_t           event;
   Int_t           acqNumber;
   Int_t           n_slboards;
   Int_t           slot[15];
   Int_t           slboard_id[15];
   Int_t           chipid[15][16];
   Int_t           nColumns[15][16];
   Int_t           startACQ[15];
   Int_t           rawTSD[15];
   Float_t         TSD[15];
   Int_t           rawAVDD0[15];
   Int_t           rawAVDD1[15];
   Float_t         AVDD0[15];
   Float_t         AVDD1[15];
   Int_t           bcid[15][16][15];
   Int_t           corrected_bcid[15][16][15];
   Int_t           badbcid[15][16][15];
   Int_t           nhits[15][16][15];
   Int_t           charge_lowGain[15][16][15][64];
   Int_t           charge_hiGain[15][16][15][64];
   Int_t           gain_hit_low[15][16][15][64];
   Int_t           gain_hit_high[15][16][15][64];

     // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_acqNumber;   //!
   TBranch        *b_n_slboards;   //!
   TBranch        *b_slot;   //!
   TBranch        *b_slboard_id;   //!
   TBranch        *b_chipid;   //!
   TBranch        *b_nColumns;   //!
   TBranch        *b_startACQ;   //!
   TBranch        *b_rawTSD;   //!
   TBranch        *b_TSD;   //!
   TBranch        *b_rawAVDD0;   //!
   TBranch        *b_rawAVDD1;   //!
   TBranch        *b_AVDD0;   //!
   TBranch        *b_AVDD1;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_corrected_bcid;   //!
   TBranch        *b_badbcid;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_lowGain;   //!
   TBranch        *b_highGain;   //!
   TBranch        *b_gain_hit_low;   //!
   TBranch        *b_gain_hit_high;   //!
  
   //DecodedSLBAnalysis(TTree *tree=0);
   DecodedSLBAnalysis(TString tree_s, TString treename);
   // DecodedSLBAnalysis(TList *f=0);

   virtual ~DecodedSLBAnalysis();
   // int     main(int argc, char* argv[2]);
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);

   virtual int     NSlabsAnalysis(TString, int);

   // write file with masked channels
   // analysis of pedestal and writting of the file with pedestals per chi/channel/sca
   virtual void     PedestalAnalysis(int slboard,TString grid, int nmaxhits);
   // read pedestals, maps, masked channels
   virtual void     ReadMap(TString filename, int slboard);
   virtual void     ReadMasked(TString filename);
   virtual void     ReadPedestals(TString filename);
   //Monitoring
   virtual void    SlowControlMonitoring(TString outputname);
   virtual void     Monitoring(TString outputname, int freq, bool shifter);
   virtual void  QuickDisplay(TString outputname);

   virtual void     SynchronizationStudies(TString outputname, int freq, bool shifter);
   //signal analysis: MIP fitt and signal/noise 
   virtual void     SignalAnalysis(int slboard, TString outputname, int nmaxhits);
   virtual TF1 *langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);
   //   virtual Double_t langaufun(Double_t *x, Double_t *par);
   // bcid correlations of retriggers (good vs bad events)
   virtual void     Retriggers(int slboard, TString sufix,  int maxnhits);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

private :

  Float_t map_pointX[15][16][64];
  Float_t map_pointY[15][16][64];
  Int_t masked[16][64];
  std::vector<std::vector<std::vector<Double_t> > > ped_mean;
  std::vector<std::vector<std::vector<Double_t> > > ped_error;
  std::vector<std::vector<std::vector<Double_t> > > ped_width;

  std::vector<std::vector<std::vector<std::vector<Double_t> > > > ped_mean_slboard;
  std::vector<std::vector<std::vector<std::vector<Double_t> > > > ped_error_slboard;
  std::vector<std::vector<std::vector<std::vector<Double_t> > > > ped_width_slboard;



};

#endif

#ifdef DecodedSLBAnalysis_cxx
DecodedSLBAnalysis::DecodedSLBAnalysis(TString tree_s, TString treename="siwecaldecoded") : fChain(0) 
{
  cout<<tree_s<<endl;  
  TFile *f = new TFile(tree_s);
  TTree *tree = (TTree*)f->Get(treename);
  //  tree->Print();
  Init(tree);
  
}

/*
DecodedSLBAnalysis::DecodedSLBAnalysis(TList *f) : fChain(0) 
{
// if parameter tree is not specified (or zero), use a list of of files provided as input

  TIter next(f);
  TSystemFile *file;
  TString fname;
  while((file = (TSystemFile*)next())){
      fname = file->GetName();
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
      TTree *tree=0;
      f->GetObject(treename,tree);
      Init(tree);
  }


}
*/
DecodedSLBAnalysis::~DecodedSLBAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DecodedSLBAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DecodedSLBAnalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}



void DecodedSLBAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("acqNumber", &acqNumber, &b_acqNumber);
   fChain->SetBranchAddress("n_slboards", &n_slboards, &b_n_slboards);
   fChain->SetBranchAddress("slot", slot, &b_slot);
   fChain->SetBranchAddress("slboard_id", slboard_id, &b_slboard_id);
   fChain->SetBranchAddress("chipid", chipid, &b_chipid);
   fChain->SetBranchAddress("nColumns", nColumns, &b_nColumns);
   fChain->SetBranchAddress("startACQ", startACQ, &b_startACQ);
   fChain->SetBranchAddress("rawTSD", rawTSD, &b_rawTSD);
   fChain->SetBranchAddress("TSD", TSD, &b_TSD);
   fChain->SetBranchAddress("rawAVDD0", rawAVDD0, &b_rawAVDD0);
   fChain->SetBranchAddress("rawAVDD1", rawAVDD1, &b_rawAVDD1);
   fChain->SetBranchAddress("AVDD0", AVDD0, &b_AVDD0);
   fChain->SetBranchAddress("AVDD1", AVDD1, &b_AVDD1);
   fChain->SetBranchAddress("bcid", bcid, &b_bcid);
   fChain->SetBranchAddress("corrected_bcid", corrected_bcid, &b_corrected_bcid);
   fChain->SetBranchAddress("badbcid", badbcid, &b_badbcid);
   fChain->SetBranchAddress("nhits", nhits, &b_nhits);
   fChain->SetBranchAddress("charge_lowGain", charge_lowGain, &b_lowGain);
   fChain->SetBranchAddress("charge_hiGain", charge_hiGain, &b_highGain);
   fChain->SetBranchAddress("gain_hit_low", gain_hit_low, &b_gain_hit_low);
   fChain->SetBranchAddress("gain_hit_high", gain_hit_high, &b_gain_hit_high);
   Notify();

}

Bool_t DecodedSLBAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
  return kTRUE;
}

void DecodedSLBAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DecodedSLBAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DecodedSLBAnalysis_cxx
