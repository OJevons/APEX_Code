//-----------------------------------------------------------------------------
// APEX VDC event display
// Oliver Jevons; 24-Mar-23
//
// Inputs: 
// 1. Path to replayed APEX file
//
// Output:
// 1. VDC even display
//-----------------------------------------------------------------------------

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include "TEntryList.h"
#include "APEXTree.C"

#include <iostream>

using namespace std;

// Declare histograms to be used
// Left VDC
TH2F* hLU1VDC_WireVsTime = new TH2F("hLU1",";Wire # (LU1);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hLU2VDC_WireVsTime = new TH2F("hLU2",";Wire # (LU2);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hLV1VDC_WireVsTime = new TH2F("hLV1",";Wire # (LV1);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hLV2VDC_WireVsTime = new TH2F("hLV2",";Wire # (LV2);Raw time",1473,-0.25,367.25,3700,0,3700);
// Right VDC
TH2F* hRU1VDC_WireVsTime = new TH2F("hRU1",";Wire # (RU1);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hRU2VDC_WireVsTime = new TH2F("hRU2",";Wire # (RU2);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hRV1VDC_WireVsTime = new TH2F("hRV1",";Wire # (RV1);Raw time",1473,-0.25,367.25,3700,0,3700);
TH2F* hRV2VDC_WireVsTime = new TH2F("hRV2",";Wire # (RV2);Raw time",1473,-0.25,367.25,3700,0,3700);
// Left and right S2 scintillators
TH1F* hLS2_HitPad = new TH1F("hLS2","",16,0,15);
TH1F* hRS2_HitPad = new TH1F("hRS2","",16,0,15);

// Clone VDC histograms for plane-by-plane plots
TH2F* hLU1VDC_WireVsTime_Clone = (TH2F*)hLU1VDC_WireVsTime->Clone("hLU1_clone");
TH2F* hLU2VDC_WireVsTime_Clone = (TH2F*)hLU2VDC_WireVsTime->Clone("hLU2_clone");
TH2F* hLV1VDC_WireVsTime_Clone = (TH2F*)hLV1VDC_WireVsTime->Clone("hLV1_clone");
TH2F* hLV2VDC_WireVsTime_Clone = (TH2F*)hLV2VDC_WireVsTime->Clone("hLV2_clone");
TH2F* hRU1VDC_WireVsTime_Clone = (TH2F*)hRU1VDC_WireVsTime->Clone("hRU1_clone");
TH2F* hRU2VDC_WireVsTime_Clone = (TH2F*)hRU2VDC_WireVsTime->Clone("hRU2_clone");
TH2F* hRV1VDC_WireVsTime_Clone = (TH2F*)hRV1VDC_WireVsTime->Clone("hRV1_clone");
TH2F* hRV2VDC_WireVsTime_Clone = (TH2F*)hRV2VDC_WireVsTime->Clone("hRV2_clone");

Bool_t kContinue{kTRUE};

void VDCEvtDisplay(Int_t runno = 4400, Bool_t kSingleEvt = kTRUE){

  cout<<"--------------------------------------------"<<endl;
  cout<<"--------- APEX VDC Event Display -----------"<<endl;
  cout<<"--------- Run Number: "<<runno<<" -----------------"<<endl;
  cout<<"--------------------------------------------"<<endl;

  TChain* C = new TChain("T");

  // Need full replayed APEX file - NOT TRIM
  TString sFile = "/w/work5/jlab/halla/APEX/full_events/apex_" + std::to_string(runno) +"*.root";
  C->Add(sFile);

  //TTree* tRaw = new TTree(C);
  APEXTree* T = new APEXTree(C);
  Long64_t nentries = C->GetEntries();
    
  // Drawing - define canvases, and draw empty histograms
  
  gStyle->SetOptStat(00000000);
  TString sCanvName = "cL_" + to_string(runno);
  TCanvas* cL = new TCanvas("cL",sCanvName,1500,1000);
  //cL->SetLeftMargin(0.12);
  //cL->SetTopMargin(0.05);
  cL->Divide(1,5,0,0);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cL->cd(5);
  hLU1VDC_WireVsTime->SetMarkerSize(1);
  hLU1VDC_WireVsTime->SetMarkerColor(kMagenta);
  hLU1VDC_WireVsTime->SetMarkerStyle(21);
  hLU1VDC_WireVsTime->Draw();
  
  cL->cd(4);
  hLV1VDC_WireVsTime->SetMarkerSize(1);
  hLV1VDC_WireVsTime->SetMarkerColor(kMagenta+1);
  hLV1VDC_WireVsTime->SetMarkerStyle(21);
  hLV1VDC_WireVsTime->Draw();
  
  cL->cd(3);
  hLU2VDC_WireVsTime->SetMarkerSize(1);
  hLU2VDC_WireVsTime->SetMarkerColor(kMagenta+2);
  hLU2VDC_WireVsTime->SetMarkerStyle(21);
  hLU2VDC_WireVsTime->Draw();
  
  cL->cd(2);
  hLV2VDC_WireVsTime->SetMarkerSize(1);
  hLV2VDC_WireVsTime->SetMarkerColor(kMagenta+3);
  hLV2VDC_WireVsTime->SetMarkerStyle(21);
  hLV2VDC_WireVsTime->Draw();
  
  cL->cd(1);
  hLS2_HitPad->SetMarkerSize(1);
  hLS2_HitPad->SetMarkerColor(kMagenta+3);
  hLS2_HitPad->SetMarkerStyle(21);
  hLS2_HitPad->Draw("p");

  sCanvName = "cR_" + to_string(runno);
  TCanvas* cR = new TCanvas("cR",sCanvName,1500,1000);
  //cR->SetLeftMargin(0.12);
  //cR->SetTopMargin(0.05);
  cR->Divide(1,5,0,0);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cR->cd(5);
  hRU1VDC_WireVsTime->SetMarkerSize(1);
  hRU1VDC_WireVsTime->SetMarkerColor(kAzure);
  hRU1VDC_WireVsTime->SetMarkerStyle(21);
  hRU1VDC_WireVsTime->Draw();

  cR->cd(4);
  hRV1VDC_WireVsTime->SetMarkerSize(1);
  hRV1VDC_WireVsTime->SetMarkerColor(kAzure+1);
  hRV1VDC_WireVsTime->SetMarkerStyle(21);
  hRV1VDC_WireVsTime->Draw();

  cR->cd(3);
  hRU2VDC_WireVsTime->SetMarkerSize(1);
  hRU2VDC_WireVsTime->SetMarkerColor(kAzure+2);
  hRU2VDC_WireVsTime->SetMarkerStyle(21);
  hRU2VDC_WireVsTime->Draw();
  
  cR->cd(2);
  hRV2VDC_WireVsTime->SetMarkerSize(1);
  hRV2VDC_WireVsTime->SetMarkerColor(kAzure+3);
  hRV2VDC_WireVsTime->SetMarkerStyle(21);
  hRV2VDC_WireVsTime->Draw();
  
  cR->cd(1);
  hRS2_HitPad->SetMarkerSize(1);
  hRS2_HitPad->SetMarkerColor(kAzure+3);
  hRS2_HitPad->SetMarkerStyle(21);
  hRS2_HitPad->Draw("p");

  sCanvName = "cL_split_" + to_string(runno);
  TCanvas* cL_split = new TCanvas("cL_split",sCanvName,1500,1000);
  cL_split->SetLeftMargin(0.12);
  cL_split->SetTopMargin(0.05);
  cL_split->Divide(2,2);

  cL_split->cd(1);
  hLU1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLU1VDC_WireVsTime_Clone->SetMarkerColor(kMagenta);
  hLU1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLU1VDC_WireVsTime_Clone->Draw();

  cL_split->cd(2);
  hLV1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLV1VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+1);
  hLV1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLV1VDC_WireVsTime_Clone->Draw();

  cL_split->cd(3);
  hLU2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLU2VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+2);
  hLU2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLU2VDC_WireVsTime_Clone->Draw();
  
  cL_split->cd(4);
  hLV2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLV2VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+3);
  hLV2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLV2VDC_WireVsTime_Clone->Draw();

  sCanvName = "cR_split_" + to_string(runno);
  TCanvas* cR_split = new TCanvas("cR_split",sCanvName,1500,1000);
  cR_split->SetLeftMargin(0.12);
  cR_split->SetTopMargin(0.05);
  cR_split->Divide(2,2);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cR_split->cd(1);
  hRU1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRU1VDC_WireVsTime_Clone->SetMarkerColor(kAzure);
  hRU1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRU1VDC_WireVsTime_Clone->Draw();

  cR_split->cd(2);
  hRV1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRV1VDC_WireVsTime_Clone->SetMarkerColor(kAzure+1);
  hRV1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRV1VDC_WireVsTime_Clone->Draw();

  cR_split->cd(3);
  hRU2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRU2VDC_WireVsTime_Clone->SetMarkerColor(kAzure+2);
  hRU2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRU2VDC_WireVsTime_Clone->Draw();
  
  cR_split->cd(4);
  hRV2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRV2VDC_WireVsTime_Clone->SetMarkerColor(kAzure+3);
  hRV2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRV2VDC_WireVsTime_Clone->Draw();
  // END OF DRAWING

  // Start from 1st entry in tree
  Int_t ev{1};
  Int_t prevev{1};

  // Holding variables for cuts
  Float_t tCoinc{0};

  // Apply cuts onto tree
  //C->Draw( ">>elist", "DL.evtype==6 && L.tr.n > 0","entrylistarray");
  //APEXTree* T = new APEXTree(C);
  
  //TEntryList *elist=(TEntryList*)gDirectory->Get("elist");
  //Long64_t listEntries = elist->GetN();
  //Int_t treenum{0};

  while(kContinue){
    if(ev%100 == 0) cout<<"Processing event "<<ev<<"/"<<nentries<<endl;

    // Reset for new event; read tree, clear any histogram contents
    //Long64_t treeEntry = elist->GetEntryAndTree(ev,treenum);
    //T->LoadTree(treeEntry); // this also returns treeEntry
    //Long64_t chainEntry = treeEntry+T->GetTreeOffset()[treenum];
    //T->GetEntry(chainEntry);
    T->GetEntry(ev);

    hLU1VDC_WireVsTime->Reset("M");
    hLU2VDC_WireVsTime->Reset("M");
    hLV1VDC_WireVsTime->Reset("M");
    hLV2VDC_WireVsTime->Reset("M");
    hRU1VDC_WireVsTime->Reset("M");
    hRU2VDC_WireVsTime->Reset("M");
    hRV1VDC_WireVsTime->Reset("M");
    hRV2VDC_WireVsTime->Reset("M");
    hLS2_HitPad->Reset("M");
    hRS2_HitPad->Reset("M");
    hLU1VDC_WireVsTime_Clone->Reset("M");
    hLU2VDC_WireVsTime_Clone->Reset("M");
    hLV1VDC_WireVsTime_Clone->Reset("M");
    hLV2VDC_WireVsTime_Clone->Reset("M");
    hRU1VDC_WireVsTime_Clone->Reset("M");
    hRU2VDC_WireVsTime_Clone->Reset("M");
    hRV1VDC_WireVsTime_Clone->Reset("M");
    hRV2VDC_WireVsTime_Clone->Reset("M");    
    
    // Undo SetRangeUser commands done in previous loops
    hLU1VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hLU1VDC_WireVsTime_Clone->GetNbinsX());
    hLU2VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hLU2VDC_WireVsTime_Clone->GetNbinsX());
    hLV1VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hLV1VDC_WireVsTime_Clone->GetNbinsX());
    hLV2VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hLV2VDC_WireVsTime_Clone->GetNbinsX());
    hRU1VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hRU1VDC_WireVsTime_Clone->GetNbinsX());
    hRU2VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hRU2VDC_WireVsTime_Clone->GetNbinsX());
    hRV1VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hRV1VDC_WireVsTime_Clone->GetNbinsX());
    hRV2VDC_WireVsTime_Clone->GetXaxis()->SetRange(1,hRV2VDC_WireVsTime_Clone->GetNbinsX());

    // Calculate coincidence time
    tCoinc = T->L_s2_time-T->R_s2_time;
    
    // Only look at events with desired properties
    //if(T->L_tr_n == 0 || T->R_tr_n == 0){     // Identified tracks in both arms
    if(T->L_tr_n == 0 || T->L_vdc_v2_nclust < 2){     // Track in left arm, with V2 plane containing more than one cluster
    //if(TMath::Abs(tCoinc) > 0.3e-7){     // Track in left arm, and coincidence time is less than 0.3 us
    ev++;
    kContinue = kTRUE;
    continue;

    if(ev>nentries) return; // Stop program if tree is exceeded
    }
    else{
      kContinue = kFALSE; // Temporarily break the loop for a good event
      cout<<"Plotting event "<<ev<<endl;
      cout<<"Number of identified tracks = "<<T->L_tr_n<<" (left) \t"<<T->R_tr_n<<" (right)"<<endl;
      cout<<"LHRS time = "<<T->L_s2_time<<"\tRHRS time = "<<T->R_s2_time<<"\tCoincidence time = "<<tCoinc<<endl;

      // Run over ALL hits in array - stores all, not just good hits
      // LU1
      for(int hit{0}; hit<T->Ndata_L_vdc_u1_wire; hit++){	
	hLU1VDC_WireVsTime->Fill(T->L_vdc_u1_wire[hit],T->L_vdc_u1_rawtime[hit]);
	hLU1VDC_WireVsTime_Clone->Fill(T->L_vdc_u1_wire[hit],T->L_vdc_u1_rawtime[hit]);
      }
      // LU2
      for(int hit{0}; hit<T->Ndata_L_vdc_u2_wire; hit++){
	hLU2VDC_WireVsTime->Fill(T->L_vdc_u2_wire[hit],T->L_vdc_u2_rawtime[hit]);
	hLU2VDC_WireVsTime_Clone->Fill(T->L_vdc_u2_wire[hit],T->L_vdc_u2_rawtime[hit]);
      }
      // LV1
      for(int hit{0}; hit<T->Ndata_L_vdc_v1_wire; hit++){
	hLV1VDC_WireVsTime->Fill(T->L_vdc_v1_wire[hit],T->L_vdc_v1_rawtime[hit]);
	hLV1VDC_WireVsTime_Clone->Fill(T->L_vdc_v1_wire[hit],T->L_vdc_v1_rawtime[hit]);
      }
      // LV2
      for(int hit{0}; hit<T->Ndata_L_vdc_v2_wire; hit++){
	hLV2VDC_WireVsTime->Fill(T->L_vdc_v2_wire[hit],T->L_vdc_v2_rawtime[hit]);
	hLV2VDC_WireVsTime_Clone->Fill(T->L_vdc_v2_wire[hit],T->L_vdc_v2_rawtime[hit]);
      }
      // RU1
      for(int hit{0}; hit<T->Ndata_R_vdc_u1_wire; hit++){
	hRU1VDC_WireVsTime->Fill(T->R_vdc_u1_wire[hit],T->R_vdc_u1_rawtime[hit]);
	hRU1VDC_WireVsTime_Clone->Fill(T->R_vdc_u1_wire[hit],T->R_vdc_u1_rawtime[hit]);
      }
      // RU2
      for(int hit{0}; hit<T->Ndata_R_vdc_u2_wire; hit++){
	hRU2VDC_WireVsTime->Fill(T->R_vdc_u2_wire[hit],T->R_vdc_u2_rawtime[hit]);
	hRU2VDC_WireVsTime_Clone->Fill(T->R_vdc_u2_wire[hit],T->R_vdc_u2_rawtime[hit]);
      }
      // RV1
      for(int hit{0}; hit<T->Ndata_R_vdc_v1_wire; hit++){
	hRV1VDC_WireVsTime->Fill(T->R_vdc_v1_wire[hit],T->R_vdc_v1_rawtime[hit]);
	hRV1VDC_WireVsTime_Clone->Fill(T->R_vdc_v1_wire[hit],T->R_vdc_v1_rawtime[hit]);
      }
      // RV2
      for(int hit{0}; hit<T->Ndata_R_vdc_v2_wire; hit++){
	hRV2VDC_WireVsTime->Fill(T->R_vdc_v2_wire[hit],T->R_vdc_v2_rawtime[hit]);
	hRV2VDC_WireVsTime_Clone->Fill(T->R_vdc_v2_wire[hit],T->R_vdc_v2_rawtime[hit]);
      }
      // LS2
      for(int hit{0}; hit<T->Ndata_L_s2_t_pads; hit++){
	hLS2_HitPad->Fill(T->L_s2_t_pads[hit]);
      }
      // RS2
      for(int hit{0}; hit<T->Ndata_R_s2_t_pads; hit++){
	hRS2_HitPad->Fill(T->R_s2_t_pads[hit]);
      }
  
      // Update canvases
      hLU1VDC_WireVsTime_Clone->GetXaxis()->SetRange( hLU1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLU1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hLV1VDC_WireVsTime_Clone->GetXaxis()->SetRange( hLV1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLV1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hLU2VDC_WireVsTime_Clone->GetXaxis()->SetRange( hLU2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLU2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hLV2VDC_WireVsTime_Clone->GetXaxis()->SetRange( hLV2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLV2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hRU1VDC_WireVsTime_Clone->GetXaxis()->SetRange( hRU1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRU1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hRV1VDC_WireVsTime_Clone->GetXaxis()->SetRange( hRV1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRV1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hRU2VDC_WireVsTime_Clone->GetXaxis()->SetRange( hRU2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRU2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      hRV2VDC_WireVsTime_Clone->GetXaxis()->SetRange( hRV2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRV2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
      
      cL->cd();
      cL->Clear();
      cL->Update();
      cL->Divide(1,5,0,0);
      cL->cd(5);
      hLU1VDC_WireVsTime->Draw();
      cL->cd(4);
      hLV1VDC_WireVsTime->Draw();
      cL->cd(3);
      hLU2VDC_WireVsTime->Draw();
      cL->cd(2);
      hLV2VDC_WireVsTime->Draw();
      cL->cd(1);
      hLS2_HitPad->Draw("p");
      cL->cd();
      cL->Update();
      
      cR->cd();
      cR->Clear();
      cR->Update();
      cR->Divide(1,5,0,0);
      cR->cd(5);
      hRU1VDC_WireVsTime->Draw();
      cR->cd(4);
      hRV1VDC_WireVsTime->Draw();
      cR->cd(3);
      hRU2VDC_WireVsTime->Draw();
      cR->cd(2);
      hRV2VDC_WireVsTime->Draw();
      cR->cd(1);
      hRS2_HitPad->Draw("p");
      cR->cd();
      cR->Update();
 
      cL_split->cd();
      cL_split->Clear();
      cL_split->Update();
      cL_split->Divide(2,2);
      cL_split->cd(1);
      hLU1VDC_WireVsTime_Clone->Draw();
      cL_split->cd(2);
      hLV1VDC_WireVsTime_Clone->Draw();
      cL_split->cd(3);
      hLU2VDC_WireVsTime_Clone->Draw();
      cL_split->cd(4);
      hLV2VDC_WireVsTime_Clone->Draw();
      cL_split->cd();
      cL_split->Update();

      cR_split->cd();
      cR_split->Clear();
      cR_split->Update();
      cR_split->Divide(2,2);
      cR_split->cd(1);
      hRU1VDC_WireVsTime_Clone->Draw();
      cR_split->cd(2);
      hRV1VDC_WireVsTime_Clone->Draw();
      cR_split->cd(3);
      hRU2VDC_WireVsTime_Clone->Draw();
      cR_split->cd(4);
      hRV2VDC_WireVsTime_Clone->Draw();
      cR_split->cd();
      cR_split->Update();
    }
    
    char cont;

    cout<<"Press 'n' to display next event. Press 'p' to display previous event. Press any other key to quit..."<<endl;
    cin>>cont;

    // If looking at next event, store current event in case of overshoot
    if(cont == 'n'){
      kContinue = kTRUE;
      prevev = ev;
      ev++;
      continue;
    }
    // In case of overshoot, go to previous relevant event
    else if(cont == 'p'){
      kContinue = kTRUE;
      ev = prevev;
      continue;
    }
  }

  return;
}
