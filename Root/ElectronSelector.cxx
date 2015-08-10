#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/SusyNt.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using Susy::ElectronSelector;
using Susy::Electron;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {
// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
ElectronSelector::ElectronSelector() :
    //////////////////////////
    // default selection
    //////////////////////////
    EL_MIN_PT_BASELINE(10.0),
    EL_MIN_PT_SIGNAL(10.0),
    EL_MAX_ETA_BASELINE(2.47),
    EL_MAX_ETA_SIGNAL(2.47),
    EL_ISO_PT_THRS(60.0),
    EL_PTCONE30_PT_CUT(0.16),
    EL_TOPOCONE30_PT_CUT(0.18),
    EL_MAX_D0SIG(5.0),
    EL_MAX_Z0_SINTHETA(0.4),
    EL_MIN_CRACK_ETA(1.37),
    EL_MAX_CRACK_ETA(1.52),
    //////////////////////////
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_vetoCrackRegion(false),
    m_doIPCut(false),
    m_eleBaseId(ElectronId::ElectronIdInvalid),
    m_eleId(ElectronId::ElectronIdInvalid),   
    m_sigIso(Isolation::IsolationInvalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_SS3L(false),
    m_verbose(false)
{
}
// -------------------------------------------------------------------------------------------- //
ElectronSelector& ElectronSelector::setAnalysis(const AnalysisType &a)
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
        m_2lep = true;

        // baseline
        m_eleBaseId = ElectronId::LooseLH;
        // signal
        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_doIPCut           = true;

        break;
    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_3Lep) : {
        m_3lep = true;

        //baseline
        m_eleBaseId = ElectronId::LooseLH;
        // signal
        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_doIPCut           = true;

        break;
    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2LepWH) : {
        m_2lepWH = true;

        // baseline
        m_eleBaseId = ElectronId::LooseLH;
        // signal
        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_doIPCut           = true;

        // cuts
        EL_PTCONE30_PT_CUT              = 0.07;
        EL_TOPOCONE30_PT_CUT            = 0.13;
        EL_MAX_D0SIG                    = 3.0; 

        break;
    }
    //////////////////////////////////////
    // SS3L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_SS3L) : { 
        m_SS3L = true;

        //basline
        m_eleBaseId              = ElectronId::LooseLH;
        m_vetoCrackRegion        = true;
        // signal
        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;
        
        m_doIPCut = true;

        // cuts
        EL_MAX_ETA_SIGNAL               = 2.0;
        EL_MAX_Z0_SINTHETA              = 0.5;

        break;
    } 
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    case(AnalysisType::kUnknown) : {
        string error = "ElectronSelector::setAnalysis error: ";
        cout << error << "Invalid AnalysisType (" << AnalysisType2str(AnalysisType::kUnknown) << ")" << endl;
        cout << error << "Check that setAnalysisType is called properly for ElectronSelector" << endl;
        cout << error << "for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    } // end switch

    // check that the ele Id's have been set
    if(m_eleId == ElectronId::ElectronIdInvalid || m_eleBaseId == ElectronId::ElectronIdInvalid) {
        string error = "ElectronSelector::setAnalysis error: "; 
        cout << error << "Baseline and/or signal electron ID requirements are not set." << endl;
        cout << error << "Baseline ID = " << ElectronId2str(m_eleBaseId) 
                                                       << " Signal ID = " << ElectronId2str(m_eleId) << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    // check that the ele signal isolation has been set
    if(m_sigIso == Isolation::IsolationInvalid) {
        string error = "ElectronSelector::setAnalysis error: ";
        cout << error << "Signal electron isolation requirement is not set." << endl;
        cout << error << "Electron isolation = " << Isolation2str(m_sigIso) << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    if (m_verbose)
        cout << "ElectronSelector    Configured electron selection for AnalysisType " << AnalysisType2str(m_analysis) << endl;
    
    return *this;
}
// ---------------------------------------------------------
ElectronSelector& ElectronSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
// ---------------------------------------------------------
bool ElectronSelector::isBaselineElectron(const Electron* ele)
{
    /////////////////////////////
    // Electron ID
    /////////////////////////////
    if(!elecPassID(ele, false)) return false;

    //////////////////////////
    // eta/pT
    //////////////////////////
    if(fabs(ele->clusEta) > EL_MAX_ETA_BASELINE) return false;
    if(ele->Pt() < EL_MIN_PT_BASELINE) return false;
       
    //////////////////////////
    // veto detector
    //////////////////////////
    if(m_vetoCrackRegion){
        if(fabs(ele->clusEta)> EL_MIN_CRACK_ETA &&
           fabs(ele->clusEta)< EL_MAX_CRACK_ETA) return false;
    }
    
    if (m_doIPCut && m_SS3L) {
        if(fabs(ele->d0sigBSCorr)  >= EL_MAX_D0SIG)   return false;
    }
    
    return true;

}
// ---------------------------------------------------------
bool ElectronSelector::isSignalElectron(const Electron* ele)
{

    /////////////////////////////
    // Electron ID
    /////////////////////////////
    if(!elecPassID(ele, true)) return false;
     
    /////////////////////////////
    // Eta
    /////////////////////////////
    if(fabs(ele->Eta()) > EL_MAX_ETA_SIGNAL ) return false;
    

    /////////////////////////////
    // Impact parameter
    /////////////////////////////
    if (m_doIPCut) {
        if(fabs(ele->d0sigBSCorr)      >= EL_MAX_D0SIG)   return false;
        if(fabs(ele->z0SinTheta()) >= EL_MAX_Z0_SINTHETA) return false;
    }
    
    /////////////////////////////
    // isolation now uses
    // isolation tool
    /////////////////////////////
    if(!elecPassIsolation(ele)) return false;

    /////////////////////////////
    // ele pt
    /////////////////////////////
    if(ele->Pt() < EL_MIN_PT_SIGNAL) return false;
    
    return true;
}
/* --------------------------------------------------------------------------------------------- */ 
bool ElectronSelector::isSemiSignalElectron(const Electron* ele)
{
    /////////////////////////////
    // Electron ID
    /////////////////////////////
    if(!elecPassID(ele, true)) return false;

    /////////////////////////////
    // Impact parameter
    /////////////////////////////
    if(m_doIPCut) {
        if(fabs(ele->d0sigBSCorr) >= EL_MAX_D0SIG) return false;
        if(fabs(ele->z0SinTheta()) >= EL_MAX_Z0_SINTHETA) return false;
    }
    return true;
}
/* --------------------------------------------------------------------------------------------- */ 
bool ElectronSelector::elecPassID(const Electron* electron, bool signalQuality)
{
    ElectronId id = signalQuality ? m_eleId : m_eleBaseId;

    if     (id == ElectronId::MediumLH)        return electron->mediumLH;
    else if(id == ElectronId::LooseLH)         return electron->looseLH;
    else if(id == ElectronId::TightLH)         return electron->tightLH;
    else if(id == ElectronId::MediumLH_nod0)   return electron->mediumLH_nod0;
    else if(id == ElectronId::TightLH_nod0)    return electron->tightLH_nod0;
    else {
        cout << "ElectronSelector::elecPassID() error: (signal) ele ID requirement not set for analysis!" << endl;
        cout << "        Will set to TightLH." << endl;
        return electron->tightLH;
    }
    
    return false;

}
/* --------------------------------------------------------------------------------------------- */ 
bool ElectronSelector::elecPassIsolation(const Electron* ele)
{
    if     (m_sigIso == Isolation::GradientLoose) return ele->isoGradientLoose;
    else if(m_sigIso == Isolation::Gradient)    return ele->isoGradient;
    else if(m_sigIso == Isolation::LooseTrackOnly) return ele->isoLooseTrackOnly;
    else if(m_sigIso == Isolation::Loose) return ele->isoLoose;
    else if(m_sigIso == Isolation::Tight) return ele->isoTight; 
    else {
        cout << "ElectronSelector::elecPassIsolation error: isolation requirement for electrons not set to signal-level isolation (Loose or Tight)" << endl;
        cout << "ElectronSelector::elecPassIsolation error: >>> Exiting." << endl;
        exit(1);
    } 
}
/* --------------------------------------------------------------------------------------------- */
//float ElectronSelector::elEtTopoConeCorr(const Electron* ele,
//                                     const ElectronVector& baseElectrons,
//                                     const MuonVector& baseMuons,
//                                     unsigned int nVtx, bool isMC, bool removeLeps)
//{
//    float slope = isMC ? EL_TOPOCONE30_SLOPE_MC_CUT : EL_TOPOCONE30_SLOPE_DATA_CUT;
//    float etcone = ele->etconetopo30 - slope*nVtx;
//    if(removeLeps) {
//        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++) {
//            const Electron* e2 = baseElectrons[iEl];
//            if(ele==e2) continue;
//            if(!isSemiSignalElectron(e2)) continue;
//            float dR = ele->DeltaR(*e2);
//            if(dR < 0.28) etcone -= e2->clusE / cosh(e2->clusEta);
//        }
//    }
//    return etcone;
//}
//float ElectronSelector::elPtConeCorr(const Electron* ele,
//                                     const ElectronVector& baseElectrons,
//                                     const MuonVector& baseMuons,
//                                     unsigned int nVtx, bool isMC, bool removeLeps)
//{
//    float ptcone = ele->ptcone30;
//    if(removeLeps) {
//        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++){
//            const Electron* e = baseElectrons[iEl];
//            if(ele == e) continue;
//            if(!isSemiSignalElectron(e)) continue;
//            float dR = ele->DeltaR(*e);
//            if(dR < 0.3) ptcone -= e->trackPt;
//        }
//        for(unsigned int iMu = 0; iMu < baseMuons.size(); iMu++){
//            const Muon* mu = baseMuons[iMu];
//            if(!MuonSelector()
//                            .setSystematic(m_systematic)
//                            .setAnalysis(m_analysis)
//                            .isSemiSignalMuon(mu)) continue;
//            float dR = ele->DeltaR(*mu);
//            if(dR < 0.3) ptcone -= mu->idTrackPt;
//        }
//    }
//    return ptcone;
//}

}; // namespace Susy
