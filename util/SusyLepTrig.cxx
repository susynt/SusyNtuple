
//std/stl
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

//ROOT
#include "TChain.h"

//SusyNtuple
#include "SusyNtuple/SusyLepTrigExample.h"
#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/string_utils.h"

void help()
{
    cout << "-----------------------------------------------------" << endl;
    cout << "  SusyLepTrigExample " << endl;
    cout << endl;
    cout << "  Options:" << endl;
    cout << "   -n            number of events to process (default: -1 (all))" << endl;
    cout << "   -i            input file (ROOT file, *.txt file, or dir)" << endl;
    cout << "   -h            print this help message" << endl;
    cout << "   --lead-pt     lead lepton pT minimum in GeV (default: 25)" << endl;
    cout << "   --sublead-pt  sub-lead lepton pT minimum in GeV (default: 20)" << endl;
    cout << endl;
    cout << "-----------------------------------------------------" << endl;
}

int main(int argc, char** argv)
{
    int n_events = -1;
    string sample = "";
    string input = "";
    bool dbg = false;
    float lead_pt = 25;
    float sub_lead_pt = 20;

    for(int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-n") == 0) n_events = atoi(argv[++i]);
        else if (strcmp(argv[i], "-s") == 0) sample = argv[++i];
        else if (strcmp(argv[i], "-i") == 0) input = argv[++i];
        else if (strcmp(argv[i], "-d") == 0) dbg = true;
        else if (strcmp(argv[i], "--lead-pt") == 0) lead_pt = atoi(argv[++i]); 
        else if (strcmp(argv[i], "--sublead-pt") == 0) sub_lead_pt = atoi(argv[++i]);
        else if (strcmp(argv[i], "-h") == 0) { help(); return 0; }
        else {
            cout << "Unknown input argument '" << argv[i] << "' provided!" << endl;
            help();
            return 0;
        }
    } // i

    if(input.empty()) {
        cout << "You did not provide ann input, exiting" << endl;
        help();
        return 1;
    }

    cout << "-----------------------------------------------------" << endl;
    cout << "  SusyLepTrigExample " << endl;
    cout << endl;
    cout << "  Run configuration: " << endl;
    cout << endl;
    cout << "   - input                       : " << input << endl;
    cout << "   - number of events to process : " << n_events << endl;
    cout << "   - lead lepton pT threshold    : " << lead_pt << endl;
    cout << "   - sublead lepton pT threshold : " << sub_lead_pt << endl;
    cout << "-----------------------------------------------------" << endl;

    TChain* chain = new TChain("susyNt");
    ChainHelper::addInput(chain, input, 1);
    Long64_t n_entries = chain->GetEntries();

    SusyLepTrigExample* ana = new SusyLepTrigExample();
    ana->set_chain(chain);
    ana->set_debug(dbg);
    ana->set_pt_thresholds(lead_pt, sub_lead_pt);
    ana->setAnaType(AnalysisType::Ana_Stop2L);
    ana->nttools().initTriggerTool(ChainHelper::firstFile(input, 1));
    if(n_events < 0) n_events = n_entries;
    cout << " > Total entries in chain      : " << n_entries << endl;
    cout << " > Total entries to process    : " << n_events << endl;
    if(n_events > 0) chain->Process(ana, input.c_str(), n_events);

    cout << endl;
    cout << "SusyLepTrigExample    Job done!" << endl;
    
    delete chain;

    return 0;
}
