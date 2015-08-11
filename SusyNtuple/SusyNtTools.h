#ifndef SusyNtuple_SusyNtTools_h
#define SusyNtuple_SusyNtTools_h


#include <iostream>

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/MuonSelector.h"
#include "SusyNtuple/ElectronSelector.h"
#include "SusyNtuple/OverlapTools.h"
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

    /// Set Analysis type to determine selection
    void setAnaType(AnalysisType A, bool verbose = false)
    {
        m_anaType = A;
        ////////////////////////////
        // ElectronSelector
        ////////////////////////////
        m_electronSelector.setAnalysis(A);
        ////////////////////////////
        // MuonSelector
        ////////////////////////////
        m_muonSelector.setAnalysis(A);
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

        // this should be in the logs no matter what
        std::cout << ">>> Setting analysis type to " << AnalysisType2str(A) << std::endl;
    };
    AnalysisType getAnaType() { return m_anaType; }

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
    JetVector      getBaselineJets(const JetVector& preJets);

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
    ElectronVector getSignalElectrons(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                      uint nVtx, bool isMC, bool removeLepsFromIso = false);
    MuonVector     getSignalMuons(const MuonVector& baseMuons, const ElectronVector& baseElecs,
                                  uint nVtx, bool isMC, bool removeLepsFromIso = false);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(const TauVector& baseTaus, TauID tauJetID = TauID_medium,
                                 TauID tauEleID = TauID_loose, TauID tauMuoID = TauID_medium);
    JetVector      getSignalJets(const JetVector& baseJets, SusyNtSys sys = NtSys::NOM);

    /// Get the signal objects
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                          const TauVector& baseTaus, const JetVector& baseJets,
                          ElectronVector& sigElecs, MuonVector& sigMuons,
                          TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, bool removeLepsFromIso = false,
                          TauID tauJetID = TauID_medium, TauID tauEleID = TauID_loose, TauID tauMuoID = TauID_medium,
                          SusyNtSys sys = NtSys::NOM);
    // This method cannot be used anymore because it doesn't provide the baseline objects after OR.
    // Analyzers need these baseline objects for cleaning cuts.
    //void getSignalObjects(Susy::SusyNtObject* susyNt, ElectronVector& sigElecs,
    //                      MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets,
    //                      JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC,
    //                      bool selectTaus=false, bool removeLepsFromIso=false,
    //                      TauID tauID=TauID_medium);

    /// New signal tau prescription, fill both ID levels at once
    // These will replace the methods above
    void getSignalTaus(const TauVector& baseTaus, TauVector& mediumTaus, TauVector& tightTaus);
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                          const TauVector& baseTaus, const JetVector& baseJets,
                          ElectronVector& sigElecs, MuonVector& sigMuons,
                          TauVector& mediumTaus, TauVector& tightTaus,
                          JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, bool removeLepsFromIso = false,
                          SusyNtSys sys = NtSys::NOM);

    /// Check if selected object
    bool isTauBDT(const Susy::Tau* tau, TauID tauJetID = TauID_medium,
                  TauID tauEleID = TauID_loose, TauID tauMuoID = TauID_medium);
    // TODO: add new selection methods for light leptons and jets
    //bool isSelectLepton()
    //virtual bool isSelectTau(const Susy::Tau* tau, TauID id=TauID_medium);
    virtual bool isSelectTau(const Susy::Tau* tau, TauID tauJetID = TauID_medium,
                             TauID tauEleID = TauID_loose, TauID tauMuoID = TauID_medium);

    /// Check if signal object
    bool isSignalLepton(const Susy::Lepton* l);
    bool isSignalElectron(const Susy::Electron* e);
    bool isSignalMuon(const Susy::Muon* m);
    bool isSignalTau(const Susy::Tau* tau, TauID tauJetID = TauID_medium,
                     TauID tauEleID = TauID_loose, TauID tauMuoID = TauID_medium);
    bool isSemiSignalElectron(const Susy::Electron* ele);
    bool isSemiSignalMuon(const Susy::Muon* mu);

    /// Build Lepton vector - we should probably sort them here
    void buildLeptons(LeptonVector &lep, ElectronVector& ele, MuonVector& muo)
    {
        for (uint ie = 0; ie < ele.size(); ie++)
            lep.push_back(ele[ie]);
        for (uint im = 0; im < muo.size(); im++)
            lep.push_back(muo[im]);
    };

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
    JetSelector m_jetSelector; ///< select jets according to the current analysis settings
    OverlapTools m_overlapTool; ///< tool to perform the analysis' OR procedure

protected:

    AnalysisType m_anaType;             ///< Analysis type. currently 2-lep or 3-lep
    bool m_doMSFOS;                     ///< toggle to set whether to remove SFOS pairs from baseline leptons (set based on AnalysisType)

};

#endif
