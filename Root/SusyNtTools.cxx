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
#include "SusyNtuple/KinematicTools.h"

#include <cassert>

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() :
    m_electronSelector(nullptr),
    m_muonSelector(nullptr),
    m_tauSelector(nullptr),
    m_jetSelector(nullptr),
    m_overlapTool(nullptr),
    m_anaType(AnalysisType::kUnknown),
    m_doSFOS(false)
{
}
//----------------------------------------------------------
SusyNtTools::~SusyNtTools()
{
    if(m_electronSelector) { delete m_electronSelector; m_electronSelector = nullptr; }
    if(m_muonSelector) { delete m_muonSelector; m_muonSelector = nullptr; }
    if(m_jetSelector) { delete m_jetSelector; m_jetSelector = nullptr; }
    if(m_overlapTool) {delete m_overlapTool; m_overlapTool = nullptr; }
}
//----------------------------------------------------------
void SusyNtTools::setAnaType(AnalysisType a, bool verbose)
{
    if(m_electronSelector) delete m_electronSelector;
    m_electronSelector = ElectronSelector::build(a, verbose);

    if(m_muonSelector) delete m_muonSelector;
    m_muonSelector = MuonSelector::build(a, verbose);
    if(m_jetSelector) delete m_jetSelector;
    m_jetSelector = JetSelector::build(a, verbose);

    if(m_tauSelector) delete m_tauSelector;
    m_tauSelector = TauSelector::build(a, verbose);

    if(m_overlapTool) delete m_overlapTool;
    m_overlapTool = OverlapTools::build(a, verbose);
    // propagate isolation requirements, needed only for removeNonisolatedLeptons()
    m_overlapTool->setElectronIsolation(electronSelector().signalIsolation());
    m_overlapTool->setMuonIsolation(muonSelector().signalIsolation());

    // set whether to perform SFOS removal on baseline objects
    setSFOSRemoval(a);

    // this should be in the logs no matter what
    cout << ">>> Setting analysis type to " << AnalysisType2str(a) << std::endl;
    // now that the tools are configured set this variable
    m_anaType = a;
}
//----------------------------------------------------------
/*--------------------------------------------------------------------------------*/
// Event selection methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passGRL(int flags)
{
    return (flags & ECut_GRL);
}
bool SusyNtTools::passLarErr(int flags)
{
    return (flags & ECut_LarErr);
}
bool SusyNtTools::passTileErr(int flags)
{
    return (flags & ECut_TileErr);
}
bool SusyNtTools::passTTC(int flags)
{
    return (flags & ECut_TTC);
}
bool SusyNtTools::passGoodVtx(int flags)
{
    return (flags & ECut_GoodVtx);
}
bool SusyNtTools::passBadMuon(const MuonVector& preMuons)
{
    // get the baseline muons prior to OR
    MuonVector baseMuons = SusyNtTools::getBaselineMuons(preMuons); 
    // now check whether any of the baseline muons is bad 
    bool pass_badMuon = true;
    for(uint imu=0; imu<baseMuons.size(); imu++){
        if(baseMuons[imu]->isBadMuon){ pass_badMuon = false; break; }
    } // imu
    return pass_badMuon;
}
bool SusyNtTools::passCosmicMuon(const MuonVector& baseMuons)
{
    // You must pass this function the baseline muons
    // that have already undergone overlap removal!!
    bool pass_cosmicMuon = true;
    for(uint imu=0; imu<baseMuons.size(); imu++){
        if(baseMuons[imu]->isCosmic){ pass_cosmicMuon = false; break; }
    } // imu
    return pass_cosmicMuon;
}
bool SusyNtTools::passJetCleaning(const JetVector& baseJets)
{
    // You must pass this function the 'Pre-Jets', i.e.
    // the jets prior to baseline selection and overlap
    // removal
    bool pass_jetCleaning = true;
    for(uint ijet=0; ijet<baseJets.size(); ijet++){
        if(baseJets[ijet]->isBadVeryLoose){ pass_jetCleaning = false; break; }
    } // ijet
    return pass_jetCleaning;
}
/*--------------------------------------------------------------------------------*/
// Full object selection methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::setSFOSRemoval(AnalysisType a)
{
    if(a == AnalysisType::Ana_2Lep ||
       a == AnalysisType::Ana_3Lep ||
       a == AnalysisType::Ana_2LepWH ) { m_doSFOS = true; }

    else if( a == AnalysisType::Ana_SS3L )   { m_doSFOS = false; }
    else if( a == AnalysisType::Ana_Stop2L ) { m_doSFOS = false; }

    else if( a == AnalysisType::kUnknown ) {
        string error = "SusyNtTools::setSFOSRemoval error: ";
        cout << error << "The AnalysisType has not been set for SusyNtTools. Check that you properly call" << endl;
        cout << error << "'setAnaType' for SusyNtTools for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    else {
        string error = "SusyNtTools::setSFOSRemoval error: ";
        cout << error << "SusyNtTools is not configured for the AnalysisType (" << AnalysisType2str(a) << ") provided in" << endl;
        cout << error << "'setAnaTypee'! Be sure that your AnalyssiType is provided for in SusyNtuple/AnalysisTyp.eh" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnaType'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

}
    
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getPreObjects(SusyNtObject* susyNt, SusyNtSys sys,
        ElectronVector& preElectrons, MuonVector& preMuons, JetVector& preJets, TauVector& preTaus)
{
    preElectrons = getPreElectrons(susyNt, sys);
    preMuons     = getPreMuons(susyNt, sys);
    preJets      = getPreJets(susyNt, sys);
    preTaus      = getPreTaus(susyNt, sys);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(const ElectronVector& preElectrons, const MuonVector& preMuons, const JetVector& preJets, const TauVector& preTaus,
                                     ElectronVector& baseElectrons, MuonVector& baseMuons, JetVector& baseJets, TauVector& baseTaus)
{
    baseElectrons = getBaselineElectrons(preElectrons);
    baseMuons     = getBaselineMuons(preMuons);
    baseJets      = getBaselineJets(preJets);
    baseTaus      = getBaselineTaus(preTaus);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(const ElectronVector& baseElectrons, const MuonVector& baseMuons, const JetVector& baseJets, const TauVector& baseTaus,
                                    ElectronVector& signalElectrons, MuonVector& signalMuons, JetVector& signalJets, TauVector& signalTaus, TauId& sigTauId)
{
    signalElectrons = getSignalElectrons(baseElectrons);
    signalMuons     = getSignalMuons(baseMuons);
    signalTaus      = getSignalTaus(baseTaus);
    signalJets      = getSignalJets(baseJets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::buildLeptons(LeptonVector& leptons, const ElectronVector& electrons, const MuonVector& muons)
{
    for(uint ie = 0; ie < electrons.size(); ie++) {
        leptons.push_back(electrons[ie]);
    }
    for(uint im = 0; im < muons.size(); im++) {
        leptons.push_back(muons[im]);
    }
    // sort by pT
    std::sort(leptons.begin(), leptons.end(), comparePt);
}

/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPairs(ElectronVector& electrons, MuonVector& muons)
{
    float mllMin = 12;
    kin::removeSFOSPair(electrons, mllMin);
    kin::removeSFOSPair(muons, mllMin);
}
//----------------------------------------------------------
bool SusyNtTools::initTriggerTool(const std::string &anyInputFilename)
{
    return m_triggerTool.init(anyInputFilename);
}
//----------------------------------------------------------
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
    // sort by pt
    std::sort(elecs.begin(), elecs.end(), comparePt);

    return elecs;
}
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getBaselineElectrons(const ElectronVector& preElecs)
{
    ElectronVector elecs;
    for (uint ie = 0; ie < preElecs.size(); ++ie) {
        Electron* e = preElecs.at(ie);
        if(electronSelector().isBaseline(e)){
            elecs.push_back(e);
        }
    } // ie
    // sort by pT
    std::sort(elecs.begin(), elecs.end(), comparePt);

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
    // sort by pT
    std::sort(muons.begin(), muons.end(), comparePt);

    return muons;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getBaselineMuons(const MuonVector& preMuons)
{
    MuonVector baseMuons;
    for (uint im = 0; im < preMuons.size(); ++im) {
        Muon* mu = preMuons.at(im);
        if(muonSelector().isBaseline(mu)){
            baseMuons.push_back(mu);
        }
    } // im
    // sort by pT
    std::sort(baseMuons.begin(), baseMuons.end(), comparePt);

    return baseMuons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getBaselineTaus(const TauVector& preTaus)
{
    TauVector baseTaus;
    for (uint iTau = 0; iTau < preTaus.size(); iTau++) {
        Tau* tau = preTaus.at(iTau);
        if(tauSelector().isBaseline(tau)){
            baseTaus.push_back(tau);
        }
    } // iTau
    // sort by pT
    std::sort(baseTaus.begin(), baseTaus.end(), comparePt);

    return baseTaus;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getPreTaus(SusyNtObject* susyNt, SusyNtSys sys)
{
    TauVector taus;
    for (uint iTau = 0; iTau < susyNt->tau()->size(); iTau++) {
        Tau* tau = &susyNt->tau()->at(iTau);
        tau->setState(sys);
        taus.push_back(tau);
    }
    // sort by pT
    std::sort(taus.begin(), taus.end(), comparePt);

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
    // sort by pT
    std::sort(jets.begin(), jets.end(), comparePt);

    return jets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBaselineJets(const JetVector& preJets)
{
    JetVector baseJets;
    for (uint ij = 0; ij < preJets.size(); ++ij) {
        Jet* j = preJets.at(ij);
        if(jetSelector().isBaseline(j)) {
            baseJets.push_back(j);
        }
    } // ij
    // sort by pT 
    std::sort(baseJets.begin(), baseJets.end(), comparePt);

    return baseJets;
}
/*--------------------------------------------------------------------------------*/
// Get Signal objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getSignalElectrons(const ElectronVector& baseElecs)
{
    ElectronVector sigElecs;
    for (uint ie = 0; ie < baseElecs.size(); ++ie) {
        Electron* e = baseElecs.at(ie);
        if (electronSelector().isSignal(e)){
            sigElecs.push_back(e);
        }
    }
    // sort by pT
    std::sort(sigElecs.begin(), sigElecs.end(), comparePt);

    return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(const MuonVector& baseMuons)
{
    MuonVector sigMuons;
    for (uint im = 0; im < baseMuons.size(); ++im) {
        Muon* mu = baseMuons.at(im);
        if (muonSelector().isSignal(mu)){
            sigMuons.push_back(mu);
        }
    }
    // sort by pT
    std::sort(sigMuons.begin(), sigMuons.end(), comparePt);
    
    return sigMuons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getSignalTaus(const TauVector& baseTaus)
{
    TauVector sigTaus;
    for (uint iTau = 0; iTau < baseTaus.size(); iTau++) {
        Tau* tau = baseTaus[iTau];

        if(tauSelector().isSignal(tau)) {
            sigTaus.push_back(tau);
        }
    } // iTau

    // sort by pT 
    std::sort(sigTaus.begin(), sigTaus.end(), comparePt);

    return sigTaus;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(const JetVector& baseJets)
{
    JetVector sigJets;
    for(uint ij=0; ij<baseJets.size(); ++ij){
        Jet* j = baseJets.at(ij);
        if(jetSelector().isSignal(j)) {
            sigJets.push_back(j);
        }
    }
    // sort by pT
    std::sort(sigJets.begin(), sigJets.end(), comparePt);

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
// Check if lepton is selected
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignal(const Lepton* l)
{
    if(l->isEle()) return electronSelector().isSignal((Electron*)l);
    else           return muonSelector().isSignal((Muon*)l);
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignal(const Electron* e)
{
    return electronSelector().isSignal(e);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignal(const Muon* m)
{
    return muonSelector().isSignal(m);
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignal(const Tau* tau)
{
    return tauSelector().isSignal(tau);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCLJets(const JetVector& jets)
{
    return jetSelector().count_CL_jets(jets);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCBJets(const JetVector& jets)
{
    return jetSelector().count_CB_jets(jets);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfFJets(const JetVector& jets)
{
    return jetSelector().count_F_jets(jets);
}
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numBJets(const JetVector& jets)
{
    return jetSelector().count_CB_jets(jets);
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
        if (jetSelector().isCentralB(jet))
            bJets.push_back(jet);
    }
    return bJets;
}

/*--------------------------------------------------------------------------------*/
// Methods to grab object SF
/*--------------------------------------------------------------------------------*/

////////////////////////////
// b-Tag efficiency SF
////////////////////////////
float SusyNtTools::bTagSF(const JetVector& jets)
{
    float sf = 1.0;
    for(uint ij = 0; ij < jets.size(); ij++){
        sf *= jets[ij]->effscalefact;
    } // ij
    return sf;
}

////////////////////////////
// Lepton efficiency SF
////////////////////////////
float SusyNtTools::leptonEffSF(const LeptonVector& leps)
{
    float sf = 1.0;
    for(uint i=0; i< leps.size(); i++){
        sf *= SusyNtTools::leptonEffSF(leps[i]);
    } // i
    return sf;
}

float SusyNtTools::leptonEffSF(const Lepton& lep)
{
    float sf = 1.0;
    if(lep.isEle()){
        sf = electronSelector().effSF((Electron&)lep);
    } else {
        sf = muonSelector().effSF((Muon&)lep);
    }
    return sf;
}
float SusyNtTools::leptonEffSFError(const Lepton& lep, const NtSys::SusyNtSys sys)
{
    float errSF = 0.0;
    if(lep.isEle()){
        errSF = electronSelector().errEffSF((Electron&)lep, sys);
    } else {
        errSF = muonSelector().errEffSF((Muon&)lep, sys);
    }
    return errSF;
}
