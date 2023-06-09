//////////////////////////////////////////////////////////////////////////
//
// Hall A analyzer replay script core
//
// https://hallaweb.jlab.org/wiki/index.php/ReplayCore
//
// Call this script after loading all the detectors to run replay
//
// if there is no rootlogon.C, execute following command in analyzer first
//   gSystem->Load("<your lib name>.so");
//  if you want to compile this script then you will also need
//   gSystem->AddIncludePath("-I<your include path>");
//   gInterpreter->AddIncludePath("<your include path>");
//
//////////////////////////////////////////////////////////////////////////
//	
//	Author : Jin Huang (jinhuang@jlab.org)    Aug 2007
//
//	Modify History:
//      Mar 2008    Jin Huang
//          add physics module support
//      Mar 2008    Jin Huang
//          separate two modes: physics replay and detector replay
//          also make it smarter
//      Apr 2008    Jin Huang
//          rootfile overwrite proof
//      May 2008    Jin Huang    
//          Separate core code of replay script to ReplayCore.C
//      May 2008    Jin Huang
//          Support Start Analyzing from an arbitary event number
//          Support Exit when getting an invalid answer.
//          Fix Bugs
//      Oct 2008    Jin Huang
//          Support raw data file name which do not have an endfix index
//      Nov 2008    Jin Huang
//          Clear Cut list before run
//		Nov 2008	Jin Huang
//			Support >2G file size on older compilers
//		Dec 2008    Jin Huang
//			Auto Runs
//
//     Feb 2011 Zhihong Ye
//          Add Helicity
//
//     Dec 2011 Ole Hansen
//          Support compilation via ROOT's ACLic.
//          Add exception handling.
//
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_REPLAY_CORE
#define ROOT_REPLAY_CORE


//#include "def_apex.h"
#include "def_apex_batch.h"
//#include "def_tritium.h"


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>

#ifndef __CINT__
//header file for compile

#include "TSystem.h"
#include "TROOT.h"
//#include "THaEvent.h"  // no longer needed
#include "THaRun.h"
#include "THaAnalyzer.h"
#include "THaGlobals.h"
#include "THaCutList.h"

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include "TSQLServer.h"
#endif//#ifdef __CINT__

#define ALLOW_ROOTFILE_OVERWRITE true 

Bool_t IsFileExist(const Char_t * fname);
void mysql_start(Int_t runnumber);
void mysql_end(Int_t runnumber);

using namespace std;

void ReplayCore(
		Int_t runnumber=0,            //run #
		Int_t all=0,                  //-1=replay all;0=ask for a number
		Int_t DefReplayNum=-1,        //default replay event num
		const char* OutFileFormat="%s/apex_%d.root", //output file format
		const char* OutDefineFile="HRS.odef",       //out define
		const char* CutDefineFile="HRS.cdef",       //cut define
		Bool_t EnableScalar=false,                    //Enable Scalar?
		Bool_t EnableHelicity=false,                  //Enable Helicity?
		Int_t FirstEventNum=0,         //First Event To Replay
		Bool_t QuietRun = kFALSE      //whether not ask question?
		)
{
  //general replay script core
  //
  // to do replay, use a script to load detectors then call this function
  //
  // it's also convenient to load detector manually in analyzer 
  // command line then call me

	int	output_Debug =0; //Flag for debuging the location of the output rootfile. 


  // step 1: Init analyzer
  cout<<"replay: Init analyzer ..."<<endl;
  THaAnalyzer* analyzer = THaAnalyzer::GetInstance();
  if( analyzer ) {
    analyzer->Close();
  } else {
    analyzer = new THaAnalyzer;
  }
  gHaCuts->Clear();

  //Enable scalers
    if (EnableScalar)
      {
      cout<<"replay: Enabling Scalars"<<endl;
      analyzer->EnableScalers();
      }
	
  //Enable Helicity
  if (EnableHelicity)
    {
      cout<<"replay: Enabling Helicity"<<endl;
      analyzer->EnableHelicity();
    }

  // step 2: setup run information
  cout<<"replay: configuring analyzer ..."<<endl;
  int nrun, nev;
  int found = 0;
  const char** path = 0;
  char filename[300],buf[300];  

  while( found==0 ) {
    if (runnumber<=0)
      {
	if (QuietRun)
	  break;
	else
	  {
	    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
	    cin >> nrun;
	    fgets(buf,300,stdin);//get the extra '\n' from stdin
	    if( nrun<=0 ) break;
	  }
      }
    else    nrun=runnumber;

    path=PATHS;

    while ( path && *path ) {
      sprintf(filename,RAW_DATA_FORMAT,*path,nrun,0);
      
      cout<<"replay: Try file "<<filename<<endl;
      if (IsFileExist(filename)) {
	found = 1;
	cout << "replay: Will analyze file "<<filename<<endl;
	break;
      }
      path++;
    }
    if ( (!path || !*path) && !found ) {
      cout << "replay: File not found. \n";
      if (runnumber>0) runnumber=0;
    }
  }
  if(nrun<=0) {
    gHaApps->Delete();
    gHaPhysics->Delete();
    gHaEvtHandlers->Delete();
    analyzer->Close();
    return;
  }


  if (all==0)
    {
      if (QuietRun)
	{
	  nev=DefReplayNum;
	}
      else
	{
	  cout << "\nreplay: Number of Events to analyze (use default for online check, -1 = replay all) "
	       <<" (default=";
	  if (DefReplayNum>0)
	    cout<<DefReplayNum<<"):";
	  else 
	    cout<<"replay all):";	
	  
	  //trick to support blank inputs
	  fgets(buf,300,stdin);
	  if (sscanf(buf,"%d\n",&nev)<1)
	    nev=DefReplayNum;
	}
    }
  else 
    nev=all;

  char outname[300];

  sprintf(outname,OutFileFormat,STD_REPLAY_OUTPUT_DIR.Data(),nrun);
	
	if(output_Debug==1){		
  		cout << "OutFileFormat =    " << OutFileFormat << endl;
  		cout << "ROOTFILE_DIR_PREFIX =   " << ROOTFILE_DIR_PREFIX << endl;
  		cout << "STD_REPLAY_OUTPUT_DIR =   " << STD_REPLAY_OUTPUT_DIR.Data() << endl;
  		cout << "nrun =    " << nrun << endl;
  		cout << "outname 1:   " << outname << endl;
	}
  found=0;    
  //rootfile overwrite proof
  while (found==0 && !QuietRun)
    {
      cout << "replay: Testing file "<<outname<<" for overwrite proof."<<endl;

      if (!IsFileExist(outname)) {found=1;break;}
      else
	{
	  Long64_t NEnt=0;
	  TFile *f=new TFile(outname);
	  if (f)
	    {
	      TTree *t=(TTree *)f->Get("T");
	      if (t!=0)  NEnt = (t->GetEntries());
	    }
	  delete f;
	  TString DefOverWriting;
	  if (NEnt==DefReplayNum || DefReplayNum<0 || NEnt==0)
	    DefOverWriting="no";
	  else DefOverWriting="yes";

	  if (NEnt<=0) {
	    cout <<endl << "replay: "<<outname
		 <<" is an invalid root file, "
		 <<"or other people is replaying it. ";
	  }
	  else{
	    cout << "replay: "<<outname<<", which contains "<<NEnt
		 <<" events, already exists. ";
	  }
#if ALLOW_ROOTFILE_OVERWRITE
	  cout<<"Do you want to overwrite it? "
	      <<"(default="<<DefOverWriting.Data()<<"; enter \"c\" means exit):";

	  //trick to support blank inputs
	  fgets(buf,300,stdin);
	  TString s(buf);
	  s.Chop();
	  s.ToLower();
	  if (s.IsNull()) s=DefOverWriting;

	  if (s=="y" || s=="yes") {
		  found=1;

		  // Longwu: add to remove old rootfiles
		  TString rootfilebase = outname, rootfilename = outname;
		  rootfilebase.Remove(rootfilebase.Last('.'),5);
		  Long_t split = 0;
		  cout << "Overwriting old files!" << endl;
		  while ( !gSystem->AccessPathName(rootfilename.Data()) ) {
			  cout << "Removing old ROOT file " << rootfilename.Data() << endl;
			  gSystem->Exec(Form("rm %s",rootfilename.Data()));
			  split++;
			  rootfilename = rootfilebase + "_" + split + ".root";
		  }

		  break;
	  }
	  else if (s=="n" || s=="no"){
	    sprintf(outname,OutFileFormat,
		    CUSTOM_REPLAY_OUTPUT_DIR.Data(),nrun);
	    cout<<endl
		<<"replay: please enter the output root file name. "<<endl
		<<"        leave blank = "<<outname<<endl
		<<"Root File:";

	    //trick to support blank inputs
	    fgets(buf,300,stdin);
	    if (buf[0]!='\n' and buf[0]!=0){
	      strcpy(outname,buf);
	    }

	    //clear the last char if it's '\n'
	    if (outname[strlen(outname)-1]=='\n') outname[strlen(outname)-1]=0;
	  }
	  else {
	    cout<<"replay: " 
		<<s.Data()
		<<"is not a valid input; Exiting."
		<<endl;
#else
cout << endl 
     << endl << "------------------------------------------"
     << endl << "Rootfile Overwriting is currently disabled." 
     << endl << "If you need to create this replay, please communicate with whoever created the current replay to determine if you may delete it."
     << endl << "------------------------------------------" << endl << endl;
#endif
	    gHaApps->Delete();
	    gHaPhysics->Delete();
	    analyzer->Close();
	    return;
#if ALLOW_ROOTFILE_OVERWRITE
	  }
#endif
	}
    }


  // insert info to msql
  //  if (nev<0) mysql_start(runnumber);



  cout<<endl<<"----------------------------------------------"<<endl;
  cout<<"replay: Inputs Summary:"<<endl;
  cout<<"        Raw data: "<<filename<<endl;
  cout<<"        Event   : ";
  if (nev<0) cout <<"all events";
  else cout << nev <<" events";
  cout<<", starting from Event # "<<FirstEventNum<<endl;
  cout<<"        Outputs : "<<outname<<endl;
  cout<<"----------------------------------------------"<<endl<<endl;

  cout<<"replay: Setup run inputs/outputs ..."<<endl;

  analyzer->SetOutFile( outname );
  analyzer->SetOdefFile(OutDefineFile);
  analyzer->SetCutFile(CutDefineFile);  
  char sumname[300];
  sprintf(sumname,SUMMARY_PHYSICS_FORMAT.Data(),nrun);
  analyzer->SetSummaryFile(sumname); // optional

  //correct the offset on the last event if first event is above 0
  if (nev>=0) nev+=FirstEventNum;

  // step 3: run it
  cout<<"replay: Start Process ..."<<endl;

  TString oldfilename="";
  THaRun *oldrun=0, *run, *runlist[30]={0};Int_t runidx=0;
  Bool_t exit=false;
  
  for (Int_t nsplit=0;!exit;nsplit++)
    {

      sprintf(filename,RAW_DATA_FORMAT,"raw data paths",nrun,nsplit);
      cout<<"replay: trying file "<<filename<<endl;

      path=PATHS;found=0;
      while ( path && *path ) {
	sprintf(filename,RAW_DATA_FORMAT,*path,nrun,nsplit);
	cout<<"replay: Try file "<<filename<<endl;
			
	if (IsFileExist(filename)) {
	  found = 1;
	  cout << "replay: Will analyze file "<<filename<<endl;
	  break;
	}
	path++;
      }

      if ( ((!path || !*path) && !found) || oldfilename==filename) {
	cout << "replay: no more raw data file to analyze"<<endl;
	exit=true;
      }
      else{
	oldfilename=filename;
	//do the analysis
	if( oldrun ) {
	  run = new THaRun(*oldrun);
	  run->SetFilename(filename);
	} else {
	  run = new THaRun(filename);
	}
	runlist[runidx]=run; runidx++;

	if(nev>=0) run->SetLastEvent(nev);
	run->SetFirstEvent(FirstEventNum);

	try {
	  analyzer->Process(run); 
	}
	catch( exception& e ) {
	  cerr << "Unhandled exception during replay: " << e.what() << endl;
	  cerr << "Exiting." << endl;
	  run->Close();
	  break;
	}
	
	run->Close();
	if (!oldrun) oldrun = run;
      }
    }



  // step 3: clean up
  cout<<"replay: Cleaning up ... "<<endl;
  for (runidx--;runidx>=0;runidx--){
    assert(runlist[runidx]);
    delete runlist[runidx];
  }
  gHaApps->Delete();
  gHaPhysics->Delete();
  gHaEvtHandlers->Delete();
  analyzer->Close();

  cout<<"replay: YOU JUST ANALYZED RUN number "<<nrun<<"."<<endl;

 
  // insert info to msql
  // if (nev<0) mysql_end(runnumber);

}

/////////////////////////////////////////////////////////////////

Bool_t IsFileExist(const Char_t * fname)
{
  fstream testfile;

  testfile.open(fname,ios_base::in);
  Bool_t isopen=testfile.is_open();
  testfile.close();

  return isopen;
}

//=========================================
  //    update mysql database 
  //=========================================


  //start of run
void mysql_start(Int_t runnumber){
    TSQLServer* Server = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
    TString query(Form("insert into fullreplay (run,anastart,start_when,anadone,end_when) values (%d,1,now(),0,0) on duplicate key update anastart=values(anastart),anadone=values(anadone),start_when=now(),end_when=0",runnumber));
    TSQLResult* result=Server->Query(query.Data());
    Server->Close();
    return;
  }

// end of run
void mysql_end(Int_t runnumber){
    TSQLServer* Server = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
    TString query(Form("update fullreplay set anadone=1,end_when=now() where run=%d",runnumber));
    TSQLResult* result=Server->Query(query.Data());
    cout<<query.Data()<<endl;
    Server->Close();
    return;
  }

//========================================

// DO NOT DELETE!
#endif//#define ROOT_REPLAY_CORE
