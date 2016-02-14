// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRON_H
#define SUSYNTUPLE_ELECTRON_H

// std
#include <vector>

// SusyNtuple
#include "SusyNtuple/Lepton.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/ElectronId.h"

namespace Susy
{
/// Electron class
class Electron : public Lepton
{
public:
    Electron(){ clear(); }
    virtual ~Electron(){};
    Electron(const Electron &);
    /** Assignment operator */
    Electron& operator=(const Electron &);

    // Cluster/track variables
    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    float trackPt;            ///< ID track pt
    float trackEta;           ///< ID track eta
    bool  OQ;                 ///< GoodOQ

    //LH quality flags
    bool veryLooseLH;        ///< veryLooseLH
    bool looseLH;            ///< looseLH
    bool mediumLH;           ///< mediumLH
    bool tightLH;            ///< tightLH

    //Without d0 cut
    bool looseLH_nod0;       ///< looseLH
    bool mediumLH_nod0;      ///< mediumLH
    bool tightLH_nod0;       ///< tightLH

    // efficiency SF per electron LH WP
    std::vector<float> eleEffSF;

    bool isMuon;              ///< True if electron track is same as a muon track
    bool isChargeFlip;        ///< Charge flip flag from RecoTruthMatch
    int truthCharge;          ///< as provided by xAOD::TruthHelpers::getTruthParticle
    int ss3lChargeFlip;       ///< as provided by ss3l_chargeflip::fillElectronChargeFlip
    int mcBkgMotherPdgId;     ///< TODO: document me
    int mcBkgTruthOrigin;     ///< TODO: document me

    //AT: This is insane >50!
    float res_all_dn;
    float res_all_up;
/*
    float res_matCalo_dn;
    float res_matCalo_up;
    float res_matCryo_dn;
    float res_matCryo_up;
    float res_matGap_dn;
    float res_matGap_up;
    float res_matId_dn;
    float res_matId_up;
    float res_nom;
    float res_none;
    float res_pileup_dn;
    float res_pileup_up;
    float res_sampTerm_dn;
    float res_sampTerm_up;
    float res_z_dn;
    float res_z_up;
*/
    float scale_all_dn;
    float scale_all_up;
/*
    float scale_G4_dn;
    float scale_G4_up;
    float scale_L1_dn;
    float scale_L1_up;
    float scale_L2_dn;
    float scale_L2_up;
    float scale_LArCalib_dn;
    float scale_LArCalib_up;
    float scale_LArECalib_dn;
    float scale_LArECalib_up;
    float scale_LArEunconv_dn;
    float scale_LArEunconv_up;
    float scale_LArUnconv_dn;
    float scale_LArUnconv_up;
    float scale_last;
    float scale_matCalo_dn;
    float scale_matCalo_up;
    float scale_matCryo_dn;
    float scale_matCryo_up;
    float scale_matId_dn;
    float scale_matId_up;
    float scale_nom;
    float scale_none;
    float scale_ped_dn;
    float scale_ped_up;
    float scale_ps_dn;
    float scale_ps_up;
    float scale_s12_dn;
    float scale_s12_up;
    float scale_ZeeStat_dn;
    float scale_ZeeStat_up;
    float scale_ZeeSys_dn;
    float scale_ZeeSys_up;
*/
  //  float eff_corr_dn;
  //  float eff_corr_up;
  //  float eff_uncorr_dn;
  //  float eff_uncorr_up;
    std::vector<float> errEffSF_id_corr_up;
    std::vector<float> errEffSF_id_corr_dn;
    std::vector<float> errEffSF_reco_corr_up;
    std::vector<float> errEffSF_reco_corr_dn;
/*
    float scale_mom_dn;
    float scale_mom_up;
*/
    // Polymorphism, baby!!
    bool isEle() const { return true;  }
    bool isMu()  const { return false; }

    /// Shift energy up/down for systematic
    void setState(int sys);

    /// Print method
    void print() const;

    /// Clear vars
    void clear(){
      clusE = clusEta = clusPhi = trackPt = trackEta = 0;
      veryLooseLH = looseLH= mediumLH = tightLH = false;
      looseLH_nod0= mediumLH_nod0 = tightLH_nod0 = false;
      isMuon = false;
      isChargeFlip = false;
      truthCharge = 0;
      ss3lChargeFlip = 0;
      OQ = false;
      mcBkgMotherPdgId = -1;
      mcBkgTruthOrigin = -1;
      
      res_all_dn= res_all_up= 0;
      /*
      res_matCalo_dn= res_matCalo_up=0;
      res_matCryo_dn= res_matCryo_up= res_matGap_dn= res_matGap_up=0;
      res_matId_dn= res_matId_up= res_nom= res_none=0;
      res_pileup_dn= res_pileup_up=0;
      res_sampTerm_dn=res_sampTerm_up= res_z_dn=res_z_up=0;
      */
      scale_all_dn=scale_all_up=0;
/*
      scale_G4_dn=scale_G4_up=0;
      scale_L1_dn=scale_L1_up=scale_L2_dn=scale_L2_up=0;
      scale_LArCalib_dn= scale_LArCalib_up=
      scale_LArECalib_dn=scale_LArECalib_up=0;
      scale_LArEunconv_dn=scale_LArEunconv_up=
      scale_LArUnconv_dn=scale_LArUnconv_up=0;
      scale_last=0;
      scale_matCalo_dn=scale_matCalo_up=
      scale_matCryo_dn=scale_matCryo_up=scale_matId_dn=0;
      scale_matId_up=scale_nom=scale_none=0;
      scale_ped_dn=scale_ped_up=scale_ps_dn=
      scale_ps_up=scale_s12_dn=scale_s12_up=0;
      scale_ZeeStat_dn=scale_ZeeStat_up=
      scale_ZeeSys_dn=scale_ZeeSys_up=scale_mom_dn=scale_mom_up=0;
*/
     // eff_corr_dn = eff_corr_up =0;
     // eff_uncorr_dn = eff_uncorr_up =0;
     eleEffSF.assign(ElectronId::ElectronIdInvalid, 1);
     errEffSF_id_corr_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_id_corr_dn.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_reco_corr_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_reco_corr_dn.assign(ElectronId::ElectronIdInvalid, 0);

      Lepton::clear();
    }

    ClassDef(Electron, 18);
};
} //Susy
#endif
