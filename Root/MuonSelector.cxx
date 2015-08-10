#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/SusyNt.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using Susy::MuonSelector;
using Susy::Muon;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {

// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
MuonSelector::MuonSelector() :
    //////////////////////////////////
    // default selection
    //////////////////////////////////
    MU_MIN_PT_BASELINE(10.0),
    MU_MIN_PT_SIGNAL(10.0),
    MU_MAX_ETA_BASELINE(2.4),
    MU_MAX_ETA_SIGNAL(2.4),
    MU_ISO_PT_THRS(60.0),
    MU_PTCONE30_SLOPE_DATA(0.01098),
    MU_PTCONE30_SLOPE_MC(0.00627),
    MU_PTCONE30_PT_CUT(0.12),
    MU_ETCONE30_K1_DATA(0.0648),
    MU_ETCONE30_K2_DATA(0.00098),
    MU_ETCONE30_K1_MC(0.0692),
    MU_ETCONE30_K2_MC(0.00076),
    MU_ETCONE30_PT_CUT(0.12),
    MU_MAX_D0SIG(3.0),
    MU_MAX_Z0_SINTHETA(0.4), 
    //////////////////////////////////
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_doIPCut(true),
    m_sigIso(Isolation::IsolationInvalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_verbose(false)
{
}
// -------------------------------------------------------------------------------------------- //
MuonSelector& MuonSelector::setAnalysis(const AnalysisType& a)
{
    m_analysis = a;

    //////////////////////////////////////
    // Set analysis-specific cuts
    //////////////////////////////////////
    switch(a) {
    //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2Lep) : { 
        m_2lep  = true;

        // signal
        m_sigIso = Isolation::GradientLoose;

        m_doIPCut           = true;

        break;
    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_3Lep) : {
        m_3lep  = true;

        // signal
        m_sigIso = Isolation::Tight;

        m_doIPCut           = true;

        break;
    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2LepWH) : {
        m_2lepWH = true;

        // signal
        m_sigIso = Isolation::Tight;

        m_doIPCut           = true;

        // muons
        MU_PTCONE30_PT_CUT = 0.06;
        MU_ETCONE30_PT_CUT = 0.14;

        break;
    }
   //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_SS3L) : { 
        m_SS3L  = true;

        m_sigIso = Isolation::GradientLoose;

        m_doIPCut = true;

        // muons
        MU_MAX_ETA_BASELINE  = 2.5;
        MU_MAX_ETA_SIGNAL    = 2.5;
        MU_MAX_Z0_SINTHETA   = 0.5;

        break;
    } 
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    case(AnalysisType::kUnknown) : {
        string error = "MuonSelector::buildRequirements error: ";
        cout << error << "Invalid AnalysisType (" << AnalysisType2str(AnalysisType::kUnknown) << ")" << endl;
        cout << error << "Check that setAnalysisType is called properly for MuonSelector" << endl;
        cout << error << "for your analysis." <<endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    } // end switch

    // check the muon signal isolation has been set
    if(m_sigIso == Isolation::IsolationInvalid) {
        string error = "MuonSelector::buildRequirements error: ";
        cout << error << "Signal muon isolation requirement is not set." << endl;
        cout << error << "Muon isolation = " << Isolation2str(m_sigIso) << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    if(m_verbose)
        cout << "MuonSelector    Configured muon selection for AnalysisType " << AnalysisType2str(m_analysis) << endl;

    return *this;
}

// -------------------------------------------------------------------------------------------- //
MuonSelector& MuonSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isBaselineMuon(const Muon* mu)
{
    if(!mu->medium)                    return false;
    if(mu->Pt() < MU_MIN_PT_BASELINE)  return false;
    if(fabs(mu->Eta()) > MU_MAX_ETA_BASELINE)   return false;

    return true;
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isSignalMuon(const Muon* mu)
{

    //////////////////////////////
    // Isolation
    // (IsolationSelectionTool)
    //////////////////////////////
    if(!muPassIsolation(mu)) return false;

    //////////////////////////////
    // Impact parameter
    //////////////////////////////
    if (m_doIPCut) {
        if(fabs(mu->d0sigBSCorr) >= MU_MAX_D0SIG)        return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }

    //////////////////////////////
    // mu pt
    //////////////////////////////
    if(mu->Pt() < MU_MIN_PT_SIGNAL) return false;

    return true;
}

// -------------------------------------------------------------------------------------------- //
bool MuonSelector::muPassIsolation(const Muon* mu)
{
    if     (m_sigIso == Isolation::GradientLoose) return mu->isoGradientLoose;
    else if(m_sigIso == Isolation::Gradient) return mu->isoGradient;
    else if(m_sigIso == Isolation::LooseTrackOnly) return mu->isoLooseTrackOnly;
    else if(m_sigIso == Isolation::Loose) return mu->isoLoose;
    else if(m_sigIso == Isolation::Tight) return mu->isoTight;
    else {
        cout << "MuonSelector::muPassIsolation error: isolation requirement for muons not set to signal-level isolation (Loose or Tight)" << endl;
        cout << "MuonSelector::muPassIsolation error: >>> Exiting." << endl;
        exit(1);
    }
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isSemiSignalMuon(const Muon* mu)
{
    /////////////////////////////
    // Impact parameter
    /////////////////////////////
    if(m_doIPCut) {
        if(fabs(mu->d0sigBSCorr) >= MU_MAX_D0SIG) return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }
    return true;
}
}; // namespace Susy
