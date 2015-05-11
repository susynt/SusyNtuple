#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/SusyNt.h"

#include <set>
#include <iostream>

using Susy::OverlapTools;
using Susy::Electron;
using Susy::Muon;
using std::cout;
using std::endl;


namespace Susy {


// ------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------ //
OverlapTools::OverlapTools() :
    m_doHarmonization(false),
    m_verbose(false)
{
}
// ------------------------------------------------------------------------------ //
OverlapTools& OverlapTools::setAnalysis(const AnalysisType& a)
{
    switch(a) {
    ///////////////////////////////////
    // Contrarian analyses
    ///////////////////////////////////
    case(AnalysisType::Ana_2Lep) : 
    case(AnalysisType::Ana_3Lep) : 
    case(AnalysisType::Ana_2LepWH) : {
        m_doHarmonization = false;
        break;
    }
    ///////////////////////////////////
    // Analyses using harmonized OR
    ///////////////////////////////////
    // None yet!
    ///////////////////////////////////
    // Too far, set default to Run-I
    ///////////////////////////////////
    case(AnalysisType::kUnknown) : {
        cout << "OverlapTools::setAnalysis() error: invalid analysis"
             << " '"<<std::underlying_type<AnalysisType>::type(a)<< "'" << endl;
        cout << "               will applay default overlap procedure (Run-1)." << endl;
#warning We should set the defaullt selections ffor tools to be a consistent AnaType
        m_doHarmonization = false;
        break;
    }
    } // switch

    if(m_doHarmonization) cout << "OverlapTools::setAnalysis(): Using harmonized overlap procedure." << endl;

    return *this;
}
// ------------------------------------------------------------------------------ //
bool OverlapTools::doHarmonization()
{
    return m_doHarmonization;
}
// ------------------------------------------------------------------------------ //
// Main overlap removal function
// ------------------------------------------------------------------------------ //

void OverlapTools::performOverlap(ElectronVector& electrons, MuonVector& muons,
                                    TauVector& taus, JetVector& jets)
{

    /** Following the prescription descibed in:
    https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr1913TeV#Overlap_Removals
    */

    ///////////////////////////////////////////
    // Remove jets overlapping with electrons
    ///////////////////////////////////////////
    j_e_overlap(electrons, jets);

    ///////////////////////////////////////////
    // Remove electrons overlapping with jets
    ///////////////////////////////////////////
    e_j_overlap(electrons, jets);

    ///////////////////////////////////////////
    // Remove muons overlapping with jets
    ///////////////////////////////////////////
    m_j_overlap(muons, jets);

    ///////////////////////////////////////////
    // Remove electrons and muons that overlap
    ///////////////////////////////////////////
    e_m_overlap(electrons, muons);

    ///////////////////////////////////////////
    // Remove electrons that overlap with 
    // each other
    ///////////////////////////////////////////
    e_e_overlap(electrons);
/*    
    // dantrim May 4 2015
    // SUSYTools does not include the tau-related OR
    // neither for harmonized nor for Run-1
    // so will keep these commented for now

    ///////////////////////////////////////////
    // Remove muons that overlap with 
    // each other
    ///////////////////////////////////////////
    m_m_overlap(muons);

    ///////////////////////////////////////////
    // Remove taus overlapping with electrons
    ///////////////////////////////////////////
    t_e_overlap(taus, electrons);
    
    ///////////////////////////////////////////
    // Remove taus overlapping with muons
    ///////////////////////////////////////////
    t_m_overlap(taus, muons);

    ///////////////////////////////////////////
    // Remove jets overlapping with taus
    ///////////////////////////////////////////
    j_t_overlap(taus, jets);

*/

}

// ------------------------------------------------------------------------------ //
// Components of Overlap Removal
// -----------------------------------------------------------------------------  //

////////////////////////////////////////////
// Remove jets overlapping with electrons
//  J_E_DR = 0.2
///////////////////////////////////////////
void OverlapTools::j_e_overlap(ElectronVector& electrons, JetVector& jets)
{
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaR(*j) > J_E_DR) continue;
            jets.erase(jets.begin()+iJ);
        } // iJ
    } // iEl
}
////////////////////////////////////////////
// Remove electrons overlapping with jets
//  E_J_DR = 0.4
////////////////////////////////////////////
void OverlapTools::e_j_overlap(ElectronVector& electrons, JetVector& jets)
{
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaR(*j) > E_J_DR) continue;
            electrons.erase(electrons.begin()+iEl);
            break; // move to next electron since iEl no longer exists!
        } // iJ
    } // iEl
}
////////////////////////////////////////////
// Remove muons overlapping with jets
//  M_J_DR = 0.4
////////////////////////////////////////////
void OverlapTools::m_j_overlap(MuonVector& muons, JetVector& jets)
{
    // Harmonized: Keep muon, remove jet if jet has less than 3 tracks
    //             else keep jet and remove muon
    // Non-harmonized: Keep jet, remove muon
    if(muons.size()==0 || jets.size()==0) return;
    if(!m_doHarmonization){
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            for(int iJ=jets.size()-1; iJ>=0; iJ--){
                const Jet* j = jets.at(iJ);
                if(mu->DeltaR(*j) > M_J_DR) continue;
                muons.erase(muons.begin()+iMu);
                break; // move to next muon since iMu no longer exists!
            } // iJ
        } // iMu
    } // !doHarm
    else if(m_doHarmonization){
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            for(int iJ=jets.size()-1; iJ>=0; iJ--){
                const Jet* j = jets.at(iJ);
                if(mu->DeltaR(*j) > M_J_DR) continue;
                int jet_nTrk = jets.at(iJ)->nTracks;
                if(jet_nTrk<3) jets.erase(jets.begin()+iJ);
                else{
                    muons.erase(muons.begin()+iMu);
                    break; // move to next muon since iMu no longer exists!
                }
            } // iJ
        } // iMu
    } // doHarm
}

////////////////////////////////////////////
// Remove electrons and muons overlapping 
// with one another
//  E_M_DR = 0.01
////////////////////////////////////////////
void OverlapTools::e_m_overlap(ElectronVector& electrons, MuonVector& muons)
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
            if(e->DeltaR(*mu) < E_M_DR){
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
            } // found one
        } // iEl
    } // el2remove

    // remove the flagged muons
    if(muonsToRemove.size()){
        for(int iMu=nMu-1; iMu>=0; iMu--){
            if(muonsToRemove.find(muons.at(iMu)) != muonsToRemove.end()){
                muons.erase(muons.begin()+iMu);
            } // found one
        } // iMu
    } // mu2remove
}
    
////////////////////////////////////////////
// Remove (sub-lead) electron overlapping
// with (lead) lepton
//  E_E_DR = 0.05
////////////////////////////////////////////
void OverlapTools::e_e_overlap(ElectronVector& electrons)
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
            if(ei->DeltaR(*ej) < E_E_DR){
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
////////////////////////////////////////////
// Remove muons overlapping with one another
//  M_M_DR = 0.05
////////////////////////////////////////////
void OverlapTools::m_m_overlap(MuonVector& muons)
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
            if(imu->DeltaR(*jmu) < M_M_DR){
                muonsToRemove.insert(imu);
                muonsToRemove.insert(jmu);
            } // if OR
        } // jMu
    } // iMu
    // remove flagged muons
    for(int iMu=nMu-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        if(muonsToRemove.find(mu) != muonsToRemove.end()) {
            muons.erase(muons.begin()+iMu);
        } // remove
    } // iMu
}
////////////////////////////////////////////
// Remove taus overlapping with electrons
//  T_E_DR = 0.2
////////////////////////////////////////////
void OverlapTools::t_e_overlap(TauVector& taus, ElectronVector& electrons)
{
    int nTau = taus.size();
    int nEle = electrons.size();
    if(nTau==0 || nEle==0) return;
    for(int iTau=nTau-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iEl=nEle-1; iEl>=0; iEl--){
            const Electron* e = electrons.at(iEl);
            if(tau->DeltaR(*e) < T_E_DR){
                taus.erase(taus.begin()+iTau);
                break; // loop tau doesn't exist anymore!
            } // dR match
        } // iEl
    } // iTau
}
////////////////////////////////////////////
// Remove taus overlapping with muons
//  T_M_DR = 0.2
////////////////////////////////////////////
void OverlapTools::t_m_overlap(TauVector& taus, MuonVector& muons)
{
    int nTau = taus.size();
    int nMuo = muons.size();
    if(nTau==0 || nMuo==0) return;
    for(int iTau=nTau-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iMu=nMuo-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            if(tau->DeltaR(*mu) < T_M_DR){
                taus.erase(taus.begin()+iTau);
                break; // loop tau doesn't exist anymore!
            } // dR match
        } // iMu
    } // iTau
}
////////////////////////////////////////////
// Remove jets overlapping with taus
//  J_T_DR = 0.2
////////////////////////////////////////////
void OverlapTools::j_t_overlap(TauVector& taus, JetVector& jets)
{
    int nTau=taus.size();
    int nJet=jets.size();
    if(nTau==0 || nJet==0) return;
    for(int iTau=nTau-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iJet=nJet-1; iJet>=0; iJet--){
            const Jet* jet=jets.at(iJet);
            // dantrim Apr 21 : previously, by default, we removed the jet over the tau --> do we want to have the option of removing the tau over the jet?
            if(tau->DeltaR(*jet) < J_T_DR) {
                jets.erase(jets.begin()+iJet); 
            } // dR match
        } // iJet
    } // iTau
}

/*
 if(tau->DeltaR(*jet) < J_T_DR) {
    if(removeJets) jets.erase(jets.begin()+iJet);
 else {
    taus.erase(taus.begin()+iTau_;
    break;
  }
*/



}; // namespace Susy
