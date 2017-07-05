#include "SKELETON_LOOPER_CLASS/SKELETON_LOOPER_CLASS.h"

// SusyNtuple
#include "SusyNtuple/KinematicTools.h"
#include "SusyNtuple/SusyDefs.h"
using namespace Susy; // everything in SusyNtuple is in this namespace

//ROOT

// std/stl
#include <iomanip> // setw
#include <iostream>
#include <string>
#include <sstream> // stringstream, ostringstream
using namespace std;

//////////////////////////////////////////////////////////////////////////////
SKELETON_LOOPER_CLASS::SKELETON_LOOPER_CLASS() :
    m_dbg(0),
    m_input_chain(nullptr),
    m_mc_weight(1.0)
{
}
//////////////////////////////////////////////////////////////////////////////
void SKELETON_LOOPER_CLASS::Begin(TTree* /*tree*/)
{
    // call base class' Begin method
    SusyNtAna::Begin(0);
    if(dbg()) cout << "SKELETON_LOOPER_CLASS::Begin" << endl;

    return;
}
//////////////////////////////////////////////////////////////////////////////
Bool_t SKELETON_LOOPER_CLASS::Process(Long64_t entry)
{

    // calling "GetEntry" loads into memory the susyNt class objects for this event
    GetEntry(entry);
    SusyNtAna::clearObjects(); // clear the previous event's objects

    // increment the chain entry (c.f. SusyNtuple/SusyNtAna.h)
    m_chainEntry++;

    // evt() provides pointer to the SusyNt::Event class object for this event
    int run_number = nt.evt()->run;
    int event_number = nt.evt()->eventNumber;

    if(dbg() || m_chainEntry%1000==0) {
        cout << "SKELETON_LOOPER_CLASS::Process    **** Processing entry " << setw(6) << m_chainEntry
                << "  run " << run_number << "  event " << event_number << " **** " << endl;
    }

    // SusyNtAna::selectObject fills the baseline and signal objects
    // for the given AnalysisType
    // m_preX    = objects before any selection (as they are in susyNt)
    // m_baseX   = objects with the Analysis' baseline selection AND overlap removal applied
    // m_signalX = objects with the Analysis' signal selection applied (and baseline AND overlap removal)
    SusyNtAna::selectObjects();

    // get the MC weight using the inherited MCWeighter object
    // (c.f. SusyNtuple/MCWeighter.h)
    if(nt.evt()->isMC) {
        float lumi = 100000; // normalize the MC to 100 fb-1
        m_mc_weight = SusyNtAna::mcWeighter().getMCWeight(nt.evt(), lumi, NtSys::NOM);
    }
    else {
        m_mc_weight = 1.; // don't re-weight data
    }

    // check that the event passes the standard ATLAS event cleaning cuts
    if(!passEventCleaning(m_preMuons, m_baseMuons, m_baseJets)) return false;


    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
bool SKELETON_LOOPER_CLASS::passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
            const JetVector& baseJets)
{
    int flags = nt.evt()->cutFlags[NtSys::NOM];

    if(!nttools().passGRL(flags))           return false;

    if(!nttools().passLarErr(flags))        return false;

    if(!nttools().passTileErr(flags))       return false;

    if(!nttools().passTTC(flags))           return false;

    if(!nttools().passSCTErr(flags))        return false;

    if(!nttools().passGoodVtx(flags))       return false;


    ///////////////////////////////////////////////////////
    // for bad muon, cosmic moun, and jet cleaning the
    // cuts depend on the baseline object defintion
    // (and in thec ase of the cosmic muon cut, it also
    // depends on the analysis' overlap removal
    // procedure) -- so we do not use the cutFlags but
    // rather use the objects that have passed the various
    // analysis selections to do the checks
    ///////////////////////////////////////////////////////
    if(!nttools().passBadMuon(preMuons))    return false;

    if(!nttools().passCosmicMuon(baseMuons)) return false;

    if(!nttools().passJetCleaning(baseJets)) return false;

    return true;
}
//////////////////////////////////////////////////////////////////////////////
void SKELETON_LOOPER_CLASS::Terminate()
{
    // close SusyNtAna and print timers
    SusyNtAna::Terminate();

    return;
}
//////////////////////////////////////////////////////////////////////////////
