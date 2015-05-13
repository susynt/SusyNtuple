// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_MUON_H
#define SUSYNTUPLE_MUON_H

#include "SusyNtuple/Lepton.h"
#include "SusyNtuple/SusyNtSys.h"

namespace Susy
{
/// Muon class
class Muon : public Lepton
{
public:
    Muon(){ clear(); }
    virtual ~Muon(){};
    Muon(const Muon &);
    /** Assignment operator */
    Muon& operator=(const Muon &);

    bool isCombined;          ///< Is combined muon, otherwise segment tagged

    float idTrackPt;          ///< ID track pt
    float idTrackEta;         ///< ID track eta
    float idTrackPhi;         ///< ID track phi
    int   idTrackQ;           ///< ID track charge
    float idTrackTheta;       ///< ID track theta //AT: redundant ?
    float idTrackQoverP;      ///< ID track q/p

    float msTrackPt;          ///< MS track pt
    float msTrackEta;         ///< MS track eta
    float msTrackPhi;         ///< MS track phi
    int   msTrackQ;           ///< MS track charge
    float msTrackTheta;       ///< MS track theta //AT: redundant ?
    float msTrackQoverP;      ///< MS track q/p

    // Muon quality flags
    bool veryLoose;           ///< VeryLoose
    bool loose;               ///< Loose
    bool medium;              ///< Medium
    bool tight;               ///< Tight

    bool isBadMuon;           ///< Bad muon flag from SUSYTools
    bool isCosmic;            ///< Cosmic muon flag from SUSYTools

    // Systematic sf
    float ms_up;              ///< MS Pt + sigma
    float ms_dn;              ///< MS Pt - sigma
    float id_up;              ///< ID Pt + sigma
    float id_dn;              ///< ID Pt - sigma
    float scale_up;           ///< SCALE Pt + sigma
    float scale_dn;           ///< SCALE Pt - sigma

    // Scale factor uncertainties
    float errEffSF_stat_up;   ///< Uncertainty on the efficiency scale factor
    float errEffSF_stat_dn;   ///< Uncertainty on the efficiency scale factor
    float errEffSF_syst_up;   ///< Uncertainty on the efficiency scale factor
    float errEffSF_syst_dn;   ///< Uncertainty on the efficiency scale factor

    // Polymorphism, baby!!
    bool isEle() const { return false; }
    bool isMu()  const { return true; }
    void setState(int sys);

    /// Print method
    void print() const;

    /// Clear vars
    void clear(){
      isCombined = 0;
      idTrackPt = idTrackEta = idTrackPhi = 0;
      msTrackPt = msTrackEta = msTrackPhi = 0;
      idTrackQ  = msTrackQ = 0;
      idTrackTheta = idTrackQoverP = 0;
      msTrackTheta = msTrackQoverP = 0;
      isBadMuon = isCosmic = false;
      ms_up = ms_dn = id_up = id_dn = scale_up = scale_dn  = 0;
      errEffSF_stat_up = errEffSF_stat_dn = errEffSF_syst_up = errEffSF_syst_dn = 0;

      Lepton::clear();
    }

    ClassDef(Muon, 9);
};
} // Susy
#endif
