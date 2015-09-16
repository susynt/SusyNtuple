#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/JetSelector.h"

#include <set>
#include <iostream>

using Susy::OverlapTools;
using Susy::Electron;
using Susy::Muon;
using std::cout;
using std::endl;
using std::string;


namespace Susy {
//----------------------------------------------------------
OverlapTools* OverlapTools::build(const AnalysisType &a, bool verbose)
{
    OverlapTools *o;
    switch(a) {
    case AnalysisType::Ana_2Lep   :
        o = new OverlapTools_2Lep();
        break;
    case AnalysisType::Ana_3Lep   :
        o = new OverlapTools_3Lep();
        break;
    case AnalysisType::Ana_2LepWH :
        o = new OverlapTools_2LepWH();
        break;
    case AnalysisType::Ana_SS3L   :
        o = new OverlapTools_SS3L();
        break;
    case AnalysisType::Ana_Stop2L :
        o = new OverlapTools_Stop2L();
        break;
    default:
        cout<<"OverlapTools::build(): unknown analysis type '"<<AnalysisType2str(a)<<"'"
            <<" returning vanilla OverlapTools"<<endl;
        o = new OverlapTools();
    }
    return o;
}
//----------------------------------------------------------
OverlapTools::OverlapTools() :
    m_electronIsolation(Isolation::IsolationInvalid),
    m_muonIsolation(Isolation::IsolationInvalid),
    m_verbose(false)
{
}
//----------------------------------------------------------
void OverlapTools::performOverlap(ElectronVector& electrons, MuonVector& muons,
                                    TauVector& taus, JetVector& jets)
{
    /** Following the rel20/13TeV prescription described in:
        https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Overlap_Removals
    */

    j_e_overlap(electrons, jets, 0.2); // was J_E_DR
    e_j_overlap(electrons, jets, 0.4); // was E_J_DR
    m_j_overlap(muons, jets, 0.4); // was M_J_DR
    e_m_overlap(electrons, muons, 0.01); // was E_M_DR
    e_e_overlap(electrons, 0.05); // was E_E_DR
/*
    // dantrim May 4 2015
    // SUSYTools does not include the tau-related OR neither for
    // harmonized nor for Run-1 so will keep these commented for now

    m_m_overlap(muons, 0.05); // was M_M_DR
    t_e_overlap(taus, electrons, 0.2);  // was T_E_DR
    t_m_overlap(taus, muons, 0.2);  // was T_M_DR
    j_t_overlap(taus, jets, 0.2);  // was J_T_DR

*/

}
//----------------------------------------------------------
bool OverlapTools::leptonPassesIsolation(const Lepton* lep, const Isolation &iso)
{

    if      (iso == Isolation::GradientLoose)   return lep->isoGradientLoose;
    else if (iso == Isolation::Gradient)        return lep->isoGradient;
    else if (iso == Isolation::LooseTrackOnly)  return lep->isoLooseTrackOnly;
    else if (iso == Isolation::Loose)           return lep->isoLoose;
    else if (iso == Isolation::Tight)           return lep->isoTight;
    else {
        cout<<"OverlapTools::leptonPassesIsolation:"
            <<" unknown isolation '"<<Isolation2str(iso)<<"'."<<endl;
        exit(1);
    }
}
//----------------------------------------------------------
void OverlapTools::removeNonisolatedLeptons(ElectronVector& electrons, MuonVector& muons)
{
    if(m_electronIsolation==Isolation::IsolationInvalid ||
       m_muonIsolation==Isolation::IsolationInvalid) {
        cout<<"OverlapTools::removeNonisolatedLeptons:"
            <<" you need to call setElectronIsolation() and setMuonIsolation() first."
            <<endl;
        exit(1);
    }
    for(size_t iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        if(!leptonPassesIsolation(e, m_electronIsolation)) { electrons.erase(electrons.begin()+iEl); continue; }
    }
    for(size_t iMu=muons.size()-1; iMu>=0; iMu--) {
        const Muon* m = muons.at(iMu);
        if(!leptonPassesIsolation(m, m_muonIsolation)) { muons.erase(muons.begin()+iMu); continue; }
    }
}
//----------------------------------------------------------
void OverlapTools::j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    // default procedure:
    //  > if dR between electron and jet < J_E_DR:
    //          --> remove the jet and keep the electron
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < dR) {
                jets.erase(jets.begin()+iJ);
            }
        } // iJ
    } // iEl

}
//----------------------------------------------------------
void OverlapTools::e_j_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* e = electrons.at(iEl);
        for(size_t iJ=0; iJ<jets.size(); iJ++){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < dR) {
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        } // for(iJ)
    } // for(iEl)
}
//----------------------------------------------------------
void OverlapTools::m_j_overlap(MuonVector& muons, JetVector& jets, double dR)
{
    if(muons.size()==0 || jets.size()==0) return;
    for(int iMu=muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            int jet_nTrk = j->nTracks;
            if(mu->DeltaRy(*j) > dR) continue;
            if(jet_nTrk < 3) {
                jets.erase(jets.begin() + iJ);
            }
            else {
                muons.erase(muons.begin() + iMu);
                break;
            }
        } // for(iJ)
    } // for(iMu)
}
//----------------------------------------------------------
void OverlapTools::e_m_overlap(ElectronVector& electrons, MuonVector& muons, double dR)
{
    int nEl = electrons.size();
    int nMu = muons.size();
    if(nEl==0 || nMu==0) return;

    static std::set<const Electron*> electronsToRemove;
    static std::set<const Muon*> muonsToRemove;
    electronsToRemove.clear();
    muonsToRemove.clear();
    for(int iEl=0; iEl<nEl; iEl++){
        const Electron* e = electrons.at(iEl);
        for(int iMu=0; iMu<nMu; iMu++){
            const Muon* mu = muons.at(iMu);
            if(e->DeltaRy(*mu) < dR){
                electronsToRemove.insert(e);
                muonsToRemove.insert(mu);
            } // dR match
        } // iMu
    } // iEl

    // remove the flagged electrons
    if(electronsToRemove.size()){
        for(int iEl=nEl-1; iEl>=0; iEl--){
            if(electronsToRemove.find(electrons.at(iEl)) != electronsToRemove.end()){
                electrons.erase(electrons.begin()+iEl);
            }
        }
    }
    // remove the flagged muons
    if(muonsToRemove.size()){
        for(int iMu=nMu-1; iMu>=0; iMu--){
            if(muonsToRemove.find(muons.at(iMu)) != muonsToRemove.end()){
                muons.erase(muons.begin()+iMu);
            }
        }
    }
}
//----------------------------------------------------------
void OverlapTools::e_e_overlap(ElectronVector& electrons, double dR)
{
    int nEl=electrons.size();
    if(nEl<2) return;

    // Find all possible e-e pairings
    static std::set<const Electron*> electronsToRemove;
    electronsToRemove.clear();
    for(int iEl=0; iEl<nEl; iEl++){
        const Electron* ei = electrons.at(iEl);
        for(int jEl=iEl+1; jEl<nEl; jEl++){
            const Electron* ej = electrons.at(jEl);
            if(ei->DeltaRy(*ej) < dR){
                if(ei->Pt() < ej->Pt()){
                    electronsToRemove.insert(ei);
                    break; // ei no longer exists for looping!
                } else{
                    electronsToRemove.insert(ej);
                }
            } // dR match
        } // jEl
    } // iEl
    // remove the flagged electrons
    for(int iEl=nEl-1; iEl>=0; iEl--){
        if(electronsToRemove.find(electrons.at(iEl)) != electronsToRemove.end()){
            electrons.erase(electrons.begin()+iEl);
        } // found one
    }
}
//----------------------------------------------------------
void OverlapTools::m_m_overlap(MuonVector& muons, double dR)
{
    int nMu = muons.size();
    if(nMu < 2) return;

    // if 2 muons overlap, flag both for removal
    static std::set<const Muon*> muonsToRemove;
    muonsToRemove.clear();
    for(int iMu=0; iMu<nMu; iMu++){
        const Muon* imu = muons.at(iMu);
        for(int jMu=iMu+1; jMu<nMu; jMu++){
            const Muon* jmu = muons.at(jMu);
            if(imu->DeltaRy(*jmu) < dR){
                muonsToRemove.insert(imu);
                muonsToRemove.insert(jmu);
            }
        } // for(jMu)
    } // for(iMu)
    // remove flagged muons
    for(int iMu=nMu-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        if(muonsToRemove.find(mu) != muonsToRemove.end()) {
            muons.erase(muons.begin()+iMu);
        }
    }
}
//----------------------------------------------------------
void OverlapTools::t_e_overlap(TauVector& taus, ElectronVector& electrons, double dR)
{
    int nTau = taus.size();
    int nEle = electrons.size();
    if(nTau==0 || nEle==0) return;
    for(int iTau=nTau-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iEl=nEle-1; iEl>=0; iEl--){
            const Electron* e = electrons.at(iEl);
            if(tau->DeltaRy(*e) < dR){
                taus.erase(taus.begin()+iTau);
                break; // loop tau doesn't exist anymore!
            } // if(dR< )
        } // for(iEl)
    } // for(iTau)
}
//----------------------------------------------------------
void OverlapTools::t_m_overlap(TauVector& taus, MuonVector& muons, double dR)
{
    int nTau = taus.size();
    int nMuo = muons.size();
    if(nTau==0 || nMuo==0) return;
    for(int iTau=nTau-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iMu=nMuo-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            if(tau->DeltaRy(*mu) < dR){
                taus.erase(taus.begin()+iTau);
                break; // loop tau doesn't exist anymore!
            } // if(dR< )
        } // for(iMu)
    } // for(iTau)
}
//----------------------------------------------------------
void OverlapTools::j_t_overlap(TauVector& taus, JetVector& jets, double dR)
{
    int nTau=taus.size();
    int nJet=jets.size();
    if(nTau==0 || nJet==0) return;
    for(int iJet=nJet-1; iJet>=0; iJet--){
        const Jet* jet = jets.at(iJet);
        for(int iTau=nTau-1; iTau>=0; iTau--){
            const Tau* tau = taus.at(iTau);
            if(tau->DeltaRy(*jet) < dR){
                jets.erase(jets.begin()+iJet);
                break; // loop jet doesn't exist anymore!
            } // if(dR< )
        } // for(iTau)
    } // for(iJet)
}
//----------------------------------------------------------
// begin OverlapTools_SS3L
//----------------------------------------------------------
void OverlapTools_SS3L::j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    // doing BjetOR procedure:
    //  > if electron close to jet:
    //       --> if bjet: keep the jet, remove electron
    //       --> if not bjet: remove the jet, keep the electron
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < 0.2){ // was J_E_DR
                bool isBjet = (j->mv2c20 > JetSelector::mv2c20_80efficiency());
                if(isBjet) {
                    electrons.erase(electrons.begin()+iEl);
                    break; // loop electron no longer exists
                } else {
                    jets.erase(jets.begin()+iJ);
                }
            } // if(dR<)
        } // for(iJ)
    } // for(iEl)
}
//----------------------------------------------------------
}; // namespace Susy
