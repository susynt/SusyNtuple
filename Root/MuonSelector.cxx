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
        s->setSignalIsolation(Isolation::FixedCutTightTrackOnly);
        break;
    case AnalysisType::Ana_4Lep   :
        s = new MuonSelector_4Lep();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::GradientLoose);
        break;
    case AnalysisType::Ana_2LepWH :
        s = new MuonSelector_2LepWH();
        s->setSignalId(MuonId::Medium);
        s->setSignalIsolation(Isolation::FixedCutTightTrackOnly);
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
bool MuonSelector::isBaseline(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->medium &&
                mu->Pt()        > 10.0 &&
                fabs(mu->Eta()) <  2.4 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector::isSignal(const Muon* mu)
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
                fabs(mu->z0SinTheta()) < 0.5 );
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
    if       (sys == NtSys::MUON_EFF_STAT_UP) {
        err = mu.errEffSF_stat_up[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_STAT_DN) {
        err = mu.errEffSF_stat_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_SYS_UP) {
        err = mu.errEffSF_syst_up[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_SYS_DN) {
        err = mu.errEffSF_syst_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_STAT_LOWPT_DN) {
        err = mu.errEffSF_stat_lowpt_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_STAT_LOWPT_UP) {
        err = mu.errEffSF_stat_lowpt_up[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_SYS_LOWPT_DN) {
        err = mu.errEffSF_syst_lowpt_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_EFF_SYS_LOWPT_UP) {
        err = mu.errEffSF_syst_lowpt_up[m_signalId];
    }
    else if(sys == NtSys::MUON_ISO_STAT_DN) {
        err = mu.errIso_stat_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_ISO_STAT_UP) {
        err = mu.errIso_stat_up[m_signalId];
    }
    else if(sys == NtSys::MUON_ISO_SYS_DN) {
        err = mu.errIso_syst_dn[m_signalId];
    }
    else if(sys == NtSys::MUON_ISO_SYS_UP) {
        err = mu.errIso_syst_up[m_signalId];
    }
    else {
        cout<<"MuonSelector::errEffSF(): you are calling this function with"
            <<" sys '"<<NtSys::SusyNtSysNames[sys]<<"'."
            <<" This is not a muon sys. Returning "<<err
            <<endl;
    }
    return err;
}
//----------------------------------------------------------
// begin MuonSelector_2Lep Ana_2Lep
//----------------------------------------------------------
bool MuonSelector_2Lep::isBaseline(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->medium &&
                mu->Pt()        > 10.0 && 
                fabs(mu->Eta()) <  2.4); // I don't think this is the best ASM 9/6/16
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_2Lep::isSignal(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaseline(mu)       &&
                mu->isoGradientLoose &&
                passIpCut(mu)        );
    }
    return pass;
}
//----------------------------------------------------------
// begin MuonSelector_3Lep Ana_3Lep
//----------------------------------------------------------
bool MuonSelector_3Lep::isSignal(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaseline(mu) &&
                mu->isoFixedCutTightTrackOnly &&
                passIpCut(mu));
    }
    return pass;
}

//----------------------------------------------------------
// begin MuonSelector_4Lep
//----------------------------------------------------------
bool MuonSelector_4Lep::isBaseline(const Muon* mu)
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
// begin MuonSelector_2LepWH Ana_2LepWH
//----------------------------------------------------------
bool MuonSelector_2LepWH::isSignal(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaseline(mu) &&
                mu->ptvarcone30/mu->Pt() < 0.06 &&
                mu->etconetopo30/mu->Pt() < 0.14 &&
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
// begin MuonSelector_SS3L Ana_SS3L
//----------------------------------------------------------
bool MuonSelector_SS3L::isBaseline(const Muon* mu)
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
bool MuonSelector_SS3L::isSignal(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (isBaseline(mu) &&
                mu->ptvarcone30/mu->Pt() < 0.06 &&
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
// begin MuonSelector_Stop2L Ana_Stop2L
//----------------------------------------------------------
bool MuonSelector_Stop2L::isBaseline(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->medium &&
                mu->Pt()        > 10.0 &&
                fabs(mu->Eta()) <  2.4 );
    }
    return pass;
}
//----------------------------------------------------------
bool MuonSelector_Stop2L::isSignal(const Muon* mu)
{
    bool pass = false;
    if(mu) {
        pass = (mu->Pt() > 10.0 &&
                isBaseline(mu) &&
                mu->isoGradientLoose &&
                passIpCut(mu));
    }
    return pass;
}
//----------------------------------------------------------
}; // namespace Susy
