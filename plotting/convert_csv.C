/********************************************************************
// John Williamson, 26/10/21

convert output from mass search from root file to csv (for convenience)

********************************************************************/



// from filename get value of parameters of mass search
TString FindVal(TString filename, TString prev, TString next){

  Int_t dot = filename.First(prev.Data());
  filename = filename.Remove(0,dot+1);
  Int_t len = filename.Length();
  Int_t dash = filename.First(next.Data());  
  filename = filename.Remove(dash,len-dash);

  return filename;
  
}

// retrieve median and +/-1,2-sigma confidence interval from histrogram
std::vector<Double_t> GetCI(TH1F* hist){

  const Int_t nq = 5;
  Double_t xq[nq];  // position where to compute the quantiles in [0,1]
  Double_t yq[nq] = {0.0455,0.3171,0.5,0.6827,0.9554};  // array to contain the quantiles
  
  hist->GetQuantiles(nq,xq,yq);
  
  //  for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;

  std::vector<Double_t> v(xq, xq + sizeof xq / sizeof xq[0]);
  return v;

}



// retrieve value for radiative fraction
Double_t getf(Double_t mass){ // mass in MeV

  Double_t m = -0.000544087; // 0.0005 value for APEX 2010
  Double_t c = 0.274032; //  0.125 value for APEX 2010

  Double_t f = m*mass + c;
  return f;
  
}

// retrieve value for N_eff
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

void WriteCsv(TString filename, ofstream& csvFile, Int_t NSig, Double_t f, Double_t Neff){

  TFile* f1 = new TFile(filename.Data());

  
  TTree* T = f1->Get<TTree>("fit_toys");

  TH1F* hTSigYield = new TH1F("hTSigYield","hTSigYield",300,-5000,5000);
  T->Draw("toy_sig_yield>>hTSigYield","","goff");
  Double_t meanYield = hTSigYield->GetMean();
  
  TH1F* hTSigYieldErr = new TH1F("hTSigYieldErr","hTSigYieldErr",300,-5000,5000);
  T->Draw("toy_sig_yield_err>>hTSigYieldErr","","goff");
  Double_t meanYieldErr = hTSigYieldErr->GetMean();

  //  TH1F* hPull = new TH1F("hPull","hPull",300,-5000,5000);
  TH1F* hPull = new TH1F("hPull","hPull",100,-100,100);
  T->Draw(Form("(toy_sig_yield-%i)/toy_sig_yield_err>>hPull",NSig),"","goff");
  Double_t meanPull = hPull->GetMean();
  Double_t meanPullErr = hPull->GetRMS();  
  
  TH1F* hBkChi2P = new TH1F("hBkChi2P","hBkChi2P",300,0,1);
  //  T->Draw("toy_bkg_chi2_prob>>hBkChi2P","","goff");
  T->Draw("bkg_chi2_prob>>hBkChi2P","","goff");
  Double_t meanBkChi2P = hBkChi2P->GetMean();


  TH1F* hBkSigChi2P = new TH1F("hBkSigChi2P","hBkSigChi2P",300,0,1);
  T->Draw("bkgsig_chi2_prob>>hBkSigChi2P","","goff");
  Double_t meanBkSigChi2P = hBkSigChi2P->GetMean();

  
  // TH1F* hToyChi2P = new TH1F("hToyChi2P","hToyChi2P",300,0,1);
  // T->Draw("toy_bkg_chi2_prob>>hToyChi2P","","goff");
  // Double_t meanToyChi2P = hToyChi2P->GetMean();

  TH1F* hToyChi2P = new TH1F("hToyChi2P","hToyChi2P",300,0,1);
  T->Draw("toyfit_chi2_prob>>hToyChi2P","","goff");
  Double_t meanToyChi2P = hToyChi2P->GetMean();  


  TH1F* hPVal = new TH1F("hPVal","hPVal",300,0,1);
  T->Draw("p_value>>hPVal","","goff");
  Double_t meanPVal = hPVal->GetMean();
 

  Double_t alpha =  0.0072973525693; // fine structure constant

  TH1F* hmass_hypo = new TH1F("hmass_hypo","hmass_hypo",300,-5000,5000);
  T->Draw("mass_hypo>>hmass_hypo","","goff");
  Double_t mass_hypo = hmass_hypo->GetMean();  
  
  TH1* masshist = f1->Get<TH1>("masshist");
  TF1 *fBg = masshist->GetFunction("bkg"); // retrieve background fit
  Double_t bg_rate = (fBg->Integral(mass_hypo-0.0005,mass_hypo+0.0005))*(1/0.00015); // need to scale integral according to bin width
   
  TH1F* hEpsilon = new TH1F("hEpsilon","hEpsilon,",300,0,1);

  T->Draw(Form("(2*(1/137)*upper_limit*%f)/(3*pi*%f*%f*(1000*mass_hypo))>>hEpsilon",Neff,f,bg_rate),"","goff"); // 0.15
  
  // //  T->Draw(Form("(2*(1/137)*upper_limit*%f)/(3*pi*%f*4*bkg_rate_mass_hypo*(1000*mass_hypo))>>hEpsilon",Neff,f),"","goff"); // 0.25
  // T->Draw(Form("(2*(1/137)*upper_limit*%f)/(3*pi*%f*6.67*bkg_rate_mass_hypo*(1000*mass_hypo))>>hEpsilon",Neff,f),"","goff"); // 0.15


  Double_t meanEpsilon = hEpsilon->GetMean();

  TH1F* hUpperLim = new TH1F("hUpperLim","hUpperLim",100000,0,10000);
  T->Draw("upper_limit>>hUpperLim","","goff");
  Double_t meanUpperLim = hUpperLim->GetMean();

  TH1F* hToyUpperLim = new TH1F("hToyUpperLim","hToyUpperLim",100000,0,10000);
  T->Draw("toy_upper_limit>>hToyUpperLim","","goff");
  //Double_t meanToyUpperLim = hToyUpperLim->GetMean();

  // Double_t ToyCi[5];
  // ToyCi = GetCI(hToyUpperLim);
  std::vector<Double_t> ToyCI = GetCI(hToyUpperLim);
  for (auto & ToyVal : ToyCI) {
    cout << "ToyVal = " << ToyVal << endl;
  }
  
    
  // bkg_rate_mass_hypo - background rate part
  // toy_bkg_chi2_prob - generator chi^2
  // toyfit_chi2_prob - also generator chi^2
  
  // find window size from file name
  // Index
  Int_t slash = filename.Last('/');
  TString nfilename = filename.Remove(0,slash+1);   


  // TString WS = nfilename.Remove(0,nfilename.First('w')+1);
  // cout << "WS = " << WS << endl;
  
  TString MH = FindVal(nfilename,"m",'w'); // mass
  TString WF = FindVal(nfilename,'w','p'); // window scaling factor
  TString PO = FindVal(nfilename,'p','r'); // polynomial order
  TString RF = FindVal(nfilename,'r','s'); // resolution factor
  

  // nfilename.Remove(0,dot);
  
  cout << "New filename = " << nfilename << endl;
  cout << "MH = " << MH << endl;
  cout << "WF = " << WF << endl;
  cout << "PO = " << PO << endl;
  cout << "RF = " << RF << endl;
  
    
  csvFile<<MH<<","<<WF<<","<<PO<<","<<RF<<","<<meanYield<<","<<meanYieldErr<<","<<meanPull<<","<<meanPullErr<<","<<meanBkChi2P<<","<<meanBkSigChi2P<<","<<meanToyChi2P<<","<<meanPVal<<","<<meanEpsilon<<","<<meanUpperLim<<","<<f<<","<<ToyCI[2]<<","<<ToyCI[0]<<","<<ToyCI[1]<<","<<ToyCI[3]<<","<<ToyCI[4]<<endl;
  
}


void convert_csv(Double_t mass, Int_t PO = 3/*Polynomial order for background*/, Int_t TPO = 3/*Polynomial order for Toy MCs*/, Int_t NSig = 0, Int_t MBg = -1/*background model choice*/, Double_t BinSize = 0){

  const char* ext = ".root";

  const char* inDir = "/w/work2/home/oliver/APEX/peaksearch/50kReplay_LVDCgap2";  
  const char* inDirOld = "/w/work5/jlab/halla/APEX/mass_search_1000";
  
  char* massText;
  if(ceilf(mass) == mass){
    massText = Form("M%im%iw",MBg,(int)mass);
  }
  else{
    massText = Form("M%im%.2fw",MBg,mass);
  }
  char* POText = Form("p%i",PO); 
  char* TPOText = Form("P%i",TPO);
  char* binText = Form("%.2fBin",BinSize);
  char* sigText = Form("s%i",NSig);
  cout << "massText = " << massText << endl;
  cout << "binText = " << binText << endl;
  cout << "sigText = " << sigText << endl;

  // if(MBg == -1){    
  //   massText = Form("_m%f",mass); // Background model number not included in name
  // }

  char* dir = gSystem->ExpandPathName(inDir);
  void* dirp = gSystem->OpenDirectory(dir);

  char* dirOld = gSystem->ExpandPathName(inDirOld);
  void* dirpOld = gSystem->OpenDirectory(dirOld);

  const char* entry;
  std::vector<TString> filenameV;
  std::vector<TString> filecheck;
  TString* filename[100];
  Int_t n = 0;
  TString str;

  while((entry = (char*)gSystem->GetDirEntry(dirp))) {
    str = entry;
    if(str.EndsWith(ext) && str.Contains(massText) && str.Contains(sigText) && str.Contains(POText) && str.Contains(TPOText) && str.Contains(binText)){      //      filename[n++] = (TString*)gSystem->ConcatFileName(dir, entry);      
      filenameV.push_back((TString)gSystem->ConcatFileName(dir, entry));
      // next lines retrieve window size
      Int_t firstW = str.First("w"); //find first w in file
      Int_t firstP = str.First("p"); //find first p in file
      TString winText = str(firstW, firstP - firstW);      
      filecheck.push_back(winText);
    }
  }


  TString csvName;
  if(ceilf(mass) == mass){
    csvName = Form("csv/%.2fBin_%iM_%im_%ip_%iP_%is_results.csv",BinSize,MBg,(int)mass,PO,TPO,NSig);
  }
  else{
    csvName = Form("csv/%.2fBin_%iM_%.2fm_%ip_%iP_%is_results.csv",BinSize,MBg,mass,PO,TPO,NSig);
  }
  
  ofstream csvFile;
  csvFile.open(csvName);
  //  csvFile << "Mass,WF,PO,RF,Yield,Yield_Err,Pull,PullErr,Bg_Chi2,BgSig_Chi2,ToyBg_Chi2,PVal,epsilon2,upper_Limits,f,ToyMedUp,ToyN2SigUp,ToyN1SigUp,ToyP1SigUp,ToyP2SigUp" << endl;
  csvFile << "Mass,WF,PO,RF,Yield,Yield_Err,Pull,PullErr,Bg_Chi2,BgSig_Chi2,ToyBg_Chi2,PVal,epsilon2,upper_Limit,f,Toy_UL,Toy_ULN2,Toy_ULN1,Toy_ULP1,Toy_ULP2" << endl;
  
  // get value of Neff for mass
  Double_t Neff = getNeff(mass/1000.0);
  cout << "For mass " << mass << " Neff = " << Neff << endl;

  // get f value for mass
  Double_t f = getf(mass);
  cout << "For mass " << mass << " f = " << f << endl;
  
  for (auto & filename : filenameV) {
    cout << filename << "!" << endl;
    WriteCsv(filename,csvFile,NSig,f,Neff);
    cout << endl;
    
  }

  cout << endl;
  
  csvFile.close();
  
}
