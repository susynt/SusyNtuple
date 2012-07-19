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
    ElectronVector getPreElectrons(Susy::SusyNtObject* susy_nt, SusyNtSys sys);
    MuonVector     getPreMuons(Susy::SusyNtObject* susy_nt, SusyNtSys sys);
    JetVector      getPreJets(Susy::SusyNtObject* susy_nt, SusyNtSys sys);
  
    // Get Baseline objects. Pre + overlap removal.
    // Might be able to add options in case of overlap 
    // removal differances in 2-lep vs. 3-lep.
    void getBaselineObjects(Susy::SusyNtObject* susy_nt, ElectronVector &elecs, 
			    MuonVector &muons, JetVector &jets, SusyNtSys sys);
  
    // Signal objects
    ElectronVector getSignalElectrons(ElectronVector bsElecs, uint nVtx, bool isMC);
    MuonVector     getSignalMuons(MuonVector bsMuons, uint nVtx, bool isMC);
    JetVector      getSignalJets(JetVector bsJets);
    PhotonVector   getSignalPhotons(Susy::SusyNtObject* susy_nt);
  
    // Get the signal objects
    void getSignalObjects(ElectronVector bsElecs, MuonVector bsMuons, JetVector bsJets,
			  ElectronVector &sigElecs, MuonVector &sigMuons, JetVector &sigJets,
                          uint nVtx, bool isMC);
    void getSignalObjects(Susy::SusyNtObject* susy_nt, ElectronVector &sigElecs, 
			  MuonVector &sigMuons, JetVector &sigJets, SusyNtSys sys,
                          uint nVtx, bool isMC);
    
    // Check if Signal, now with mc/data dependent iso cuts
    bool isSignalLepton(const Susy::Lepton* l, uint nVtx, bool isMC);
    bool isSignalElectron(const Susy::Electron* ele, uint nVtx, bool isMC);
    bool isSignalMuon(const Susy::Muon* mu, uint nVtx, bool isMC);
    bool isSignalJet(const Susy::Jet* jet);
  
    // Build Lepton vector - we should probably sort them here
    void buildLeptons(LeptonVector &lep, ElectronVector &ele, MuonVector &muo){
      for(uint ie=0; ie<ele.size(); ++ie)
        lep.push_back( ele.at( ie ) );
      for(uint im=0; im<muo.size(); ++im)
        lep.push_back( muo.at( im ) );
    };
    
    //
    //Electron, Muon isolation correction for pileup
    //
    float elEtTopoConeCorr(const Susy::Electron* e, uint nVtx, bool isMC);
    float muPtConeCorr(const Susy::Muon* m, uint nVtx, bool isMC);
    float muEtConeCorr(const Susy::Muon* m, uint nVtx, bool isMC);
  
    // Get the Met, for the appropriate systematic
    Susy::Met* getMet(Susy::SusyNtObject* susy_nt, SusyNtSys sys);

    //
    // Methods for performing overlap removal
    //
  
    // Perform all overlap on pre objects  
    void performOverlap(ElectronVector &elecs, MuonVector &muons, JetVector &jets);

    // e-e overlap
    void e_e_overlap(ElectronVector &elecs, float minDr);
  
    // e-j overlap
    void e_j_overlap(ElectronVector &elecs, JetVector &jets, float minDr, bool removeJets = true);
  
    // m-j overlap
    void m_j_overlap(MuonVector &muons, JetVector jets, float minDr);

    // e-m overlap 
    void e_m_overlap(ElectronVector &elecs, MuonVector &muons, float minDr);
  
    // m-m overlap
    void m_m_overlap(MuonVector& muons, float minDr);
  
    // Msfos cuts now applied along with overlap removal
    void removeSFOSPair(ElectronVector &elecs, float MllCut);
    void removeSFOSPair(MuonVector &muons, float MllCut);
  
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
    // Top Tagger
    // 
    bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Susy::Met* met,
		    int opt=0, float ptJetCut=30, float mEffCut=100);
    
    bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met);
  
    bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l,
		  TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int iopt1);
  
    float calcMCT(TLorentzVector v1, TLorentzVector v2);
  
    //
    // Methods to get useful quantities
    // 
  
    float getMetRel(const Susy::Met* met, const LeptonVector& leptons, const JetVector& jets, float minJetPt=-1);
  
    float getMT2(const LeptonVector& leptons, const Susy::Met* met);
    

    //
    // Methods to handle Trigger checking
    //
  
    // Will need to be updated if more flags are added
    //bool passTrigger(uint trig, int flag){ return (trig & flag); };
    
  protected:
    
    AnalysisType m_anaType;                 // Analysis type. currently 2-lep or 3-lep

};

#endif
