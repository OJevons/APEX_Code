/********************************************************************
// John Williamson, 26/10/21
plot_pvals.C

plot p values of searches with different conditions

********************************************************************/


#include "CsvParser.h"


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




void plot_lims(Double_t mass, Int_t POSet = 3/*Polynomial order for background*/,  Int_t TPOSet = 3/*Polynomial order for Toy MCs*/, Int_t NSig = 0, Int_t MBg = -1/*background model choicde*/, Double_t BinSize = 0){



  // read csv file with results from mass hypo


  TString csvName;
  if(ceilf(mass) == mass){
    csvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,(int)mass,POSet,TPOSet,NSig);
  }
  else{
    csvName = Form("csv/%.2fBin_%iM_%.2fm_%ip_%iP_%is_results.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);
  }
  //  TString CsvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);
  
  CsvParser massOut(csvName.Data());
  massOut.SortByColumn(1);
  
  std::vector<string> Mass_s = massOut.GetColumn(0);
  std::vector<Double_t> Mass;
  std::vector<string> WF_s = massOut.GetColumn(1);
  std::vector<Double_t> WF;
  std::vector<string> PO_s = massOut.GetColumn(2);
  std::vector<Double_t> PO;
  std::vector<string> RF_s = massOut.GetColumn(3);
  std::vector<Double_t> RF;
  std::vector<string> Yield_s = massOut.GetColumn(4);
  std::vector<Double_t> Yield;
  std::vector<string> YieldErr_s = massOut.GetColumn(5);
  std::vector<Double_t> YieldErr;
  std::vector<string> Pull_s = massOut.GetColumn(6);
  std::vector<Double_t> Pull;
  std::vector<string> PullErr_s = massOut.GetColumn(7);
  std::vector<Double_t> PullErr;
  std::vector<string> BgChi2_s = massOut.GetColumn(8);
  cout << "Size of BcChi2_s = " << BgChi2_s.size() << endl;
  std::vector<Double_t> BgChi2;
  std::vector<string> BgSigChi2_s = massOut.GetColumn(9);
  std::vector<Double_t> BgSigChi2;
  std::vector<string> ToyBgChi2_s = massOut.GetColumn(10);
  std::vector<Double_t> ToyBgChi2;
  std::vector<string> PVal_s = massOut.GetColumn(11);
  std::vector<Double_t> PVal;
  std::vector<string> Epsilon_s = massOut.GetColumn(12);
  std::vector<Double_t> Epsilon;
  std::vector<string> UpperLim_s = massOut.GetColumn(13);
  std::vector<Double_t> UpperLim;
  std::vector<string> fVal_s = massOut.GetColumn(14);
  std::vector<Double_t> fVal;
  std::vector<string> ToyUpMed_s = massOut.GetColumn(15);
  std::vector<Double_t> ToyUpMed;
  std::vector<string> ToyUpN2_s = massOut.GetColumn(16);
  std::vector<Double_t> ToyUpN2;
  std::vector<string> ToyUpN1_s = massOut.GetColumn(17);
  std::vector<Double_t> ToyUpN1;
  std::vector<string> ToyUpP1_s = massOut.GetColumn(18);
  std::vector<Double_t> ToyUpP1;
  std::vector<string> ToyUpP2_s = massOut.GetColumn(19);
  std::vector<Double_t> ToyUpP2;
  std::vector<Double_t> Zeros;


  //  cuts for acceptable pull and bg chi^2
  Double_t pullLim = 2;
  Double_t chiLim = 0.01;

  // vector of vectors to store 'streaks' of window factors that survive cuts
  //  std::vector<std::vector<Double_t>> Streaks;
  std::vector<std::vector<Int_t>> Streaks;

  Bool_t PrevPass = false;
  
  //  std::vector<Double_t> newStreak;
  std::vector<Int_t> newStreak; // store integer of entry (ie 5th entry in WF, epsioon etc)

  // record Window size to pass conditions
  Double_t LWin = 1;
  
  for(Int_t i = 0; i<Mass_s.size(); i++){
    cout << "i = " << i << endl;
    Mass.push_back(stod(Mass_s[i]));
    cout << "Done mass " << endl;
    WF.push_back(stod(WF_s[i]));
    PO.push_back(stod(PO_s[i]));
    RF.push_back(stod(RF_s[i]));
    cout << "Done RF " << endl;
    Yield.push_back(stof(Yield_s[i]));
    cout << "Done Yield" << endl;
    YieldErr.push_back(stof(YieldErr_s[i]));
    cout << "Done Yield Err" << endl;
    Pull.push_back(stof(Pull_s[i]));
    cout << "Done Pull" << endl;
    PullErr.push_back(2*stof(PullErr_s[i]));
    cout << "Done Pull Err" << endl;
    cout << "BgChi2_s[" << i << "] = " << BgChi2_s[i] << endl;
    try{
      BgChi2.push_back(stod(BgChi2_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      BgChi2.push_back(0.0);
    }
    try{
      BgSigChi2.push_back(stod(BgSigChi2_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      BgSigChi2.push_back(0.0);
    }
    try{
      ToyBgChi2.push_back(stod(ToyBgChi2_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyBgChi2.push_back(0.0);
    }
    try{
      PVal.push_back(stod(PVal_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      PVal.push_back(0.0);
    }
    try{
      Epsilon.push_back(stod(Epsilon_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      Epsilon.push_back(0.0);
    }
    try{
      UpperLim.push_back(stod(UpperLim_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      UpperLim.push_back(0.0);
    }
    try{
      fVal.push_back(stod(fVal_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      fVal.push_back(0.0);
    }
    try{
      ToyUpMed.push_back(stod(ToyUpMed_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyUpMed.push_back(0.0);
    }
    try{
      ToyUpN2.push_back(stod(ToyUpN2_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyUpN2.push_back(0.0);
    }
    try{
      ToyUpN1.push_back(stod(ToyUpN1_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyUpN1.push_back(0.0);
    }
    try{
      ToyUpP1.push_back(stod(ToyUpP1_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyUpP1.push_back(0.0);
    }
    try{
      ToyUpP2.push_back(stod(ToyUpP2_s[i]));
    }
    catch(const std::out_of_range& e) {
      cout << "Out of Range error.";
      ToyUpP2.push_back(0.0);
    }

    cout << "Done BgChi2" << endl;
    Zeros.push_back(0);
    cout << "Done zeros" << endl;
    
    //    cout << i << "th entry: Mass = " << Mass[i] << ", WF = " << WF[i] << ", PO = " << PO[i] << ", RF = " << RF[i] << ", Yield = " << Yield[i] <<  ", YieldErr = " << YieldErr[i] << ", BgChi2 = " << BgChi2[i] << endl;
    cout << i << "th entry: Mass = " << Mass[i] << ", WF = " << WF[i] << ", PO = " << PO[i] << ", RF = " << RF[i] << ", Yield = " << Yield[i] <<  ", YieldErr = " << YieldErr[i] << ", BgChi2 = " << BgChi2[i] << ", BgSigChi2 = " << BgSigChi2[i] << ", ToyBgChi2 = " << ToyBgChi2[i] << ", PVal = " << PVal[i] << ", Epsilon = " << Epsilon[i] << ", UpperLim = " << UpperLim[i] << ", fVal = " << fVal[i] << ", Pull = " << Pull[i] << endl;


  }

    
  // plot those numbers
  Int_t n = Mass_s.size();

  TCanvas* c1 = new TCanvas("c1","c1",1500,1000);
  c1->SetLogy();
  c1->SetLeftMargin(0.12);
  //  c1->SetTopMargin(0.05);
  TGraph* gUpperLim = new TGraph(n,&WF[0],&UpperLim[0]);  
  gUpperLim->SetTitle(Form("%.2f MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));

  
  //  gUpperLim->SetTitle(Form("%.3f GeV, N_{bg} = %.0f",1e-3*mass,PO[0]));
  //  gUpperLim->SetTitle("");
  //  gUpperLim->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gUpperLim->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gUpperLim->GetXaxis()->CenterTitle();
  gUpperLim->GetXaxis()->SetTitleOffset(1.2);
  gUpperLim->GetYaxis()->SetTitle("Signal Upper Limit");
  gUpperLim->GetYaxis()->CenterTitle();
  // gUpperLim->GetHistogram()->SetMinimum(1e-10);
  // gUpperLim->GetHistogram()->SetMinimum(1e-8);
  // gUpperLim->GetHistogram()->SetMaximum(1);
  gUpperLim->SetMarkerStyle(8); 
  gUpperLim->SetMarkerColor(kBlack);
  gUpperLim->SetLineColor(kBlack);
  gUpperLim->SetLineWidth(2);
  gUpperLim->GetYaxis()->SetTitleOffset(1.6);
  
  //gUpperLim->Draw("APL");

  TGraph* gToyUpP2 = new TGraph(n,&(WF[0]),&(ToyUpP2[0]));  
  TGraph* gToyUpP1 = new TGraph(n,&(WF[0]),&(ToyUpP1[0]));
  TGraph* gToyUpMed = new TGraph(n,&(WF[0]),&(ToyUpMed[0]));
  TGraph* gToyUpN1 = new TGraph(n,&(WF[0]),&(ToyUpN1[0]));
  TGraph* gToyUpN2 = new TGraph(n,&(WF[0]),&(ToyUpN2[0]));
  
  gToyUpMed->SetLineStyle(7);
  gToyUpMed->SetLineWidth(3);
  gToyUpMed->SetTitle(Form("%.2f MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));
  gToyUpMed->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gToyUpMed->GetXaxis()->CenterTitle();
  gToyUpMed->GetXaxis()->SetTitleOffset(1.2);
  gToyUpMed->GetYaxis()->SetTitle("Signal Upper Limit");
  gToyUpMed->GetYaxis()->CenterTitle();
  gToyUpMed->Draw("APl");

  auto grshP2 = shadeGraph(WF,ToyUpP2,ToyUpP1,n,kYellow-4);
  auto grshP1 = shadeGraph(WF,ToyUpP1,ToyUpMed,n);
  auto grshN1 = shadeGraph(WF,ToyUpMed,ToyUpN1,n);
  auto grshN2 = shadeGraph(WF,ToyUpN1,ToyUpN2,n,kYellow-4);

  grshP2->Draw("f");
  grshP1->Draw("f");
  grshN1->Draw("f");
  grshN2->Draw("f");

  gToyUpMed->Draw("l same");
  gUpperLim->Draw("PL same");

  TLegend* legLim = new TLegend(0.75,0.70,0.90,0.9);
  // legLim->SetNColumns(2);
  legLim->SetTextSize(0.025);
  legLim->AddEntry(gUpperLim,"#mu_{up}","l");
  legLim->AddEntry(gToyUpMed,"#mu_{med}","l");
  legLim->AddEntry(grshP1,"#pm 1#sigma CI","f");
  legLim->AddEntry(grshP2,"#pm 2#sigma CI","f");
  legLim->Draw();
  
  
  TCanvas* c2 = new TCanvas("c2","c2",1500,1000);
  c2->SetLogy();
  c2->SetLeftMargin(0.12);
  //  c2->SetTopMargin(0.05);
  TGraph* gEpsilon = new TGraph(n,&WF[0],&Epsilon[0]);
  gEpsilon->SetTitle(Form("%.2f MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));
  //  gEpsilon->SetTitle(Form("%.3f GeV, N_{bg} = %.0f",1e-3*mass,PO[0]));
  //  gEpsilon->SetTitle("");
  //  gEpsilon->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gEpsilon->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gEpsilon->GetXaxis()->CenterTitle();
  gEpsilon->GetXaxis()->SetTitleOffset(1.2);
  gEpsilon->GetYaxis()->SetTitle("#epsilon^{2}");
  gEpsilon->GetYaxis()->CenterTitle();
  // gEpsilon->GetHistogram()->SetMinimum(1e-10);
  // gEpsilon->GetHistogram()->SetMinimum(1e-8);
  // gEpsilon->GetHistogram()->SetMaximum(1);
  gEpsilon->SetMarkerStyle(8); 
  gEpsilon->SetMarkerColor(kBlack);
  gEpsilon->SetLineColor(kBlack);
  gEpsilon->SetLineWidth(2);
  gEpsilon->GetYaxis()->SetTitleOffset(1.6);
  
  //gEpsilon->Draw("APL");
  
  
  vector <Double_t> EpsilonP2 = scaleEpsilon(Epsilon,UpperLim,ToyUpP2);
  vector <Double_t> EpsilonP1 = scaleEpsilon(Epsilon,UpperLim,ToyUpP1);
  vector <Double_t> EpsilonMed = scaleEpsilon(Epsilon,UpperLim,ToyUpMed);
  vector <Double_t> EpsilonN1 = scaleEpsilon(Epsilon,UpperLim,ToyUpN1);
  vector <Double_t> EpsilonN2 = scaleEpsilon(Epsilon,UpperLim,ToyUpN2);


  TGraph* gEpsilonP2 = new TGraph(n,&(WF[0]),&(EpsilonP2[0]));
  TGraph* gEpsilonP1 = new TGraph(n,&(WF[0]),&(EpsilonP1[0]));
  TGraph* gEpsilonMed = new TGraph(n,&(WF[0]),&(EpsilonMed[0]));
  TGraph* gEpsilonN1 = new TGraph(n,&(WF[0]),&(EpsilonN1[0]));
  TGraph* gEpsilonN2 = new TGraph(n,&(WF[0]),&(EpsilonN2[0]));

  gEpsilonMed->SetLineStyle(7);
  gEpsilonMed->SetLineWidth(3);
  gEpsilonMed->SetTitle(Form("%.2f MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));
  gEpsilonMed->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gEpsilonMed->GetXaxis()->CenterTitle();
  gEpsilonMed->GetXaxis()->SetTitleOffset(1.2);
  gEpsilonMed->GetYaxis()->SetTitle("#epsilon^{2}");
  gEpsilonMed->GetYaxis()->CenterTitle();
  gEpsilonMed->Draw("APL");

  auto grshEP2 = shadeGraph(WF,EpsilonP2,EpsilonP1,n,kYellow-4);
  auto grshEP1 = shadeGraph(WF,EpsilonP1,EpsilonMed,n);
  auto grshEN1 = shadeGraph(WF,EpsilonMed,EpsilonN1,n);
  auto grshEN2 = shadeGraph(WF,EpsilonN1,EpsilonN2,n,kYellow-4);
  grshEP2->Draw("f");
  grshEP1->Draw("f");
  grshEN1->Draw("f");
  grshEN2->Draw("f");

  gEpsilonMed->Draw("L");
  gEpsilon->Draw("PL same");
  legLim->Draw();

  
  // save plots


  if(ceilf(mass) == mass){
    c1->Print(Form("plots/Limits/%.2fBin_%iM_%im_%ip_%iP_%is_Sig.png",BinSize,MBg,(int)mass,POSet,TPOSet,NSig));
    c2->Print(Form("plots/Limits/%.2fBin_%iM_%im_%ip_%iP_%is_Eps.png",BinSize,MBg,(int)mass,POSet,TPOSet,NSig));
  }
  else{
    c1->Print(Form("plots/Limits/%.2fBin_%iM_%.2fm_%ip_%iP_%is_Sig.png",BinSize,MBg,mass,POSet,TPOSet,NSig));
    c2->Print(Form("plots/Limits/%.2fBin_%iM_%.2fm_%ip_%iP_%is_Eps.png",BinSize,MBg,mass,POSet,TPOSet,NSig));
  }
  

      
  
}
