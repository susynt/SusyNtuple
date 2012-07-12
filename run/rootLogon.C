//----------------------------------------------------------------------------
//  rootlogon.C: a sample ROOT logon macro allowing use of ROOT script.
//               The name of this macro file is defined by the .rootrc file
//               loaded automatically via .rootrc
//
//  Dec 17 2008 A.Taffard
//
//----------------------------------------------------------------------------
{
#include <string>
#include <iomanip.h>
#include <time.h>
#include <TObjectTable.h>

  using namespace std;  

  // this line reports the ROOT process ID .. helpful to kill ROOT !
  gInterpreter->ProcessLine(".! ps | grep root.exe");
  gErrorIgnoreLevel=kError;   //disable ROOT warning

  cout <<"WORKAREA = "<<getenv("WORKAREA") <<endl;

  cout<<"Begin loading libs, 0 = success: "<<endl;

  cout<<"\t libMinuit = "<<gSystem->Load("libMinuit")<<endl;
  cout<<"\t libMinuit2 = "<<gSystem->Load("libMinuit2")<<endl;
  cout<<"\t libXmlParser = "<<gSystem->Load("libXMLParser")<<endl;

  //Load all the libs 
  gSystem->Exec("more $ROOTCOREDIR/packages |cut -d\"/\" -f7-8 >libs.list");
  FILE* fInput;
  if ((fInput = fopen("libs.list","r")) == NULL) {
    printf("File libs.list could not be opened. Exit\n");
    abort();
  }
  char _lib[200];
  while (!feof( fInput )) {
    if (fscanf(fInput, "%s\n",&_lib)){
      string cmd = string(getenv("ROOTCOREDIR")) + "/lib/lib";
      std::cout << "Lib " << cmd << std::endl;
      cmd.append(_lib);
      cmd += ".so";
      //cout << "test " << cmd << endl;
      cout<<"\t Loading " << _lib << " "  <<gSystem->Load(cmd.c_str()) << endl;
    }
  }

  cout<<endl;

  gInterpreter->AddIncludePath("$ROOTCOREDIR/include");
  gSystem->AddIncludePath(" -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXEPTIONS ");  

#include <SusyNtuple/TGuiUtils.h>
  TGuiUtils* utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  utils->atlasStyle->SetOptStat("emr");

  cout<<endl;
  cout<<endl;

  gROOT->LoadMacro("browseNt.C");
  
}
