//SusyNtuple
#include "SKELETON_LOOPER_CLASS/SKELETON_LOOPER_CLASS.h"
#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/string_utils.h"

//std/stl
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

//ROOT
#include "TChain.h"

//////////////////////////////////////////////////////
//
// SKELETON_DESCRIPTION 
//
//
//////////////////////////////////////////////////////


void help()
{
    cout << "----------------------------------------------------------" << endl;
    cout << " SKELETON_EXEC_NAME" << endl;
    cout << endl;
    cout << "  Options:" << endl;
    cout << "   -n          number of events to process (default: all)" << endl;
    cout << "   -d          debug level (integer) (default: 0)" << endl;
    cout << "   -i          input file (ROOT file, *.txt file, or directory)" << endl;
    cout << "   -h          print this help message" << endl;
    cout << endl;
    cout << "  Example Usage:" << endl;
    cout << "   SKELETON_EXEC_NAME -i susyNt.root -n 500" << endl;
    cout << "----------------------------------------------------------" << endl;
}

int main(int argc, char** argv)
{

    /////////////////////////
    // cmd line options
    /////////////////////////

    int n_events = -1;
    int dbg = 0;
    string input = "";

    for(int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-n") == 0) n_events = atoi(argv[++i]);
        else if (strcmp(argv[i], "-d") == 0) dbg = atoi(argv[++i]);
        else if (strcmp(argv[i], "-i") == 0) input = argv[++i];
        else if (strcmp(argv[i], "-h") == 0) { help(); return 0; }
        else {
            cout << "SKELETON_EXEC_NAME    Unknown command line argument '" << argv[i] << "', exiting" << endl;
            help();
            return 1;
        }
    } // i

    if(input.empty()) {
        cout << "SKELETON_EXEC_NAME    You must specify an input" << endl;
        return 1;
    }


    /////////////////////////////////////////////////////////
    // Build the TChain object
    // For SusyNtuple analysis, the chain name is susyNt
    /////////////////////////////////////////////////////////
    TChain* chain = new TChain("susyNt");

    // use ChainHelper to infer the input type (ROOT file, *.txt, or dir/)
    // and build the full chain of the input files
    // (c.f. SusyNtuple/ChainHelper.h)
    ChainHelper::addInput(chain, input, dbg>0);
    Long64_t n_entries_in_chain = chain->GetEntries();
    // let's see what it looks like
    chain->ls();

    /////////////////////////////////////////////////////////
    // Build the TSelector object
    // SusyNt analyses inheriting from SusyNtAna must
    // build their own TSelector looper
    /////////////////////////////////////////////////////////
    SKELETON_LOOPER_CLASS* analysis = new SKELETON_LOOPER_CLASS();

    // set to do the 2 lepton analysis object selection (c.f. SusyNtuple/AnalysisType.h)
    // the AnalysisType configures all of the selector tools (c.f. SusyNtuple/SusyNtTools.h)
    analysis->setAnaType(AnalysisType::Ana_2Lep);

    analysis->set_debug(dbg);
    analysis->setSampleName(ChainHelper::sampleName(input, dbg>0)); // SusyNtAna setSampleName (c.f. SusyNtuple/SusyNtAna.h)
    analysis->set_chain(chain); // propagate the TChain to the analysis

    // for using the TriggerTools (c.f. SusyNtuple/TriggerTools.h) we
    // must provide the first file in our chain to initialize the
    // underlying TriggerTool object (to inspect the "trig" histogram
    // stored in the susyNt file)

    // we use the inherited SusyNtTools object from SusyNtAna base class
    // to initialize the underlying TriggerTool object
    analysis->nttools().initTriggerTool(ChainHelper::firstFile(input, dbg>0));

    if(n_events < 0) n_events = n_entries_in_chain;

    cout << "---------------------------------------------------------" << endl;
    cout << " Total entries in input chain          : " << n_entries_in_chain << endl;
    cout << " Total entries to process for analysis : " << n_events << endl;
    cout << "---------------------------------------------------------" << endl;

    
    // call TChain Process to star the TSelector looper over the input TChain
    if(n_events > 0) chain->Process(analysis, input.c_str(), n_events);

    cout << endl;
    cout << "SKELETON_EXEC_NAME    Analysis loop done" << endl;

    delete chain;
    return 0;
}
