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
#include "SusyNtuple/OverlapTools.h"
#include "SusyNtuple/TauSelector.h"
#include "SusyNtuple/TauId.h"

// SUSYTools
#include "SUSYTools/SUSYCrossSection.h"

using namespace Susy;
using namespace NtSys;

/// A class of useful tools for working with SusyNt
class SusyNtTools {
public:

    /// Constructor and destructor
    SusyNtTools();
    virtual ~SusyNtTools()
    {
    };

    void setSFOSRemoval(AnalysisType A);
    bool doSFOSRemoval() { return m_doSFOS; }
    void removeSFOSPairs(ElectronVector& baseElectrons, MuonVector& baseMuons);

    /// Set Analysis type to determine selection
    void setAnaType(AnalysisType A, bool verbose = false)
    {
        ////////////////////////////
        // ElectronSelector
        ////////////////////////////
        m_electronSelector.setAnalysis(A);
        ////////////////////////////
        // MuonSelector
        ////////////////////////////
        m_muonSelector.setAnalysis(A);
        ////////////////////////////
        // TauSelector
        ////////////////////////////
        m_tauSelector.setAnalysis(A);
        ////////////////////////////
        // JetSelector
        ////////////////////////////
        m_jetSelector.setAnalysis(A);
        ////////////////////////////
        // OverlapTool
        ////////////////////////////
        // propagate isolation requirements
        m_overlapTool.setElectronIsolation(m_electronSelector.signalIsolation());
        m_overlapTool.setMuonIsolation(m_muonSelector.signalIsolation());
        // propagate OR loose b-tag WP
        m_overlapTool.setORBtagEff(m_jetSelector.overlapRemovalBtagEffWP());
        // now setAnalysis
        m_overlapTool.setAnalysis(A);

        // set whether to perform SFOS removal on baseline objects
        setSFOSRemoval(A);

        // this should be in the logs no matter what
        std::cout << ">>> Setting analysis type to " << AnalysisType2str(A) << std::endl;

        // now that the tools are configured set this variable
        m_anaType = A;
    };
    AnalysisType getAnaType() { return m_anaType; }

    //
    // Methods to perform event selection
    //
    bool passGRL(int flags);
    bool passLarErr(int flags);
    bool passTileErr(int flags);
    bool passTTC(int flags);
    bool passGoodVtx(int flags);
    bool passBadMuon(const MuonVector* preMuons) { return SusyNtTools::passBadMuon(*preMuons); }
    bool passBadMuon(const MuonVector& preMuons);
    bool passCosmicMuon(const MuonVector* baseMuons) { return SusyNtTools::passCosmicMuon(*baseMuons); }
    bool passCosmicMuon(const MuonVector& baseMuons);
    bool passJetCleaning(const JetVector* preJets) { return SusyNtTools::passJetCleaning(*preJets); }
    bool passJetCleaning(const JetVector& preJets);

    //
    // Methods to grab objects based on systematic shift desired
    //

    /// 'Pre' objects - before full baseline requirements are applied, raw objects stored in SusyNt.
    /// Note: In these methods the object systematics are applied.
    ElectronVector getPreElectrons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    MuonVector     getPreMuons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    TauVector      getPreTaus(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    JetVector      getPreJets(Susy::SusyNtObject* susyNt, SusyNtSys sys);

    /// Apply Baseline selection to the 'Pre' objects
    ElectronVector getBaselineElectrons(const ElectronVector& preElectrons);
    MuonVector     getBaselineMuons(const MuonVector& preMuons);
    TauVector      getBaselineTaus(const TauVector& preTaus);
    JetVector      getBaselineJets(const JetVector& preJets);

    /// Get 'Pre' Objects. These are the objects ase they are in the SusyNt.
    /// The systematic variations are applied here.
    void getPreObjects(Susy::SusyNtObject* susyNt, SusyNtSys sys,
                                                   ElectronVector& preElectrons,
                                                   MuonVector& preMuons,
                                                   JetVector& preJets,
                                                   TauVector& preTaus);

    /// Get Baseline objects
    void getBaselineObjects(const ElectronVector& preElectrons, const MuonVector& preMuons, const JetVector& preJets, const TauVector& preTaus,
                            ElectronVector& baseElectrons, MuonVector& baseMuons, JetVector& baseJets, TauVector& baseTaus);
    /// Get Signal Objects
    void getSignalObjects(const ElectronVector& baseElectrons, const MuonVector& baseMuons, const JetVector& baseJets, const TauVector& baseTaus,
                            ElectronVector& signalElectrons, MuonVector& signalMuons, JetVector& signalJets, TauVector& signalTaus, TauId& sigTauId);



    /// Get Baseline objects. Pre + overlap removal.
    /** First method provides the pre-selected objects before OR and baseline objects after OR. */
    void getBaselineObjects(Susy::SusyNtObject* susyNt,
                            ElectronVector& preElecs, MuonVector& preMuons, JetVector& preJets,
                            ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets,
                            SusyNtSys sys, bool selectTaus = false);
    /// Second method only provides the baseline objects after OR.
    void getBaselineObjects(Susy::SusyNtObject* susyNt, ElectronVector& elecs,
                            MuonVector& muons, TauVector& taus, JetVector& jets,
                            SusyNtSys sys, bool selectTaus = false);

    /// Signal objects
    ElectronVector getSignalElectrons(const ElectronVector& baseElecs);
    MuonVector     getSignalMuons(const MuonVector& baseMuons);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(const TauVector& baseTaus, TauId tauJetID = TauId::Medium,
                                 TauId tauEleID = TauId::Loose, TauId tauMuoID = TauId::Medium);
    JetVector      getSignalJets(const JetVector& baseJets);

    /// Get the signal objects
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                          const TauVector& baseTaus, const JetVector& baseJets,
                          ElectronVector& sigElecs, MuonVector& sigMuons,
                          TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC,
                          TauId tauJetID = TauId::Medium, TauId tauEleID = TauId::Loose, TauId tauMuoID = TauId::Medium,
                          SusyNtSys sys = NtSys::NOM);
    // This method cannot be used anymore because it doesn't provide the baseline objects after OR.
    // Analyzers need these baseline objects for cleaning cuts.
    //void getSignalObjects(Susy::SusyNtObject* susyNt, ElectronVector& sigElecs,
    //                      MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets,
    //                      JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC,
    //                      bool selectTaus=false,
    //                      TauId tauID=TauId::Medium);

    /// New signal tau prescription, fill both ID levels at once
    // These will replace the methods above
    void getSignalTaus(const TauVector& baseTaus, TauVector& mediumTaus, TauVector& tightTaus);
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                          const TauVector& baseTaus, const JetVector& baseJets,
                          ElectronVector& sigElecs, MuonVector& sigMuons,
                          TauVector& mediumTaus, TauVector& tightTaus,
                          JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, SusyNtSys sys = NtSys::NOM);

    /// Check if selected object
    bool isTauBDT(const Susy::Tau* tau, TauId tauJetID = TauId::Medium,
                  TauId tauEleID = TauId::Loose, TauId tauMuoID = TauId::Medium);
    // TODO: add new selection methods for light leptons and jets
    //bool isSelectLepton()
    //virtual bool isSelectTau(const Susy::Tau* tau, TauId id=TauId::Medium);
    //virtual bool isSelectTau(const Susy::Tau* tau, TauId tauJetID = TauId::Medium,
    //                         TauId tauEleID = TauId::Loose, TauId tauMuoID = TauId::Medium);

    /// Check if signal object
    bool isSignalLepton(const Susy::Lepton* l);
    bool isSignalElectron(const Susy::Electron* e);
    bool isSignalMuon(const Susy::Muon* m);
    bool isSignalTau(const Susy::Tau* tau, TauId tauJetID = TauId::Medium,
                     TauId tauEleID = TauId::Loose, TauId tauMuoID = TauId::Medium);
    bool isSemiSignalElectron(const Susy::Electron* ele);
    bool isSemiSignalMuon(const Susy::Muon* mu);

    /// Build lepton vector, sort by pT
    void buildLeptons(LeptonVector &lep, const ElectronVector& ele, const MuonVector& muo);

    /// Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);//, bool useNomPhiForMetSys = true);
    Susy::TrackMet* getTrackMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);//, bool useNomPhiForMetSys = true);

    //
    // Methods to get useful quantities for event, leptons, or jets
    //

    // B jets
    int numBJets(const JetVector& jets);
    bool hasBJet(const JetVector& jets);
    JetVector getBJets(const JetVector& jets);

//    static JetVector getBTagSFJets2Lep(const JetVector& baseJets);
    float bTagSF(const Susy::Event*, const JetVector& jets, int mcID, BTagSys sys = BTag_NOM);

    int numberOfCLJets(const JetVector& jets);
    int numberOfCBJets(const JetVector& jets);
    int numberOfFJets(const JetVector& jets);

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

    /////////////////////////////////////////////
    // Object Selectors and Tools
    /////////////////////////////////////////////
    ElectronSelector m_electronSelector;
    MuonSelector m_muonSelector;
    TauSelector  m_tauSelector;
    JetSelector m_jetSelector;              ///< select jets according to the current analysis settings
    OverlapTools m_overlapTool;             ///< tool to perform the analysis' OR procedure

protected:

    AnalysisType m_anaType;             ///< Analysis type. currently 2-lep or 3-lep
    bool m_doSFOS;                     ///< toggle to set whether to remove SFOS pairs from baseline leptons (set based on AnalysisType)

};

#endif
