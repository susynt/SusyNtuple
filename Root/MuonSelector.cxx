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

namespace Susy {

void MuonSelector::buildRequirements(const AnalysisType& a)
{
    switch(a) {
    //////////////////////////////////////
    // 2L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2Lep) : { 
        m_2lep  = true;

        m_sigIso = Isolation::GradientLoose;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtConeCut = true;
        m_doMuEtconeCut = false;        

        // muons
        MU_MIN_PT                       = 10.0;
        MU_MAX_ETA                      = 2.4;
        MU_ISO_PT_THRS                  = 60.0;
        MU_PTCONE30_SLOPE_DATA          = 0.01098;
        MU_PTCONE30_SLOPE_MC            = 0.00627;
        MU_PTCONE30_PT_CUT              = 0.12;
        //MU_PTCONE30ELSTYLE_PT_CUT       = 0.12;
        MU_PTCONE30_PT_CUT              = 0.12;
        MU_ETCONE30_K1_DATA             = 0.0648;
        MU_ETCONE30_K2_DATA             = 0.00098;
        MU_ETCONE30_K1_MC               = 0.0692;
        MU_ETCONE30_K2_MC               = 0.00076;
        MU_ETCONE30_PT_CUT              = 0.12;
        MU_MAX_D0SIG_CUT                = 3.0;
        MU_MAX_Z0_SINTHETA              = 0.4;

        break;
    } 
    //////////////////////////////////////
    // 3L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_3Lep) : {
        m_3lep  = true;

        m_sigIso = Isolation::Tight;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtConeCut = true;
        m_doMuEtconeCut = false;        
    
        // muons
        MU_MIN_PT                       = 10.0;
        MU_MAX_ETA                      = 2.4;
        MU_ISO_PT_THRS                  = 60.0;
        MU_PTCONE30_SLOPE_DATA          = 0.01098;
        MU_PTCONE30_SLOPE_MC            = 0.00627;
        MU_PTCONE30_PT_CUT              = 0.12;
        //MU_PTCONE30ELSTYLE_PT_CUT       = 0.12;
        MU_PTCONE30_PT_CUT              = 0.12;
        MU_ETCONE30_K1_DATA             = 0.0648;
        MU_ETCONE30_K2_DATA             = 0.00098;
        MU_ETCONE30_K1_MC               = 0.0692;
        MU_ETCONE30_K2_MC               = 0.00076;
        MU_ETCONE30_PT_CUT              = 0.12;
        MU_MAX_D0SIG_CUT                = 3.0;
        MU_MAX_Z0_SINTHETA              = 0.4;

        break;
    }
    //////////////////////////////////////
    // WH-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_2LepWH) : {
        m_2lepWH = true;

        m_sigIso = Isolation::Tight;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtConeCut = true;
        m_doMuEtconeCut = false;        

        // muons
        MU_MIN_PT                       = 10.0;
        MU_MAX_ETA                      = 2.4;
        MU_ISO_PT_THRS                  = 60.0;
        MU_PTCONE30_SLOPE_DATA          = 0.01098;
        MU_PTCONE30_SLOPE_MC            = 0.00627;
        MU_PTCONE30_PT_CUT              = 0.12;
        //MU_PTCONE30ELSTYLE_PT_CUT       = 0.06;
        MU_PTCONE30_PT_CUT              = 0.06;
        MU_ETCONE30_K1_DATA             = 0.0648;
        MU_ETCONE30_K2_DATA             = 0.00098;
        MU_ETCONE30_K1_MC               = 0.0692;
        MU_ETCONE30_K2_MC               = 0.00076;
        MU_ETCONE30_PT_CUT              = 0.14;
        MU_MAX_D0SIG_CUT                = 3.0;
        MU_MAX_Z0_SINTHETA              = 0.4;

        break;
    }
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    case(AnalysisType::kUnknown) : {
        cout << "MuonSelector::buildRequirements() error: invalid analysis"
             <<" '"<<std::underlying_type<AnalysisType>::type(a)<< "'" << endl;
        cout << "              will apply default muon selection (Ana_2Lep)." << endl;
        m_analysis = AnalysisType::Ana_2Lep;
        m_2lep = true;
    
        m_sigIso = Isolation::Tight;

        m_removeLepsFromIso = false;
        m_doIPCut = true;
        m_doPtconeCut = true;
        m_doElEtConeCut = true;
        m_doMuEtconeCut = false;        

        // muons
        MU_MIN_PT                       = 10.0;
        MU_MAX_ETA                      = 2.4;
        MU_ISO_PT_THRS                  = 60.0;
        MU_PTCONE30_SLOPE_DATA          = 0.01098;
        MU_PTCONE30_SLOPE_MC            = 0.00627;
        MU_PTCONE30_PT_CUT              = 0.12;
        //MU_PTCONE30ELSTYLE_PT_CUT       = 0.12;
        MU_PTCONE30_PT_CUT              = 0.12;
        MU_ETCONE30_K1_DATA             = 0.0648;
        MU_ETCONE30_K2_DATA             = 0.00098;
        MU_ETCONE30_K1_MC               = 0.0692;
        MU_ETCONE30_K2_MC               = 0.00076;
        MU_ETCONE30_PT_CUT              = 0.12;
        MU_MAX_D0SIG_CUT                = 3.0;
        MU_MAX_Z0_SINTHETA              = 0.4;

        break;

    }

    } // end switch
}

// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
MuonSelector::MuonSelector() :
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_doIPCut(true),
    m_doPtconeCut(true),
    m_doElEtConeCut(true),
    m_doMuEtconeCut(false),
    m_sigIso(Isolation::IsolationInvalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_verbose(false)
{
}
// -------------------------------------------------------------------------------------------- //
MuonSelector& MuonSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
// -------------------------------------------------------------------------------------------- //
MuonSelector& MuonSelector::setAnalysis(const AnalysisType &a)
{
    m_analysis = a;
    buildRequirements(a);
    return *this;
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isSignalMuon(const Muon* mu,
                                const ElectronVector& baseElectrons,
                                const MuonVector& baseMuons,
                                const unsigned int nVtx, bool isMC,
                                bool removeLepsFromIso)
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
        if(fabs(mu->d0Sig() >= MU_MAX_D0SIG_CUT))        return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }

    //////////////////////////////
    // mu pt
    //////////////////////////////
    if(mu->Pt() < 10) return false;
    


//    old:
//    //////////////////////////////
//    // Impact parameter
//    //////////////////////////////
//    if(m_doIPCut) {
//        // all ana using unbiased IP
//        if(fabs(mu->d0Sig()) >= MU_MAX_D0SIG_CUT) return false;
//        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
//    }
//    //////////////////////////////
//    // ptcone isolation cut 
//    // with pileup correction
//    //////////////////////////////
//    if(m_doPtconeCut) { // true by default
//        if(!muPassPtIso(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso)) return false;
//    }
//    //////////////////////////////
//    // etcone isolation
//    //////////////////////////////
//    // TODO: danny -- These selections seem ~wrong --> Can you have m_doMuEtconeCut AND Ana_2LepWH??
//    if(m_doMuEtconeCut) {
//        float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
//        if(etcone30/mu->Pt() >= MU_ETCONE30_PT_CUT) return false;
//    }
//    else if (m_2lepWH) {
//        float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
//        float pt = mu->Pt();
//        if(pt==0.0 || (etcone30/std::min(pt, MU_ISO_PT_THRS) >= MU_ETCONE30_PT_CUT)) return false;
//    }

    return true;
}

// -------------------------------------------------------------------------------------------- //
bool MuonSelector::muPassIsolation(const Muon* mu)
{
    if     (m_sigIso == Isolation::GradientLoose) return mu->isoGradientLoose;
    else if(m_sigIso == Isolation::Gradient) return mu->isoGradient;
    else if(m_sigIso == Isolation::VeryLoose) return mu->isoVeryLoose;
    else if(m_sigIso == Isolation::Loose) return mu->isoLoose;
    else if(m_sigIso == Isolation::Tight) return mu->isoTight;
    else {
        cout << "MuonSelector::muPassIsolation error: isolation requirement for muons not set to signal-level isolation (Loose or Tight)" << endl;
        cout << "MuonSelector::muPassIsolation error: >>> Exiting." << endl;
        exit(1);
    }
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::muPassPtIso(const Muon* mu,
                               const ElectronVector& baseElectrons,
                               const MuonVector& baseMuons,
                               const unsigned int nVtx, bool isMC,
                               bool removeLepsFromIso)
{
    if(m_3lep) {
        float ptcone30 = muPtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
        if(ptcone30/mu->Pt() >= MU_PTCONE30_PT_CUT) return false;
    }
    else if(m_2lepWH) {
        float ptcone30 = mu->ptcone30;
        float pt = mu->Pt();
        if(ptcone30/std::min(pt, MU_ISO_PT_THRS) >= MU_PTCONE30_PT_CUT) return false;
    }
    else {
        float ptcone30 = mu->ptcone30;
        float pt = mu->Pt();
        if(ptcone30/pt >= MU_PTCONE30_PT_CUT) return false;
    }
    return true;
}
// -------------------------------------------------------------------------------------------- //
float MuonSelector::muPtConeCorr(const Muon* mu,
                                 const ElectronVector& baseElectrons,
                                 const MuonVector& baseMuons,
                                 unsigned int nVtx, bool isMC, bool removeLeps)
{
    float slope = isMC ? MU_PTCONE30_SLOPE_MC : MU_PTCONE30_SLOPE_DATA;
    float ptcone = mu->ptcone30 - slope*nVtx;
    if(removeLeps){
        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e = baseElectrons[iEl];
            if(!ElectronSelector()
                                .setSystematic(m_systematic)
                                .setAnalysis(m_analysis)
                                .isSemiSignalElectron(e)) continue;
            float dR = mu->DeltaR(*e);
            if(dR < 0.3) ptcone -= e->trackPt;
        }
        for(unsigned int iMu = 0; iMu < baseMuons.size(); iMu++) {
            const Muon* mu2 = baseMuons[iMu];
            if(mu==mu2) continue;
            if(!isSemiSignalMuon(mu2)) continue;
            float dR = mu->DeltaR(*mu2);
            if(dR < 0.3) ptcone -= mu2->idTrackPt;
        }
    }
    return ptcone;
}
// -------------------------------------------------------------------------------------------- //
bool MuonSelector::isSemiSignalMuon(const Muon* mu)
{
    /////////////////////////////
    // Impact parameter
    /////////////////////////////
    if(m_doIPCut) {
        if(fabs(mu->d0Sig()) >= MU_MAX_D0SIG_CUT) return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }
    return true;
}
// -------------------------------------------------------------------------------------------- //
float MuonSelector::muEtConeCorr(const Muon* mu,
                                 const ElectronVector& baseElectrons, 
                                 const MuonVector& baseMuons,
                                 unsigned int nVtx, bool isMC, bool removeLeps)
{
    float k1 = isMC ? MU_ETCONE30_K1_MC : MU_ETCONE30_K1_DATA;
    float k2 = isMC ? MU_ETCONE30_K2_MC : MU_ETCONE30_K2_DATA;
    float etcone = mu->etcone30 - k1*nVtx - k2*nVtx*nVtx;
    if(removeLeps){
        for(unsigned int iEl = 0; iEl < baseElectrons.size(); iEl++){
            const Electron* e = baseElectrons[iEl];
            if(!ElectronSelector()
                                .setSystematic(m_systematic)
                                .setAnalysis(m_analysis)
                                .isSemiSignalElectron(e)) continue;
            float dR = mu->DeltaR(*e);
            if(dR < 0.28) etcone -= e->clusE / cosh(e->clusEta);
        }
    }
    return etcone;
}
            
}; // namespace Susy
