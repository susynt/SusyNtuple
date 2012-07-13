#ifndef SusyCommon_SusyNtAna_h
#define SusyCommon_SusyNtAna_h

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

    //**************
    // To Be deleted
    //std::ofstream evtlist;


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
    // Default weight uses ICHEP dataset, A-B14 lumi
    // You can supply a different luminosity, but the pileup weights will still
    // correspond to A-B14
    virtual float getEventWeight(float lumi = LUMI_A_B14);
    // Use this function to scale MC to the A-B3 unblinded dataset
    // This will use the correct pileup weights for A-B3
    virtual float getEventWeight1fb();

    // Object selection
    void clearObjects();
    void selectObjects(SusyNtSys sys = NtSys_NOM);

    // Event and object dumps
    void dumpEvent();
    void dumpBaselineObjects();
    void dumpSignalObjects();

    // Debug level
    void setDebug(int dbg) { m_dbg = dbg; }
    int dbg() { return m_dbg; }

    // To debug events in input file 
    typedef std::map< unsigned int, std::set<unsigned int>* > RunEventMap;
    RunEventMap _eventList;   //run:event to debug 
    void setEvtDebug() {m_dbgEvt=true;}
    void loadEventList();
    bool processThisEvent(unsigned int run, unsigned int event);
    bool checkRunEvent(const RunEventMap &runEventMap, unsigned int run, unsigned int event);
    bool checkAndAddRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event);
    void addRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event) 
    { checkAndAddRunEvent(runEventMap, run, event); }

    // Sample name - can be used however you like
    std::string sampleName() { return m_sample; }
    void setSampleName(std::string s) { m_sample = s; }

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

    int   m_dbg;                // debug level
    bool  m_dbgEvt;             // debug events

    std::string m_sample;       // sample name string


    //
    // Object collections
    //

    ElectronVector      m_baseElectrons;        // baseline electrons
    ElectronVector      m_signalElectrons;      // signal electrons
    MuonVector          m_baseMuons;            // baseline muons
    MuonVector          m_signalMuons;          // signal muons
    LeptonVector        m_baseLeptons;          // baseline leptons
    LeptonVector        m_signalLeptons;        // signal leptons
    JetVector           m_baseJets;             // base jets
    JetVector           m_signalJets;           // signal jets
    const Susy::Met*    m_met;                  // Met

    // Timer
    TStopwatch          m_timer;

};



#endif
