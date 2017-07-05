//  -*- c++ -*-
#ifndef SusyNtuple_SusyNtTools_h
#define SusyNtuple_SusyNtTools_h

// std
#include <iostream>

// SusyNtuple
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/PhotonSelector.h"
#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/TauSelector.h"
#include "SusyNtuple/TauId.h"
#include "SusyNtuple/TriggerTools.h"

// SUSYTools
#include "SUSYTools/SUSYCrossSection.h"

using namespace Susy;
using namespace NtSys;

/// A class of useful tools for working with SusyNt
class SusyNtTools {
public:

    /// Constructor and destructor
    SusyNtTools();
    virtual ~SusyNtTools();

    void setSFOSRemoval(AnalysisType A);
    bool doSFOSRemoval() { return m_doSFOS; }
    void removeSFOSPairs(ElectronVector& baseElectrons, MuonVector& baseMuons);
    
    /**
        Set AnalysisType to determine object selections
        and overlap procedure
    */
    void setAnaType(AnalysisType A, bool verbose = false);
    AnalysisType getAnaType() { return m_anaType; }

    /// initialize the trigger tool; return success
    /**
       TriggerTools needs to read the histogram with the triggers.
       You need to call this function if you want to use the trigger
       tool. The histogram can be read from any of the input files
       (for example the one from ChainHelper::firstFile()).
     */
    bool initTriggerTool(const std::string &anyInputFilename);

    //
    // Methods to return the tools
    //
    ElectronSelector&   electronSelector()   { return *m_electronSelector; }
    MuonSelector&       muonSelector()       { return *m_muonSelector; }
    TauSelector&        tauSelector()        { return *m_tauSelector; }
    JetSelector&        jetSelector()        { return *m_jetSelector; }
    PhotonSelector&     photonSelector()     { return *m_photonSelector; }
    OverlapTools&       overlapTool()        { return *m_overlapTool; }
    TriggerTools&       triggerTool()        { return m_triggerTool; }

    //
    // Methods to perform event selection
    //
    bool passGRL(int flags);
    bool passLarErr(int flags);
    bool passTileErr(int flags);
    bool passSCTErr(int flags);
    bool passTTC(int flags);
    bool passGoodVtx(int flags);
    bool passBadMuon(const MuonVector* preMuons) { return SusyNtTools::passBadMuon(*preMuons); }
    bool passBadMuon(const MuonVector& preMuons);
    bool passCosmicMuon(const MuonVector* baseMuons) { return SusyNtTools::passCosmicMuon(*baseMuons); }
    bool passCosmicMuon(const MuonVector& baseMuons);
    bool passJetCleaning(const JetVector* preJets) { return SusyNtTools::passJetCleaning(*preJets); }
    bool passJetCleaning(const JetVector& preJets);

    // TST MET bug cleaning (temporary fix)
    // see:
    // https://indico.cern.ch/event/528073/contributions/2161457/attachments/1272110/1885539/ETmiss_intro_20160512.pdf
    // https://indico.cern.ch/event/524019/
    bool passMetCleaning(const Met* met);

    //
    // Methods to grab objects based on systematic shift desired
    //

    /// 'Pre' objects - before full baseline requirements are applied, raw objects stored in SusyNt.
    /// Note: In these methods the object systematics are applied.
    ElectronVector getPreElectrons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    MuonVector     getPreMuons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    TauVector      getPreTaus(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    JetVector      getPreJets(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    PhotonVector   getPrePhotons(Susy::SusyNtObject* susyNt, SusyNtSys sys);

    /// Apply Baseline selection to the 'Pre' objects
    ElectronVector getBaselineElectrons(const ElectronVector& preElectrons);
    MuonVector     getBaselineMuons(const MuonVector& preMuons);
    TauVector      getBaselineTaus(const TauVector& preTaus);
    JetVector      getBaselineJets(const JetVector& preJets);
    PhotonVector   getBaselinePhotons(const PhotonVector& prePhotons);

    /// Get 'Pre' Objects. These are the objects ase they are in the SusyNt.
    /// The systematic variations are applied here and then propagated to baseline
    /// and signal objects.
    void getPreObjects(Susy::SusyNtObject* susyNt, SusyNtSys sys,
                                                   ElectronVector& preElectrons,
                                                   MuonVector& preMuons,
                                                   JetVector& preJets,
                                                   TauVector& preTaus,
                                                   PhotonVector& prePhotons);

    /// Get Baseline objects
    void getBaselineObjects(const ElectronVector& preElectrons, const MuonVector& preMuons, const JetVector& preJets, const TauVector& preTaus, const PhotonVector& prePhotons,
                            ElectronVector& baseElectrons, MuonVector& baseMuons, JetVector& baseJets, TauVector& baseTaus, PhotonVector& basePhotons);
    /// Get Signal Objects
    void getSignalObjects(const ElectronVector& baseElectrons, const MuonVector& baseMuons, const JetVector& baseJets, const TauVector& baseTaus, const PhotonVector& basePhotons,
                            ElectronVector& signalElectrons, MuonVector& signalMuons, JetVector& signalJets, TauVector& signalTaus, PhotonVector& signalPhotons);


    /// Signal objects
    ElectronVector getSignalElectrons(const ElectronVector& baseElecs);
    MuonVector     getSignalMuons(const MuonVector& baseMuons);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(const TauVector& baseTaus);
    JetVector      getSignalJets(const JetVector& baseJets);
    PhotonVector   getSignalPhotons(const PhotonVector& basePhotons);

    /// Check if signal object
    bool isSignal(const Susy::Lepton* l);
    bool isSignal(const Susy::Electron* e);
    bool isSignal(const Susy::Muon* m);
    bool isSignal(const Susy::Tau* tau);
    bool isSignal(const Susy::Photon* photon);

    /// Build lepton vector, sort by pT
    void buildLeptons(LeptonVector &lep, const ElectronVector& ele, const MuonVector& muo);

    /// Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    Susy::TrackMet* getTrackMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);

    //
    // Methods to get useful quantities for event, leptons, or jets
    //

    // B jets
    int numBJets(const JetVector& jets);
    bool hasBJet(const JetVector& jets);
    JetVector getBJets(const JetVector& jets);


    int numberOfCLJets(const JetVector& jets);
    int numberOfCBJets(const JetVector& jets);
    int numberOfFJets(const JetVector& jets);

    ///////////////////////////////////////////
    // Methods to grab obect SF
    ///////////////////////////////////////////

    /// Method to get the nominal b-Tag efficiency scale-factor for the collection of jets
    float bTagSF(const JetVector& jets);
    float bTagSFError(const JetVector& jets, const NtSys::SusyNtSys sys);
   
    /// Method to get the nominal lepton efficiency scale-factor for the collection of leptons
    float leptonEffSF(const LeptonVector& leps, const NtSys::SusyNtSys sys = NtSys::NOM);
    float leptonEffSF(const Lepton* lep, const NtSys::SusyNtSys sys = NtSys::NOM);
    float leptonEffSF(const Lepton& lep, const NtSys::SusyNtSys sys = NtSys::NOM);

    /// Method to get the difference w.r.t. nominal for a given SF variation
    float leptonEffSFError(const Lepton& lep, const NtSys::SusyNtSys sys);
    float leptonEffSFError(const Lepton* lep, const NtSys::SusyNtSys sys) { return leptonEffSFError((*lep), sys); }

    /// Methods to grab lepton trigger scale-factors
    float leptonTriggerSF(const LeptonVector& leps, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);

    /// Methods to grab muon trigger scale-factors
    float get_muon_trigger_scale_factor(const MuonVector& muons, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);
    float get_muon_trigger_scale_factor(Susy::Muon& muon, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);
    float get_muon_trigger_scale_factor(Susy::Muon& mu1, Susy::Muon& mu2, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);
    float get_single_muon_trigger_scale_factor(const MuonVector& muons, MuonId id, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM); 
    float get_dimuon_trigger_scale_factor(const MuonVector& muons, MuonId id, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);

    /// Methods to grab electron trigger scale-factors
    float get_electron_trigger_scale_factor(const ElectronVector& electrons, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);
    float get_electron_trigger_scale_factor(Susy::Electron& electron, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);
    float get_electron_trigger_scale_factor(Susy::Electron& el1, Susy::Electron& el2, std::string trigger, const NtSys::SusyNtSys sys = NtSys::NOM);


    ///////////////////////////////////////////
    // check of sample is Sherpa2.2 V+jet
    ///////////////////////////////////////////
    static bool isSherpa22Vjet(const int dsid);
    static bool isSherpa22Wjet(const int dsid);
    static bool isSherpa22Zjet(const int dsid);

    //
    // Object printing
    //

    /// Call print on all objects in a vector
    template<class T> void dumpObjects(const std::vector<T>& objects)
    {
        for (unsigned int i = 0; i < objects.size(); i++) {
            std::cout << i << " ";
            objects[i].print();
        }
    }
    template<class T> void dumpObjects(const std::vector<T*>& objects)
    {
        for (unsigned int i = 0; i < objects.size(); i++) {
            std::cout << i << " ";
            objects[i]->print();
        }
    }

    // \todo make these datamembers protected
    // Object Selectors and Tools
    ElectronSelector* m_electronSelector; ///< select electrons according to the current analysis settings
    MuonSelector* m_muonSelector;         ///< select muons according to the current analysis settings
    TauSelector*  m_tauSelector;          ///< select taus according to the current analysis settings
    JetSelector* m_jetSelector;           ///< select jets according to the current analysis settings
    PhotonSelector* m_photonSelector;     ///< select photons according to the current analysis settings
    OverlapTools* m_overlapTool;          ///< tool to perform the analysis' OR procedure
    TriggerTools m_triggerTool;  ///< tool to access the trigger information

protected:

    AnalysisType m_anaType;    ///< Analysis type. currently 2-lep or 3-lep
    bool m_doSFOS;             ///< toggle to set whether to remove SFOS pairs from baseline leptons (set based on AnalysisType)
    int n_warning;
};

#endif
