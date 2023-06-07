//-----------------------------------------------------------------------------
// Code to modify APEX invariant mass histograms (no 0-content bins)
// Oliver Jevons; 02-Mar-2023
//
// Inputs: 
// 1. Path to folder containing relevant files
// 1. ROOT file name containing invariant mass histogram
//
// Output:
// 1. ROOT file containing modified histogram
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <TString.h>
#include <string>

// Path to input file
const TString sFilePath = "/w/work2/home/oliver/APEX/invmass/TrimAsJW/";
const TString sInFileName = "combmass_15MeV.root";
const TString sOutFileName = "combmass_15MeV_NoZero.root";

void masshistonozero(){
  // Load input file and histogram
  TString sFileIn = sFilePath + sInFileName;
  TFile* fIn = new TFile(sFileIn);
  if(!fIn || fIn->IsZombie()){
    std::cout<<"Unable to open file "<<sFileIn<<" for reading..."<<std::endl;
    return;
  }
  TH1F* hMassIn = (TH1F*)fIn->Get("masshist");
  
  // Copy input histogram to output histogram
  TH1F* hMassOut = (TH1F*)hMassIn->Clone();

  // Run over all bins in histogram
  for(int bin{1}; bin<hMassIn->GetNbinsX(); bin++){
    // If bin content = 0, replace with 1
    if(hMassIn->GetBinContent(bin) == 0) hMassOut->SetBinContent(bin,1);
  }

  // Write output histogram to output file
  TString sFileOut = sFilePath + sOutFileName;
  TFile* fOut = new TFile(sFileOut,"RECREATE");
  hMassOut->Write();

  return;
}
