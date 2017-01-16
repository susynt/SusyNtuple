#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/JetSelector.h"


#include <cassert>
#include <set>
#include <iomanip>
#include <iostream>
#include <sstream>

using Susy::OverlapTools;
using Susy::Electron;
using Susy::Muon;
using Susy::Jet;
using Susy::Lepton;
using std::cout;
using std::cerr;
using std::endl;
using std::string;


#include <stdio.h>
//----------------------------------------------------------
// a few helper functions to print things out
// (the str() ones could go in Jet, Lepton, etc.)
//----------------------------------------------------------
string str(const Jet* j)
{
    std::ostringstream oss;
    oss<<"jet "
       <<"("
       <<std::setprecision(3)<<j->Pt()<<", "
       <<std::setprecision(3)<<j->Eta()<<", "
       <<std::setprecision(3)<<j->Phi()<<", "
       <<"nTk "<<j->nTracks<<", "
       <<"mv2c10 "<<j->mv2c10
       <<")";
    return oss.str();
}

string str(const Lepton* l)
{
    std::ostringstream oss;
    oss<<(l->isEle() ? "el":"mu")<<" "
       <<"("
       <<std::setprecision(3)<<l->Pt()<<", "
       <<std::setprecision(3)<<l->Eta()<<", "
       <<std::setprecision(3)<<l->Phi()
       <<")";
    return oss.str();
}
string str(const Susy::Photon* pho)
{
    std::ostringstream oss;
    oss <<"("
        <<std::setprecision(3)<<pho->Pt()<<", "
        <<std::setprecision(3)<<pho->Eta()<<", "
        <<std::setprecision(3)<<pho->Phi()
        <<")";
    return oss.str();
}

void print_rm_msg(const string &msg, const Jet* j_drop, const Lepton* l_keep)
{
    cout<<msg<<" removing "<<str(j_drop)<<" overlapping (dRy="<<l_keep->DeltaRy(*j_drop)<<") with "<<str(l_keep)<<endl;
}
void print_rm_msg(const string &msg, const Lepton* l_drop, const Jet* j_keep)
{
    cout<<msg<<" removing "<<str(l_drop)<<" overlapping (dRy="<<l_drop->DeltaRy(*j_keep)<<") with "<<str(j_keep)<<endl;
}
void print_rm_msg(const string &msg, const Lepton* l_drop, const Lepton* l_keep)
{
    cout<<msg<<" removing "<<str(l_drop)<<" overlapping (dRy="<<l_drop->DeltaRy(*l_keep)<<") with "<<str(l_keep)<<endl;
}
void print_rm_msg(const string &msg, const Susy::Photon* pho_drop, const Lepton* l_keep)
{
    cout<<msg<<" removing photon "<<str(pho_drop)<<" overlapping (dRy="<<pho_drop->DeltaRy(*l_keep)<<") with "<<str(l_keep)<<endl;
}
void print_rm_msg(const string &msg, const Jet* j_drop, const Susy::Photon* p_keep)
{
    cout<<msg<<" removing jet " << str(j_drop)<<" overlapping (dRy="<<j_drop->DeltaRy(*p_keep)<<") with photon "<<str(p_keep)<<endl;
}
//----------------------------------------------------------

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
    case AnalysisType::Ana_4Lep   :
        o = new OverlapTools_4Lep();
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
    m_verbose(false),
    m_useOldOverlap(false)
{
}
//----------------------------------------------------------
void OverlapTools::performOverlap(ElectronVector& electrons, MuonVector& muons,
                                    JetVector& jets, TauVector& taus, PhotonVector& photons)
{

    // ---------------------------------------------- //
    /*    Implement default SUSYTools OR              */
    // ---------------------------------------------- //
    // default specification as seen in Bkg Forum
    // on January 26 2016 :
    //  https://indico.cern.ch/event/490240/attachments/1225853/1794468/ORsummary.pdf

    /* --------------------------------------------
        Remove overlapping taus and (light) leptons

            Remove taus matched within dRy cone with electrons/muons
                > default cone size is dRy = 0.2
    */

    /*
    // tau OR not default -- TODO : add toggle to turn this on/off

    t_e_overlap(taus, electrons, 0.2);
    t_m_overlap(taus, muons, 0.2);
    */

    /* ---------------------------------------------
        Remove overlapping electrons and muons

        step 1:
            Remove calo-tagged muons that have shared ID track
            with electrons.
            See OverlapTools::eleMuSharedTrack(...)

            m_e_overlap(muons, electrons)

        step 2:
            Remove electrons that have shared ID track
            with remaining muons.
            See OverlapTools::eleMuSharedTrack(...)

            e_m_overlap(electrons, muons)

    */
    m_e_overlap(muons, electrons);
    e_m_overlap(electrons, muons);


    /* --------------------------------------------
        Remove overlapping photons and (light) leptons

            Remove photons matched within dRy with electrons/muons
                > default cone size is dRy = 0.4
    */

    /*
    // photon OR not default -- TODO : add toggle to turn this on/off

    p_e_overlap(photons, electrons, 0.4);
    p_m_overlap(photons, muons, 0.4);
    */

    /* --------------------------------------------
        Remove overlapping jets and electrons

        step 1:
            Remove jets overlapping with electrons
            j_e_overlap(electrons, jets, float dRcone, bool doBJetOR)
                > doBJetOR : if true, do not compare jets and electrons when jet is
                             tagged as a b-jet.
                             See OverlapTools::isBJetOR(...)

        step 2:
            Remove electrons overlapping with remaining jets
            e_j_overlap(electrons, jets, float dRcone, bool slidingCone, bool applyJVT)
                > slidingCone : if true, use a cone with dR size determined by
                                sliding cone algorithm. See
                                OverlapTools::getSlidingDRCone(...)
                > applyJVT : if true, do not remove electron if the jet
                             being compared to is flagged as a pileup jet
    */
    j_e_overlap(electrons, jets, 0.2, true);
    e_j_overlap(electrons, jets, 0.4, false, true);

    /* --------------------------------------------
        Remove overlapping jets and muons

        step 1:
            Remove jets overlapping with muons
            j_m_overlap(jets, muons, float dRcone, bool doBJetOR, bool doGhost)
                > doBJetOR : if true, do not compare jets and muons when jet is
                             tagged as a b-jet
                             See OverlapTools::isBJetOR(...)
                > doGhost  : if true, check if muon is ghost associated with the jet
                             and rmoeve the jet if it is (in addition to the dR match).
                             See OverlapTools::muonIsGhostMatched(...)

        step 2:
            Remove muons overlapping with remaining jets
            m_j_overlap(muons, jets, float dRcone, bool slidingCone, bool applyJVT)
                > slidingCone : if true, use a cone with dR size determined by
                                sliding cone algorithm. See
                                OverlapTools::getSlidingDRCone(...)
                > applyJVT : if true, do not remove muon if the jet
                             being compared to is flagged as a pileup jet
    */
    j_m_overlap(jets, muons, 0.2, true, true);
    m_j_overlap(muons, jets, 0.4, true, true);

    /* --------------------------------------------
        Remove overlapping taus and jets

            Remove jets matched within dRy cone with remaining taus
                > default cone size is dRy = 0.2
    */

    // tau OR not default -- TODO add toggle
    //j_t_overlap(taus, jets, 0.2);

    /* --------------------------------------------
        Remove overlapping jets and photons

            Remove jets matched within dRy cone with remaining photons
                > default cone size is dRy = 0.4
    */
    // photon OR not default -- TODO add toggle
    //j_p_overlap(jets, photons, 0.4);

}
//----------------------------------------------------------
bool OverlapTools::leptonPassesIsolation(const Lepton* lep, const Isolation &iso)
{

    if      (iso == Isolation::GradientLoose)           return lep->isoGradientLoose;
    else if (iso == Isolation::Gradient)                return lep->isoGradient;
    else if (iso == Isolation::LooseTrackOnly)          return lep->isoLooseTrackOnly;
    else if (iso == Isolation::Loose)                   return lep->isoLoose;
    else if (iso == Isolation::FixedCutTightTrackOnly)  return lep->isoFixedCutTightTrackOnly;
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
bool OverlapTools::muonIsGhostMatched(const Muon* mu, const Jet* jet)
{
    if(!(mu->ghostTrack.size()>0))
        return false;
    if(jet->idx > ((int)mu->ghostTrack.size()-1))
        return false;
    return ((mu->ghostTrack[jet->idx]==1) ? true : false);
}
//----------------------------------------------------------
bool OverlapTools::eleMuSharedTrack(const Electron* el, const Muon* mu)
{
    if(!(el->sharedMuTrk.size()>0))
        return false;
    if(mu->idx > ((int)el->sharedMuTrk.size()-1))
        return false;
    return ((el->sharedMuTrk[mu->idx]==1) ? true : false);
}
//----------------------------------------------------------
float OverlapTools::getSlidingDRCone(float pT, float c1, float c2, float maxConeSize)
{
    float dRcone = c1 + ( c2 / pT * 1.0);
    // return upper-limit on cone size
    return std::min(dRcone, maxConeSize);
}
//----------------------------------------------------------
bool OverlapTools::isBJetOR(const Jet* jet)
{
    // TODO -- implement this from within JetSelector ?
    return ((jet->Pt()        > 20.0 /*GeV*/) &&
            (fabs(jet->Eta()) < 2.5         ) &&
            (jet->mv2c10      > JetSelector::mv2c10_85efficiency()) &&
            (JetSelector::passJvt(jet)));
}
//----------------------------------------------------------
void OverlapTools::j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR /*=0.2*/, bool doBJetOR /*=true*/)
{
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iJ=jets.size()-1; iJ>=0; iJ--){
        const Jet* j = jets.at(iJ);
        if(doBJetOR)
            if(isBJetOR(j)) continue;
        for(int iEl=0; iEl < (int)electrons.size(); iEl++){
            const Electron* el = electrons.at(iEl);
            if(el->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("j_e_overlap: ", j, el);
                jets.erase(jets.begin()+iJ);
                break;
            }
        } // iEl
    } // iJ
/*
 // old method for removing jets from electrons

    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("j_e_overlap: ", j, e);
                jets.erase(jets.begin()+iJ);
            }
        } // iJ
    } // iEl
*/

}
//----------------------------------------------------------
void OverlapTools::e_j_overlap(ElectronVector& electrons, JetVector& jets, double dR /*=0.4*/, bool doSlidingCone /*=false*/, bool applyJVT /*=true*/)
{
    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl = electrons.size()-1; iEl>=0; iEl--){
        const Electron* el = electrons.at(iEl);
        for(int iJ = 0; iJ < (int)jets.size(); iJ++){
            const Jet* j = jets.at(iJ);

            // don't reject electron due to pileup
            if(applyJVT && !JetSelector::passJvt(j)) continue;
            // use sliding cone dR if needed
            if(doSlidingCone) dR = getSlidingDRCone(el->Pt());
            if(el->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("e_j_overlap: ", el, j);
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        } // iJ
    } // iEl

/*
 // old method

    if(electrons.size()==0 || jets.size()==0) return;
    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* e = electrons.at(iEl);
        for(size_t iJ=0; iJ<jets.size(); iJ++){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("e_j_overlap: ", e, j);
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        } // for(iJ)
    } // for(iEl)
*/
}
//----------------------------------------------------------
void OverlapTools::j_m_overlap(JetVector& jets, MuonVector& muons, double dR /*=0.2*/, bool doBJetOR /*=true*/, bool doGhost /*=true*/)
{
    if(jets.size()==0 || muons.size()==0) return;
    for(int iJ = jets.size()-1; iJ>=0; iJ--){
        const Jet* j = jets.at(iJ);

        if(doBJetOR)
            if(isBJetOR(j)) continue;

        for(int iMu = 0; iMu < (int)muons.size(); iMu++){
            const Muon* mu = muons.at(iMu);

            float ptRatio = mu->Pt() / j->Pt() * 1.0;
            float trkRatio = mu->Pt() / j->sumTrkPt * 1.0;
            if( (j->nTracks >= 3) &&
                ( ptRatio<0.5 || trkRatio<0.7) ) continue;

            bool remove_jet = false;
            // check dR match regardless
            if(j->DeltaRy(*mu) < dR) remove_jet = true;
            if(doGhost) {
                if(muonIsGhostMatched(mu, j)) remove_jet = true;
            }
            if(remove_jet) {
                if(verbose()) print_rm_msg("j_m_overlap: ", j, mu);
                jets.erase(jets.begin()+iJ);
                break;
            }
        } // iMu
    } // iJ
}
//----------------------------------------------------------
void OverlapTools::m_j_overlap(MuonVector& muons, JetVector& jets, double dR /*=0.4*/, bool doSlidingCone /*=false*/, bool applyJVT /*=true*/)
{

    if(muons.size()==0 || jets.size()==0) return;
    for(int iMu = muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        for(int iJ = 0; iJ < (int)jets.size(); iJ++){
            const Jet* j = jets.at(iJ);

            // don't reject muons due to pileup
            if(applyJVT && !JetSelector::passJvt(j)) continue;
            // use sliding cone dR if needed
            if(doSlidingCone) dR = getSlidingDRCone(mu->Pt());
            if(mu->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("m_j_overlap: ", mu, j);
                muons.erase(muons.begin()+iMu);
                break;
            }
        } // iJ
    } // iMu

/*
 // old method
    if(muons.size()==0 || jets.size()==0) return;
    for(int iMu=muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            int jet_nTrk = j->nTracks;
            if(mu->DeltaRy(*j) > dR) continue;
            if(jet_nTrk < 3) {
                if(verbose()) print_rm_msg("m_j_overlap: ", j, mu);
                jets.erase(jets.begin() + iJ);
            }
            else {
                if(verbose()) print_rm_msg("m_j_overlap: ", mu, j);
                muons.erase(muons.begin() + iMu);
                break;
            }
        } // for(iJ)
    } // for(iMu)
*/
}
//----------------------------------------------------------
void OverlapTools::m_e_overlap(MuonVector& muons, ElectronVector& electrons)
{
    if(electrons.size()==0 || muons.size()==0) return;

    for(int iMu=muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        if( !mu->isCaloTagged ) continue;
        for(int iEl=0; iEl<(int)electrons.size(); iEl++){
            const Electron* el = electrons.at(iEl);
            if(eleMuSharedTrack(el, mu)) {
                if(verbose()) print_rm_msg("m_e_overlap: ", mu, el);
                muons.erase(muons.begin()+iMu);
                break;
            } // shared tracks
        } // iEl
    } // iMu
}
//----------------------------------------------------------
void OverlapTools::e_m_overlap(ElectronVector& electrons, MuonVector& muons)
{
    if(electrons.size()==0 || muons.size()==0) return;

    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* el = electrons.at(iEl);
        for(int iMu=0; iMu<(int)muons.size(); iMu++){
            const Muon* mu = muons.at(iMu);
            if(eleMuSharedTrack(el, mu)) {
                if(verbose()) print_rm_msg("e_m_overlap: ", el, mu);
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        } // iMu
    } // iEl
}

//----------------------------------------------------------
void OverlapTools::e_m_overlap(ElectronVector& electrons, MuonVector& muons, float dR)
{
 // old e_m_overlap method
 // TODO -- check if any analysis really does this dR based one still

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
                if(verbose()) print_rm_msg("e_m_overlap: ", e, mu);
                if(verbose()) print_rm_msg("e_m_overlap: ", mu, e);
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
                    if(verbose()) print_rm_msg(":e_e_overlap: ", ei, ej);
                    electronsToRemove.insert(ei);
                    break; // ei no longer exists for looping!
                } else{
                    if(verbose()) print_rm_msg(":e_e_overlap: ", ej, ei);
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
                if(verbose()) print_rm_msg("m_m_overlap: ", imu, jmu);
                if(verbose()) print_rm_msg("m_m_overlap: ", jmu, imu);
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
                // \todo if(verbose()) print_rm_msg()
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

            // muons must have a min. pT for comparison
            if(mu->Pt() < 2.0 /*GeV*/) continue;

            // high pT taus are only compared to combined muons
            if( (tau->Pt() > 50 /*GeV*/) && !mu->isCombined) continue;

            if(tau->DeltaRy(*mu) < dR){
                // \todo if(verbose()) print_rm_msg()
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
                // \todo if(verbose()) print_jet("j_t_overlap: removing jet ", jets[iJet]);
                jets.erase(jets.begin()+iJet);
                break; // loop jet doesn't exist anymore!
            } // if(dR< )
        } // for(iTau)
    } // for(iJet)
}
//----------------------------------------------------------
void OverlapTools::p_e_overlap(PhotonVector& photons, ElectronVector& electrons, double dR)
{
    if(photons.size()==0 || electrons.size()==0) return;
    for(int iPh = photons.size()-1; iPh>=0; iPh--){
        const Photon* pho = photons.at(iPh);
        for(int iEl = 0; iEl < (int)electrons.size(); iEl++){
            const Electron* ele = electrons.at(iEl);
            if(pho->DeltaRy(*ele) < dR) {
                if(verbose()) print_rm_msg("p_e_overlap: ", pho, ele);
                photons.erase(photons.begin()+iPh);
                break;
            }
        } // iEl
    } // iPh
}
//----------------------------------------------------------
void OverlapTools::p_m_overlap(PhotonVector& photons, MuonVector& muons, double dR)
{
    if(photons.size()==0 || muons.size()==0) return;
    for(int iPh = photons.size()-1; iPh>=0; iPh--){
        const Photon* pho = photons.at(iPh);
        for(int iMu = 0; iMu < (int)muons.size(); iMu++){
            const Muon* mu = muons.at(iMu);
            if(pho->DeltaRy(*mu) < dR) {
                if(verbose()) print_rm_msg("p_m_overlap: ", pho, mu);
                photons.erase(photons.begin()+iPh);
                break;
            }
        } // iMu
    } // iPh
}
//----------------------------------------------------------
void OverlapTools::j_p_overlap(JetVector& jets, PhotonVector& photons, double dR /*=0.4*/)
{
    if(jets.size()==0 || photons.size()==0) return;
    for(int iJ = jets.size()-1; iJ>=0; iJ--){
        const Jet* j = jets.at(iJ);
        for(int iP = 0; iP < (int)photons.size(); iP++) {
            const Photon* pho = photons.at(iP);
            if(pho->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("j_p_overlap: ", j, pho);
                jets.erase(jets.begin()+iJ);
                break;
            }
        } // iP
    } // iJ
}

//----------------------------------------------------------
// begin OverlapTools_4Lep Ana_4Lep
//----------------------------------------------------------
void OverlapTools_4Lep::performOverlap(ElectronVector& electrons, MuonVector& muons,
				       TauVector& taus,TauVector& LOOSEtaus, JetVector& jets, PhotonVector& photons, bool m_TauCtrlReg)
{

    // if PhotonVector is empty, photon overlap won't be performed
    // if m_TauCtrlReg, then jets will be discarded on overlap with loose taus
    /// updated for SUSYTools/harmonization consistency Feb/16

    /*1*/   if(!m_TauCtrlReg) t_e_overlap(taus,electrons,0.2); // straightforward dR comparison
            else t_e_overlap(LOOSEtaus,electrons,0.2);
    /*2*/   if(!m_TauCtrlReg) t_m_overlap(taus, muons,0.2);    // dR comparison for taus pT<50. For tau pT>50, only compare to combined muons
            else t_m_overlap(LOOSEtaus, muons,0.2);

    /*3*/   e_m_overlap(electrons,muons);    // discard electrons sharing ID track with muons (Calo muons are be removed first if they share a track with an electron)
    /*4,5*/ photon_lep_overlap(photons, electrons, muons, 0.4);	// discard pho overlapping with e/mu
    /*6*/   j_e_overlap(electrons, jets, 0.2);  // discard non-b-tagged jets with dR<0.2
            e_j_overlap(electrons,jets,0.4);    // discard ele with dR<0.4 to non-pileup jets

    /*7*/   j_m_overlap(muons, jets,0.2);  // discard jets with few tracks or small jet/mu pT ratio.
            m_j_overlap(muons, jets,0.4);       // discard ele with dR<0.4 to non-pileup jets

    /*8*/   if(!m_TauCtrlReg) j_t_overlap(taus,jets,0.2);  // discard jets overlapping with signal taus
            else j_t_overlap(LOOSEtaus,jets,0.2);  // compare to all loose taus for CR

    /*9*/   j_photon_overlap(photons, jets, 0.4);	// discard jet overlapping with photon
}

//----------------------------------------------------------
void OverlapTools_4Lep::e_m_overlap(ElectronVector& electrons, MuonVector& muons)
{
    if(electrons.size()==0 || muons.size()==0) return;

    bool removeCaloMuons=true;
    if(m_useOldOverlap) removeCaloMuons=false;

    // Discard Calo muons if they share a track with an electron
    if( removeCaloMuons ){
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            if(!mu->isCaloTagged) continue;
            for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
                const Electron* e = electrons.at(iEl);

                // compare the tracks and dR to make sure it is overlapping THIS muon track
                if( eleMuSharedTrack(e, mu) && e->DeltaRy(*mu) < 0.001){
                    muons.erase(muons.begin()+iMu);
                    break;
                }
            } // iEl
        } // iMu
    } // if removeCaloMuons

    // discard electrons sharing ID track with surviving muons
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);

            if( eleMuSharedTrack(e, mu) && e->DeltaRy(*mu) < 0.001){
                electrons.erase(electrons.begin()+iEl);
                break;
            }
        } // iMu
    } // iEl

}

//----------------------------------------------------------
void OverlapTools_4Lep::t_m_overlap(TauVector& taus, MuonVector& muons, double dR)
{
    if(taus.size()==0 || muons.size()==0) return;

    for(int iTau=taus.size()-1; iTau>=0; iTau--){
        const Tau* tau = taus.at(iTau);
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);
            //High-pT taus are only compared to combined muons
            if(tau->Pt()>50.0 && !mu->isCombined) continue;

            if(tau->DeltaRy(*mu) < dR){
                taus.erase(taus.begin()+iTau);
                break;
            } // if(dR< )

        } // for(iMu)
    } // for(iTau)
}

//----------------------------------------------------------
void OverlapTools_4Lep::photon_lep_overlap(PhotonVector& photons, ElectronVector& /*pre*/electrons, MuonVector& /*pre*/muons, double dR)
{
    if( photons.size()==0) return;

    // discard photons overlapping with muons
    for(int iPho=photons.size()-1; iPho>=0; iPho--){
        const Photon* pho = photons.at(iPho);
        for(int iMu=muons.size()-1; iMu>=0; iMu--){
            const Muon* mu = muons.at(iMu);

            if(pho->DeltaRy(*mu) < dR){
                photons.erase(photons.begin()+iPho);
                break;
            } // if(dR< )

        } // for(iMu)
    } // for(iPho)

    // discard photons overlapping with electrons
    for(int iPho=photons.size()-1; iPho>=0; iPho--){
        const Photon* pho = photons.at(iPho);
        for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
            const Electron* el = electrons.at(iEl);

            if(pho->DeltaRy(*el) < dR){
                photons.erase(photons.begin()+iPho);
                break;
            } // if(dR< )

        } // for(iEl)
    } // for(iPho)

}

//----------------------------------------------------------
void OverlapTools_4Lep::j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    if(electrons.size()==0 || jets.size()==0) return;

    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);

            bool is_bjet = isBJetOR(j);
            // only compare non-b-tagged jets
            if( is_bjet ) continue;

            if(e->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("j_e_overlap: ", j, e);
                jets.erase(jets.begin()+iJ);
            }

        } // iJ
    } // iEl

}

//----------------------------------------------------------
void OverlapTools_4Lep::e_j_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    if(electrons.size()==0 || jets.size()==0) return;

    for(int iEl=electrons.size()-1; iEl>=0; iEl--){
        const Electron* e = electrons.at(iEl);
        for(size_t iJ=0; iJ<jets.size(); iJ++){
            const Jet* j = jets.at(iJ);

            bool pass_jvt = JetSelector::passJvt(j);
            //don't compare with pileup jets
            if( !m_useOldOverlap && !pass_jvt ) continue;
                if(e->DeltaRy(*j) < dR) {
                    if(verbose()) print_rm_msg("e_j_overlap: ", e, j);
                    electrons.erase(electrons.begin()+iEl);
                    break;
                }
        } // for(iJ)
    } // for(iEl)
}

//----------------------------------------------------------
void OverlapTools_4Lep::j_m_overlap(MuonVector& muons, JetVector& jets, double dR)
{
    if(m_useOldOverlap) return;
    if(muons.size()==0 || jets.size()==0) return;

    for(int iMu=muons.size()-1; iMu>=0; iMu--) {
        const Muon* mu = muons.at(iMu);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);

            bool is_bjet = isBJetOR(j);
            // only compare non-b-tagged jets
            if( is_bjet ) continue;

            bool discard_jet = false;

            // only compare jets with high nTrk and low mu/jet pt ratio
            int jet_nTrk = j->nTracks;
            double sumTrkPt = j->sumTrkPt;
            if( jet_nTrk >= 3  && (mu->Pt()/j->Pt() < 0.5 || mu->Pt()/sumTrkPt < 0.7)) continue;
            if(mu->DeltaRy(*j) < dR) discard_jet = true;

            // discard jets with ghost tracks matched to the muon
            if(muonIsGhostMatched(mu, j) && mu->DeltaRy(*j) < dR ) discard_jet=true;

            if(discard_jet) {
                if(verbose()) print_rm_msg("j_m_overlap: ", j, mu);
                jets.erase(jets.begin()+iJ);
            }

        } // iJ
    } // iMu
}

//----------------------------------------------------------
void OverlapTools_4Lep::m_j_overlap(MuonVector& muons, JetVector& jets, double dR)
{
    if(muons.size()==0 || jets.size()==0) return;

    for(int iMu=muons.size()-1; iMu>=0; iMu--){
        const Muon* mu = muons.at(iMu);
        for(size_t iJ=0; iJ<jets.size(); iJ++){
            const Jet* j = jets.at(iJ);

            bool pass_jvt = m_jetSelector->passJvt(j);
            //don't compare with pileup jets
            if( !m_useOldOverlap && !pass_jvt ) continue;

            if(mu->DeltaRy(*j) < dR) {
                if(verbose()) print_rm_msg("m_j_overlap: ", mu, j);
                muons.erase(muons.begin()+iMu);
                break;
            }

        } // for(iJ)
    } // for(iMu)
}

//----------------------------------------------------------
void OverlapTools_4Lep::j_photon_overlap(PhotonVector& photons, JetVector& jets, double dR)
{
    if(photons.size()==0 || jets.size()==0) return;
    for(int iPh=photons.size()-1; iPh>=0; iPh--) {
        const Photon* ph = photons.at(iPh);

        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);

            if(ph->DeltaRy(*j) < dR) {
                jets.erase(jets.begin()+iJ);
            }
        } // iJ
    } // iPh
}


//----------------------------------------------------------
// begin OverlapTools_SS3L
//----------------------------------------------------------
OverlapTools_SS3L::OverlapTools_SS3L():
    OverlapTools(),
    m_jetSelector(nullptr){}
//----------------------------------------------------------
void OverlapTools_SS3L::performOverlap(ElectronVector& electrons, MuonVector& muons,
                                    JetVector& jets, TauVector& taus)
{
    j_e_overlap(electrons, jets, 0.2);
    e_j_overlap(electrons, jets, 0.4);
    m_j_overlap(muons, jets, 0.4);
    e_m_overlap(electrons, muons, 0.01);
    e_e_overlap(electrons, 0.05);
}
//----------------------------------------------------------
void OverlapTools_SS3L::j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR)
{
    // doing BjetOR procedure:
    //  > if electron close to jet:
    //       --> if bjet: keep the jet, remove electron
    //       --> if not bjet: remove the jet, keep the electron
    if(electrons.size()==0 || jets.size()==0) return;
    JetSelector_SS3L* jetSelector = static_cast<JetSelector_SS3L*>(JetSelector::build(AnalysisType::Ana_SS3L, true));
    if(not jetSelector) {
        cerr<<"OverlapTools_SS3L: cannot perform j_e_overlap without JetSelector"<<endl
            <<"Please call OverlapTools_SS3L::jetSelector(JetSelector*)"<<endl;
        // assert(false);
    }
    for(int iEl=electrons.size()-1; iEl>=0; iEl--) {
        const Electron* e = electrons.at(iEl);
        for(int iJ=jets.size()-1; iJ>=0; iJ--){
            const Jet* j = jets.at(iJ);
            if(e->DeltaRy(*j) < dR){
                bool isBjet = jetSelector->isB_for_OR(j);
                if(isBjet) {
                    if(verbose()) print_rm_msg("j_e_overlap: ", e, j);
                    electrons.erase(electrons.begin()+iEl);
                    break; // loop electron no longer exists
                } else {
                    if(verbose()) print_rm_msg("j_e_overlap: ", j, e);
                    jets.erase(jets.begin()+iJ);
                }
            } // if(dR<)
        } // for(iJ)
    } // for(iEl)
}
//----------------------------------------------------------
}; // namespace Susy
