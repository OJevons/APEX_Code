//-----------------------------------------------------------------------------
// Code to plot output of HPS bump hunt: p-value, signal upper limit and sensitivity
// Oliver Jevons; 18-Nov-22
//
// Inputs: 
// 1. Path to peak search output (TH1 created by PeasSearchReadable.cxx)
//
// Output:
// 1. Canvas for p-value plot, with 1sigma global limit.
// 2. Canvas for upper limit, with bands for 1/2 sigma confidence intervals
// 3. Canvas for epsilon2, with bands for 1/2 sigma CI
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <TString.h>
#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>

//const TString sFileIn = "/scratch/oliver/fOut_JWNoZero.root";
//const TString sFileIn = "./rootfiles/fOut_JWMaster_noblind.root";
//const TString sFileIn = "/scratch/oliver/fOut_JWNoZeroWFMinus5.root";
const TString sFileIn = "/scratch/oliver/fOut_NoZero_WFConst9.root";

const Float_t WFLine1{144.};
const Float_t WFLine2{165.};
const Float_t MaxOver3Lo{138.325};
const Float_t MaxOver3Hi{200.875};

// Scale epsilon squared value based on upper limit
std::vector<Double_t> scaleEpsilon(std::vector<Double_t> epsilon_up, std::vector<Double_t> mu_up, std::vector<Double_t> mu_alt){

  std::vector<Double_t> epsilon_alt;
  
  for(Int_t i = 0; i < epsilon_up.size(); i++){

    epsilon_alt.push_back((mu_alt[i]/mu_up[i])*epsilon_up[i]);
    
  }

  return epsilon_alt;
}

// Create shaded graphs for Brazil plots
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

// Calculate arrays for TGraphErrors from arrays of upper limits
std::vector<Double_t> errArray(std::vector<Double_t> points_hi, std::vector<Double_t> points_lo){
  
  std::vector<Double_t> errs;

  for(Int_t i{0}; i<points_hi.size(); i++){
    errs.push_back(points_hi[i] - points_lo[i]);
  }

  return errs;
}

void plotresults(){
  // Load input file
  TFile* fIn = new TFile(sFileIn);
  if(!fIn || fIn->IsZombie()){
    std::cout<<"Unable to open file "<<sFileIn<<" for reading..."<<std::endl;
    return;
  }

  // Vectors of important quantities
  std::vector<Double_t> Mass;
  std::vector<Double_t> PVal;
  std::vector<Double_t> UpperLim;
  std::vector<Double_t> EpsilonSq;
  std::vector<Double_t> ToyUpMed;
  std::vector<Double_t> ToyUpN2;
  std::vector<Double_t> ToyUpN1;
  std::vector<Double_t> ToyUpP1;
  std::vector<Double_t> ToyUpP2;
  std::vector<Double_t> Chi2Bkg;
  std::vector<Double_t> Chi2BkgSig;
  std::vector<Double_t> Chi2ToyBkg;

  // Read input histogram and fill vector; bins as follows
  //  1 - Mass hypothesis     2 - Window size              3 - Polynomial order
  //  4 - Yield               5 - Error on yield
  //  6 - Pull                7 - Error on pull
  //  8 - Chi2 of bkg fit     9 - Chi2 of Bkg_Sig fit     10 - Chi2 of toy bkg fit
  // 11 - p value            12 - epsilon2
  // 13 - signal upper lim.  14 - radiative fraction
  // 15 - Median toy UL      16 - Toy UL; 2 sigma lo      17 - Toy UL; 1 sigma lo
  // 18 - Toy UL; 1 sigma hi 18 - Toy UL; 2 sigma hi
  TKey *key;
  TList* contents = fIn->GetListOfKeys();
  contents->Sort();
  
  TIter next(contents);
  while((key = (TKey *)next())){
    TClass *cl1 = gROOT->GetClass(key->GetClassName());
    if(cl1->InheritsFrom("TH1")){
      TH1* h = (TH1*)key->ReadObj();

      Mass.push_back(h->GetBinContent(1));
      PVal.push_back(h->GetBinContent(11));
      UpperLim.push_back(h->GetBinContent(13));
      EpsilonSq.push_back(h->GetBinContent(12));
      ToyUpMed.push_back(h->GetBinContent(15));
      ToyUpN2.push_back(h->GetBinContent(16));
      ToyUpN1.push_back(h->GetBinContent(17));
      ToyUpP1.push_back(h->GetBinContent(18));
      ToyUpP2.push_back(h->GetBinContent(19));

      Chi2Bkg.push_back(h->GetBinContent(8));
      Chi2ToyBkg.push_back(h->GetBinContent(10));    
      if(h->GetBinContent(9) == 1.00000 || h->GetBinContent(9) == 0.00000) Chi2BkgSig.push_back(2.0000); 
      else Chi2BkgSig.push_back(h->GetBinContent(9));
    }
  }

  //sort(Mass.begin(),Mass.end());
  
  // CANVAS - p values
  TCanvas* c1 = new TCanvas("c1","c1",1500,1000);
  c1->SetLogy();
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.05);

  TGraph* gPVal = new TGraph(Mass.size(),&Mass[0],&PVal[0]);
  gPVal->SetTitle("");
  gPVal->SetMarkerColor(kBlue);
  gPVal->SetLineColor(kBlue);
  gPVal->SetMarkerStyle(8);
  gPVal->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (MeV/c^{2})");
  gPVal->GetXaxis()->SetTitleSize(0.035);
  gPVal->GetXaxis()->CenterTitle();
  gPVal->GetXaxis()->SetTitleOffset(1.2);
  gPVal->GetYaxis()->SetTitle("Local p-value");
  gPVal->GetYaxis()->SetTitleSize(0.035);
  gPVal->GetYaxis()->CenterTitle();
  gPVal->SetMinimum(0.00001);
  gPVal->SetLineWidth(2);

  TLine* sigmaLine = new TLine(122,0.0019609,228,0.0019609); // 0.00164 old value
  sigmaLine->SetLineColor(kRed);
  sigmaLine->SetLineWidth(2);
  gPVal->Draw("AL");

  // LEE stuff
  // correction = W/sigma_m
  // W = total mass range, sigma_m = mean mass resolution
  // W = 220-130 = 90, sigma_m = 1.112407
  // correction = 80.91
  // (local) 1 sigma limit =   0.1586552594
  // (global) 1 sigma limit =   0.1586552594 / 80.91 = 0.0019609
  
  TPaveText *ptPVal = new TPaveText(0.40,0.12,0.6,0.32,"NDC");
  ptPVal->SetTextSize(0.035);
  ptPVal->SetBorderSize(0);
  ptPVal->AddText("1#sigma Global Threshold");
  ptPVal->SetFillStyle(0);
  ptPVal->Draw();
 
  sigmaLine->Draw("same");
  
  // Draw lines marking WF changes and histogram hi stats region
  Float_t PVal_max = *std::max_element(PVal.begin(),PVal.end());
  Float_t PVal_min = *std::min_element(PVal.begin(),PVal.end());
  
  if(PVal_min < 0.00001) PVal_min = 0.00001;

  TLine* lWFLine1_c1 = new TLine(WFLine1,PVal_min,WFLine1,PVal_max);
  lWFLine1_c1->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine1_c1->SetLineStyle(10);
  lWFLine1_c1->SetLineWidth(3);

  TLine* lWFLine2_c1 = new TLine(WFLine2,PVal_min,WFLine2,PVal_max);
  lWFLine2_c1->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine2_c1->SetLineStyle(10);
  lWFLine2_c1->SetLineWidth(3);

  TLine* lMaxFracLo_c1 = new TLine(MaxOver3Lo,PVal_min,MaxOver3Lo,PVal_max);
  lMaxFracLo_c1->SetLineColor(kMagenta);
  lMaxFracLo_c1->SetLineStyle(2);
  lMaxFracLo_c1->SetLineWidth(3);

  TLine* lMaxFracHi_c1 = new TLine(MaxOver3Hi,PVal_min,MaxOver3Hi,PVal_max);
  lMaxFracHi_c1->SetLineColor(kMagenta);
  lMaxFracHi_c1->SetLineStyle(2);
  lMaxFracHi_c1->SetLineWidth(3);
  
  lWFLine1_c1->Draw("same");
  lWFLine2_c1->Draw("same");
  lMaxFracLo_c1->Draw("same");
  lMaxFracHi_c1->Draw("same");

  // CANVAS - signal upper limits
  TCanvas* c2 = new TCanvas("c2","c2",1500,1000);
  c2->SetLogy();
  c2->SetLeftMargin(0.12);
  c2->SetTopMargin(0.05);
  TGraph* gUpperLim = new TGraph(Mass.size(),&(Mass[0]),&(UpperLim[0]));
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
  
  TGraph* gToyUpMed = new TGraph(Mass.size(),&(Mass[0]),&(ToyUpMed[0]));
  gToyUpMed->SetLineStyle(7);
  gToyUpMed->SetLineWidth(3);

  // Calculate error arrays for Brazil plots
  /*std::vector<Double_t> OneSigmaLo = errArray(ToyUpMed, ToyUpN1);
  std::vector<Double_t> OneSigmaHi = errArray(ToyUpP1, ToyUpMed);
  std::vector<Double_t> TwoSigmaLo = errArray(ToyUpMed, ToyUpN2);
  std::vector<Double_t> TwoSigmaHi = errArray(ToyUpP2, ToyUpMed);
  std::vector<Double_t> Err_x = errArray(Mass, Mass);
  
  TGraphAsymmErrors* gae1Sigma = new TGraphAsymmErrors(Mass.size(),&(Mass[0]),&(ToyUpMed[0]),&(Err_x[0]),&(Err_x[0]),&(OneSigmaLo[0]),&(OneSigmaHi[0]));
  TGraphAsymmErrors* gae2Sigma = new TGraphAsymmErrors(Mass.size(),&(Mass[0]),&(ToyUpMed[0]),&(Err_x[0]),&(Err_x[0]),&(TwoSigmaLo[0]),&(TwoSigmaHi[0]));
  
  gae1Sigma->SetFillColor(kGreen+1);
  gae2Sigma->SetFillColor(kYellow+4);
  gae2Sigma->Draw("a3");
  gae1Sigma->Draw("a3 same");*/
  
  Int_t N = Mass.size();
  
  auto grshP2 = shadeGraph(Mass,ToyUpP2,ToyUpP1,N,kYellow-4);
  auto grshP1 = shadeGraph(Mass,ToyUpP1,ToyUpMed,N);
  auto grshN1 = shadeGraph(Mass,ToyUpMed,ToyUpN1,N);
  auto grshN2 = shadeGraph(Mass,ToyUpN1,ToyUpN2,N,kYellow-4);
  grshP2->Draw("f");
  grshP1->Draw("f");
  grshN1->Draw("f");
  grshN2->Draw("f");

  gToyUpMed->Draw("l same");
  gUpperLim->Draw("L same");
  
  TLegend* legLim = new TLegend(0.75,0.75,0.90,0.95);
  legLim->SetTextSize(0.030);
  legLim->AddEntry(gUpperLim,"#mu_{up}","l");
  legLim->AddEntry(gToyUpMed,"#mu_{med}","l");
  legLim->AddEntry(grshP1,"#pm 1#sigma CI","f");
  legLim->AddEntry(grshP2,"#pm 2#sigma CI","f");
  legLim->Draw();

  // Add in WF and statistics lines
  Float_t UL_max = *std::max_element(UpperLim.begin(),UpperLim.end());
  Float_t UL_min = *std::min_element(UpperLim.begin(),UpperLim.end());

  TLine* lWFLine1_c2 = new TLine(WFLine1,UL_min,WFLine1,UL_max);
  lWFLine1_c2->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine1_c2->SetLineStyle(10);
  lWFLine1_c2->SetLineWidth(3);

  TLine* lWFLine2_c2 = new TLine(WFLine2,UL_min,WFLine2,UL_max);
  lWFLine2_c2->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine2_c2->SetLineStyle(10);
  lWFLine2_c2->SetLineWidth(3);

  TLine* lMaxFracLo_c2 = new TLine(MaxOver3Lo,UL_min,MaxOver3Lo,UL_max);
  lMaxFracLo_c2->SetLineColor(kMagenta);
  lMaxFracLo_c2->SetLineStyle(2);
  lMaxFracLo_c2->SetLineWidth(3);

  TLine* lMaxFracHi_c2 = new TLine(MaxOver3Hi,UL_min,MaxOver3Hi,UL_max);
  lMaxFracHi_c2->SetLineColor(kMagenta);
  lMaxFracHi_c2->SetLineStyle(2);
  lMaxFracHi_c2->SetLineWidth(3);
  
  lWFLine1_c2->Draw("same");
  lWFLine2_c2->Draw("same");
  lMaxFracLo_c2->Draw("same");
  lMaxFracHi_c2->Draw("same");

  // CANVAS - sensitivity
  TCanvas* c3 = new TCanvas("c3","c3",1500,1000);
  c3->SetLogy();
  c3->SetLeftMargin(0.12);
  c3->SetTopMargin(0.05);
  
  TGraph* gEpsilon = new TGraph(Mass.size(),&(Mass[0]),&(EpsilonSq[0]));
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

  vector <Double_t> EpsilonP2 = scaleEpsilon(EpsilonSq,UpperLim,ToyUpP2);
  vector <Double_t> EpsilonP1 = scaleEpsilon(EpsilonSq,UpperLim,ToyUpP1);
  vector <Double_t> EpsilonMed = scaleEpsilon(EpsilonSq,UpperLim,ToyUpMed);
  vector <Double_t> EpsilonN1 = scaleEpsilon(EpsilonSq,UpperLim,ToyUpN1);
  vector <Double_t> EpsilonN2 = scaleEpsilon(EpsilonSq,UpperLim,ToyUpN2);

  TGraph* gEpsilonMed = new TGraph(Mass.size(),&(Mass[0]),&(EpsilonMed[0]));

  gEpsilonMed->SetLineStyle(7);
  gEpsilonMed->SetLineWidth(3);
  gEpsilonMed->Draw("L");

  auto grshEP2 = shadeGraph(Mass,EpsilonP2,EpsilonP1,N,kYellow-4);
  auto grshEP1 = shadeGraph(Mass,EpsilonP1,EpsilonMed,N);
  auto grshEN1 = shadeGraph(Mass,EpsilonMed,EpsilonN1,N);
  auto grshEN2 = shadeGraph(Mass,EpsilonN1,EpsilonN2,N,kYellow-4);
  grshEP2->Draw("f");
  grshEP1->Draw("f");
  grshEN1->Draw("f");
  grshEN2->Draw("f");

  gEpsilonMed->Draw("L");
  gEpsilon->Draw("L same");

  TLegend* legELim = new TLegend(0.75,0.10,0.90,0.3);
  legELim->SetTextSize(0.030);
  legELim->AddEntry(gUpperLim,"#mu_{up}","l");
  legELim->AddEntry(gToyUpMed,"#mu_{med}","l");
  legELim->AddEntry(grshP1,"#pm 1#sigma CI","f");
  legELim->AddEntry(grshP2,"#pm 2#sigma CI","f");
  legELim->Draw();

  // Add in WF and statistics lines
  Float_t Eps_max = *std::max_element(EpsilonSq.begin(),EpsilonSq.end());
  Float_t Eps_min = *std::min_element(EpsilonSq.begin(),EpsilonSq.end());

  TLine* lWFLine1_c3 = new TLine(WFLine1,Eps_min,WFLine1,Eps_max);
  lWFLine1_c3->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine1_c3->SetLineStyle(10);
  lWFLine1_c3->SetLineWidth(3);

  TLine* lWFLine2_c3 = new TLine(WFLine2,Eps_min,WFLine2,Eps_max);
  lWFLine2_c3->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine2_c3->SetLineStyle(10);
  lWFLine2_c3->SetLineWidth(3);

  TLine* lMaxFracLo_c3 = new TLine(MaxOver3Lo,Eps_min,MaxOver3Lo,Eps_max);
  lMaxFracLo_c3->SetLineColor(kMagenta);
  lMaxFracLo_c3->SetLineStyle(2);
  lMaxFracLo_c3->SetLineWidth(3);

  TLine* lMaxFracHi_c3 = new TLine(MaxOver3Hi,Eps_min,MaxOver3Hi,Eps_max);
  lMaxFracHi_c3->SetLineColor(kMagenta);
  lMaxFracHi_c3->SetLineStyle(2);
  lMaxFracHi_c3->SetLineWidth(3);
  
  lWFLine1_c3->Draw("same");
  lWFLine2_c3->Draw("same");
  lMaxFracLo_c3->Draw("same");
  lMaxFracHi_c3->Draw("same");
  
  // CANVAS - Chi2 values
  TCanvas* c4 = new TCanvas("c4","c4",1500,1000);
  c4->SetLogy();
  c4->SetLeftMargin(0.12);
  c4->SetTopMargin(0.05);

  TGraph* gChi2Bkg = new TGraph(Mass.size(),&Mass[0],&Chi2Bkg[0]);
  gChi2Bkg->SetTitle("");
  //gChi2Bkg->SetMarkerColor(kBlue);
  gChi2Bkg->SetLineColor(kBlack);
  //gChi2Bkg->SetMarkerStyle(8);
  gChi2Bkg->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (MeV/c^{2})");
  gChi2Bkg->GetXaxis()->SetTitleSize(0.035);
  gChi2Bkg->GetXaxis()->CenterTitle();
  gChi2Bkg->GetXaxis()->SetTitleOffset(1.2);
  gChi2Bkg->GetYaxis()->SetTitle("Fit #chi^{2}");
  gChi2Bkg->GetYaxis()->SetTitleSize(0.035);
  gChi2Bkg->GetYaxis()->CenterTitle();
  gChi2Bkg->SetMinimum(1e-10);
  gChi2Bkg->SetMaximum(2.5);
  gChi2Bkg->SetLineWidth(2);
  gChi2Bkg->Draw("AL");

  TGraph* gChi2SigBkg = new TGraph(Mass.size(),&Mass[0],&Chi2BkgSig[0]);
  gChi2SigBkg->SetLineColor(kBlue);
  gChi2SigBkg->SetLineWidth(2);
  gChi2SigBkg->Draw("same");

  TGraph* gChi2ToyBkg = new TGraph(Mass.size(),&Mass[0],&Chi2ToyBkg[0]);
  gChi2ToyBkg->SetLineColor(kRed);
  gChi2ToyBkg->SetLineWidth(2);
  gChi2ToyBkg->Draw("same");

  TLine* chi2Line = new TLine(122,1e-2,228,1e-2); // 0.00164 old value
  chi2Line->SetLineColor(kBlack);
  chi2Line->SetLineStyle(2); 
  chi2Line->SetLineWidth(2); 
  chi2Line->Draw("same");

   // Add in WF and statistics lines
  Float_t Chi2_max = *std::max_element(Chi2BkgSig.begin(),Chi2BkgSig.end());
  //Float_t Chi2_min = *std::min_element(Chi2BkgSig.begin(),Chi2BkgSig.end());
  Float_t Chi2_min = 1e-10;

  TLine* lWFLine1_c4 = new TLine(WFLine1,Chi2_min,WFLine1,Chi2_max);
  lWFLine1_c4->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine1_c4->SetLineStyle(10);
  lWFLine1_c4->SetLineWidth(3);

  TLine* lWFLine2_c4 = new TLine(WFLine2,Chi2_min,WFLine2,Chi2_max);
  lWFLine2_c4->SetLineColorAlpha(kCyan+1,0.7);
  lWFLine2_c4->SetLineStyle(10);
  lWFLine2_c4->SetLineWidth(3);

  TLine* lMaxFracLo_c4 = new TLine(MaxOver3Lo,Chi2_min,MaxOver3Lo,Chi2_max);
  lMaxFracLo_c4->SetLineColor(kMagenta);
  lMaxFracLo_c4->SetLineStyle(2);
  lMaxFracLo_c4->SetLineWidth(3);

  TLine* lMaxFracHi_c4 = new TLine(MaxOver3Hi,Chi2_min,MaxOver3Hi,Chi2_max);
  lMaxFracHi_c4->SetLineColor(kMagenta);
  lMaxFracHi_c4->SetLineStyle(2);
  lMaxFracHi_c4->SetLineWidth(3);
  
  lWFLine1_c4->Draw("same");
  lWFLine2_c4->Draw("same");
  lMaxFracLo_c4->Draw("same");
  lMaxFracHi_c4->Draw("same");

  TLegend* legChi2 = new TLegend(0.65,0.1,0.90,0.3);
  legChi2->SetTextSize(0.030);
  legChi2->SetFillColorAlpha(kWhite,0);
  legChi2->AddEntry(gChi2Bkg,"Background only","l");
  legChi2->AddEntry(gChi2SigBkg,"Signal + background","l");
  legChi2->AddEntry(gChi2ToyBkg,"Toy background","l");
  legChi2->Draw();

  c1->SaveAs("plots/PVals_FullStatsNoZero_WF9.png");
  c2->SaveAs("plots/UpperLimits_FullStatsNoZero_WF9.png");
  c3->SaveAs("plots/Epsilon_FullStatsNoZero_WF9.png");
  c4->SaveAs("plots/Chi2_FullStatsNoZero_WF9.png");

  return;
}
