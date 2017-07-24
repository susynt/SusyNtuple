// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_MUON_H
#define SUSYNTUPLE_MUON_H

#include "SusyNtuple/Lepton.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/MuonId.h"

#include <vector>
#include <map>

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
    std::map<int, float> muoTrigEffData_medium;
    std::map<int, float> muoTrigEffMC_medium;
    std::map<int, float> muoTrigEffData_loose;
    std::map<int, float> muoTrigEffMC_loose;

    // error on trigger efficiency SF -- only storing this for medium muons
    std::map<int, float> muoTrigEffErrData_stat_up_medium;
    std::map<int, float> muoTrigEffErrData_stat_dn_medium;
    std::map<int, float> muoTrigEffErrMC_stat_up_medium;
    std::map<int, float> muoTrigEffErrMC_stat_dn_medium;
    std::map<int, float> muoTrigEffErrData_syst_up_medium;
    std::map<int, float> muoTrigEffErrData_syst_dn_medium;
    std::map<int, float> muoTrigEffErrMC_syst_up_medium;
    std::map<int, float> muoTrigEffErrMC_syst_dn_medium;
    

    // indices of other muons passing dimuon triggers with this muon
    std::map<std::string, std::vector<unsigned int>> diMuTrigMap;

    bool isBadMuon;           ///< Bad muon flag from SUSYTools
    bool isCosmic;            ///< Cosmic muon flag from SUSYTools

    // Systematic sf
    float ms_up;              ///< MS Pt + sigma
    float ms_dn;              ///< MS Pt - sigma
    float id_up;              ///< ID Pt + sigma
    float id_dn;              ///< ID Pt - sigma
    float scale_up;           ///< SCALE Pt + sigma
    float scale_dn;           ///< SCALE Pt - sigma
    float sagitta_bias_dn;    ///< MUON_SAGITTA_RESBIAS_DN
    float sagitta_bias_up;    ///< MUON_SAGITTA_RESBIAS_UP
    float sagitta_rho_dn;     ///< MUON_SAGITTA_RHO_DN
    float sagitta_rho_up;     ///< MUON_SAGITTA_RHO_UP

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
    std::vector<float> errTTVA_stat_up;         // MUON_TTVA_STAT_UP
    std::vector<float> errTTVA_stat_dn;         // MUON_TTVA_STAT_DN
    std::vector<float> errTTVA_syst_up;         // MUON_TTVA_SYS_UP
    std::vector<float> errTTVA_syst_dn;         // MUON_TTVA_SYS_DN
    std::vector<float> errBadMu_stat_dn;        // MUON_BADMUON_STAT_DN
    std::vector<float> errBadMu_stat_up;        // MUON_BADMUON_STAT_UP
    std::vector<float> errBadMu_syst_dn;        // MUON_BADMUON_SYS_DN
    std::vector<float> errBadMu_syst_up;        // MUON_BADMUON_SYS_UP

    // Polymorphism, baby!!
    bool isEle() const { return false; }
    bool isMu()  const { return true; }
    void setState(int sys);

    /// Print method
    void print() const;

    /// Clear vars
    void clear(){
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
      errTTVA_stat_up.assign(MuonId::MuonIdInvalid, 0);
      errTTVA_stat_dn.assign(MuonId::MuonIdInvalid, 0);
      errTTVA_syst_up.assign(MuonId::MuonIdInvalid, 0);
      errTTVA_syst_dn.assign(MuonId::MuonIdInvalid, 0);
      errBadMu_stat_dn.assign(MuonId::MuonIdInvalid, 0);
      errBadMu_stat_up.assign(MuonId::MuonIdInvalid, 0);
      errBadMu_syst_dn.assign(MuonId::MuonIdInvalid, 0);
      errBadMu_syst_up.assign(MuonId::MuonIdInvalid, 0);
      diMuTrigMap.clear();
      isBadMuon = isCosmic = false;
      ms_up = ms_dn = id_up = id_dn = scale_up = scale_dn  = 0;
      sagitta_bias_dn = sagitta_bias_up = 0;
      sagitta_rho_dn = sagitta_rho_up = 0;

      Lepton::clear();
    }

    ClassDef(Muon, 22);
};
} // Susy
#endif
