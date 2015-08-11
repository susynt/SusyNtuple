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
#include "SusyNtuple/SusyKin.h"

#include <cassert>

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() :
m_anaType(AnalysisType::kUnknown),
m_doMSFOS(false)
{
}
/*--------------------------------------------------------------------------------*/
// Full object selection methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::setMSFOSRemoval(AnalysisType A)
{
    switch(A) {
    case(AnalysisType::Ana_2Lep) :
    case(AnalysisType::Ana_3Lep) :
    case(AnalysisType::Ana_2LepWH) : {
        m_doMSFOS = true;
        break;
    }
    case(AnalysisType::Ana_SS3L) : {
        m_doMSFOS = false;
        break;
    }
    case(AnalysisType::kUnknown) : {
        string error = "SusyNtTools::setMSFOSRemoval error: ";
        cout << error << "Invalid AnalysisType (" << AnalysisType2str(AnalysisType::kUnknown) << ")" << endl;
        cout << error << "Check that 'setAnalysisType' is called properly before calling this method." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    } // switch
}
    
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt,
                                     ElectronVector& preElecs, MuonVector& preMuons, JetVector& preJets,
                                     ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets,
                                     SusyNtSys sys, bool selectTaus)
{
    // Preselection
    preElecs = getPreElectrons(susyNt, sys);
    preMuons = getPreMuons(susyNt, sys);
    preJets  = getPreJets(susyNt, sys);
    if (selectTaus) taus = getPreTaus(susyNt, sys);
    else taus.clear();

    // Baseline objects
    elecs = getBaselineElectrons(preElecs);
    muons = getBaselineMuons(preMuons);
    jets  = getBaselineJets(preJets);

    // Overlap removal
    if(!m_overlapTool.useSignalLeptons()){
        m_overlapTool.performOverlap(elecs, muons, taus, jets);
    }

    // Remove MSFOS < 12 GeV
    if(m_doMSFOS) {
        SusyKin::removeSFOSPair(elecs, MLL_MIN);
        SusyKin::removeSFOSPair(muons, MLL_MIN);
    }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt, ElectronVector& elecs,
                                     MuonVector& muons, TauVector& taus, JetVector& jets,
                                     SusyNtSys sys, bool selectTaus)
{
    // Get all objects
    elecs = getPreElectrons(susyNt, sys);
    muons = getPreMuons(susyNt, sys);
    jets  = getPreJets(susyNt, sys);
    if (selectTaus) taus = getPreTaus(susyNt, sys);
    else taus.clear();

    // Baseline objects
    elecs = getBaselineElectrons(elecs);
    muons = getBaselineMuons(muons);
    jets  = getBaselineJets(jets);

    // Now perform the overlap removals
    if(!m_overlapTool.useSignalLeptons()){
        m_overlapTool.performOverlap(elecs, muons, taus, jets);
    }

    // Do SFOS removal for Mll < 12 
    if(m_doMSFOS) {
        SusyKin::removeSFOSPair(elecs, MLL_MIN);
        SusyKin::removeSFOSPair(muons, MLL_MIN);
        // TODO: revisit this??
        //removeSFOSPair(taus, MLL_MIN);
    }
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
    sigTaus  = getSignalTaus(baseTaus, tauJetID, tauEleID, tauMuoID);
    sigJets  = getSignalJets(baseJets, sys);

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
    sigJets  = getSignalJets(baseJets, sys);

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
        elecs.push_back(e);
    }
    return elecs;
}
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getBaselineElectrons(const ElectronVector& preElecs)
{
    ElectronVector elecs;
    for (uint ie = 0; ie < preElecs.size(); ++ie) {
        Electron* e = preElecs.at(ie);
        if(m_electronSelector.isBaselineElectron(e)){
            elecs.push_back(e);
        }
    } // ie

    return elecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getPreMuons(SusyNtObject* susyNt, SusyNtSys sys)
{
    MuonVector muons;
    for (uint im = 0; im < susyNt->muo()->size(); ++im) {
        Muon* mu = &susyNt->muo()->at(im);
        mu->setState(sys);
        muons.push_back(mu);
    }
    return muons;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getBaselineMuons(const MuonVector& preMuons)
{
    MuonVector baseMuons;
    for (uint im = 0; im < preMuons.size(); ++im) {
        Muon* mu = preMuons.at(im);
        if(m_muonSelector.isBaselineMuon(mu)){
            baseMuons.push_back(mu);
        }
    } // im
    return baseMuons;
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
        jets.push_back(j);
    }

    return jets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBaselineJets(const JetVector& preJets)
{
    JetVector baseJets;
    for (uint ij = 0; ij < preJets.size(); ++ij) {
        Jet* j = preJets.at(ij);
        if(j->Pt() < m_jetSelector.JET_MIN_PT_BASELINE) continue;
        baseJets.push_back(j);
    } // ij
    return baseJets;
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
        if (m_electronSelector.isSignalElectron(e)){ 
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
        if (m_muonSelector.isSignalMuon(mu)){
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
bool SusyNtTools::isSignalLepton(const Lepton* l)
{
    if(l->isEle()) return m_electronSelector.isSignalElectron((Electron*)l);
    else           return m_muonSelector.isSignalMuon((Muon*)l);
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* e)
{
    return m_electronSelector.isSignalElectron(e);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* m)
{
    return m_muonSelector.isSignalMuon(m);
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
    return m_electronSelector.isSemiSignalElectron(ele);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSemiSignalMuon(const Muon* mu)
{
    return m_muonSelector.isSemiSignalMuon(mu);
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
float SusyNtTools::bTagSF(const Event* evt, const JetVector& jets, int mcID, BTagSys sys)
{
    return 1.0;
    if (!evt->isMC) return 1;
}

