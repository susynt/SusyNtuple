#ifndef SusyNtuple_SusyNtAna_h
#define SusyNtuple_SusyNtAna_h

#include "TSelector.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/SusyNtSys.h"

#include <fstream>
#include <map>
#include <set>


// To debug events in input file 
typedef std::map< unsigned int, std::set<unsigned int>* > RunEventMap;


///    SusyNtAna - base class for analyzing SusyNt
class SusyNtAna : public TSelector
{

  public:

    /// Constructor and destructor
    SusyNtAna();
    virtual ~SusyNtAna(){};

    /// SusyNt object, access to the SusyNt variables
    Susy::SusyNtObject nt;
    /// helper tools and functions
    SusyNtTools m_nttools;
    inline void setAnaType(AnalysisType v){ m_nttools.setAnaType(v); }
    /*const*/ SusyNtTools& nttools() /*const*/ { return m_nttools; } // DG should be const

    //
    // TSelector methods
    //

    /// Init is called every time a new TTree is attached
    virtual void    Init(TTree *tree);
    /// Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    /// Called at the first entry of a new file in a chain
    virtual Bool_t  Notify() { return kTRUE; }
    /// Terminate is called after looping is finished
    virtual void    Terminate();
    /** Due to ROOT's stupid design, need to specify version >= 2 or the tree will not connect automatically */
    virtual Int_t   Version() const {
      return 2;
    }

    /// Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    /** Get entry simply communicates the entry number from TSelector 
        to this class and hence to all of the VarHandles */
    virtual Int_t   GetEntry(Long64_t e, Int_t getall = 0) {
      m_entry=e;
      return kTRUE;
    }

    // Object selection
    void clearObjects();
    void selectObjects(Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM,
                       bool removeLepsFromIso=false, 
                       TauID signalTauID=TauID_medium);

    // Cleaning cuts
    int cleaningCutFlags();

    // Event and object dumps
    void dumpEvent();
    void dumpNtupleObjects();
    void dumpBaselineObjects();
    void dumpBaselineLeptons();
    void dumpBaselineJets();
    void dumpSignalObjects();
    void dumpSignalLeptons();
    void dumpSignalJets();

    /// Toggle tau selection and overlap removal
    void setSelectTaus(bool doIt) { m_selectTaus = doIt; }

    /// Print frequency
    void setPrintFreq(int freq) { m_printFreq = freq; }

    /// Debug level
    void setDebug(int dbg) { m_dbg = dbg; m_mcWeighter.setVerbose(dbg); }
    int dbg() { return m_dbg; }

    void toggleCheckDuplicates(bool b=true) { m_duplicate = b; }
    bool checkDuplicate() { return m_duplicate; }
    
    void setEvtDebug() { m_dbgEvt = true; }
    bool dbgEvt() const { return m_dbgEvt; }
    void loadEventList();
    bool processThisEvent(unsigned int run, unsigned int event);
    bool checkRunEvent(const RunEventMap &runEventMap, unsigned int run, unsigned int event);
    bool checkAndAddRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event);
    void addRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event) 
    { checkAndAddRunEvent(runEventMap, run, event); }

    bool isDuplicate(unsigned int run, unsigned int event);

    // Sample name - can be used however you like
    std::string sampleName() const { return m_sample; }
    void setSampleName(std::string s) { m_sample = s; }

    /// getter to be used from outside (set xsec dir, access weight, etc.)
    MCWeighter& mcWeighter() { return m_mcWeighter; }

    /// Dump timer
    void dumpTimer();

    /// Access tree
    TTree* getTree() { return m_tree; }

    ClassDef(SusyNtAna, 1);

  protected:

    //
    // General
    //

    TTree* m_tree;              ///< Input tree (I think it actually points to a TChain)

    Long64_t m_entry;           ///< Current entry in the current tree (not chain index!)
    Long64_t m_chainEntry;      ///< Current entry in the full TChain

    bool m_selectTaus;          ///< switch to toggle tau selection and OR

    int   m_printFreq;          ///< Number of events between printouts
    int   m_dbg;                ///< debug level
    bool  m_dbgEvt;             ///< debug events
    bool  m_duplicate;          ///< duplicate event
    
    std::string m_sample;       ///< sample name string

    // To debug events in input file 
    RunEventMap m_eventList;          ///< run:event to debug 
    RunEventMap m_eventListDuplicate; ///< Checks for duplicate run/event

    MCWeighter m_mcWeighter;   // provides MC normalization and event weight


    //
    // Object collections
    //

    ElectronVector      m_preElectrons;         ///< selected electrons before OR
    MuonVector          m_preMuons;             ///< selected muons before OR
    JetVector           m_preJets;              ///< selected jets before OR

    ElectronVector      m_baseElectrons;        ///< baseline electrons
    MuonVector          m_baseMuons;            ///< baseline muons
    LeptonVector        m_baseLeptons;          ///< baseline leptons
    TauVector           m_baseTaus;             ///< baseline taus
    JetVector           m_baseJets;             ///< baseline jets

    ElectronVector      m_signalElectrons;      ///< signal electrons
    MuonVector          m_signalMuons;          ///< signal muons
    LeptonVector        m_signalLeptons;        ///< signal leptons
    TauVector           m_signalTaus;           ///< signal taus
    JetVector           m_signalJets;           ///< signal jets
    JetVector           m_signalJets2Lep;       ///< signal jets for 2 Lep

    // New organization of tau selections
    TauVector           m_mediumTaus;           ///< taus with medium ID
    TauVector           m_tightTaus;            ///< taus with tight ID

    const Susy::Met*    m_met;                  ///< Met
    const Susy::TrackMet*    m_trackMet;        ///< TrackMet

    /// Timer
    TStopwatch          m_timer;

};



#endif
