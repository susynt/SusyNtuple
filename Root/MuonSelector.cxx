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
    m_muBaseId(MuonId::MuonIdInvalid),
    m_muId(MuonId::MuonIdInvalid),
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

    //////////////////////////////////////
    // Set analysis-specific cuts
    //////////////////////////////////////

    //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    if( a == AnalysisType::Ana_2Lep ) {
        m_2lep  = true;

        //baseline
        m_muBaseId = MuonId::Medium;

        //signal
        m_muId = MuonId::Medium;
        m_sigIso = Isolation::GradientLoose;
        m_doIPCut           = true;
    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_3Lep ) {
        m_3lep  = true;

        //baseline
        m_muBaseId = MuonId::Medium;
        //signal
        m_muId = MuonId::Medium;
        m_sigIso = Isolation::Tight;
        m_doIPCut           = true;
    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_2LepWH ) {
        m_2lepWH = true;

        //baseline
        m_muBaseId = MuonId::Medium;

        //signal
        m_muId = MuonId::Medium;
        m_sigIso = Isolation::Tight;
        m_doIPCut           = true;

        // muons
        MU_PTCONE30_PT_CUT = 0.06;
        MU_ETCONE30_PT_CUT = 0.14;
    }
    //////////////////////////////////////
    // SS3L-ANALYSIS
    // values from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_SS3L ) {
        m_SS3L  = true;
        //baseline
        m_muBaseId           = MuonId::Medium;
        MU_MIN_PT_BASELINE   = 10.0;
        MU_MAX_ETA_BASELINE  = 2.5;
        //signal
        m_muId               = MuonId::Medium;
        m_sigIso             = Isolation::GradientLoose;
        m_doIPCut            = true;
        MU_MIN_PT_SIGNAL     = 10.0
        MU_MAX_ETA_SIGNAL    = 2.5;
        MU_MAX_D0SIG         = 3.0;
        MU_MAX_Z0_SINTHETA   = 0.5;

    //////////////////////////////////
    } 
    //////////////////////////////////////
    // Stop2L-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_Stop2L ) {
        m_stop2l = true;

        //baseline
        m_muBaseId = MuonId::Loose;

        //signal
        m_muId = MuonId::Loose;
        m_sigIso = Isolation::GradientLoose;
        m_doIPCut = true;

        //cuts
        MU_MAX_Z0_SINTHETA = 0.5;
    }

    //////////////////////////////////////
    // Didn't set AnalysisType
    //////////////////////////////////////
    else if( a == AnalysisType::kUnknown ) {
        string error  = "MuonSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for MuonSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for MuonSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    else {
        string error = "MuonSelector::setAnalysis error: ";
        cout << error << "MuonSelector is not configured for the AnalysisType (" << AnalysisType2str(a) << ") provided in" << endl;
        cout << error << "'setAnalysis'! Be sure that your AnalysisType is provided for in SusyNtuple/AnalysisType.h" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    m_analysis = a;

    // check that the muon ID qualities are set
    if(m_muBaseId == MuonId::MuonIdInvalid || m_muId == MuonId::MuonIdInvalid) {
        string error = "MuonSelector::setAnalysis error: ";
        cout << error << "Muon ID quality is not set." << endl;
        cout << error << "Muon baseline ID quality = " << MuonId2str(m_muBaseId) << endl;
        cout << error << "Muon signal ID quality   = " << MuonId2str(m_muId) << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    
    // check the muon signal isolation has been set
    if(m_sigIso == Isolation::IsolationInvalid) {
        string error = "MuonSelector::setAnalysis error: ";
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
    MuonSelector::check();

   // if(!mu->medium)                    return false;
    if(!muPassQuality(mu, false))      return false;
    if(mu->Pt() < MU_MIN_PT_BASELINE)  return false;
    if(fabs(mu->Eta()) > MU_MAX_ETA_BASELINE)   return false;

    return true;
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isSignalMuon(const Muon* mu)
{
    MuonSelector::check();

    if(!muPassQuality(mu, true)) return false;

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
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::muPassQuality(const Muon* mu, bool signalQuality)
{
    MuonId id = signalQuality ? m_muId : m_muBaseId;

    if     (id == MuonId::VeryLoose)    return mu->veryLoose;
    else if(id == MuonId::Loose)        return mu->loose;
    else if(id == MuonId::Medium)       return mu->medium;
    else if(id == MuonId::Tight)        return mu->tight;
    else {
        cout << "MuonSelector::muPassQuality error: MuonId requirement for muons not set for " << (signalQuality ? "signal" : "baseline") << "-level muons." << endl;
        cout << "MuonSelector::muPassQuality error: >>> Exiting." << endl;
        exit(1);
    }

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
float MuonSelector::effSF(const Muon& mu)
{
    MuonSelector::check();
    // return the EffSF for the corresponding (signal) muon quality
    return mu.muoEffSF[m_muId];
}
// -------------------------------------------------------------------------------------------- //
float MuonSelector::errEffSF(const Muon& mu, const SusyNtSys sys)
{
    MuonSelector::check();
    // return the error on the muon SF associated with systematc sys
    float err = 0.0;
    if(sys == NtSys::MUONSFSTAT_UP) {
        err = mu.errEffSF_stat_up[m_muId];
    }
    else if(sys == NtSys::MUONSFSTAT_DN) {
        err = mu.errEffSF_stat_dn[m_muId];
    }
    else if(sys == NtSys::MUONSFSYS_UP) {
        err = mu.errEffSF_syst_up[m_muId];
    }
    else if(sys == NtSys::MUONSFSYS_DN) {
        err = mu.errEffSF_syst_dn[m_muId];
    }
    return err;
}
// -------------------------------------------------------------------------------------------- //
void MuonSelector::check()
{
    if(m_analysis == AnalysisType::kUnknown) {
        string error  = "MuonSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for MuonSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for MuonSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    return;
}
}; // namespace Susy
