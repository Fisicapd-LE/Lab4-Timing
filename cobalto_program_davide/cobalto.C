#include <iostream>
#include <TList.h>
#include <TH1F.h>
#include <vector>
#include "calib.C"

TTree* calib(TTree* data_no_calib);

int cobalto()
{
  using namespace std;
  TFile* Input = TFile::Open("Co_moreamplified.root", "READ");
  
  if (!Input) {
    cout << "Error; data not found" << endl;
    return -1;
  }
  
  TTree* data_tree = (TTree*) Input->Get("pjmca");
  TTree* calibrated_tree = calib(data_tree);
  
  if(calibrated_tree == 0)
  {
    cout << "Error: calibration non done" << endl;
    return -1;
  }
  
  
  TFile *fout = new TFile("Co_analysis.root","RECREATE"); // output file
  
  //threshold
  Double_t min_thresh = 100;
  Double_t max_thresh = 1000;
  Double_t step_thresh = 100;
  
  TList threshold_list;
  TH1F * threshold_histo;
  
  for (int i = min_thresh; i < max_thresh; i += step_thresh)
  {
    calibrated_tree->Draw("ch_1>>threshold_histo", "ch2 + ch3 < 2 * i", "goff");
    threshold_list.Add(threshold_histo);
  }
  threshold_list.Write();
  
  //interval
  Double_t min_interv = 50;
  Double_t max_interv = 950;
  Double_t step_interv = 100;
  
  TList interval_list;
  TH1F * interval_histo;
  
  for (int i = min_interv; i < max_interv; i += step_interv)
  {
    calibrated_tree->Draw("ch_1>>calibrated_histo", "ch2 + ch3 > 2 * i && ch2 + ch3 < 2 * (i + step_interv)", "goff");
    interval_list.Add(interval_histo);
  }
  interval_list.Write();
  
  calibrated_tree->Write();
  
  fout->Close();
  
  return 0;
}
