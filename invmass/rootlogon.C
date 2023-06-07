//////////////////////////////////////////////////////////////////////////
//
// rootlogon.C
//
// Load Lib, paths and possible scripts to the analyzer upon start
//
//////////////////////////////////////////////////////////////////////////
//
// Author : Jin Huang <mailto:jinhuang@jlab.org>    Mar 2008
//
//////////////////////////////////////////////////////////////////////////
{
// 	printf("\nrootlogon.C: Loading BigBite Library...");
// 	gSystem->Load("libBigBite.so");
// 	gSystem->Load("onlineGUI/online_C.so");
//	gSystem->Load("$ROOTSYS/lib/libThread.so");
     //	gSystem->Load("libRICH.so");
// 	gSystem->Load("libBigBite.so");

//     //Load more libs here, if necessary.
//     //Make sure it's in path of $LD_LIBRARY_PATH
// 	printf("\nrootlogon.C: Adding include directories...");
//	gSystem->AddIncludePath("-I$ANALYZER");
//	gInterpreter->AddIncludePath("$ANALYZER/");
//



    TVirtualFitter::SetDefaultFitter("Minuit2");

    //    std::cout << "Execute command: " << std::endl;
    //    gSystem->Exec("cd $ROOTSYS/bin; source thisroot.csh; cd -");
    //    gSystem->Exec("cd $ROOTSYS/bin; bash thisroot.sh; cd -");
    
    //    gROOT->ProcessLine(".! bash ${ROOTSYS}/bin/thisroot.sh");
    gROOT->LoadMacro("FillCuts.C+");
		
}

