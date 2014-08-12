
#include <cstdlib>
#include <string>

#include "TChain.h"
#include "Cintex/Cintex.h"

#include "SusyNtuple/SusyNtTruthAna.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/ChainHelper.h"

using namespace std;

/*

    Susy2LepTruthCF - an executable for running a basic looper/dumper of SusyNt

*/

void help()
{
  cout << "  Options:"                          << endl;
  cout << "  -n number of events to process"    << endl;
  cout << "     defaults: -1 (all events)"      << endl;

  cout << "  -k number of events to skip"       << endl;
  cout << "     defaults: 0"                    << endl;

  cout << "  -d debug printout level"           << endl;
  cout << "     defaults: 0 (quiet) "           << endl;

  cout << "  -i input (file, list, or dir)"     << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -s sample name, for naming files"  << endl;
  cout << "     defaults: ntuple sample name"   << endl;

  cout << "  -h print this help"                << endl;
}


int main(int argc, char** argv)
{
  ROOT::Cintex::Cintex::Enable();

  int nEvt = -1;
  int nSkip = 0;
  int dbg = 0;
  string sample;
  string input;
  
  cout << "Susy2LepTruthCF" << endl;
  cout << endl;

  /** Read inputs to program */
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) nEvt = atoi(argv[++i]);
    else if (strcmp(argv[i], "-k") == 0) nSkip = atoi(argv[++i]);
    else if (strcmp(argv[i], "-d") == 0) dbg = atoi(argv[++i]);
    else if (strcmp(argv[i], "-i") == 0) input = argv[++i];
    else if (strcmp(argv[i], "-s") == 0) sample = argv[++i];
    else {
        help();
        return 0;
    }
  }

  cout << "flags:" << endl;
  cout << "  sample  " << sample   << endl;
  cout << "  nEvt    " << nEvt     << endl;
  cout << "  nSkip   " << nSkip    << endl;
  cout << "  dbg     " << dbg      << endl;
  cout << "  input   " << input    << endl;
  cout << endl;

  // Build the input chain
  TChain* chain = new TChain("susyNt");
  ChainHelper::addInput(chain, input, dbg>0);
  Long64_t nEntries = chain->GetEntries();
  chain->ls();

  // Build the TSelector
  SusyNtTruthAna* susyAna = new SusyNtTruthAna();
  susyAna->setDebug(dbg);

  // Run the job
  if(nEvt<0) nEvt = nEntries;
  cout << endl;
  cout << "Total entries:   " << nEntries << endl;
  cout << "Process entries: " << nEvt << endl;
  chain->Process(susyAna, sample.c_str(), nEvt, nSkip);

  cout << endl;
  cout << "Susy2LepTruthCF job done" << endl;

  delete chain;
  return 0;
}
