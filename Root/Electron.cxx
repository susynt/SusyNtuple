#include "SusyNtuple/Electron.h"

#include <iostream>
#include <iomanip>

using namespace std;
using Susy::Electron;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Electron::Electron(const Electron &rhs):
  Lepton(rhs),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
  clusPhi(rhs.clusPhi),
  trackPt(rhs.trackPt),
  mediumPP(rhs.mediumPP),
  tightPP(rhs.tightPP),
  looseLLH(rhs.looseLLH),
  mediumLLH(rhs.mediumLLH),
  veryTightLLH(rhs.veryTightLLH),
  etcone30Corr(rhs.etcone30Corr),
  topoEtcone30Corr(rhs.topoEtcone30Corr),
  isChargeFlip(rhs.isChargeFlip),
  effSF_LLH(rhs.effSF_LLH),
  errEffSF_LLH(rhs.errEffSF_LLH),
  /*
  ees_z_up(rhs.ees_z_up),
  ees_z_dn(rhs.ees_z_dn),  
  ees_mat_up(rhs.ees_mat_up),
  ees_mat_dn(rhs.ees_mat_dn),
  ees_ps_up(rhs.ees_ps_up),
  ees_ps_dn(rhs. ees_ps_dn),
  ees_low_up(rhs.ees_low_up),
  ees_low_dn(rhs.ees_low_dn),
  eer_up(rhs.eer_up),
  eer_dn(rhs.eer_dn)*/
  res_all_dn(rhs.res_all_dn),
  res_all_up(rhs.res_all_up),
  res_matCalo_dn(rhs.res_matCalo_dn),
  res_matCalo_up(rhs.res_matCalo_up),
  res_matCryo_dn(rhs.res_matCryo_dn),
  res_matCryo_up(rhs.res_matCryo_up),
  res_matGap_dn(rhs.res_matGap_dn),
  res_matGap_up(rhs.res_matGap_up),
  res_matId_dn(rhs.res_matId_dn),
  res_matId_up(rhs.res_matId_up),
  res_nom(rhs.res_nom),
  res_none(rhs.res_none),
  res_pileup_dn(rhs.res_pileup_dn),
  res_pileup_up(rhs.res_pileup_up),
  res_sampTerm_dn(rhs.res_sampTerm_dn),
  res_sampTerm_up(rhs.res_sampTerm_up),
  res_z_dn(rhs.res_z_dn),
  res_z_up(rhs.res_z_up),
  scale_all_dn(rhs.scale_all_dn),
  scale_all_up(rhs.scale_all_up),
  scale_G4_dn(rhs.scale_G4_dn),
  scale_G4_up(rhs.scale_G4_up),
  scale_L1_dn(rhs.scale_L1_dn),
  scale_L1_up(rhs.scale_L1_up),
  scale_L2_dn(rhs.scale_L2_dn),
  scale_L2_up(rhs.scale_L2_up),
  scale_LArCalib_dn(rhs.scale_LArCalib_dn),
  scale_LArCalib_up(rhs.scale_LArCalib_up),
  scale_LArECalib_dn(rhs.scale_LArECalib_dn),
  scale_LArECalib_up(rhs.scale_LArECalib_up),
  scale_LArEunconv_dn(rhs.scale_LArEunconv_dn),
  scale_LArEunconv_up(rhs.scale_LArEunconv_up),
  scale_LArUnconv_dn(rhs.scale_LArUnconv_dn),
  scale_LArUnconv_up(rhs.scale_LArUnconv_up),
  scale_last(rhs.scale_last),
  scale_matCalo_dn(rhs.scale_matCalo_dn),
  scale_matCalo_up(rhs.scale_matCalo_up),
  scale_matCryo_dn(rhs.scale_matCryo_dn),
  scale_matCryo_up(rhs.scale_matCryo_up),
  scale_matId_dn(rhs.scale_matId_dn),
  scale_matId_up(rhs.scale_matId_up),
  scale_nom(rhs.scale_nom),
  scale_none(rhs.scale_none),
  scale_ped_dn(rhs.scale_ped_dn),
  scale_ped_up(rhs.scale_ped_up),
  scale_ps_dn(rhs.scale_ps_dn),
  scale_ps_up(rhs.scale_ps_up),
  scale_s12_dn(rhs.scale_s12_dn),
  scale_s12_up(rhs.scale_s12_up),
  scale_ZeeStat_dn(rhs.scale_ZeeStat_dn),
  scale_ZeeStat_up(rhs.scale_ZeeStat_up),
  scale_ZeeSys_dn(rhs.scale_ZeeSys_dn),
  scale_ZeeSys_up(rhs.scale_ZeeSys_up),
  scale_mom_dn(rhs.scale_mom_dn),
  scale_mom_up(rhs.scale_mom_up)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Electron& Electron::operator=(const Electron &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
    clusPhi = rhs.clusPhi;
    trackPt = rhs.trackPt;
    mediumPP = rhs.mediumPP;
    tightPP = rhs.tightPP;
    looseLLH = rhs.looseLLH;
    mediumLLH = rhs.mediumLLH;
    veryTightLLH = rhs.veryTightLLH;
    etcone30Corr = rhs.etcone30Corr;
    topoEtcone30Corr = rhs.topoEtcone30Corr;
    isChargeFlip = rhs.isChargeFlip;
    effSF_LLH = rhs.effSF_LLH;
    errEffSF_LLH = rhs.errEffSF_LLH;
    /*
    ees_z_up = rhs.ees_z_up;
    ees_z_dn = rhs.ees_z_dn;  
    ees_mat_up = rhs.ees_mat_up;
    ees_mat_dn = rhs.ees_mat_dn;
    ees_ps_up = rhs.ees_ps_up;
    ees_ps_dn = rhs. ees_ps_dn;
    ees_low_up = rhs.ees_low_up;
    ees_low_dn = rhs.ees_low_dn;
    eer_up = rhs.eer_up;
    eer_dn = rhs.eer_dn;*/
    res_all_dn = rhs.res_all_dn;
    res_all_up = res_all_up;
    res_matCalo_dn = rhs.res_matCalo_dn;
    res_matCalo_up = rhs.res_matCalo_up;
    res_matCryo_dn = rhs.res_matCryo_dn;
    res_matCryo_up = rhs.res_matCryo_up;
    res_matGap_dn = rhs.res_matGap_dn;
    res_matGap_up = rhs.res_matGap_up;
    res_matId_dn = rhs.res_matId_dn;
    res_matId_up = rhs.res_matId_up;
    res_nom = rhs.res_nom;
    res_none = rhs.res_none;
    res_pileup_dn = rhs.res_pileup_dn;
    res_pileup_up = rhs.res_pileup_up;
    res_sampTerm_dn = rhs.res_sampTerm_dn;
    res_sampTerm_up = rhs.res_sampTerm_up;
    res_z_dn = rhs.res_z_dn;
    res_z_up = rhs.res_z_up;
    scale_all_dn = rhs.scale_all_dn;
    scale_all_up = rhs.scale_all_up;
    scale_G4_dn = rhs.scale_G4_dn;
    scale_G4_up = rhs.scale_G4_up;
    scale_L1_dn = rhs.scale_L1_dn;
    scale_L1_up = rhs.scale_L1_up;
    scale_L2_dn = rhs.scale_L2_dn;
    scale_L2_up = rhs.scale_L2_up;
    scale_LArCalib_dn = rhs.scale_LArCalib_dn;
    scale_LArCalib_up = rhs.scale_LArCalib_up;
    scale_LArECalib_dn = rhs.scale_LArECalib_dn;
    scale_LArECalib_up = rhs.scale_LArECalib_up;
    scale_LArEunconv_dn = rhs.scale_LArEunconv_dn;
    scale_LArEunconv_up = rhs.scale_LArEunconv_up;
    scale_LArUnconv_dn = rhs.scale_LArUnconv_dn;
    scale_LArUnconv_up = rhs.scale_LArUnconv_up;
    scale_last = rhs.scale_last;
    scale_matCalo_dn = rhs.scale_matCalo_dn;
    scale_matCalo_up = rhs.scale_matCalo_up;
    scale_matCryo_dn = rhs.scale_matCryo_dn;
    scale_matCryo_up = rhs.scale_matCryo_up;
    scale_matId_dn = rhs.scale_matId_dn;
    scale_matId_up = rhs.scale_matId_up;
    scale_nom = rhs.scale_nom;
    scale_none = rhs.scale_none;
    scale_ped_dn = rhs.scale_ped_dn;
    scale_ped_up = rhs.scale_ped_up;
    scale_ps_dn = rhs.scale_ps_dn;
    scale_ps_up = rhs.scale_ps_up;
    scale_s12_dn = rhs.scale_s12_dn;
    scale_s12_up = rhs.scale_s12_up;
    scale_ZeeStat_dn = rhs.scale_ZeeStat_dn;
    scale_ZeeStat_up = rhs.scale_ZeeStat_up;
    scale_ZeeSys_dn = rhs.scale_ZeeSys_dn;
    scale_ZeeSys_up = rhs.scale_ZeeSys_up;
    scale_mom_dn = rhs.scale_mom_dn;
    scale_mom_up = rhs.scale_mom_up;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Electron Set State
/*--------------------------------------------------------------------------------*/
void Electron::setState(int sys)
{
  resetTLV();
  if(sys == NtSys::NOM) return;
  
  float sf = 0;
  //if     ( sys == NtSys_EES_UP ) sf = ees_up;
  //else if( sys == NtSys_EES_DN ) sf = ees_dn;
  /*
  if     ( sys == NtSys::EES_Z_UP   ) sf = ees_z_up;
  else if( sys == NtSys::EES_Z_DN   ) sf = ees_z_dn;
  else if( sys == NtSys::EES_MAT_UP ) sf = ees_mat_up;
  else if( sys == NtSys::EES_MAT_DN ) sf = ees_mat_dn;
  else if( sys == NtSys::EES_PS_UP  ) sf = ees_ps_up;
  else if( sys == NtSys::EES_PS_DN  ) sf = ees_ps_dn;
  else if( sys == NtSys::EES_LOW_UP ) sf = ees_low_up;
  else if( sys == NtSys::EES_LOW_DN ) sf = ees_low_dn;
  else if( sys == NtSys::EER_UP     ) sf = eer_up;
  else if( sys == NtSys::EER_DN     ) sf = eer_dn;
  */

  if     ( sys == NtSys::EG_RESOLUTION_ALL_DN ) sf = res_all_dn;
  else if( sys == NtSys::EG_RESOLUTION_ALL_UP ) sf = res_all_up;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALCALO_DN ) sf = res_matCalo_dn;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALCALO_UP ) sf = res_matCalo_up;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALCRYO_DN ) sf = res_matCryo_dn;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALCRYO_UP ) sf = res_matCryo_up;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALGAP_DN ) sf = res_matGap_dn;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALGAP_UP) sf = res_matGap_up;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALID_DN ) sf = res_matId_dn;
  else if( sys == NtSys::EG_RESOLUTION_MATERIALID_UP ) sf = res_matId_up;
  else if( sys == NtSys::EG_RESOLUTION_NOMINAL ) sf = res_nom;
  else if( sys == NtSys::EG_RESOLUTION_NONE ) sf = res_none;
  else if( sys == NtSys::EG_RESOLUTION_PILEUP_DN ) sf = res_pileup_dn;
  else if( sys == NtSys::EG_RESOLUTION_PILEUP_UP ) sf = res_pileup_up;
  else if( sys == NtSys::EG_RESOLUTION_SAMPLINGTERM_DN ) sf = res_sampTerm_dn;
  else if( sys == NtSys::EG_RESOLUTION_SAMPLINGTERM_UP ) sf = res_sampTerm_up;
  else if( sys == NtSys::EG_RESOLUTION_ZSMEARING_DN ) sf = res_z_dn;
  else if( sys == NtSys::EG_RESOLUTION_ZSMEARING_UP ) sf = res_z_up;
  else if( sys == NtSys::EG_SCALE_ALL_DN ) sf = scale_all_dn;
  else if( sys == NtSys::EG_SCALE_ALL_UP ) sf = scale_all_up;
  else if( sys == NtSys::EG_SCALE_G4_DN ) sf = scale_G4_dn;
  else if( sys == NtSys::EG_SCALE_G4_UP ) sf = scale_G4_up;
  else if( sys == NtSys::EG_SCALE_L1GAIN_DN ) sf = scale_L1_dn;
  else if( sys == NtSys::EG_SCALE_L1GAIN_UP ) sf = scale_L1_up;
  else if( sys == NtSys::EG_SCALE_L2GAIN_DN ) sf = scale_L2_dn;
  else if( sys == NtSys::EG_SCALE_L2GAIN_UP ) sf = scale_L2_up;
  else if( sys == NtSys::EG_SCALE_LARCALIB_DN ) sf = scale_LArCalib_dn;
  else if( sys == NtSys::EG_SCALE_LARCALIB_UP ) sf = scale_LArCalib_up;
  else if( sys == NtSys::EG_SCALE_LARELECCALIB_DN ) sf = scale_LArECalib_dn;
  else if( sys == NtSys::EG_SCALE_LARELECCALIB_UP ) sf = scale_LArECalib_up;
  else if( sys == NtSys::EG_SCALE_LARELECUNCONV_DN ) sf = scale_LArEunconv_dn;
  else if( sys == NtSys::EG_SCALE_LARELECUNCONV_UP ) sf = scale_LArEunconv_up;
  else if( sys == NtSys::EG_SCALE_LARUNCONVCALIB_DN ) sf = scale_LArUnconv_dn;
  else if( sys == NtSys::EG_SCALE_LARUNCONVCALIB_UP ) sf = scale_LArUnconv_up;
  else if( sys == NtSys::EG_SCALE_LASTSCALEVARIATION ) sf = scale_last;
  else if( sys == NtSys::EG_SCALE_MATCALO_DN ) sf = scale_matCalo_dn;
  else if( sys == NtSys::EG_SCALE_MATCALO_UP ) sf = scale_matCalo_up;
  else if( sys == NtSys::EG_SCALE_MATCRYO_DN ) sf = scale_matCryo_dn;
  else if( sys == NtSys::EG_SCALE_MATCRYO_UP ) sf = scale_matCryo_up;
  else if( sys == NtSys::EG_SCALE_MATID_DN ) sf = scale_matId_dn;
  else if( sys == NtSys::EG_SCALE_MATID_UP ) sf = scale_matId_up;
  else if( sys == NtSys::EG_SCALE_NOMINAL ) sf = scale_nom;
  else if( sys == NtSys::EG_SCALE_NONE ) sf = scale_none;
  else if( sys == NtSys::EG_SCALE_PEDESTAL_DN ) sf = scale_ped_dn;
  else if( sys == NtSys::EG_SCALE_PEDESTAL_UP ) sf = scale_ped_up;
  else if( sys == NtSys::EG_SCALE_PS_DN ) sf = scale_ps_dn;
  else if( sys == NtSys::EG_SCALE_PS_UP ) sf = scale_ps_up;
  else if( sys == NtSys::EG_SCALE_S12_DN ) sf = scale_s12_dn;
  else if( sys == NtSys::EG_SCALE_S12_UP ) sf = scale_s12_up;
  else if( sys == NtSys::EG_SCALE_ZEESTAT_DN ) sf = scale_ZeeStat_dn;
  else if( sys == NtSys::EG_SCALE_ZEESTAT_UP ) sf = scale_ZeeStat_up;
  else if( sys == NtSys::EG_SCALE_ZEESYST_DN ) sf = scale_ZeeSys_dn;
  else if( sys == NtSys::EG_SCALE_ZEESYST_UP ) sf = scale_ZeeSys_up;
  else if( sys == NtSys::EL_SCALE_MOMENTUM_DN ) sf = scale_mom_dn;
  else if( sys == NtSys::EL_SCALE_MOMENTUM_UP ) sf = scale_mom_up;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Electron print
/*--------------------------------------------------------------------------------*/
void Electron::print() const
{
  cout.precision(2);
  cout << fixed << "El : q " << setw(2) << q << " pt " << setw(6) << Pt() << " eta " << setw(5) << Eta() 
       << " phi " << setw(5) << Phi()
       << " tight " << tightPP 
       << " type " << mcType << " origin " << mcOrigin 
       //<< " index " << idx 
       // No way do we want to see this every time we print
       //<< " ees_up " << ees_up << " ees_dn " << ees_dn << " eer_up " << eer_up << " eer_dn " << eer_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}