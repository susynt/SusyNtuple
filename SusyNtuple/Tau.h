// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TAU_H
#define SUSYNTUPLE_TAU_H

#include "SusyNtuple/Particle.h"
#include "SusyNtuple/SusyNtSys.h"

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
    int nTrack;               ///< number of tracks

    bool loose;               ///< passes TauAnalysisTool::TauSelectionTool "Loose" WP
    bool medium;              ///< passes TauAnalysisTool::TauSelectionTool "Medium" WP
    bool tight;               ///< passes TauAnalysisTool::TauSelectionTool "Tight" WP

    bool isTruthMatched;      ///< matched to true tau
    int truthPdgId;           ///< pdgId of truth particle matched to tau
    int truthNProngs;         ///< number of prongs of matched truth-tau
    int truthCharge;          ///< charge of matched truth-tau
    bool isHadronicTau;       ///< tau is truth matched to hadronic tau

    /** MCTruthClassifier enum
        http://atlas-computing.web.cern.ch/atlas-computing/links/buildDirectory/AtlasOffline/19.1.0/InstallArea/doc/MCTruthClassifier/html/MCTruthClassifierDefs_8h_source.html
    */
    int truthType;            ///< MCTruthClassifier Particle Type
    int truthOrigin;          ///< MCTruthClassifier Particle Origin

    // Taus have a lot of SFs
    float looseEffSF;         ///< Loose efficiency scale factor
    float errLooseEffSF;      ///< Loose efficiency scale factor uncertaintiy
    float mediumEffSF;        ///< Medium efficiency scale factor
    float errMediumEffSF;     ///< Medium efficiency scale factor uncertainty
    float tightEffSF;         ///< Tight efficiency scale factor
    float errTightEffSF;      ///< Tight efficiency scale factor uncertainty
 
    // Systematic factors
    float sme_total_up;       ///< tau energy scale + sigma
    float sme_total_dn;       ///< tau energy scale - sigma

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
        q = nTrack = 0;
        loose = medium = tight = false;
        isTruthMatched = false;
        truthPdgId = 0;
        truthNProngs = truthCharge = 0;
        isHadronicTau = false;
        truthType = truthOrigin = -1;
        looseEffSF = mediumEffSF = tightEffSF = 1;
        errLooseEffSF = errMediumEffSF = errTightEffSF = 0;
        sme_total_up = sme_total_dn = 0;
        trigFlags = 0;
        Particle::clear();
    }

    ClassDef(Tau, 8);
  };
} // Susy
#endif
