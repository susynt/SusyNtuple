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

    // Isolation flags
    bool isoGradientLoose;    ///< GradientLoose WP from IsolationSelectionTool
    bool isoGradient;         ///< Gradient WP from IsolationSelectionTool
    bool isoLooseTrackOnly;   ///< LooseTrackOnly WP from IsolationSelectionTool
    bool isoLoose;            ///< Loose WP from IsolationSelectionTool
    bool isoTight;            ///< Tight WP from IsolationSelectionTool

    bool isBadMuon;           ///< Bad muon flag from SUSYTools
    bool isCosmic;            ///< Cosmic muon flag from SUSYTools

    // Systematic sf
    float ms_up;              ///< MS Pt + sigma
    float ms_dn;              ///< MS Pt - sigma
    float id_up;              ///< ID Pt + sigma
    float id_dn;              ///< ID Pt - sigma
    float scale_up;           ///< SCALE Pt + sigma
    float scale_dn;           ///< SCALE Pt - sigma

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

      Lepton::clear();
    }

    ClassDef(Muon, 11);
};
} // Susy
#endif
