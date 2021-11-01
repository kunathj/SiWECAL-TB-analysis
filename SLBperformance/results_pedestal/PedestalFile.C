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

//#include "../../style/Style.C"
//#include "../../style/Labels.C"

void PedestalFile(TString rootfile="") {
  int min_entries_in_peak_bin = 5;
  int n_slb = 15;
  int n_chips = 16;
  int n_channels = 64;
  int n_scas = 15;
  int current_iter = 0;
  int n_iterations = n_slb * n_chips * n_channels * n_scas;
  std::string progress_bar;
  double bar_width = 30;
  int delta_progress_bar = 1000;

  std::ofstream fout_ped(rootfile+".txt",ios::out);
  fout_ped << "#pedestal results (fit to a gaussian) remove channels/sca with two pedestals peaks from the analysis : PROTO15" << std::endl;
  fout_ped << "#layer chip channel ped0 eped0 widthped0 ped1 eped1 widthped1... ped14 eped14 widhtped14 (all SCA)" << std::endl;

  TFile *_file0 = TFile::Open(rootfile+".root");
  for (int i_layer = 0; i_layer < n_slb; i_layer++) {
    for (int i_chip = 0; i_chip < n_chips; i_chip++) {
      for (int i_channel = 0; i_channel < n_channels; i_channel++) {
        fout_ped << i_layer<< " " << i_chip << " " << i_channel << " ";
        for (int i_sca = 0; i_sca < n_scas; i_sca++) {
          if (bar_width * current_iter++ / n_iterations > progress_bar.length()) {
            progress_bar.push_back('#');
          }
          std::cout << std::flush << "Pedestal fit "
                    << " [" << std::left << std::setw(bar_width) << progress_bar << "] "
                    << std::right << std::setw(2) << i_sca << "/" << n_scas << " SCAs, " 
                    << std::right << std::setw(2) << i_channel << "/" << n_channels << " channels, " 
                    << std::right << setw(2) << i_chip << "/" << n_chips << " chips, "
                    << std::right << setw(2) << i_layer << "/" << n_slb << " layers\r";
          TH1F* h_temp = (TH1F*)_file0->Get(
              TString::Format("layer_%i/ped_chip%i_chn%i_sca%i", i_layer, i_chip, i_channel, i_sca));

          if (h_temp->GetEntries() < 5 * min_entries_in_peak_bin) {
            fout_ped << 0 << " " << 0 << " " << 0 << " ";
            continue;
          }

          TSpectrum *s = new TSpectrum();
          int n_peaks = s->Search(h_temp, 2, "", 0.8);
          if (n_peaks == 0) {
            fout_ped << 0 << " " << 0 << " " << 0 << " ";
            delete s;
            continue;
          }

          Double_t *mean_peak = s->GetPositionX();
          Double_t *mean_high = s->GetPositionY();
          double mean_peak_highest = 0;
          double mean_high_highest = 0;
          int n_peak_max = 0;
          for (int i_peak = 0; i_peak < n_peaks; i_peak++) {
            if (mean_high[i_peak] > mean_high_highest) {
              mean_high_highest = mean_high[i_peak];
              mean_peak_highest = mean_peak[i_peak];
              n_peak_max = i_peak;
            }
          }
          if (mean_high_highest < min_entries_in_peak_bin) {
            fout_ped << 0 << " " << 0 << " " << 0 << " ";
            delete s;
            continue;
          }
          if (n_peaks > 1) {  
            // Jonas: Don't know why we would not do the fit like for n_peaks == 0,
            // but I'll stick with what was in the code so far.
            fout_ped << mean_peak_highest << " " << 10 << " " << 0 << " ";
            delete s;
            continue;
          }
          TF1 *f0 = new TF1("f0", "gaus" , mean_peak_highest - h_temp->GetRMS(), mean_peak_highest + h_temp->GetRMS());
          h_temp->Fit("f0", "RQNOC");

          bool has_peak_outside_window = fabs(f0->GetParameter(1) - mean_peak[0]) > h_temp->GetRMS();
          bool has_few_entries_in_fit_region = (
            h_temp->Integral(h_temp->FindFixBin(f0->GetParameter(1) - f0->GetParameter(2)), 
                             h_temp->FindFixBin(f0->GetParameter(1) + f0->GetParameter(2)))
            < 5 * min_entries_in_peak_bin
          );
          if (has_peak_outside_window || has_few_entries_in_fit_region) {
            // Jonas: The fit went terribly wrong - discard it.
            fout_ped << 0 << " " << 0 << " " << 0 << " ";
            delete s;
            continue;
          }
          TF1 *f1 = new TF1("f1", "gaus", f0->GetParameter(1) - f0->GetParameter(2), f0->GetParameter(1) + f0->GetParameter(2));
          h_temp->Fit("f1", "RQME");
          fout_ped << f1->GetParameter(1) << " " << f1->GetParError(1) << " " << f1->GetParameter(2) << " ";
          delete s;
        }
        fout_ped << std::endl;
      }
    }
  }
}
