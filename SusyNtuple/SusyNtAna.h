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

#include <fstream>
#include <map>
#include <set>


// To debug events in input file 
typedef std::map< unsigned int, std::set<unsigned int>* > RunEventMap;


/*

    SusyNtAna - base class for analyzing SusyNt

*/

class SusyNtAna : public TSelector, public SusyNtTools
{

  public:

    // Constructor and destructor
    SusyNtAna();
    virtual ~SusyNtAna(){};

    // SusyNt object, access to the SusyNt variables
    Susy::SusyNtObject nt;


    //
    // TSelector methods
    //

    // Init is called every time a new TTree is attached
    virtual void    Init(TTree *tree);
    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Called at the first entry of a new file in a chain
    virtual Bool_t  Notify() { return kTRUE; }
    // Terminate is called after looping is finished
    virtual void    Terminate();
    // Due to ROOT's stupid design, need to specify version >= 2 or the tree will not connect automatically
    virtual Int_t   Version() const {
      return 2;
    }

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    // Get entry simply communicates the entry number from TSelector 
    // to this class and hence to all of the VarHandles
    virtual Int_t   GetEntry(Long64_t e, Int_t getall = 0) {
      m_entry=e;
      return kTRUE;
    }

    // Get event weight - contains generator, pileup, xsec, and lumi weights
    // Default weight uses A-L lumi and pileup weights
    // You can supply a different luminosity, but the pileup weights will store correspond to the default dataset
    virtual float getEventWeight(float lumi = LUMI_A_L, bool useSumwMap = false);
    // Temporary fixed version for n0105
    virtual float getEventWeightFixed(unsigned int mcChannel, float lumi = LUMI_A_L);
    // Use this function to scale MC to the A-B3 unblinded dataset
    // This will use the correct pileup weights for A-B3
    virtual float getEventWeightAB3();
    // Scale MC to A-B (5.83/fb)
    virtual float getEventWeightAB();

    // Object selection
    void clearObjects();
    void selectObjects(SusyNtSys sys = NtSys_NOM, bool removeLepsFromIso=false, TauID signalTauID=TauID_medium);

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

    // Toggle tau selection and overlap removal
    void setSelectTaus(bool doIt) { m_selectTaus = doIt; }

    // Print frequency
    void setPrintFreq(int freq) { m_printFreq = freq; }

    // Debug level
    void setDebug(int dbg) { m_dbg = dbg; }
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
    std::string sampleName() { return m_sample; }
    void setSampleName(std::string s) { m_sample = s; }

    // Build a map of MCID -> sumw.
    // This method will loop over the input files associated with the TChain.
    // The MCID in the first entry of the tree will be used, so one CANNOT use this
    // if multiple datasets are combined into one SusyNt tree file!
    // The generator weighted cutflow histograms will then be used to calculate the total sumw for each MCID.
    // Each dataset used here must be complete, they CANNOT be spread out across multiple jobs.
    // However, one can have more than one (complete) dataset in the chain which is why we use the map.
    void buildSumwMap(TChain* chain);
    const std::map<unsigned int, float>* getSumwMap() { return &m_sumwMap; }

    // Dump timer
    void dumpTimer();

    // Access tree
    TTree* getTree() { return m_tree; }

    ClassDef(SusyNtAna, 1);

  protected:

    //
    // General
    //

    TTree* m_tree;              // Input tree (I think it actually points to a TChain)

    Long64_t m_entry;           // Current entry in the current tree (not chain index!)
    Long64_t m_chainEntry;      // Current entry in the full TChain

    bool m_selectTaus;          // switch to toggle tau selection and OR

    int   m_printFreq;          // Number of events between printouts
    int   m_dbg;                // debug level
    bool  m_dbgEvt;             // debug events
    bool  m_duplicate;          // duplicate event
    
    std::string m_sample;       // sample name string

    // To debug events in input file 
    RunEventMap m_eventList;          //run:event to debug 
    RunEventMap m_eventListDuplicate; //Checks for duplicate run/event

    // Map of MCID -> sumw, optionaly filled at beginning of job
    std::map<unsigned int, float>       m_sumwMap;


    //
    // Object collections
    //

    ElectronVector      m_preElectrons;         // selected electrons before OR
    MuonVector          m_preMuons;             // selected muons before OR
    JetVector           m_preJets;              // selected jets before OR

    ElectronVector      m_baseElectrons;        // baseline electrons
    MuonVector          m_baseMuons;            // baseline muons
    LeptonVector        m_baseLeptons;          // baseline leptons
    TauVector           m_baseTaus;             // baseline taus
    JetVector           m_baseJets;             // baseline jets

    ElectronVector      m_signalElectrons;      // signal electrons
    MuonVector          m_signalMuons;          // signal muons
    LeptonVector        m_signalLeptons;        // signal leptons
    TauVector           m_signalTaus;           // signal taus
    JetVector           m_signalJets;           // signal jets
    JetVector           m_signalJets2Lep;       // signal jets for 2 Lep

    // New organization of tau selections
    TauVector           m_mediumTaus;           // taus with medium ID
    TauVector           m_tightTaus;            // taus with tight ID

    const Susy::Met*    m_met;                  // Met

    // Timer
    TStopwatch          m_timer;

};



#endif
