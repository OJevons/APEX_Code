//-----------------------------------------------------------------------------
// Code to check if output files from jobs exist - FOR PEAK SEARCH
// Oliver Jevons; 16-Nov-2022
//
// Inputs: 
// 1. Path to .csv file containing parameters
// 2. Path to folder to be checked for job output
//
// Output:
// 1. .csv file with parameters of missing jobs
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <TString.h>
#include <string>

// Change input paths here
//const TString sRunsFileName = "../peaksearch/PSParams/mass_params_blindchoice.csv";
const TString sRunsFileName = "../peaksearch/PSParams/mass_params_WF9.csv";
const TString sPathToCheck = "/w/work2/home/oliver/APEX/peaksearch/NoZero_WFConst9/";

void checkfile_peaksearch(){
  // Storage variables: run number, input filestream, final file path to check, output filestreams
  string sParamString;
  ifstream fRunsFile(sRunsFileName);
  TString sFileName;

  ofstream fFilesFound("mass_params_FOUND.csv");
  ofstream fFilesMissing("mass_params_MISSING.csv");
  
  // Use counter to check if runlist is complete, or how many are missing.
  Int_t Count{0};
  
  // Read in from runlist file line-by-line
  while(std::getline(fRunsFile,sParamString)){
    // Need to convert from type <string> (needed for getline command) to type <TString> for ROOT file access. 
    TString sParamStringHold(sParamString);
    
    // TStrings for holding parameter values
    TString sMASS_H, sPO, sTPO, sWF, sNBg, sNSig, sMT;

    // Print first line of .csv to both output files (headings needed for peak search submitter)
    // If looking at header line, don't bother checking for files.
    if(sParamStringHold.Contains("MASS_H")){
      fFilesFound << sParamStringHold << std::endl;
      fFilesMissing << sParamStringHold << std::endl;
    }
    else{
      // Parameter format: Mass hypothesis; Polynomial order; Toy polynomial order; Mass window ssize; No. of bkg. events; No. of signal events; MT(?)
      // MASS_H, PO, TPO, WF, NBg, NSig, MT

      // Need to pass line TString to each varaible TString independently, as TString::Remove() operations modify the initial TString.
      sMASS_H = sParamStringHold;
      sMASS_H.Remove(sMASS_H.Index(","));

      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sPO = sParamStringHold;
      sPO.Remove(sPO.Index(","));

      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sTPO = sParamStringHold;
      sTPO.Remove(sTPO.Index(","));

      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sWF = sParamStringHold;
      sWF.Remove(sWF.Index(","));
      
      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sNBg = sParamStringHold;
      sNBg.Remove(sNBg.Index(","));

      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sNSig = sParamStringHold;
      sNSig.Remove(sNSig.Index(","));

      sParamStringHold.Remove(0,sParamStringHold.Index(",")+1);
      sMT = sParamStringHold;

      sFileName = sPathToCheck + "bhToys_0.15Bin_M" + sMT + "m" + sMASS_H + "w" + sWF + "p" + sPO + "r100s" + sNSig + "N" + sNBg + "P" + sTPO + ".root";
     
      // Check if ROOT can find the relevant file.
      // If YES, save to 'FOUND' file; if NO, save to 'MISSING' file
      if(!gSystem->AccessPathName(sFileName,kFileExists)){
	fFilesFound << sParamString << std::endl;
      }
      else {
	fFilesMissing << sParamString << std::endl;
	Count++;
      }   
    }
  }

  if(Count == 0) std::cout<<"Runlist COMPLETE."<<std::endl;
  else std::cout<<"Still MISSING "<<Count<<" runs."<<std::endl;

  return 0;
}
