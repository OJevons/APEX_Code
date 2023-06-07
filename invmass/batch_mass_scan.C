//////////////////////////////////////
// This script will be used to run 
// the mass script in the batch system.
// This script will run the already compiled
// version of mass scan. This script will need
// the run number!!
/////////////////////////////////////////

void batch_mass_scan(int run){
  gROOT->LoadMacro("mass_files_C.so");
  gROOT->ProcessLine(Form("mass_files(%d)",run));
}
