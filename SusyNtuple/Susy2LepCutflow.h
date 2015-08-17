#ifndef SusyAna_Susy2LepCutflow_h
#define SusyAna_Susy2LepCutflow_h


// Common Packages

// Root Packages
#include "TTree.h"
#include "TChain.h"

// SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/TriggerTools.h"

#include <fstream>

/// Two lepton cutflow
/**
General script to implement basic selection with all signal region cut
 methods.
*/
class Susy2LepCutflow : public SusyNtAna
{

  public:

    Susy2LepCutflow();
    virtual ~Susy2LepCutflow(){};

    // Propagate the input TChain
    void setChain(TChain* chain) { m_input_chain = chain; }

    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    // Event cleaning cuts
    bool passEventCleaning(int cutflags, const MuonVector& preMuons, const MuonVector& baseMuons,
                                const JetVector& baseJets);
    // Full event selection. Specify which leptons to use.
    bool selectEvent(const LeptonVector& leptons, const LeptonVector& baseLeptons);
		     
    // Signal regions
    bool passSR1(const LeptonVector& leptons, const JetVector& jets, const Met* met);
    bool passSR2(const LeptonVector& leptons, const JetVector& jets, const Met* met);
    bool passSR3(const LeptonVector& leptons, const JetVector& jets, const Met* met);
    bool passSR4(const LeptonVector& leptons, const JetVector& jets, const Met* met);
    bool passSR5(const LeptonVector& leptons, const JetVector& jets, const Met* met);

    // Cut methods
    bool passNLepCut(const LeptonVector& leptons);
    bool passNBaseLepCut(const LeptonVector& baseLeptons);
    bool passTrigger(const LeptonVector& leptons, const Met* met);
    bool sameFlavor(const LeptonVector& leptons);
    bool oppositeFlavor(const LeptonVector& leptons);
    bool sameSign(const LeptonVector& leptons);
    bool oppositeSign(const LeptonVector& leptons);
    bool passMll(const LeptonVector& leptons, float mll = 20);

    // Signal Region Cuts
    bool passJetVeto(const JetVector& jets);
    bool passZVeto(const LeptonVector& leptons, float Zlow = 81.2, float Zhigh = 101.2);
    bool passMETRel(const Met *met, const LeptonVector& leptons, 
		    const JetVector& jets, float maxMet = 100);
    bool passbJetVeto(const JetVector& jets);
    bool passge2Jet(const JetVector& jets);
    bool passdPhi(TLorentzVector v0, TLorentzVector v1, float cut);
    bool passMT2(const LeptonVector& leptons, const Met* met, float cut);


    // Dump cutflow - if derived class uses different cut ordering,
    // override this method
    virtual void dumpEventCounters();

    // debug check
    bool debugEvent();

    ClassDef(Susy2LepCutflow, 1);

  protected:

    TChain*             m_input_chain;  // input chain being processed

    TriggerTools*       m_ntTrig;       // Trigger class object for testing trigger bits

    // Cut variables
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    bool                m_cutNBaseLep;  // apply nLep cuts to baseline leptons as well as signal

    DiLepEvtType        m_ET;           // Dilepton event type to store cf

    // Event counters
    uint                n_readin;
    uint                n_pass_grl;
    uint                n_pass_LAr;
    uint                n_pass_tileErr;
    uint                n_pass_ttc;
    uint                n_pass_BadJet;
    uint                n_pass_BadMuon;
    uint                n_pass_goodVtx;
    uint                n_pass_Cosmic;
    uint                n_pass_flavor[ET_N];
    uint                n_pass_nLep[ET_N];
    uint                n_pass_mll[ET_N];    
    uint                n_pass_os[ET_N];
    uint                n_pass_ss[ET_N];
    uint                n_pass_trig[ET_N];

    // SR1 counts
    uint                n_pass_SR1jv[ET_N];
    uint                n_pass_SR1Zv[ET_N];
    uint                n_pass_SR1MET[ET_N];

    // SR2 counts
    uint                n_pass_SR2jv[ET_N];
    uint                n_pass_SR2MET[ET_N];

    // SR3 counts
    uint                n_pass_SR3ge2j[ET_N];
    uint                n_pass_SR3Zv[ET_N];
    uint                n_pass_SR3bjv[ET_N];
    uint                n_pass_SR3mct[ET_N];
    uint                n_pass_SR3MET[ET_N];

    // SR4 counts
    uint                n_pass_SR4jv[ET_N];
    uint                n_pass_SR4MET[ET_N];
    uint                n_pass_SR4Zv[ET_N];
    uint                n_pass_SR4L0pt[ET_N];
    uint                n_pass_SR4SUMpt[ET_N];
    uint                n_pass_SR4dPhiMETLL[ET_N];
    uint                n_pass_SR4dPhiMETL1[ET_N];
    
    // SR5 counts
    uint                n_pass_SR5jv[ET_N];
    uint                n_pass_SR5Zv[ET_N];
    uint                n_pass_SR5MET[ET_N];
    uint                n_pass_SR5MT2[ET_N];
    


};

#endif
