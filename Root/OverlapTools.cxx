#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/SusyNt.h"

#include <set>
#include <iostream>

using Susy::OverlapTools;
using Susy::Electron;
using Susy::Muon;
using std::cout;
using std::endl;
using std::string;


namespace Susy {


// ------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------ //
OverlapTools::OverlapTools() :
    m_analysis(AnalysisType::kUnknown),
    m_useSignalLeptons(false),
    m_useIsoLeptons(false),
    m_electronIsolation(Isolation::IsolationInvalid),
    m_muonIsolation(Isolation::IsolationInvalid),
    m_mv2c20_ORcut(-999),
    m_doBjetOR(false),
    m_verbose(false)
{
}
// ------------------------------------------------------------------------------ //
OverlapTools& OverlapTools::setAnalysis(const AnalysisType& a)
{

    ////////////////////////////////////////
    // Set analysis-specific OR procedures
    ////////////////////////////////////////
    
    ///////////////////////////////////
    // Contrarian analyses
    ///////////////////////////////////
    if( a == AnalysisType::Ana_2Lep   ||
        a == AnalysisType::Ana_3Lep   ||
        a == AnalysisType::Ana_2LepWH ||
        a == AnalysisType::Ana_Stop2L ) {
        m_useSignalLeptons = false;
        m_useIsoLeptons    = false;
        m_doBjetOR         = false;
    }
    ///////////////////////////////////
    // SS3L-ANALYSIS
    ///////////////////////////////////
    else if ( a == AnalysisType::Ana_SS3L ) {
        // change muon-jet dr
        M_J_DR = 0.2;
        m_useSignalLeptons = false;
        m_useIsoLeptons    = false;
        m_doBjetOR         = true;
    }
    ///////////////////////////////////
    // Didn't set AnalysisType
    ///////////////////////////////////
    else if ( a == AnalysisType::kUnknown ) {
        string error = "OverlapTools::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for OverlapTools. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for OverlapTools for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    ///////////////////////////////////
    // Gone fishin'
    ///////////////////////////////////
    else {
        string error = "OverlapTools::setAnalysis error: ";
        cout << error << "OverlapTools is not configured for the AnalysisType (" << AnalysisType2str(a) << ") provided in " << endl;
        cout << error << "'setAnalysis'! Be sure that your AnalysisType is provided for in SusyNtuple/AnalysisType.h" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    
    m_analysis = a;

    // check that ele/muon isolation has been set if we are using isolated leptons
    if(useIsolatedLeptons() && (m_electronIsolation==Isolation::IsolationInvalid || m_muonIsolation==Isolation::IsolationInvalid)) {
        string error = "OverlapTools::setAnalysis error: ";
        cout << error << "You have configured the overlap removal to use isolated leptons but" << endl;
        cout << error << "have not set the electron and/or muon isolation qualities:" << endl;
        cout << error << " > Electron isolation = " << Isolation2str(m_electronIsolation) << endl;
        cout << error << " > Muon isolation = " << Isolation2str(m_muonIsolation) << endl;
        cout << error << "You must use the 'setElectronIsolation' and 'setMuonIsolation' methods of" << endl;
        cout << error << "OverlapTools to set these values prior to calling 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1); 
    }

    // check that the b-tag WP has been set if we are using the b-jet OR procedure
    if(doBjetOR() && (m_mv2c20_ORcut==-999)) {
        string error = "OverlapTools::setAnalysis error: ";
        cout << error << "You have configured the overlap removal to use the b-jet OR procedure but" << endl;
        cout << error << "have not set the b-tag WP (MV2C20 score) to use." << endl;
        cout << error << "You must use the 'setORBtagEff' methods of OverlapTools to set this value" << endl;
        cout << error << "prior to calling 'setAnalysis'." << endl;
        cout << error << "The recommended MV2C20 cut value is provided by the JetSelector method" << endl;
        cout << error << "'overlapRemovalBtagEffWP()'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    cout << "OverlapTools    Configured overlap removal procedure for AnalysisType " << AnalysisType2str(m_analysis) << endl;
    cout << "OverlapTools     > using signal leptons    : " << (m_useSignalLeptons ? "True" : "False") << endl;
    cout << "OverlapTools     > using isolated leptons  : " << (m_useIsoLeptons ? "True" : "False");
    if(m_useIsoLeptons && m_verbose) cout << " (with electron isolation: " << Isolation2str(m_electronIsolation) << " and muon isolation: " << Isolation2str(m_muonIsolation) << ")" << endl;
    else { cout << endl; }
    cout << "OverlapTools     > doing b-jet OR procedure: " << (m_doBjetOR ? "True" : "False");
    if(m_doBjetOR && m_verbose) cout << " (with MV2C20 cut: " << m_mv2c20_ORcut << ")" << endl;
    else { cout << endl; }

    return *this;
}
// ------------------------------------------------------------------------------ //
// Main overlap removal function
// ------------------------------------------------------------------------------ //

void OverlapTools::performOverlap(ElectronVector& electrons, MuonVector& muons,
                                    TauVector& taus, JetVector& jets)
{
    OverlapTools::check();

    /** Following the rel20/13TeV prescription described in:
    https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Overlap_Removals
    */

    ///////////////////////////////////////////
    // If using isolated leptons in OR, remove
    // the non-isolated leptons
    // --> using GradientLoose WP for isolation
    //     (this is default in SUSYTools)
    ///////////////////////////////////////////
    if(useIsolatedLeptons()){
        for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
            const Electron* e = electrons.at(iEl);
            if(!leptonPassesIsolation(e, true)) { electrons.erase(electrons.begin()+iEl); continue; }
            //if(!e->isoGradientLoose) { electrons.erase(electrons.begin() + iEl); continue; }
        }
        for(int iMu=muons.size()-1; iMu>=0; iMu--) {
            const Muon* m = muons.at(iMu);
            if(!leptonPassesIsolation(m, false)) { muons.erase(muons.begin()+iMu); continue; }
            //if(!m->isoGradientLoose) { muons.erase(muons.begin() + iMu); continue; }
        }
    }

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

bool OverlapTools::leptonPassesIsolation(const Lepton* lep, bool isEle)
{
    OverlapTools::check();

    Isolation iso = isEle ? m_electronIsolation : m_muonIsolation;

    if      (iso == Isolation::GradientLoose)   return lep->isoGradientLoose;
    else if (iso == Isolation::Gradient)        return lep->isoGradient;
    else if (iso == Isolation::LooseTrackOnly)  return lep->isoLooseTrackOnly;
    else if (iso == Isolation::Loose)           return lep->isoLoose;
    else if (iso == Isolation::Tight)           return lep->isoTight;
    else {
        cout << "OverlapTools::leptonPassesIsolation error: isolation requirement for leptons not set." << endl;
        cout << "OverlapTools::leptonPassesIsolation error:   >>> Exiting." << endl;
        exit(1);
    }
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
    /////////////////////////
    // update
    /////////////////////////
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            // if doing BjetOR procedure:
            //  > if dR between electron and jet < J_E_DR:
            //          --> if bjet: keep the jet, remove electron
            //          --> if not bjet: remove the jet, keep the electron
            // if not doing BjetOR procedure:
            //  > if dR between electron and jet < J_E_DR:
            //          --> remove the jet and keep the electron
            if(doBjetOR()){
                if(e->DeltaRy(*j) < J_E_DR){
                    if(j->mv2c20 > -0.5517) {
                        electrons.erase(electrons.begin()+iEl);
                        break; // loop electron no longer exists
                    } else { jets.erase(jets.begin()+iJ); }
                } // dR match
            } // doBjetOR
            else {
                if(e->DeltaRy(*j) < J_E_DR) { jets.erase(jets.begin()+iJ); }
            } // not doBjetOR
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
        for(uint iJ=0; iJ<jets.size(); iJ++){
       // for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j)<E_J_DR) {
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        //    if(e->DeltaRy(*j) > E_J_DR) continue;
        //    electrons.erase(electrons.begin()+iEl);
        //    break; // move to next electron since iEl no longer exists!
        } // iJ
    } // iEl
}
////////////////////////////////////////////
// Remove muons overlapping with jets
//  M_J_DR = 0.4
////////////////////////////////////////////
void OverlapTools::m_j_overlap(MuonVector& muons, JetVector& jets)
{
    /////////////////////////
    // update
    /////////////////////////
    if(muons.size()==0 || jets.size()==0) return;
    for(int iMu=muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            int jet_nTrk = j->nTracks;
            if(mu->DeltaRy(*j) > M_J_DR) continue;
            if(jet_nTrk < 3) {
                jets.erase(jets.begin() + iJ);
                //AT: Don't we need a break here ???
            }
            else {
                muons.erase(muons.begin() + iMu);
                break;
            }
        } // iJ
    } // iMu
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
            if(e->DeltaRy(*mu) < E_M_DR){
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
            if(ei->DeltaRy(*ej) < E_E_DR){
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
            if(imu->DeltaRy(*jmu) < M_M_DR){
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
            if(tau->DeltaRy(*e) < T_E_DR){
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
            if(tau->DeltaRy(*mu) < T_M_DR){
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
    for(int iJet=nJet-1; iJet>=0; iJet--){
        const Jet* jet = jets.at(iJet);
        for(int iTau=nTau-1; iTau>=0; iTau--){
            const Tau* tau = taus.at(iTau);
            if(tau->DeltaRy(*jet) < J_T_DR){
                jets.erase(jets.begin()+iJet);
                break; // loop jet doesn't exist anymore!
            } // dR match
        } // iTau
    } // iJet
}


// ------------------------------------------------------------------------------ //
void OverlapTools::check()
{
    if(m_analysis == AnalysisType::kUnknown) {
        string error = "OverlapTools::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for OverlapTools. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for OverlapTools for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    return;
}

}; // namespace Susy
