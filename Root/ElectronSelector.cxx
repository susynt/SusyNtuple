#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/Electron.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath> // abs

using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {

//----------------------------------------------------------
ElectronSelector* ElectronSelector::build(const AnalysisType &a, bool verbose)
{
    ElectronSelector* s = nullptr;
    switch(a) {
    case AnalysisType::Ana_2Lep:
        s = new ElectronSelector_2Lep();
        s->setSignalId(ElectronId::TightLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_3Lep:
        s = new ElectronSelector_3Lep();
        s->setSignalId(ElectronId::TightLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_2LepWH:
        s = new ElectronSelector_2LepWH();
        s->setSignalId(ElectronId::TightLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_SS3L:
        s = new ElectronSelector_SS3L();
        s->setSignalId(ElectronId::TightLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_Stop2L:
        s = new ElectronSelector_Stop2L();
        s->setSignalId(ElectronId::MediumLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    default:
        cout<<"ElectronSelector::build(): unknown analysis type '"<<AnalysisType2str(a)<<"'"
            <<" returning vanilla ElectronSelector"<<endl;
        s = new ElectronSelector();
    }
    return s;
}
//---------------------------------------------------------
ElectronSelector::ElectronSelector() :
    m_signalId(ElectronId::ElectronIdInvalid),
    m_signalIsolation(Isolation::IsolationInvalid),
    m_verbose(false)
{
}
//---------------------------------------------------------
bool ElectronSelector::isBaseline(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->looseLLH && // good for all
                el->Pt()  > 10.0 && // was EL_MIN_PT_BASELINE, good for all
                std::abs(el->clusEta) < 2.47 ); // was EL_MAX_ETA_BASELINE, good for all
    }
    return pass;
}
//----------------------------------------------------------
bool ElectronSelector::isSignal(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->Pt() > 10.0 && // good for all
                std::abs(el->Eta()) < 2.47 && // was EL_MAX_ETA_SIGNAL
                el->tightLLH && // good for all run1 ana
                passIpCut(*el) && // 5.0 0.4 good for 2l 3l
                el->isoGradientLoose); // good for all run1
    }
    return pass;
}
//----------------------------------------------------------
float ElectronSelector::effSF(const Electron& ele)
{
    // return the EffSF for the corresponding (signal) EL ID WP
    return ele.eleEffSF[m_signalId];
}
//----------------------------------------------------------
float ElectronSelector::errEffSF(const Electron& ele, const SusyNtSys sys)
{
    // return the error on the electron SF associated with systematic sys
    float err = 0.0;
    if(sys == NtSys::EL_EFF_ID_TotalCorrUncertainty_UP) {
        err = ele.errEffSF_id_corr_up[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_ID_TotalCorrUncertainty_DN) {
        err = ele.errEffSF_id_corr_dn[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Reco_TotalCorrUncertainty_UP) {
        err = ele.errEffSF_reco_corr_up[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Reco_TotalCorrUncertainty_DN) {
        err = ele.errEffSF_reco_corr_dn[m_signalId];
    }
    return err;
}
//----------------------------------------------------------
bool ElectronSelector::passIpCut(const Electron &el)
{
    return (std::abs(el.d0sigBSCorr)  < 5.0 &&
            std::abs(el.z0SinTheta()) < 0.5 );
}
//----------------------------------------------------------
bool ElectronSelector::outsideCrackRegion(const Electron &el)
{
    return (std::abs(el.clusEta) < 1.37 ||
            std::abs(el.clusEta) > 1.52 );
}

//----------------------------------------------------------
// begin ElectronSelector_2Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin ElectronSelector_3Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin ElectronSelector_2LepWH
//----------------------------------------------------------
bool ElectronSelector_2LepWH::passIpCut(const Electron &el)
{
    return (std::abs(el.d0sigBSCorr)  < 3.0 && // tighter than default
            std::abs(el.z0SinTheta()) < 0.4 );

}

//----------------------------------------------------------
// begin ElectronSelector_SS3L
//----------------------------------------------------------
bool ElectronSelector_SS3L::isBaseline(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->looseLLH &&
                el->Pt()             > 10.0  &&
                std::abs(el->clusEta)    <  2.47 &&
                abs(el->d0sigBSCorr) <  5.0  &&
                outsideCrackRegion(*el));
    }
    return pass;
}
//----------------------------------------------------------
bool ElectronSelector_SS3L::isSignal(const Electron* el)
{
    bool pass = false;
    if(el) {
        bool isIsolated = ((el->ptvarcone20/el->Pt()  < 0.06) &&
                           (el->etconetopo20/el->Pt() < 0.06) );
        pass = (isBaseline(el) &&
                isIsolated &&
                el->tightLLH &&
                std::abs(el->trackEta)     <  2.0 &&
                std::abs(el->z0SinTheta()) <  0.5 );
    }
    return pass;
}

//----------------------------------------------------------
// begin ElectronSelector_Stop2L
//----------------------------------------------------------
bool ElectronSelector_Stop2L::passIpCut(const Electron &el)
{
    return (std::abs(el.d0sigBSCorr)  < 3.0 &&
            std::abs(el.z0SinTheta()) < 0.5 );

}
//----------------------------------------------------------
bool ElectronSelector_Stop2L::isSignal(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (isBaseline(el) &&
                el->mediumLLH &&
                el->isoGradientLoose &&
                passIpCut(*el));
    }
    return pass;
}
//----------------------------------------------------------
} // namespace Susy
