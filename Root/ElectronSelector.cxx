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
        s->setSignalId(ElectronId::MediumLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_3Lep:
        s = new ElectronSelector_3Lep();
        s->setSignalId(ElectronId::TightLLH);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_4Lep:
        s = new ElectronSelector_4Lep();
        s->setSignalId(ElectronId::MediumLLH);
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
        pass = (el->looseLLHBLayer && // good for all
                el->passOQBadClusElectron &&
                el->Pt()  > 10.0 &&
                std::abs(el->clusEta) < 2.47 ); // SUSYTools uses CaloCluster::eta() for this but CaloCluster::etaBE(2) for crack region...
    }
    return pass;
}
//----------------------------------------------------------
bool ElectronSelector::isSignal(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->Pt() > 10.0 && // good for all
                std::abs(el->Eta()) < 2.47 &&
                el->tightLLH &&
                passIpCut(*el) &&
                el->isoGradientLoose);
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
    if     (sys == NtSys::EL_EFF_ID_TOTAL_Uncorr_UP) {
        err = ele.errEffSF_id_up[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_ID_TOTAL_Uncorr_DN) {
        err = ele.errEffSF_id_dn[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Reco_TOTAL_Uncorr_UP) {
        err = ele.errEffSF_reco_up[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Reco_TOTAL_Uncorr_DN) {
        err = ele.errEffSF_reco_dn[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Iso_TOTAL_Uncorr_UP) {
        err = ele.errEffSF_iso_up[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Iso_TOTAL_Uncorr_DN) {
        err = ele.errEffSF_iso_dn[m_signalId];
    }
    #warning for trigger SF systematics we only handle single electron sf
    else if(sys == NtSys::EL_EFF_Trigger_TOTAL_DN) {
        err = ele.errEffSF_trig_dn_single[m_signalId];
    }
    else if(sys == NtSys::EL_EFF_Trigger_TOTAL_UP) {
        err = ele.errEffSF_trig_up_single[m_signalId];
    }
    else {
        cout << "ElectronSelector::errEffSF(): you are calling this function with"
             <<" sys '" << NtSys::SusyNtSysNames.at(sys)<<"'."
             <<" This is not an electron sys. Returning " << err
             << endl;
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
    return (std::abs(el.clusEtaBE) < 1.37 ||
            std::abs(el.clusEtaBE) > 1.52 );
}

//----------------------------------------------------------
// begin ElectronSelector_2Lep Ana_2Lep
//----------------------------------------------------------
bool ElectronSelector_2Lep::isBaseline(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->looseLLHBLayer && 
                el->passOQBadClusElectron &&
                el->Pt()  > 10.0 && 
                std::abs(el->clusEta) < 2.47);
    }
    return pass;
}
//----------------------------------------------------------
bool ElectronSelector_2Lep::passIpCut(const Electron &el)
{
    return (std::abs(el.d0sigBSCorr)  < 5.0 &&
            std::abs(el.z0SinTheta()) < 0.5 );

}
//----------------------------------------------------------
bool ElectronSelector_2Lep::isSignal(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (isBaseline(el)       &&
                el->mediumLLH        &&
                el->isoGradientLoose &&
                passIpCut(*el));
    }
    return pass;
}

//----------------------------------------------------------
// begin ElectronSelector_3Lep Ana_3Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin ElectronSelector_4Lep Ana_4Lep
//----------------------------------------------------------

bool ElectronSelector_4Lep::isSignal(const Electron* el)
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
// begin ElectronSelector_2LepWH Ana_2LepWH
//----------------------------------------------------------
bool ElectronSelector_2LepWH::passIpCut(const Electron &el)
{
    return (std::abs(el.d0sigBSCorr)  < 3.0 && // tighter than default
            std::abs(el.z0SinTheta()) < 0.4 );

}

//----------------------------------------------------------
// begin ElectronSelector_SS3L Ana_SS3L
//----------------------------------------------------------
bool ElectronSelector_SS3L::isBaseline(const Electron* el)
{
    bool pass = false;
    if(el) {
        pass = (el->looseLLH &&
                el->passOQBadClusElectron &&
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

        bool passLH = false;
        if (el->Pt() < 300) {
            if(el->tightLLH) passLH = true;
        } else {
            if(el->mediumLLH) passLH = true;
        }
        pass = (isBaseline(el) &&
                isIsolated &&
                passLH &&
                std::abs(el->trackEta)     <  2.0 &&
                std::abs(el->z0SinTheta()) <  0.5 );
    }
    return pass;
}

//----------------------------------------------------------
// begin ElectronSelector_Stop2L Ana_Stop2L
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
