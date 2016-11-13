#include <TTree.h>

TTree* calib(TTree * data_no_calib)
{
  //to take from Ivan analysis
  Double_t R1_m   = 1.152;
  Double_t R1_q   = 8.441;
  Double_t R2_m   = 0.9551;
  Double_t R2_q   = 0.7332;
  Double_t time_m = 40.32;
  Double_t time_q = -91.27;
  
  
  TBranch * ch0 = data_no_calib->GetBranch("ch0");
  TBranch * ch1 = data_no_calib->GetBranch("ch1");
  TBranch * ch2 = data_no_calib->GetBranch("ch2");
  TBranch * ch3 = data_no_calib->GetBranch("ch3");
  if(ch0 == 0 || ch1 == 0 || ch2 == 0 || ch3 == 0)
  {
    cout << "BRANCHES NOT READ! CAN'T CALIBRATE" << endl;
    return 0;
  }
  
  
  
  Int_t entries_ch0 = ch0->GetEntries();
  Int_t entries_ch1 = ch1->GetEntries();
  Int_t entries_ch2 = ch2->GetEntries();
  Int_t entries_ch3 = ch3->GetEntries();
  if(entries_ch0 != entries_ch1 || entries_ch0 != entries_ch2 || entries_ch0 != entries_ch3)
  {
    cout << "RATE NOT EQUAL IN ALL CHANNELS, CHECK DATA" << endl;
    return 0;
  }
  
  
  Float_t temp0, temp0cal;
  Float_t temp1, temp1cal;
  Float_t temp2, temp2cal;
  Float_t temp3, temp3cal;
  
  //creating output variables
  TTree * calibrated = new TTree("calibrated", "calibrated");
  calibrated->Branch("ch0cal" , "ch0cal/F", &temp0cal);
  calibrated->Branch("ch1cal" , "ch1cal/F", &temp1cal);
  calibrated->Branch("ch2cal" , "ch2cal/F", &temp2cal);
  calibrated->Branch("ch3cal" , "ch3cal/F", &temp3cal);
  
  ch0->SetAddress(&temp0);
  ch1->SetAddress(&temp1);
  ch2->SetAddress(&temp2);
  ch3->SetAddress(&temp3);
  
  for(int i = 0; i < entries_ch0; i++)
  { 
    data_no_calib->GetEntry(i);
    
    temp0cal = temp0;				//trigger doesn't change
    temp1cal = time_m   * temp1 + time_q;	//time calibration
    temp2cal = R1_m     * temp2 + R1_q;		//energy calibration of R1
    temp3cal = R2_m     * temp3 + R2_q;		//energy calibration on R2
    
    calibrated->Fill();
  }
  
  calibrated->Write();
  
  return calibrated;
}
