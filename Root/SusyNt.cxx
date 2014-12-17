#include <iostream>
#include <iomanip>
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;



/*--------------------------------------------------------------------------------*/
// Event print
/*--------------------------------------------------------------------------------*/
void Event::print() const
{
  cout << "Run " << run;
  if(isMC) cout << " MCID " << mcChannel;
  cout << " Event " << event;
  if(isMC) cout << " proc " << susyFinalState;
  cout << " Stream " << streamName(stream) 
       << " w " << w << endl;
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Particle::Particle(const Particle &rhs):
  TLorentzVector(rhs),
  idx(rhs.idx),
  pt(rhs.pt),
  eta(rhs.eta),
  phi(rhs.phi),
  m(rhs.m)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Particle& Particle::operator=(const Particle &rhs)
{
  if (this != &rhs) {
    TLorentzVector::operator=(rhs);
    idx  = rhs.idx; 
    pt = rhs.pt; 
    eta = rhs.eta; 
    phi = rhs.phi; 
    m = rhs.m; 
  }
  return *this;
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Lepton::Lepton(const Lepton &rhs):
  Particle(rhs),
  q(rhs.q),
  isBaseline(rhs.isBaseline),
  isSignal(rhs.isSignal),
  etcone20(rhs.etcone20),
  ptcone20(rhs.ptcone20),
  ptcone30(rhs.ptcone30),
  d0(rhs.d0),
  errD0(rhs.errD0),
  z0(rhs.z0),
  errZ0(rhs.errZ0),
  mcType(rhs.mcType),
  mcOrigin(rhs.mcOrigin),
  matched2TruthLepton(rhs.matched2TruthLepton),
  //truthMatchType(rhs.truthMatchType),
  truthType(rhs.truthType),
  effSF(rhs.effSF),
  errEffSF(rhs.errEffSF),
  trigFlags(rhs.trigFlags)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Lepton& Lepton::operator=(const Lepton &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    q = rhs.q; 
    isBaseline = rhs.isBaseline;
    isSignal = rhs.isSignal;
    etcone20 = rhs.etcone20;
    ptcone20 = rhs.ptcone20;
    ptcone30 = rhs.ptcone30;
    d0 = rhs.d0;
    errD0 = rhs.errD0;
    z0 = rhs.z0;
    errZ0 = rhs.errZ0;
    mcType = rhs.mcType;
    mcOrigin = rhs.mcOrigin;
    matched2TruthLepton = rhs.matched2TruthLepton;
    //truthMatchType = rhs.truthMatchType;
    truthType = rhs.truthType;
    effSF = rhs.effSF;
    errEffSF = rhs.errEffSF;
    trigFlags = rhs.trigFlags;
  }
  return *this;
}


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


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Muon::Muon(const Muon &rhs):
  Lepton(rhs),
  isCombined(rhs.isCombined),
  idTrackPt(rhs.idTrackPt),
  idTrackEta(rhs.idTrackEta),
  idTrackPhi(rhs.idTrackPhi),
  idTrackQ(rhs.idTrackQ),
  msTrackPt(rhs.msTrackPt),
  msTrackEta(rhs.msTrackEta),
  msTrackPhi(rhs.msTrackPhi),
  msTrackQ(rhs.msTrackQ),
  thetaPV(rhs.thetaPV),
  etcone30(rhs.etcone30),
  ptcone30ElStyle(rhs.ptcone30ElStyle),
  id_theta(rhs.id_theta),
  id_phi(rhs.id_phi),
  id_qoverp(rhs.id_qoverp),
  ms_theta(rhs.ms_theta),
  ms_phi(rhs.ms_phi),
  ms_qoverp(rhs.ms_qoverp),
  isBadMuon(rhs.isBadMuon),
  isCosmic(rhs.isCosmic),
  ms_up(rhs.ms_up),
  ms_dn(rhs.ms_dn),
  id_up(rhs.id_up),
  id_dn(rhs.id_dn),
  scale_up(rhs.scale_up),
  scale_dn(rhs.scale_dn)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Muon& Muon::operator=(const Muon &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    isCombined = rhs.isCombined;
    idTrackPt = rhs.idTrackPt;
    idTrackEta = rhs.idTrackEta;
    idTrackPhi = rhs.idTrackPhi;
    idTrackQ = rhs.idTrackQ;
    msTrackPt = rhs.msTrackPt;
    msTrackEta = rhs.msTrackEta;
    msTrackPhi = rhs.msTrackPhi;
    msTrackQ = rhs.msTrackQ;
    thetaPV = rhs.thetaPV;
    etcone30 = rhs.etcone30;
    ptcone30ElStyle = rhs.ptcone30ElStyle;
    id_theta = rhs.id_theta;
    id_phi = rhs.id_phi;
    id_qoverp = rhs.id_qoverp;
    ms_theta = rhs.ms_theta;
    ms_phi = rhs.ms_phi;
    ms_qoverp = rhs.ms_qoverp;
    isBadMuon = rhs.isBadMuon;
    isCosmic = rhs.isCosmic;
    ms_up = rhs.ms_up;
    ms_dn = rhs.ms_dn;
    id_up = rhs.id_up;
    id_dn = rhs.id_dn;
    scale_up = rhs.scale_up;
    scale_dn = rhs.scale_dn;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Muon Set State
/*--------------------------------------------------------------------------------*/
void Muon::setState(int sys)
{
  resetTLV();
  if(sys == NtSys::NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys::MUONS_MS_UP ) sf = ms_up;
  else if( sys == NtSys::MUONS_MS_DN ) sf = ms_dn;
  else if( sys == NtSys::MUONS_ID_UP ) sf = id_up;
  else if( sys == NtSys::MUONS_ID_DN ) sf = id_dn;
  else if( sys == NtSys::MUONS_SCALE_UP ) sf = scale_up;
  else if( sys == NtSys::MUONS_SCALE_DN ) sf = scale_dn;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Muon print
/*--------------------------------------------------------------------------------*/
void Muon::print() const
{
  cout.precision(2);
  cout << fixed << "Mu : q " << setw(2) << q << " pt " << setw(6) << Pt() << " eta " << setw(5) << Eta()
       << " phi " << setw(5) << Phi()
       << " cb " << isCombined
       << " type " << mcType << " origin " << mcOrigin 
       // No way do we want to see this every time we print
       //<< " ms_up " << ms_up << " ms_dn " << ms_dn << " id_up " << id_up << " id_dn " << id_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Tau::Tau(const Tau &rhs):
  //Lepton(rhs),
  Particle(rhs),
  q(rhs.author),
  author(rhs.author),
  nTrack(rhs.nTrack),
  eleBDT(rhs.eleBDT),
  jetBDT(rhs.jetBDT),
  jetBDTSigLoose(rhs.jetBDTSigLoose),
  jetBDTSigMedium(rhs.jetBDTSigMedium),
  jetBDTSigTight(rhs.jetBDTSigTight),
  eleBDTLoose(rhs.eleBDTLoose),
  eleBDTMedium(rhs.eleBDTMedium),
  eleBDTTight(rhs.eleBDTTight),
  muonVeto(rhs.muonVeto),
  trueTau(rhs.trueTau),
  matched2TruthLepton(rhs.matched2TruthLepton),
  truthType(rhs.truthType),
  detailedTruthType(rhs.detailedTruthType),
  //effSF(rhs.effSF),
  //errEffSF(rhs.errEffSF),
  looseEffSF(rhs.looseEffSF),
  errLooseEffSF(rhs.errLooseEffSF),
  mediumEffSF(rhs.mediumEffSF),
  errMediumEffSF(rhs.errMediumEffSF),
  tightEffSF(rhs.tightEffSF),
  errTightEffSF(rhs.errTightEffSF),
  looseEVetoSF(rhs.looseEVetoSF),
  errLooseEVetoSF(rhs.errLooseEVetoSF),
  mediumEVetoSF(rhs.mediumEVetoSF),
  errMediumEVetoSF(rhs.errMediumEVetoSF),
  tightEVetoSF(rhs.tightEVetoSF),
  errTightEVetoSF(rhs.errTightEVetoSF),
  sme_total_up(rhs.sme_total_up),
  sme_total_dn(rhs.sme_total_dn),
  trigFlags(rhs.trigFlags)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Tau& Tau::operator=(const Tau &rhs)
{
  if (this != &rhs) {
    //Lepton::operator=(rhs);
    Particle::operator=(rhs);
    q = rhs.q;
    author = rhs.author; 
    nTrack = rhs.nTrack;
    eleBDT = rhs.eleBDT;
    jetBDT = rhs.jetBDT;
    jetBDTSigLoose = rhs.jetBDTSigLoose;
    jetBDTSigMedium = rhs.jetBDTSigMedium;
    jetBDTSigTight = rhs.jetBDTSigTight;
    eleBDTLoose = rhs.eleBDTLoose;
    eleBDTMedium = rhs.eleBDTMedium;
    eleBDTTight = rhs.eleBDTTight;
    muonVeto = rhs.muonVeto;
    trueTau = rhs.trueTau;
    matched2TruthLepton = rhs.matched2TruthLepton;
    truthType = rhs.truthType;
    detailedTruthType = rhs.detailedTruthType;
    //effSF = rhs.effSF;
    //errEffSF = rhs.errEffSF;
    looseEffSF = rhs.looseEffSF;
    errLooseEffSF = rhs.errLooseEffSF;
    mediumEffSF = rhs.mediumEffSF;
    errMediumEffSF = rhs.errMediumEffSF;
    tightEffSF = rhs.tightEffSF;
    errTightEffSF = rhs.errTightEffSF;
    looseEVetoSF = rhs.looseEVetoSF;
    errLooseEVetoSF = rhs.errLooseEVetoSF;
    mediumEVetoSF = rhs.mediumEVetoSF;
    errMediumEVetoSF = rhs.errMediumEVetoSF;
    tightEVetoSF = rhs.tightEVetoSF;
    errTightEVetoSF = rhs.errTightEVetoSF;
    sme_total_up = rhs.sme_total_up;
    sme_total_dn = rhs.sme_total_dn;
    trigFlags = rhs.trigFlags;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Tau Set State
/*--------------------------------------------------------------------------------*/
void Tau::setState(int sys)
{
  resetTLV();
  if(sys == NtSys::NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys::TAUS_SME_TOTAL_UP ) sf = sme_total_up;
  else if( sys == NtSys::TAUS_SME_TOTAL_DN ) sf = sme_total_dn;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Tau print
/*--------------------------------------------------------------------------------*/
void Tau::print() const
{
  cout.precision(2);
  cout << fixed << "Tau : q " << setw(2) << q << " Et " << setw(6) << Et() << " eta " << setw(5) << Eta()
       << " nTrk " << nTrack
       << " phi " << setw(5) << Phi()
       << " jetBDT " << jetBDTSigLoose << jetBDTSigMedium << jetBDTSigTight
       << " eleBDT " << eleBDTLoose << eleBDTMedium << eleBDTTight
       << " muVeto " << muonVeto
       << " true " << trueTau
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Photon::Photon(const Photon &rhs):
  Particle(rhs),
  isConv(rhs.isConv),
  tight(rhs.tight),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
  clusPhi(rhs.clusPhi),
  OQ(rhs.OQ),
  topoEtcone40(rhs.topoEtcone40)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Photon& Photon::operator=(const Photon &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    isConv  = rhs.isConv; 
    tight = rhs.tight;
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
    clusPhi = rhs.clusPhi;
    OQ = rhs.OQ;
    topoEtcone40 = rhs.topoEtcone40;
  }
  return *this;
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Jet::Jet(const Jet &rhs):
  Particle(rhs),
  jvf(rhs.jvf),
  jvt(rhs.jvt),
  detEta(rhs.detEta),
  emfrac(rhs.emfrac),
  truthLabel(rhs.truthLabel),
  matchTruth(rhs.matchTruth),
  sv0(rhs.sv0),
  combNN(rhs.combNN),
  mv1(rhs.mv1),
  sv1plusip3d(rhs.sv1plusip3d),
  jfit_mass(rhs.jfit_mass),
  sv0p_mass(rhs.sv0p_mass),
  svp_mass (rhs.svp_mass ),
  isBadVeryLoose(rhs.isBadVeryLoose),
  isHotTile(rhs.isHotTile),
  bch_corr_jet(rhs.bch_corr_jet),
  bch_corr_cell(rhs.bch_corr_cell),
  isBadMediumBCH(rhs.isBadMediumBCH),
  isBadMediumBCH_up(rhs.isBadMediumBCH_up),
  isBadMediumBCH_dn(rhs.isBadMediumBCH_dn),
  isBadTightBCH(rhs.isBadTightBCH),
  jes_up(rhs.jes_up),
  jes_dn(rhs.jes_dn),
  jer(rhs.jer),
  bjes(rhs.bjes),
  effNp(rhs.effNp),
  etaInter(rhs.etaInter),
  flavor(rhs.flavor),
  pileup(rhs.pileup),
  punchThrough(rhs.punchThrough),
  relativeNC(rhs.relativeNC),
  singlePart(rhs.singlePart),
  met_wpx(rhs.met_wpx),
  met_wpy(rhs.met_wpy)
{ 
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Jet& Jet::operator=(const Jet &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    jvf  = rhs.jvf; 
    jvt  = rhs.jvt; 
    detEta = rhs.detEta;
    emfrac = rhs.emfrac;
    truthLabel = rhs.truthLabel;
    matchTruth = rhs.matchTruth;
    sv0 = rhs.sv0;
    combNN = rhs.combNN;
    mv1 = rhs.mv1;
    jfit_mass = rhs.jfit_mass;
    sv0p_mass = rhs.sv0p_mass;
    svp_mass  = rhs.svp_mass ;
    isBadVeryLoose = rhs.isBadVeryLoose;
    isHotTile = rhs.isHotTile;
    bch_corr_jet = rhs.bch_corr_jet;
    bch_corr_cell = rhs.bch_corr_cell;
    isBadMediumBCH = rhs.isBadMediumBCH;
    isBadMediumBCH_up = rhs.isBadMediumBCH_up;
    isBadMediumBCH_dn = rhs.isBadMediumBCH_dn;
    isBadTightBCH = rhs.isBadTightBCH;
    jes_up = rhs.jes_up;
    jes_dn = rhs.jes_dn;
    jer = rhs.jer;
    bjes=rhs.bjes;
    effNp=rhs.effNp;
    etaInter=rhs.etaInter;
    flavor=rhs.flavor;
    pileup=rhs.pileup;
    punchThrough=rhs.punchThrough;
    relativeNC=rhs.relativeNC;
    singlePart=rhs.singlePart;
    met_wpx = rhs.met_wpx;
    met_wpy = rhs.met_wpy;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Jet Set State
/*--------------------------------------------------------------------------------*/
void Jet::setState(int sys)
{
  resetTLV();
  if(sys == NtSys::NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys::JER) sf = jer;
  else if( sys == NtSys::JET_BJES_Response_DN) sf = bjes[0];
  else if( sys == NtSys::JET_BJES_Response_UP) sf = bjes[1];
  else if( sys == NtSys::JET_EffectiveNP_1_DN) sf = effNp[0];
  else if( sys == NtSys::JET_EffectiveNP_1_UP) sf = effNp[1];
  else if( sys == NtSys::JET_EffectiveNP_2_DN) sf = effNp[2];
  else if( sys == NtSys::JET_EffectiveNP_2_UP) sf = effNp[3];
  else if( sys == NtSys::JET_EffectiveNP_3_DN) sf = effNp[4];
  else if( sys == NtSys::JET_EffectiveNP_3_UP) sf = effNp[5];
  else if( sys == NtSys::JET_EffectiveNP_4_DN) sf = effNp[6];
  else if( sys == NtSys::JET_EffectiveNP_4_UP) sf = effNp[7];
  else if( sys == NtSys::JET_EffectiveNP_5_DN) sf = effNp[8];
  else if( sys == NtSys::JET_EffectiveNP_5_UP) sf = effNp[9];
  else if( sys == NtSys::JET_EffectiveNP_6restTerm_DN) sf = effNp[10];
  else if( sys == NtSys::JET_EffectiveNP_6restTerm_UP) sf =  effNp[11];
  else if( sys == NtSys::JET_EtaIntercalibration_Modelling_DN) sf = etaInter[0];
  else if( sys == NtSys::JET_EtaIntercalibration_Modelling_UP) sf = etaInter[1];
  else if( sys == NtSys::JET_EtaIntercalibration_TotalStat_DN) sf = etaInter[2];
  else if( sys == NtSys::JET_EtaIntercalibration_TotalStat_UP) sf = etaInter[3];
  else if( sys == NtSys::JET_Flavor_Composition_DN) sf = flavor[0];
  else if( sys == NtSys::JET_Flavor_Composition_UP) sf = flavor[1];
  else if( sys == NtSys::JET_Flavor_Response_DN) sf = flavor[2];
  else if( sys == NtSys::JET_Flavor_Response_UP) sf = flavor[3];
  else if( sys == NtSys::JET_Pileup_OffsetMu_DN) sf = pileup[0];
  else if( sys == NtSys::JET_Pileup_OffsetMu_UP) sf =  pileup[1];
  else if( sys == NtSys::JET_Pileup_OffsetNPV_DN) sf = pileup[2];
  else if( sys == NtSys::JET_Pileup_OffsetNPV_UP) sf = pileup[3];
  else if( sys == NtSys::JET_Pileup_PtTerm_DN) sf =  pileup[4];
  else if( sys == NtSys::JET_Pileup_PtTerm_UP) sf =  pileup[5];
  else if( sys == NtSys::JET_Pileup_RhoTopology_DN) sf =  pileup[6];
  else if( sys == NtSys::JET_Pileup_RhoTopology_UP) sf =  pileup[7];
  else if( sys == NtSys::JET_PunchThrough_MC12_DN) sf = punchThrough[0];
  else if( sys == NtSys::JET_PunchThrough_MC12_UP) sf = punchThrough[1];
  else if( sys == NtSys::JET_RelativeNonClosure_MC12_DN) sf = relativeNC[0];
  else if( sys == NtSys::JET_RelativeNonClosure_MC12_UP) sf = relativeNC[1];
  else if( sys == NtSys::JET_SingleParticle_HighPt_DN) sf = singlePart[0];
  else if( sys == NtSys::JET_SingleParticle_HighPt_UP) sf = singlePart[1];
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Jet print
/*--------------------------------------------------------------------------------*/
void Jet::print() const
{
  cout.precision(2);
  cout << fixed << "Jet : pt " << setw(6) << Pt() << " eta " << setw(5) << Eta()
       << " phi " << setw(5) << Phi()
       << " jvf " << setw(4) << jvf
       << " jvt " << setw(4) << jvt
       << " mv1 " << setw(5) << mv1
       // No way do we want to see this every time we print
       //<< " jer " << jer << " jes_up " << jes_up << " jes_dn " << jes_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Met::Met(const Met &rhs):
  TObject(rhs),
  Et(rhs.Et),
  phi(rhs.phi),
  sumet(rhs.sumet),
  refEle(rhs.refEle),
  refEle_etx(rhs.refEle_etx),
  refEle_ety(rhs.refEle_ety),
  refEle_sumet(rhs.refEle_sumet),
  refMuo(rhs.refMuo),
  refMuo_etx(rhs.refMuo_etx),
  refMuo_ety(rhs.refMuo_ety),
  refMuo_sumet(rhs.refMuo_sumet),
  refJet(rhs.refJet),
  refJet_etx(rhs.refJet_etx),
  refJet_ety(rhs.refJet_ety),
  refJet_sumet(rhs.refJet_sumet),
  softJet(rhs.softJet),
  softJet_etx(rhs.softJet_etx),
  softJet_ety(rhs.softJet_ety),
  refGamma(rhs.refGamma),
  refGamma_etx(rhs.refGamma_etx),
  refGamma_ety(rhs.refGamma_ety),
  refGamma_sumet(rhs.refGamma_sumet),
  refCell(rhs.refCell),
  refCell_etx(rhs.refCell_etx),
  refCell_ety(rhs.refCell_ety),
  softTerm(rhs.softTerm),
  softTerm_etx(rhs.softTerm_etx),
  softTerm_ety(rhs.softTerm_ety),
  softTerm_sumet(rhs.softTerm_sumet),
  sys(rhs.sys)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Met& Met::operator=(const Met &rhs)
{
  if (this != &rhs) {
    TObject::operator=(rhs);
    Et = rhs.Et; 
    phi = rhs.phi; 
    sumet = rhs.sumet;
    refEle = rhs.refEle; 
    refEle_etx = rhs.refEle_etx; 
    refEle_ety = rhs.refEle_ety; 
    refEle_sumet = rhs.refEle_sumet; 
    refMuo = rhs.refMuo; 
    refMuo_etx = rhs.refMuo_etx; 
    refMuo_ety = rhs.refMuo_ety; 
    refMuo_sumet = rhs.refMuo_sumet; 
    refJet = rhs.refJet; 
    refJet_etx = rhs.refJet_etx; 
    refJet_ety = rhs.refJet_ety; 
    refJet_sumet = rhs.refJet_sumet; 
    softJet = rhs.softJet; 
    softJet_etx = rhs.softJet_etx; 
    softJet_ety = rhs.softJet_ety; 
    refGamma = rhs.refGamma; 
    refGamma_etx = rhs.refGamma_etx; 
    refGamma_ety = rhs.refGamma_ety; 
    refGamma_sumet = rhs.refGamma_sumet; 
    refCell = rhs.refCell; 
    refCell_etx = rhs.refCell_etx; 
    refCell_ety = rhs.refCell_ety; 
    softTerm_etx = rhs.softTerm_etx;
    softTerm_ety = rhs.softTerm_ety;
    softTerm_sumet = rhs.softTerm_sumet;
    sys = rhs.sys; 
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Met print
/*--------------------------------------------------------------------------------*/
void Met::print() const
{
  cout.precision(2);
  cout << fixed << "Met : pt " << setw(6) << Et << " phi " << setw(4) << phi
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
TruthParticle::TruthParticle(const TruthParticle &rhs):
  Particle(rhs),
  pdgId(rhs.pdgId),
  charge(rhs.charge),
  status(rhs.status),
  motherPdgId(rhs.motherPdgId)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
TruthParticle& TruthParticle::operator=(const TruthParticle &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    pdgId       = rhs.pdgId;
    charge      = rhs.charge;
    status      = rhs.status;
    motherPdgId = rhs.motherPdgId;    
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// TruthParticle print
/*--------------------------------------------------------------------------------*/
void TruthParticle::print() const
{
  cout.precision(2);
  cout << fixed << "TruthParticle : pt " << setw(6) << Pt() << " eta " << setw(5) << Eta()
       << " phi "         << setw(5)  << Phi()
       << " pdgId "       << setw(10) << pdgId
       << " charge "      << setw(2)  << charge
       //<< " status "      << setw(2)  << status
       //<< " motherPdgId " << setw(10) << motherPdgId 
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
TruthJet::TruthJet(const TruthJet &rhs):
  Particle(rhs),
  flavor(rhs.flavor)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
TruthJet& TruthJet::operator=(const TruthJet &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    flavor = rhs.flavor;    
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// TruthJet print
/*--------------------------------------------------------------------------------*/
void TruthJet::print() const
{
  cout.precision(2);
  cout << fixed << "TruthJet : pt " << setw(6) << Pt() << " eta " << setw(5) << Eta()
       << " phi "    << setw(5)  << Phi()
       << " flavor " << setw(5) << flavor 
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
TruthMet::TruthMet(const TruthMet &rhs):
  TObject(rhs),
  Et(rhs.Et),
  phi(rhs.phi)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
TruthMet& TruthMet::operator=(const TruthMet &rhs)
{
  if (this != &rhs) {
    TObject::operator=(rhs);
    Et  = rhs.Et; 
    phi = rhs.phi; 
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// TruthMet print
/*--------------------------------------------------------------------------------*/
void TruthMet::print() const
{
  cout.precision(2);
  cout << fixed << "TruthMet : pt " << setw(6) << Et << " phi " << setw(4) << phi
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
