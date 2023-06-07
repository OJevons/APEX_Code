//-----------------------------------------------------------------------------
// Code to copy peak search files to new directory
// Oliver Jevons; 15-Feb-2023
//
// Inputs: 
// 1. Path to .csv file containing parameters
// 2. Path to folder to copy files from
// 3. Path to folder to copy files to
//
// Output:
// 1. .csv file with parameters of missing jobs
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <TString.h>
#include <string>

// Change input paths here
const TString sRunsFileName = "../peaksearch/PSParams/mass_params_blindchoice.csv";
const TString sCopyFrom = "/w/work5/jlab/halla/APEX/mass_search/";
const TString sCopyTo = "/w/work2/home/oliver/APEX/peaksearch/Summer2022Files/";

void copyfiles(){
  // Storage variables: run number, input filestream, final file path to check, output filestreams
  string sParamString;
  ifstream fRunsFile(sRunsFileName);
  TString sFileName;
  TString copycommand;

  // Read in from runlist file line-by-line
  while(std::getline(fRunsFile,sParamString)){
    // Need to convert from type <string> (needed for getline command) to type <TString> for ROOT file access. 
    TString sParamStringHold(sParamString);
    
    // TStrings for holding parameter values
    TString sMASS_H, sPO, sTPO, sWF, sNBg, sNSig, sMT;

    // Don't bother looking for file using first line of parameter list (just for titles)
    if(sParamStringHold.Contains("MASS_H")) continue;
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

      sFileName = "bhToys_0.15Bin_M" + sMT + "m" + sMASS_H + "w" + sWF + "p" + sPO + "r100s" + sNSig + "N" + sNBg + "P" + sTPO + ".root";
     
      copycommand = "cp " + sCopyFrom + sFileName + " " + sCopyTo + sFileName;
      
      gSystem->Exec(copycommand);
    }
  }

  return 0;
}
