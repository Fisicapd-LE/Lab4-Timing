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
  
  
  TFile *fout = new TFile("Co_analysis.root","RECREATE"); // output file
  TTree* calibrated_tree = calib(data_tree);
  
  if(calibrated_tree == 0)
  {
    cout << "Error: calibration non done" << endl;
    return -1;
  }
  
  //threshold
  Double_t min_thresh = 100;
  Double_t max_thresh = 1000;
  Double_t step_thresh = 100;
  
  TList threshold_list;
  TH1F * threshold_histo = new TH1F("test", "", 10,0,100);
  
  for (int i = min_thresh; i < max_thresh; i += step_thresh)
  {
  	string cut = "ch2cal + ch3cal < 2 * #i";
  	cut.replace(cut.find("#i"), 2, to_string(i));
    calibrated_tree->Draw("ch1cal>>threshold_histo", cut.c_str(), "goff");
    threshold_list.Add(threshold_histo->Clone());
  }
  threshold_list.Write();
  
  //interval
  Double_t min_interv = 50;
  Double_t max_interv = 950;
  Double_t step_interv = 100;
  
  TList interval_list;
  TH1F * interval_histo = new TH1F("test2", "", 10,0,100);
  
  for (int i = min_interv; i < max_interv; i += step_interv)
  {
  	string cut = "ch2cal + ch3cal > 2 * #i && ch2cal + ch3cal < 2 * (#i + #step_interv)";
  	cut.replace(cut.find("#i"), 2, to_string(i));
  	cut.replace(cut.find("#i"), 2, to_string(i));
  	cut.replace(cut.find("#step_interv"), 12, to_string(step_interv));
    calibrated_tree->Draw("ch1cal>>interval_histo", cut.c_str(), "goff");
    interval_list.Add(interval_histo->Clone());
  }
  interval_list.Write();
  
  calibrated_tree->Write();
  
  fout->Close();
  
  return 0;
}
