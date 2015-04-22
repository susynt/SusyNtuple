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
    int idTrackQ;             ///< ID track charge

    float msTrackPt;          ///< MS track pt
    float msTrackEta;         ///< MS track eta
    float msTrackPhi;         ///< MS track phi
    int msTrackQ;             ///< ID track charge

    float thetaPV;            ///< Theta extrapolated to PV, by request.
    float etcone30;           ///< etcone iso
    float ptcone30ElStyle;    ///< ptcone with electron style tracks

    // Variables for charge misid
    // \TODO: Redundant variables?!
    float id_theta;
    float id_phi;
    float id_qoverp;
    float ms_theta;
    float ms_phi;
    float ms_qoverp;

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
      thetaPV = etcone30 = ptcone30ElStyle = 0;
      id_theta = id_phi = id_qoverp = 0;
      ms_theta = ms_phi = ms_qoverp = 0;
      isBadMuon = isCosmic = false;
      ms_up = ms_dn = id_up = id_dn = scale_up = scale_dn  = 0;

      Lepton::clear();
    }

    ClassDef(Muon, 7);
};
} // Susy
#endif
