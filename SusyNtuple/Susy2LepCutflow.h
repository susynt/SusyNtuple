#ifndef SusyAna_Susy2LepCutflow_h
#define SusyAna_Susy2LepCutflow_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

// Common Packages
#include "Mt2/mt2_bisect.h" // I don't want to recode this..

// Root Packages
#include "TTree.h"

// Susy Common
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/DilTrigLogic.h"
#include "SusyNtuple/SusyNtTools.h"

#include <fstream>

class Susy2LepCutflow : public SusyNtAna
{

  public:

    Susy2LepCutflow();
    virtual ~Susy2LepCutflow(){};

    ofstream out;

    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

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
    bool passTrigger(const LeptonVector& leptons);
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

    DilTrigLogic*       m_trigObj;      // My trigger logic class

    // Cut variables
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    bool                m_cutNBaseLep;  // apply nLep cuts to baseline leptons as well as signal

    // Event counters
    uint                n_readin;
    uint                n_pass_LAr;
    uint                n_pass_BadJet;
    uint                n_pass_BadMuon;
    uint                n_pass_Cosmic;
    uint                n_pass_nLep;
    uint                n_pass_flavor;    
    uint                n_pass_mll;    
    uint                n_pass_os;
    uint                n_pass_ss;
    uint                n_pass_trig;

    // SR1 counts
    uint                n_pass_SR1jv;
    uint                n_pass_SR1Zv;
    uint                n_pass_SR1MET;

    // SR2 counts
    uint                n_pass_SR2jv;
    uint                n_pass_SR2MET;

    // SR3 counts
    uint                n_pass_SR3ge2j;
    uint                n_pass_SR3Zv;
    uint                n_pass_SR3bjv;
    uint                n_pass_SR3mct;
    uint                n_pass_SR3MET;

    // SR4 counts
    uint                n_pass_SR4jv;
    uint                n_pass_SR4MET;
    uint                n_pass_SR4Zv;
    uint                n_pass_SR4L0pt;
    uint                n_pass_SR4SUMpt;
    uint                n_pass_SR4dPhiMETLL;
    uint                n_pass_SR4dPhiMETL1;
    
    // SR5 counts
    uint                n_pass_SR5jv;
    uint                n_pass_SR5Zv;
    uint                n_pass_SR5MET;
    uint                n_pass_SR5MT2;
    


};

#endif
