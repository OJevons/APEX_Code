//////////////////////////////////////////////////////////////////////////
//
// Bigbite lib replay script definition header
//
// https://hallaweb.jlab.org/wiki/index.php/ReplayCore
//
// It is suggested to use this file set replay environmental variable for 
// different machine. So, they can use same replay script
//	
//////////////////////////////////////////////////////////////////////////
//	
//	Author : Jin Huang (jinhuang@jlab.org)    Aug 2007
//	Modify History:
//
//      27 Mar 08 ole  Change ROOT file names for DetL and PhysL replays
//                     from _det_nnnn to _det_L_nnnn etc.
//
//////////////////////////////////////////////////////////////////////////
//
//  To Do:
//  - Raw File Format - change to tritium
//  - File path names
//
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_REPLAY_DEF
#define ROOT_REPLAY_DEF


//////////////////////////////////////////////////////////////////
// extra structures
//////////////////////////////////////////////////////////////////

#include "TObject.h" //trick to avoid error: `Int_t' does not name a type
#include "TString.h"

//Used for setting paths. %s is necessary so that it can be used to Form the paths.
//This allows changing the directory in fewer places for ease of portability.
const char* REPLAY_DIR_PREFIX = "./%s";
//const char* ROOTFILE_DIR_PREFIX = "./apex_root/%s";
//const char* ROOTFILE_DIR_PREFIX = "/w/work5/jlab/halla/APEX/replays_OJ/TrimAsJW";
const char* ROOTFILE_DIR_PREFIX = "/scratch/oliver/";

typedef struct _sReplaySetUp
{
    Int_t DefReplayNum; //defaut replay event num
    char* OutFileFormat;//output file format
    char* OutDefineFile;//out define
    char* CutDefineFile;//cut define
}sReplaySetUp;


//////////////////////////////////////////////////////////////////
// common defines
//////////////////////////////////////////////////////////////////

static const char* PATHS[] = {
  "/scratch/oliver/",			      
  //"./codafiles",
  //"/w/work3/home/johnw/raw",
  //"/w/work5/jlab/halla/APEX/coda/",
   0
};

//static const char* RAW_DATA_FORMAT="%s/triton_%d.dat.%d";
//need to change when the offical exp swithc happens
static const char* RAW_DATA_FORMAT="%s/apex_%d.dat.%d";

TString STD_REPLAY_OUTPUT_DIR= TString::Format(ROOTFILE_DIR_PREFIX,"Rootfiles");	//pass1 auto hack
//TString STD_REPLAY_OUTPUT_DIR= TString::Format("/volatile/halla/triton/Marathon_Rootfiles/pass1_test");	//pass 1 hack
TString CUSTOM_REPLAY_OUTPUT_DIR= TString::Format(ROOTFILE_DIR_PREFIX,"ScratchROOTfiles");
TString SUMMARY_PHYSICS_FORMAT= TString::Format(REPLAY_DIR_PREFIX,"summaryfiles/summaryphy_%d.log");

//not used since Nov 15, 2008
static const Int_t ANA_MARK_INTERVAL=1000;

// DO NOT DELETE!
#endif//#define ROOT_REPLAY_DEF

