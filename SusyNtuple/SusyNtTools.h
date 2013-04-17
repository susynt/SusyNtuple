#ifndef SusyNtuple_SusyNtTools_h
#define SusyNtuple_SusyNtTools_h


#include <iostream>

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"

/*

    SusyNtTools - a class of useful tools for working with SusyNt

*/

class SusyNtTools
{

  public:

    // Constructor and destructor
    SusyNtTools();
    virtual ~SusyNtTools(){};

    //
    // Set Analysis type to determine selection
    //
    void setAnaType(AnalysisType A){ m_anaType = A; };

    //
    // Get event weight - contains generator, pileup, xsec, and lumi weights
    //

    // Default weight uses full dataset, currently A-L
    // Pileup weights correspond to same dataset.
    virtual float getEventWeight(const Susy::Event* evt, float lumi = LUMI_A_L, 
                                 bool useSumwMap = false, std::map<unsigned int, float>* sumwMap = 0);
    // Temporary fixed version of getEventWeight for n0138
    virtual float getEventWeightFixed(unsigned int mcChannel, const Susy::Event* evt, float lumi = LUMI_A_L);

    // Use this function to scale MC to the A-B3 unblinded dataset (1.04/fb)
    // This will use the correct pileup weights for A-B3
    virtual float getEventWeightAB3(const Susy::Event* evt);

    // Scale MC to A-B dataset (5.83/fb)
    virtual float getEventWeightAB(const Susy::Event* evt);

    //
    // Methods to grab objects based on systematic shift desired
    //

    // 'Pre' Objects. Keep same naming convention as I saw in SusyD3PDAna
    ElectronVector getPreElectrons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    MuonVector     getPreMuons(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    TauVector      getPreTaus(Susy::SusyNtObject* susyNt, SusyNtSys sys);
    JetVector      getPreJets(Susy::SusyNtObject* susyNt, SusyNtSys sys);
  
    // Get Baseline objects. Pre + overlap removal.
    // Might be able to add options in case of overlap 
    // removal differances in 2-lep vs. 3-lep.
    // TODO: make baseline object references const
    void getBaselineObjects(Susy::SusyNtObject* susyNt, ElectronVector& elecs, 
			    MuonVector& muons, TauVector& taus, JetVector& jets, 
                            SusyNtSys sys, bool selectTaus=false);
  
    // Signal objects
    ElectronVector getSignalElectrons(ElectronVector& baseElecs, MuonVector& baseMuons, 
                                      uint nVtx, bool isMC, bool removeLepsFromIso=false);
    MuonVector     getSignalMuons(MuonVector& baseMuons, ElectronVector& baseElecs, 
                                  uint nVtx, bool isMC, bool removeLepsFromIso=false);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(TauVector& baseTaus, TauID id=TauID_medium);
    JetVector      getSignalJets(JetVector& baseJets);
    JetVector      getSignalJets2Lep(JetVector& baseJets);
  
    // Get the signal objects
    void getSignalObjects(ElectronVector& baseElecs, MuonVector& baseMuons, 
                          TauVector& baseTaus, JetVector& baseJets, 
			  ElectronVector& sigElecs, MuonVector& sigMuons, 
                          TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep,
                          uint nVtx, bool isMC, bool removeLepsFromIso=false,
                          TauID tauID=TauID_medium);
    void getSignalObjects(Susy::SusyNtObject* susyNt, ElectronVector& sigElecs, 
			  MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets, 
                          JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC, 
                          bool selectTaus=false, bool removeLepsFromIso=false,
                          TauID tauID=TauID_medium);
    
    // Check if Signal.
    // Signal lepton definitions include pileup and near-by lepton corrected isolation cuts
    bool isSignalLepton(const Susy::Lepton* l, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                        uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalElectron(const Susy::Electron* ele, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                          uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalMuon(const Susy::Muon* mu, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                      uint nVtx, bool isMC, bool removeLepsFromIso=false);
    bool isSignalTau(const Susy::Tau* tau, TauID id=TauID_medium);
    bool isSignalJet(const Susy::Jet* jet);
    bool isSignalJet2Lep(const Susy::Jet* jet);

    // To determine if there is baseline jets within bad FCAL region 
    bool hasJetInBadFCAL(const JetVector& baseJets, uint run=206248, bool isMC=false);
    bool isBadFCALJet(const Susy::Jet* jet);


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
    float elPtConeCorr(const Susy::Electron* e, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
    float elEtTopoConeCorr(const Susy::Electron* e, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                           uint nVtx, bool isMC, bool removeLeps=false);
    float muPtConeCorr(const Susy::Muon* mu, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
    float muEtConeCorr(const Susy::Muon* mu, ElectronVector& baseElectrons, MuonVector& baseMuons, 
                       uint nVtx, bool isMC, bool removeLeps=false);
  
    // Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);//, bool useNomPhiForMetSys = true);

    //
    // Methods for performing overlap removal
    //
  
    // Perform all overlap on pre objects  
    void performOverlap(ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets);

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
    //bool passLAr(int flag)     { return ( flag & PASS_LAr );     }
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

    // Pass FEB dead region check
    bool passDeadRegions(const JetVector& baseJets, const Susy::Met* met, int RunNumber, bool isMC);

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
    bool isSameFlav(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isOppSign(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isOppSign(const Susy::Tau* tau1, const Susy::Tau* tau2);
    bool isOppSign(const Susy::Lepton* lep, const Susy::Tau* tau);
    bool isSFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isSFSS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isOFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool hasSFOS(const LeptonVector& leps);
    bool hasSFSS(const LeptonVector& leps);
    bool hasOFOS(const LeptonVector& leps);
    bool hasOS(const LeptonVector& leps);
    bool hasSS(const LeptonVector& leps);
    bool hasOS(const TauVector& taus);

    // Mass calculation methods
    float Mll(const Susy::Particle* l1, const Susy::Particle* l2);
    float Mlll(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3);
    float Mllll(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3, const Susy::Lepton* l4);
    float Mjj(const Susy::Jet* j1, const Susy::Jet* j2);
    float Mlljj(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Jet* j1, const Susy::Jet* j2);
    float Mt(const Susy::Lepton* lep, const Susy::Met* met);
    float Meff(const LeptonVector& leps, const JetVector& jets, const Susy::Met* met);
    float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Susy::Met* met);

    // Z selection methods
    bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, float massWindow=10.);
    bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3, 
             float massWindow=10.);
    bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3, 
             const Susy::Lepton* l4, float massWindow=10.);
    bool isZWindow(const Susy::Lepton* l1, const Susy::Lepton* l2, 
                   float minMll=MZ-10, float maxMll=MZ+10);
    // NEW Argument useMultiLep will check for mlll and mllll in Z peak
    bool hasZ(const LeptonVector& leps, float massWindow=10., bool useMultiLep=false);
    bool hasZWindow(const LeptonVector& leps, float minMll=MZ-10, float maxMll=MZ+10);
    //void bestZ(uint& l1, uint& l2, const LeptonVector& leps, bool ignoreTau=true);

    // Safer functions, to replace the one above, which return false if no candidates found
    bool findBestZ(uint& l1, uint& l2, const LeptonVector& leps);
    bool findBestZ(uint& j1, uint& j2, const JetVector& jets);
    bool findBestW(uint& j1, uint& j2, const JetVector& jets);

    // B jets
    bool hasBJet(const JetVector& jets, float weight=MV1_85);
    bool isBJet(const Susy::Jet* jet, float weight=MV1_85);
    
    float bTagSF(const Susy::Event*, const JetVector& jets, bool useNoJVF=false,
		 std::string taggerName = "MV1", std::string OP="0_122", float opval=MV1_85,
		 BTagSys sys=BTag_NOM);    

    // Move to this once 3-lep moves to MV1_80
    //float bTagSF(const Susy::Event*, const JetVector& jets, bool useNoJVF=false,
    //std::string taggerName = "MV1", std::string OP="0_3511", float opval=MV1_80,
    //BTagSys sys=BTag_NOM);

    // 2 Lepton jet methods and counters
    bool isCentralLightJet(const Susy::Jet* jet);
    bool isCentralBJet    (const Susy::Jet* jet);
    bool isForwardJet     (const Susy::Jet* jet);

    int numberOfCLJets    (const JetVector& jets);
    int numberOfCBJets    (const JetVector& jets);
    int numberOfFJets     (const JetVector& jets);
    void getNumberOf2LepJets(const JetVector& jets, int& Ncl, int& Ncb, int& Nf);

    // MET Rel
    float getMetRel(const Susy::Met* met, const LeptonVector& leptons, const JetVector& jets, bool useForward=false);
  
    // MT2
    float getMT2(const LeptonVector& leptons, const Susy::Met* met);
    float getMT2(const TLorentzVector* lep1, const TLorentzVector* lep2, const Susy::Met* met);

    // Top Tagger methods
    bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Susy::Met* met,
		    int opt=0, float ptJetCut=0, float mEffCut=100);
    
    bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met);
  
    bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l,
		  TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int iopt1);
  
    float calcMCT(TLorentzVector v1, TLorentzVector v2);
  

    //
    // Misc methods
    //

    // Build a map of MCID -> sumw.
    // This method will loop over the input files associated with the TChain.
    // The MCID in the first entry of the tree will be used, so one CANNOT use this
    // if multiple datasets are combined into one SusyNt tree file!
    // The generator weighted cutflow histograms will then be used to calculate the total sumw for each MCID.
    // Each dataset used here must be complete, they CANNOT be spread out across multiple jobs.
    // However, one can have more than one (complete) dataset in the chain which is why we use the map.
    std::map<unsigned int, float> buildSumwMap(TChain* chain);

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

};

#endif
