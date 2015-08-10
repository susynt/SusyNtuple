#ifndef SusyAna_Susy3LepCutflow_h
#define SusyAna_Susy3LepCutflow_h


// Root Packages
#include "TTree.h"

// Susy Common
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

#include <fstream>

/// Three lepton cutflow
/**
General script to implement basic selection with all signal region cut
methods.
*/

class Susy3LepCutflow : public SusyNtAna
{

  public:

    Susy3LepCutflow();
    virtual ~Susy3LepCutflow(){};

    // Output Text File
    std::ofstream out;

    // Init is called when TTree (or TChain) is attached
    virtual void    Init(TTree* tree);
    // Begin is called before looping on entries
    virtual void    Begin(TTree* tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    // Book histograms
    void bookHistos();

    // Full event selection. Specify which leptons to use.
    bool selectEvent(const LeptonVector& leptons, const TauVector& taus, 
                     const JetVector& jets, const Susy::Met* met);

    // Fill histograms
    void fillHistos(const LeptonVector& leptons, const TauVector& taus,
                    const JetVector& jets, const Susy::Met* met, float weight);

    // Finalize histograms
    void finalizeHistos();
		     
    // Cut methods
//    bool passFCal();
    bool passNLepCut(const LeptonVector& leptons);
    bool passNTauCut(const TauVector& taus);
    bool passTrigger(const LeptonVector& leptons);
    bool passSFOSCut(const LeptonVector& leptons);
    bool passMetCut(const Susy::Met* met);
    bool passZCut(const LeptonVector& leptons);
    bool passBJetCut();
    bool passMtCut(const LeptonVector& leptons, const Susy::Met* met);

    // Event weighting
    float getLeptonSF(const LeptonVector& leptons);
    float getTauSF(const TauVector& taus);
            
    // Dump cutflow - if derived class uses different cut ordering,
    // override this method
    virtual void dumpEventCounters();

    // Selection region
    void setSelection(std::string s) { m_sel = s; }

    // debug check
    bool debugEvent();


    ClassDef(Susy3LepCutflow, 1);

  protected:

    std::string         m_sel;          // event selection string


    // Cut variables
    uint                m_nBaseLepMin;  // min base leptons
    uint                m_nBaseLepMax;  // max base leptons
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    uint                m_nTauMin;      // min taus
    uint                m_nTauMax;      // max taus
    float               m_baseLepMinDR; // min dR between base leptons
    bool                m_selectSFOS;   // switch to select SFOS pairs
    bool                m_vetoSFOS;     // switch to veto SFOS pairs
    bool                m_selectZ;      // switch to select Zs
    bool                m_vetoZ;        // switch to veto Zs
    bool                m_selectB;      // switch to select b-tagged jets
    bool                m_vetoB;        // switch to veto b-tagged jets
    double              m_metMin;       // min MET cut
    double              m_mtMin;        // minimum Mt cut

    bool                m_writeOut;     // switch to control output dump

    // Event counters
    uint                n_readin;
    uint                n_pass_hotSpot;
    uint                n_pass_badJet;
    uint                n_pass_badMuon;
    uint                n_pass_cosmic;
    uint                n_pass_feb;
    uint                n_pass_nLep;
    uint                n_pass_nTau;
    uint                n_pass_trig;
    uint                n_pass_sfos;
    uint                n_pass_z;
    uint                n_pass_met;
    uint                n_pass_bJet;
    uint                n_pass_mt;

    // Final estimate weighted to full lumi
    float               n_evt_tot;
};

#endif
