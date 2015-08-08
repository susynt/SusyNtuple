#include <set>

#include "TFile.h"
#include "TKey.h"
#include "TChainElement.h"
#include "TH1F.h"
#include "TSystem.h"

#include "Mt2/mt2_bisect.h" 

#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/OverlapTools.h"

#include <cassert>

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() :
m_anaType(AnalysisType::Ana_3Lep),
m_doPtconeCut(true),
m_doElEtconeCut(true),
m_doMuEtconeCut(false),
m_doIPCut(true)
{
}
/*--------------------------------------------------------------------------------*/
// Full object selection methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt,
                                     ElectronVector& preElecs, MuonVector& preMuons, JetVector& preJets,
                                     ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets,
                                     SusyNtSys sys, bool selectTaus)
{
    // Preselection
    preElecs = getPreElectrons(susyNt, sys);
    preMuons = getPreMuons(susyNt, sys);
    preJets = getPreJets(susyNt, sys);
    if (selectTaus) taus = getPreTaus(susyNt, sys);
    else taus.clear();

    // Baseline objects
    elecs = preElecs;
    muons = preMuons;
    jets = preJets;

    // Overlap removal
    if(!m_overlapTool.useSignalLeptons()){
        m_overlapTool.performOverlap(elecs, muons, taus, jets);
    }

    // Remove MSFOS < 12 GeV
    removeSFOSPair(elecs, MLL_MIN);
    removeSFOSPair(muons, MLL_MIN);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt, ElectronVector& elecs,
                                     MuonVector& muons, TauVector& taus, JetVector& jets,
                                     SusyNtSys sys, bool selectTaus)
{
    // Get all objects
    elecs = getPreElectrons(susyNt, sys);
    muons = getPreMuons(susyNt, sys);
    jets = getPreJets(susyNt, sys);
    if (selectTaus) taus = getPreTaus(susyNt, sys);
    else taus.clear();

    //cout<<"Select Taus: "<<selectTaus<<" size: "<<taus.size()<<endl;

    // Now perform the overlap removals
    if(!m_overlapTool.useSignalLeptons()){
        m_overlapTool.performOverlap(elecs, muons, taus, jets);
    }

    // Do SFOS removal for Mll < 12 
    removeSFOSPair(elecs, MLL_MIN);
    removeSFOSPair(muons, MLL_MIN);
    // TODO: revisit this??
    //removeSFOSPair(taus, MLL_MIN);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                   const TauVector& baseTaus, const JetVector& baseJets,
                                   ElectronVector& sigElecs, MuonVector& sigMuons,
                                   TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep,
                                   uint nVtx, bool isMC, bool removeLepsFromIso,
                                   TauID tauJetID, TauID tauEleID, TauID tauMuoID,
                                   SusyNtSys sys)
{
    // Set signal objects
    sigElecs = getSignalElectrons(baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso);
    sigMuons = getSignalMuons(baseMuons, baseElecs, nVtx, isMC, removeLepsFromIso);
    sigTaus = getSignalTaus(baseTaus, tauJetID, tauEleID, tauMuoID);
    sigJets = getSignalJets(baseJets, sys);
//    sigJets2Lep = getSignalJets2Lep(baseJets, sys);

    if(m_overlapTool.useSignalLeptons()){
        m_overlapTool.performOverlap(sigElecs, sigMuons, sigTaus, sigJets);
    }


}
/*--------------------------------------------------------------------------------*/
// New signal tau prescription, fill both ID levels at once!
void SusyNtTools::getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                   const TauVector& baseTaus, const JetVector& baseJets,
                                   ElectronVector& sigElecs, MuonVector& sigMuons,
                                   TauVector& mediumTaus, TauVector& tightTaus,
                                   JetVector& sigJets, JetVector& sigJets2Lep,
                                   uint nVtx, bool isMC, bool removeLepsFromIso,
                                   SusyNtSys sys)
{
    // Set signal objects
    sigElecs = getSignalElectrons(baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso);
    sigMuons = getSignalMuons(baseMuons, baseElecs, nVtx, isMC, removeLepsFromIso);
    sigJets = getSignalJets(baseJets, sys);
//    sigJets2Lep = getSignalJets2Lep(baseJets, sys);

    getSignalTaus(baseTaus, mediumTaus, tightTaus);


    if(m_overlapTool.useSignalLeptons()){
        // Taus not used if harmonized OR -- using tightTaus as dummy signature
        m_overlapTool.performOverlap(sigElecs, sigMuons, tightTaus, sigJets);
    }


}
/*--------------------------------------------------------------------------------*/
// This method cannot be used anymore. 
// Analyzers must store the baseline objects for cleaning cuts!
/*--------------------------------------------------------------------------------*/
/*void SusyNtTools::getSignalObjects(SusyNtObject* susyNt, ElectronVector& sigElecs,
                   MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets,
                   JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC,
                   bool selectTaus, bool removeLepsFromIso, TauID tauID)
                   {
                   // Temporary vectors for baseline objects
                   ElectronVector elecs = getPreElectrons(susyNt, sys);
                   MuonVector     muons = getPreMuons(susyNt, sys);
                   JetVector      jets  = getPreJets(susyNt, sys);

                   TauVector      taus;
                   if(selectTaus) taus = getPreTaus(susyNt, sys);

                   // Perform overlap removal
                   performOverlap(elecs, muons, taus, jets);

                   // Now set the signal objects
                   sigElecs = getSignalElectrons(elecs, muons, nVtx, isMC, removeLepsFromIso);
                   sigMuons = getSignalMuons(muons, elecs, nVtx, isMC, removeLepsFromIso);
                   sigTaus  = getSignalTaus(taus, tauID);
                   sigJets  = getSignalJets(jets);
                   sigJets2Lep = getSignalJets2Lep(jets);
                   }*/

/*--------------------------------------------------------------------------------*/
// Methods to grab the Baseline objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getPreElectrons(SusyNtObject* susyNt, SusyNtSys sys)
{
    // Not sure if I want to pass SusyNt object around or not... but just do it this way
    // for now for lack of a more creative idea.
    ElectronVector elecs;
    for (uint ie = 0; ie < susyNt->ele()->size(); ++ie) {
        Electron* e = &susyNt->ele()->at(ie);
        e->setState(sys);

        //////////////////////////
        // Following SUSYTools
        //////////////////////////
    
        //if(!e->looseLH)                continue;
        //if(e->Eta() > ELECTRON_ETA_CUT) continue;
        //if(e->Pt() < ELECTRON_PT_CUT)   continue;

        if (m_electronSelector.isBaselineElectron(e)){ 
            elecs.push_back(e);
        }
    }
    return elecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getPreMuons(SusyNtObject* susyNt, SusyNtSys sys)
{
    MuonVector muons;
    for (uint im = 0; im < susyNt->muo()->size(); ++im) {
        Muon* mu = &susyNt->muo()->at(im);
        mu->setState(sys);

        //////////////////////////
        // Following SUSYTools
        //////////////////////////
    
        //if(!mu->medium)              continue;
        //if(mu->Eta() > MUON_ETA_CUT) continue;
        //if(mu->Pt() < MUON_PT_CUT)   continue;

        // Save
        if (m_muonSelector.isBaselineMuon(mu)){ 
            muons.push_back(mu);
        }

    }
    return muons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getPreTaus(SusyNtObject* susyNt, SusyNtSys sys)
{
    TauVector taus;
    for (uint iTau = 0; iTau < susyNt->tau()->size(); iTau++) {
        Tau* tau = &susyNt->tau()->at(iTau);
        tau->setState(sys);

        // Apply any additional selection
        if (isSelectTau(tau)) taus.push_back(tau);
    }
    return taus;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getPreJets(SusyNtObject* susyNt, SusyNtSys sys)
{
    JetVector jets;
    for (uint ij = 0; ij < susyNt->jet()->size(); ++ij) {
        Jet* j = &susyNt->jet()->at(ij);
        j->setState(sys);
        if (j->Pt() < JetSelector::defaultMinPt()) continue;
        jets.push_back(j);
    }

    return jets;
}

/*--------------------------------------------------------------------------------*/
// Get Signal objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getSignalElectrons(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                               uint nVtx, bool isMC, bool removeLepsFromIso)
{
    ElectronVector sigElecs;
    for (uint ie = 0; ie < baseElecs.size(); ++ie) {
        Electron* e = baseElecs.at(ie);
        if (m_electronSelector.isSignalElectron(e, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)){ 
      //  if (isSignalElectron(e, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)) {
            sigElecs.push_back(e);
        }
    }
    return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(const MuonVector& baseMuons, const ElectronVector& baseElecs,
                                       uint nVtx, bool isMC, bool removeLepsFromIso)
{
    MuonVector sigMuons;
    for (uint im = 0; im < baseMuons.size(); ++im) {
        Muon* mu = baseMuons.at(im);
        if (m_muonSelector.isSignalMuon(mu, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)){
      //  if (isSignalMuon(mu, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)) {
            sigMuons.push_back(mu);
        }
    }
    return sigMuons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getSignalTaus(const TauVector& baseTaus, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
    TauVector sigTaus;
    for (uint iTau = 0; iTau < baseTaus.size(); iTau++) {
        Tau* tau = baseTaus[iTau];
        if (isSignalTau(tau, tauJetID, tauEleID, tauMuoID)) {
            sigTaus.push_back(tau);
        }
    }
    return sigTaus;
}
/*--------------------------------------------------------------------------------*/
// New signal tau prescription, fill both ID levels at once!
void SusyNtTools::getSignalTaus(const TauVector& baseTaus, TauVector& mediumTaus, TauVector& tightTaus)
{
    for (uint iTau = 0; iTau < baseTaus.size(); iTau++) {
        Tau* tau = baseTaus[iTau];
        // For now, tight taus are defined as all tight criteria
        if (isSignalTau(tau, TauID_tight, TauID_tight, TauID_tight)) {
            tightTaus.push_back(tau);
            mediumTaus.push_back(tau);
        }
        // For now, medium taus use loose ele BDT
        else if (isSignalTau(tau, TauID_medium, TauID_loose, TauID_medium)) mediumTaus.push_back(tau);
    }
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(const JetVector& baseJets, SusyNtSys sys)
{
    JetVector sigJets;
    for(uint ij=0; ij<baseJets.size(); ++ij){
        Jet* j = baseJets.at(ij);
        if(JetSelector().setSystematic(sys).setAnalysis(m_anaType).isSignalJet(j)){
            sigJets.push_back(j);
        }
    }
    return sigJets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets2Lep(const JetVector& baseJets, SusyNtSys sys)
{
    JetVector sigJets;
    for(uint ij=0; ij<baseJets.size(); ++ij){
        Jet* j = baseJets.at(ij);
        if(JetSelector().setSystematic(sys).setAnalysis(m_anaType).isSignalJet2Lep(j)){
            sigJets.push_back(j);
        }
    }
    return sigJets;
}
/*--------------------------------------------------------------------------------*/
PhotonVector SusyNtTools::getSignalPhotons(SusyNtObject* susyNt)
{
    // Currently only storing signal photons, so just a conv way to get them.
    PhotonVector sigPho;
    for (uint ip = 0; ip < susyNt->pho()->size(); ip++) {
        sigPho.push_back(&(susyNt->pho()->at(ip)));
    }

    return sigPho;
}

/*--------------------------------------------------------------------------------*/
// Get Met
/*--------------------------------------------------------------------------------*/
Met* SusyNtTools::getMet(SusyNtObject* susyNt, SusyNtSys sys)//, bool useNomPhiForMetSys)
{
    // Right now not being clever. Could maybe make sys index correspond to 
    // index on the array.

    Met* met = NULL;
    vector<Met>* metTmp = susyNt->met();
    for (uint i = 0; i < metTmp->size(); i++) {
        if (metTmp->at(i).sys == sys) {
            met = &(metTmp->at(i));

            // NEW: For NtSys_SCALEST_UP/NtSys_SCALEST_DOWN use nominal phi
            // Mar 1, 2013 -- No longer recommeneded, left over from HCP.
            //if(useNomPhiForMetSys && (sys == NtSys_SCALEST_UP || 
            //sys == NtSys_SCALEST_DN ||
            //sys == NtSys_RESOST)){
            //for(uint j=0; j<metTmp->size(); ++j){
            //if(metTmp->at(j).sys == NtSys_NOM){
            //met->phi = metTmp->at(j).phi;
            //break;
            //}
            //}
            //}

            return met;
        }
    }
    if (!met) {
        cout << "Error: Unable to find met for given systematic!  Returning NULL!! " << sys << endl;
    }

    return met;
}
/*--------------------------------------------------------------------------------*/
// Get Track Met
/*--------------------------------------------------------------------------------*/
TrackMet* SusyNtTools::getTrackMet(SusyNtObject* susyNt, SusyNtSys sys)//, bool useNomPhiForMetSys)
{
    // Right now not being clever. Could maybe make sys index correspond to 
    // index on the array.

    TrackMet* metTrack = NULL;
    vector<TrackMet>* metTrackTmp = susyNt->tkm();
    for (uint i = 0; i < metTrackTmp->size(); i++)
    {
        if (metTrackTmp->at(i).sys == sys)
        {
            metTrack = &(metTrackTmp->at(i));

            // NEW: For NtSys_SCALEST_UP/NtSys_SCALEST_DOWN use nominal phi
            // Mar 1, 2013 -- No longer recommeneded, left over from HCP.
            //if(useNomPhiForMetSys && (sys == NtSys_SCALEST_UP || 
            //sys == NtSys_SCALEST_DN ||
            //sys == NtSys_RESOST)){
            //for(uint j=0; j<metTmp->size(); ++j){
            //if(metTmp->at(j).sys == NtSys_NOM){
            //met->phi = metTmp->at(j).phi;
            //break;
            //}
            //}
            //}

            return metTrack;
        }
    }
    if (!metTrack)
    {
        cout << "Error: Unable to find metTrack for given systematic!  Returning NULL!! " << sys << endl;
    }

    return metTrack;
}

/*--------------------------------------------------------------------------------*/
// Method for applying tau BDT selection only
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isTauBDT(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
    // Jet BDT cut
    if (tauJetID == TauID_loose) {
        if (tau->jetBDTSigLoose != 1) return false;
    }
    else if (tauJetID == TauID_medium) {
        if (tau->jetBDTSigMedium != 1) return false;
    }
    else if (tauJetID == TauID_tight) {
        if (tau->jetBDTSigTight != 1) return false;
    }

    // Ele BDT cut
    if (tauEleID == TauID_loose) {
        if (tau->nTrack == 1 && tau->eleBDTLoose != 0) return false;
    }
    else if (tauEleID == TauID_medium) {
        if (tau->nTrack == 1 && tau->eleBDTMedium != 0) return false;
    }
    else if (tauEleID == TauID_tight) {
        if (tau->nTrack == 1 && tau->eleBDTTight != 0) return false;
    }

    // Muon veto
    if (tauMuoID >= TauID_medium) {
        if (tau->muonVeto != 0) return false;
    }

    // none selection can be used for container taus
    //else if(id == TauID_none){
    //}
    //else{
    //  cout << "SusyNtTools::WARNING - tau ID " << id << " not supported!" << endl;
    //  return false;
    //}

    return true;
}

/*--------------------------------------------------------------------------------*/
// Check if lepton is selected
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSelectTau(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
    // At the moment, we only apply pt and BDT here.
    // TODO: Make the pt cut configurable
    if (tau->Pt() < TAU_PT_CUT) return false;
    return isTauBDT(tau, tauJetID, tauEleID, tauMuoID);
}

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalLepton(const Lepton* l,
                                 const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                 uint nVtx, bool isMC, bool removeLepsFromIso)
{
    if (l->isEle()) return isSignalElectron((Electron*)l, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
    else           return isSignalMuon((Muon*)l, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* ele,
                                   const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                   uint nVtx, bool isMC, bool removeLepsFromIso)
{
/*
  //AT OBSOLETE -- In ElectronSelector
    if (!ele->tightLH) return false;

    // Impact parameter
    if (m_doIPCut) {
        // All ana using unbiased IP cuts
        //if(fabs(ele->d0Sig(m_anaType == Ana_2Lep)) >= ELECTRON_D0SIG_CUT) return false;
        //if(fabs(ele->z0SinTheta(m_anaType == Ana_2Lep)) >= ELECTRON_Z0_SINTHETA_CUT) return false;
        float maxD0Sig = (m_anaType != AnalysisType::Ana_2LepWH ? ELECTRON_D0SIG_CUT : ELECTRON_D0SIG_CUT_WH);
        if (fabs(ele->d0Sig()) >= maxD0Sig) return false;
        if (fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;
    }

    float pt = ele->Pt();

    // Relative ptcone iso
    if (m_doPtconeCut) { // true by default
        float ptcone30 = elPtConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
        if (m_anaType == AnalysisType::Ana_2LepWH) {
            if (ptcone30 / std::min(pt, ELECTRON_ISO_PT_THRS) >= ELECTRON_PTCONE30_PT_WH_CUT) return false;
        }
        else
            if (ptcone30 / pt >= ELECTRON_PTCONE30_PT_CUT) return false;
    }

    // Topo etcone isolation cut
    if (m_doElEtconeCut) { // true by default
        float etcone = elEtTopoConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
        if (m_anaType == AnalysisType::Ana_2LepWH) {
            if (etcone / std::min(pt, ELECTRON_ISO_PT_THRS) >= ELECTRON_TOPOCONE30_PT_WH_CUT) return false;
        }
        else
            if (etcone / pt >= ELECTRON_TOPOCONE30_PT_CUT) return false;
    }
*/
    return false;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* mu,
                               const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                               uint nVtx, bool isMC, bool removeLepsFromIso)
{
/*
  //AT OBSOLETE -- In MuonSelector
    // Impact parameter
    if (m_doIPCut) {
        // All ana using unbiased IP
        //if(fabs(mu->d0Sig(m_anaType == Ana_2Lep)) >= MUON_D0SIG_CUT) return false;
        //if(fabs(mu->z0SinTheta(m_anaType == Ana_2Lep)) >= MUON_Z0_SINTHETA_CUT) return false;
        if (fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
        if (fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;
    }

    // ptcone isolation cut with pileup correction
    if (m_doPtconeCut) { // true by default
        if (m_anaType == AnalysisType::Ana_3Lep) {
            float ptcone30 = muPtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
            if (ptcone30 / mu->Pt() >= MUON_PTCONE30_PT_CUT) return false;
        }
        else {
            float ptcone30 = mu->ptcone30; // no corrections at the moment. AT: default  ptcone in xAOD
            float pt = mu->Pt();
            if (m_anaType == AnalysisType::Ana_2LepWH) {
                if (ptcone30 / std::min(pt, MUON_ISO_PT_THRS) >= MUON_PTCONE30ELSTYLE_PT_WH_CUT) return false;
            }
            else
                if (ptcone30 / mu->Pt() >= MUON_PTCONE30ELSTYLE_PT_CUT) return false;
        }
    }

    // etcone isolation cut - not applied by default, but here for studies
    if (m_doMuEtconeCut) { // FALSE by default
        float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
        if (m_doMuEtconeCut && etcone30 / mu->Pt() >= MUON_ETCONE30_PT_CUT) return false;
    }
    else if (m_anaType == AnalysisType::Ana_2LepWH) {
        float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
        float pt = mu->Pt();
        if (pt == 0.0 || (etcone30 / std::min(pt, MUON_ISO_PT_THRS) >= MUON_ETCONE30_PT_WH_CUT)) return false;
    }
*/
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalTau(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
    // At the moment, signal taus only use additional BDT selection
    return isTauBDT(tau, tauJetID, tauEleID, tauMuoID);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSemiSignalElectron(const Electron* ele)
{
    /*
  //AT OBSOLETE -- In ElectronSelector
    if (!ele->tightLH) return false;
    // Impact parameter
    if (m_doIPCut) {
        if (fabs(ele->d0Sig()) >= ELECTRON_D0SIG_CUT) return false;
        if (fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;
    }
    */
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSemiSignalMuon(const Muon* mu)
{
    /*
    //AT OBSOLETE -- In MuonSelector
    // Impact parameter
    if (m_doIPCut) {
        if (fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
        if (fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;
    }
    */
    return false;
}

/*--------------------------------------------------------------------------------*/
// Isolation corrections
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elPtConeCorr(const Electron* e,
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                uint /*nVtx*/, bool /*isMC*/, bool removeLeps)
{
/*
  //AT OBSOLETE -- In ElectronSelector
    float ptcone = e->ptcone30;
    if (removeLeps) {
        for (uint iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e2 = baseElectrons[iEl];
            if (e == e2) continue;
            if (!isSemiSignalElectron(e2)) continue;
            float dR = e->DeltaR(*e2);
            if (dR < 0.3) ptcone -= e2->trackPt;
        }
        for (uint iMu = 0; iMu < baseMuons.size(); iMu++) {
            const Muon* mu = baseMuons[iMu];
            if (!isSemiSignalMuon(mu)) continue;
            float dR = e->DeltaR(*mu);
            if (dR < 0.3) ptcone -= mu->idTrackPt;
        }
    }
    return ptcone;
*/
    return -999;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elEtTopoConeCorr(const Electron* e,
                                    const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                    uint nVtx, bool isMC, bool removeLeps)
{
/*
  //AT OBSOLETE -- In ElectronSelector
    float slope = isMC ? ELECTRON_TOPOCONE30_SLOPE_MC : ELECTRON_TOPOCONE30_SLOPE_DATA;
    float etcone = e->etconetopo30 - slope*nVtx;
    if (removeLeps) {
        for (uint iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e2 = baseElectrons[iEl];
            if (e == e2) continue;
            if (!isSemiSignalElectron(e2)) continue;
            float dR = e->DeltaR(*e2);
            if (dR < 0.28) etcone -= e2->clusE / cosh(e2->clusEta);
        }
    }
    return etcone;
*/
    return -999;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muPtConeCorr(const Muon* mu,
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                uint nVtx, bool isMC, bool removeLeps)
{
/*
  //AT OBSOLETE -- In MuonSelector
    float slope = isMC ? MUON_PTCONE30_SLOPE_MC : MUON_PTCONE30_SLOPE_DATA;
    float ptcone = mu->ptcone30 - slope*nVtx;
    if (removeLeps) {
        for (uint iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e = baseElectrons[iEl];
            if (!isSemiSignalElectron(e)) continue;
            float dR = mu->DeltaR(*e);
            if (dR < 0.3) ptcone -= e->trackPt;
        }
        for (uint iMu = 0; iMu < baseMuons.size(); iMu++) {
            const Muon* mu2 = baseMuons[iMu];
            if (mu == mu2) continue;
            if (!isSemiSignalMuon(mu2)) continue;
            float dR = mu->DeltaR(*mu2);
            if (dR < 0.3) ptcone -= mu2->idTrackPt;
        }
    }
    return ptcone;
*/
    return -999;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muEtConeCorr(const Muon* mu,
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons,
                                uint nVtx, bool isMC, bool removeLeps)
{
/*
  //AT OBSOLETE -- In MuonSelector
    float k1 = isMC ? MUON_ETCONE30_K1_MC : MUON_ETCONE30_K1_DATA;
    float k2 = isMC ? MUON_ETCONE30_K2_MC : MUON_ETCONE30_K2_DATA;
    float etcone = mu->etcone30 - k1*nVtx - k2*nVtx*nVtx;
    if (removeLeps) {
        for (uint iEl = 0; iEl < baseElectrons.size(); iEl++) {
            const Electron* e = baseElectrons[iEl];
            if (!isSemiSignalElectron(e)) continue;
            float dR = mu->DeltaR(*e);
            if (dR < 0.28) etcone -= e->clusE / cosh(e->clusEta);
        }
    }
    return etcone;
*/
    return -999;
}

/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCLJets(const JetVector& jets)
{
    return m_jetSelector.count_CL_jets(jets);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCBJets(const JetVector& jets)
{
    return m_jetSelector.count_CB_jets(jets);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfFJets(const JetVector& jets)
{
    return m_jetSelector.count_F_jets(jets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getNumberOf2LepJets(const JetVector& jets, int& Ncl, int& Ncb, int& Nf,
                                      SusyNtSys sys, AnalysisType anaType)
{
    cout<<"SusyNtTools::getNumberOf2LepJets: this function is obsolete"<<endl
        <<"and it will be soon removed (DG 2015-04)"<<endl
        <<"use JetSelector after setting the 2l analysis type."<<endl;
    return;
}
/*--------------------------------------------------------------------------------*/
// Methods to handle signal region cuts
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(ElectronVector& elecs, float MllCut)
{
    uint nEle = elecs.size();
    if (nEle < 2) return;

    // Now removing all combinations of SFOS pairs with mll < cut
    ElectronVector electronsPass;

    // Use a double loop to check all combinatorics
    for (uint i = 0; i < nEle; i++) {
        Electron* e1 = elecs[i];
        bool pass = true;
        for (uint j = 0; j < nEle; j++) {
            if (i == j) continue;
            Electron* e2 = elecs[j];
            if (isSFOS(e1, e2) && Mll(e1, e2) < MllCut) {
                pass = false;
                break;
            }
        }
        if (pass) electronsPass.push_back(e1);
    }

    // Now replace the supplied vector with this cleaned vector
    elecs = electronsPass;
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(MuonVector& muons, float MllCut)
{
    uint nMu = muons.size();
    if (nMu < 2) return;

    // Now removing all combinations of SFOS pairs with mll < cut
    MuonVector muonsPass;

    // Use a double loop to check all combinatorics
    for (uint i = 0; i < nMu; i++) {
        Muon* m1 = muons[i];
        bool pass = true;
        for (uint j = 0; j < nMu; j++) {
            if (i == j) continue;
            Muon* m2 = muons[j];
            if (isSFOS(m1, m2) && Mll(m1, m2) < MllCut) {
                pass = false;
                break;
            }
        }
        if (pass) muonsPass.push_back(m1);
    }

    // Now replace the supplied vector with this cleaned vector
    muons = muonsPass;
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(TauVector& taus, float MllCut)
{
    uint nTau = taus.size();
    if (nTau < 2) return;

    // Now removing all combinations of SFOS pairs with mll < cut
    TauVector tausPass;

    // Use a double loop to check all combinatorics
    for (uint i = 0; i < nTau; i++) {
        Tau* t1 = taus[i];
        bool pass = true;
        for (uint j = 0; j < nTau; j++) {
            if (i == j) continue;
            Tau* t2 = taus[j];
            if (isOppSign(t1, t2) && Mll(t1, t2) < MllCut) {
                pass = false;
                break;
            }
        }
        if (pass) tausPass.push_back(t1);
    }

    // Now replace the supplied vector with this cleaned vector
    taus = tausPass;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::eventHasSusyPropagators(const std::vector< int > &pdgs,
                                          const std::vector< std::vector< int > > &parentIndices)
{
    // Loop over mc_* particles, find the C1 that is not a self-copy,
    // look at the mother and accept event only if it's Z0/gamma/W+-.
    // Code is not optmized in any meaningful way, feel free to do so
    // if you wish. Also please report any unwanted behavior to
    // amete@cern.ch
    //bool passDiagram = true;
    const int  kPgam(+22), kPz(+23), kPw(+24), kPchargino1(1000024);
    size_t nParticles(pdgs.size());
    for (size_t ii = 0; ii < nParticles; ++ii) {
        int pdg(TMath::Abs(pdgs.at(ii)));
        //if(passDiagram && pdg==kPchargino1)
        if (pdg == kPchargino1) {
            for (unsigned int jj = 0; jj < parentIndices.at(ii).size(); ++jj) {
                int parenPdg(TMath::Abs(pdgs.at(parentIndices.at(ii).at(jj))));
                if (parenPdg == kPchargino1) break; // Self-copy
                if (parenPdg != kPgam && parenPdg != kPz && parenPdg != kPw) {
                    return true;
                    //passDiagram=false; 
                    //break; 
                }
            } // end of parent loop
        }
    } // end of truth particle loop

    return false;
    //bool involvesSusyProp(!passDiagram);
    //return involvesSusyProp;
}
/*--------------------------------------------------------------------------------*/
// Event cleaning cut flags
/*--------------------------------------------------------------------------------*/
int SusyNtTools::cleaningCutFlags(int ntCutFlag,
                                  const MuonVector& preMuons, const MuonVector& baseMuons,
                                  const JetVector& preJets, const JetVector& baseJets)
{
    int cutMask = ECut_GRL | ECut_LarErr | ECut_TTC | ECut_GoodVtx | ECut_TileTrip;
    int cutFlags = ntCutFlag & cutMask;
    if (!hasBadMuon(preMuons))     cutFlags |= ECut_BadMuon;
    if (!hasCosmicMuon(baseMuons)) cutFlags |= ECut_Cosmic;
    if (!hasHotSpotJet(preJets))   cutFlags |= ECut_HotSpot;
    if (!hasBadJet(baseJets))      cutFlags |= ECut_BadJet;
    //cout << "cutFlags " << cutFlags << endl;
    return cutFlags;
}
/*--------------------------------------------------------------------------------*/
// Object level cleaning cut flag methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBadMuon(const MuonVector& preMuons)
{
    uint nMu = preMuons.size();
    for (uint i = 0; i < nMu; i++) {
        if (preMuons[i]->isBadMuon) return true;
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasCosmicMuon(const MuonVector& baseMuons)
{
    uint nMu = baseMuons.size();
    for (uint i = 0; i < nMu; i++) {
        if (baseMuons[i]->isCosmic) return true;
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasHotSpotJet(const JetVector& preJets)
{
    uint nJet = preJets.size();
    for (uint i = 0; i < nJet; i++) {
        if (preJets[i]->isHotTile) return true;
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBadJet(const JetVector& baseJets)
{
    uint nJet = baseJets.size();
    for (uint i = 0; i < nJet; i++) {
        if (baseJets[i]->isBadVeryLoose) return true;
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
// Pass Dead Region based on met, jets and run number
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passDeadRegions(const JetVector& preJets, const Met* met, int RunNumber, bool isMC)
{
    // Info taken from here:
    // https://indico.cern.ch/getFile.py/access?contribId=9&resId=0&materialId=slides&confId=223778
    // Loop over all selected jets with Pt > 30 GeV and BCH_Corr_JET > 0.05
    // If dPhi(met, jet) < 0.3, reject event
    // This is only done run number > 213863, when FEB turned off in HEC, and for all MC

    if (!(RunNumber > 213863 || isMC)) return true;

    for (uint ij = 0; ij < preJets.size(); ++ij) {
        const Jet* jet = preJets.at(ij);
        if (!(jet->Pt() > 40.))          continue;
        if (!(jet->bch_corr_jet > 0.05)) continue;

        if (fabs(jet->DeltaPhi(met->lv())) < 0.3) return false;
    }

    return true;
}

/*--------------------------------------------------------------------------------*/
// Lepton flavor methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSameFlav(const Lepton* l1, const Lepton* l2)
{
    return l1->isEle() == l2->isEle() && l1->isMu() == l2->isMu();
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Lepton* l1, const Lepton* l2)
{
    return l1->q*l2->q < 0;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Tau* tau1, const Tau* tau2)
{
    return tau1->q*tau2->q < 0;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Lepton* lep, const Tau* tau)
{
    return lep->q*tau->q < 0;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFOS(const Lepton* l1, const Lepton* l2)
{
    return isSameFlav(l1, l2) && isOppSign(l1, l2);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFSS(const Lepton* l1, const Lepton* l2)
{
    return isSameFlav(l1, l2) && !isOppSign(l1, l2);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOFOS(const Lepton* l1, const Lepton* l2)
{
    return !isSameFlav(l1, l2) && isOppSign(l1, l2);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSFOS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (isSFOS(leps[i], leps[j])) return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSFSS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (isSFSS(leps[i], leps[j])) return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOFOS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (isOFOS(leps[i], leps[j])) return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (isOppSign(leps[i], leps[j])) return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (!isOppSign(leps[i], leps[j])) return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOS(const TauVector& taus)
{
    uint nTau = taus.size();
    for (uint i = 0; i < nTau; i++) {
        for (uint j = i + 1; j < nTau; j++) {
            if (isOppSign(taus[i], taus[j])) return true;
        }
    }
    return false;
}

/*--------------------------------------------------------------------------------*/
// Mass calculation methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mll(const Particle* l1, const Particle* l2)
{
    return (*l1 + *l2).M();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
{
    return (*l1 + *l2 + *l3).M();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mllll(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4)
{
    return (*l1 + *l2 + *l3 + *l4).M();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mjj(const Jet* j1, const Jet* j2)
{
    return (*j1 + *j2).M();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlljj(const Lepton* l1, const Lepton* l2, const Jet* j1, const Jet* j2)
{
    return (*l1 + *l2 + *j1 + *j2).M();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mt(const TLorentzVector* lep, const Met* met)
{
    return sqrt(2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))));
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const JetVector& jets, const Met* met, float jetPtCut)
{
    float meff = 0;
    for (uint i = 0; i < jets.size(); i++) {
        if (jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if (met) meff += met->Et;
    return meff;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const JetVector& jets, const Met* met, float jetPtCut)
{
    float meff = 0;
    for (uint i = 0; i < leps.size(); i++) meff += leps[i]->Pt();
    for (uint i = 0; i < jets.size(); i++) {
        if (jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if (met) meff += met->Et;
    return meff;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met* met, float jetPtCut)
{
    float meff = 0;
    for (uint i = 0; i < leps.size(); i++) meff += leps[i]->Pt();
    for (uint i = 0; i < taus.size(); i++) meff += taus[i]->Pt();
    for (uint i = 0; i < jets.size(); i++) {
        if (jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if (met) meff += met->Et;
    return meff;
}

/*--------------------------------------------------------------------------------*/
// Z selection methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{
    return isSFOS(l1, l2) && fabs(Mll(l1, l2) - MZ) < massWindow;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, float massWindow)
{
    return (isSFOS(l1, l2) || isSFOS(l1, l3) || isSFOS(l2, l3)) && fabs(Mlll(l1, l2, l3) - MZ) < massWindow;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4, float massWindow)
{
    // Require 2 SFOS pairs
    if (((isSFOS(l1, l2) && isSFOS(l3, l4)) ||
        (isSFOS(l1, l3) && isSFOS(l2, l4)) ||
        (isSFOS(l1, l4) && isSFOS(l2, l3))) &&
        fabs(Mllll(l1, l2, l3, l4) - MZ) < massWindow) return true;
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
{
    float mll = Mll(l1, l2);
    return (isSFOS(l1, l2) && mll > minMll && mll < maxMll);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps, float massWindow, bool useMultiLep)
{
    uint dummy;
    return hasZ(leps, &dummy, &dummy, massWindow, useMultiLep);
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps,
                       uint* Zl2, uint* Zl1,
                       float massWindow, bool useMultiLep)
{
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            // check for Z->ll
            if (isZ(leps[i], leps[j], massWindow)) {
                *Zl1 = i;
                *Zl2 = j;
                return true;
            }
            if (useMultiLep) {
                for (uint k = j + 1; k < nLep; k++) {
                    // check for Z->lll(l)
                    if (isZ(leps[i], leps[j], leps[k], massWindow)) return true;
                    for (uint l = k + 1; l < nLep; l++) {
                        // check for Z->llll
                        if (isZ(leps[i], leps[j], leps[k], leps[l], massWindow)) return true;
                    }
                }
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZlll(const LeptonVector& leps, float massWindow)
{
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            for (uint k = j + 1; k < leps.size(); k++) {
                if (isZ(leps[i], leps[j], leps[k], massWindow)) return true;
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZllll(const LeptonVector& leps, float massWindow)
{
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            for (uint k = j + 1; k < leps.size(); k++) {
                for (uint l = k + 1; l < leps.size(); l++) {
                    if (isZ(leps[i], leps[j], leps[k], leps[l], massWindow)) return true;
                }
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZllZll(const LeptonVector& leps,
                            uint* Z1l1, uint* Z1l2, uint* Z2l1, uint* Z2l2,
                            float massWindow)
{

    // Find first pair
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            if (isZ(leps[i], leps[j], massWindow)) {
                *Z1l1 = i;
                *Z1l2 = j;
                // Find second pair
                for (uint k = 0; k < leps.size(); k++) {
                    if (k == i || k == j) continue;
                    for (uint l = k + 1; l < leps.size(); l++) {
                        if (l == i || l == j) continue;
                        if (isZ(leps[k], leps[l], massWindow)) {
                            *Z2l1 = k;
                            *Z2l2 = l;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZWindow(const LeptonVector& leps, float minMll, float maxMll)
{
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            if (isZWindow(leps[i], leps[j], minMll, maxMll)) return true;
        }
    }
    return false;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the pair closest to the Z mass
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestZ(uint& l1, uint& l2, const LeptonVector& leps)
{
    float minDM = -1;
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (!isSFOS(leps[i], leps[j])) continue;
            float dM = fabs(Mll(leps[i], leps[j]) - MZ);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                l1 = i;
                l2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestZ(uint& j1, uint& j2, const JetVector& jets)
{
    float minDM = -1;
    uint nJet = jets.size();
    for (uint i = 0; i < nJet; i++) {
        for (uint j = i + 1; j < nJet; j++) {
            float dM = fabs(Mjj(jets[i], jets[j]) - MZ);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                j1 = i;
                j2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the jet pair closest to the W mass
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestW(uint& j1, uint& j2, const JetVector& jets)
{
    float minDM = -1;
    uint nJet = jets.size();
    for (uint i = 0; i < nJet; i++) {
        for (uint j = i + 1; j < nJet; j++) {
            float dM = fabs(Mjj(jets[i], jets[j]) - MZ);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                j1 = i;
                j2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}

/*--------------------------------------------------------------------------------*/
int SusyNtTools::numBJets(const JetVector& jets)
{
    return m_jetSelector.JetSelector::count_CB_jets(jets);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBJet(const JetVector& jets)
{
    return numBJets(jets)>0;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBJets(const JetVector& jets)
{
    JetVector bJets;
    for(auto jet : jets) {
        if (m_jetSelector.isCentralBJet(jet))
            bJets.push_back(jet);
    }
    return bJets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBTagSFJets2Lep(const JetVector& baseJets)
{
    cout<<"SusyNtTools::getBTagSFJets2Lep: obsolete, use jet selector instead"<<endl;
    JetVector bTagSFJets;
    for (uint i = 0; i<baseJets.size(); i++) {
        Jet* jet = baseJets[i];
        if (jet->Pt() > JetSelector::JET_PT_B20_CUT && fabs(jet->detEta) < JetSelector::JET_ETA_CUT_2L) {
            bTagSFJets.push_back(jet);
        }
    }
    return bTagSFJets;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::bTagSF(const Event* evt, const JetVector& jets, int mcID, BTagSys sys)
{
    return 1.0;
    if (!evt->isMC) return 1;
}

/*--------------------------------------------------------------------------------*/
// MissingET Rel
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets)
{
    const TLorentzVector metLV = met->lv();
    float dPhi = TMath::Pi() / 2.;

    for (uint il = 0; il < leptons.size(); ++il)
        if (fabs(metLV.DeltaPhi(*leptons.at(il))) < dPhi)
            dPhi = fabs(metLV.DeltaPhi(*leptons.at(il)));
    for (uint ij = 0; ij < jets.size(); ++ij) {
        const Jet* jet = jets.at(ij);
        if (fabs(metLV.DeltaPhi(*jet)) < dPhi)
            dPhi = fabs(metLV.DeltaPhi(*jet));
    }// end loop over jets
    return metLV.Et() * sin(dPhi);
}

/*--------------------------------------------------------------------------------*/
// Calculate MT2
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const LeptonVector& leptons, const Met* met)
{
    if (leptons.size() < 2) return -999;

    // necessary variables
    TLorentzVector metlv = met->lv();
    TLorentzVector l0 = *leptons.at(0);
    TLorentzVector l1 = *leptons.at(1);

    double pTMiss[3] = { 0.0, metlv.Px(), metlv.Py() };
    double pA[3] = { 0.0, l0.Px(), l0.Py() };
    double pB[3] = { 0.0, l1.Px(), l1.Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(0); // LSP mass = 0 is Generic

    return mt2_event.get_mt2();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const TLorentzVector* lep1, const TLorentzVector* lep2, const Met* met)
{
    // necessary variables
    TLorentzVector metLV = met->lv();

    double pTMiss[3] = { 0.0, metLV.Px(), metLV.Py() };
    double pA[3] = { 0.0, lep1->Px(), lep1->Py() };
    double pB[3] = { 0.0, lep2->Px(), lep2->Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(0); // LSP mass = 0 is Generic

    return mt2_event.get_mt2();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met,
                          bool zeroMass, float lspMass)
{
    // necessary variables
    TLorentzVector metLV = met->lv();

    double pTMiss[3] = { 0.0, metLV.Px(), metLV.Py() };
    double pA[3] = { (zeroMass) ? 0.0 : p1->M(), p1->Px(), p1->Py() };
    double pB[3] = { (zeroMass) ? 0.0 : p2->M(), p2->Px(), p2->Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(lspMass);

    return mt2_event.get_mt2();

}

/*--------------------------------------------------------------------------------*/
// Calculate HT
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getHT(const JetVector& jets)
{
    float ht = 0;
    for (uint i = 0; i < jets.size(); i++) {
        float pt = jets[i]->Pt();
        if (pt > 40) ht += pt;
    }
    return ht;
}

/*--------------------------------------------------------------------------------*/
// Calculate HT
/*--------------------------------------------------------------------------------*/
float SusyNtTools::mljj(const LeptonVector& leptons, const JetVector& jets)
{
    if (jets.size() < 1 || leptons.size() < 2) return -999;

    const Lepton& l0 = *leptons.at(0);
    const Lepton& l1 = *leptons.at(1);
    TLorentzVector jj(*jets.at(0));
    if (jets.size() >= 2) jj += *jets.at(1);
    //TLorentzVector l0 = *leptons.at(0);
    //TLorentzVector l1 = *leptons.at(1);
    //TLorentzVector j0 = *jets.at(0);
    //TLorentzVector j1; //initilized to (0,0,0,0)
    //if(jets.size()>=2) j1 = *jets.at(1);
    //TLorentzVector jj = j0+j1;

    float dR1 = jj.DeltaR(l0);
    float dR2 = jj.DeltaR(l1);

    return (dR1 < dR2) ? (jj + l0).M() : (jj + l1).M();
}


/*--------------------------------------------------------------------------------*/
// Calculate transverse thrust
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getThrT(const LeptonVector& leptons)
{
    float sumPx = 0;
    float sumPy = 0;
    float sumPt = 0;
    float max = 0;

    for (uint i = 0; i < leptons.size(); i++) {
        sumPx += leptons[i]->Px();
        sumPy += leptons[i]->Py();
        sumPt += leptons[i]->Pt();
    }
    float nx = sumPx / sqrt(sumPx*sumPx + sumPy*sumPy);
    float ny = sumPy / sqrt(sumPx*sumPx + sumPy*sumPy);

    for (uint i = 0; i < leptons.size(); i++) {
        max += fabs(leptons[i]->Px()*nx + leptons[i]->Py()*ny);
    }
    return max / sumPt;
}

/*--------------------------------------------------------------------------------*/
// Top Tagging methods borrowed from SUSYTools. Yes this is duplication of code...
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met,
                             int opt, float ptJetCut, float mEffCut)
{
    // Defaults opt = 0, ptJetCut = 0 GeV, mEffCut = 100 GeV
    // Tagging is based on number of jets and at least two leptons
    if (leptons.size() < 2) return false;

    // Define some necessary vars
    uint njets = jets.size();
    uint njetscan = njets < 3 ? njets : 3; // Only consider up to 3 jets 
    TLorentzVector v1l = *leptons.at(0);
    TLorentzVector v2l = *leptons.at(1);
    double phi = met->phi;
    double Et = met->Et;
    TVector2 metv2(Et * cos(phi), Et * sin(phi));

    // Check to see if our leptons and met is
    // consistent with a W candidate
    bool maybeW = toptag0j(v1l, v2l, metv2);
    if (!maybeW)            return true;
    if (jets.size() < 2)  return true;

    // Check to see if we have top
    int npairs = 0;
    for (uint i1 = 0; i1 < njetscan - 1; ++i1) {
        if (i1 == 0 || opt == 0) { // opt=0 implies compare all jets
            for (uint i2 = i1 + 1; i2 < njetscan; ++i2) {
                TLorentzVector v1j = *jets.at(i1);
                TLorentzVector v2j = *jets.at(i2);
                bool maybeTop = toptag2j(ptJetCut, mEffCut, v1l, v2l, v1j, v2j, metv2, opt);
                if (maybeTop) npairs++;
            }// end loop over second jet
        }// end if options
    }// end loop over first jet

    if (npairs > 0) return false;
    return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met)
{
    // Taken from TopTag.cxx in SUSYTools

    static const Double_t wmass = 80.4;
    double metx = met.X();
    double mety = met.Y();

    // First mct(ll)
    //---------------
    double mctll = calcMCT(v1l, v2l);

    double pxus = v1l.Px() + v2l.Px() + metx;
    double pyus = v1l.Py() + v2l.Py() + mety;
    double rr = sqrt(pxus*pxus + pyus*pyus) / (2.*wmass);
    double fact = rr + sqrt(1 + rr*rr);

    //cout << "mctll " << mctll << " cut value " << wmass*fact << std::endl;
    return mctll < wmass*fact ? true : false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag2j(double ptjcut, double meffcut,
                           TLorentzVector v1l, TLorentzVector v2l,
                           TLorentzVector v1j, TLorentzVector v2j,
                           TVector2 met, int iopt)
{
    // Taken from TopTag.cxx in SUSYTools

    //
    const double tmass = 172.5;
    const double mljcut = 155.;
    const double mctjcut = 137.;
    //
    TLorentzVector vjb[2];
    double ml1j[2];
    double ml2j[2];
    //
    double metx = met.X();  double mety = met.Y();
    double ptl1 = v1l.Pt();   double ptl2 = v2l.Pt();
    double ptj1 = v1j.Pt();   double ptj2 = v2j.Pt();
    double pxus = v1l.Px() + v2l.Px() + metx;
    double pyus = v1l.Py() + v2l.Py() + mety;
    double mefftop = ptj1 + ptj2 + ptl1 + ptl2;

    if (ptj2 < ptjcut) return false;
    if (iopt == 0 && mefftop < meffcut) return false;

    // First part:
    //-------------
    vjb[0] = v1j;
    vjb[1] = v2j;
    double mctjj = calcMCT(vjb[0], vjb[1]);
    double pxusjl = vjb[0].Px() + vjb[1].Px() + pxus;
    double pyusjl = vjb[0].Py() + vjb[1].Py() + pyus;
    double rrj = sqrt(pxusjl*pxusjl + pyusjl*pyusjl) / (2.*tmass);
    double factj = rrj + sqrt(1 + rrj*rrj);

    bool imct = mctjj < mctjcut*factj ? true : false;
    if (iopt == 1) return imct;

    // Second part:
    //--------------
    for (int i = 0; i < 2; ++i) {
        ml1j[i] = (v1l + vjb[i]).M();
        ml2j[i] = (v2l + vjb[i]).M();
    }
    int ncou = 0;
    int icou1[2];
    int icou2[2];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (i != j) {
                if (ml1j[i] < mljcut && ml2j[j] < mljcut) {
                    icou1[ncou] = i;
                    icou2[ncou] = j;
                    ncou++;
                }
            }
        }
    }
    bool imjl = ncou > 0 ? true : false;

    // Third part: finally mct(ql,ql) for each coupling passing the mjl cut
    //-------------exploit the dependence of mct((jl),(jl)) on the maximum
    //             mass^2 of the two jl combinations 
    int ngcou = 0;
    for (int i = 0; i<ncou; ++i) {
        int ij1 = icou1[i];
        int ij2 = icou2[i];
        TLorentzVector vtot1 = vjb[ij1] + v1l;
        TLorentzVector vtot2 = vjb[ij2] + v2l;
        double mctjl = calcMCT(vtot1, vtot2);
        double mm1 = (vjb[ij1] + v1l).M();
        double mm2 = (vjb[ij2] + v2l).M();
        double mmax2 = mm1>mm2 ? (mm1*mm1) : (mm2*mm2);
        double upl = mmax2 / (tmass)+(tmass);
        //cout << " i " << " mctjl " << mctjl << " mmax2 " << mmax2 << 
        //" upl " << upl << endl;
        if (mctjl < upl*factj)  ngcou++;
    }

    bool imctjl = ngcou > 0 ? true : false;
    //cout << " ll " << imctll << " ct " << imct << " jl " << imjl <<
    //" ctlj " << imctjl << endl;

    return imct & imjl & imctjl;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::calcMCT(TLorentzVector v1, TLorentzVector v2)
{
    float mct = (v1.Mt() + v2.Mt())*(v1.Mt() + v2.Mt()) - (v1 - v2).Perp2();
    mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);
    return mct;
}

/*--------------------------------------------------------------------------------*/
void SusyNtTools::superRazor(const LeptonVector& leptons, const Susy::Met* met,
                             TVector3& vBETA_z, TVector3& pT_CM,
                             TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                             double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                             double& gamma_R, double&  dphi_vBETA_R_vBETA_T,
                             double& MDELTAR, double& costhetaRp1)
{
    // MDR CALCULATION 
    //
    // Code written by Christopher Rogan <crogan@cern.ch>, 04-23-13
    // Details given in paper (http://arxiv.org/abs/1310.4827) written by 
    // Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
    //
    if (leptons.size() < 2) return;

    // necessary variables
    TLorentzVector metlv = met->lv();
    TLorentzVector l0 = *leptons.at(0);
    TLorentzVector l1 = *leptons.at(1);

    //
    // Lab frame
    //
    //Longitudinal boost
    vBETA_z = (1. / (l0.E() + l1.E()))*(l0 + l1).Vect();
    vBETA_z.SetX(0.0);
    vBETA_z.SetY(0.0);

    l0.Boost(-vBETA_z);
    l1.Boost(-vBETA_z);

    //pT of CM frame
    pT_CM = (l0 + l1).Vect() + metlv.Vect();
    pT_CM.SetZ(0.0);

    TLorentzVector ll = l0 + l1;
    //invariant mass of the total event
    SHATR = sqrt(2.*(ll.E()*ll.E() - ll.Vect().Dot(pT_CM)
        + ll.E()*sqrt(ll.E()*ll.E() + pT_CM.Mag2() - 2.*ll.Vect().Dot(pT_CM))));

    vBETA_T_CMtoR = (1. / sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;

    l0.Boost(-vBETA_T_CMtoR);
    l1.Boost(-vBETA_T_CMtoR);
    ll.Boost(-vBETA_T_CMtoR);

    //
    //R-frame
    //
    dphi_LL_vBETA_T = fabs((ll.Vect()).DeltaPhi(vBETA_T_CMtoR));

    dphi_L1_L2 = fabs(l0.Vect().DeltaPhi(l1.Vect()));

    vBETA_R = (1. / (l0.E() + l1.E()))*(l0.Vect() - l1.Vect());

    gamma_R = 1. / sqrt(1. - vBETA_R.Mag2());

    dphi_vBETA_R_vBETA_T = fabs(vBETA_R.DeltaPhi(vBETA_T_CMtoR));

    l0.Boost(-vBETA_R);
    l1.Boost(vBETA_R);

    //
    //R+1 frame
    //
    MDELTAR = 2.*l0.E();
    costhetaRp1 = l0.Vect().Dot(vBETA_R) / (l0.Vect().Mag()*vBETA_R.Mag());

    return;
}
/*--------------------------------------------------------------------------------*/
// Check if mcID is a known sherpa sample
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSherpaSample(unsigned int mcID)
{
    // Sherpa ttbar dilep and semilep
    if (mcID >= 117800 && mcID <= 117809) return true;
    // Sherpa W/Z gamma
    if (mcID == 126739 || mcID == 126742 || mcID == 126854 || mcID == 126856 || mcID == 145161 || mcID == 145162) return true;
    // Sherpa diboson
    if (mcID >= 126892 && mcID <= 126895) return true;
    if (mcID == 126989) return true;
    // Z + LF/HF
    if (mcID >= 128975 && mcID <= 128977) return true;
    if (mcID >= 146820 && mcID <= 146822) return true;
    // W/Z inclusive
    if (mcID >= 147770 && mcID <= 147776) return true;

    return false;
}
//----------------------------------------------------------
