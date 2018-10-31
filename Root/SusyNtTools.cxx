#include <set>

#include "TFile.h"
#include "TKey.h"
#include "TChainElement.h"
#include "TH1F.h"
#include "TSystem.h"

#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/TauSelector.h"
#include "SusyNtuple/PhotonSelector.h"
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
    m_photonSelector(nullptr),
    m_overlapTool(nullptr),
    m_anaType(AnalysisType::kUnknown),
    m_doSFOS(false),
    n_warning(0)
{
}
//----------------------------------------------------------
SusyNtTools::~SusyNtTools()
{
    if(m_electronSelector) { delete m_electronSelector; m_electronSelector = nullptr; }
    if(m_muonSelector) { delete m_muonSelector; m_muonSelector = nullptr; }
    if(m_jetSelector) { delete m_jetSelector; m_jetSelector = nullptr; }
    if(m_tauSelector) { delete m_tauSelector; m_tauSelector = nullptr; }
    if(m_photonSelector) { delete m_photonSelector; m_photonSelector = nullptr; }
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

    if(m_photonSelector) delete m_photonSelector;
    m_photonSelector = PhotonSelector::build(a, verbose);

    if(m_overlapTool) delete m_overlapTool;
    m_overlapTool = OverlapTools::build(a, verbose);
    // propagate isolation requirements, needed only for removeNonisolatedLeptons()
    m_overlapTool->setElectronIsolation(electronSelector().signalIsolation());
    m_overlapTool->setMuonIsolation(muonSelector().signalIsolation());
    m_overlapTool->jetSelector(m_jetSelector);

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
bool SusyNtTools::passSCTErr(int flags)
{
    return (flags & ECut_SCTErr);
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
bool SusyNtTools::passMetCleaning(const Met* in)
{
    float met = in->Et;
    float met_phi = in->phi;
    float tst_met = in->softTerm_et;
    float tst_met_phi = in->softTerm_phi;

    if(tst_met < 100) return true;
    if(met>0.)
        if( (tst_met/met) < 0.4 ) return true;
    if(fabs(TVector2::Phi_mpi_pi( tst_met_phi - met_phi) ) > 0.8) return true;
    return false;
}
/*--------------------------------------------------------------------------------*/
// Full object selection methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::setSFOSRemoval(AnalysisType a)
{
    if(a == AnalysisType::Ana_2Lep ||
       a == AnalysisType::Ana_3Lep ||
       a == AnalysisType::Ana_4Lep ||
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
        ElectronVector& preElectrons, MuonVector& preMuons, JetVector& preJets, TauVector& preTaus, PhotonVector& prePhotons)
{
    preElectrons = getPreElectrons(susyNt, sys);
    preMuons     = getPreMuons(susyNt, sys);
    preJets      = getPreJets(susyNt, sys);
    preTaus      = getPreTaus(susyNt, sys);
    prePhotons   = getPrePhotons(susyNt, sys);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(const ElectronVector& preElectrons, const MuonVector& preMuons, const JetVector& preJets, const TauVector& preTaus,
                                        const PhotonVector& prePhotons,
                                     ElectronVector& baseElectrons, MuonVector& baseMuons, JetVector& baseJets, TauVector& baseTaus,
                                        PhotonVector& basePhotons)
{
    baseElectrons = getBaselineElectrons(preElectrons);
    baseMuons     = getBaselineMuons(preMuons);
    baseJets      = getBaselineJets(preJets);
    baseTaus      = getBaselineTaus(preTaus);
    basePhotons   = getBaselinePhotons(prePhotons);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(const ElectronVector& baseElectrons, const MuonVector& baseMuons, const JetVector& baseJets, const TauVector& baseTaus,
                                    const PhotonVector& basePhotons,
                                    ElectronVector& signalElectrons, MuonVector& signalMuons, JetVector& signalJets, TauVector& signalTaus,
                                    PhotonVector& signalPhotons)
{
    signalElectrons = getSignalElectrons(baseElectrons);
    signalMuons     = getSignalMuons(baseMuons);
    signalJets      = getSignalJets(baseJets);
    signalTaus      = getSignalTaus(baseTaus);
    signalPhotons   = getSignalPhotons(basePhotons);
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
PhotonVector SusyNtTools::getBaselinePhotons(const PhotonVector& prePhotons)
{
    PhotonVector basePhotons;
    for(uint iPho = 0; iPho < prePhotons.size(); iPho++){
        Photon* pho = prePhotons.at(iPho);
        if(photonSelector().isBaseline(pho)){
            basePhotons.push_back(pho);
        }
    } // iPho
    // sort by pT
    std::sort(basePhotons.begin(), basePhotons.end(), comparePt);

    return basePhotons;
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
PhotonVector SusyNtTools::getPrePhotons(SusyNtObject* susyNt, SusyNtSys sys)
{
    PhotonVector photons;
    for(uint iPho = 0; iPho < susyNt->pho()->size(); iPho++) {
        Photon* photon = &susyNt->pho()->at(iPho);
        photon->setState(sys);
        photons.push_back(photon);
    }
    // sort by pT
    std::sort(photons.begin(), photons.end(), comparePt);

    return photons;
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
PhotonVector SusyNtTools::getSignalPhotons(const PhotonVector& basePhotons)
{
    PhotonVector sigPhotons;
    for(uint iPho = 0; iPho < basePhotons.size(); iPho++) {
        Photon* pho = basePhotons[iPho];
        if(photonSelector().isSignal(pho)) {
            sigPhotons.push_back(pho);
        }
    } // iPho
    // sort by pT
    std::sort(sigPhotons.begin(), sigPhotons.end(), comparePt);
    return sigPhotons;
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
    float total_sf = 1.0;
    for(auto & jet : jets) {
        float sf = 1.0;
        if(fabs(jet->Eta())>2.5) continue;
        if(jet->Pt()<20.) continue;
        if(jetSelector().isB(jet)) {
            sf = jetSelector().bjet_sf(jet);
        }
        else {
            sf = jetSelector().bjet_sf(jet, true);
        }
        total_sf *= sf;
    }
    return total_sf;
    //float sf = 1.0;
    //for(uint ij = 0; ij < jets.size(); ij++){
    //    sf *= jets[ij]->effscalefact;
    //} // ij
    //return sf;
}

float SusyNtTools::bTagSFError(const JetVector& jets, const NtSys::SusyNtSys sys)
{
    float outSF = 1.0;
    for(auto & jet : jets) {
        float sf = 1.0;
        if(fabs(jet->Eta())>2.5) continue;
        if(jet->Pt()<20.) continue;
        if(jetSelector().isB(jet)) {
            sf = jetSelector().bjet_sf_err(jet, sys, false);
        }
        else {
            sf = jetSelector().bjet_sf_err(jet, sys, true);
        }
        outSF *= sf;
    }
    return outSF;
    //float outSF = 1.0;
    //float delta = 0.0;
    //float sf = 1.0;
    //for(uint ij = 0; ij < jets.size(); ij++) {
    //    delta = jets[ij]->getFTSys(sys);
    //    sf = jets[ij]->effscalefact;
    //    outSF *= (sf + delta);
    //} // ij
    //return outSF;
}

////////////////////////////
// Lepton efficiency SF
////////////////////////////
float SusyNtTools::leptonEffSF(const LeptonVector& leps, const NtSys::SusyNtSys sys)
{
    float sf = 1.0;
    for(uint i = 0; i < leps.size(); i++) {
        sf *= SusyNtTools::leptonEffSF(leps.at(i), sys);
    } // i
    return sf;
}

float SusyNtTools::leptonEffSF(const Lepton* lep, const NtSys::SusyNtSys sys)
{
    return leptonEffSF(*lep, sys);
}

float SusyNtTools::leptonEffSF(const Lepton& lep, const NtSys::SusyNtSys sys)
{
    float sf = 1.0;
    if(lep.isEle()) {
        sf = electronSelector().effSF((Electron&)lep, sys);
    }
    else {
        sf = muonSelector().effSF((Muon&)lep, sys);
    }
    return sf;
}

float SusyNtTools::leptonEffSFError(const Lepton& lep, const NtSys::SusyNtSys sys)
{
    float delta = 0.0;
    if(lep.isEle()){
        delta = electronSelector().errEffSF((Electron&)lep, sys);
    } else {
        delta = muonSelector().errEffSF((Muon&)lep, sys);
    }
    return delta;
}
float SusyNtTools::leptonTriggerSF(const LeptonVector& leptons, std::string trigger,
        const NtSys::SusyNtSys sys)
{
    string hlt = "HLT_";
    bool has_hlt_start = (trigger.find(hlt) != std::string::npos);
    if(!has_hlt_start) {
        cout << "SusyNtTools::leptonTriggerSF    ERROR Triggers must start with 'HLT_', the trigger "
            << "you provided (" << trigger << ") does not contain this -- Exiting." << endl;
        exit(1);
    }
    float trigger_sf = 1.0;

    if(leptons.size()==0) return trigger_sf;

    if(leptons.size() > 2) {
        if(n_warning < 20) {
            cout << "SusyNtTools::leptonTriggerSF    WARNING [" << (n_warning + 1) << "/" << n_warning << "]    Provided more than 2 leptons, we can handle only 1 and 2 leptons for trigger scale factors - returning 1.0" << endl;
            n_warning++;
        }
        return 1.0;
    }

    int n_mu = 0;
    int n_el = 0;

    for(auto & l : leptons) {
        if(l->isEle()) n_el++;
        if(!l->isEle()) n_mu++;
    }

    if( (n_mu>0) && (n_el>0) ) {
        cout << "SusyNtTools::leptonTriggerSF    WARNING we do not handle (yet) mixed (dilepton) trigger scale factors, returning 1.0" << endl;
        return 1.0;
    }

    if(n_mu>0) {
        MuonVector muons;
        for(auto & l : leptons) {
            Muon* m = dynamic_cast<Susy::Muon*>(l);
            muons.push_back(m);
        }
        trigger_sf = get_muon_trigger_scale_factor(muons, trigger, sys);
    }

    else if(n_el>0) {
        ElectronVector electrons;
        for(auto & l : leptons) {
            Electron* e = dynamic_cast<Susy::Electron*>(l);
            electrons.push_back(e);
        }
        trigger_sf = get_electron_trigger_scale_factor(electrons, trigger, sys);
    }
    return trigger_sf;
}
float SusyNtTools::get_muon_trigger_scale_factor(Muon& mu1, Muon& mu2, std::string trigger,
        const SusyNtSys sys)
{
    MuonVector trig_muons;
    trig_muons.push_back(&mu1);
    trig_muons.push_back(&mu2);
    return get_muon_trigger_scale_factor(trig_muons, trigger, sys);
}
float SusyNtTools::get_muon_trigger_scale_factor(Muon& muon, std::string trigger,
        const SusyNtSys sys)
{
    MuonVector trig_muons;
    trig_muons.push_back(&muon);
    return get_muon_trigger_scale_factor(trig_muons, trigger, sys);
}
float SusyNtTools::get_muon_trigger_scale_factor(const MuonVector& muons, std::string trigger,
        const SusyNtSys sys)
{
    float scale_factor = 1.0;
    const vector<string> triggers = TriggerTools::getTrigNames();
    vector<string> muon_triggers;
    vector<string> single_muon_triggers = TriggerTools::single_muo_triggers();
    vector<string> di_muon_triggers = TriggerTools::di_muo_triggers();

    // single muon
    bool is_single = (std::find(single_muon_triggers.begin(), single_muon_triggers.end(), trigger) != single_muon_triggers.end());
    bool is_dimuon = (std::find(di_muon_triggers.begin(), di_muon_triggers.end(), trigger) != di_muon_triggers.end());

    if(!(is_single || is_dimuon)) {
        cout << "SusyNtuple::get_muon_trigger_scale_factor    ERROR Provided trigger " << trigger << " is "
            << "not a supported single or dimuon trigger. Acceptable triggers to provide are "
            << "(c.f. SusyNtuple/TriggerList.h):" << endl;
        for(auto x : single_muon_triggers)
            cout << "SusyNtuple::get_muon_trigger_scale_factor    " << x << endl;
        for(auto x : di_muon_triggers)
            cout << "SusyNtuple::get_muon_trigger_scale_factor    " << x << endl;
        cout << "SusyNtuple::get_muon_trigger_scale_factor    Exiting." << endl;
        exit(1);
    }

    if(is_single) {
        scale_factor = get_single_muon_trigger_scale_factor(muons, muonSelector().signalId(), trigger, sys);
    }
    else if(is_dimuon) {
        scale_factor = get_dimuon_trigger_scale_factor(muons, muonSelector().signalId(), trigger, sys);
    }

    return scale_factor;
}
float SusyNtTools::get_single_muon_trigger_scale_factor(const MuonVector& muons, MuonId id, string trigger,
        const SusyNtSys sys)
{
    int idx = triggerTool().idx_of_trigger(trigger);
    double rate_not_fired_data = 1.0;
    double rate_not_fired_mc = 1.0;
    for(auto & m : muons) {
        double eff_data = 1.0;
        double eff_mc = 1.0;
        if(id == MuonId::Medium) {
            if(sys == NtSys::NOM) {
                eff_data = m->muoTrigEffData_medium[idx];
                eff_mc = m->muoTrigEffMC_medium[idx];
            }
            else if(sys == NtSys::MUON_EFF_TRIG_STAT_UP) {
                eff_data = m->muoTrigEffErrData_stat_up_medium[idx];
                eff_mc = m->muoTrigEffErrMC_stat_up_medium[idx];
            }
            else if(sys == NtSys::MUON_EFF_TRIG_STAT_DN) {
                eff_data = m->muoTrigEffErrData_stat_dn_medium[idx];
                eff_mc = m->muoTrigEffErrMC_stat_dn_medium[idx];
            }
            else if(sys == NtSys::MUON_EFF_TRIG_SYST_UP) {
                eff_data = m->muoTrigEffErrData_syst_up_medium[idx];
                eff_mc = m->muoTrigEffErrMC_syst_up_medium[idx];
            }
            else if(sys == NtSys::MUON_EFF_TRIG_SYST_DN) {
                eff_data = m->muoTrigEffErrData_syst_dn_medium[idx];
                eff_mc = m->muoTrigEffErrMC_syst_dn_medium[idx];
            }
        }
        else if(id == MuonId::Loose) {
            if(sys != NtSys::NOM) {
                cout << "SusyNtTools::get_single_muon_trigger_scale_factor   "
                        << "ERROR We only store trigger SF variations for Medium muons -- Exiting." << endl;
                exit(1);
            }
            eff_data = m->muoTrigEffData_loose[idx];
            eff_mc = m->muoTrigEffMC_loose[idx];
        }
        else {
            cout << "SusyNtTools::get_single_muon_trigger_scale_factor    "
                << "ERROR Unhandled MuonId, we only accept Medium and Loose muon ID working points -- "
                 << "Exiting." << endl;
            exit(1); // dantrim June 29 2017 -- just exit, ignorance is not bliss
        }
        rate_not_fired_data *= (1. - eff_data);
        rate_not_fired_mc *= (1. - eff_mc);
    } // m
    float scale_factor = 1.0;
    if(1. - rate_not_fired_data == 0) { scale_factor = 0.; } 
    if(1. - rate_not_fired_mc > 0.) {
        scale_factor = ( 1. - rate_not_fired_data ) / ( 1. - rate_not_fired_mc );
    }
    return scale_factor;
}
float SusyNtTools::get_dimuon_trigger_scale_factor(const MuonVector& muons, MuonId id, string trigger,
        const SusyNtSys sys)
{
    // if "mu8noL1" is not in the trigger or if "2mu" is in the trigger return
    // the assumption is that htere is only TWO mu chains int he trigger name, e.g.:
    // HLT_mu22_mu8noL1 and NOT HLT_mu24_mu14_mu8noL1
    string second_leg = "_mu8noL1";
    string true_dimu = "2mu";
    bool has_mu8noL1 = (trigger.find(second_leg) != std::string::npos);
    bool is_true_dimu = (trigger.find(true_dimu) != std::string::npos);

    if(is_true_dimu) {
        cout << "SusyNtTools::get_dimuon_trigger_scale_factor    "
            << "ERROR We do not store muon trigger efficiencies for the dimuon triggers "
            << "with '2mu' (e.g. HLT_2mu10 or HLT_2mu14), you have provided " << trigger << " -- Exiting." << endl;
        exit(1);
    }
    if(!has_mu8noL1) {
        cout << "SusyNtTools::get_dimuon_trigger_scale_factor    "
            << "ERROR We can only handle dimuon trigger scale factors for those dimuon triggers "
            << "with the second leg 'mu8noL1', you have provided " << trigger << " -- Exiting." << endl;
        exit(1);
    }

    float scale_factor = 1.0;

    string hlt = "HLT_";
    string trig_test = trigger;
    trig_test.erase(0, hlt.length());
    size_t second_pos = trig_test.find(second_leg);
    trig_test.erase(second_pos, second_leg.length());
    second_leg = "mu8noL1";

    string trig_first_leg = "HLT_" + trig_test;
    string trig_second_leg = "HLT_" + second_leg;
    vector<string> trigs_split { trig_first_leg, trig_second_leg };
    for(auto t : trigs_split) {
        int idx_t = triggerTool().idx_of_trigger(t);
        double rate_not_fired_data = 1.0;
        double rate_not_fired_mc = 1.0;
        for(auto & m : muons) {
            double eff_data = 1.0;
            double eff_mc = 1.0;
            if(id == MuonId::Medium) {
                if(sys == NtSys::NOM) {
                    eff_data = m->muoTrigEffData_medium[idx_t];
                    eff_mc = m->muoTrigEffMC_medium[idx_t];
                }
                else if(sys == NtSys::MUON_EFF_TRIG_STAT_UP) {
                    eff_data = m->muoTrigEffErrData_stat_up_medium[idx_t];
                    eff_mc = m->muoTrigEffErrMC_stat_up_medium[idx_t];
                }
                else if(sys == NtSys::MUON_EFF_TRIG_STAT_DN) {
                    eff_data = m->muoTrigEffErrData_stat_dn_medium[idx_t];
                    eff_mc = m->muoTrigEffErrMC_stat_dn_medium[idx_t];
                }
                else if(sys == NtSys::MUON_EFF_TRIG_SYST_UP) {
                    eff_data = m->muoTrigEffErrData_syst_up_medium[idx_t];
                    eff_mc = m->muoTrigEffErrMC_syst_up_medium[idx_t];
                }
                else if(sys == NtSys::MUON_EFF_TRIG_SYST_DN) {
                    eff_data = m->muoTrigEffErrData_syst_dn_medium[idx_t];
                    eff_mc = m->muoTrigEffErrMC_syst_dn_medium[idx_t];
                }
            }
            else if(id == MuonId::Loose) {
                if(sys != NtSys::NOM) {
                    cout << "SusyNtTools::get_dimuon_trigger_scale_factor   "
                            << "ERROR We only store trigger SF variations for Medium muons -- Exiting." << endl;
                    exit(1);
                }
                eff_data = m->muoTrigEffData_loose[idx_t];
                eff_mc = m->muoTrigEffMC_loose[idx_t];
            }
            else {
                cout << "SusyNtTools::get_dimuon_trigger_scale_factor    "
                    << "ERROR Unhandled MuonId, we only accept Medium and Loose muon ID working points -- "
                     << "Exiting." << endl;
                exit(1); // dantrim June 29 2017 -- just exit, ignorance is not bliss
            }
            rate_not_fired_data *= (1. - eff_data);
            rate_not_fired_mc *= (1. - eff_mc);
        } // m
        if(1. - rate_not_fired_data == 0) { scale_factor = 0.; }
        if(1. - rate_not_fired_mc > 0.) {
            scale_factor *= (1. - rate_not_fired_data) / (1. - rate_not_fired_mc);
        }
    } // t
    return scale_factor;

}
float SusyNtTools::get_electron_trigger_scale_factor(Susy::Electron& ele, string trigger,
        const NtSys::SusyNtSys sys)
{
    ElectronVector trig_ele;
    trig_ele.push_back(&ele);
    return get_electron_trigger_scale_factor(trig_ele, trigger, sys);
}
float SusyNtTools::get_electron_trigger_scale_factor(Susy::Electron& el1, Susy::Electron& el2, string trigger,
        const NtSys::SusyNtSys sys)
{
    ElectronVector trig_ele;
    trig_ele.push_back(&el1);
    trig_ele.push_back(&el2);
    return get_electron_trigger_scale_factor(trig_ele, trigger, sys);
}
float SusyNtTools::get_electron_trigger_scale_factor(const ElectronVector& electrons, string trigger,
        const NtSys::SusyNtSys sys)
{
    float scale_factor = 1.0;

    vector<string> single_ele_triggers = triggerTool().single_ele_triggers();
    vector<string> di_ele_triggers = triggerTool().di_ele_triggers();
    vector<string> mixed_ele_muo_triggers = triggerTool().ele_muo_triggers();

    bool is_single = 
    (std::find(single_ele_triggers.begin(), single_ele_triggers.end(), trigger) != single_ele_triggers.end());
    bool is_double =
    (std::find(di_ele_triggers.begin(), di_ele_triggers.end(), trigger) != di_ele_triggers.end());    
    bool is_mixed =
    (std::find(mixed_ele_muo_triggers.begin(), mixed_ele_muo_triggers.end(), trigger) != mixed_ele_muo_triggers.end());

    if(!(is_single || is_double || is_mixed)) {
        cout << "SusyNtTools::get_electron_trigger_scale_factor    ERROR Could not find "
            << "requested trigger (" << trigger << ") in list of electron triggers "
            << "(c.f. SusyNtuple/TriggerList.h) -- Exiting" << endl;
        exit(1);
    }

    ElectronId id = electronSelector().signalId();


    // check the systematic
    int sys_ele = 0;
    if(sys == NtSys::EL_EFF_Trigger_TOTAL_DN) {
        sys_ele = -1;
    }
    else if(sys == NtSys::EL_EFF_Trigger_TOTAL_UP) {
        sys_ele = 1;
    }

    // get the god-given, so-called "single", "double", or "mixed" electron trigger SF's
    for(auto & el : electrons) {
        // single
        if(is_single) {
            if(sys_ele==0) {
                scale_factor *= el->eleTrigSF_single[id];
            }
            else if(sys_ele<0) {
                scale_factor *= el->errEffSF_trig_dn_single[id];
            }
            else if(sys_ele>0) {
                scale_factor *= el->errEffSF_trig_up_single[id];
            }
        }
        // double
        else if(is_double) {
            if(sys_ele==0) {
                scale_factor *= el->eleTrigSF_double[id];
            }
            else if(sys_ele<0) {
                scale_factor *= el->errEffSF_trig_dn_double[id];
            }
            else if(sys_ele>0) {
                scale_factor *= el->errEffSF_trig_up_double[id];
            }
        }
        // mixed
        else if(is_mixed) {
            if(sys_ele==0) {
                scale_factor *= el->eleTrigSF_mixed[id];
            }
            else if(sys_ele<0) {
                scale_factor *= el->errEffSF_trig_dn_mixed[id];
            }
            else if(sys_ele>0) {
                scale_factor *= el->errEffSF_trig_up_mixed[id];
            }
        }
    }

    return scale_factor;
}
///////////////////////////////////////////////////////////////////////
// Sherpa 2.2 V+jets
///////////////////////////////////////////////////////////////////////
bool SusyNtTools::isSherpa22Vjet(const int dsid)
{
    return (isSherpa22Wjet(dsid) || isSherpa22Zjet(dsid)); 
}
bool SusyNtTools::isSherpa22Wjet(const int mc_)
{
    bool is_wjet = false;
    if( (mc_ >= 363331 && mc_ <= 363354) ||
        (mc_ >= 363436 && mc_ <= 363459) ||
        (mc_ >= 363460 && mc_ <= 363483) ) is_wjet = true;
    return is_wjet;
}
bool SusyNtTools::isSherpa22Zjet(const int mc_)
{
    bool is_zjet = false;
    if( (mc_ >= 363102 && mc_ <= 363122) ||
        (mc_ >= 363361 && mc_ <= 363363) ||
        (mc_ >= 363388 && mc_ <= 363411) ||
        (mc_ >= 363364 && mc_ <= 363387) ||
        (mc_ >= 363412 && mc_ <= 363433) ) is_zjet = true;
    return is_zjet;
}
