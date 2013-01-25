#ifndef SusyNtuple_SusyNtTruthAna_h
#define SusyNtuple_SusyNtTruthAna_h

#include "TSelector.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "SusyNtuple/SusyNtObject.h"

#include <fstream>
#include <map>
#include <set>

class SusyNtTruthAna : public TSelector
{

  public:

    // Constructor and destructor
    SusyNtTruthAna();
    virtual ~SusyNtTruthAna(){};

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

    // Object selection
    void clearTruthObjects();
    void selectTruthObjects();

    //
    // Methods to grab objects
    //
    TruthParticleVector getPreTruthLeptons   (Susy::SusyNtObject* susyNt, int pdgId);
    TruthJetVector      getPreTruthJets      (Susy::SusyNtObject* susyNt           );
    TruthParticleVector getSignalTruthLeptons(TruthParticleVector& truthLeptons    );
    TruthJetVector      getSignalTruthJets   (TruthJetVector& truthJets            );

    // Get Baseline Objects
    void getBaselineTruthObjects(Susy::SusyNtObject*  susyNt, 
                                 TruthParticleVector& truthElectrons, 
                                 TruthParticleVector& truthMuons, 
                                 TruthParticleVector& truthTaus, 
                                 TruthJetVector&      truthJets);
    // Get Signal Objects
    void getSignalTruthObjects (TruthParticleVector& truthBaseElectrons, 
                                TruthParticleVector& truthBaseMuons, 
                                TruthParticleVector& truthBaseTaus, 
                                TruthJetVector&      truthBaseJets,
                                TruthParticleVector& truthSignalElectrons, 
                                TruthParticleVector& truthSignalMuons, 
                                TruthParticleVector& truthSignalTaus, 
                                TruthJetVector&      truthSignalJets);

    // Get Met
    Susy::TruthMet* getTruthMet(Susy::SusyNtObject* susyNt);

    // Misc
    void removeSFOSPair        (TruthParticleVector& leptons, 
                                float MllCut = 20.); // 20 GeV

    // Event and object dumps
    void dumpEvent();
    void dumpBaselineTruthObjects();
    void dumpSignalTruthObjects();

    // Set-Get options
    void setDebug(bool flag) { m_dbg = flag; }
 
    // Dump timer
    void dumpTimer();

    // Access tree
    TTree* getTree() { return m_tree; }

    ClassDef(SusyNtTruthAna, 1);

  protected:

    //
    // General
    //

    TTree* m_tree;              // Input tree (I think it actually points to a TChain)

    Long64_t m_entry;           // Current entry in the current tree (not chain index!)
    Long64_t m_chainEntry;      // Current entry in the full TChain

    int   m_dbg;                // debug level
 
    //
    // Object collections
    //
    TruthParticleVector         m_baseTruthElectrons;
    TruthParticleVector         m_baseTruthMuons;
    TruthParticleVector         m_baseTruthTaus;
    TruthJetVector              m_baseTruthJets;
    
    TruthParticleVector         m_signalTruthElectrons;
    TruthParticleVector         m_signalTruthMuons;
    TruthParticleVector         m_signalTruthTaus;
    TruthJetVector              m_signalTruthJets;
    const Susy::TruthMet*       m_truthMet;

    // Timer
    TStopwatch          m_timer;
};

#endif
