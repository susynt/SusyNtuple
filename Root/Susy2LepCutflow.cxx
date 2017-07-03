#include "SusyNtuple/Susy2LepCutflow.h"

// SusyNtuple
#include "SusyNtuple/KinematicTools.h"
using namespace Susy; // everything in SusyNtuple is in this namespace

//ROOT

// std/stl
#include <iomanip> // setw
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
Susy2LepCutflow::Susy2LepCutflow() :
    m_dbg(0),
    m_input_chain(nullptr)
{
    initialize_counters();
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::initialize_counters()
{
    n_readin = 0;
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::Begin(TTree* /*tree*/)
{
    // call base class' Begin method
    SusyNtAna::Begin(0);
    if(dbg()) cout << "Susy2LepCutflow::Begin" << endl;

    return;
}
//////////////////////////////////////////////////////////////////////////////
Bool_t Susy2LepCutflow::Process(Long64_t entry)
{

    // calling "GetEntry" loads into memory the susyNt class objects for this event
    GetEntry(entry);
    SusyNtAna::clearObjects(); // clear the previous event's objects
    // m_ET = ET_Unknown

    // increment for each processed event
    n_readin++;

    // increment the chain entry (c.f. SusyNtuple/SusyNtAna.h)
    m_chainEntry++;

    // evt() provides pointer to the SusyNt::Event class object for this event
    int run_number = nt.evt()->run;
    int event_number = nt.evt()->eventNumber;

    if(dbg() || m_chainEntry%1000==0) {
        cout << "Susy2LepCutflow::Process    **** Processing entry " << setw(6) << m_chainEntry
                << "  run " << run_number << "  event " << event_number << " **** " << endl;
    }

    // SusyNtAna::selectObject fills the baseline and signal objects
    // for the given AnalysisType
    // m_preX    = objects before any selection (as they are in susyNt)
    // m_baseX   = objects with the Analysis' baseline selection AND overlap removal applied
    // m_signalX = objects with the Analysis' signal selection applied (and baseline AND overlap removal)
    SusyNtAna::selectObjects();

    // check that the event passes the standard ATLAS event cleaning cuts
    if(!passEventCleaning(m_preMuons, m_baseMuons, m_baseJets)) return false;
    


    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
bool Susy2LepCutflow::passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
            const JetVector& baseJets)
{
    int flags = nt.evt()->cutFlags[NtSys::NOM];

    if(!nttools().passGRL(flags))           return false;
    event_cleaning_counters.grl++;



    return true;
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::Terminate()
{
    cout << "read in    : " << n_readin << endl;
    cout << "grl        : " << event_cleaning_counters.grl << endl;

    return;
}
//////////////////////////////////////////////////////////////////////////////
