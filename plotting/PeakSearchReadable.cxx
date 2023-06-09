//-----------------------------------------------------------------------------
// Code to parse output of HPSTR bump hunt; putting values into TH1
// Oliver Jevons; 16-Nov-22
//
// Inputs: 
// 1. Path to peak search output
//
// Output:
// 1. .root files containing a histogram
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <TString.h>
#include <TH1.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>

const TString sPeakSearchOutDir = "/w/work2/home/oliver/APEX/peaksearch/NoZero_WFConst9";
//const TString sPeakSearchOutDir = "/w/work2/home/oliver/APEX/peaksearch/Summer2022Files";
//const TString sPeakSearchOutDir = "/w/work2/home/oliver/APEX/peaksearch/JWTrim50k";  // Default, as John 50k
const char* ext = ".root";

// From filename get value of parameters of mass search
TString FindVal(TString filename, TString prev, TString next){

  Int_t dot = filename.First(prev.Data());
  filename = filename.Remove(0,dot+1);
  Int_t len = filename.Length();
  Int_t dash = filename.First(next.Data());  
  filename = filename.Remove(dash,len-dash);

  return filename;
  
}

// Retrieve median and +/-1,2-sigma confidence interval from histrogram
std::vector<Double_t> GetCI(TH1F* hist){

  const Int_t nq = 5;
  Double_t xq[nq];  // position where to compute the quantiles in [0,1]
  Double_t yq[nq] = {0.0455,0.3171,0.5,0.6827,0.9554};  // array to contain the quantiles
  
  ROOT::Math::IntegratorOneDimOptions::SetDefaultAbsTolerance(1.E-6);
  ROOT::Math::IntegratorOneDimOptions::SetDefaultRelTolerance(1.E-6);

  hist->GetQuantiles(nq,xq,yq);
  
  std::vector<Double_t> v(xq, xq + sizeof xq / sizeof xq[0]);
  return v;

}

// Retrieve value for radiative fraction
Double_t getf(Double_t mass){ // mass in MeV

  Double_t m = -0.000544087; // 0.0005 value for APEX 2010
  Double_t c = 0.274032; //  0.125 value for APEX 2010

  Double_t f = m*mass + c;
  return f;
  
}

// Retrieve value for N_eff
Double_t getNeff(Double_t mass){ // mass in MeV

  std::ifstream inFile("Neff/Neff_new.txt");

  // masses and corresponding Neffs above and below input mass in table
  Double_t mass_h = 100;
  Double_t mass_l = 0.0;
  Double_t Neff_h = 100;
  Double_t Neff_l = 0.0;

  Double_t massC = 0.0; // current
  Double_t massP = 0.0; //previous
  Double_t NeffC = 0.0; // current
  Double_t NeffP = 0.0; //previous
    
  if (inFile.is_open()){
    std::string line;

      
    while( std::getline(inFile,line) ){
      std::stringstream ss(line);
	  
      massP = massC;
      NeffP = NeffC;
	  
      std::string massSt, NeffSt;
      std::getline(ss,massSt,',');   // std::cout<<"\""<<massSt<<"\"";
      std::getline(ss,NeffSt,','); //std::cout<<", \""<<NeffSt<<"\"";

      massC = std::stod(massSt);
      NeffC = std::stod(NeffSt);
	  
      if(massC>mass){
	mass_l = massP;
	mass_h = massC;
	Neff_l = NeffP;
	Neff_h = NeffC;
	//	    std::cout<<"\n";	  
	break;
      }	    
      //std::cout<<"\n";	  
	  
    }
  }

  Double_t Neff = 0.0;

  Neff = Neff_l + ((mass-mass_l)/(mass_h-mass_l))*(Neff_h-Neff_l);
  
  return Neff;  
}

void FillHist(TString filename, TH1F* hOut, Int_t NSig, Double_t f, Double_t Neff){
  TFile* f1 = new TFile(filename.Data());

  TTree* T = f1->Get<TTree>("fit_toys");
  

  TH1F* hTSigYield = new TH1F("hTSigYield","hTSigYield",300,-5000,5000);
  T->Draw("toy_sig_yield>>hTSigYield","","goff");
  Double_t meanYield = hTSigYield->GetMean();
  
  TH1F* hTSigYieldErr = new TH1F("hTSigYieldErr","hTSigYieldErr",300,-5000,5000);
  T->Draw("toy_sig_yield_err>>hTSigYieldErr","","goff");
  Double_t meanYieldErr = hTSigYieldErr->GetMean();

  TH1F* hPull = new TH1F("hPull","hPull",100,-100,100);
  T->Draw(Form("(toy_sig_yield-%i)/toy_sig_yield_err>>hPull",NSig),"","goff");
  Double_t meanPull = hPull->GetMean();
  Double_t meanPullErr = hPull->GetRMS();  
  
  TH1F* hBkChi2P = new TH1F("hBkChi2P","hBkChi2P",300,0,1);
  T->Draw("bkg_chi2_prob>>hBkChi2P","","goff");
  Double_t meanBkChi2P = hBkChi2P->GetMean();

  TH1F* hBkSigChi2P = new TH1F("hBkSigChi2P","hBkSigChi2P",300,0,1);
  T->Draw("bkgsig_chi2_prob>>hBkSigChi2P","","goff");
  Double_t meanBkSigChi2P = hBkSigChi2P->GetMean();

  TH1F* hToyChi2P = new TH1F("hToyChi2P","hToyChi2P",300,0,1);
  T->Draw("toyfit_chi2_prob>>hToyChi2P","","goff");
  Double_t meanToyChi2P = hToyChi2P->GetMean();  

  //TH1F* hPVal = new TH1F("hPVal","hPVal",300,0,1);
  TH1F* hPVal = new TH1F("hPVal","hPVal",300,0,1.5);
  T->Draw("p_value>>hPVal","","goff");
  Double_t meanPVal = hPVal->GetMean();
 
  Double_t alpha =  0.0072973525693; // fine structure constant

  TH1F* hmass_hypo = new TH1F("hmass_hypo","hmass_hypo",300,-5000,5000);
  T->Draw("mass_hypo>>hmass_hypo","","goff");
  Double_t mass_hypo = hmass_hypo->GetMean();  
  
  TH1* masshist = f1->Get<TH1>("masshist");
  //TH1* masshist = f1->Get<TH1>("mashist");
  TF1 *fBg = masshist->GetFunction("bkg"); // retrieve background fit
  Double_t bg_rate = (fBg->Integral(mass_hypo-0.0005,mass_hypo+0.0005))*(1/0.00015); // need to scale integral according to bin width
   
  TH1F* hEpsilon = new TH1F("hEpsilon","hEpsilon,",300,0,1);

  T->Draw(Form("(2*(1/137)*upper_limit*%f)/(3*pi*%f*%f*(1000*mass_hypo))>>hEpsilon",Neff,f,bg_rate),"","goff"); // 0.15
 
  Double_t meanEpsilon = hEpsilon->GetMean();

  TH1F* hUpperLim = new TH1F("hUpperLim","hUpperLim",500000,0,50000);
  T->Draw("upper_limit>>hUpperLim","","goff");
  Double_t meanUpperLim = hUpperLim->GetMean();

  TH1F* hToyUpperLim = new TH1F("hToyUpperLim","hToyUpperLim",450000,0,45000);
  T->Draw("toy_upper_limit>>hToyUpperLim","","goff");
 
  std::vector<Double_t> ToyCI = GetCI(hToyUpperLim);
  /*for (auto & ToyVal : ToyCI) {
    cout << "ToyVal = " << ToyVal << endl;
    }*/
    
  // bkg_rate_mass_hypo - background rate part
  // toy_bkg_chi2_prob - generator chi^2
  // toyfit_chi2_prob - also generator chi^2
  
  // Find window size from file name
  // Index
  Int_t slash = filename.Last('/');
  TString nfilename = filename.Remove(0,slash+1);   
  
  TString MH = FindVal(nfilename,"m",'w'); // mass
  TString WF = FindVal(nfilename,'w','p'); // window scaling factor
  TString PO = FindVal(nfilename,'p','r'); // polynomial order
  TString RF = FindVal(nfilename,'r','s'); // resolution factor
  
  hOut->Fill("Mass",std::stod(MH.Data()));
  hOut->Fill("WF",std::stod(WF.Data()));
  hOut->Fill("PO",std::stod(PO.Data()));
  hOut->Fill("Yield",meanYield);
  hOut->Fill("Yield_Err",meanYieldErr);
  hOut->Fill("Pull",meanPull);
  hOut->Fill("Pull_Err",meanPullErr);
  hOut->Fill("Bkg_Chi2",meanBkChi2P);
  hOut->Fill("BkgSig_Chi2",meanBkSigChi2P);
  hOut->Fill("ToyBkg_Chi2",meanToyChi2P);
  hOut->Fill("PVal",meanPVal);
  hOut->Fill("epsilon2",meanEpsilon);
  hOut->Fill("upper_limit",meanUpperLim);
  hOut->Fill("Rad_Frac",f);
  hOut->Fill("Toy_UL",ToyCI[2]);
  hOut->Fill("Toy_UL_2Lo",ToyCI[0]);
  hOut->Fill("Toy_UL_1Lo",ToyCI[1]);
  hOut->Fill("Toy_UL_1Hi",ToyCI[3]);
  hOut->Fill("Toy_UL_2Hi",ToyCI[4]);

  f1->Close();

  std::cout<<"For mass "<<std::stod(MH.Data())<<" MeV, p-value is "<<meanPVal<<std::endl;
}

void PeakSearchReadable(){
  
  // Access peak search output directory
  TSystemDirectory dir(sPeakSearchOutDir.Data(),sPeakSearchOutDir.Data());
  TList *files = dir.GetListOfFiles();
  files->Sort();
  
  std::cout<<"Looking in directory "<<dir.GetName()<<std::endl;
  std::cout<<files->LastIndex()<<" files in search directory"<<std::endl;
  Int_t NoOfFiles = files->LastIndex()-1;
  
  TH1F* hOut[NoOfFiles];
  //TH1F* hOut = new TH1F("hOut","Output from peak search",19,0,18);

  Int_t Counter{0};
  
  // Run over all files in dir
  // If file is .root, read and fill histogram
  if(files){
    TFile *file;
    TString fname;
    TIter next(files);

    while((file=(TFile*)next())){
      fname = Form("%s/%s",sPeakSearchOutDir.Data(),file->GetName());
      //cout << "Looking at file = " << fname << endl;
      if(fname.EndsWith(ext)){
	// Need to recover mass hypothesis from file name
	Int_t slash = fname.Last('/');
	TString nfilename = fname;   
	nfilename.Remove(0,slash+1);   
	TString MH = FindVal(nfilename,"m",'w'); // mass
	
	TString sHistName = "hOut_"+MH;
	if(!MH.Contains(".")) sHistName += ".00"; // Manually add ".00" to integer mass histograms, for ease in later plotting stages

	hOut[Counter] = new TH1F(sHistName,"Output from peak search",19,0,18);

	Double_t mass = std::stod(MH.Data());
	
	// Get values of Neff and f for mass
	Double_t Neff = getNeff(mass/1000.0);
	Double_t f = getf(mass);
	
	Int_t NSig{0};
	
	FillHist(fname,hOut[Counter],NSig,f,Neff);

	Counter++;
      }
    }
  }
  
  //TFile* fOut = new TFile("/home/oliver/APEX/APEX_Code/plotting/rootfiles/fOut_JWTrim50k.root","RECREATE");
  //TFile* fOut = new TFile("/scratch/oliver/fOut_JWMaster.root","RECREATE");
  TFile* fOut = new TFile("/scratch/oliver/fOut_NoZero_WFConst9.root","RECREATE");
  //TFile* fOut = new TFile("/scratch/oliver/fOut_JWTrim50k.root","RECREATE");
  for(int i{0}; i<NoOfFiles; i++){ hOut[i]->Write(); }
  fOut->Close();
  return 0;
}
