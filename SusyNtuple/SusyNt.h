#ifndef SusyNtuple_SusyNt_h
#define SusyNtuple_SusyNt_h


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

      unsigned int larError;    // LAr error flag

      unsigned int nVtx;        // number of good vertices
      float avgMu;              // average interactions per bunch crossing
      
      int hfor;                 // heavy flavor overlap removal decision

      int susyFinalState;       // Susy process
      int hDecay;               // Higgs decay (see WhTruthExtractor)
      bool eventWithSusyProp;   // Event generated involving SUSY propagators (See Serhan's msg http://goo.gl/ucwl9)

      float mllMcTruth;         // mll from mcTruth (needed to avoid samples' overlap, filled for Z->ll samples)
      bool passMllForAlpgen;    // computed from value above; see MultiLep/TruthTools for details

      unsigned int trigFlags;   // Event level trigger bits

      // Check trigger firing
      // provide the trigger chain via bit mask, e.g. TRIG_mu18
      bool passTrig(unsigned int mask) const {
        return (trigFlags & mask) == mask;
      }

      // Event Flag to check for LAr, bad jet, etc. List found in SusyDefs.h under EventCheck
      // This will be replaced
      int evtFlag[NtSys_N];

      // Event cleaning cut flags. The bits are defined in SusyDefs as EventCleaningCuts
      unsigned int cutFlags[NtSys_N];

      // Reweighting and scaling
      float wPileup;            // pileup weight for full dataset
      float wPileupAB3;         // pileup weight for 2012 period A-B3 only
      float wPileupAB;          // pileup weight for 2012 period A-B only
      float wPileupIL;          // pileup weight for 2012 period I,L only (TEMPORARY)
      float wPileupAE;          // pileup weight for HCP dataset A-E
      float xsec;               // cross section * kfactor * efficiency, from SUSY db
      float errXsec;            // cross section uncertainty
      float sumw;               // Sum of generator weights 
      float pdfSF;              // PDF weight, for scaling 7TeV MC to 8TeV

      // PDF Systematic information
      int pdf_id1;
      int pdf_id2;
      double pdf_x1;     
      double pdf_x2;
      double pdf_scale;

      // print event
      void print() const;

      // clear vars
      void clear(){
        run = event = lb = 0;
        stream = Stream_Unknown;
        isMC = false;
        mcChannel = w = 0;
        larError = 0;
        nVtx = avgMu = trigFlags = 0;
        hfor = -1;
        susyFinalState = -1;
        mllMcTruth = -1.0;
        hDecay = -1;
        eventWithSusyProp = false;
        passMllForAlpgen = true;
        memset(evtFlag,0,sizeof(evtFlag));
        memset(cutFlags,0,sizeof(cutFlags));
        wPileup = wPileupAB3 = wPileupAB = wPileupIL = wPileupAE = 0;
        xsec = errXsec = sumw = pdfSF = 0;
        pdf_id1 = pdf_id2 = pdf_x1 = pdf_x2 = pdf_scale = 0;
      }

      ClassDef(Event, 23);
  };

  // Particle class, base class for other object types
  class Particle : public TLorentzVector
  {
    public:
      Particle(){ clear(); }
      virtual ~Particle(){};
      /** Copy constructor */
      Particle(const Particle &);
      /** Assignment operator */
      Particle& operator=(const Particle &);

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
      /** Copy constructor */
      Lepton(const Lepton &);
      /** Assignment operator */
      Lepton& operator=(const Lepton &);

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

      bool matched2TruthLepton; // flag from RecoTruthMatch::Matched2TruthLepton
      int truthType;            // RecoTruthMatch::type

      float effSF;              // Efficiency scale factor
      float errEffSF;           // Uncertainty on the efficiency scale factor

      unsigned int trigFlags;   // Bit word representing matched trigger chains

      // functions to return impact parameter variables
      // Note that these are not absolute valued!
      float d0Sig(bool unbiased=false) const {
        if(unbiased) return d0Unbiased/errD0Unbiased;
	else return d0/errD0;
      }
      float z0SinTheta(bool unbiased=false) const {
        if(unbiased) return z0Unbiased * sin(Theta());
	else return z0 * sin(Theta());
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
	d0Unbiased=errD0Unbiased=z0Unbiased=errZ0Unbiased=0;
        mcType = mcOrigin = 0;
        matched2TruthLepton = false;
        truthType = -1;
        effSF = 1; 
        errEffSF = 0;
        trigFlags = 0;
        Particle::clear();
      }
      
      ClassDef(Lepton, 10);
  };

  // Electron class
  class Electron : public Lepton
  {
    public:
      Electron(){ clear(); }
      virtual ~Electron(){};
      Electron(const Electron &);
      /** Assignment operator */
      Electron& operator=(const Electron &);

      float clusE;              // CaloCluster energy
      float clusEta;            // CaloCluster eta
      float clusPhi;            // CaloCluster phi
      float trackPt;            // ID track pt
      bool mediumPP;            // isEM medium++
      bool tightPP;             // isEM tight++

      // New isolation variables, put them here for now
      float etcone30Corr;       // Pt and ED corrected etcone iso
      float topoEtcone30Corr;   // Corrected topo clus based iso

      bool isChargeFlip;        // Charge flip flag from RecoTruthMatch

      // polymorphism, baby!!
      bool isEle() const { return true;  }
      bool isMu()  const { return false; }

      // Systematic scale factors
      //float ees_up;             // Energy Scale + sigma
      //float ees_dn;             // Energy Scale - sigma
      float ees_z_up;           // Energy Scale Z + sigma
      float ees_z_dn;           // Energy Scale Z - sigma
      float ees_mat_up;         // Energy Scale Material + sigma
      float ees_mat_dn;         // Energy Scale Material - sigma
      float ees_ps_up;          // Energy Scale Presampler + sigma
      float ees_ps_dn;          // Energy Scale Presampler - sigma
      float ees_low_up;         // Energy Scale Low Pt + sigma
      float ees_low_dn;         // Energy Scale Low Pt - sigma
      float eer_up;             // Energy Reso. + sigma
      float eer_dn;             // Energy Reso. - sigma

      void setState(int sys);

      // print method
      void print() const;

      // clear vars
      void clear(){
        clusE = clusEta = clusPhi = trackPt = 0;
        mediumPP = tightPP = false;
        etcone30Corr = topoEtcone30Corr = 0;
        isChargeFlip = false;
	//ees_up = ees_dn = eer_up = eer_dn = 0;
	ees_z_up = ees_z_dn = ees_mat_up = ees_mat_dn = 0;
	ees_ps_up = ees_ps_dn = ees_low_up = ees_low_dn = 0;
	eer_up = eer_dn = 0;
        Lepton::clear();
      }

      ClassDef(Electron, 5);
  };

  // Muon class
  class Muon : public Lepton
  {
    public:
      Muon(){ clear(); }
      virtual ~Muon(){};
      Muon(const Muon &);
      /** Assignment operator */
      Muon& operator=(const Muon &);

      bool isCombined;          // Is combined muon, otherwise segment tagged

      float idTrackPt;          // ID track pt
      float idTrackEta;         // ID track eta
      float idTrackPhi;         // ID track phi
      int idTrackQ;             // ID track charge

      float msTrackPt;          // MS track pt
      float msTrackEta;         // MS track pt
      float msTrackPhi;         // MS track pt
      int msTrackQ;             // ID track charge

      float thetaPV;            // Theta extrapolated to PV, by request.
      float etcone30;           // etcone iso
      float ptcone30ElStyle;    // ptcone with electron style tracks

      // Variables for charge misid
      // TODO: these variables are not currently used???
      float id_theta;
      float id_phi;
      float id_qoverp;
      float ms_theta;
      float ms_phi;
      float ms_qoverp;

      bool isBadMuon;           // Bad muon flag from SUSYTools
      bool isCosmic;            // Cosmic muon flag from SUSYTools
      
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
        idTrackPt = idTrackEta = idTrackPhi = 0;
        msTrackPt = msTrackEta = msTrackPhi = 0;
	idTrackQ  = msTrackQ = 0;
        thetaPV = etcone30 = ptcone30ElStyle = 0;
        id_theta = id_phi = id_qoverp = 0;
        ms_theta = ms_phi = ms_qoverp = 0;
        isBadMuon = isCosmic = false;
	ms_up = ms_dn = id_up = id_dn = 0;

        Lepton::clear();
      }

      ClassDef(Muon, 6);
  };

  // Tau class
  class Tau : public Particle
  {
    public:
      Tau() { clear(); }
      virtual ~Tau(){};
      Tau(const Tau &);
      /** Assignment operator */
      Tau& operator=(const Tau &);

      int q;                    // Charge
      int author;               // reconstruction author
      int nTrack;               // number of tracks
      float eleBDT;             // electron BDT score
      float jetBDT;             // jet BDT score
      int jetBDTSigLoose;       // jet BDT working point
      int jetBDTSigMedium;      // jet BDT working point
      int jetBDTSigTight;       // jet BDT working point
      int eleBDTLoose;          // ele BDT working point
      int eleBDTMedium;         // ele BDT working point
      int eleBDTTight;          // ele BDT working point
      int muonVeto;             // muon veto flag

      bool trueTau;             // trueTauAssocSmall_matched d3pd variable

      bool matched2TruthLepton; // flag from RecoTruthMatch::Matched2TruthLepton
      int truthType;            // RecoTauMatch::TauFakeType
      int detailedTruthType;    // RecoTauMatch::TauDetailedFakeType

      // Taus have a lot of SFs
      float looseEffSF;         // Loose efficiency scale factor
      float errLooseEffSF;      // Loose efficiency scale factor uncertaintiy
      float mediumEffSF;        // Medium efficiency scale factor
      float errMediumEffSF;     // Medium efficiency scale factor uncertainty
      float tightEffSF;         // Tight efficiency scale factor
      float errTightEffSF;      // Tight efficiency scale factor uncertainty
      float looseEVetoSF;       // Loose efficiency scale factor
      float errLooseEVetoSF;    // Loose efficiency scale factor uncertaintiy
      float mediumEVetoSF;      // Medium efficiency scale factor
      float errMediumEVetoSF;   // Medium efficiency scale factor uncertainty
      float tightEVetoSF;       // Tight efficiency scale factor
      float errTightEVetoSF;    // Tight efficiency scale factor uncertainty
      //float looseEleSF
      //float effSF;            // Efficiency scale factor
      //float errEffSF;         // Uncertainty on the efficiency scale factor

      // systematic factors
      float tes_up;             // tau energy scale + sigma
      float tes_dn;             // tau energy scale - sigma

      unsigned int trigFlags;   // Bit word representing matched trigger chains

      // trigger matching
      // provide the trigger chain via bit mask, e.g. TRIG_mu18
      bool matchTrig(unsigned int mask) const {
        return (trigFlags & mask) == mask;
      }

      // set systematic state
      void setState(int sys);

      // print method
      void print() const;

      void clear(){
        author = nTrack = 0;
        eleBDT = jetBDT = 0;
        jetBDTSigLoose = jetBDTSigMedium = jetBDTSigTight = 0;
        eleBDTLoose = eleBDTMedium = eleBDTTight = 0;
        muonVeto = 0;
        trueTau = false;
        matched2TruthLepton = false;
        truthType = detailedTruthType = -1;
        //effSF = errEffSF = 0;
        looseEffSF = mediumEffSF = tightEffSF = 1;
        errLooseEffSF = errMediumEffSF = errTightEffSF = 0;
        looseEVetoSF = mediumEVetoSF = tightEVetoSF = 1;
        errLooseEVetoSF = errMediumEVetoSF = errTightEVetoSF = 0;
        tes_up = tes_dn = 0;
        trigFlags = 0;
        Particle::clear();
      }

      ClassDef(Tau, 5);
  };

  // Photon class
  class Photon : public Particle
  {
    public:
      Photon(){ clear(); };
      virtual ~Photon(){};
      Photon(const Photon &);
      /** Assignment operator */
      Photon& operator=(const Photon &);

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

  // Jet class
  class Jet : public Particle
  {
    public:
      Jet(){ clear(); }
      virtual ~Jet(){};
      Jet(const Jet &);
      /** Assignment operator */
      Jet& operator=(const Jet &);

      float jvf;                // Jet vertex fraction
      float detEta;             // Detector eta
      int truthLabel;           // Flavor truth label
      bool matchTruth;          // Matches truth jet

      // btagging
      float sv0;                // SV0 btag weight
      float combNN;             // JetFitterCombNN btag weight
      float mv1;                // MV1 btag weight

      // Flags/variables for cleaning
      bool isBadVeryLoose;      // bad jet flag computed with SUSYTools
      bool isHotTile;           // tile hot spot flag computed with SUSYTools
      float bch_corr_jet;       // Needed for dead region veto

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
        matchTruth = false;
        detEta = 0;
        sv0 = combNN = mv1 = 0;
        isBadVeryLoose = isHotTile = false;
	bch_corr_jet = 0;
	jer = jes_up = jes_dn = 0;
        Particle::clear();
      }

      ClassDef(Jet, 6);
  };

  // Met class
  // No longer inherits from Particle or TLorentzVector
  class Met : public TObject
  {
    public:
      Met(){ clear(); }
      virtual ~Met(){};
      /** Copy constructor */
      Met(const Met &);
      /** Assignment operator */
      Met& operator=(const Met &);
      
      float Et;
      float phi;

      // Build a TLorentzVector on the fly
      const TLorentzVector lv() const {
	TLorentzVector tlv;
	tlv.SetPtEtaPhiE(this->Et,0,this->phi,this->Et);
	return tlv;
      }

      // MET Composition info - do we want TLorentzVectors, TVector2, or just floats?
      float refEle;             // Ref electron term magnitude
      float refEle_etx;         // Ref electron term
      float refEle_ety;         // Ref electron term
      float refMuo;             // Ref muon term magnitude
      float refMuo_etx;         // Ref muon term
      float refMuo_ety;         // Ref muon term
      float refJet;             // Ref jet term
      float refJet_etx;         // Ref jet term
      float refJet_ety;         // Ref jet term
      float softJet;            // Soft jet term
      float softJet_etx;        // Soft jet term
      float softJet_ety;        // Soft jet term
      float refGamma;           // Ref gamma term
      float refGamma_etx;       // Ref gamma term
      float refGamma_ety;       // Ref gamma term
      float refCell;            // Cellout term
      float refCell_etx;        // Cellout term
      float refCell_ety;        // Cellout term
      float softTerm;           // Soft term
      float softTerm_etx;       // Soft term
      float softTerm_ety;       // Soft term

      // I'm not a big fan of the way this is setup, would like to improve it someday
      int sys;                  // Met stored in vector for each sys shift, this identifies met for specific shift

      // print vars
      void print() const;

      // clear vars
      void clear(){
        Et = phi = 0;
        refEle = refMuo = refJet = softJet = refGamma = refCell = softTerm = 0;
        refEle_etx = refMuo_etx = refJet_etx = softJet_etx = refGamma_etx = refCell_etx = softTerm_etx = 0;
        refEle_ety = refMuo_ety = refJet_ety = softJet_ety = refGamma_ety = refCell_ety = softTerm_ety = 0;
        sys = 0;
      }

      ClassDef(Met, 3);
  };

  // TruthParticle
  class TruthParticle : public Particle
  {
    public:
      TruthParticle(){ clear(); }
      virtual ~TruthParticle(){};
      TruthParticle(const TruthParticle &);
      /** Assignment operator */
      TruthParticle& operator=(const TruthParticle &);

      int pdgId;
      int charge;
      int status;
      int motherPdgId;

      // print method
      void print() const;

      void clear(){
        pdgId  = 0;
        charge = 0;
        status = 0;
        motherPdgId = 0;
        Particle::clear();
      }

      ClassDef(TruthParticle,1);
  };

  // TruthJet
  class TruthJet : public Particle
  {
    public:
      TruthJet(){ clear(); }
      virtual ~TruthJet(){};
      TruthJet(const TruthJet &);
      /** Assignment operator */
      TruthJet& operator=(const TruthJet &);

      int flavor;

      // print method
      void print() const;

      void clear(){
        flavor = 0;
        Particle::clear();
      }

      ClassDef(TruthJet,1);
  };

  // TruthMet class
  class TruthMet : public TObject
  {
    public:
      TruthMet(){ clear(); }
      virtual ~TruthMet(){};
      /** Copy constructor */
      TruthMet(const TruthMet &);
      /** Assignment operator */
      TruthMet& operator=(const TruthMet &);
      
      float Et;
      float phi;

      // Build a TLorentzVector on the fly
      const TLorentzVector lv() const {
	TLorentzVector tlv;
	tlv.SetPtEtaPhiE(this->Et,0,this->phi,this->Et);
	return tlv;
      }

      // print vars
      void print() const;

      // clear vars
      void clear(){
        Et  = 0;
        phi = 0;
      }

      ClassDef(TruthMet, 1);
  };

};


#endif
