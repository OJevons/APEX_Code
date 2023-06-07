//-----------------------------------------------------------------------------
// Code to check if output files from jobs exist.
// Oliver Jevons; 03-Nov-2022
//
// Inputs: 
// 1. Path to run list
// 2. Path to folder to be checked for job output
//
// Output:
// 1. .txt files containing run numbers of found and missing jobs
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <TString.h>
#include <string>

// Change input paths here
const TString sRunsFileName = "../runlists/production_runlist_new.txt";
const TString sPathToCheckReplays = "/w/work2/home/oliver/APEX/replays/50kReplay_LVDCgap2/";
const TString sPathToCheckMass = "/w/work2/home/oliver/APEX/invmass/TrimAsJW/root-output/";

// Default to checking replays
void checkfile(Bool_t kReplays = false){
  // Storage variables: run number, input filestream, final file path to check, output filestreams
  string sRunNo;
  ifstream fRunsFile(sRunsFileName);
  TString sFileName;

  // Print whether checking replay or mass jobs
  if(kReplays){
    std::cout<<"Checking REPLAY jobs; directory = "<<sPathToCheckReplays<<std::endl;
  }
  else if(!kReplays){
    std::cout<<"Checking MASS jobs; directory = "<<sPathToCheckMass<<std::endl;
  }
  
  ofstream fRunsFound("production_runlist_FOUND.txt");
  ofstream fRunsMissing("production_runlist_MISSING.txt");
  
  // Use counter to automatically detect if runlist is complete
  Int_t Count{0};

  // Read in from runlist file line-by-line
  while(std::getline(fRunsFile,sRunNo)){
    // Need to convert from type <string> (needed for getline command) to type <TString> for ROOT file access. 
    TString sRunNoHold(sRunNo);
    
    // Checking replay directory
    if(kReplays){
      sFileName = sPathToCheckReplays + "apex_Trim_" + sRunNoHold + ".root";
    }
    // Checking mass directory
    else if(!kReplays){
      sFileName = sPathToCheckMass + "mass_" + sRunNoHold + ".root";
    }

    // Check if ROOT can find the relevant file.
    // If YES, save to 'FOUND' file; if NO, save to 'MISSING' file
    if(!gSystem->AccessPathName(sFileName,kFileExists)){
      fRunsFound << sRunNo << std::endl;
    }
    else {
      fRunsMissing << sRunNo << std::endl;
      Count++;
    }
  }

  if(Count == 0) std::cout<<"Runlist COMPLETE."<<std::endl;
  else std::cout<<"Still MISSING "<<Count<<" runs."<<std::endl;

  return 0;
}
