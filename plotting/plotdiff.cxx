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

const TString sFileDefault = "/scratch/oliver/fOut_JWTrim50k_noblind.root";
//const TString sFileVaried = "/scratch/oliver/fOut_JWTrim50k_DblMassRes.root";
const TString sFileVaried = "/scratch/oliver/fOut_50kLVDCgap0_noblind.root";

// Scale epsilon squared value based on upper limit
std::vector<Double_t> scaleEpsilon(std::vector<Double_t> epsilon_up, std::vector<Double_t> mu_up, std::vector<Double_t> mu_alt){

  std::vector<Double_t> epsilon_alt;
  
  for(Int_t i = 0; i < epsilon_up.size(); i++){

    epsilon_alt.push_back((mu_alt[i]/mu_up[i])*epsilon_up[i]);
    
  }

  return epsilon_alt;
}

void plotdiff(){
  // Load input files
  TFile* fInDefault = new TFile(sFileDefault);
  TFile* fInVaried = new TFile(sFileVaried);

  if(!fInDefault || fInDefault->IsZombie()){
    std::cout<<"Unable to open file "<<sFileDefault<<" for reading..."<<std::endl;
    return;
  }
  if(!fInVaried || fInVaried->IsZombie()){
    std::cout<<"Unable to open file "<<sFileVaried<<" for reading..."<<std::endl;
    return;
  }

  // Vectors of important quantities - from default file
  std::vector<Double_t> Mass_Def;
  std::vector<Double_t> PVal_Def;
  std::vector<Double_t> UpperLim_Def;
  std::vector<Double_t> EpsilonSq_Def;
  std::vector<Double_t> ToyUpMed_Def;
  std::vector<Double_t> ToyUpN2_Def;
  std::vector<Double_t> ToyUpN1_Def;
  std::vector<Double_t> ToyUpP1_Def;
  std::vector<Double_t> ToyUpP2_Def;
  // Vectors of important quantities - from secondary file
  std::vector<Double_t> Mass_Var;
  std::vector<Double_t> PVal_Var;
  std::vector<Double_t> UpperLim_Var;
  std::vector<Double_t> EpsilonSq_Var;
  std::vector<Double_t> ToyUpMed_Var;
  std::vector<Double_t> ToyUpN2_Var;
  std::vector<Double_t> ToyUpN1_Var;
  std::vector<Double_t> ToyUpP1_Var;
  std::vector<Double_t> ToyUpP2_Var;

  // Read input histogram and fill vector; bins as follows
  //  1 - Mass hypothesis     2 - Window size              3 - Polynomial order
  //  4 - Yield               5 - Error on yield
  //  6 - Pull                7 - Error on pull
  //  8 - Chi2 of bkg fit     9 - Chi2 of Bkg_Sig fit     10 - Chi2 of toy bkg fit
  // 11 - p value            12 - epsilon2
  // 13 - signal upper lim.  14 - radiative fraction
  // 15 - Median toy UL      16 - Toy UL; 2 sigma lo      17 - Toy UL; 1 sigma lo
  // 18 - Toy UL; 1 sigma hi 18 - Toy UL; 2 sigma hi
  
  // For default file
  TKey *keyDef;
  TList* contents = fInDefault->GetListOfKeys();
  contents->Sort();
  
  TIter nextDef(contents);
  while((keyDef = (TKey *)nextDef())){
    TClass *cl1 = gROOT->GetClass(keyDef->GetClassName());
    if(cl1->InheritsFrom("TH1")){
      TH1* h = (TH1*)keyDef->ReadObj();

      Mass_Def.push_back(h->GetBinContent(1));
      PVal_Def.push_back(h->GetBinContent(11));
      UpperLim_Def.push_back(h->GetBinContent(13));
      EpsilonSq_Def.push_back(h->GetBinContent(12));
      ToyUpMed_Def.push_back(h->GetBinContent(15));
      ToyUpN2_Def.push_back(h->GetBinContent(16));
      ToyUpN1_Def.push_back(h->GetBinContent(17));
      ToyUpP1_Def.push_back(h->GetBinContent(18));
      ToyUpP2_Def.push_back(h->GetBinContent(19));
    }
  }
  
  // For secondary file
  TKey *keyVar;
  contents = fInVaried->GetListOfKeys();
  contents->Sort();
  
  TIter nextVar(contents);
  while((keyVar = (TKey *)nextVar())){
    TClass *cl1 = gROOT->GetClass(keyVar->GetClassName());
    if(cl1->InheritsFrom("TH1")){
      TH1* h = (TH1*)keyVar->ReadObj();

      Mass_Var.push_back(h->GetBinContent(1));
      PVal_Var.push_back(h->GetBinContent(11));
      UpperLim_Var.push_back(h->GetBinContent(13));
      EpsilonSq_Var.push_back(h->GetBinContent(12));
      ToyUpMed_Var.push_back(h->GetBinContent(15));
      ToyUpN2_Var.push_back(h->GetBinContent(16));
      ToyUpN1_Var.push_back(h->GetBinContent(17));
      ToyUpP1_Var.push_back(h->GetBinContent(18));
      ToyUpP2_Var.push_back(h->GetBinContent(19));
    }
  }

  // CANVAS - p values
  TCanvas* c1 = new TCanvas("c1","c1",1500,1000);
  //c1->SetLogy();
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.05);

  // Calculate what is to be plotted from input arrays
  std::vector<Double_t> PVal_ToPlot;
  for(int i{0}; i<Mass_Def.size(); i++){
    PVal_ToPlot.push_back( 100*(PVal_Var[i] - PVal_Def[i])/PVal_Def[i] );
  }

  TGraph* gPVal = new TGraph(Mass_Def.size(),&Mass_Def[0],&PVal_ToPlot[0]);
  gPVal->SetTitle("");
  gPVal->SetMarkerColor(kBlue);
  gPVal->SetLineColor(kBlue);
  gPVal->SetMarkerStyle(8);
  gPVal->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gPVal->GetXaxis()->SetTitleSize(0.035);
  gPVal->GetXaxis()->CenterTitle();
  gPVal->GetXaxis()->SetTitleOffset(1.2);
  gPVal->GetYaxis()->SetTitle("% diff. in local p-value");
  gPVal->GetYaxis()->SetTitleSize(0.035);
  //gPVal->GetYaxis()->SetRangeUser(-150,200);
  gPVal->GetYaxis()->CenterTitle();
  gPVal->SetLineWidth(2);

  gPVal->Draw("AL");
  
  TPaveText *ptPVal = new TPaveText(0.15,0.12,0.5,0.22,"NDC");
  ptPVal->SetTextSize(0.035);
  ptPVal->SetBorderSize(0);
  ptPVal->AddText("Default: as JW");
  ptPVal->AddText("Comparison: Left VDC, cluster gap 2");
  ptPVal->SetFillStyle(0);
  ptPVal->Draw();
  
  // CANVAS - signal upper limits

  // Calculate what is to be plotted from input arrays
  std::vector<Double_t> UpperLim_ToPlot;
  std::vector<Double_t> ToyUpMed_ToPlot;
  std::vector<Double_t> ToyUp1Sig_ToPlot;
  std::vector<Double_t> ToyUp2Sig_ToPlot;

  Double_t OneSigWindow_Def{0};
  Double_t OneSigWindow_Var{0};
  Double_t TwoSigWindow_Def{0};
  Double_t TwoSigWindow_Var{0};

  for(int i{0}; i<Mass_Def.size(); i++){
    UpperLim_ToPlot.push_back( 100*(UpperLim_Var[i] - UpperLim_Def[i])/UpperLim_Def[i] );
    ToyUpMed_ToPlot.push_back( 100*(ToyUpMed_Var[i] - ToyUpMed_Def[i])/ToyUpMed_Def[i] );
  
    OneSigWindow_Def = ToyUpP1_Def[i]-ToyUpN1_Def[i];
    OneSigWindow_Var = ToyUpP1_Var[i]-ToyUpN1_Var[i];
    TwoSigWindow_Def = ToyUpP2_Def[i]-ToyUpN2_Def[i];
    TwoSigWindow_Var = ToyUpP2_Var[i]-ToyUpN2_Var[i];

    ToyUp1Sig_ToPlot.push_back( 100*(OneSigWindow_Var - OneSigWindow_Def)/OneSigWindow_Def );
    ToyUp2Sig_ToPlot.push_back( 100*(TwoSigWindow_Var - TwoSigWindow_Def)/TwoSigWindow_Def );
  }
  
  TCanvas* c2 = new TCanvas("c2","c2",1500,1000);
  //c2->SetLeftMargin(0.12);
  //c2->SetTopMargin(0.05);
  c2->Divide(2,2);
  
  // Pad 1: Signal upper limit
  c2->cd(1);
  TGraph* gUpperLim = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(UpperLim_ToPlot[0]));
  gUpperLim->SetTitle("Signal Upper Limit");
  gUpperLim->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gUpperLim->GetXaxis()->CenterTitle();
  gUpperLim->GetXaxis()->SetTitleOffset(1.2);
  gUpperLim->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gUpperLim->GetYaxis()->CenterTitle();
  //gUpperLim->GetYaxis()->SetRangeUser(-50,50);

  gUpperLim->SetMarkerStyle(8); 
  gUpperLim->SetMarkerColor(kBlack);
  gUpperLim->SetLineColor(kBlack);
  gUpperLim->SetLineWidth(2);
  gUpperLim->Draw("AL");
  
  // Pad 2: Median toy upper limit
  c2->cd(2);
  TGraph* gToyUpMed = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(ToyUpMed_ToPlot[0]));
  gToyUpMed->SetTitle("Median Toy Upper Limit");
  gToyUpMed->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gToyUpMed->GetXaxis()->CenterTitle();
  gToyUpMed->GetXaxis()->SetTitleOffset(1.2);
  gToyUpMed->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gToyUpMed->GetYaxis()->CenterTitle();
  //gToyUpMed->GetYaxis()->SetRangeUser(-50,50);
  gToyUpMed->SetLineStyle(7);
  gToyUpMed->SetLineWidth(3);
  gToyUpMed->Draw("AL");
 
  // Pad 3: 1 sigma window from toys
  c2->cd(3);

  TGraph* gToyUp1Sig = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(ToyUp1Sig_ToPlot[0]));
  gToyUp1Sig->SetTitle("1#sigma window");
  gToyUp1Sig->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gToyUp1Sig->GetXaxis()->CenterTitle();
  gToyUp1Sig->GetXaxis()->SetTitleOffset(1.2);
  gToyUp1Sig->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gToyUp1Sig->GetYaxis()->CenterTitle();
  //gToyUp1Sig->GetYaxis()->SetRangeUser(-50,50);
  gToyUp1Sig->SetLineStyle(9);
  gToyUp1Sig->SetLineColor(kYellow+2);
  gToyUp1Sig->SetLineWidth(3);
  gToyUp1Sig->Draw("AL");

  // Pad 3: 1 sigma window from toys
  c2->cd(4);

  TGraph* gToyUp2Sig = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(ToyUp2Sig_ToPlot[0]));
  gToyUp2Sig->SetTitle("2#sigma window");
  gToyUp2Sig->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gToyUp2Sig->GetXaxis()->CenterTitle();
  gToyUp2Sig->GetXaxis()->SetTitleOffset(1.2);
  gToyUp2Sig->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gToyUp2Sig->GetYaxis()->CenterTitle();
  //gToyUp2Sig->GetYaxis()->SetRangeUser(-50,50);
  gToyUp2Sig->SetLineStyle(9);
  gToyUp2Sig->SetLineColor(kGreen+2);
  gToyUp2Sig->SetLineWidth(3);
  gToyUp2Sig->Draw("AL");
  
  // CANVAS - sensitivity
  
  // Get arrays of eps^2 values - default
  vector <Double_t> EpsilonP2_Def = scaleEpsilon(EpsilonSq_Def,UpperLim_Def,ToyUpP2_Def);
  vector <Double_t> EpsilonP1_Def = scaleEpsilon(EpsilonSq_Def,UpperLim_Def,ToyUpP1_Def);
  vector <Double_t> EpsilonMed_Def = scaleEpsilon(EpsilonSq_Def,UpperLim_Def,ToyUpMed_Def);
  vector <Double_t> EpsilonN1_Def = scaleEpsilon(EpsilonSq_Def,UpperLim_Def,ToyUpN1_Def);
  vector <Double_t> EpsilonN2_Def = scaleEpsilon(EpsilonSq_Def,UpperLim_Def,ToyUpN2_Def);
  // Get arrays of eps^2 values - secondary
  vector <Double_t> EpsilonP2_Var = scaleEpsilon(EpsilonSq_Var,UpperLim_Var,ToyUpP2_Var);
  vector <Double_t> EpsilonP1_Var = scaleEpsilon(EpsilonSq_Var,UpperLim_Var,ToyUpP1_Var);
  vector <Double_t> EpsilonMed_Var = scaleEpsilon(EpsilonSq_Var,UpperLim_Var,ToyUpMed_Var);
  vector <Double_t> EpsilonN1_Var = scaleEpsilon(EpsilonSq_Var,UpperLim_Var,ToyUpN1_Var);
  vector <Double_t> EpsilonN2_Var = scaleEpsilon(EpsilonSq_Var,UpperLim_Var,ToyUpN2_Var);

  // Calculate what is to be plotted from input arrays
  std::vector<Double_t> EpsilonSq_ToPlot;
  std::vector<Double_t> EpsilonMed_ToPlot;
  std::vector<Double_t> Epsilon1Sig_ToPlot;
  std::vector<Double_t> Epsilon2Sig_ToPlot;

  OneSigWindow_Def = 0;
  OneSigWindow_Var = 0;
  TwoSigWindow_Def = 0;
  TwoSigWindow_Var = 0;

  for(int i{0}; i<Mass_Def.size(); i++){
    EpsilonSq_ToPlot.push_back( 100*(EpsilonSq_Var[i] - EpsilonSq_Def[i])/EpsilonSq_Def[i] );
    EpsilonMed_ToPlot.push_back( 100*(EpsilonMed_Var[i] - EpsilonMed_Def[i])/EpsilonMed_Def[i] );
  
    OneSigWindow_Def = EpsilonP1_Def[i]-EpsilonN1_Def[i];
    OneSigWindow_Var = EpsilonP1_Var[i]-EpsilonN1_Var[i];
    TwoSigWindow_Def = EpsilonP2_Def[i]-EpsilonN2_Def[i];
    TwoSigWindow_Var = EpsilonP2_Var[i]-EpsilonN2_Var[i];

    Epsilon1Sig_ToPlot.push_back( 100*(OneSigWindow_Var - OneSigWindow_Def)/OneSigWindow_Def );
    Epsilon2Sig_ToPlot.push_back( 100*(TwoSigWindow_Var - TwoSigWindow_Def)/TwoSigWindow_Def );
  }
  
  TCanvas* c3 = new TCanvas("c3","c3",1500,1000);
  //c3->SetLeftMargin(0.12);
  //c3->SetTopMargin(0.05);
  c3->Divide(2,2);
  
  // Pad 1: epsilon^2
  c3->cd(1);
  TGraph* gEpsilon = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(EpsilonSq_ToPlot[0]));
  gEpsilon->SetTitle("#epsilon^{2} - from data");
  gEpsilon->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gEpsilon->GetXaxis()->CenterTitle();
  gEpsilon->GetXaxis()->SetTitleOffset(1.2);
  gEpsilon->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gEpsilon->GetYaxis()->CenterTitle();
  //gEpsilon->GetYaxis()->SetRangeUser(-50,50);
  gEpsilon->SetMarkerStyle(8); 
  gEpsilon->SetMarkerColor(kBlack);
  gEpsilon->SetLineColor(kBlack);
  gEpsilon->SetLineWidth(2);
  gEpsilon->Draw("AL");
  
  // Pad 2: epsilon^2 from median toy UL
  c3->cd(2);
  TGraph* gEpsilonMed = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(EpsilonMed_ToPlot[0]));
  gEpsilonMed->SetTitle("#epsilon^{2} - from median toy");
  gEpsilonMed->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gEpsilonMed->GetXaxis()->CenterTitle();
  gEpsilonMed->GetXaxis()->SetTitleOffset(1.2);
  gEpsilonMed->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gEpsilonMed->GetYaxis()->CenterTitle();
  //gEpsilonMed->GetYaxis()->SetRangeUser(-50,50);
  gEpsilonMed->SetLineStyle(7);
  gEpsilonMed->SetLineWidth(3);
  gEpsilonMed->Draw("AL");
 
  // Pad 3: 1 sigma window from toys
  c3->cd(3);

  TGraph* gEpsilon1Sig = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(Epsilon1Sig_ToPlot[0]));
  gEpsilon1Sig->SetTitle("1#sigma window");
  gEpsilon1Sig->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gEpsilon1Sig->GetXaxis()->CenterTitle();
  gEpsilon1Sig->GetXaxis()->SetTitleOffset(1.2);
  gEpsilon1Sig->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gEpsilon1Sig->GetYaxis()->CenterTitle();
  //gEpsilon1Sig->GetYaxis()->SetRangeUser(-50,50);
  gEpsilon1Sig->SetLineStyle(9);
  gEpsilon1Sig->SetLineColor(kYellow+2);
  gEpsilon1Sig->SetLineWidth(3);
  gEpsilon1Sig->Draw("AL");

  // Pad 3: 1 sigma window from toys
  c3->cd(4);

  TGraph* gEpsilon2Sig = new TGraph(Mass_Def.size(),&(Mass_Def[0]),&(Epsilon2Sig_ToPlot[0]));
  gEpsilon2Sig->SetTitle("2#sigma window");
  gEpsilon2Sig->GetXaxis()->SetTitle("M_{e^{+}e^{-}} (MeV/c^{2})");
  gEpsilon2Sig->GetXaxis()->CenterTitle();
  gEpsilon2Sig->GetXaxis()->SetTitleOffset(1.2);
  gEpsilon2Sig->GetYaxis()->SetTitle("% diff. (comparison - default)");
  gEpsilon2Sig->GetYaxis()->CenterTitle();
  //gEpsilon2Sig->GetYaxis()->SetRangeUser(-50,50);
  gEpsilon2Sig->SetLineStyle(9);
  gEpsilon2Sig->SetLineColor(kGreen+2);
  gEpsilon2Sig->SetLineWidth(3);
  gEpsilon2Sig->Draw("AL");

  c1->SaveAs("plots/Diff_Pvals_DefVsLVDCGap0_UNBLINDED.png");
  c2->SaveAs("plots/Diff_UpperLimits_DefVsLVDCGap0_UNBLINDED.png");
  c3->SaveAs("plots/Diff_Epsilon_DefVsLVDCGap0_UNBLINDED2.png");

  return;
}
