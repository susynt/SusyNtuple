#ifndef SusyCommon_SusyNtTools_h
#define SusyCommon_SusyNtTools_h


#include <iostream>

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"

/*

    SusyNtTools - a class of useful tools for reading in SusyNt (ie. D4PDs :P)

*/

// Don't pollute global namespace!!
//using namespace std;
//using namespace Susy;

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

    // Default weight uses ICHEP dataset, A-B14 lumi
    // Pileup weights correspond to A-B14 only
    virtual float getEventWeight(const Susy::Event* evt, float lumi = LUMI_A_B14);

    // Use this function to scale MC to the A-B3 unblinded dataset
    // This will use the correct pileup weights for A-B3
    virtual float getEventWeight1fb(const Susy::Event* evt);

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
    void getBaselineObjects(Susy::SusyNtObject* susyNt, ElectronVector& elecs, 
			    MuonVector& muons, TauVector& taus, JetVector& jets, 
                            SusyNtSys sys, bool selectTaus=false);
  
    // Signal objects
    ElectronVector getSignalElectrons(ElectronVector baseElecs, uint nVtx, bool isMC);
    MuonVector     getSignalMuons(MuonVector baseMuons, uint nVtx, bool isMC);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susyNt);
    TauVector      getSignalTaus(TauVector baseTaus);
    JetVector      getSignalJets(JetVector baseJets);
  
    // Get the signal objects
    void getSignalObjects(ElectronVector baseElecs, MuonVector baseMuons, 
                          TauVector baseTaus, JetVector baseJets, 
			  ElectronVector& sigElecs, MuonVector& sigMuons, 
                          TauVector& sigTaus, JetVector& sigJets,
                          uint nVtx, bool isMC);
    void getSignalObjects(Susy::SusyNtObject* susyNt, ElectronVector& sigElecs, 
			  MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets, 
                          SusyNtSys sys, uint nVtx, bool isMC, bool selectTaus=false);
    
    // Check if Signal, now with mc/data dependent iso cuts
    bool isSignalLepton(const Susy::Lepton* l, uint nVtx, bool isMC);
    bool isSignalElectron(const Susy::Electron* ele, uint nVtx, bool isMC);
    bool isSignalMuon(const Susy::Muon* mu, uint nVtx, bool isMC);
    bool isSignalTau(const Susy::Tau* tau);
    bool isSignalJet(const Susy::Jet* jet);
  
    // Build Lepton vector - we should probably sort them here
    // TODO: include taus here
    void buildLeptons(LeptonVector &lep, ElectronVector& ele, MuonVector& muo, TauVector& tau){
      for(uint ie=0; ie<ele.size(); ie++)
        lep.push_back( ele[ie] );
      for(uint im=0; im<muo.size(); im++)
        lep.push_back( muo[im] );
      for(uint it=0; it<tau.size(); it++)
        lep.push_back( tau[it] );
    };
    
    //
    // Electron, Muon isolation correction for pileup
    //
    float elEtTopoConeCorr(const Susy::Electron* e, uint nVtx, bool isMC);
    float muPtConeCorr(const Susy::Muon* m, uint nVtx, bool isMC);
    float muEtConeCorr(const Susy::Muon* m, uint nVtx, bool isMC);
  
    // Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susyNt, SusyNtSys sys);

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
    bool passLAr(int flag)     { return ( flag & PASS_LAr );     }

    // Pass Tile hot spot veto
    bool passHotSpot(int flag) { return ( flag & PASS_HotSpot ); }
  
    // Pass the Bad Jet requirement
    bool passBadJet(int flag)  { return ( flag & PASS_BadJet );  }
    
    // Pass the Bad Muon requirement
    bool passBadMuon(int flag) { return ( flag & PASS_BadMuon ); }
    
    // No cosmic muons
    bool passCosmic(int flag)  { return ( flag & PASS_Cosmic );  }
    
    // Pass All the above, incase you don't care about cut flow
    bool passAll(int flag)     { return ( flag & PASS_Event );   }
  

    //
    // Methods to get useful quantities for event, leptons, or jets
    // Moving global functions from SusyDefs here
    // 
  
    // Lepton flavor checks
    bool isSameFlav(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isSFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool isSFSS(const Susy::Lepton* l1, const Susy::Lepton* l2);
    bool hasSFOS(const LeptonVector& leps);

    // Mass calculation methods
    float Mll(const Susy::Lepton* l1, const Susy::Lepton* l2);
    float Mlll(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3);
    float Mt(const Susy::Lepton* lep, const Susy::Met* met);
    float Meff(const LeptonVector& leps, const JetVector& jets, const Susy::Met* met);

    // Z selection methods
    bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, float massWindow=10.);
    bool isZWindow(const Susy::Lepton* l1, const Susy::Lepton* l2, 
                   float minMll=MZ-10, float maxMll=MZ+10);
    bool hasZ(const LeptonVector& leps, float massWindow=10., bool ignoreTau=true);
    bool hasZWindow(const LeptonVector& leps, float minMll=MZ-10, float maxMll=MZ+10, bool ignoreTau=true);
    void bestZ(uint& l1, uint& l2, const LeptonVector& leps, bool ignoreTau=true);
    // Safer function, to replace the one above, which returns false if no SFOS pair found
    bool findBestZ(uint& l1, uint& l2, const LeptonVector& leps, bool ignoreTau=true);

    // B jets
    bool hasBJet(const JetVector& jets, float weight=MV1_60);
    bool isBJet(const Susy::Jet* jet, float weight=MV1_60);

    // MET Rel
    float getMetRel(const Susy::Met* met, const LeptonVector& leptons, const JetVector& jets, float minJetPt=-1);
  
    // MT2
    float getMT2(const LeptonVector& leptons, const Susy::Met* met);
    
    // Top Tagger methods
    bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Susy::Met* met,
		    int opt=0, float ptJetCut=30, float mEffCut=100);
    
    bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met);
  
    bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l,
		  TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int iopt1);
  
    float calcMCT(TLorentzVector v1, TLorentzVector v2);
  

    //
    // Methods to handle Trigger checking
    //
  
    // Will need to be updated if more flags are added
    //bool passTrigger(uint trig, int flag){ return (trig & flag); };
    
  protected:
    
    AnalysisType m_anaType;                 // Analysis type. currently 2-lep or 3-lep

};

#endif
