/********************************************************************
// John Williamson, 26/10/21
plot_pvals.C

plot p values of searches with different conditions

********************************************************************/


#include "CsvParser.h"



void plot_pvals(Int_t mass, Int_t POSet = 3/*Polynomial order for background*/,  Int_t TPOSet = 3/*Polynomial order for Toy MCs*/, Int_t NSig = 0, Int_t MBg = -1/*background model choicde*/, Double_t BinSize = 0){



  // read csv file with results from mass hypo


  //  TString CsvName = Form("csv/%iM_%im_%ip_results_alt.csv",MBg,mass,POSet);
  TString CsvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);
  
  CsvParser massOut(CsvName.Data());
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


    // test if Pull and bg chi^2 pass conditions

    if(abs(Pull[i])<pullLim && BgChi2[i]>chiLim){
      cout << "Entry passes" << endl;

      LWin = WF[i];
      
      if(PrevPass){
	//	newStreak.push_back(WF[i]);
	newStreak.push_back(i);
      }
      else{
	//	newStreak.push_back(WF[i]);
	newStreak.push_back(i);
      }
      PrevPass = true;      
    }
    else if(PrevPass){
      cout << "Entry fails" << endl;
      Streaks.push_back(newStreak);
      newStreak.clear();
      PrevPass = false;
    }
    cout << endl;
  }

  // catch any streak existing at end of WFs
  if(newStreak.size()>0){
    Streaks.push_back(newStreak);
  }

  
  cout << endl;
  cout << "Number of streak = " << Streaks.size() << endl;
  for (auto streak : Streaks) {
    cout << "new streak: " << endl;
    for(auto streakWF  : streak) {
      cout << streakWF << ", ";
    }
    cout << endl;
  }
  
  
  // plot those numbers
  Int_t n = Mass_s.size();

  TCanvas* c1 = new TCanvas("c1","c1",1500,1000);
  c1->SetLogy();
  c1->SetLeftMargin(0.12);
  //  c1->SetTopMargin(0.05);
  TGraph* gBgChi2 = new TGraph(n,&WF[0],&BgChi2[0]);
  gBgChi2->SetTitle(Form("%i MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));
  //  gBgChi2->SetTitle(Form("%.3f GeV, N_{bg} = %.0f",1e-3*mass,PO[0]));
  //  gBgChi2->SetTitle("");
  //  gBgChi2->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gBgChi2->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gBgChi2->GetXaxis()->CenterTitle();
  gBgChi2->GetXaxis()->SetTitleOffset(1.2);
  gBgChi2->GetYaxis()->SetTitle("#chi^{2}-Probability");
  gBgChi2->GetYaxis()->CenterTitle();
  // gBgChi2->GetHistogram()->SetMinimum(1e-10);
  gBgChi2->GetHistogram()->SetMinimum(1e-8);
  gBgChi2->GetHistogram()->SetMaximum(1);
  gBgChi2->SetMarkerStyle(8); 
  gBgChi2->SetMarkerColor(kRed);
  gBgChi2->SetLineColor(kRed);
  gBgChi2->GetYaxis()->SetTitleOffset(1.6);
  
  gBgChi2->Draw("APL");
  

  // draw tbox to represent 'stable' part of range
  std::vector<TBox*> bBgStreaks;
  for (auto streak : Streaks) {
    bBgStreaks.push_back(new TBox(WF[streak[0]],1e-8,WF[streak[streak.size()-1]],1));
  }
  
  for (auto box : bBgStreaks) {
    box->SetFillColor(kGreen-3);
    box->SetLineColor(kGreen-3);
    box->SetFillStyle(3005);
    //box->SetFillColorAlpha(kGreen-3,0.01);
    box->Draw("same");
  }

  TLine* lBgAcc = new TLine(gBgChi2->GetXaxis()->GetXmin(),chiLim,gBgChi2->GetXaxis()->GetXmax(),chiLim);
  lBgAcc->SetLineStyle(9);
  lBgAcc->SetLineColor(kBlue);
  lBgAcc->SetLineWidth(1.5);
  lBgAcc->Draw("same");

  gBgChi2->Draw("P same");

  TCanvas* c2 = new TCanvas("c2","c2",1500,1000);
  //c2->SetTopMargin(0.05);
  TGraphErrors* gPull = new TGraphErrors(n,&WF[0],&Pull[0],&Zeros[0],&PullErr[0]);
  gPull->SetTitle(Form("%i MeV, %.0f order bg, %.2f MeV bin",mass,PO[0],BinSize));
  //gPull->SetTitle(Form("%.3f GeV, N_{bg} = %.0f",1e-3*mass,PO[0]));
  //gPull->SetTitle("");
  gPull->SetMarkerStyle(8);
  gPull->SetMarkerColor(kRed);
  gPull->SetLineColor(kRed);
  //  gPull->GetXaxis()->SetTitle("Window Factor (n_{#sigma})");
  gPull->GetXaxis()->SetTitle("Window Size (n_{#sigma})");
  gPull->GetXaxis()->CenterTitle();
  gPull->GetXaxis()->SetTitleOffset(1.2);
  gPull->GetYaxis()->SetTitle("Pull");
  gPull->GetYaxis()->CenterTitle();
  gPull->GetYaxis()->SetTitleOffset(1.2);
  gPull->Draw("AP");
    

  // draw tbox to represent 'stable' part of range  
  std::vector<TBox*> bPullStreaks;
  Double_t maxPull = gPull->GetYaxis()->GetXmax();
  Double_t minPull = gPull->GetYaxis()->GetXmin();
  for (auto streak : Streaks) {
    //    bPullStreaks.push_back(new TBox(streak[0],minPull,streak[streak.size()-1],maxPull));
    bPullStreaks.push_back(new TBox(WF[streak[0]],minPull,WF[streak[streak.size()-1]],maxPull));
  }
  
  for (auto box : bPullStreaks) {
    box->SetFillColor(kGreen-3);
    box->SetLineColor(kGreen-3);
    box->SetFillStyle(3005);
    // box->SetFillStyle(1001);
    //    box->SetFillColorAlpha(kGreen-3,0.01);
    box->Draw("same");
  }



  // draw lines to represent acceptable limits of pull (+/- 2)
  
  TLine* lPullH = new TLine(gPull->GetXaxis()->GetXmin(),pullLim,gPull->GetXaxis()->GetXmax(),pullLim);
  lPullH->SetLineStyle(9);
  lPullH->SetLineColor(kBlue);
  lPullH->SetLineWidth(1.5);
  lPullH->Draw("same");

  TLine* lPullL = new TLine(gPull->GetXaxis()->GetXmin(),-pullLim,gPull->GetXaxis()->GetXmax(),-pullLim);
  lPullL->SetLineStyle(9);
  lPullL->SetLineColor(kBlue);
  lPullL->SetLineWidth(1.5);
  lPullL->Draw("same");

  gPull->Draw("P same");
  
  TCanvas* c3 = new TCanvas("c3","c3",1500,1000);
  c3->SetLeftMargin(0.12);
  //  c3->SetTopMargin(0.05);
  c3->SetLogy();
  gBgChi2->Draw("APL");

  
  TGraph* gBgSigChi2 = new TGraph(n,&WF[0],&BgSigChi2[0]);
  gBgSigChi2->SetMarkerColor(kBlue);
  gBgSigChi2->SetLineColor(kBlue);
  gBgSigChi2->SetMarkerStyle(8);
  gBgSigChi2->Draw("SAME PL");

  TGraph* gToyBgChi2 = new TGraph(n,&WF[0],&ToyBgChi2[0]);
  gToyBgChi2->SetMarkerColor(kGreen);
  gToyBgChi2->SetLineColor(kGreen);
  gToyBgChi2->SetMarkerStyle(8);
  gToyBgChi2->Draw("SAME PL");
  
  TGraph* gPVal = new TGraph(n,&WF[0],&PVal[0]);
  gPVal->SetMarkerColor(kBlack);
  gPVal->SetLineColor(kBlack);
  gPVal->SetMarkerStyle(8);
  gPVal->Draw("SAME PL");

  TGraph* gEpsilon = new TGraph(n,&WF[0],&Epsilon[0]);
  gEpsilon->SetMarkerColor(kYellow+2);
  gEpsilon->SetLineColor(kYellow+2);
  gEpsilon->SetMarkerStyle(8);
  gEpsilon->Draw("SAME PL");


  TLegend* legProb = new TLegend(0.65,0.5,0.85,0.8);
  legProb->AddEntry(gPVal,"p-Value","l");
  legProb->AddEntry(gBgChi2,"Bkg #chi^{2}","l");
  legProb->AddEntry(gBgSigChi2,"Sig+Bkg #chi^{2}","l");
  legProb->AddEntry(gToyBgChi2,"Generator #chi^{2}","l");
  legProb->AddEntry(gEpsilon,"#epsilon^{2}","l");

  legProb->Draw();

  // save plots

  c1->Print(Form("plots/Chi2/%.2fBin_%iM_%im_%ip_%iP_%is_chi2.png",BinSize,MBg,mass,POSet,TPOSet,NSig));

  c2->Print(Form("plots/Pulls/%.2fBin_%iM_%im_%ip_%iP_%is_Pull.png",BinSize,MBg,mass,POSet,TPOSet,NSig));
  
  c3->Print(Form("plots/MassScan/%.2fBin_%iM_%im_%ip_%iP_%is_results.png",BinSize,MBg,mass,POSet,TPOSet,NSig));


  // find optimal p val


  Double_t optEps = 1.0;
  Double_t optUL = 0.0;
  Double_t optPVal = 2.0;
  Int_t optWF = 0;
  // median toy upper limit and +/-1,2 sigma deviations
  Double_t optTUL = 0.0;
  Double_t optTULN2 = 0.0;
  Double_t optTULN1 = 0.0;
  Double_t optTULP1 = 0.0;
  Double_t optTULP2 = 0.0; 

  Int_t minStreak = 10; // minimum length of streak (to demonstrate stability)


  if(Streaks.size()>1){

    Int_t LEnt = Streaks[0][0]; // last entry
    for (auto streak : Streaks) {

      cout << "(streak[0]-LEnt) = " << (streak[0]-LEnt) << endl;
      
      if((streak[0]-LEnt)<4){ // allow gap of two in streak
      
	for(auto streakWF  : streak) {

	  LEnt = streakWF;
	  if(Epsilon[streakWF] < optEps){
	    optEps = Epsilon[streakWF];
	    optUL = UpperLim[streakWF];
	    optPVal = PVal[streakWF];
	    optWF = WF[streakWF];
	    optTUL = ToyUpMed[streakWF];
	    optTULN2 = ToyUpN2[streakWF];
	    optTULN1 = ToyUpN1[streakWF];
	    optTULP1 = ToyUpP1[streakWF];
	    optTULP2 = ToyUpP2[streakWF];
	  }
	}

      }
	
    }    
    
  }
  else if(Streaks.size()==1){
    for(auto streakWF  : Streaks[0]) {
      
      if(Epsilon[streakWF] < optEps){
	optEps = Epsilon[streakWF];
	optUL = UpperLim[streakWF];
	optPVal = PVal[streakWF];
	optWF = WF[streakWF];
	optTUL = ToyUpMed[streakWF];
	optTULN2 = ToyUpN2[streakWF];
	optTULN1 = ToyUpN1[streakWF];
	optTULP1 = ToyUpP1[streakWF];
	optTULP2 = ToyUpP2[streakWF];
      }
      
    }
  }
  
  // // for only in first streak
  // for(auto streakWF  : Streaks[0]) {

  //   if(Epsilon[streakWF] < optEps){
  // 	  optEps = Epsilon[streakWF];
  // 	  optPVal = PVal[streakWF];
  // 	  optWF = WF[streakWF];	  
  // 	}
	
  // }
      


  
  //all streaks
  // for (auto streak : Streaks) {
    
  //   if(streak.size() >= minStreak){
      
  //     for(auto streakWF  : streak) {

  // 	if(Epsilon[streakWF] < optEps){
  // 	  optEps = Epsilon[streakWF];
  // 	  optPVal = PVal[streakWF];
  // 	  optWF = WF[streakWF];	  
  // 	}
	
  //     }
      
  //   }    
  // }



  
  TString allCsvName = Form("csv/allPlot/%.2fBin_%iM_%im_%ip_%iP_%is_opt_epsilon.csv",BinSize,MBg,mass,POSet,TPOSet,NSig);

  cout << "opt params: " << endl;
  cout << "WF = " << optWF << ", optPVal = " << optPVal << ", optUL = " << optUL << ", optEps = " << optEps << ", LWin = " << LWin << endl;

  ofstream csvFile;
  csvFile.open(allCsvName);
  csvFile << "Mass,WF,PVal,epsilon2,LWin,UL,TUL,TULN2,TULN1,TULP1,TULP2" << endl;
  csvFile << mass << "," << optWF << "," << optPVal << "," << optEps << "," << LWin << "," << optUL << "," << optTUL << "," << optTULN2 << "," << optTULN1 << "," << optTULP1 << "," << optTULP2 << endl;
  csvFile.close();

  

  
  
  
}
