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

    /// Author information
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EGammaIdentificationRun2
    int author;
    bool authorElectron;            ///< electron reconstructed exclusively as an electron (author == 1)
    bool authorAmbiguous;           ///< electron reconstructed both as electron and photon (author == 16)

    // Cluster/track variables
    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    float clusEtaBE;          ///< CaloCluster eta (2nd sampling) EMB/EMEC combined barycenter eta
    float clusPhiBE;          ///< CaloCluster phi (2nd sampling) EMB/EMEC combined barycenter phi
    float trackPt;            ///< ID track pt
    float trackEta;           ///< ID track eta

    //LH quality flags
    bool veryLooseLLH;        ///< veryLooseLLH
    bool looseLLH;            ///< looseLLH
    bool looseLLHBLayer;      ///< LooseAndBLayerLLH
    bool mediumLLH;           ///< mediumLLH
    bool tightLLH;            ///< tightLLH

    // object quality
    bool passOQBadClusElectron; ///< True if not xAOD::EgammaParameters::BADCLUSELECTRON

    // efficiency SF per electron LH WP
    std::vector<float> eleEffSF;
    // trigger efficiency SF per electron LH WP
    std::vector<float> eleTrigSF_single;
    std::vector<float> eleTrigSF_double;
    std::vector<float> eleTrigSF_mixed;
    // charge flip efficiency SF per electron LH WP
    std::vector<float> eleCHFSF;

    std::vector<int> sharedMuTrk; ///< Indices of SusyNt preMuons with which this electron's track is shared
    std::vector<int> sharedEleEleTrk; ///< Indices of SusyNt preElectrons which this electron's track is shared
    bool isChargeFlip;        ///< Charge flip flag from RecoTruthMatch
    int truthCharge;          ///< as provided by xAOD::TruthHelpers::getTruthParticle
    int ss3lChargeFlip;       ///< as provided by ss3l_chargeflip::fillElectronChargeFlip
    bool passChargeFlipTagger; ///< boolean result from ElectronPhotonSelector/AsgElectronChargeFlipTaggerTool
    double chargeFlipBDT;       ///< BDT value from ElectronPhotonSelector/AsgElectronChargeFlipTaggerTool 

    //AT: This is insane >50!
    float res_all_dn;
    float res_all_up;
    float scale_all_dn;
    float scale_all_up;

    std::vector<float> errEffSF_id_up;     // EL_EFF_ID_TOTAL_Uncorr_UP 
    std::vector<float> errEffSF_id_dn;     // EL_EFF_ID_TOTAL_Uncorr_DN 
    std::vector<float> errEffSF_reco_up;   // EL_EFF_Reco_TOTAL_Uncorr_UP
    std::vector<float> errEffSF_reco_dn;   // EL_EFF_Reco_TOTAL_Uncorr_DN
    std::vector<float> errEffSF_iso_up;    // EL_EFF_Iso_TOTAL_Uncorr_UP
    std::vector<float> errEffSF_iso_dn;    // EL_EFF_Iso_TOTAL_Uncorr_DN
    std::vector<float> errEffSF_trig_up;   // EL_EFF_Trigger_TOTAL_Uncorr_UP
    std::vector<float> errEffSF_trig_dn;   // EL_EFF_Trigger_TOTAL_Uncorr_DN
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
      author = 0;
      authorElectron = authorAmbiguous = false;
      clusE = clusEta = clusPhi = clusEtaBE = clusPhiBE = trackPt = trackEta = 0;
      veryLooseLLH = looseLLH = looseLLHBLayer = mediumLLH = tightLLH = false;
      passOQBadClusElectron = false;
      sharedMuTrk.assign(50,0);
      sharedEleEleTrk.assign(50,0);
      isChargeFlip = false;
      truthCharge = 0;
      ss3lChargeFlip = 0;
      passChargeFlipTagger = false;
      chargeFlipBDT = -99;
      
      res_all_dn= res_all_up= 0;
      scale_all_dn=scale_all_up=0;

     eleEffSF.assign(ElectronId::ElectronIdInvalid, 1);
     eleTrigSF_single.assign(ElectronId::ElectronIdInvalid, 1);
     eleTrigSF_double.assign(ElectronId::ElectronIdInvalid, 1);
     eleTrigSF_mixed.assign(ElectronId::ElectronIdInvalid, 1);
     eleCHFSF.assign(ElectronId::ElectronIdInvalid, 1);
     errEffSF_id_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_id_dn.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_reco_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_reco_dn.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_iso_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_iso_dn.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_trig_up.assign(ElectronId::ElectronIdInvalid, 0);
     errEffSF_trig_dn.assign(ElectronId::ElectronIdInvalid, 0);

      Lepton::clear();
    }

    ClassDef(Electron, 25);
};
} //Susy
#endif
