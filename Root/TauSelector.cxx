#include "SusyNtuple/TauSelector.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/TauId.h"

#include <iostream>

using Susy::TauSelector;
using Susy::Tau;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {

// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
TauSelector::TauSelector() :
    //////////////////////////
    // default selection
    //////////////////////////
    TAU_MIN_PT_BASELINE(20.0),
    TAU_MIN_PT_SIGNAL(20.0),
    //////////////////////////
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_signalTauId(TauId::Invalid),
    m_tauEleBaseId(TauId::Invalid),
    m_tauJetBaseId(TauId::Invalid),
    m_tauMuoBaseId(TauId::Invalid),
    m_tauEleId(TauId::Invalid),
    m_tauJetId(TauId::Invalid),
    m_tauMuoId(TauId::Invalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_SS3L(false),
    m_stop2l(false),
    m_verbose(false)
{
}
// -------------------------------------------------------------------------------------------- //
TauSelector& TauSelector::setAnalysis(const AnalysisType &a)
{

    //////////////////////////////////////
    // Set analysis-specific cuts
    //////////////////////////////////////

    //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    if( a == AnalysisType::Ana_2Lep ) {
        m_2lep = true;

        m_signalTauId = TauId::Medium;

        m_tauEleBaseId = TauId::Loose;
        m_tauJetBaseId = TauId::Medium;
        m_tauMuoBaseId = TauId::Medium;

        m_tauEleId = TauId::Loose;
        m_tauJetId = TauId::Medium;
        m_tauMuoId = TauId::Medium;

    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_3Lep ) {
        m_3lep = true;

        m_signalTauId = TauId::Medium;

        m_tauEleBaseId = TauId::Loose;
        m_tauJetBaseId = TauId::Medium;
        m_tauMuoBaseId = TauId::Medium;

        m_tauEleId = TauId::Loose;
        m_tauJetId = TauId::Medium;
        m_tauMuoId = TauId::Medium;

    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    else if( a== AnalysisType::Ana_2LepWH ) {
        m_2lepWH = true;

        m_signalTauId = TauId::Medium;

        m_tauEleBaseId = TauId::Loose;
        m_tauJetBaseId = TauId::Medium;
        m_tauMuoBaseId = TauId::Medium;

        m_tauEleId = TauId::Loose;
        m_tauJetId = TauId::Medium;
        m_tauMuoId = TauId::Medium;

    }
    //////////////////////////////////////
    // SS3L-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_SS3L ) {
        m_SS3L = true;

        m_signalTauId = TauId::Medium;

        m_tauEleBaseId = TauId::Loose;
        m_tauJetBaseId = TauId::Medium;
        m_tauMuoBaseId = TauId::Medium;

        m_tauEleId = TauId::Loose;
        m_tauJetId = TauId::Medium;
        m_tauMuoId = TauId::Medium;

    }
    //////////////////////////////////////
    // STOP2L-ANALYSIS
    //////////////////////////////////////
    else if( a == AnalysisType::Ana_Stop2L ) {
        m_stop2l = true;

        m_signalTauId = TauId::Medium;

        m_tauEleBaseId = TauId::Loose;
        m_tauJetBaseId = TauId::Medium;
        m_tauMuoBaseId = TauId::Medium;

        m_tauEleId = TauId::Loose;
        m_tauJetId = TauId::Medium;
        m_tauMuoId = TauId::Medium;

    }
    //////////////////////////////////////
    // Didn't set AnalysisType
    //////////////////////////////////////
    else if( a == AnalysisType::kUnknown ) {
        string error = "TauSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for TauSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for TauSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    else {
        string error = "TauSelector::setAnalysis error: ";
        cout << error << "TauSelector is not configured for the AnalysisType (" << AnalysisType2str(a) << ")  provided in " << endl;
        cout << error << "'setAnalysis'! Be sure that your AnalysisType is provided for in SusyNtuple/AnalysisType.h" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    m_analysis = a;

    if(m_verbose)
        cout << "TauSelector    Configured tau selection for AnalysisType " << AnalysisType2str(m_analysis) << endl;

    return *this;
}
// ---------------------------------------------------------
TauSelector& TauSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}

// ---------------------------------------------------------
bool TauSelector::isBaselineTau(const Tau* tau)
{
    TauSelector::check();
    return TauSelector::isBaselineTau(*tau);
}
bool TauSelector::isBaselineTau(const Tau& tau)
{
    /////////////////////////
    // pT requirement
    /////////////////////////
    if(tau.Pt() < TAU_MIN_PT_BASELINE) return false;

    /////////////////////////
    // BDT requirement
    /////////////////////////
    if(!tauPassBDT(tau, false)) return false;

    return true;
}

// ---------------------------------------------------------
bool TauSelector::isSignalTau(const Tau* tau, const TauId& jetId, const TauId& eleId, const TauId& muoId)
{
    TauSelector::check();
    return TauSelector::isSignalTau(*tau, jetId, eleId, muoId);
}
bool TauSelector::isSignalTau(const Tau& tau, const TauId& jetId, const TauId& eleId, const TauId& muoId)
{
    return TauSelector::isTauBDT(tau, jetId, eleId, muoId);
}
// ---------------------------------------------------------
bool TauSelector::isTauBDT(const Tau* tau, const TauId& jetId, const TauId& eleId, const TauId& muoId)
{
    return TauSelector::isTauBDT(*tau, jetId, eleId, muoId);
}
bool TauSelector::isTauBDT(const Tau& tau, const TauId& jetId, const TauId& eleId, const TauId& muoId)
{
    
    /////////////////////////
    // Jet BDT cut
    /////////////////////////
    if(jetId == TauId::Loose){
        if(tau.jetBDTSigLoose != 1) return false;
    }
    else if(jetId == TauId::Medium){
        if(tau.jetBDTSigMedium != 1 ) return false;
    }
    else if(jetId == TauId::Tight){
        if(tau.jetBDTSigTight != 1) return false;
    }

    /////////////////////////
    // Electron BDT cut
    /////////////////////////
    if(eleId == TauId::Loose){
        if(tau.nTrack == 1 && tau.eleBDTLoose != 0) return false;
    }
    else if(eleId == TauId::Medium){
        if(tau.nTrack == 1 && tau.eleBDTMedium != 0) return false;
    }
    else if(eleId == TauId::Tight){
        if(tau.nTrack == 1 && tau.eleBDTTight != 0) return false;
    }

    /////////////////////////
    // Muon veto
    /////////////////////////
    if(muoId >= TauId::Medium) {
        if(tau.muonVeto != 0) return false;
    }

    return true;
}

//// ---------------------------------------------------------
//bool TauSelector::isSignalTau(const Tau* tau)
//{
//    TauSelector::check();
//    return TauSelector::isSignalTau(*tau);
//}
//bool TauSelector::isSignalTau(const Tau& tau)
//{
//    ////////////////////////////
//    // BDT requirement
//    ////////////////////////////
//    if(!tauPassBDT(tau, true)) return false;
//
//    return true;
//}
// ---------------------------------------------------------
bool TauSelector::tauPassBDT(const Tau& tau, bool isSignal)
{
    TauId jetId = isSignal ? m_tauJetId : m_tauJetBaseId;
    TauId eleId = isSignal ? m_tauEleId : m_tauEleBaseId;
    TauId muoId = isSignal ? m_tauMuoId : m_tauMuoBaseId; 

    /////////////////////////
    // Jet BDT cut
    /////////////////////////
    if(jetId == TauId::Loose){
        if(tau.jetBDTSigLoose != 1) return false;
    }
    else if(jetId == TauId::Medium){
        if(tau.jetBDTSigMedium != 1 ) return false;
    }
    else if(jetId == TauId::Tight){
        if(tau.jetBDTSigTight != 1) return false;
    }

    /////////////////////////
    // Electron BDT cut
    /////////////////////////
    if(eleId == TauId::Loose){
        if(tau.nTrack == 1 && tau.eleBDTLoose != 0) return false;
    }
    else if(eleId == TauId::Medium){
        if(tau.nTrack == 1 && tau.eleBDTMedium != 0) return false;
    }
    else if(eleId == TauId::Tight){
        if(tau.nTrack == 1 && tau.eleBDTTight != 0) return false;
    }

    /////////////////////////
    // Muon veto
    /////////////////////////
    if(muoId >= TauId::Medium) {
        if(tau.muonVeto != 0) return false;
    }

    return true;
}
    
/* --------------------------------------------------------------------------------------------- */
void TauSelector::check()
{
    if(m_analysis == AnalysisType::kUnknown) {
        string error = "TauSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for TauSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for TauSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    return;
}
/* --------------------------------------------------------------------------------------------- */
} // namespace Susy
