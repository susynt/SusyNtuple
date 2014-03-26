#ifndef SusyNtuple_SusyNtTools_h
#define SusyNtuple_SusyNtTools_h


#include <iostream>

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/MCWeighter.h"
#include "SUSYTools/BTagCalib.h"
#include "SUSYTools/SUSYCrossSection.h"
#include "JVFUncertaintyTool/JVFUncertaintyTool.h"

/*

    SusyNtTools - a class of useful tools for working with SusyNt

*/

class SusyNtTools
{

  public:

    // Constructor and destructor
    SusyNtTools();
    virtual ~SusyNtTools(){
      if(m_btagTool) delete m_btagTool;
    };

    // Set Analysis type to determine selection
    void setAnaType(AnalysisType A, bool verbose=false){ 
      m_anaType = A; 
      if(verbose) std::cout << ">>> Setting analysis type to " << SusyNtAnalysisType[A] << std::endl;
    };

    // Configure the btag sf tool
    void configureBTagTool(std::string OP, float opVal, bool isJVF);
    //void configureJVFTool(std::string jetAlgo="AntiKt4TopoEM");

    //
    // Get event weight - contains generator, pileup, xsec, and lumi weights
    // TODO: move this machinery into MCWeighter class
    //

    // Default weight uses full dataset, currently A-L
    // Pileup weights correspond to same dataset.
    virtual float getEventWeight(const Susy::Event* evt, float lumi = LUMI_A_L, 
                                 bool useSumwMap = false, 
                                 const SumwMap* sumwMap = 0,
                                 bool useProcSumw = false,
                                 bool useSusyXsec = false,
                                 MCWeighter::WeightSys sys=MCWeighter::Sys_NOM);

    // Use this function to scale MC to the A-B3 unblinded dataset (1.04/fb)
    // This will use the correct pileup weights for A-B3
    //virtual float getEventWeightAB3(const Susy::Event* evt);

    // Scale MC to A-B dataset (5.83/fb)
    //virtual float getEventWeightAB(const Susy::Event* evt);

    // Get the sumw for this event
    static float getSumw(const Susy::Event* evt, const SumwMap* sumwMap, 
                         bool useSumwMap=true, bool useProcSumw=true);
    // Get the SUSYTools cross section info for this event
    static SUSY::CrossSectionDB::Process getCrossSection(const Susy::Event* evt);
    // Get the final cross section times efficiency for this event
    static float getXsecTimesEff(const Susy::Event* evt, bool useSusyXsec=true, 
                                 MCWeighter::WeightSys sys=MCWeighter::Sys_NOM);
    // Get the pileup weight
    static float getPileupWeight(const Susy::Event* evt, MCWeighter::WeightSys sys=MCWeighter::Sys_NOM);

    //
    // Methods to grab objects based on systematic shift desired
    //

    // 'Pre' Objects. Keep same naming convention as I saw in SusyD3PDAna
    ElectronVector getPreElectrons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    // Bugfix for muon systematics in n0150
    MuonVector     getPreMuons(Susy::SusyNtObject* susyNt, SusyNtSys sys, bool n0150BugFix=false);
    TauVector      getPreTaus(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    JetVector      getPreJets(Susy::SusyNtObject* susyNt, SusyNtSys sys);
  
    // Get Baseline objects. Pre + overlap removal.
    // First method provides the pre-selected objects before OR and baseline objects after OR.
    void getBaselineObjects(Susy::SusyNtObject* susyNt, 
                            ElectronVector& preElecs, MuonVector& preMuons, JetVector& preJets,
                            ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets, 
                            SusyNtSys sys, bool selectTaus=false, bool n0150BugFix=false);
    // Second method only provides the baseline objects after OR.
    void getBaselineObjects(Susy::SusyNtObject* susyNt, ElectronVector& elecs, 
                            MuonVector& muons, TauVector& taus, JetVector& jets, 
                            SusyNtSys sys, bool selectTaus=false, bool n0150BugFix=false);
  
    // Signal objects
    ElectronVector getSignalElectrons(const ElectronVector& baseElecs, const MuonVector& baseMuons, 
                                      uint nVtx, bool isMC, bool removeLepsFromIso=false);
    MuonVector     getSignalMuons(const MuonVector& baseMuons, const ElectronVector& baseElecs, 
                                  uint nVtx, bool isMC, bool removeLepsFromIso=false);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(const TauVector& baseTaus, TauID tauJetID=TauID_medium, 
                                 TauID tauEleID=TauID_loose, TauID tauMuoID=TauID_medium);
    JetVector      getSignalJets(const JetVector& baseJets, SusyNtSys sys=NtSys_NOM);
    JetVector      getSignalJets2Lep(const JetVector& baseJets, SusyNtSys sys=NtSys_NOM);

    // Get the signal objects
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons, 
                          const TauVector& baseTaus, const JetVector& baseJets, 
                          ElectronVector& sigElecs, MuonVector& sigMuons, 
                          TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, bool removeLepsFromIso=false,
                          TauID tauJetID=TauID_medium, TauID tauEleID=TauID_loose, TauID tauMuoID=TauID_medium,
                          SusyNtSys sys=NtSys_NOM);
    // This method cannot be used anymore because it doesn't provide the baseline objects after OR.
    // Analyzers need these baseline objects for cleaning cuts.
    //void getSignalObjects(Susy::SusyNtObject* susyNt, ElectronVector& sigElecs, 
    //                      MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets, 
    //                      JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC, 
    //                      bool selectTaus=false, bool removeLepsFromIso=false,
    //                      TauID tauID=TauID_medium);

    // New signal tau prescription, fill both ID levels at once
    // These will replace the methods above
    void getSignalTaus(const TauVector& baseTaus, TauVector& mediumTaus, TauVector& tightTaus);
    void getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons, 
                          const TauVector& baseTaus, const JetVector& baseJets, 
                          ElectronVector& sigElecs, MuonVector& sigMuons, 
                          TauVector& mediumTaus, TauVector& tightTaus, 
                          JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, bool removeLepsFromIso=false,
                          SusyNtSys sys=NtSys_NOM);
    
    // Check if selected object
    bool isTauBDT(const Susy::Tau* tau, TauID tauJetID=TauID_medium, 
                  TauID tauEleID=TauID_loose, TauID tauMuoID=TauID_medium);
    // TODO: add new selection methods for light leptons and jets
    //bool isSelectLepton()
    //virtual bool isSelectTau(const Susy::Tau* tau, TauID id=TauID_medium);
    virtual bool isSelectTau(const Susy::Tau* tau, TauID tauJetID=TauID_medium, 
                             TauID tauEleID=TauID_loose, TauID tauMuoID=TauID_medium);

    // Check if signal object
    // Signal lepton definitions include pileup and near-by lepton corrected isolation cuts
    bool isSignalLepton(const Susy::Lepton* l, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                        uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalElectron(const Susy::Electron* ele, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                          uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalMuon(const Susy::Muon* mu, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                      uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalTau(const Susy::Tau* tau, TauID tauJetID=TauID_medium, 
                     TauID tauEleID=TauID_loose, TauID tauMuoID=TauID_medium);
    bool isSemiSignalElectron(const Susy::Electron* ele);
    bool isSemiSignalMuon(const Susy::Muon* mu);
    bool isSignalJet(const Susy::Jet* jet, SusyNtSys sys=NtSys_NOM);
    bool isSignalJet2Lep(const Susy::Jet* jet, SusyNtSys sys=NtSys_NOM);


    // Build Lepton vector - we should probably sort them here
    void buildLeptons(LeptonVector &lep, ElectronVector& ele, MuonVector& muo)
    {
      for(uint ie=0; ie<ele.size(); ie++)
        lep.push_back( ele[ie] );
      for(uint im=0; im<muo.size(); im++)
        lep.push_back( muo[im] );
    };
    
    //
    // Electron, Muon isolation correction for pileup
    //
    float elPtConeCorr(const Susy::Electron* e, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
    float elEtTopoConeCorr(const Susy::Electron* e, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                           uint nVtx, bool isMC, bool removeLeps=false);
    float muPtConeCorr(const Susy::Muon* mu, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
    float muEtConeCorr(const Susy::Muon* mu, const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
  
    // Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);//, bool useNomPhiForMetSys = true);

    //
    // Methods for performing overlap removal
    //
  
    // Perform all overlap on pre objects  
    virtual void performOverlap(ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets);

    // e-e overlap
    void e_e_overlap(ElectronVector& elecs, float minDr);
  
    // e-j overlap
    void e_j_overlap(ElectronVector& elecs, JetVector& jets, float minDr, 
                     bool removeJets = true);
  
    // m-j overlap
    void m_j_overlap(MuonVector& muons, JetVector jets, float minDr);

    // e-m overlap 
    void e_m_overlap(ElectronVector& elecs, MuonVector& muons, float minDr);
  
    // m-m overlap
    void m_m_overlap(MuonVector& muons, float minDr);

    // t-e overlap
    void t_e_overlap(TauVector& taus, ElectronVector& elecs, float minDr);

    // t-m overlap
    void t_m_overlap(TauVector& taus, MuonVector& muons, float minDr);

    // t-j overlap
    void t_j_overlap(TauVector& taus, JetVector& jets, float minDr, 
                     bool removeJets = true);

    // Msfos cuts now applied along with overlap removal
    void removeSFOSPair(ElectronVector& elecs, float MllCut);
    void removeSFOSPair(MuonVector& muons, float MllCut);
    void removeSFOSPair(TauVector& taus, float MllCut);
  
    //
    // Event level checks
    //
  
    // No electron or jet in the LAr hole - shouldn't be used anymore
    bool passLAr(int flag)     { return true; }

    // Pass Tile hot spot veto
    //bool passHotSpot(int flag) { return ( flag & PASS_HotSpot ); }
    bool passHotSpot(int flag) { return ( flag & ECut_HotSpot ); }
  
    // Pass the Bad Jet requirement
    bool passBadJet(int flag)  { return ( flag & ECut_BadJet );  }
    
    // Pass the Bad Muon requirement
    bool passBadMuon(int flag) { return ( flag & ECut_BadMuon ); }
    
    // No cosmic muons
    bool passCosmic(int flag)  { return ( flag & ECut_Cosmic );  }
    
    // Pass Smart Veto
    bool passSmartVeto(int flag) { return (flag & ECut_SmartVeto); }

    // Pass All the above, incase you don't care about cut flow
    bool passAll(int flag)
    { 
      int mask = ECut_HotSpot || ECut_BadJet || ECut_BadMuon || ECut_Cosmic || ECut_SmartVeto;
      return (flag & mask) == mask;
    }


    // Additional event level checks added
    // in the event filtering has been turned off
    // NOTE: Filtering on by default!
    
    // pass GRL
    bool passGRL(int flag)         { return (flag & ECut_GRL);      }

    // pass LArErr
    bool passLarErr(int flag)      { return (flag & ECut_LarErr);   }
    
    // pass Tile Err
    bool passTileErr(int flag)     { return (flag & ECut_TileErr);  }

    // Pass TTC veto
    bool passTTCVeto(int flag)     { return (flag & ECut_TTC);      }

    // pass primary vertex
    bool passGoodVtx(int flag)     { return (flag & ECut_GoodVtx);  }

    // pass tile trip cut
    bool passTileTripCut(int flag) { return (flag & ECut_TileTrip); }

    // look at the MC truth record and determine whether SUSY propagators were involved
    static bool eventHasSusyPropagators(const std::vector< int > &pdgs,
                                        const std::vector< std::vector< int > > &parentIndices);

    //
    // Object level cleaning cut methods
    //

    int cleaningCutFlags(int ntCutFlag, 
                         const MuonVector& preMuons, const MuonVector& baseMuons, 
                         const JetVector& preJets, const JetVector& baseJets);

    // Bad muon
    bool hasBadMuon(const MuonVector& preMuons);
    // Cosmic muons
    bool hasCosmicMuon(const MuonVector& baseMuons);
    // Tile hot spot
    bool hasHotSpotJet(const JetVector& preJets);
    // Bad jet
    bool hasBadJet(const JetVector& baseJets);

    // Pass FEB dead region check
    bool passDeadRegions(const JetVector& preJets, const Susy::Met* met, int RunNumber, bool isMC);

    // To determine if there is baseline jets within bad FCAL region 
    bool hasJetInBadFCAL(const JetVector& baseJets, uint run=206248, bool isMC=false);
    bool isBadFCALJet(const Susy::Jet* jet);

  
    //
    // Object selection control toggles
    // Currently all are on by default except muon etcone
    //
    void setDoPtcone(bool doPtcone=true) { m_doPtconeCut = doPtcone; }
    void setDoElEtcone(bool doElEtcone=true) { m_doElEtconeCut = doElEtcone; }
    void setDoMuEtcone(bool doMuEtcone=false) { m_doMuEtconeCut = doMuEtcone; }
    void setDoIP(bool doIP=true) { m_doIPCut = doIP; }


    //
    // Methods to get useful quantities for event, leptons, or jets
    // Moving global functions from SusyDefs here
    // 
  
    // Lepton flavor checks
    static bool isSameFlav(const Susy::Lepton* l1, const Susy::Lepton* l2);
    static bool isOppSign(const Susy::Lepton* l1, const Susy::Lepton* l2);
    static bool isOppSign(const Susy::Tau* tau1, const Susy::Tau* tau2);
    static bool isOppSign(const Susy::Lepton* lep, const Susy::Tau* tau);
    static bool isSFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    static bool isSFSS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    static bool isOFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    static bool hasSFOS(const LeptonVector& leps);
    static bool hasSFSS(const LeptonVector& leps);
    static bool hasOFOS(const LeptonVector& leps);
    static bool hasOS(const LeptonVector& leps);
    static bool hasSS(const LeptonVector& leps);
    static bool hasOS(const TauVector& taus);

    // Mass calculation methods
    static float Mll(const Susy::Particle* l1, const Susy::Particle* l2);
    static float Mlll(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3);
    static float Mllll(const Susy::Lepton* l1, const Susy::Lepton* l2,
                       const Susy::Lepton* l3, const Susy::Lepton* l4);
    static float Mjj(const Susy::Jet* j1, const Susy::Jet* j2);
    static float Mlljj(const Susy::Lepton* l1, const Susy::Lepton* l2,
                       const Susy::Jet* j1, const Susy::Jet* j2);
    //static float Mt(const Susy::Lepton* lep, const Susy::Met* met);
    static float Mt(const TLorentzVector* lep, const Susy::Met* met);
    static float Meff(const JetVector& jets, const Susy::Met* met, float jetPtCut=40.);
    static float Meff(const LeptonVector& leps, const JetVector& jets, const Susy::Met* met, float jetPtCut=40.);
    static float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets,
                      const Susy::Met* met, float jetPtCut=40.);

    // Z selection methods
    static bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, float massWindow=10.);
    static bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3, 
                    float massWindow=10.);
    static bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3, 
                    const Susy::Lepton* l4, float massWindow=10.);
    static bool isZWindow(const Susy::Lepton* l1, const Susy::Lepton* l2, 
                          float minMll=MZ-10, float maxMll=MZ+10);
    // NEW Argument useMultiLep will check for mlll and mllll in Z peak
    static bool hasZ(const LeptonVector& leps, float massWindow=10., bool useMultiLep=false);
    static bool hasZ(const LeptonVector& leps,  uint* Zl1, uint* Zl2, float massWindow=10.,bool useMultiLep=false);
    static bool hasZlll(const LeptonVector& leps, float massWindow=10.);
    static bool hasZllll(const LeptonVector& leps, float massWindow=10.);
    static bool hasZllZll(const LeptonVector& leps, uint* Z1l1, uint* Z1l2, uint* Z2l1, uint* Z2l2,  float massWindow=10.);
    static bool hasZWindow(const LeptonVector& leps, float minMll=MZ-10, float maxMll=MZ+10);
    //void bestZ(uint& l1, uint& l2, const LeptonVector& leps, bool ignoreTau=true);

    // Safer functions, to replace the one above, which return false if no candidates found
    static bool findBestZ(uint& l1, uint& l2, const LeptonVector& leps);
    static bool findBestZ(uint& j1, uint& j2, const JetVector& jets);
    static bool findBestW(uint& j1, uint& j2, const JetVector& jets);

    // B jets
    static int numBJets(const JetVector& jets, float weight=MV1_80);
    static bool hasBJet(const JetVector& jets, float weight=MV1_80);
    static bool isBJet(const Susy::Jet* jet, float weight=MV1_80);
    static JetVector getBJets(const JetVector& jets, float weight=MV1_80);
    
    static JetVector getBTagSFJets2Lep(const JetVector& baseJets);
    float bTagSF(const Susy::Event*, const JetVector& jets, int mcID, BTagSys sys=BTag_NOM);

    // 2 Lepton jet methods and counters
    // These will no longer be static because of the systematic uncert requirement
    bool isCentralLightJet(const Susy::Jet* jet, SusyNtSys sys=NtSys_NOM);
    bool isCentralBJet    (const Susy::Jet* jet);
    bool isForwardJet     (const Susy::Jet* jet);

    int numberOfCLJets    (const JetVector& jets, SusyNtSys sys=NtSys_NOM);
    int numberOfCBJets    (const JetVector& jets);
    int numberOfFJets     (const JetVector& jets);
    void getNumberOf2LepJets(const JetVector& jets, int& Ncl, int& Ncb, int& Nf, 
                             SusyNtSys sys=NtSys_NOM);

    // MET Rel
    float getMetRel(const Susy::Met* met, const LeptonVector& leptons, const JetVector& jets, 
                    bool useForward=false);
  
    // MT2
    static float getMT2(const LeptonVector& leptons, const Susy::Met* met);
    static float getMT2(const TLorentzVector* lep1, const TLorentzVector* lep2, const Susy::Met* met);
    static float getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Susy::Met* met,  bool zeroMass, float lspMass=0);

    // HT
    static float getHT(const JetVector& jets);


    //Mljj. Returns mlj if only 1-jet
    static float mljj(const LeptonVector& leptons, const JetVector& jets);


    // Transverse thrust
    static float getThrT(const LeptonVector& leptons);

    // Top Tagger methods
    bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Susy::Met* met,
                    int opt=0, float ptJetCut=0, float mEffCut=100);
    
    bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met);
  
    bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l,
                  TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int iopt1);
  
    float calcMCT(TLorentzVector v1, TLorentzVector v2);
  
    //
    // Object printing
    //

    // Call print on all objects in a vector
    template<class T> void dumpObjects(const std::vector<T>& objects){
      for(unsigned int i = 0; i < objects.size(); i++){
        std::cout << i << " ";
        objects[i].print();
      }
    }
    template<class T> void dumpObjects(const std::vector<T*>& objects){
      for(unsigned int i = 0; i < objects.size(); i++){
        std::cout << i << " ";
        objects[i]->print();
      }
    }

    //
    // Misc methods
    //

    // Build a map of MCID -> sumw.
    // This method will loop over the input files associated with the TChain.
    // The MCID in the first entry of the tree will be used, so one CANNOT use this
    // if multiple datasets are combined into one SusyNt tree file!
    // The generator weighted cutflow histograms will then be 
    // used to calculate the total sumw for each MCID.
    // Each dataset used here must be complete, they CANNOT be spread out across multiple jobs.
    // However, one can have more than one (complete) dataset in the chain.
    SumwMap buildSumwMap(TChain* chain, bool isSimplifiedModel = false);

    // Sherpa sample check
    bool isSherpaSample(unsigned int mcID);
    
  protected:
    
    AnalysisType m_anaType;             // Analysis type. currently 2-lep or 3-lep

    // For analyzing different signal lepton cut scenarios, adding some toggles
    // This might only be temporary

    bool m_doPtconeCut;                 // ptcone isolation cuts
    bool m_doElEtconeCut;               // etcone isolation cuts for electrons
    bool m_doMuEtconeCut;               // etcone isolation cuts for muons
    bool m_doIPCut;                     // impact parameter cuts

    // Using static btag tool for now...
    //BTagCalib* m_btagTool;            // BTag tool
    static BTagCalib* m_btagTool;       // BTag tool
    JVFUncertaintyTool* m_jvfTool;      // JVF tool

};

#endif
