#include <iostream>
#include <iomanip>
#include "SusyNtuple/Muon.h"

using namespace std;

using Susy::Muon;
using Susy::Lepton;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Muon::Muon(const Muon &rhs):
  Lepton(rhs),
  isCaloTagged(rhs.isCaloTagged),
  isSiForward(rhs.isSiForward),
  isCombined(rhs.isCombined),
  ghostTrack(rhs.ghostTrack),
  idTrackPt(rhs.idTrackPt),
  idTrackEta(rhs.idTrackEta),
  idTrackPhi(rhs.idTrackPhi),
  idTrackQ(rhs.idTrackQ),
  idTrackTheta(rhs.idTrackTheta),
  idTrackQoverP(rhs.idTrackQoverP),
  msTrackPt(rhs.msTrackPt),
  msTrackEta(rhs.msTrackEta),
  msTrackPhi(rhs.msTrackPhi),
  msTrackQ(rhs.msTrackQ),
  msTrackTheta(rhs.msTrackTheta),
  msTrackQoverP(rhs.msTrackQoverP),
  veryLoose(rhs.veryLoose),
  loose(rhs.loose),
  medium(rhs.medium),
  tight(rhs.tight),
  muoEffSF(rhs.muoEffSF),
  muoTrigSF(rhs.muoTrigSF),
  muoTrigEffData_medium(rhs.muoTrigEffData_medium),
  muoTrigEffMC_medium(rhs.muoTrigEffMC_medium),
  muoTrigEffData_loose(rhs.muoTrigEffData_loose),
  muoTrigEffMC_loose(rhs.muoTrigEffMC_loose),
  muoTrigEffErrData_stat_up_medium(rhs.muoTrigEffErrData_stat_up_medium),
  muoTrigEffErrData_stat_dn_medium(rhs.muoTrigEffErrData_stat_dn_medium),
  muoTrigEffErrMC_stat_up_medium(rhs.muoTrigEffErrMC_stat_up_medium),
  muoTrigEffErrMC_stat_dn_medium(rhs.muoTrigEffErrMC_stat_dn_medium),
  muoTrigEffErrData_syst_up_medium(rhs.muoTrigEffErrData_syst_up_medium),
  muoTrigEffErrData_syst_dn_medium(rhs.muoTrigEffErrData_syst_dn_medium),
  muoTrigEffErrMC_syst_up_medium(rhs.muoTrigEffErrMC_syst_up_medium),
  muoTrigEffErrMC_syst_dn_medium(rhs.muoTrigEffErrMC_syst_dn_medium),
  diMuTrigMap(rhs.diMuTrigMap),
  isBadMuon(rhs.isBadMuon),
  isCosmic(rhs.isCosmic),
  ms_up(rhs.ms_up),
  ms_dn(rhs.ms_dn),
  id_up(rhs.id_up),
  id_dn(rhs.id_dn),
  scale_up(rhs.scale_up),
  scale_dn(rhs.scale_dn),
  sagitta_bias_dn(rhs.sagitta_bias_dn),
  sagitta_bias_up(rhs.sagitta_bias_up),
  sagitta_rho_dn(rhs.sagitta_rho_dn),
  sagitta_rho_up(rhs.sagitta_rho_up),
  errEffSF_stat_up(rhs.errEffSF_stat_up),
  errEffSF_stat_dn(rhs.errEffSF_stat_dn),
  errEffSF_syst_up(rhs.errEffSF_syst_up),
  errEffSF_syst_dn(rhs.errEffSF_syst_dn),
  errEffSF_stat_lowpt_up(rhs.errEffSF_stat_lowpt_up),
  errEffSF_stat_lowpt_dn(rhs.errEffSF_stat_lowpt_dn),
  errEffSF_syst_lowpt_up(rhs.errEffSF_syst_lowpt_up),
  errEffSF_syst_lowpt_dn(rhs.errEffSF_syst_lowpt_dn),
  errIso_stat_up(rhs.errIso_stat_up),
  errIso_stat_dn(rhs.errIso_stat_dn),
  errIso_syst_up(rhs.errIso_syst_up),
  errIso_syst_dn(rhs.errIso_syst_dn),
  errTTVA_stat_up(rhs.errTTVA_stat_up),
  errTTVA_stat_dn(rhs.errTTVA_stat_dn),
  errTTVA_syst_up(rhs.errTTVA_syst_up),
  errTTVA_syst_dn(rhs.errTTVA_syst_dn),
  errBadMu_stat_dn(rhs.errBadMu_stat_dn),
  errBadMu_stat_up(rhs.errBadMu_stat_up),
  errBadMu_syst_dn(rhs.errBadMu_syst_dn),
  errBadMu_syst_up(rhs.errBadMu_syst_up)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Muon& Muon::operator=(const Muon &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    isCaloTagged = rhs.isCaloTagged;
    isSiForward = rhs.isSiForward;
    isCombined = rhs.isCombined;
    ghostTrack = rhs.ghostTrack;
    idTrackPt = rhs.idTrackPt;
    idTrackEta = rhs.idTrackEta;
    idTrackPhi = rhs.idTrackPhi;
    idTrackQ = rhs.idTrackQ;
    idTrackTheta = rhs.idTrackTheta;
    idTrackQoverP = rhs.idTrackQoverP;
    msTrackPt = rhs.msTrackPt;
    msTrackEta = rhs.msTrackEta;
    msTrackPhi = rhs.msTrackPhi;
    msTrackQ = rhs.msTrackQ;
    msTrackTheta = rhs.msTrackTheta;
    msTrackQoverP = rhs.msTrackQoverP;
    veryLoose = rhs.veryLoose;
    loose = rhs.loose;
    medium = rhs.medium;
    tight = rhs.tight;
    diMuTrigMap = rhs.diMuTrigMap;
    isBadMuon = rhs.isBadMuon;
    isCosmic = rhs.isCosmic;
    ms_up = rhs.ms_up;
    ms_dn = rhs.ms_dn;
    id_up = rhs.id_up;
    id_dn = rhs.id_dn;
    scale_up = rhs.scale_up;
    scale_dn = rhs.scale_dn;
    sagitta_bias_dn = rhs.sagitta_bias_dn;
    sagitta_bias_up = rhs.sagitta_bias_up;
    sagitta_rho_dn = rhs.sagitta_rho_dn;
    sagitta_rho_up = rhs.sagitta_rho_up;
    muoEffSF = rhs.muoEffSF;
    muoTrigSF = rhs.muoTrigSF;
    muoTrigEffData_medium = rhs.muoTrigEffData_medium;
    muoTrigEffMC_medium = rhs.muoTrigEffMC_medium;
    muoTrigEffData_loose = rhs.muoTrigEffData_loose;
    muoTrigEffMC_loose = rhs.muoTrigEffMC_loose;
    muoTrigEffErrData_stat_up_medium = rhs.muoTrigEffErrData_stat_up_medium;
    muoTrigEffErrData_stat_dn_medium = rhs.muoTrigEffErrData_stat_dn_medium;
    muoTrigEffErrMC_stat_up_medium = rhs.muoTrigEffErrMC_stat_up_medium;
    muoTrigEffErrMC_stat_dn_medium = rhs.muoTrigEffErrMC_stat_dn_medium;
    muoTrigEffErrData_syst_up_medium = rhs.muoTrigEffErrData_syst_up_medium;
    muoTrigEffErrData_syst_dn_medium = rhs.muoTrigEffErrData_syst_dn_medium;
    muoTrigEffErrMC_syst_up_medium = rhs.muoTrigEffErrMC_syst_up_medium;
    muoTrigEffErrMC_syst_dn_medium = rhs.muoTrigEffErrMC_syst_dn_medium;
    errEffSF_stat_up = rhs.errEffSF_stat_up;
    errEffSF_stat_dn = rhs.errEffSF_stat_dn;
    errEffSF_syst_up = rhs.errEffSF_syst_up;
    errEffSF_syst_dn = rhs.errEffSF_syst_dn;
    errEffSF_stat_lowpt_up = rhs.errEffSF_stat_lowpt_up;
    errEffSF_stat_lowpt_dn = rhs.errEffSF_stat_lowpt_dn;
    errEffSF_syst_lowpt_up = rhs.errEffSF_syst_lowpt_up;
    errEffSF_syst_lowpt_dn = rhs.errEffSF_syst_lowpt_dn;
    errIso_stat_up = rhs.errIso_stat_up;
    errIso_stat_dn = rhs.errIso_stat_dn;
    errIso_syst_up = rhs.errIso_syst_up;
    errIso_syst_dn = rhs.errIso_syst_dn;
    errTTVA_stat_up = rhs.errTTVA_stat_up;
    errTTVA_stat_dn = rhs.errTTVA_stat_dn;
    errTTVA_syst_up = rhs.errTTVA_syst_up;
    errTTVA_syst_dn = rhs.errTTVA_syst_dn;
    errBadMu_stat_dn = rhs.errBadMu_stat_dn;
    errBadMu_stat_up = rhs.errBadMu_stat_up;
    errBadMu_syst_dn = rhs.errBadMu_syst_dn;
    errBadMu_syst_up = rhs.errBadMu_syst_up;
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
  if     ( sys == NtSys::MUON_MS_UP ) sf = ms_up;
  else if( sys == NtSys::MUON_MS_DN ) sf = ms_dn;
  else if( sys == NtSys::MUON_ID_UP ) sf = id_up;
  else if( sys == NtSys::MUON_ID_DN ) sf = id_dn;
  else if( sys == NtSys::MUON_SCALE_UP ) sf = scale_up;
  else if( sys == NtSys::MUON_SCALE_DN ) sf = scale_dn;
  else if( sys == NtSys::MUON_SAGITTA_RESBIAS_UP ) sf = sagitta_bias_up;
  else if( sys == NtSys::MUON_SAGITTA_RESBIAS_DN ) sf = sagitta_bias_dn;
  else if( sys == NtSys::MUON_SAGITTA_RHO_UP ) sf = sagitta_rho_up;
  else if( sys == NtSys::MUON_SAGITTA_RHO_DN ) sf = sagitta_rho_dn;
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
       << " loose " << loose
       << " medium " << medium
       << " isoGradientLoose " << isoGradientLoose
       << " isCosmic " << isCosmic
       << " isBadMuon " << isBadMuon
       //<< " type " << mcType << " origin " << mcOrigin 
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}

