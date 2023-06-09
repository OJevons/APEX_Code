/* 
 * APEX Replay Script
 * Works for non-coincidence experiments using 2 daqs
 * Designed for reduced output
 */


//#include "def_tritium.h"
#include "def_apex.h"
//#include "def_apex_batch.h"
using namespace std;

// #define RIGHT_ARM_CONDITION runnumber>=20000
// #define LEFT_ARM_CONDITION  runnumber<20000   //replace purely with given parameter

void replay_apex_trim(Int_t runnumber=0,Int_t numevents=0,Int_t fstEvt=0,Bool_t QuietRun = kFALSE, Bool_t OnlineReplay =kFALSE, Bool_t bPlots = kFALSE, Bool_t left = kTRUE, Bool_t right = kTRUE, Bool_t autoreplay = kFALSE, Bool_t skim = kFALSE, Bool_t coinc_cut = kFALSE){

  char buf[300];
  Int_t nrun=0;

  if (runnumber<=0)
  {
	    cout << "\nreplay: Please enter a Run Number (-1 to exit):";
	    cin >> nrun;
	    fgets(buf,300,stdin);//get the extra '\n' from stdin
	    if( nrun<=0 ) return;
	    runnumber = nrun;
  }

   
  //Enable modules
  Bool_t bScaler   =   kTRUE;
  Bool_t bHelicity =   kFALSE;
  Bool_t bBeam     =   kTRUE;
  Bool_t bPhysics  =   kTRUE;
  Bool_t bEloss    =   kFALSE;
  Bool_t bOldTrack =   kFALSE;
  Bool_t bRaster   =   kTRUE;   


  TString rootname;
  if(OnlineReplay){
    //rootname = "%s/apex_online_3P_full_%d.root";}
    //    rootname = "%s/apex_online_3P_analytic_hit_3P_fit_3_above_clust_skippiv_exit_%d.root";}
    //    rootname = "%s/apex_online_3P_Test_All_%d.root";}
    //    rootname = "%s/apex_online_3P_Lookup_test_Chamb_noparam_%d.root";}
    rootname = "%s/apex_online_2PA_Lookup_newS2_altRS2_%d.root";}
  else {
    rootname = "%s/apex_Trim_%d.root";
    if (skim)
      rootname = "%s/skim_%d.root";
  }
  
  const char* exp=getenv("EXPERIMENT");
  exp = "APEX"; // short-term fix

  const char* RNAME=rootname.Data();
  TString ODEF;
  TString CUTS;


 
  //==================================
  //  Both arms (default)
  //==================================
  if(right && left){
    ODEF = "coinc_trim.odef";
    CUTS = "coinc.cuts";

    if(coinc_cut){
      CUTS = "coinc_online.cuts";
    }

    

    if(autoreplay)  ODEF=Form(REPLAY_DIR_PREFIX,"coinc_auto.odef");

	//=====================================================================================================================
	// Detectors
	//==================================
	// LHRS Detectors
	//==================================	
	//THaHRS *HRSL = new THaHRS("L","Left arm HRS"); //Add vdc,s2...uses s0 for track beta
	Tritium_HRS* HRSL = new Tritium_HRS("L","Left arm HRS");
	HRSL->AutoStandardDetectors(kFALSE);
	gHaApps->Add( HRSL );
	HRSL->AddDetector( new TriFadcXscin    ("s0"  , "s0 scintillator",kFALSE )); // boolean is about the fact that pmts A and B are flipped in the HRSs
	HRSL->AddDetector( new THaVDC          ("vdc" , "Vertical Drift Chamber" ));
	HRSL->AddDetector( new TriFadcCherenkov("cer" , "Gas Cherenkov counter"  ));
	HRSL->AddDetector( new TriFadcScin     ("s2"  , "S2 Scintillator"        ));
	HRSL->AddDetector( new TriFadcShower       ("prl1", "Pre-shower pion rej."   ));
	HRSL->AddDetector( new TriFadcShower       ("prl2", "Show pion rej."         ));
	HRSL->AddDetector( new THaTriggerTime       ("trg", "Trigger Times"         )); 


	//==================================
	// RHRS Detectors
	//==================================
	// THaHRS* HRSR = new THaHRS("R","Right arm HRS");
	Tritium_HRS* HRSR = new Tritium_HRS("R","Right arm HRS");
	HRSR->AutoStandardDetectors(kFALSE);
	gHaApps->Add( HRSR );
	HRSR->AddDetector( new TriFadcXscin    ("s0" , "s0 scintillator",kTRUE        ));
	HRSR->AddDetector( new THaVDC          ("vdc", "Vertical Drift Chamber"       ));
	HRSR->AddDetector( new TriFadcCherenkov("cer", "Gas Cherenkov counter - FADC" ));
	HRSR->AddDetector( new TriFadcScin     ("s2" , "S2 Scintillator - FADC"       ));
	HRSR->AddDetector( new THaShower       ("ps" , "Pre-shower pion rej."         ));
	HRSR->AddDetector( new THaShower       ("sh" , "Show pion rej."               ));
	HRSR->AddDetector( new THaTriggerTime       ("trg", "Trigger Times"         )); 


	//==================================
	if(bOldTrack){
		THaApparatus* OldTrackL = new TriHRS("OldTrackL","old analyzer track");
		OldTrackL->AddDetector( new TriXscin("s0","s0 sintillator",kFALSE) );
		gHaApps->Add( OldTrackL );

		THaApparatus* OldTrackR = new TriHRS("OldTrackR","old analyzer track");
		OldTrackR->AddDetector( new TriXscin("s0","s0 sintillator",kTRUE) );
		gHaApps->Add( OldTrackR );
	}

	//=====================================================================================================================
	// Scalers
	//==================================
	if(bScaler){	
		//==================================
		// LHRS Scalers
		//==================================
		TriScalerEvtHandler* lscaler = new TriScalerEvtHandler("Left","HA scaler event type 140 on L-HRS");
		gHaEvtHandlers->Add(lscaler);

		Tritium_TSScaler* levscaler = new Tritium_TSScaler("evLeft","HA scaler event type 1-14 on L-HRS");
		gHaEvtHandlers->Add(levscaler);

		Tritium_THaScaler100EvtHandler* lEndscaler = new Tritium_THaScaler100EvtHandler("EndLeft","HA scaler event type 100");
		gHaEvtHandlers->Add(lEndscaler);

		

		// Evan - V1495 Clock Counter:
		gHaEvtHandlers->Add (new ClockCountEvtHandler("LV1495","V1495 LHRS"));// do not change the "LV1495" word

		//==================================
		// RHRS Scalers
		//==================================
		TriScalerEvtHandler* rscaler = new TriScalerEvtHandler("Right","HA scaler event type 140 on R-HRS");
		gHaEvtHandlers->Add(rscaler);

		Tritium_TSScaler* revscaler = new Tritium_TSScaler("evRight","HA scaler event type 1-14 on R-HRS");
		gHaEvtHandlers->Add(revscaler);

		Tritium_THaScaler100EvtHandler* rEndscaler = new Tritium_THaScaler100EvtHandler("EndRight","HA scaler event type 100");
		gHaEvtHandlers->Add(rEndscaler);
		
		// Evan - V1495 Clock Counter:
		gHaEvtHandlers->Add (new ClockCountEvtHandler("RV1495","V1495 RHRS")); //do not change the "RV1495"  word
	}

	//=====================================================================================================================
	// Decoder Data
	//==================================
	// RHRS
	THaApparatus* decR = new THaDecData("DR","Misc. Decoder Data");
	gHaApps->Add( decR );
	//==================================
	// LHRS
	THaApparatus* decL = new THaDecData("DL","Misc. Decoder Data");
	gHaApps->Add( decL );

	//=====================================================================================================================
	// Beam
	//==================================
	if(bBeam){	


		//==================================
		// LHRS Beam
		//==================================

		TriFadcUnRasteredBeam * Lurb = new TriFadcUnRasteredBeam("Lurb","Unrastered beam to LHRS");
		gHaApps->Add(Lurb);

		TriFadcRasteredBeam* Lrb = new TriFadcRasteredBeam("Lrb", "Rastered beam to LHRS");
		gHaApps->Add(Lrb);


		//==================================
		// RHRS Beam
		//==================================	

		TriFadcUnRasteredBeam * Rurb = new TriFadcUnRasteredBeam("Rurb","Unrastered beam to RHRS");
                gHaApps->Add(Rurb);

		TriFadcRasteredBeam* Rrb = new TriFadcRasteredBeam("Rrb", "Rastered beam to the RHRS");
		gHaApps->Add(Rrb);

	}  

	//=====================================================================================================================
	// Physics
	//==================================
	if(bPhysics){
		Double_t mass_el  = 0.000511      ; // electron mass
		Double_t amu      = 931.494*1.e-3 ; // amu to GeV
		Double_t mass_He3 = 3.0160293 *amu;
		Double_t mass_H2  = 2.01410178*amu;
		Double_t mass_H3  = 3.0160492 *amu;
		Double_t mass_tg  = 0.938         ; //default target 
		Double_t mass_prot= 0.938         ; //GeV proton mass

		string word[5],line;
		Double_t Z, A, density;

		// File to look up the target enconder position to determine the target for this run
		//TString filename = Form("/adaqfs/home/adaq/epics/runfiles_tritium_Coinc/Start_of_Run_%d.epics",runnumber);
		TString filename = Form("../runfiles_tritium_Coinc/Start_of_Run_%d.epics",runnumber);

		ifstream infile;
		infile.open(filename);
		double pos=0       ;
		double t2 =33106235;
		double d2 =29367355;
		double h2 =25610043;
		double he3=21875520;

		while(getline(infile,line)){
			istringstream str(line);
			str>>word[0]>>word[1];
			if(word[0]=="Encoder" && word[1]=="Position"){
				str>>word[2]>>word[3];
				pos = atof(word[3].c_str()); 

				// Tritium target
				if     (abs(pos-t2 )<100){
					mass_tg = mass_H3  ;
					Z       = 1.;
                                        A       = 3.;
                                        density = 0.003065; // g/cm3
				}
				// Deuterium target
				else if(abs(pos-d2 )<100){
					mass_tg = mass_H2  ;
					Z       = 1;
					A       = 2;
					density = 0.005686; // g/cm3
				}
				// Helium-3 target
				else if(abs(pos-he3)<100){
					mass_tg = mass_He3 ;
					Z       = 2.;
                                        A       = 3.;
                                        density = 0.002; // g/cm3
				}
				// Hydrogen target
				else if(abs(pos-h2 )<100){
					mass_tg = mass_prot;
					Z       = 1.;
                                        A       = 1.;
                                        density = 0.002832; // g/cm3
				}
				else{
					bEloss=kFALSE;
					cout << "WARNING: WON'T BE DOING ENERGY LOSS CORRECTIONS" << endl;
				}
				break;
			}
		}
		

		//==================================
		// LHRS
		//==================================
		THaPhysicsModule *Lgold   = new THaGoldenTrack  ("L.gold"   ,"LHRS Golden Track"                       ,"L"                 );
		THaReactionPoint *rpl     = new THaReactionPoint("rpl"      ,"Reaction vertex for LHRS"                ,"L"   ,"Lrb"        );
		THaExtTarCor     *exL     = new THaExtTarCor    ("exL"      ,"Corrected for extended target, LHRS"     ,"L"   ,"rpl"        );

		gHaPhysics->Add(Lgold  );
		//		gHaPhysics->Add(Lvdceff);
		gHaPhysics->Add(rpl    );
		gHaPhysics->Add(exL    );

		//==================================
		// RHRS
		//==================================
		THaPhysicsModule *Rgold   = new THaGoldenTrack  ("R.gold"    ,"RHRS Golden Track"                      ,"R"                     );
		THaReactionPoint *rpr     = new THaReactionPoint("rpr"       ,"Reaction vertex for RHRS"               ,"R"    ,"Rrb"           );
		THaExtTarCor     *exR     = new THaExtTarCor    ("exR"       ,"Corrected for extended target, RHRS"    ,"R"    ,"rpl"           );

		gHaPhysics->Add(Rgold  );
		//		gHaPhysics->Add(Rvdceff);
		gHaPhysics->Add(rpr    );
		gHaPhysics->Add(exR    );

		//=====================================================================================================================		
	}
	
  }

  
  //=====================================
  //  Set up Analyzer and replay data
  //=====================================
  ReplayCore(
	     runnumber,        //run #
	     numevents,        //-1=replay all;0=ask for a number
	     50000,            //default replay event num
	     RNAME,            //output file format
	     ODEF.Data(),	   //out define
	     CUTS.Data(), 	   //empty cut define
	     bScaler,          //replay scalar?
	     bHelicity,        //repaly helicity
	     fstEvt,	       //First Event To Replay
	     QuietRun	       //whether ask user for inputs
	     );

  //=====================================
  //Generate online plots
  //=====================================
  // if(bPlots){

  //   cout << " passed bplots condition " << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;
  //   cout << endl << endl << endl << endl << endl << endl << endl << endl;
								      
  //  const char* GUI_DIR = Form(REPLAY_DIR_PREFIX,"onlineGUI64/");
  //  const char* SUM_DIR = Form(REPLAY_DIR_PREFIX,"summaryfiles/");


  //    if(!left && right){

  //      cout << "Passed RIGHT ARM condition for plots" << endl;
  //      const char* CONFIGFILE=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS.cfg");
  //      const char* CONFIGFILEPHYS=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_phy.cfg");

  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILE,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_detectors_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf %sright_detectors_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf /chafs2/work1/%s/Run_pdfs/right_detectors_latest.pdf",exp,runnumber,exp));
              
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_physics_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf %sright_physics_latest.pdf",exp,runnumber,SUM_DIR));    
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf /chafs2/work1/%s/Run_pdfs/right_physics_latest.pdf",exp,runnumber,exp));
     
  //      const char* config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_online.cfg");
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, config_online,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_online_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_online_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_online_%d.pdf %sright_online_latest.pdf",exp,runnumber,SUM_DIR)); 
  //    }
    
  //    else if(left && !right){ 

  //      const char* CONFIGFILE_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS.cfg");
  //      const char* CONFIGFILEPHYS_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_phy.cfg");
       
  //      cout << "Passed LEFT arm condition for plots" << endl;
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILE_L,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_detectors_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf %sleft_detectors_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf /chafs2/work1/%s/Run_pdfs/left_detectors_latest.pdf",exp,runnumber,exp));
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS_L,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_physics_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf %sleft_physics_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf /chafs2/work1/%s/Run_pdfs/left_physics_latest.pdf",exp,runnumber,exp));
       
  //      const char* config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_online.cfg");
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, config_online,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_online_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_online_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_online_%d.pdf %sleft_online_latest.pdf",exp,runnumber,SUM_DIR));


  //    }
  // }
  //    else if(left && right){
       
  //      cout << "Passed COINCIDENCE (both arms) condition for plots" << endl;

       
  // 		//==================================
  // 		// LHRS
  // 		//==================================
       
  //      const char* CONFIG_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS.cfg");
  //      const char* CONFIGPHYS_L=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_phy.cfg");
       
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d"                                   ,GUI_DIR      ,CONFIG_L ,runnumber              ));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_detectors_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf %sleft_detectors_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/left_detectors_%d.pdf /chafs2/work1/%s/Run_pdfs/left_detectors_latest.pdf",exp,runnumber,exp));
       
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGPHYS_L,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_physics_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf %sleft_physics_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/left_physics_%d.pdf /chafs2/work1/%s/Run_pdfs/left_physics_latest.pdf",exp,runnumber,exp));
       
  //      const char* config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/LHRS_online.cfg");

  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, config_online,runnumber)); 
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/left_online_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sleft_online_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/left_online_%d.pdf %sleft_online_latest.pdf",exp,runnumber,SUM_DIR));
       
       
       
       
  //      //==================================
  //      // RHRS
  //      //==================================
  //      const char* CONFIGFILE_R=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS.cfg");
  //      const char* CONFIGFILEPHYS=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_phy.cfg");
       
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d"                                     ,GUI_DIR      ,CONFIGFILE_R ,runnumber              ));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_detectors_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf %sright_detectors_latest.pdf",exp,runnumber,SUM_DIR));
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/right_detectors_%d.pdf /chafs2/work1/%s/Run_pdfs/right_detectors_latest.pdf",exp,runnumber,exp));
       


  //      config_online=Form(REPLAY_DIR_PREFIX,"onlineGUI64/RHRS_online.cfg");


  //      gSystem->Exec(Form("%sonline -P -f %s -r %d", GUI_DIR, config_online,runnumber)); 
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_online_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_online_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_online_%d.pdf %sright_online_latest.pdf",exp,runnumber,SUM_DIR)); 


  //      SUM_DIR = Form(REPLAY_DIR_PREFIX,"summaryfiles/"); // not sure why this had to be added, but SUM_DIR seem to be redefined otherwise

  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR, CONFIGFILEPHYS,runnumber));
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
  //      gSystem->Exec(Form("unlink %sright_physics_latest.pdf",SUM_DIR));
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf %sright_physics_latest.pdf",exp,runnumber,SUM_DIR));    
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/right_physics_%d.pdf /chafs2/work1/%s/Run_pdfs/right_physics_latest.pdf",exp,runnumber,exp));
       

  //      //==================================
  //      // Both arms combined
  //      //==================================
  //      const char* CONFIGCOINC=Form(REPLAY_DIR_PREFIX,"onlineGUI64/coinc.cfg");
  //      // const char* CONFIGCOINCPHYS = Form(REPLAY_DIR_PREFIX,"onlineGUI64/config_coinc_2018/Coinc_physics.cfg");

       
  //      GUI_DIR = Form(REPLAY_DIR_PREFIX,"onlineGUI64/");
       
  //      gSystem->Exec(Form("%sonline -P -f %s -r %d",GUI_DIR,CONFIGCOINC,runnumber));
  //      // gSystem->Exec(Form("%sonline -P -f %s -r %d"                                     ,GUI_DIR      ,CONFIGCOINC,runnumber                   ));
      		
  //      SUM_DIR = Form(REPLAY_DIR_PREFIX,"summaryfiles/");
       
  //      gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/coinc_%d.pdf",SUM_DIR,runnumber,exp,runnumber));
       
  //      gSystem->Exec(Form("unlink %scoinc_latest.pdf",SUM_DIR));
       
  //      gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/coinc_%d.pdf %scoinc_latest.pdf",exp,runnumber,SUM_DIR));
          
  //      gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/coinc_%d.pdf /chafs2/work1/%s/Run_pdfs/coinc_latest.pdf",exp,runnumber,exp));
       
       
  //      //gSystem->Exec(Form("%sonline -P -f %s -r %d"                                     ,GUI_DIR      ,CONFIGCOINCPHYS ,runnumber              ));
  //      //gSystem->Exec(Form("mv %stemp_%d.pdf /chafs2/work1/%s/Run_pdfs/coinc_physics_%d.pdf"                     ,SUM_DIR      ,runnumber       ,exp,runnumber));
  //      //gSystem->Exec(Form("unlink %scoinc_physics_latest.pdf"                           ,SUM_DIR                                               ));
  //      //gSystem->Exec(Form("ln -s /chafs2/work1/%s/Run_pdfs/coinc_physics_%d.pdf %scoinc_physics_latest.pdf"     ,exp,runnumber       ,SUM_DIR                ));    
  //      //gSystem->Exec(Form("ln -sf /chafs2/work1/%s/Run_pdfs/coinc_physics_%d.pdf %scoinc_physics_latest.pdf"    ,exp,runnumber       ,exp          ));
  //    }

       


	 
	 
     

  // }

  exit(0);
}
 

