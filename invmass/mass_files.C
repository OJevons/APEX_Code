/********************************************************************
mass_files.C

Eric Jensen
ejensen@jlab.org
June 28th, 2011

Produces invariant mass spectrum for full production statistics.
Plots spectrum for coincidence timing window, accidental timing 
window, and accidentals subtracted.

* Modification

26sep2013

Unbinned mass file for James

>>>>>>>

Updated 31/8/21 for APEX 2019 run
John Williamson

********************************************************************/

#include "./rootalias.h"
#include "./SQLanalysis.h"
#include "./inc1.h"

#include "FillCuts.h"
#include "InputAPEXL.h"

#include "TEntryList.h"
#include <iomanip>

void mass_files(Int_t runno){
  
  const Double_t pi = 4.0*atan(1.0);

  Int_t run_number;    
  TString filename;

  TChain *T = new TChain("T");
  T->Add(LoadTrim(runno));
  
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(102);
  gRandom = new TRandom3(0);

  FillCuts FCut;

  FCut.FillTCutG(cutg1,cutg1V);
  cutg1->SetVarX("L.gold.ph");
  cutg1->SetVarY("L.gold.dp");

  FCut.FillTCutG(cutg2,cutg2V);
  cutg2->SetVarX("L.gold.th");
  cutg2->SetVarY("L.gold.dp");
  
  FCut.FillTCutG(cutg3,cutg3V);
  cutg3->SetVarX("L.gold.ph");
  cutg3->SetVarY("L.gold.th");
  
  FCut.FillTCutG(cutg4,cutg4V);
  cutg4->SetVarX("L.gold.ph");
  cutg4->SetVarY("L.tr.a_vz[0]");

  FCut.FillTCutG(cutg5,cutg5V);
  cutg5->SetVarX("R.gold.ph");
  cutg5->SetVarY("R.gold.dp");
  
  FCut.FillTCutG(cutg6,cutg6V);
  cutg6->SetVarX("R.gold.th");
  cutg6->SetVarY("R.gold.dp");

  FCut.FillTCutG(cutg7,cutg7V);
  cutg7->SetVarX("R.gold.ph");
  cutg7->SetVarY("R.gold.th");

  FCut.FillTCutG(cutg8,cutg8V);
  cutg8->SetVarX("R.gold.ph");
  cutg8->SetVarY("R.tr.a_vz[0]");
  

  //////////////////////////
  // Unbinned mass analysis
  //////////////////////////

  Double_t Ldp[1],Ltheta[1],Lphi[1];
  Double_t rbx[1],rby[1];
  Double_t Rdp[1],Rtheta[1],Rphi[1];
  Double_t Lpx[1],Lpy[1],Lpz[1];
  Double_t Rpx[1],Rpy[1],Rpz[1];  
  Double_t mass[1000000];
  Double_t Lp[1],Rp[1];
  Double_t Lvz[4],Rvz[4];
  Double_t lthsum,lphsum,rthsum,rphsum,nsum;

  
  Double_t maxTracks = 50;
  Double_t Lpse[1],Lshe[1], Lcer_asumc[1];
  Double_t Rpse[1],Rshe[1], Rcer_asumc[1];
    
  const Int_t NS2Pad = 16;
  Double_t Ls2time[NS2Pad], RS2time[NS2Pad];
  Int_t L_s2_tpads, R_s2_tpads;
  Double_t DL_L1A_t, DR_L1A_t;
  
  T->SetBranchStatus("*",0);

  T->SetBranchStatus("L.gold.dp",1);
  T->SetBranchStatus("L.gold.th",1);
  T->SetBranchStatus("L.gold.ph",1);
  T->SetBranchStatus("R.gold.dp",1);
  T->SetBranchStatus("R.gold.th",1);
  T->SetBranchStatus("R.gold.ph",1);
  T->SetBranchStatus("L.gold.px",1);
  T->SetBranchStatus("L.gold.py",1);
  T->SetBranchStatus("L.gold.pz",1);
  T->SetBranchStatus("L.gold.y",1);
  T->SetBranchStatus("R.gold.px",1);
  T->SetBranchStatus("R.gold.py",1);
  T->SetBranchStatus("R.gold.pz",1);
  T->SetBranchStatus("R.gold.y",1);
  T->SetBranchStatus("L.gold.p",1);
  T->SetBranchStatus("R.gold.p",1);
  T->SetBranchStatus("L.tr.a_vz",1);
  T->SetBranchStatus("R.tr.a_vz",1);  
  T->SetBranchStatus("L.gold.ok",1);
  T->SetBranchStatus("R.gold.ok",1);

  T->SetBranchStatus("L.s2.time",1);
  T->SetBranchStatus("L.s2.t_pads",1);
  T->SetBranchStatus("R.s2.time",1);
  T->SetBranchStatus("R.s2.t_pads",1);
  T->SetBranchStatus("DL.ltL1ARemote",1);
  T->SetBranchStatus("DR.rtL1Aremote",1);
  
  
  T->SetBranchStatus("L.prl1.e",1);
  T->SetBranchStatus("L.prl2.e",1);
  T->SetBranchStatus("L.cer.asum_c",1);
  T->SetBranchStatus("R.ps.e",1);
  T->SetBranchStatus("R.sh.e",1);
  T->SetBranchStatus("R.cer.asum_c",1);

  T->SetBranchStatus("Lrb.x",1);
  T->SetBranchStatus("Lrb.y",1);

  T->SetBranchStatus("L.tr.n",1);
  T->SetBranchStatus("R.tr.n",1);
  T->SetBranchStatus("DL.evtype",1);
  
  // Set Branch Address
  T->SetBranchAddress("L.gold.dp",Ldp);
  T->SetBranchAddress("L.gold.th",Ltheta);
  T->SetBranchAddress("L.gold.ph",Lphi);
  T->SetBranchAddress("R.gold.dp",Rdp);
  T->SetBranchAddress("R.gold.th",Rtheta);
  T->SetBranchAddress("R.gold.ph",Rphi);
  T->SetBranchAddress("L.gold.px",Lpx);
  T->SetBranchAddress("L.gold.py",Lpy);
  T->SetBranchAddress("L.gold.pz",Lpz);
  T->SetBranchAddress("R.gold.px",Rpx);
  T->SetBranchAddress("R.gold.py",Rpy);
  T->SetBranchAddress("R.gold.pz",Rpz);
  T->SetBranchAddress("L.gold.p",Lp);
  T->SetBranchAddress("R.gold.p",Rp);
  T->SetBranchAddress("L.tr.a_vz",Lvz);
  T->SetBranchAddress("R.tr.a_vz",Rvz);

  T->SetBranchAddress("L.prl1.e",Lpse);
  T->SetBranchAddress("L.prl2.e",Lshe);
  T->SetBranchAddress("L.cer.asum_c",Lcer_asumc);
  T->SetBranchAddress("R.ps.e",Rpse);
  T->SetBranchAddress("R.sh.e",Rshe);
  T->SetBranchAddress("R.cer.asum_c",Rcer_asumc);

  //  T->SetBranchAddress("L.s2.time[L  
  
  // T->SetBranchAddress("exL.dp",Ldp);
  // T->SetBranchAddress("exL.th",Ltheta);
  // T->SetBranchAddress("exL.ph",Lphi);
  // T->SetBranchAddress("exR.dp",Rdp);
  // T->SetBranchAddress("exR.th",Rtheta);
  // T->SetBranchAddress("exR.ph",Rphi);
  // T->SetBranchAddress("exL.px",Lpx);
  // T->SetBranchAddress("exL.py",Lpy);
  // T->SetBranchAddress("exL.pz",Lpz);
  // T->SetBranchAddress("exR.px",Rpx);
  // T->SetBranchAddress("exR.py",Rpy);
  // T->SetBranchAddress("exR.pz",Rpz);
  // T->SetBranchAddress("exL.p",Lp);
  // T->SetBranchAddress("exR.p",Rp);
  
  T->SetBranchAddress("Lrb.x",rbx);
  T->SetBranchAddress("Lrb.y",rby);

  TH1F *masshist = new TH1F("masshist","Calculated invariant mass (GeV)",4000,0.1,0.3); // bin width defined as 0.05 MeV
  //  TH1F *masshist = new TH1F("masshist","Calculated invariant mass (MeV)",130,175,240);
  // TH1F *masshist = new TH1F("masshist","Calculated invariant mass (MeV)",300,100,300);
  // masshist->GetXaxis()->SetTitle("m_{e^{+}e^{-}}(MeV/c^2)");
  
  ofstream myFile;
  myFile.open(Form("output/Unbinned_mass_%i.txt",runno));
  myFile << std::setprecision(13);

  Double_t    prl12_min = 0.70;                   // for (L.prl1.e+L.prl2.e)
  Double_t    prl12_max = 1.4;                   // for (L.prl1.e+L.prl2.e)
  Double_t    prl1_min  = 0.2;                   // for L.prl1.e
  Double_t    Lcer_min = 650;                   // the limit for L Cherenkov
  
  Double_t    pssh_min  = 0.572;                   // for (R.ps.e+R.sh.e)
  Double_t    pssh_max  = 1.25;                   // for (R.ps.e+R.sh.e)
  Double_t    ps_min  = 0.2;                   // for R.ps.e
  Double_t    Rcer_min = 1000;                   // the limit for Cherenkov
  // Int_t    cer_min = 2000;                   // the limit for Cherenkov
  Double_t phmin     = -0.04, phmax = 0.04;  // the limits for tg_ph
  Double_t thmin     = -0.05, thmax = 0.065;  // the limits for tg_th
  Double_t dpmin     = -0.04, dpmax = 0.05;  // the limits for tg_dp
  Double_t dpminR     = -0.04, dpmaxR = 0.05;  // the limits for tg_dp (R)
  Double_t ytmin     = -0.05, ytmax = 0.03;  // the limits for tg_y

  
  TCut Lct1        = Form("(L.prl1.e+L.prl2.e)/(1000)>%f&&(L.prl1.e+L.prl2.e)/(1000)<%f&&(L.prl1.e)/(1000)>%f", prl12_min, prl12_max, prl1_min);
  TCut Lct2        = Form("L.cer.asum_c>%f", Lcer_min);
  
  TCut LPID       = Lct1 + Lct2;
  TCut Lct3        = Form("L.gold.ph<%.2f&&L.gold.ph>%.2f", phmax, phmin);
  TCut Lct4        = Form("L.gold.th<%.2f&&L.gold.th>%.2f", thmax, thmin);
  TCut Lct5        = Form("L.gold.dp<%.2f&&L.gold.dp>%.2f", dpmax, dpmin);
  TCut LVertexCut  = Form("L.gold.y<%.4f&&L.gold.y>%.4f",   ytmax, ytmin);

  TCut LFPCut = Lct3 + Lct4 + Lct5 + LVertexCut;

  TCut LGeneralCut = LFPCut + LPID;
  
  TCut Rct1        = Form("(R.ps.e+R.sh.e)/(1000)>%f&&(R.ps.e)/(1000)>%f", pssh_min, ps_min);
  TCut Rct2        = Form("R.cer.asum_c>%f", Rcer_min);
  
  TCut RPID       = Rct1 + Rct2;
  TCut Rct3        = Form("R.gold.ph<%.2f&&R.gold.ph>%.2f", phmax, phmin);
  TCut Rct4        = Form("R.gold.th<%.2f&&R.gold.th>%.2f", thmax, thmin);
  TCut Rct5        = Form("R.gold.dp<%.2f&&R.gold.dp>%.2f", dpmaxR, dpminR);
  TCut RVertexCut  = Form("R.gold.y<%.4f&&R.gold.y>%.4f",   ytmax, ytmin);

  TCut RFPCut = Rct3 + Rct4 + Rct5 + RVertexCut;  
  TCut RGeneralCut = RFPCut + RPID;

  Double_t tpeak = 204.39; // in ns    

  if(runno>4363 && runno<=4407){
    tpeak = 200.5; 
  }
  else if(runno>=4425){
    tpeak = 202.38;
  }

  Double_t twidth = 1.83; // 3 sigma (0.61 ns)

  Double_t tmin = tpeak - twidth;
  Double_t tmax = tpeak + twidth;
  
  TCut TimingCut  = Form("(1e9*(L.s2.time[L.s2.t_pads]-R.s2.time[R.s2.t_pads])-0.25*(DL.ltL1ARemote[0]-DR.rtL1Aremote[0]))<%f&&(1e9*(L.s2.time[L.s2.t_pads]-R.s2.time[R.s2.t_pads])-0.25*(DL.ltL1ARemote[0]-DR.rtL1Aremote[0]))>%f", tmax, tmin);

  // both arm reactz cut
  Double_t RzCutL = -0.152711;
  Double_t RzCutH = 0.094715;  
  TCut ReactzCut = Form("(L.tr.a_vz[0]-R.tr.a_vz[0])>%f && (L.tr.a_vz[0]-R.tr.a_vz[0])<%f",RzCutL,RzCutH);

  // both arm mom cut
  Double_t dpSumCutL = 0.0187;
  TCut dpSumCut = Form("(L.gold.dp + R.gold.dp)<%f",dpSumCutL);  
  
  T->Draw( ">>elist", "DL.evtype==6 &&  R_gcut_dpph && R_gcut_dpth && R_gcut_thph && R_gcut_vzph && L_gcut_dpph && L_gcut_dpth && L_gcut_thph && L_gcut_vzph && L.tr.n>0 && R.tr.n>0 && L.gold.ok==1 && R.gold.ok==1" + LGeneralCut + RGeneralCut + TimingCut + ReactzCut + dpSumCut,"entrylistarray");
  
  //  T->Draw( ">>elist", "DL.evtype==6 && DR.rrawt2<1705 &&DR.rrawt2>1690 && R_gcut_dpph && R_gcut_dpth && R_gcut_thph &&L_gcut_dpph && L_gcut_dpth && L_gcut_thph && L.tr.n>0 && R.tr.n>0 && exR.ok>0 && exL.ok>0 && (exR.p+exL.p)<2.261" + LGeneralCut + RGeneralCut,"entrylistarray");

  TEntryList *elist=(TEntryList*)gDirectory->Get("elist");
  Long64_t listEntries = elist->GetN();
  int chainEntries=T->GetEntries();

  cout << "Going to entry list " << endl;
  T->SetEntryList(elist);
  cout << "Set entry list " << endl;
  
  std::cout<<"list entries = "<<listEntries<<std::endl;
  std::cout<<"chain entries = "<< chainEntries << std::endl;

  std::cout<<"Updated chain entries = "<< T->GetEntries() << std::endl;

  Double_t Lpz_t = 0.0;
  Double_t Lpx_calc = 0.0;
  Double_t Lpy_calc = 0.0;
  Double_t Lpz_calc = 0.0;

  Double_t Rpz_t = 0.0;
  Double_t Rpx_calc = 0.0;
  Double_t Rpy_calc = 0.0;
  Double_t Rpz_calc = 0.0;
  
  Int_t treenum = 0;

  // Double_t LAng = 5.366*TMath::DegToRad();
  // Double_t RAng = -5.372*TMath::DegToRad();

  Double_t LAng = 5.0*TMath::DegToRad();
  Double_t RAng = -5.0*TMath::DegToRad();

  
  for (Long64_t el = 0; el < listEntries; el++) {

    Long64_t treeEntry = elist->GetEntryAndTree(el,treenum);
    T->LoadTree(treeEntry); // this also returns treeEntry
    Long64_t chainEntry = treeEntry+T->GetTreeOffset()[treenum];
    T->GetEntry(chainEntry);

    //    printf("el=%lld, treeEntry=%lld, chainEntry=%lld, treenum=%d\n", el, treeEntry, chainEntry, treenum);
    
    // Lpz_t = Lp[0]/sqrt(1 + pow((Ltheta[0]),2) + pow((Lphi[0]),2));
    // Lpx_calc = Lpz_t*((Lphi[0])*cos(LAng)+sin(LAng));
    // Lpy_calc = -Lpz_t*(Ltheta[0]);
    // Lpz_calc = Lpz_t*(cos(LAng)-(Lphi[0])*sin(LAng));
    
    // Rpz_t = Rp[0]/sqrt(1 + pow((Rtheta[0]),2) + pow((Rphi[0]),2));
    // Rpx_calc = Rpz_t*((Rphi[0])*cos(RAng)+sin(RAng));
    // Rpy_calc = -Rpz_t*(Rtheta[0]);
    // Rpz_calc = Rpz_t*(cos(RAng)-(Rphi[0])*sin(RAng));

    //    mass[el] = 1000*sqrt(2*( abs(Lp[0])*abs(Rp[0])-(Lpx_calc*Rpx_calc+Lpy_calc*Rpy_calc+Lpz_calc*Rpz_calc)));
    
    mass[el] = 1000*sqrt(2*( abs(Lp[0])*abs(Rp[0])-(Lpx[0]*Rpx[0]+Lpy[0]*Rpy[0]+Lpz[0]*Rpz[0])));

    // cout << "mass[" << el << "] = " << mass[el] << endl;
    masshist->Fill(1e-3*mass[el]);
    
    myFile << mass[el] << endl;        
    
    //    needed_branch->GetEntry(treeEntry);   
  }

  myFile.close();

  //  root-output      
  // save mass hist to file
  
  std::unique_ptr<TFile> myRFile( TFile::Open(Form("root-output/mass_%i.root",runno), "RECREATE") );

  myRFile->WriteObject(masshist,"masshist");
  myRFile->Close();  
}
