//-----------------------------------------------------------------------------
// Code to plot chi2, pull and p-value from different WF tests
// Oliver Jevons; 04-May-23
//
// Inputs: 
// 1. Path to peak search output (TH1 created by PeasSearchReadable.cxx)
//
// Output:
// 1. Canvases showing chi2, pull and p-values as afunction of WF for...
// 2. 130, 155, 180 and 210 MeV
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <TString.h>
#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>

void plotWF(){
  // Load input files (default and all WF tests
  TFile* fIn[8];
  fIn[0] = new TFile("/scratch/oliver/fOut_JWNoZeroWFPlus5.root");
  fIn[1] = new TFile("/scratch/oliver/fOut_JWNoZeroWFPlus3.root");
  fIn[2] = new TFile("/scratch/oliver/fOut_JWNoZero.root");
  fIn[3] = new TFile("/scratch/oliver/fOut_JWNoZeroWFMinus1.root");
  fIn[4] = new TFile("/scratch/oliver/fOut_JWNoZeroWFMinus2.root");
  fIn[5] = new TFile("/scratch/oliver/fOut_JWNoZeroWFMinus3.root");
  fIn[6] = new TFile("/scratch/oliver/fOut_JWNoZeroWFMinus4.root");
  fIn[7] = new TFile("/scratch/oliver/fOut_JWNoZeroWFMinus5.root");
  
  // Check files exist
  for(int i{0}; i<8; i++){
    if(!fIn[i] || fIn[i]->IsZombie()){
      std::cout<<"Unable to open file "<<fIn[i]->GetName()<<" for reading..."<<std::endl;
      return;
    }
  }
  
  // Declare arrays of storage histograms and variables
  TH1F* hPSOutput[4];
  
  // Window sizes
  Float_t WF_130[8];
  Float_t WF_155[8];
  Float_t WF_180[8];
  Float_t WF_210[8];
  // Chi2
  Float_t Chi2_130[8];
  Float_t Chi2_155[8];
  Float_t Chi2_180[8];
  Float_t Chi2_210[8];
  // Pull
  Float_t Pull_130[8];
  Float_t Pull_155[8];
  Float_t Pull_180[8];
  Float_t Pull_210[8];
  // Pull error
  Float_t PullErr_130[8];
  Float_t PullErr_155[8];
  Float_t PullErr_180[8];
  Float_t PullErr_210[8];
  // P-values
  Float_t Pval_130[8];
  Float_t Pval_155[8];
  Float_t Pval_180[8];
  Float_t Pval_210[8];

  Float_t WFErr[8] = {0,0,0,0,0,0,0,0};

  // Run over all input files
  for(int file{0}; file<8; file++){
    hPSOutput[0] = (TH1F*)fIn[file]->Get("hOut_130.00");
    hPSOutput[1] = (TH1F*)fIn[file]->Get("hOut_155.00");
    hPSOutput[2] = (TH1F*)fIn[file]->Get("hOut_180.00");
    hPSOutput[3] = (TH1F*)fIn[file]->Get("hOut_210.00");
  
    
    WF_130[file] = hPSOutput[0]->GetBinContent(2);
    Chi2_130[file] = hPSOutput[0]->GetBinContent(9);
    Pull_130[file] = hPSOutput[0]->GetBinContent(6);
    PullErr_130[file] = hPSOutput[0]->GetBinContent(7);
    Pval_130[file] = hPSOutput[0]->GetBinContent(11);

    WF_155[file] = hPSOutput[1]->GetBinContent(2);
    Chi2_155[file] = hPSOutput[1]->GetBinContent(9);
    Pull_155[file] = hPSOutput[1]->GetBinContent(6);
    PullErr_155[file] = hPSOutput[1]->GetBinContent(7);
    Pval_155[file] = hPSOutput[1]->GetBinContent(11);

    WF_180[file] = hPSOutput[2]->GetBinContent(2);
    Chi2_180[file] = hPSOutput[2]->GetBinContent(9);
    Pull_180[file] = hPSOutput[2]->GetBinContent(6);
    PullErr_180[file] = hPSOutput[2]->GetBinContent(7);
    Pval_180[file] = hPSOutput[2]->GetBinContent(11);

    WF_210[file] = hPSOutput[3]->GetBinContent(2);
    Chi2_210[file] = hPSOutput[3]->GetBinContent(9);
    Pull_210[file] = hPSOutput[3]->GetBinContent(6);
    PullErr_210[file] = hPSOutput[3]->GetBinContent(7);
    Pval_210[file] = hPSOutput[3]->GetBinContent(11);
  }

  TCanvas* cChi2130 = new TCanvas("cChiSq130","cChiSq130",1500,1000);
  cChi2130->SetLogy();
  cChi2130->SetLeftMargin(0.12);
  cChi2130->SetTopMargin(0.1);
  TGraph* gChi2_130 = new TGraph(8, WF_130, Chi2_130);
  gChi2_130->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gChi2_130->GetYaxis()->SetTitle("#chi^{2}");
  gChi2_130->SetTitle("WF - 130 MeV");
  gChi2_130->GetYaxis()->SetRangeUser(1e-14,2);
  gChi2_130->SetMarkerStyle(22);
  gChi2_130->SetMarkerSize(2);
  gChi2_130->Draw("AP");

  TCanvas* cChi2155 = new TCanvas("cChiSq155","cChiSq155",1500,1000);
  cChi2155->SetLogy();
  cChi2155->SetLeftMargin(0.12);
  cChi2155->SetTopMargin(0.1);
  TGraph* gChi2_155 = new TGraph(8, WF_155, Chi2_155);
  gChi2_155->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gChi2_155->GetYaxis()->SetTitle("#chi^{2}");
  gChi2_155->SetTitle("WF - 155 MeV");
  gChi2_155->GetYaxis()->SetRangeUser(1e-14,2);
  gChi2_155->SetMarkerStyle(22);
  gChi2_155->SetMarkerSize(2);
  gChi2_155->Draw("AP");

  TCanvas* cChi2180 = new TCanvas("cChiSq180","cChiSq180",1500,1000);
  cChi2180->SetLogy();
  cChi2180->SetLeftMargin(0.12);
  cChi2180->SetTopMargin(0.1);
  TGraph* gChi2_180 = new TGraph(8, WF_180, Chi2_180);
  gChi2_180->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gChi2_180->GetYaxis()->SetTitle("#chi^{2}");
  gChi2_180->SetTitle("WF - 180 MeV");
  gChi2_180->GetYaxis()->SetRangeUser(1e-14,2);
  gChi2_180->SetMarkerStyle(22);
  gChi2_180->SetMarkerSize(2);
  gChi2_180->Draw("AP");

  TCanvas* cChi2210 = new TCanvas("cChiSq210","cChiSq210",1500,1000);
  cChi2210->SetLogy();
  cChi2210->SetLeftMargin(0.12);
  cChi2210->SetTopMargin(0.1);
  TGraph* gChi2_210 = new TGraph(8, WF_210, Chi2_210);
  gChi2_210->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gChi2_210->GetYaxis()->SetTitle("#chi^{2}");
  gChi2_210->SetTitle("WF - 210 MeV");
  gChi2_210->GetYaxis()->SetRangeUser(1e-14,2);
  gChi2_210->SetMarkerStyle(22);
  gChi2_210->SetMarkerSize(2);
  gChi2_210->Draw("AP");
  
  TCanvas* cPull130 = new TCanvas("cPull130","cPull130",1500,1000);
  cPull130->SetLeftMargin(0.12);
  cPull130->SetTopMargin(0.1);
  TGraphErrors* gePull_130 = new TGraphErrors(8,WF_130,Pull_130,WFErr,PullErr_130);
  gePull_130->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gePull_130->GetYaxis()->SetTitle("Pull");
  gePull_130->SetTitle("Pull - 130 MeV");
  gePull_130->GetYaxis()->SetRangeUser(-5,5);
  gePull_130->SetMarkerStyle(22);
  gePull_130->SetMarkerSize(2);
  gePull_130->Draw("AP");

  TCanvas* cPull155 = new TCanvas("cPull155","cPull155",1500,1000);
  cPull155->SetLeftMargin(0.12);
  cPull155->SetTopMargin(0.1);
  TGraphErrors* gePull_155 = new TGraphErrors(8,WF_155,Pull_155,WFErr,PullErr_155);
  gePull_155->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gePull_155->GetYaxis()->SetTitle("Pull");
  gePull_155->SetTitle("Pull - 155 MeV");
  gePull_155->GetYaxis()->SetRangeUser(-5,5);
  gePull_155->SetMarkerStyle(22);
  gePull_155->SetMarkerSize(2);
  gePull_155->Draw("AP");

  TCanvas* cPull180 = new TCanvas("cPull180","cPull180",1500,1000);
  cPull180->SetLeftMargin(0.12);
  cPull180->SetTopMargin(0.1);
  TGraphErrors* gePull_180 = new TGraphErrors(8,WF_180,Pull_180,WFErr,PullErr_180);
  gePull_180->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gePull_180->GetYaxis()->SetTitle("Pull");
  gePull_180->SetTitle("Pull - 180 MeV");
  gePull_180->GetYaxis()->SetRangeUser(-5,5);
  gePull_180->SetMarkerStyle(22);
  gePull_180->SetMarkerSize(2);
  gePull_180->Draw("AP");

  TCanvas* cPull210 = new TCanvas("cPull210","cPull210",1500,1000);
  cPull210->SetLeftMargin(0.12);
  cPull210->SetTopMargin(0.1);
  TGraphErrors* gePull_210 = new TGraphErrors(8,WF_210,Pull_210,WFErr,PullErr_210);
  gePull_210->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gePull_210->GetYaxis()->SetTitle("Pull");
  gePull_210->SetTitle("Pull - 210 MeV");
  gePull_210->GetYaxis()->SetRangeUser(-5,5);
  gePull_210->SetMarkerStyle(22);
  gePull_210->SetMarkerSize(2);
  gePull_210->Draw("AP");

  TCanvas* cPval130 = new TCanvas("cPval130","cPval130",1500,1000);
  cPval130->SetLogy();
  cPval130->SetLeftMargin(0.12);
  cPval130->SetTopMargin(0.1);
  TGraph* gPval_130 = new TGraph(8, WF_130, Pval_130);
  gPval_130->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gPval_130->GetYaxis()->SetTitle("p-value");
  gPval_130->SetTitle("WF - 130 MeV");
  gPval_130->GetYaxis()->SetRangeUser(1e-10,1.5);
  gPval_130->SetMarkerStyle(22);
  gPval_130->SetMarkerSize(2);
  gPval_130->Draw("AP");

  TCanvas* cPval155 = new TCanvas("cPval155","cPval155",1500,1000);
  cPval155->SetLogy();
  cPval155->SetLeftMargin(0.12);
  cPval155->SetTopMargin(0.1);
  TGraph* gPval_155 = new TGraph(8, WF_155, Pval_155);
  gPval_155->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gPval_155->GetYaxis()->SetTitle("p-value");
  gPval_155->SetTitle("WF - 155 MeV");
  gPval_155->GetYaxis()->SetRangeUser(1e-10,1.5);
  gPval_155->SetMarkerStyle(22);
  gPval_155->SetMarkerSize(2);
  gPval_155->Draw("AP");

  TCanvas* cPval180 = new TCanvas("cPval180","cPval180",1500,1000);
  cPval180->SetLogy();
  cPval180->SetLeftMargin(0.12);
  cPval180->SetTopMargin(0.1);
  TGraph* gPval_180 = new TGraph(8, WF_180, Pval_180);
  gPval_180->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gPval_180->GetYaxis()->SetTitle("p-value");
  gPval_180->SetTitle("WF - 180 MeV");
  gPval_180->GetYaxis()->SetRangeUser(1e-10,1.5);
  gPval_180->SetMarkerStyle(22);
  gPval_180->SetMarkerSize(2);
  gPval_180->Draw("AP");

  TCanvas* cPval210 = new TCanvas("cPval210","cPval210",1500,1000);
  cPval210->SetLogy();
  cPval210->SetLeftMargin(0.12);
  cPval210->SetTopMargin(0.1);
  TGraph* gPval_210 = new TGraph(8, WF_210, Pval_210);
  gPval_210->GetXaxis()->SetTitle("Window factor [n_{#sigma}]");
  gPval_210->GetYaxis()->SetTitle("p-value");
  gPval_210->SetTitle("WF - 210 MeV");
  gPval_210->GetYaxis()->SetRangeUser(1e-10,1.5);
  gPval_210->SetMarkerStyle(22);
  gPval_210->SetMarkerSize(2);
  gPval_210->Draw("AP");

  
  cChi2130->Print("apex_wf_chi2_130.pdf");
  cChi2155->Print("apex_wf_chi2_155.pdf");
  cChi2180->Print("apex_wf_chi2_180.pdf");
  cChi2210->Print("apex_wf_chi2_210.pdf");
  cPull130->Print("apex_wf_pull_130.pdf");
  cPull155->Print("apex_wf_pull_155.pdf");
  cPull180->Print("apex_wf_pull_180.pdf");
  cPull210->Print("apex_wf_pull_210.pdf");
  cPval130->Print("apex_wf_pval_130.pdf");
  cPval155->Print("apex_wf_pval_155.pdf");
  cPval180->Print("apex_wf_pval_180.pdf");
  cPval210->Print("apex_wf_pval_210.pdf");

  gSystem->Exec("pdfunite apex_wf*.pdf WFVarTest_5May23.pdf");
  gSystem->Exec("rm apex_wf*.pdf");

  return 0;
}

