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
#include "APEXTree.C"

#include <iostream>

using namespace std;

// Declare histograms to be used
// Left VDC
TH2F* hLU1VDC_WireVsTime = new TH2F("hLU1",";Wire # (LU1);Raw time",368,0,367,2200,1000,3200);
TH2F* hLU2VDC_WireVsTime = new TH2F("hLU2",";Wire # (LU2);Raw time",368,0,367,2200,1000,3200);
TH2F* hLV1VDC_WireVsTime = new TH2F("hLV1",";Wire # (LV1);Raw time",368,0,367,2200,1000,3200);
TH2F* hLV2VDC_WireVsTime = new TH2F("hLV2",";Wire # (LV2);Raw time",368,0,367,2200,1000,3200);
// Right VDC
TH2F* hRU1VDC_WireVsTime = new TH2F("hRU1",";Wire # (RU1);Raw time",368,0,367,2200,1000,3200);
TH2F* hRU2VDC_WireVsTime = new TH2F("hRU2",";Wire # (RU2);Raw time",368,0,367,2200,1000,3200);
TH2F* hRV1VDC_WireVsTime = new TH2F("hRV1",";Wire # (RV1);Raw time",368,0,367,2200,1000,3200);
TH2F* hRV2VDC_WireVsTime = new TH2F("hRV2",";Wire # (RV2);Raw time",368,0,367,2200,1000,3200);

// Clone VDC histograms for plane-by-plane plots
TH2F* hLU1VDC_WireVsTime_Clone = (TH2F*)hLU1VDC_WireVsTime->Clone("hLU1_clone");
TH2F* hLU2VDC_WireVsTime_Clone = (TH2F*)hLU2VDC_WireVsTime->Clone("hLU2_clone");
TH2F* hLV1VDC_WireVsTime_Clone = (TH2F*)hLV1VDC_WireVsTime->Clone("hLV1_clone");
TH2F* hLV2VDC_WireVsTime_Clone = (TH2F*)hLV2VDC_WireVsTime->Clone("hLV2_clone");
TH2F* hRU1VDC_WireVsTime_Clone = (TH2F*)hRU1VDC_WireVsTime->Clone("hRU1_clone");
TH2F* hRU2VDC_WireVsTime_Clone = (TH2F*)hRU2VDC_WireVsTime->Clone("hRU2_clone");
TH2F* hRV1VDC_WireVsTime_Clone = (TH2F*)hRV1VDC_WireVsTime->Clone("hRV1_clone");
TH2F* hRV2VDC_WireVsTime_Clone = (TH2F*)hRV2VDC_WireVsTime->Clone("hRV2_clone");

// Gradients of partial cluster
TH2F* h5WireGrad = new TH2F("h5wiregrad",";Before pivot;After pivot",500,-500,500,500,-500,500);

void APEXVDC(Int_t runno = 4030, Int_t evtno = 955, Bool_t kSingleEvt = kTRUE){

  TChain* C = new TChain("T");

  // Need full replayed APEX file - NOT TRIM
  TString sFile = "/w/work5/jlab/halla/APEX/full_events/apex_" + std::to_string(runno) +"*.root";
  C->Add(sFile);

  APEXTree* T = new APEXTree(C);
  Long64_t nentries = C->GetEntries();

  // For testing - run over fewer events
  nentries = 1000;
    
  // Define histograms
  // Left VDC
  /*TH2F* hLU1VDC_WireVsTime = new TH2F("hLU1",";Wire # (LU1);Raw time",368,0,367,2200,1000,3200);
    TH2F* hLU2VDC_WireVsTime = new TH2F("hLU2",";Wire # (LU2);Raw time",368,0,367,2200,1000,3200);
    TH2F* hLV1VDC_WireVsTime = new TH2F("hLV1",";Wire # (LV1);Raw time",368,0,367,2200,1000,3200);
    TH2F* hLV2VDC_WireVsTime = new TH2F("hLV2",";Wire # (LV2);Raw time",368,0,367,2200,1000,3200);
    // Right VDC
    TH2F* hRU1VDC_WireVsTime = new TH2F("hRU1",";Wire # (RU1);Raw time",368,0,367,2200,1000,3200);
    TH2F* hRU2VDC_WireVsTime = new TH2F("hRU2",";Wire # (RU2);Raw time",368,0,367,2200,1000,3200);
    TH2F* hRV1VDC_WireVsTime = new TH2F("hRV1",";Wire # (RV1);Raw time",368,0,367,2200,1000,3200);
    TH2F* hRV2VDC_WireVsTime = new TH2F("hRV2",";Wire # (RV2);Raw time",368,0,367,2200,1000,3200);*/
  cout<<"Processing "<<nentries<<" entries"<<endl;

  // Counting variables
  // No. of VDC clusters
  Int_t nOneClust{0};
  Int_t nOneClustGivenRight{0};
  Int_t nTwoClust{0};
  Int_t nTwoClustGivenRight{0};
  Int_t nThreeClust{0};
  Int_t nThreeClustGivenRight{0};
  Int_t nMoreClust{0};
  Int_t nMoreClustGivenRight{0};
  // No. of identified tracks
  Int_t nTotTrackL{0};
  Int_t nTotTrackR{0};
  Int_t nTrackWOutVDCL{0};
  Int_t nTrackWOutVDCR{0};
  Int_t nCoinEvt{0};
  Int_t nCoinOneTrackBothArms{0};

  Long64_t lOneTenth = ((double)(nentries) / 10.);
  for(Long64_t ev{0}; ev<nentries; ev++){
    if(ev%lOneTenth==0) cout<<(long)(((double)(ev)/(double)(nentries))*100.001)<<"% processed"<<endl;
    T->GetEntry(ev);

    // Counting number of VDC clusters in LHRS
    if(T->L_vdc_u1_nclust == 1){
      if(T->R_vdc_u1_nclust != 0) nOneClustGivenRight++;
      nOneClust++;
    }
    if(T->L_vdc_u1_nclust == 2){
      if(T->R_vdc_u1_nclust != 0) nTwoClustGivenRight++;
      nTwoClust++;
    }
    if(T->L_vdc_u1_nclust == 3){
      if(T->R_vdc_u1_nclust != 0) nThreeClustGivenRight++;
      nThreeClust++;
    }
    else if(T->L_vdc_u1_nclust > 3){
      if(T->R_vdc_u1_nclust != 0) nMoreClustGivenRight++;
      nMoreClust++;
    }
    
    // Counting number of recorded tracks
    nTotTrackL += T->L_tr_n;
    nTotTrackR += T->R_tr_n;
    if(T->L_tr_n > 0 && T->L_vdc_u1_nclust == 0) nTrackWOutVDCL += T->L_tr_n;
    if(T->R_tr_n > 0 && T->R_vdc_u1_nclust == 0) nTrackWOutVDCR += T->R_tr_n;
    if(T->L_tr_n > 0 && T->R_tr_n > 0){
      nCoinEvt++;
      //cout<<"Event: "<<ev<<"\tNo. of LHRS tracks: "<<T->L_tr_n<<"; No. of LU1 clusters: "<<T->L_vdc_u1_nclust<<"\tNo. of RHRS tracks: "<<T->R_tr_n<<"; No. of RU1 clusters: "<<T->R_vdc_u1_nclust<<endl;
    }
    if(T->L_tr_n == 1 && T->R_tr_n == 1) nCoinOneTrackBothArms++;

    //if(T->L_vdc_u1_nclust != 0 && T->R_vdc_u1_nclust != 0) cout<<"Event "<<ev<<"\tLeft VDC clusters: "<<T->L_vdc_u1_nclust<<"\tRight VDC clusters: "<<T->R_vdc_u1_nclust<<endl;

    //if(ev != 146534) continue;
    if(kSingleEvt && ev != evtno) continue;
    //std::cout<<"Event "<<ev<<"\tWires hit = "<<T->Ndata_L_vdc_u1_wire<<std::endl;

    // Look at LHRS fTracks object - look for corresponding cluster numbers in VDC chamber
    Int_t nTrkClNo_U1{0};
    Int_t nTrkClNo_V1{0};
    Int_t nTrkClNo_U2{0};
    Int_t nTrkClNo_V2{0};

    for(int iTr{0}; iTr<T->Ndata_L_tr_trknum; iTr++){
      nTrkClNo_U1 = T->L_tr_U1clsnum[iTr];
      nTrkClNo_V1 = T->L_tr_V1clsnum[iTr];
      nTrkClNo_U2 = T->L_tr_U1clsnum[iTr];
      nTrkClNo_V2 = T->L_tr_U1clsnum[iTr];

      cout<<"Event ["<<ev<<"]; LHRS track number ["<<T->L_tr_trknum[iTr]<<"]"<<endl;
      cout<<"\t U1 cluster number: "<<T->L_tr_U1clsnum[iTr]<<"; pivot wire "<<T->L_vdc_u1_clpivot[nTrkClNo_U1]<<endl;
      cout<<"\t V1 cluster number: "<<T->L_tr_V1clsnum[iTr]<<"; pivot wire "<<T->L_vdc_v1_clpivot[nTrkClNo_V1]<<endl;
      cout<<"\t U2 cluster number: "<<T->L_tr_U2clsnum[iTr]<<"; pivot wire "<<T->L_vdc_u2_clpivot[nTrkClNo_U2]<<endl;
      cout<<"\t V2 cluster number: "<<T->L_tr_V2clsnum[iTr]<<"; pivot wire "<<T->L_vdc_v2_clpivot[nTrkClNo_V2]<<endl;
    }

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
    
    // Run over all clusters in event
    for(int clustno{0}; clustno<T->L_vdc_u1_nclust; clustno++){      
      // Skip if not 5-wire cluster
      if(T->L_vdc_u1_clsiz[clustno]!=5){
	continue;
      }
      else{
	Int_t lPivot = (Int_t)T->L_vdc_u1_clpivot[clustno];
	Int_t lPivotHit{0}, lPivMin1{0}, lPivMin2{0}, lPivPlu1{0}, lPivPlu2{0};

	// Find pivot wire in array of hits
	for(int hit{0}; hit<T->Ndata_L_vdc_u1_wire; hit++){
	  if(T->L_vdc_u1_wire[hit] == lPivot) lPivotHit = hit;	  
	}
	// Check that wires are only used once - going down (assuming that each wire has only been hit twice max)
	if(T->L_vdc_u1_wire[lPivotHit-1] == T->L_vdc_u1_wire[lPivotHit]) lPivMin1 = lPivotHit-2;
	else lPivMin1 = lPivotHit-1;
	if(T->L_vdc_u1_wire[lPivMin1-1] == T->L_vdc_u1_wire[lPivotHit-1]) lPivMin2 = lPivMin1-2;
	else lPivMin2 = lPivMin1-1;
	if(T->L_vdc_u1_wire[lPivotHit+1] == T->L_vdc_u1_wire[lPivotHit]) lPivPlu1 = lPivotHit+2;
	else lPivPlu1 = lPivotHit+1;
	if(T->L_vdc_u1_wire[lPivPlu1+1] == T->L_vdc_u1_wire[lPivotHit+1]) lPivPlu2 = lPivPlu1+2;
	else lPivPlu2 = lPivPlu1+1;

	// Gradients:
	// After pivot wire = (t_{p+2} - t_{p+1})/(wireno_{p+2} - wireno_{p+1})
	// Before pivot wire = (t_{p-1} - t_{p-2})/(wireno_{p-1} - wireno_{p-2})
	
	Float_t lGradAfter  = T->L_vdc_u1_rawtime[lPivotHit+2] - T->L_vdc_u1_rawtime[lPivotHit+1];
	Float_t lGradBefore = T->L_vdc_u1_rawtime[lPivotHit-1] - T->L_vdc_u1_rawtime[lPivotHit-2];
	
	h5WireGrad->Fill(lGradBefore,lGradAfter);

	cout<<"Event "<<ev<<"; cluster "<<clustno<<"\t pivot wire "<<T->L_vdc_u1_clpivot[clustno]<<endl;
	/*cout<<"\t pivot-2: "<<lPivot-2<<" (in array): "<<T->L_vdc_u1_wire[lPivMin2]<<"; rawtime: "<<T->L_vdc_u1_rawtime[lPivotHit-2]<<endl;
	cout<<"\t pivot-1: "<<lPivot-1<<" (in array): "<<T->L_vdc_u1_wire[lPivMin1]<<"; rawtime: "<<T->L_vdc_u1_rawtime[lPivotHit-1]<<endl;
	cout<<"\t pivot+1: "<<lPivot+1<<" (in array): "<<T->L_vdc_u1_wire[lPivPlu1]<<"; rawtime: "<<T->L_vdc_u1_rawtime[lPivotHit+1]<<endl;
	cout<<"\t pivot+2: "<<lPivot+2<<" (in array): "<<T->L_vdc_u1_wire[lPivPlu2]<<"; rawtime: "<<T->L_vdc_u1_rawtime[lPivotHit+2]<<endl;*/
      }
    }

  }
  
  cout<<"------------------------------------------------\nCounting Variables\n------------------------------------------------"<<endl;
  cout<<"In "<<nentries<<" entries:"<<endl;
  cout<<"No. of 1-cluster events (LU1): "<<nOneClust<<"\n\t Where RU1 != 0: "<<nOneClustGivenRight<<endl;
  cout<<"No. of 2-cluster events (LU1): "<<nTwoClust<<"\n\t Where RU1 != 0: "<<nTwoClustGivenRight<<endl;
  cout<<"No. of 3-cluster events (LU1): "<<nThreeClust<<"\n\t Where RU1 != 0: "<<nThreeClustGivenRight<<endl;
  cout<<"No. of (4+)-cluster events (LU1): "<<nMoreClust<<"\n\t Where RU1 != 0: "<<nMoreClustGivenRight<<endl;
  cout<<"Total number of recorded tracks (LHRS): "<<nTotTrackL<<"\n\tWhere LU1 == 0: "<<nTrackWOutVDCL<<endl;
  cout<<"Total number of recorded tracks (RHRS): "<<nTotTrackR<<"\n\tWhere RU1 == 0: "<<nTrackWOutVDCR<<endl;
  cout<<"Events w/ both arms record tracks: "<<nCoinEvt<<"\n\tOne track in both arms: "<<nCoinOneTrackBothArms<<endl;
  
  cout<<"\nCoincidence tracks = "<<((float)nCoinEvt/nentries)*100<<"% of triggers"<<endl;

  gStyle->SetOptStat(00000000);

  TString sCanvName = "cL_" + to_string(runno) + "_" + to_string(evtno);
  TCanvas* cL = new TCanvas("cL",sCanvName,1500,1000);
  cL->SetLeftMargin(0.12);
  cL->SetTopMargin(0.05);
  cL->Divide(1,4,0,0);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cL->cd(4);
  hLU1VDC_WireVsTime->SetMarkerSize(1);
  hLU1VDC_WireVsTime->SetMarkerColor(kMagenta);
  hLU1VDC_WireVsTime->SetMarkerStyle(21);
  hLU1VDC_WireVsTime->Draw();
  
  cL->cd(3);
  hLV1VDC_WireVsTime->SetMarkerSize(1);
  hLV1VDC_WireVsTime->SetMarkerColor(kMagenta+1);
  hLV1VDC_WireVsTime->SetMarkerStyle(21);
  hLV1VDC_WireVsTime->Draw();
  
  cL->cd(2);
  hLU2VDC_WireVsTime->SetMarkerSize(1);
  hLU2VDC_WireVsTime->SetMarkerColor(kMagenta+2);
  hLU2VDC_WireVsTime->SetMarkerStyle(21);
  hLU2VDC_WireVsTime->Draw();
  
  cL->cd(1);
  hLV2VDC_WireVsTime->SetMarkerSize(1);
  hLV2VDC_WireVsTime->SetMarkerColor(kMagenta+3);
  hLV2VDC_WireVsTime->SetMarkerStyle(21);
  hLV2VDC_WireVsTime->Draw();
  
  sCanvName = "cR_" + to_string(runno) + "_" + to_string(evtno);
  TCanvas* cR = new TCanvas("cR",sCanvName,1500,1000);
  cR->SetLeftMargin(0.12);
  cR->SetTopMargin(0.05);
  cR->Divide(1,4,0,0);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cR->cd(4);
  hRU1VDC_WireVsTime->SetMarkerSize(1);
  hRU1VDC_WireVsTime->SetMarkerColor(kAzure);
  hRU1VDC_WireVsTime->SetMarkerStyle(21);
  hRU1VDC_WireVsTime->Draw();

  cR->cd(3);
  hRV1VDC_WireVsTime->SetMarkerSize(1);
  hRV1VDC_WireVsTime->SetMarkerColor(kAzure+1);
  hRV1VDC_WireVsTime->SetMarkerStyle(21);
  hRV1VDC_WireVsTime->Draw();

  cR->cd(2);
  hRU2VDC_WireVsTime->SetMarkerSize(1);
  hRU2VDC_WireVsTime->SetMarkerColor(kAzure+2);
  hRU2VDC_WireVsTime->SetMarkerStyle(21);
  hRU2VDC_WireVsTime->Draw();
  
  cR->cd(1);
  hRV2VDC_WireVsTime->SetMarkerSize(1);
  hRV2VDC_WireVsTime->SetMarkerColor(kAzure+3);
  hRV2VDC_WireVsTime->SetMarkerStyle(21);
  hRV2VDC_WireVsTime->Draw();

  // Clone VDC histograms for plane-by-plane plots
  /*TH2F* hLU1VDC_WireVsTime_Clone = (TH2F*)hLU1VDC_WireVsTime->Clone("hLU1_clone");
    TH2F* hLU2VDC_WireVsTime_Clone = (TH2F*)hLU2VDC_WireVsTime->Clone("hLU2_clone");
    TH2F* hLV1VDC_WireVsTime_Clone = (TH2F*)hLV1VDC_WireVsTime->Clone("hLV1_clone");
    TH2F* hLV2VDC_WireVsTime_Clone = (TH2F*)hLV2VDC_WireVsTime->Clone("hLV2_clone");
    TH2F* hRU1VDC_WireVsTime_Clone = (TH2F*)hRU1VDC_WireVsTime->Clone("hRU1_clone");
    TH2F* hRU2VDC_WireVsTime_Clone = (TH2F*)hRU2VDC_WireVsTime->Clone("hRU2_clone");
    TH2F* hRV1VDC_WireVsTime_Clone = (TH2F*)hRV1VDC_WireVsTime->Clone("hRV1_clone");
    TH2F* hRV2VDC_WireVsTime_Clone = (TH2F*)hRV2VDC_WireVsTime->Clone("hRV2_clone");*/
  
  sCanvName = "cL_split_" + to_string(runno) + "_" + to_string(evtno);
  TCanvas* cL_split = new TCanvas("cL_split",sCanvName,1500,1000);
  cL_split->SetLeftMargin(0.12);
  cL_split->SetTopMargin(0.05);
  cL_split->Divide(2,2);

  cL_split->cd(1);
  hLU1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLU1VDC_WireVsTime_Clone->SetMarkerColor(kMagenta);
  hLU1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLU1VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hLU1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLU1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hLU1VDC_WireVsTime_Clone->Draw();

  cL_split->cd(2);
  hLV1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLV1VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+1);
  hLV1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLV1VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hLV1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLV1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hLV1VDC_WireVsTime_Clone->Draw();

  cL_split->cd(3);
  hLU2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLU2VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+2);
  hLU2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLU2VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hLU2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLU2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hLU2VDC_WireVsTime_Clone->Draw();
  
  cL_split->cd(4);
  hLV2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hLV2VDC_WireVsTime_Clone->SetMarkerColor(kMagenta+3);
  hLV2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hLV2VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hLV2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hLV2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hLV2VDC_WireVsTime_Clone->Draw();

  sCanvName = "cR_split_" + to_string(runno) + "_" + to_string(evtno);
  TCanvas* cR_split = new TCanvas("cR_split",sCanvName,1500,1000);
  cR_split->SetLeftMargin(0.12);
  cR_split->SetTopMargin(0.05);
  cR_split->Divide(2,2);
  
  // Plot electron track from bottom of canvas (U1->V1->U2->V2)
  cR_split->cd(1);
  hRU1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRU1VDC_WireVsTime_Clone->SetMarkerColor(kAzure);
  hRU1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRU1VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hRU1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRU1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hRU1VDC_WireVsTime_Clone->Draw();

  cR_split->cd(2);
  hRV1VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRV1VDC_WireVsTime_Clone->SetMarkerColor(kAzure+1);
  hRV1VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRV1VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hRV1VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRV1VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hRV1VDC_WireVsTime_Clone->Draw();

  cR_split->cd(3);
  hRU2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRU2VDC_WireVsTime_Clone->SetMarkerColor(kAzure+2);
  hRU2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRU2VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hRU2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRU2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hRU2VDC_WireVsTime_Clone->Draw();
  
  cR_split->cd(4);
  hRV2VDC_WireVsTime_Clone->SetMarkerSize(2);
  hRV2VDC_WireVsTime_Clone->SetMarkerColor(kAzure+3);
  hRV2VDC_WireVsTime_Clone->SetMarkerStyle(21);
  hRV2VDC_WireVsTime_Clone->GetXaxis()->SetRangeUser( hRV2VDC_WireVsTime_Clone->ProjectionX()->FindFirstBinAbove(0,1,1,-1)-5 , hRV2VDC_WireVsTime_Clone->ProjectionX()->FindLastBinAbove(0,1,1,-1)+5  );
  hRV2VDC_WireVsTime_Clone->Draw();

  TCanvas* c5WireGrad = new TCanvas("c5wiregrad","c5wiregrad",1500,1500);
  c5WireGrad->SetLeftMargin(0.12);
  c5WireGrad->SetTopMargin(0.05);
  h5WireGrad->Draw("colz");

  return;
}
