#include "TFile.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TH1.h"
#include "TROOT.h"
#include "TRint.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TString.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "TLatex.h"
#include "../../include/utils.h"

//#include "../../style/Style.C"
//#include "../../style/Labels.C"

using namespace std;

int nchips=16;
int nlayers=15;

void pedanalysis(TString run="PedestalMIP_3GeVMIPscan", TString gain="low"){
  
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


  TFile *_file0 = TFile::Open(TString::Format("../results_calib/PedestalMIP_%s.root",run.Data()));

  TH2F* ped_all=new TH2F("ped_all","pedestal pos. ; Layer*20+Chip  ; SCA*100 +chn",300,-0.5,299.5,1500,-0.5,1499.5);
  TH2F* rms_all=new TH2F("rms_all","width of pedestal ; Layer*20+Chip  ; SCA*100 +chn",300,-0.5,299.5,1500,-0.5,1499.5);

  ofstream fout_ped(TString::Format("../../pedestals/Pedestal_%s_%sgain.txt",run.Data(),gain.Data()).Data(),ios::out);
  fout_ped<<"#pedestal results (fit to a gaussian) remove channels/sca with two pedestals peaks from the analysis : PROTO15"<<endl;
  fout_ped<<"#layer chip channel ped0 eped0 widthped0 ped1 eped1 widthped1... ped14 eped14 widhtped14 (all SCA)"<<endl;

  for(int layer=0; layer<nlayers; layer++) {
   
    // Comparing nbr entries in tag or not tag // GetWidth and Mean
    //TString map="../../mapping/fev10_chip_channel_x_y_mapping.txt";
    //  if(layer==0 || layer==2)  map="../../mapping/fev11_cob_chip_channel_x_y_mapping.txt";
    //  ReadMap(map);

     for(int i=0;i<nchips;i++){
    
      double nch=0;
      float avmean=0, avwidth=0;
    
      for(int j=0; j<64; j++) {
	fout_ped << layer<<" "<<i <<" " <<j<< " ";

	double ped[15]={0};
	double pederror[15]={0};
	double pedwidth[15]={0};
	double pedwidtherror[15]={0};


      	for(int isca=0; isca<15; isca++) {
	  TH1F* temp=(TH1F*)_file0->Get(TString::Format("layer_%i/ped_%s_chip%i_chn%i_sca%i",layer,gain.Data(),i,j,isca));
        
	  if(temp->GetEntries()>50) {
	    TSpectrum *s = new TSpectrum();
	    int npeaks = s->Search(temp,2,"",0.8);
	    if(npeaks == 1) {
	      Double_t *mean_peak=s->GetPositionX();
	      Double_t *mean_high=s->GetPositionY();
	      double mean_peak_higher=0;
	      double mean_high_higher=0;
	      int npeak_max=0;
              for(int ipeak=0; ipeak<npeaks; ipeak ++) {
                if(mean_high[ipeak]>mean_high_higher && mean_peak[ipeak]>180) {
                  mean_high_higher=mean_high[ipeak];
                  mean_peak_higher=mean_peak[ipeak];
                  npeak_max=ipeak;
                }
              }
	      	   
	      float mean_start_fit=mean_peak_higher;
	      
	      TF1 *f0 = new TF1("f0","gaus",mean_peak_higher-2,mean_peak_higher+2);
	      TF1 *f1 = new TF1("f1","gaus",mean_peak_higher-20,mean_peak_higher+20);
	      temp->Fit("f0","RQE");
	      temp->Fit("f1","RQE");

	      if(f0->GetParameter(2)<f1->GetParameter(2))  {
		ped[isca]=f0->GetParameter(1);
		pedwidth[isca]=f0->GetParameter(2);
		pederror[isca]=f0->GetParError(1);
		pedwidtherror[isca]=f0->GetParError(2);
	      }  else  {
		ped[isca]=f1->GetParameter(1);
		pedwidth[isca]=f1->GetParameter(2);
		pederror[isca]=f1->GetParError(1);
		pedwidtherror[isca]=f1->GetParError(2);
	      }
	      
	      delete s;
	    }
	  } else pedwidth[isca]=-1;
	  
	}//isca

	double ped_av=weigthedAv(ped, pederror, 15);
	double pedwidth_av=weigthedAv(pedwidth, pedwidtherror, 15);
	for(int isca=0; isca<15; isca++) {  
	  if(pedwidth[isca]>0.5 && pedwidth[isca]<8 ) {
	    fout_ped<<ped[isca]<< " " << pederror[isca]<<" "<<pedwidth[isca]<<" "; //no peaks found
	    ped_all->Fill(20*layer+i,100*isca+j,ped[isca]);
	    rms_all->Fill(20*layer+i,100*isca+j,pedwidth[isca]);
	  } else  {
	    if(pedwidth[isca]>-0.5) {
	      fout_ped<<ped_av<< " " << -5<<" "<<pedwidth_av<<" "; //no peaks found
	      ped_all->Fill(20*layer+i,100*isca+j,ped_av);
	      rms_all->Fill(20*layer+i,100*isca+j,pedwidth_av);
	    }
	    if(pedwidth[isca]<0) {
	      fout_ped<<-5<< " " << -5<<" "<<-5<<" "; //no peaks found
	      ped_all->Fill(20*layer+i,100*isca+j,100);
	      rms_all->Fill(20*layer+i,100*isca+j,100);
	    }
	  }
	}
	fout_ped<<endl;
      }//j
    }
  }


  TFile *_file1 = new TFile(TString::Format("../../pedestals/%s_%sgain_PedSummary.root",run.Data(),gain.Data()),"RECREATE");
  TCanvas* canvas0= new TCanvas("PedAna_Summary","Pedestal Summary",1800,400);   
  canvas0->Divide(2,1);
  canvas0->cd(1);
  ped_all->GetZaxis()->SetRangeUser(180,300);
  ped_all->Draw("colz");
  canvas0->cd(2);
  rms_all->GetZaxis()->SetRangeUser(1.0,6.0);
  if(gain=="low") rms_all->GetZaxis()->SetRangeUser(0.5,3.0);
  rms_all->Draw("colz");
  canvas0->Print(TString::Format("../../pedestals/%s_%sgain_PedSummary.png",run.Data(),gain.Data()));
  _file1->cd();
  ped_all->Write();
  rms_all->Write();
  canvas0->Write();
  _file1 ->Close();
  

}


 void mipanalysis_summary(TString run="3GeVMIPscan", TString gain="high"){
  
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


  TH2F* MIPM_all=new TH2F("MIPM_all","average of MPVs ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);
  TH2F* MIPN_all=new TH2F("MIPN_all","channels fitted ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);
  TH2F* MIPrms_all=new TH2F("MIPrms_all","rms  of MPVs ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);

  TH2F* mpv_all=new TH2F("mpv_all","mpv pos. ; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);
  TH2F* width_all=new TH2F("width_all","width of Landau ; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);
  TH2F* nentries_all=new TH2F("nentries_all","N entries; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);

  ofstream fout_mip(TString::Format("../../mip_calib/MIP_%s_%sgain.txt",run.Data(),gain.Data()).Data(),ios::out);
  fout_mip<<"#mip results PROTO15-TB2021-11"<<endl;
  fout_mip<<"#layer chip channel mpv empv widthmpv chi2ndf nentries"<<endl;

  ReadPedestalsProto(TString::Format("../../pedestals/Pedestal_%s_%sgain.txt",run.Data(),gain.Data()).Data(),false);

  TFile *_file1 = new TFile(TString::Format("../../mip_calib/%s_%sgain_MIPSummary.root",run.Data(),gain.Data()),"RECREATE");


  for(int layer=0; layer<nlayers; layer++) {

    TFile *_file0 = TFile::Open(TString::Format("../results_calib/PedestalMIP_%s.root",run.Data()));
     
    float avmpv[16]={0};
    float nmpv[16]={0};
    float rmsmpv[16]={0};

    cout<<"   LAYER : "<<layer<<endl;


    for(int i=0;i<nchips;i++){
      
      for(int j=0; j<64; j++) {
	TH1F *hmips = new TH1F("hmips","hmips",600,-199.5,400.5);
	hmips->Sumw2();
	for(int isca=0; isca<15; isca++) {
	  _file0->cd();
	  //TCanvas* canvashmips= new TCanvas("canvashmips","canvashmips",100,100);
	  //canvashmips->cd();
	  TH1F *temp=(TH1F*)_file0->Get(TString::Format("layer_%i/mip_%s_chip%i_chn%i_sca%i",layer,gain.Data(),i,j,isca));
	  
	  if(temp==NULL) continue;

	  for (int k=1;k<500;k++) {
	    double y = temp->GetBinContent(k);
	    double x = temp->GetXaxis()->GetBinCenter(k) -  ped_mean_slboard.at(layer).at(i).at(j).at(isca);
	    if(y>0 && ped_mean_slboard.at(layer).at(i).at(j).at(isca)>0) hmips->Fill(int(x),y);
	  }
	  // delete canvashmips;
	}

	for(int k=0; k<600; k++) hmips->SetBinError(k,sqrt(hmips->GetBinContent(k)));


	//	MIPN->Fill(map_pointX[i][j],map_pointY[i][j],hmips->GetEntries());
	
	Double_t fr[2];
	Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];
	if(gain=="high") {
	  pllo[0]=0.5; pllo[1]=15; pllo[2]=1.0; pllo[3]=0.5;
	  plhi[0]=10.0; plhi[1]=220.0; plhi[2]=100000000.0; plhi[3]=10.0;
	} else {
	  pllo[0]=0.1; pllo[1]=2; pllo[2]=1.0; pllo[3]=0.05;
	  plhi[0]=5; plhi[1]=15.0; plhi[2]=100000000.0; plhi[3]=2.0;
	}

	Double_t chisqr;
	Int_t    ndf;
	
	if(gain=="high") hmips->Rebin(2);

	
	if(hmips->Integral()>100){
	  hmips->GetXaxis()->SetRangeUser(3, 200);

	  if(gain=="high") {
	    fr[0]=TMath::Max(hmips->GetMean()-hmips->GetRMS(),10.);
	    hmips->GetXaxis()->SetRangeUser(fr[0], 200);
	    fr[1]=hmips->GetMean()*1.1;
	  } else {
	    fr[0]=TMath::Max(hmips->GetMean()-2.*hmips->GetRMS(),1.);
	    hmips->GetXaxis()->SetRangeUser(fr[0], 25.);
	    fr[1]=hmips->GetMean()*1.1 + 2.*hmips->GetRMS();
	  }

	  TF1 *fitlandau= new TF1("fitlandau","landau",fr[0],fr[1]);
	  hmips->Fit("fitlandau","QRE");
	  hmips->GetXaxis()->SetRangeUser(fr[0], fr[1]);

	  sv[0] = TMath::Min(TMath::Max(fitlandau->GetParameter(2),pllo[0]),plhi[0]);
	  sv[1] = TMath::Min(TMath::Max(fitlandau->GetParameter(1),pllo[1]),plhi[1]);
	  sv[2] = hmips->Integral("width") * 1.2;
	  sv[3] = TMath::Min(TMath::Max(hmips->GetRMS()* 0.1,pllo[3]),plhi[3]);
	  
	  TF1 *fitsnr_hmips=langaufit(hmips,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
	  for (int k = 0; k < 4; k++) {
	    if ((sv[k] >= pllo[k]) && (sv[k] <= plhi[k])) continue;
	    std::cout << "Langaus fit parameter " << k << " has starting value " << sv[k]
		      <<  " outside the range [" << pllo[k] << ", " << plhi[k] << "]!" << std::endl;
	  }
	  double mpv=fitsnr_hmips->GetParameter(1);
	  double empv=fitsnr_hmips->GetParError(1);
	  double wmpv=fitsnr_hmips->GetParameter(0);
	  double chi2ndf=0;
	  if(ndf>0) chi2ndf=chisqr/ndf;
	  float mpvmin=15;
	  if(gain=="low") mpvmin=0.8;
	  if(chi2ndf>0 && mpv>mpvmin) {
	    avmpv[i]+=mpv;
	    rmsmpv[i]+=mpv*mpv;
	    nmpv[i]++;
	  }
	  fout_mip<<layer<<" "<<i<<" "<<j<<" "<<mpv<<" "<<empv<<" "<<wmpv<<" "<<chi2ndf<<" "<<hmips->GetEntries()<<"\n";
	  mpv_all->Fill(20*layer+i,j,mpv);
	  width_all->Fill(20*layer+i,j,wmpv);
	  nentries_all->Fill(20*layer+i,j,hmips->GetEntries());

	} else {
	  fout_mip<<layer<<" "<<i<<" "<<j<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<"\n";
	}
	_file1->cd();
        hmips->SetName(TString::Format("mip_%s_layer%i_chip%i_chn%i",gain.Data(),layer,i,j));
        hmips->SetTitle(TString::Format("mip_%s_layer%i_chip%i_chn%i",gain.Data(),layer,i,j));
        hmips->Write();

	delete hmips;
      }
      //  file->cd();
      //canvas_mip->Print(TString::Format("plots/%s_layer_%i_chip%i.png",run.Data(),layer,i));
      //  canvas_mip->Write();
      //delete canvas_mip;
    }
    for(int i=0;i<nchips;i++){
      if(nmpv[i]!=0 ) {
    	avmpv[i]/=nmpv[i];
    	rmsmpv[i]=sqrt( (rmsmpv[i]-nmpv[i]*avmpv[i]*avmpv[i]) / (nmpv[i]-1) );
    	MIPM_all->Fill(layer,i,avmpv[i]);
    	MIPrms_all->Fill(layer,i,rmsmpv[i]);
    	MIPN_all->Fill(layer,i,nmpv[i]);
      }
   
    }


  }

 
  TCanvas* canvassummary= new TCanvas("MIPAna","MIPAna",1600,800);
  canvassummary->Divide(3,1);
    
  canvassummary->cd(1);
  // MIPM_all->GetXaxis()->SetTitle("x");
  // MIPM_all->GetYaxis()->SetTitle("y");
  if(gain=="high")  MIPM_all->GetZaxis()->SetRangeUser(0,100);
  else MIPM_all->GetZaxis()->SetRangeUser(0,15);
  MIPM_all->Draw("colz");

  canvassummary->cd(2);
  // MIPrms_all->GetXaxis()->SetTitle("x");
  // MIPrms_all->GetYaxis()->SetTitle("y");
  if(gain=="high") MIPrms_all->GetZaxis()->SetRangeUser(0,10);
  else MIPrms_all->GetZaxis()->SetRangeUser(0,5);
  MIPrms_all->Draw("COLZ");
    
  canvassummary->cd(3);
  //gPad->SetLogz();
  // MIPN_all->GetXaxis()->SetTitle("x");
  // MIPN_all->GetYaxis()->SetTitle("y");
  MIPN_all->GetZaxis()->SetRangeUser(0,64.5);
  MIPN_all->Draw("COLZ");
  
  _file1->cd();
  canvassummary->Write();
  //  hmips->Draw();
  mpv_all->Write();
  width_all->Write();
  nentries_all->Write();
  _file1->Write();
  _file1->Close();

  delete canvassummary;
}


 void s_n_analysis_summary(TString run="3GeVMIPscan", TString gain="high"){
  
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


  TH2F* SNM_all=new TH2F("SNM_all","average of S/N ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);
  TH2F* SNN_all=new TH2F("SNN_all","channels fitted ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);
  TH2F* SNrms_all=new TH2F("SNrms_all","rms  of S/N ; Layer  ; Chip",15,-0.5,14.5,16,-0.5,15.5);

  TH2F* mpv_all=new TH2F("mpv_all","S/N mpv pos. ; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);
  TH2F* width_all=new TH2F("width_all","width of Landau S/N ; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);
  TH2F* nentries_all=new TH2F("nentries_all","N entries; Layer*20+Chip  ; chn",300,-0.5,299.5,64,-0.5,63.5);

  ofstream fout_mip(TString::Format("../../mip_calib/SN_%s_%sgain.txt",run.Data(),gain.Data()).Data(),ios::out);
  fout_mip<<"#mip results PROTO15-TB2021-11"<<endl;
  fout_mip<<"#layer chip channel sn_mpv sn_empv sn_widthmpv chi2ndf nentries"<<endl;

  ReadPedestalsProto(TString::Format("../../pedestals/Pedestal_%s_%sgain.txt",run.Data(),gain.Data()).Data(),false);

  TFile *_file1 = new TFile(TString::Format("../../mip_calib/%s_%sgain_SNSummary.root",run.Data(),gain.Data()),"RECREATE");


  for(int layer=0; layer<nlayers; layer++) {

    TFile *_file0 = TFile::Open(TString::Format("../results_calib/PedestalMIP_%s.root",run.Data()));
     
    float avmpv[16]={0};
    float nmpv[16]={0};
    float rmsmpv[16]={0};

    cout<<"   LAYER : "<<layer<<endl;


    for(int i=0;i<nchips;i++){
      
      for(int j=0; j<64; j++) {
	TH1F *hmips = new TH1F("hmips","hmips",1000,-19.95,80.05);
	hmips->Sumw2();
	for(int isca=0; isca<15; isca++) {
	  _file0->cd();
	  //TCanvas* canvashmips= new TCanvas("canvashmips","canvashmips",100,100);
	  //canvashmips->cd();
	  TH1F *temp=(TH1F*)_file0->Get(TString::Format("layer_%i/mip_%s_chip%i_chn%i_sca%i",layer,gain.Data(),i,j,isca));
	  
	  if(temp==NULL) continue;

	  for (int k=1;k<500;k++) {
	    double y = temp->GetBinContent(k);
	    double x = (temp->GetXaxis()->GetBinCenter(k) -  ped_mean_slboard.at(layer).at(i).at(j).at(isca))/ped_width_slboard.at(layer).at(i).at(j).at(isca);
	    if(y>0 && ped_mean_slboard.at(layer).at(i).at(j).at(isca)>0) hmips->Fill(x,y);
	  }
	  // delete canvashmips;
	}

	for(int k=0; k<600; k++) hmips->SetBinError(k,sqrt(hmips->GetBinContent(k)));


	Double_t fr[2];
	Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];
	pllo[0]=0.05; pllo[1]=1.5; pllo[2]=1.0; pllo[3]=0.05;
	plhi[0]=1.0; plhi[1]=30.0; plhi[2]=100000000.0; plhi[3]=3.0;

	Double_t chisqr;
	Int_t    ndf;
	

	if(gain=="low") hmips->Rebin(8);
	else hmips->Rebin(4);

	if(hmips->Integral()>100){

	  hmips->GetXaxis()->SetRangeUser(0.5, 50);

	  fr[0]=TMath::Max(hmips->GetMean()-hmips->GetRMS(),1.);
	  hmips->GetXaxis()->SetRangeUser(fr[0], 50);
	  fr[1]=hmips->GetMean()*1.1 + 2.*hmips->GetRMS();
	  

	  TF1 *fitlandau= new TF1("fitlandau","landau",fr[0],fr[1]);
	  hmips->Fit("fitlandau","QRE");
	  hmips->GetXaxis()->SetRangeUser(fr[0], fr[1]);


	  sv[0] = TMath::Min(TMath::Max(fitlandau->GetParameter(2),pllo[0]),plhi[0]);
	  sv[1] = TMath::Min(TMath::Max(fitlandau->GetParameter(1),pllo[1]),plhi[1]);
	  sv[2] = hmips->Integral("width") * 1.2;
	  sv[3] = TMath::Min(TMath::Max(hmips->GetRMS()* 0.1,pllo[3]),plhi[3]);
	  
	  TF1 *fitsnr_hmips=langaufit(hmips,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
	  for (int k = 0; k < 4; k++) {
	    if ((sv[k] >= pllo[k]) && (sv[k] <= plhi[k])) continue;
	    std::cout << "Langaus fit parameter " << k << " has starting value " << sv[k]
		      <<  " outside the range [" << pllo[k] << ", " << plhi[k] << "]!" << std::endl;
	  }
	  double mpv=fitsnr_hmips->GetParameter(1);
	  double empv=fitsnr_hmips->GetParError(1);
	  double wmpv=fitsnr_hmips->GetParameter(0);
	  double chi2ndf=0;
	  if(ndf>0) chi2ndf=chisqr/ndf;
	  float mpvmin=2;
	  if(chi2ndf>0 && mpv>mpvmin) {
	    avmpv[i]+=mpv;
	    rmsmpv[i]+=mpv*mpv;
	    nmpv[i]++;
	  }
	  fout_mip<<layer<<" "<<i<<" "<<j<<" "<<mpv<<" "<<empv<<" "<<wmpv<<" "<<chi2ndf<<" "<<hmips->GetEntries()<<"\n";
	  mpv_all->Fill(20*layer+i,j,mpv);
	  width_all->Fill(20*layer+i,j,wmpv);
	  nentries_all->Fill(20*layer+i,j,hmips->GetEntries());

	} else {
	  fout_mip<<layer<<" "<<i<<" "<<j<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<"\n";
	}
	_file1->cd();
        hmips->SetName(TString::Format("sn_%s_layer%i_chip%i_chn%i",gain.Data(),layer,i,j));
        hmips->SetTitle(TString::Format("sn_%s_layer%i_chip%i_chn%i",gain.Data(),layer,i,j));
        hmips->Write();

	delete hmips;
      }
      //  file->cd();
      //canvas_mip->Print(TString::Format("plots/%s_layer_%i_chip%i.png",run.Data(),layer,i));
      //  canvas_mip->Write();
      //delete canvas_mip;
    }
    for(int i=0;i<nchips;i++){
      if(nmpv[i]!=0 ) {
    	avmpv[i]/=nmpv[i];
    	rmsmpv[i]=sqrt( (rmsmpv[i]-nmpv[i]*avmpv[i]*avmpv[i]) / (nmpv[i]-1) );
    	SNM_all->Fill(layer,i,avmpv[i]);
    	SNrms_all->Fill(layer,i,rmsmpv[i]);
    	SNN_all->Fill(layer,i,nmpv[i]);
      }
   
    }


  }

 
  TCanvas* canvassummary= new TCanvas("SNAna","SNAna",1600,800);
  canvassummary->Divide(3,1);
    
  canvassummary->cd(1);
  // SNM_all->GetXaxis()->SetTitle("x");
  // SNM_all->GetYaxis()->SetTitle("y");
  if(gain=="high")  SNM_all->GetZaxis()->SetRangeUser(0,40);
  else SNM_all->GetZaxis()->SetRangeUser(0,35);
  SNM_all->Draw("colz");

  canvassummary->cd(2);
  // SNrms_all->GetXaxis()->SetTitle("x");
  // SNrms_all->GetYaxis()->SetTitle("y");
  if(gain=="high") SNrms_all->GetZaxis()->SetRangeUser(0,10);
  else SNrms_all->GetZaxis()->SetRangeUser(0,5);
  SNrms_all->Draw("COLZ");
    
  canvassummary->cd(3);
  //gPad->SetLogz();
  // SNN_all->GetXaxis()->SetTitle("x");
  // SNN_all->GetYaxis()->SetTitle("y");
  SNN_all->GetZaxis()->SetRangeUser(0,64.5);
  SNN_all->Draw("COLZ");
  
  _file1->cd();
  canvassummary->Write();
  //  hmips->Draw();
  mpv_all->Write();
  width_all->Write();
  nentries_all->Write();
  _file1->Write();
  _file1->Close();

  delete canvassummary;
  
}



