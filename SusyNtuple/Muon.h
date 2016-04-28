// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_MUON_H
#define SUSYNTUPLE_MUON_H

#include "SusyNtuple/Lepton.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/MuonId.h"

#include <vector>

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

    int idx;                  ///< index of muon in muon collection stored in SusyNt

    bool isCaloTagged;        ///< Is calo tagged
    bool isSiForward;         ///< Is Silicon associated forward muon
    bool isCombined;          ///< Is combined muon, otherwise segment tagged
    std::vector<int> ghostTrack; ///< Indices of SusyNt preJets to which this muon's ID track is ghost associated

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

    // efficiency SF per muon quality WP
    std::vector<float> muoEffSF;
    // trigger efficiency SF per muon quality WP
    std::vector<float> muoTrigSF;

    bool isBadMuon;           ///< Bad muon flag from SUSYTools
    bool isCosmic;            ///< Cosmic muon flag from SUSYTools

    // Systematic sf
    float ms_up;              ///< MS Pt + sigma
    float ms_dn;              ///< MS Pt - sigma
    float id_up;              ///< ID Pt + sigma
    float id_dn;              ///< ID Pt - sigma
    float scale_up;           ///< SCALE Pt + sigma
    float scale_dn;           ///< SCALE Pt - sigma

    // SF uncertainties
    std::vector<float> errEffSF_stat_up;        // MUON_EFF_STAT_UP
    std::vector<float> errEffSF_stat_dn;        // MUON_EFF_STAT_DN
    std::vector<float> errEffSF_syst_up;        // MUON_EFF_SYS_UP
    std::vector<float> errEffSF_syst_dn;        // MUON_EFF_SYS_DN
    std::vector<float> errEffSF_stat_lowpt_up;  // MUON_EFF_STAT_LOWPT_UP
    std::vector<float> errEffSF_stat_lowpt_dn;  // MUON_EFF_STAT_LOWPT_DN
    std::vector<float> errEffSF_syst_lowpt_up;  // MUON_EFF_SYS_LOWPT_UP
    std::vector<float> errEffSF_syst_lowpt_dn;  // MUON_EFF_SYS_LOWPT_DN
    std::vector<float> errIso_stat_up;          // MUON_ISO_STAT_UP
    std::vector<float> errIso_stat_dn;          // MUON_ISO_STAT_DN
    std::vector<float> errIso_syst_up;          // MUON_ISO_SYS_UP
    std::vector<float> errIso_syst_dn;          // MUON_ISO_SYS_DN

    // Polymorphism, baby!!
    bool isEle() const { return false; }
    bool isMu()  const { return true; }
    void setState(int sys);

    /// Print method
    void print() const;

    /// Clear vars
    void clear(){
      idx = 0;
      isCombined = 0;
      isCaloTagged = 0;
      isSiForward = 0;
      ghostTrack.assign(100, 0);
      idTrackPt = idTrackEta = idTrackPhi = 0;
      msTrackPt = msTrackEta = msTrackPhi = 0;
      idTrackQ  = msTrackQ = 0;
      idTrackTheta = idTrackQoverP = 0;
      msTrackTheta = msTrackQoverP = 0;
      veryLoose = loose = medium = tight = false;

      muoEffSF.assign(MuonId::MuonIdInvalid, 1);
      muoTrigSF.assign(MuonId::MuonIdInvalid, 1);
      errEffSF_stat_up.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_stat_dn.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_syst_up.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_syst_dn.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_stat_lowpt_up.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_stat_lowpt_dn.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_syst_lowpt_up.assign(MuonId::MuonIdInvalid, 0);
      errEffSF_syst_lowpt_dn.assign(MuonId::MuonIdInvalid, 0);
      errIso_stat_up.assign(MuonId::MuonIdInvalid, 0);
      errIso_stat_dn.assign(MuonId::MuonIdInvalid, 0);
      errIso_syst_up.assign(MuonId::MuonIdInvalid, 0);
      errIso_syst_dn.assign(MuonId::MuonIdInvalid, 0);

      isBadMuon = isCosmic = false;
      ms_up = ms_dn = id_up = id_dn = scale_up = scale_dn  = 0;

      Lepton::clear();
    }

    ClassDef(Muon, 18);
};
} // Susy
#endif
