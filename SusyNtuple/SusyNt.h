#ifndef SusyCommon_SusyNt_h
#define SusyCommon_SusyNt_h


//-----------------------------------------------------------------------------------
//  SusyNt
//
//  This header file contains definitions for the SusyNt tree structure
//  The classes here were adapted from UCINtuple classes but designed
//  to be more streamlined for quick analysis.  
//  They should hold the minimal content for doing analysis.
//  If an advanced study requires many additional variables, it might be preferred 
//  to write a new derived class rather than clutter theses classes up.
//-----------------------------------------------------------------------------------

#include <iostream>

#include "TLorentzVector.h"

#include "SusyNtuple/SusyDefs.h"


namespace Susy
{

  // Particle class, base class for other object types
  class Particle : public TLorentzVector
  {
    public:
      Particle(){ clear(); }
      virtual ~Particle(){};

      unsigned int idx;   // d3pd index
      
      // Crowd wants pt, eta, phi of nominal stored
      float pt;
      float eta;
      float phi;
      float m;
      void resetTLV(){ this->SetPtEtaPhiM(pt,eta,phi,m); };
	
      // Internal state for particles.
      // Makes easier to grab right TLV
      // given systematic variations
      void setState(int sys){ resetTLV(); };

      void clear() { 
	TLorentzVector::Clear(); 
	idx = 999;      // This is not a very good choice...
      }

      // print method
      virtual void print() const {};

      // Comparison operators for sorting, etc.
      inline bool operator > (const Particle & other) const {
        return Pt() > other.Pt();
      }
      inline bool operator < (const Particle & other) const {
        return Pt() < other.Pt();
      }

      ClassDef(Particle, 1);
  };

  // Lepton class, common to electrons and muons 
  class Lepton : public Particle
  {
    public:
      Lepton(){ clear(); }
      virtual ~Lepton(){};

      // public member vars
      int q;                    // Charge
      float ptcone20;           // ptcone20 isolation
      float d0;                 // d0 extrapolated to PV 
      float errD0;              // Uncertainty on d0
      unsigned int mcType;      // MCTruthClassifier particle type
      unsigned int mcOrigin;    // MCTruthClassifier particle origin

      float effSF;              // Efficiency scale factor
      float errEffSF;           // Uncertainty on the efficiency scale factor

      unsigned int trigFlags;   // Bit word representing matched trigger chains

      // trigger matching
      // provide the trigger chain via bit mask, 
      // e.g. TRIG_mu18
      bool matchTrig(unsigned int mask) const {
        return (trigFlags & mask) == mask;
      }


      // polymorphism, baby!!
      virtual bool isEle() const { return false; }
      virtual bool isMu()  const { return false; }
      void setState(int sys){ resetTLV(); }

      // print method
      virtual void print() const {};

      // clear vars
      void clear(){
        q = ptcone20 = d0 = errD0 = mcType = mcOrigin = trigFlags = 0;
        Particle::clear();
      }
      
      ClassDef(Lepton, 4);
  };

  // Electron class
  class Electron : public Lepton
  {
    public:
      Electron(){ clear(); }
      virtual ~Electron(){};

      float clusE;              // CaloCluster energy
      float clusEta;            // CaloCluster eta
      bool mediumPP;            // isEM medium++
      bool tightPP;             // isEM tight++

      // Systematic scale factors
      float ees_up;             // Energy Scale + sigma
      float ees_dn;             // Energy Scale - sigma
      float eer_up;             // Energy Reso. + sigma
      float eer_dn;             // Energy Reso. - sigma

      // polymorphism, baby!!
      bool isEle() const { return true; }
      bool isMu()  const { return false; }

      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        clusE = clusEta = mediumPP = tightPP = 0;
	ees_up = ees_dn = eer_up = eer_dn = 0;
        Lepton::clear();
      }

      ClassDef(Electron, 1);
  };

  // Muon class
  class Muon : public Lepton
  {
    public:
      Muon(){ clear(); }
      virtual ~Muon(){};

      bool isCombined;          // Is combined muon, otherwise segment tagged

      // Systematic sf
      float ms_up;              // MS Pt + sigma
      float ms_dn;              // MS Pt - sigma
      float id_up;              // ID Pt + sigma
      float id_dn;              // ID Pt - sigma

      // polymorphism, baby!!
      bool isEle() const { return false; }
      bool isMu()  const { return true; }
      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        isCombined = 0;
	ms_up = ms_dn = id_up = id_dn = 0;
        Lepton::clear();
      }

      ClassDef(Muon, 1);
  };

  // Jet class
  class Jet : public Particle
  {
    public:
      Jet(){ clear(); }
      virtual ~Jet(){};

      float jvf;                // Jet vertex fraction
      float combNN;             // JetFitterCombNN b-tag weight
      int truthLabel;           // Flavor truth label

      // Systematics
      float jes_up;
      float jes_dn;
      float jer;

      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        jvf = combNN = 0;
	jer = jes_up = jes_dn = 0;
	truthLabel = 0;
        Particle::clear();
      }

      ClassDef(Jet, 1);
  };

  // Met class
  class Met : public TObject // public TLorentzVector // No longer inherit from particle
  {
    public:
      Met(){ clear(); }
      virtual ~Met(){};

      float Et;
      float phi;
      const TLorentzVector lv() const {
	TLorentzVector tlv;
	tlv.SetPtEtaPhiE(this->Et,0,this->phi,this->Et);
	return tlv;
      }

      // MET Composition info - do we want TLorentzVectors, TVector2, or just floats?
      float refEle;             // Ref electron term
      float refMuo;             // Ref muon term
      float refJet;             // Ref jet term
      float refCell;            // Cellout term

      int sys;

      // print vars
      void print() const;

      // clear vars
      void clear(){
        refEle = refMuo = refJet = refCell = sys = 0;
        //Particle::clear();
      }

      ClassDef(Met, 1);
  };

  // Event class
  class Event: public TObject
  {
    public:
      Event(){ clear(); }
      virtual ~Event(){};

      unsigned int run;         // run number 
      unsigned int event;       // event number
      unsigned int lb;          // lumi block number
      DataStream stream;        // DataStream enum, defined in SusyDefs.h

      unsigned int nVtx;        // number of good vertices
      float avgMu;              // average interactions per bunch crossing

      bool isMC;                // is MC flag
      unsigned int mcChannel;   // MC channel ID number (mc run number)
      float w;                  // MC generator weight

      // Reweighting and scaling
      float wPileup;            // pileup weight
      float xsec;               // cross section * kfactor * efficiency, from SUSY db
      float lumiSF;             // luminosity scale factor = integrated lumi / sum of mc weights

      // Event Flag to check for LAr, bad jet, etc. List found in SusyDefs.h under EventCheck
      int evtFlag[NtSys_N];

      // Combined normalized event weight
      float fullWeight() const { return wPileup*xsec*lumiSF; }

      // print event
      void print() const;

      // clear vars
      void clear(){
        run = event = lb = nVtx = 0;
        stream = Stream_Unknown;
        isMC = false;
        mcChannel = w = 0;
        wPileup = xsec = lumiSF = 1;
	memset(evtFlag,0,sizeof(evtFlag));
      }

      ClassDef(Event, 4);
  };

};


#endif
