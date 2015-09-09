#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/Muon.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {
//----------------------------------------------------------
MuonSelector* MuonSelector::build(const AnalysisType &a, bool verbose)
{
    MuonSelector *s;
    switch(a) {
    case AnalysisType::Ana_2Lep   :
        s = new MuonSelector_2Lep();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_3Lep   :
        s = new MuonSelector_3Lep();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::Tight);
        break;
    case AnalysisType::Ana_2LepWH :
        s = new MuonSelector_2LepWH();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::Tight);
        break;
    case AnalysisType::Ana_SS3L   :
        s = new MuonSelector_SS3L();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_Stop2L :
        s = new MuonSelector_Stop2L();
        s->setSignalId(MuonId::Loose);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    default:
        cout<<"MuonSelector::build(): unknown analysis type '"<<AnalysisType2str(a)<<"'"
            <<" returning vanilla MuonSelector"<<endl;
        s = new MuonSelector();
    }
    return s;
}
//----------------------------------------------------------
MuonSelector::MuonSelector():
    m_signalId(Susy::MuonId::MuonIdInvalid),
    m_signalIsolation(Isolation::IsolationInvalid),
    m_verbose(false)
{
}
//----------------------------------------------------------
bool MuonSelector::isBaselineMuon(const Muon* mu)
{
    // works for Ana_2Lep, Ana_3Lep, Ana_2LepWH
    bool pass = false;
    if(mu) {
        pass = (mu->medium &&
                mu->Pt()        > 10.0 &&
                fabs(mu->Eta()) <  2.4 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector::isSignalMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->Pt() > 10.0 &&
                mu->medium &&
                mu->isoGradientLoose &&
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector::passIpCut(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (fabs(mu->d0sigBSCorr)  < 3.0 &&
                fabs(mu->z0SinTheta()) < 0.4 );
    }
    return pass;
}
//----------------------------------------------------------
float MuonSelector::effSF(const Muon& mu)
{
    // return the EffSF for the corresponding (signal) muon quality
    return mu.muoEffSF[m_signalId];
}
//----------------------------------------------------------
float MuonSelector::errEffSF(const Muon& mu, const SusyNtSys sys)
{
    // return the error on the muon SF associated with systematc sys
    float err = 0.0;
    if(sys == NtSys::MUONSFSTAT_UP) {
        err = mu.errEffSF_stat_up[m_signalId];
    } else if(sys == NtSys::MUONSFSTAT_DN) {
        err = mu.errEffSF_stat_dn[m_signalId];
    } else if(sys == NtSys::MUONSFSYS_UP) {
        err = mu.errEffSF_syst_up[m_signalId];
    } else if(sys == NtSys::MUONSFSYS_DN) {
        err = mu.errEffSF_syst_dn[m_signalId];
    } else {
        cout<<"MuonSelector::errEffSF(): you are calling this function with"
            <<" sys '"<<NtSys::SusyNtSysNames[sys]<<"'."
            <<" This is not a muon sys. Returning "<<err
            <<endl;
    }
    return err;
}
//----------------------------------------------------------
// begin MuonSelector_3Lep
//----------------------------------------------------------
bool MuonSelector_3Lep::isSignalMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaselineMuon(mu) &&
                mu->isoTight &&
                passIpCut(mu));
    }
    return pass;
}

//----------------------------------------------------------
// begin MuonSelector_ss3l
//----------------------------------------------------------
bool MuonSelector_2LepWH::isSignalMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaselineMuon(mu) &&
                mu->ptvarcone30/mu->Pt() < 0.06 && // note: isBaselineMuon guarantees pt>0
                mu->etconetopo30/mu->Pt() < 0.14 && // was etcone?
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
// begin MuonSelector_ss3l
//----------------------------------------------------------
bool MuonSelector_SS3L::passIpCut(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (fabs(mu->d0sigBSCorr)  < 3.0 &&
                fabs(mu->z0SinTheta()) < 0.5 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_SS3L::isBaselineMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->medium &&
                mu->Pt()        > 10.0 &&
                fabs(mu->Eta()) <  2.5 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_SS3L::isSignalMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaselineMuon(mu) &&
                mu->ptvarcone30/mu->Pt() < 0.06 && // note: isBaselineMuon guarantees pt>0
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
// begin MuonSelector_Stop2L
// TODO Danny please check values
//----------------------------------------------------------
bool MuonSelector_Stop2L::passIpCut(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (fabs(mu->d0sigBSCorr)  < 3.0 &&
                fabs(mu->z0SinTheta()) < 0.5 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_Stop2L::isBaselineMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->loose &&
                mu->Pt()        > 10.0 &&
                fabs(mu->Eta()) <  2.4 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_Stop2L::isSignalMuon(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->Pt() > 10.0 &&
                isBaselineMuon(mu) &&
                mu->isoGradientLoose &&
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
}; // namespace Susy
