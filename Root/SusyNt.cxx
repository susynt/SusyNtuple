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
  etcone20(rhs.etcone20),
  ptcone20(rhs.ptcone20),
  ptcone30(rhs.ptcone30),
  d0(rhs.d0),
  errD0(rhs.errD0),
  z0(rhs.z0),
  errZ0(rhs.errZ0),
  d0Unbiased(rhs.d0Unbiased),
  errD0Unbiased(rhs.errD0Unbiased),
  z0Unbiased(rhs.z0Unbiased),
  errZ0Unbiased(rhs.errZ0Unbiased),
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
    etcone20 = rhs.etcone20;
    ptcone20 = rhs.ptcone20;
    ptcone30 = rhs.ptcone30;
    d0 = rhs.d0;
    errD0 = rhs.errD0;
    z0 = rhs.z0;
    errZ0 = rhs.errZ0;
    d0Unbiased = rhs.d0Unbiased;
    errD0Unbiased = rhs.errD0Unbiased;
    z0Unbiased = rhs.z0Unbiased;
    errZ0Unbiased = rhs.errZ0Unbiased;
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
  etcone30Corr(rhs.etcone30Corr),
  topoEtcone30Corr(rhs.topoEtcone30Corr),
  isChargeFlip(rhs.isChargeFlip),
  ees_z_up(rhs.ees_z_up),
  ees_z_dn(rhs.ees_z_dn),  
  ees_mat_up(rhs.ees_mat_up),
  ees_mat_dn(rhs.ees_mat_dn),
  ees_ps_up(rhs.ees_ps_up),
  ees_ps_dn(rhs. ees_ps_dn),
  ees_low_up(rhs.ees_low_up),
  ees_low_dn(rhs.ees_low_dn),
  eer_up(rhs.eer_up),
  eer_dn(rhs.eer_dn)
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
    etcone30Corr = rhs.etcone30Corr;
    topoEtcone30Corr = rhs.topoEtcone30Corr;
    isChargeFlip = rhs.isChargeFlip;
    ees_z_up = rhs.ees_z_up;
    ees_z_dn = rhs.ees_z_dn;  
    ees_mat_up = rhs.ees_mat_up;
    ees_mat_dn = rhs.ees_mat_dn;
    ees_ps_up = rhs.ees_ps_up;
    ees_ps_dn = rhs. ees_ps_dn;
    ees_low_up = rhs.ees_low_up;
    ees_low_dn = rhs.ees_low_dn;
    eer_up = rhs.eer_up;
    eer_dn = rhs.eer_dn;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Electron Set State
/*--------------------------------------------------------------------------------*/
void Electron::setState(int sys)
{
  resetTLV();
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  //if     ( sys == NtSys_EES_UP ) sf = ees_up;
  //else if( sys == NtSys_EES_DN ) sf = ees_dn;
  if     ( sys == NtSys_EES_Z_UP   ) sf = ees_z_up;
  else if( sys == NtSys_EES_Z_DN   ) sf = ees_z_dn;
  else if( sys == NtSys_EES_MAT_UP ) sf = ees_mat_up;
  else if( sys == NtSys_EES_MAT_DN ) sf = ees_mat_dn;
  else if( sys == NtSys_EES_PS_UP  ) sf = ees_ps_up;
  else if( sys == NtSys_EES_PS_DN  ) sf = ees_ps_dn;
  else if( sys == NtSys_EES_LOW_UP ) sf = ees_low_up;
  else if( sys == NtSys_EES_LOW_DN ) sf = ees_low_dn;
  else if( sys == NtSys_EER_UP     ) sf = eer_up;
  else if( sys == NtSys_EER_DN     ) sf = eer_dn;
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
  id_dn(rhs.id_dn)
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
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Muon Set State
/*--------------------------------------------------------------------------------*/
void Muon::setState(int sys, bool isTag0150)
{
  resetTLV();
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys_MS_UP ) sf = ms_up;
  else if( sys == NtSys_MS_DN ) sf = ms_dn;
  else if( sys == NtSys_ID_UP ) sf = id_up;
  else if( sys == NtSys_ID_DN ) sf = id_dn;
  else return;

  // Bugfix for SusyNt tag n0150
  if(isTag0150) sf *= 1000.;

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
  tes_up(rhs.tes_up),
  tes_dn(rhs.tes_dn),
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
    tes_up = rhs.tes_up;
    tes_dn = rhs.tes_dn;
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
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys_TES_UP ) sf = tes_up;
  else if( sys == NtSys_TES_DN ) sf = tes_dn;
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
  isConv(rhs.isConv)
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
  }
  return *this;
}


/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Jet::Jet(const Jet &rhs):
  Particle(rhs),
  jvf(rhs.jvf),
  detEta(rhs.detEta),
  emfrac(rhs.emfrac),
  truthLabel(rhs.truthLabel),
  matchTruth(rhs.matchTruth),
  sv0(rhs.sv0),
  combNN(rhs.combNN),
  mv1(rhs.mv1),
  jfit_mass(rhs.jfit_mass),
  sv0p_mass(rhs.sv0p_mass),
  svp_mass (rhs.svp_mass ),
  isBadVeryLoose(rhs.isBadVeryLoose),
  isHotTile(rhs.isHotTile),
  bch_corr_jet(rhs.bch_corr_jet),
  bch_corr_cell(rhs.bch_corr_cell),
  jes_up(rhs.jes_up),
  jes_dn(rhs.jes_dn),
  jer(rhs.jer),
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
    jes_up = rhs.jes_up;
    jes_dn = rhs.jes_dn;
    jer = rhs.jer;
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
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys_JER    ) sf = jer;
  else if( sys == NtSys_JES_UP ) sf = jes_up;
  else if( sys == NtSys_JES_DN ) sf = jes_dn;
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
