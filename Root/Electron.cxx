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
  author(rhs.author),
  authorElectron(rhs.authorElectron),
  authorAmbiguous(rhs.authorAmbiguous),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
  clusPhi(rhs.clusPhi),
  clusEtaBE(rhs.clusEtaBE),
  clusPhiBE(rhs.clusPhiBE),
  trackPt(rhs.trackPt),
  trackEta(rhs.trackEta),
  veryLooseLLH(rhs.veryLooseLLH),
  looseLLH(rhs.looseLLH),
  looseLLHBLayer(rhs.looseLLHBLayer),
  mediumLLH(rhs.mediumLLH),
  tightLLH(rhs.tightLLH),
  passOQBadClusElectron(rhs.passOQBadClusElectron),
  eleEffSF(rhs.eleEffSF),
  eleTrigSF_single(rhs.eleTrigSF_single),
  eleTrigSF_double(rhs.eleTrigSF_double),
  eleTrigSF_mixed(rhs.eleTrigSF_mixed),
  eleCHFSF(rhs.eleCHFSF),
  sharedMuTrk(rhs.sharedMuTrk),
  sharedEleEleTrk(rhs.sharedEleEleTrk),
  isChargeFlip(rhs.isChargeFlip),
  truthCharge(rhs.truthCharge),
  ss3lChargeFlip(rhs.ss3lChargeFlip),
  passChargeFlipTagger(rhs.passChargeFlipTagger),
  chargeFlipBDT(rhs.chargeFlipBDT),
  res_all_dn(rhs.res_all_dn),
  res_all_up(rhs.res_all_up),
  scale_all_dn(rhs.scale_all_dn),
  scale_all_up(rhs.scale_all_up),
  errEffSF_id_up(rhs.errEffSF_id_up),
  errEffSF_id_dn(rhs.errEffSF_id_dn),
  errEffSF_reco_up(rhs.errEffSF_reco_up),
  errEffSF_reco_dn(rhs.errEffSF_reco_dn),
  errEffSF_iso_up(rhs.errEffSF_iso_up),
  errEffSF_iso_dn(rhs.errEffSF_iso_dn),
  errEffSF_trig_up(rhs.errEffSF_trig_up),
  errEffSF_trig_dn(rhs.errEffSF_trig_dn)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Electron& Electron::operator=(const Electron &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    author = rhs.author;
    authorElectron = rhs.authorElectron;
    authorAmbiguous = rhs.authorAmbiguous;
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
    clusPhi = rhs.clusPhi;
    clusEtaBE = rhs.clusEtaBE;
    clusPhiBE = rhs.clusPhiBE;
    trackPt = rhs.trackPt;
    trackEta = rhs.trackEta;
    veryLooseLLH = rhs.veryLooseLLH;
    looseLLH = rhs.looseLLH;
    looseLLHBLayer = rhs.looseLLHBLayer;
    mediumLLH = rhs.mediumLLH;
    tightLLH = rhs.tightLLH;
    passOQBadClusElectron = rhs.passOQBadClusElectron;
    eleEffSF = rhs.eleEffSF;
    eleTrigSF_single = rhs.eleTrigSF_single;
    eleTrigSF_double = rhs.eleTrigSF_double;
    eleTrigSF_mixed  = rhs.eleTrigSF_mixed;
    eleCHFSF = rhs.eleCHFSF;
    sharedMuTrk = rhs.sharedMuTrk;
    sharedEleEleTrk = rhs.sharedEleEleTrk;
    isChargeFlip = rhs.isChargeFlip;
    truthCharge = rhs.truthCharge;
    ss3lChargeFlip = rhs.ss3lChargeFlip;
    passChargeFlipTagger = rhs.passChargeFlipTagger;
    chargeFlipBDT = rhs.chargeFlipBDT;
 
    res_all_dn = rhs.res_all_dn;
    res_all_up = res_all_up;
    scale_all_dn = rhs.scale_all_dn;
    scale_all_up = rhs.scale_all_up;
    errEffSF_id_up = rhs.errEffSF_id_up;
    errEffSF_id_dn = rhs.errEffSF_id_dn;
    errEffSF_reco_up = rhs.errEffSF_reco_up;
    errEffSF_reco_dn = rhs.errEffSF_reco_dn;
    errEffSF_iso_up = rhs.errEffSF_iso_up;
    errEffSF_iso_dn = rhs.errEffSF_iso_dn;
    errEffSF_trig_up = rhs.errEffSF_trig_up;
    errEffSF_trig_dn = rhs.errEffSF_trig_dn;
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
  
  if     ( sys == NtSys::EG_RESOLUTION_ALL_DN ) sf = res_all_dn;
  else if( sys == NtSys::EG_RESOLUTION_ALL_UP ) sf = res_all_up;
  else if( sys == NtSys::EG_SCALE_ALL_DN ) sf = scale_all_dn;
  else if( sys == NtSys::EG_SCALE_ALL_UP ) sf = scale_all_up;
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
       << " cluster eta " << setw(5) << clusEta
       << " loose " << looseLLH 
       << " medium " << mediumLLH 
       << " isoGradientLoose " << isoGradientLoose
       //<< " type " << mcType << " origin " << mcOrigin 
       << " OQ " << (passOQBadClusElectron ? 1 : 0)
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
