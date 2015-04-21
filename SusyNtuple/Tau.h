// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TAU_H
#define SUSYNTUPLE_TAU_H


#include "SusyNtuple/Particle.h"

namespace Susy
{
/// Tau class
class Tau : public Particle
{
public:
    Tau() { clear(); }
    virtual ~Tau(){};
    Tau(const Tau &);
    /** Assignment operator */
    Tau& operator=(const Tau &);

    int q;                    ///< Charge
    int author;               ///< reconstruction author
    int nTrack;               ///< number of tracks
    float eleBDT;             ///< electron BDT score
    float jetBDT;             ///< jet BDT score
    int jetBDTSigLoose;       ///< jet BDT working point
    int jetBDTSigMedium;      ///< jet BDT working point
    int jetBDTSigTight;       ///< jet BDT working point
    int eleBDTLoose;          ///< ele BDT working point
    int eleBDTMedium;         ///< ele BDT working point
    int eleBDTTight;          ///< ele BDT working point
    int muonVeto;             ///< muon veto flag

    bool trueTau;             ///< trueTauAssocSmall_matched d3pd variable

    bool matched2TruthLepton; ///< flag from RecoTruthMatch::Matched2TruthLepton
    int truthType;            ///< RecoTauMatch::TauFakeType
    int detailedTruthType;    ///< RecoTauMatch::TauDetailedFakeType

    // Taus have a lot of SFs
    float looseEffSF;         ///< Loose efficiency scale factor
    float errLooseEffSF;      ///< Loose efficiency scale factor uncertaintiy
    float mediumEffSF;        ///< Medium efficiency scale factor
    float errMediumEffSF;     ///< Medium efficiency scale factor uncertainty
    float tightEffSF;         ///< Tight efficiency scale factor
    float errTightEffSF;      ///< Tight efficiency scale factor uncertainty
    float looseEVetoSF;       ///< Loose efficiency scale factor
    float errLooseEVetoSF;    ///< Loose efficiency scale factor uncertaintiy
    float mediumEVetoSF;      ///< Medium efficiency scale factor
    float errMediumEVetoSF;   ///< Medium efficiency scale factor uncertainty
    float tightEVetoSF;       ///< Tight efficiency scale factor
    float errTightEVetoSF;    ///< Tight efficiency scale factor uncertainty
    //float looseEleSF
    //float effSF;            ///< Efficiency scale factor
    //float errEffSF;         ///< Uncertainty on the efficiency scale factor

    // Systematic factors
    float sme_total_up;             ///< tau energy scale + sigma
    float sme_total_dn;             ///< tau energy scale - sigma

    long long trigFlags;      ///< Bit word representing matched trigger chains

    /// Trigger matching
    /** provide the trigger chain via bit mask, e.g. TRIG_mu18 */
    bool matchTrig(long long mask) const {
      return (trigFlags & mask) == mask;
    }

    /// Set systematic state
    void setState(int sys);

    /// Print method
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
      sme_total_up = sme_total_dn = 0;
      trigFlags = 0;
      Particle::clear();
    }

    ClassDef(Tau, 7);
  };
} // Susy
#endif
