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
    // 2L-ANALYSIS
    //////////////////////////////////////
    case(AnalysisType::Ana_SS3L) : { 
        m_SS3L  = true;

        m_sigIso = Isolation::GradientLoose;

        //m_removeLepsFromIso = false;
        m_doIPCut = true;
        //m_doPtconeCut = true;
        //m_doElEtConeCut = true;
        //m_doMuEtconeCut = false;        

        // muons
        MU_MIN_PT                       = 10.0;
        MU_MAX_ETA                      = 2.5;
        //MU_ISO_PT_THRS                  = 60.0;
        //MU_PTCONE30_SLOPE_DATA          = 0.01098;
        //MU_PTCONE30_SLOPE_MC            = 0.00627;
        //MU_PTCONE30_PT_CUT              = 0.12;
        //MU_PTCONE30ELSTYLE_PT_CUT       = 0.12;
        //MU_PTCONE30_PT_CUT              = 0.12;
        //MU_ETCONE30_K1_DATA             = 0.0648;
        //MU_ETCONE30_K2_DATA             = 0.00098;
        //MU_ETCONE30_K1_MC               = 0.0692;
        //MU_ETCONE30_K2_MC               = 0.00076;
        //MU_ETCONE30_PT_CUT              = 0.12;
        MU_MAX_D0SIG_CUT                = 3.0;
        MU_MAX_Z0_SINTHETA              = 0.5;

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
bool MuonSelector::isBaselineMuon(const Muon* mu)
{
    if(!mu->medium)                  return false;
    if(mu->Pt() < MU_MIN_PT)         return false;
    if(fabs(mu->Eta()) > MU_MAX_ETA) return false;

    return true;
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
        if(fabs(mu->d0sigBSCorr) >= MU_MAX_D0SIG_CUT)        return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }

    //////////////////////////////
    // mu pt
    //////////////////////////////
    if(mu->Pt() < MU_MIN_PT) return false;

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
        if(fabs(mu->d0sigBSCorr) >= MU_MAX_D0SIG_CUT) return false;
        if(fabs(mu->z0SinTheta()) >= MU_MAX_Z0_SINTHETA) return false;
    }
    return true;
}
}; // namespace Susy
