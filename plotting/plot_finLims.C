/********************************************************************
// John Williamson, 22/03/22
plot_finLims.C

plot epsilon and upper limit for each window size

********************************************************************/


#include "CsvParser.h"


// scale epsilon squared value based on upper limit
std::vector<Double_t> scaleEpsilon(std::vector<Double_t> epsilon_up, std::vector<Double_t> mu_up, std::vector<Double_t> mu_alt){

  std::vector<Double_t> epsilon_alt;
  
  for(Int_t i = 0; i < epsilon_up.size(); i++){

    epsilon_alt.push_back((mu_alt[i]/mu_up[i])*epsilon_up[i]);
    
  }

  return epsilon_alt;
}



TGraph* shadeGraph(vector<Double_t> vx, vector<Double_t> vUp, vector<Double_t> vLow, Int_t n, Int_t colour = kGreen+1){

  TGraph* grshade = new TGraph(2*n);
  for (Int_t i=0;i<n;i++) {
    grshade->SetPoint(i,vx[i],vUp[i]);
    grshade->SetPoint(n+i,vx[n-i-1],vLow[n-i-1]);
  }

  grshade->SetFillStyle(1001);
  grshade->SetFillColor(colour);
  //grshade->SetFillColorAlpha(colour,0.15);

  return grshade;
}

std::vector<Double_t> readFile(Double_t mass, Int_t POSet = 3/*Polynomial order for background*/,  Int_t NSig = 0, Int_t MBg = -1/*background model choicde*/, Double_t BinSize = 0, Int_t WSize = 0){

  Int_t TPOSet = POSet;

  TString CsvName;
  if(ceilf(mass) == mass){
      CsvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,(int)mass,POSet,TPOSet,NSig);
    }
    else{
      CsvName = Form("csv/%.2fBin_%iM_%.2fm_%ip_%iP_%is_results.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);
    }

  // TString CsvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);
  
  CsvParser massOut(CsvName.Data());
  massOut.SortByColumn(1);
  

  std::vector<string> WF_s = massOut.GetColumn(1);
  std::vector<string> PO_s = massOut.GetColumn(2);
  std::vector<string> Yield_s = massOut.GetColumn(4);
  std::vector<string> YieldErr_s = massOut.GetColumn(5);
  std::vector<string> Pull_s = massOut.GetColumn(6);
  std::vector<string> PullErr_s = massOut.GetColumn(7);
  std::vector<string> BgChi2_s = massOut.GetColumn(8);
  std::vector<string> BgSigChi2_s = massOut.GetColumn(9);
  std::vector<string> PVal_s = massOut.GetColumn(11);
  std::vector<string> Epsilon_s = massOut.GetColumn(12);
  std::vector<string> UpperLim_s = massOut.GetColumn(13);
  std::vector<string> ToyUpMed_s = massOut.GetColumn(15);
  std::vector<string> ToyUpN2_s = massOut.GetColumn(16);
  std::vector<string> ToyUpN1_s = massOut.GetColumn(17);
  std::vector<string> ToyUpP1_s = massOut.GetColumn(18);
  std::vector<string> ToyUpP2_s = massOut.GetColumn(19);

  Double_t Pull = 0;
  Double_t PullErr = 0;
  Double_t BgChi2 = 0;
  Double_t PVal = 0;
  Double_t Epsilon = 0;
  Double_t UpperLim = 0;
  Double_t ToyUpMed = 0;
  Double_t ToyUpN2 = 0;
  Double_t ToyUpN1 = 0;
  Double_t ToyUpP1 = 0;
  Double_t ToyUpP2 = 0;
  

  Int_t WF_read = -1; // valur of WF read from csv
  Int_t i = 0;
  while(WF_read != WSize && i<WF_s.size()){
    WF_read = stoi(WF_s[i]);
    i++;
  }

  if(WF_read == WSize){
    Pull = stod(Pull_s[i]);
    PullErr = stod(PullErr_s[i]);
    BgChi2 = stod(BgChi2_s[i]);
    PVal = stod(PVal_s[i]);
    Epsilon = stod(Epsilon_s[i]);
    UpperLim = stod(UpperLim_s[i]);
    ToyUpMed = stod(ToyUpMed_s[i]);
    ToyUpN2 = stod(ToyUpN2_s[i]);
    ToyUpN1 = stod(ToyUpN1_s[i]);
    ToyUpP1 = stod(ToyUpP1_s[i]);
    ToyUpP2 = stod(ToyUpP2_s[i]);
  }

  
  return vector<Double_t> {Pull,PullErr,BgChi2,PVal,Epsilon,UpperLim,ToyUpMed,ToyUpN2,ToyUpN1,ToyUpP1,ToyUpP2};


}


// rules for getting window size from mass
Int_t getWindowSize(Double_t mass){

  Int_t wSize = 5;

  if(mass<144){
    wSize = 10;
  }
  else if(mass>=144 && mass<165){
    wSize = 11;
  }
  else if(mass>=165 && mass<185){
    wSize = 12;
  }
  else if(mass>=185 && mass<205){
    wSize = 12;
  }
  else if(mass>=205 && mass<216){
    wSize = 12;
  }
  else if(mass>=216){
    wSize = 11;
  }

  return wSize;

}




void plot_finLims(Int_t POSet = 3/*Polynomial order for background*/, Double_t BinSize = 0.15, Int_t NSig = 0, Int_t MBg = 3/*background model choicde*/){

  // first plot window sizes from csv file (largest, smallest and chosen for each mass)
  
  // read csv file with results from mass hypo
  
  TString WCsvName = "params/test_params.csv";
  
  CsvParser massOut(WCsvName.Data());
  //massOut.SortByColumn(0);
  
  std::vector<string> Mass_s = massOut.GetColumn(0);
  std::vector<Int_t> Mass;
  std::vector<string> SWin_s = massOut.GetColumn(1);
  std::vector<Int_t> SWin;
  std::vector<string> LWin_s = massOut.GetColumn(2);
  std::vector<Int_t> LWin;
  std::vector<string> MWin_s = massOut.GetColumn(3);
  std::vector<Int_t> MWin;


  for(Int_t i = 0; i<Mass_s.size(); i++){
    Mass.push_back(stoi(Mass_s[i]));
    SWin.push_back(stoi(SWin_s[i]));
    LWin.push_back(stoi(LWin_s[i]));
    MWin.push_back(stoi(MWin_s[i]));

    
    //    cout << i << "th entry: Mass = " << Mass[i] << ", SWin = " << SWin[i] << ", LWin = " << LWin[i] << ", MWin = " << MWin[i] << endl << endl;

  }


  
  // plot those numbers
  Int_t n = Mass_s.size();

  TCanvas* c1 = new TCanvas("c1","c1",1500,1000);
  // c1->SetLogy();
  // c1->SetLeftMargin(0.12);
  // //  c1->SetTopMargin(0.05);
  TGraph* gLWin = new TGraph(n,&Mass[0],&LWin[0]);
  gLWin->SetTitle("");
  gLWin->GetXaxis()->SetTitle("Mass (MeV)");
  gLWin->GetXaxis()->CenterTitle();
  gLWin->GetXaxis()->SetTitleOffset(1.2);
  gLWin->GetYaxis()->SetTitle("Window Size (n_{#sigma})");
  gLWin->GetYaxis()->CenterTitle();
  // gLWin->SetMarkerStyle(8); 
  // gLWin->SetMarkerColor(kRed);
  // gLWin->SetLineColor(kRed);
  // gLWin->GetYaxis()->SetTitleOffset(1.6);
  
  gLWin->Draw("APL");
  gLWin->SetMinimum(4);
    //  gUpperLim->GetYaxis()->SetRangeUser(*std::min_element(ToyUpN2.begin(),ToyUpN2.end()),std::max(*std::max_element(ToyUpP2.begin(),ToyUpP2.end())*1.4,gUpperLim->GetMaximum()*1.4));


  TGraph* gMWin = new TGraph(n,&Mass[0],&MWin[0]);
  gMWin->SetLineColor(kBlue);
  gMWin->Draw("PL same");

  TGraph* gSWin = new TGraph(n,&Mass[0],&SWin[0]);
  gSWin->SetLineColor(kRed);
  gSWin->Draw("PL same");


  TLegend* legLimA = new TLegend(0.10,0.70,0.35,0.9);
  legLimA->SetTextSize(0.025);
  legLimA->AddEntry(gLWin,"Largets Window Size","l");
  legLimA->AddEntry(gSWin,"Smallest Window Size","l");
  legLimA->AddEntry(gMWin,"'Optimal' Window Size","l");
  legLimA->Draw();



  TCanvas* c2 = new TCanvas("c2","c2",1500,1000);

  gMWin->Draw("APL");
  gMWin->SetTitle("");
  gMWin->GetXaxis()->SetTitle("Mass (MeV)");
  gMWin->GetXaxis()->CenterTitle();
  gMWin->GetXaxis()->SetTitleOffset(1.2);
  gMWin->GetYaxis()->SetTitle("Window Size (n_{#sigma})");
  gMWin->GetYaxis()->CenterTitle();


  // define window sizes

  std::vector<Int_t> TWin;
  for(Int_t i = 0; i<n; i++){    
    TWin.push_back(getWindowSize(Mass[i]));
  }
  
  TGraph* gTWin = new TGraph(n,&Mass[0],&TWin[0]);
  gTWin->SetLineColor(kGreen+2);
  gTWin->Draw("PL same");

  TLegend* legLimM = new TLegend(0.10,0.70,0.35,0.9);
  legLimM->SetTextSize(0.025);
  legLimM->AddEntry(gMWin,"'Optimal' Window Size","l");
  legLimM->AddEntry(gTWin,"Chosen Window Size","l");
  legLimM->Draw();



  // now plot upper limits for chosen window sizes
  
  Int_t TPOSet = POSet;  

  

  Double_t lowMass = 130.0;
  Double_t upMass = 220.00;

  std::vector<Double_t> Masses;
  Double_t mass = lowMass;
  while(mass<=upMass){
    Masses.push_back(mass);
    mass += 0.25;
  }

  
  std::vector<Double_t> MassD;
  std::vector<Double_t> Pull;
  std::vector<Double_t> PullErr;
  std::vector<Double_t> BgChi2;
  std::vector<Double_t> PVal;
  std::vector<Double_t> Epsilon;
  std::vector<Double_t> UpperLim;
  std::vector<Double_t> ToyUpMed;
  std::vector<Double_t> ToyUpN2;
  std::vector<Double_t> ToyUpN1;
  std::vector<Double_t> ToyUpP1;
  std::vector<Double_t> ToyUpP2;

  
  
  for (auto & massVal : Masses) {

    Int_t WSize = getWindowSize(massVal);
    
    TString CsvName;
    if(ceilf(massVal) == massVal){
      CsvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,(int)massVal,POSet,TPOSet,NSig);
    }
    else{
      CsvName = Form("csv/%.2fBin_%iM_%.2fm_%ip_%iP_%is_results.csv",BinSize,MBg,massVal,POSet,TPOSet,NSig);
    }

    if(gSystem->AccessPathName(CsvName)){
      continue;
    }
    else{
      std::vector<Double_t> results = readFile(massVal, POSet, NSig, MBg, BinSize, WSize);
      if(results[4] == 0){
	continue;
      }
      
      MassD.push_back((Double_t)massVal);
      Pull.push_back(results[0]);
      Epsilon.push_back(results[4]);
      UpperLim.push_back(results[5]);
      ToyUpMed.push_back(results[6]);
      ToyUpN2.push_back(results[7]);
      ToyUpN1.push_back(results[8]);
      ToyUpP1.push_back(results[9]);
      ToyUpP2.push_back(results[10]);      
    }
  }
  

  // cout << "Pull" << endl;
  // for (auto & element : Pull) {
  //   cout << "pull = " << element << endl;
  // }




  TCanvas* c4 = new TCanvas("c4","c4",1500,1000);
  c4->SetLogy();
  c4->SetLeftMargin(0.12);
  TGraph* gUpperLim = new TGraph(MassD.size(),&(MassD[0]),&(UpperLim[0]));
  gUpperLim->SetTitle("");
  gUpperLim->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (MeV/c^{2})");
  gUpperLim->GetXaxis()->CenterTitle();
  gUpperLim->GetXaxis()->SetTitleOffset(1.2);
  gUpperLim->GetYaxis()->SetTitle("Signal Upper Limit");
  gUpperLim->GetYaxis()->CenterTitle();
  gUpperLim->GetYaxis()->SetRangeUser(*std::min_element(ToyUpN2.begin(),ToyUpN2.end()),std::max(*std::max_element(ToyUpP2.begin(),ToyUpP2.end())*1.4,gUpperLim->GetMaximum()*1.4));
  gUpperLim->SetMarkerStyle(8); 
  gUpperLim->SetMarkerColor(kBlack);
  gUpperLim->SetLineColor(kBlack);
  gUpperLim->SetLineWidth(2);
  gUpperLim->Draw("AL");

  
  TGraph* gToyUpP2 = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpP2[0]));  
  TGraph* gToyUpP1 = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpP1[0]));
  TGraph* gToyUpMed = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpMed[0]));
  TGraph* gToyUpN1 = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpN1[0]));
  TGraph* gToyUpN2 = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpN2[0]));

  gToyUpMed->SetLineStyle(7);
  gToyUpMed->SetLineWidth(3);
  
  // gToyUpP2->Draw("l");
  // gToyUpP1->Draw("l");
  gToyUpMed->Draw("l");
  // gToyUpN1->Draw("l");
  // gToyUpN2->Draw("l");
  

  Int_t N = MassD.size();
  
  auto grshP2 = shadeGraph(MassD,ToyUpP2,ToyUpP1,N,kYellow-4);
  auto grshP1 = shadeGraph(MassD,ToyUpP1,ToyUpMed,N);
  auto grshN1 = shadeGraph(MassD,ToyUpMed,ToyUpN1,N);
  auto grshN2 = shadeGraph(MassD,ToyUpN1,ToyUpN2,N,kYellow-4);
  grshP2->Draw("f");
  grshP1->Draw("f");
  grshN1->Draw("f");
  grshN2->Draw("f");
  

  gToyUpMed->Draw("l same");
  gUpperLim->Draw("L same");

  TLegend* legLim = new TLegend(0.75,0.70,0.90,0.9);
  // legLim->SetNColumns(2);
  legLim->SetTextSize(0.025);
  legLim->AddEntry(gUpperLim,"#mu_{up}","l");
  legLim->AddEntry(gToyUpMed,"#mu_{med}","l");
  legLim->AddEntry(grshP1,"#pm 1#sigma CI","f");
  legLim->AddEntry(grshP2,"#pm 2#sigma CI","f");
  legLim->Draw();
  
  // auto *mg = new TMultiGraph();
  // mg->Add(gToyUpN1);
  // mg->Add(gToyUpN2);
  // mg->Draw("ALF same");
  
  
  
  // TGraph* gToyUpMed = new TGraph(MassD.size(),&(MassD[0]),&(ToyUpMed[0]));
  // gToyUpMed->Draw("FPL same");

  TCanvas* c3 = new TCanvas("c3","c3",1500,1000);
  c3->SetLogy();
  c3->SetLeftMargin(0.12);
  
  TGraph* gEpsilon = new TGraph(MassD.size(),&(MassD[0]),&(Epsilon[0]));
  gEpsilon->SetTitle("");
  gEpsilon->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (MeV/c^{2})");
  gEpsilon->GetXaxis()->CenterTitle();
  gEpsilon->GetXaxis()->SetTitleOffset(1.2);
  gEpsilon->GetYaxis()->SetTitle("#epsilon^{2}");
  gEpsilon->GetYaxis()->CenterTitle();
  gEpsilon->SetMarkerStyle(8); 
  gEpsilon->SetMarkerColor(kBlack);
  gEpsilon->SetLineColor(kBlack);
  gEpsilon->SetLineWidth(2);
  gEpsilon->Draw("AL");


  vector <Double_t> EpsilonP2 = scaleEpsilon(Epsilon,UpperLim,ToyUpP2);
  vector <Double_t> EpsilonP1 = scaleEpsilon(Epsilon,UpperLim,ToyUpP1);
  vector <Double_t> EpsilonMed = scaleEpsilon(Epsilon,UpperLim,ToyUpMed);
  vector <Double_t> EpsilonN1 = scaleEpsilon(Epsilon,UpperLim,ToyUpN1);
  vector <Double_t> EpsilonN2 = scaleEpsilon(Epsilon,UpperLim,ToyUpN2);

  
  TGraph* gEpsilonP2 = new TGraph(MassD.size(),&(MassD[0]),&(EpsilonP2[0]));
  TGraph* gEpsilonP1 = new TGraph(MassD.size(),&(MassD[0]),&(EpsilonP1[0]));
  TGraph* gEpsilonMed = new TGraph(MassD.size(),&(MassD[0]),&(EpsilonMed[0]));
  TGraph* gEpsilonN1 = new TGraph(MassD.size(),&(MassD[0]),&(EpsilonN1[0]));
  TGraph* gEpsilonN2 = new TGraph(MassD.size(),&(MassD[0]),&(EpsilonN2[0]));

  gEpsilonMed->SetLineStyle(7);
  gEpsilonMed->SetLineWidth(3);
  gEpsilonMed->Draw("L");

  

  auto grshEP2 = shadeGraph(MassD,EpsilonP2,EpsilonP1,N,kYellow-4);
  auto grshEP1 = shadeGraph(MassD,EpsilonP1,EpsilonMed,N);
  auto grshEN1 = shadeGraph(MassD,EpsilonMed,EpsilonN1,N);
  auto grshEN2 = shadeGraph(MassD,EpsilonN1,EpsilonN2,N,kYellow-4);
  grshEP2->Draw("f");
  grshEP1->Draw("f");
  grshEN1->Draw("f");
  grshEN2->Draw("f");

  gEpsilonMed->Draw("L");
  gEpsilon->Draw("L same");

  TLegend* legELim = new TLegend(0.75,0.10,0.90,0.3);
  legELim->SetTextSize(0.025);
  legELim->AddEntry(gUpperLim,"#mu_{up}","l");
  legELim->AddEntry(gToyUpMed,"#mu_{med}","l");
  legELim->AddEntry(grshP1,"#pm 1#sigma CI","f");
  legELim->AddEntry(grshP2,"#pm 2#sigma CI","f");
  legELim->Draw(); 

  
  // save plots
  c1->SaveAs("plots/finLimits/Window_options.png");
  c2->SaveAs("plots/finLimits/Window_choices.png");
  c3->SaveAs("plots/finLimits/fin_Epsilon.png");
  c4->SaveAs("plots/finLimits/fin_UpLimits.png");

  //std::vector<Double_t> results = readFile(mass, POSet, NSig, MBg, BinSize, WSize);
    
  // read csv file with results from mass hypo


  //  TString CsvName = Form("csv/%iM_%im_%ip_results_alt.csv",MBg,mass,POSet);

  
  
}
