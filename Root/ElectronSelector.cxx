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

namespace Susy {

void ElectronSelector::buildRequirements(const AnalysisType &a)
{
    switch(a) {
    //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2Lep) : { 
        m_2lep = true;

        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtconeCut = true;
        m_doMuEtconeCut = false;
    
        // cuts
        EL_MIN_PT                       = 10.0;
        EL_MAX_ETA                      = 2.47;
        EL_ISO_PT_THRS                  = 60.0;
        EL_PTCONE30_PT_CUT              = 0.16;
        EL_TOPOCONE30_SLOPE_DATA_CUT    = 0.02015;
        EL_TOPOCONE30_SLOPE_MC_CUT      = 0.01794;
        EL_TOPOCONE30_PT_CUT            = 0.18;
        EL_MAX_D0SIG_CUT                = 5.0; 
        EL_MAX_Z0_SINTHETA              = 0.4;

        break;
    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_3Lep) : {
        m_3lep = true;

        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtconeCut = true;
        m_doMuEtconeCut = false;

        // cuts
        EL_MIN_PT                       = 10.0;
        EL_MAX_ETA                      = 2.47;
        EL_ISO_PT_THRS                  = 60.0;
        EL_PTCONE30_PT_CUT              = 0.16;
        EL_TOPOCONE30_SLOPE_DATA_CUT    = 0.02015;
        EL_TOPOCONE30_SLOPE_MC_CUT      = 0.01794;
        EL_TOPOCONE30_PT_CUT            = 0.18;
        EL_MAX_D0SIG_CUT                = 5.0; 
        EL_MAX_Z0_SINTHETA              = 0.4;

        break;
    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2LepWH) : {
        m_2lepWH = true;

        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtconeCut = true;
        m_doMuEtconeCut = false;

        // cuts
        EL_MIN_PT                       = 10.0;
        EL_MAX_ETA                      = 2.47;
        EL_ISO_PT_THRS                  = 60.0;
        EL_PTCONE30_PT_CUT              = 0.07;
        EL_TOPOCONE30_SLOPE_DATA_CUT    = 0.02015;
        EL_TOPOCONE30_SLOPE_MC_CUT      = 0.01794;
        EL_TOPOCONE30_PT_CUT            = 0.13;
        EL_MAX_D0SIG_CUT                = 3.0; 
        EL_MAX_Z0_SINTHETA              = 0.4;


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

        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        //m_doPtconeCut = true;
        //m_doElEtconeCut = true;
        //m_doMuEtconeCut = false;
    
        // cuts
        EL_MAX_BASELINE_ETA             = 2.47;
        EL_MIN_CRACK_ETA                = 1.37;
        EL_MAX_CRACK_ETA                = 1.52;
        EL_MIN_PT                       = 10.0;
        EL_MAX_ETA                      = 2.0;
        //EL_ISO_PT_THRS                  = 60.0;
        //EL_PTCONE30_PT_CUT              = 0.16;
        //EL_TOPOCONE30_SLOPE_DATA_CUT    = 0.02015;
        //EL_TOPOCONE30_SLOPE_MC_CUT      = 0.01794;
        //EL_TOPOCONE30_PT_CUT            = 0.18;
        EL_MAX_D0SIG_CUT                = 5.0; 
        EL_MAX_Z0_SINTHETA              = 0.5;

        break;
    } 
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    case(AnalysisType::kUnknown) : {
        cout << "ElectronSelector::buildRequirements() error: invalid analysis"
             <<" '"<<std::underlying_type<AnalysisType>::type(a)<< "'" << endl;
        cout << "               will apply default electron selection (Ana_2Lep)." << endl; 
        m_analysis = AnalysisType::Ana_2Lep;
        m_2lep = true;

        m_eleId  = ElectronId::TightLH;
        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtconeCut = true;
        m_doMuEtconeCut = false;
    
        // cuts
        EL_MIN_PT                       = 10.0;
        EL_MAX_ETA                      = 2.47;
        EL_ISO_PT_THRS                  = 60.0;
        EL_PTCONE30_PT_CUT              = 0.16;
        EL_TOPOCONE30_SLOPE_DATA_CUT    = 0.02015;
        EL_TOPOCONE30_SLOPE_MC_CUT      = 0.01794;
        EL_TOPOCONE30_PT_CUT            = 0.18;
        EL_MAX_D0SIG_CUT                = 5.0; 
        EL_MAX_Z0_SINTHETA              = 0.4;

        break;
    }

    } // end switch
}
// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
ElectronSelector::ElectronSelector(): 
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_vetoCrackRegion(false),
    m_doIPCut(false),
    m_doPtconeCut(false),
    m_doElEtconeCut(false),
    m_doMuEtconeCut(false),
    m_eleBaseId(ElectronId::LooseLH),
    m_eleId(ElectronId::TightLH),   
    m_sigIso(Isolation::IsolationInvalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_SS3L(false),
    m_verbose(false)
{
}
// ---------------------------------------------------------
ElectronSelector& ElectronSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
// ---------------------------------------------------------
ElectronSelector& ElectronSelector::setAnalysis(const AnalysisType &a)
{
    m_analysis = a;
    buildRequirements(a);
    return *this;
}
// ---------------------------------------------------------
bool ElectronSelector::isBaselineElectron(const Electron* ele)
{
    /////////////////////////////
    // Electron ID
    /////////////////////////////
    if(!elecPassID(ele, false)) return false;

    if(fabs(ele->clusEta) > EL_MAX_BASELINE_ETA) return false;
    if(ele->Pt() < EL_MIN_PT)                    return false;
       
    if(m_vetoCrackRegion){
        if(fabs(ele->clusEta)> EL_MIN_CRACK_ETA &&
           fabs(ele->clusEta)< EL_MAX_CRACK_ETA) return false;
    }
    
    if (m_doIPCut) {
        if(fabs(ele->d0sigBSCorr)  >= EL_MAX_D0SIG_CUT)   return false;
    }
    
    return true;

}
// ---------------------------------------------------------
bool ElectronSelector::isSignalElectron(const Electron* ele,
                                        const ElectronVector& baseElectrons,
                                        const MuonVector& baseMuons,
                                        const unsigned int nVtx, bool isMC,
                                        bool removeLepsFromIso)
{

    /////////////////////////////
    // Electron ID
    /////////////////////////////
    if(!elecPassID(ele, true)) return false;
     
    /////////////////////////////
    // Eta
    /////////////////////////////
    if(fabs(ele->Eta()) > EL_MAX_ETA ) return false;
    

    /////////////////////////////
    // Impact parameter
    /////////////////////////////
    if (m_doIPCut) {
        if(fabs(ele->d0sigBSCorr)      >= EL_MAX_D0SIG_CUT)   return false;
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
    if(ele->Pt() < 10) return false;

    
    
    // old:
    ///////////////////////////////
    //// ptcone isolation
    ///////////////////////////////
    //if(m_doPtconeCut) {
    //    float ptcone30 = elPtConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso); 
    //    if(m_2lepWH){
    //        if(ptcone30/std::min(pt, EL_ISO_PT_THRS) >= EL_PTCONE30_PT_CUT) return false;
    //    }
    //    else 
    //        if (ptcone30/pt >= EL_PTCONE30_PT_CUT) return false;
    //}
    ///////////////////////////////
    //// topo etcone isolation
    ///////////////////////////////
    //if(m_doElEtconeCut) { // true by default
    //    float etcone = elEtTopoConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
    //    if(m_2lepWH){
    //        if(etcone/std::min(pt,EL_ISO_PT_THRS) >= EL_TOPOCONE30_PT_CUT) return false;
    //    }
    //    else
    //        if(etcone/pt >= EL_TOPOCONE30_PT_CUT) return false;
    //}
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
        if(fabs(ele->d0sigBSCorr) >= EL_MAX_D0SIG_CUT) return false;
        if(fabs(ele->z0SinTheta()) >= EL_MAX_Z0_SINTHETA) return false;
    }
    return true;
}
/* --------------------------------------------------------------------------------------------- */ 
// Isolation
/* --------------------------------------------------------------------------------------------- */
float ElectronSelector::elPtConeCorr(const Electron* ele,
                                     const ElectronVector& baseElectrons,
                                     const MuonVector& baseMuons,
                                     unsigned int nVtx, bool isMC, bool removeLeps)
{
    float ptcone = ele->ptcone30;
    if(removeLeps) {
        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++){
            const Electron* e = baseElectrons[iEl];
            if(ele == e) continue;
            if(!isSemiSignalElectron(e)) continue;
            float dR = ele->DeltaR(*e);
            if(dR < 0.3) ptcone -= e->trackPt;
        }
        for(unsigned int iMu = 0; iMu < baseMuons.size(); iMu++){
            const Muon* mu = baseMuons[iMu];
            if(!MuonSelector()
                            .setSystematic(m_systematic)
                            .setAnalysis(m_analysis)
                            .isSemiSignalMuon(mu)) continue;
            float dR = ele->DeltaR(*mu);
            if(dR < 0.3) ptcone -= mu->idTrackPt;
        }
    }
    return ptcone;
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
float ElectronSelector::elEtTopoConeCorr(const Electron* ele,
                                     const ElectronVector& baseElectrons,
                                     const MuonVector& baseMuons,
                                     unsigned int nVtx, bool isMC, bool removeLeps)
{
    float slope = isMC ? EL_TOPOCONE30_SLOPE_MC_CUT : EL_TOPOCONE30_SLOPE_DATA_CUT;
    float etcone = ele->etconetopo30 - slope*nVtx;
    if(removeLeps) {
        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e2 = baseElectrons[iEl];
            if(ele==e2) continue;
            if(!isSemiSignalElectron(e2)) continue;
            float dR = ele->DeltaR(*e2);
            if(dR < 0.28) etcone -= e2->clusE / cosh(e2->clusEta);
        }
    }
    return etcone;
}

}; // namespace Susy
