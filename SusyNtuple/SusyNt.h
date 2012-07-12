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
        pt = eta = phi = m = 0;
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
      float ptcone30;           // ptcone30 isolation
      // TODO: move additional iso variables here??

      float d0;                 // d0 extrapolated to PV 
      float errD0;              // Uncertainty on d0
      float z0;                 // z0 extrapolated to PV 
      float errZ0;              // Uncertainty on z0

      float d0Unbiased;         // Unbiased d0
      float errD0Unbiased;      // Uncertainty on unbiased d0
      float z0Unbiased;         // Unbiased z0
      float errZ0Unbiased;      // Uncertainty on unbiased z0

      unsigned int mcType;      // MCTruthClassifier particle type
      unsigned int mcOrigin;    // MCTruthClassifier particle origin

      float effSF;              // Efficiency scale factor
      float errEffSF;           // Uncertainty on the efficiency scale factor

      unsigned int trigFlags;   // Bit word representing matched trigger chains

      // functions to return impact parameter variables
      // Note that these are not absolute valued!
      float d0Sig() const {
        return d0/errD0;
      }
      float z0SinTheta() const {
        return z0 * sin(Theta());
      }

      // trigger matching
      // provide the trigger chain via bit mask, e.g. TRIG_mu18
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
        q = ptcone20 = ptcone30 = 0;
        d0 = errD0 = z0 = errZ0 = 0;
        mcType = mcOrigin = 0;
        effSF = errEffSF = 0;
        trigFlags = 0;
        Particle::clear();
      }
      
      ClassDef(Lepton, 7);
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

      // New isolation variables, put them here for now
      float etcone30Corr;       // Pt and ED corrected etcone iso
      float topoEtcone30Corr;   // Corrected topo clus based iso

      // Systematic scale factors
      float ees_up;             // Energy Scale + sigma
      float ees_dn;             // Energy Scale - sigma
      float eer_up;             // Energy Reso. + sigma
      float eer_dn;             // Energy Reso. - sigma

      // polymorphism, baby!!
      bool isEle() const { return true;  }
      bool isMu()  const { return false; }

      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        clusE = clusEta = mediumPP = tightPP = 0;
        etcone30Corr = topoEtcone30Corr = 0;
	ees_up = ees_dn = eer_up = eer_dn = 0;
        Lepton::clear();
      }

      ClassDef(Electron, 2);
  };

  // Muon class
  class Muon : public Lepton
  {
    public:
      Muon(){ clear(); }
      virtual ~Muon(){};

      bool isCombined;          // Is combined muon, otherwise segment tagged

      float thetaPV;            // Theta extrapolated to PV, by request.
      float etcone30;           // etcone iso

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
        thetaPV = etcone30 = 0;
	ms_up = ms_dn = id_up = id_dn = 0;
        Lepton::clear();
      }

      ClassDef(Muon, 2);
  };

  // Jet class
  class Jet : public Particle
  {
    public:
      Jet(){ clear(); }
      virtual ~Jet(){};

      float jvf;                // Jet vertex fraction
      int truthLabel;           // Flavor truth label

      // btagging
      float sv0;                // SV0 btag weight
      float combNN;             // JetFitterCombNN btag weight
      float mv1;                // MV1 btag weight

      // Systematics
      float jes_up;             // jet energy scale up
      float jes_dn;             // jet energy scale down
      float jer;                // jet energy resolution

      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        jvf = truthLabel = 0;
        sv0 = combNN = mv1 = 0;
	jer = jes_up = jes_dn = 0;
        Particle::clear();
      }

      ClassDef(Jet, 2);
  };

  // Photon class
  class Photon : public Particle
  {
    public:
      Photon(){ clear(); };
      virtual ~Photon(){};

      // Conversion Information
      bool isConv;

      // Systematics
      //float pes_up;        // Photon Energy Scale up
      //float pes_dn;        // Photon Energy Scale down
      //float per_up;        // Photon Energy Resolution up
      //float per_dn;        // Photon Energy Resolution down

      void setState(int sys){ resetTLV();};
      
      // print method
      void print() const {};

      // clear
      void clear(){
	//pes_up = pes_dn = per_up = per_dn = 0;
	isConv = false;
	Particle::clear();
      };
      
      ClassDef(Photon, 1);
  };

  // Met class
  class Met : public TObject // public TLorentzVector // No longer inherit from particle
  {
    public:
      Met(){ clear(); }
      virtual ~Met(){};

      float Et;
      float phi;

      // Build a TLorentzVector on the fly
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

      int sys;                  // ???

      // print vars
      void print() const;

      // clear vars
      void clear(){
        Et = phi = 0;
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

      bool isMC;                // is MC flag
      unsigned int mcChannel;   // MC channel ID number (mc run number)
      float w;                  // MC generator weight

      unsigned int nVtx;        // number of good vertices
      float avgMu;              // average interactions per bunch crossing

      unsigned int trigFlags;   // Event level trigger bits

      // Check trigger firing
      // provide the trigger chain via bit mask, e.g. TRIG_mu18
      bool passTrig(unsigned int mask) const {
        return (trigFlags & mask) == mask;
      }

      // Event Flag to check for LAr, bad jet, etc. List found in SusyDefs.h under EventCheck
      // This is somewhat poorly named...
      int evtFlag[NtSys_N];

      // Reweighting and scaling
      float wPileup;            // pileup weight
      float wPileup1fb;         // pileup weight for 2012 period A-B3 only
      float xsec;               // cross section * kfactor * efficiency, from SUSY db
      float lumiSF;             // luminosity scale factor = integrated lumi / sum of mc weights
      float sumw;               // Sum of generator weights (I may drop the lumiSF above...)
      float pdfSF;              // PDF weight, for scaling 7TeV MC to 8TeV

      // Combined normalized event weight
      float fullWeight() const { return wPileup*xsec*lumiSF; }

      // print event
      void print() const;

      // clear vars
      void clear(){
        run = event = lb = 0;
        stream = Stream_Unknown;
        isMC = false;
        mcChannel = w = 0;
        nVtx = avgMu = trigFlags = 0;
	memset(evtFlag,0,sizeof(evtFlag));
        wPileup = wPileup1fb = xsec = lumiSF = sumw = pdfSF = 0;
      }

      ClassDef(Event, 9);
  };

};


#endif
