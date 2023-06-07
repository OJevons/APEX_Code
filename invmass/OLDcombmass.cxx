//-----------------------------------------------------------------------------
// Code to combine ROOT histograms from invariant mass jobs
// Oliver Jevons; 04-Nov-2022
//
// Inputs: 
// 1. Path to job output
// 2. If final file is to be blinded or not
//-----------------------------------------------------------------------------

#include <iostream>
#include <TFile.h>
#include <TString.h>

const TString sFilePath = "/w/work2/home/oliver/APEX/invmass/TrimAsJW/";

void combmass(Bool_t IsBlind=false){
  TString sFilesToCombine = sFilePath + "/root-output/*.root";
  TString sCombinedFile = sFilePath + "/combinedmass.root";
  TString sHaddCommand = "hadd -f " + sCombinedFile + " " + sFilesToCombine;

  gSystem->Exec(sHaddCommand);
 
  TFile fRawMass(sCombinedFile, "READ");
  TH1F* hRawMass = (TH1F*)fRawMass.Get("masshist");

  std::cout<<"masshist has "<<hRawMass->GetEntries()<<" entries"<<std::endl;
  
  TH1F* hMassRebin = new TH1F("masshist","Calculated invariant mass (GeV)",4000,0.1,0.3); // bin width defined as 0.05 MeV
  hMassRebin->Rebin(3); // Rebinning to define bin width as 0.15 MeV
  
  // Run over all events in mass histogram - if blinded, only take 10% of events
  TString hFileOut;
  for(int i{0}; i<){
    if(IsBlind){
    
    }
  }

  return 0;
}
