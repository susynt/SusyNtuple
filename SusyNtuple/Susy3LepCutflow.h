#ifndef SusyAna_Susy3LepCutflow_h
#define SusyAna_Susy3LepCutflow_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////


// Root Packages
#include "TTree.h"

// Susy Common
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/TrilTrigLogic.h"

#include <fstream>

class Susy3LepCutflow : public SusyNtAna
{

  public:

    Susy3LepCutflow();
    virtual ~Susy3LepCutflow(){};

    // Output Text File
    ofstream out;

    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    // Full event selection. Specify which leptons to use.
    bool selectEvent(const LeptonVector& leptons, const JetVector& jets, const Susy::Met* met);
		     
    // Cut methods
    bool passFCal();
    bool passNLepCut(const LeptonVector& leptons);
    bool passTrigger(const LeptonVector& leptons);
    bool passSFOSCut(const LeptonVector& leptons);
    bool passMetCut(const Susy::Met* met);
    bool passZCut(const LeptonVector& leptons);
    bool passBJetCut();
    bool passMtCut(const LeptonVector& leptons, const Susy::Met* met);
            
    // Dump cutflow - if derived class uses different cut ordering,
    // override this method
    virtual void dumpEventCounters();

    // Selection region
    void setSelection(std::string s) { m_sel = s; }

    // MC Weighter
    void setMCWeighter(MCWeighter* mcw) { m_mcWeighter = mcw; }

    // debug check
    bool debugEvent();


    ClassDef(Susy3LepCutflow, 1);

  protected:

    std::string         m_sel;          // event selection string

    MCWeighter*         m_mcWeighter;   // My MC weight class
    TrilTrigLogic*      m_trigObj;      // My trigger logic class

    // Cut variables
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    bool                m_selectZ;      // switch to select Zs
    bool                m_vetoZ;        // switch to veto Zs
    bool                m_selectB;      // switch to select b-tagged jets
    bool                m_vetoB;        // switch to veto b-tagged jets
    bool                m_selectSFOS;   // switch to select SFOS pairs
    bool                m_vetoSFOS;     // switch to veto SFOS pairs
    double              m_metMin;       // min MET cut
    double              m_minMt;        // minimum Mt cut

    bool                m_writeOut;     // switch to control output dump

    // Event counters
    uint                n_readin;
    uint                n_pass_LAr;
    uint                n_pass_HotSpot;
    uint                n_pass_BadJet;
    uint                n_pass_BadMuon;
    uint                n_pass_Cosmic;
    uint                n_pass_nLep;
    uint                n_pass_trig;
    uint                n_pass_sfos;
    uint                n_pass_met;
    uint                n_pass_z;
    uint                n_pass_bjet;
    uint                n_pass_mt;
};

#endif
