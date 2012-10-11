#include <iostream>
#include <iomanip>
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
//Copy constructor
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
//Assignment operator
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
//Copy constructor
/*--------------------------------------------------------------------------------*/
Lepton::Lepton(const Lepton &rhs):
  Particle(rhs),
  q(rhs.q),
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
  truthMatchType(rhs.truthMatchType),
  effSF(rhs.effSF),
  errEffSF(rhs.errEffSF),
  trigFlags(rhs.trigFlags)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
/*--------------------------------------------------------------------------------*/
Lepton& Lepton::operator=(const Lepton &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    q = rhs.q; 
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
    truthMatchType = rhs.truthMatchType;
    effSF = rhs.effSF;
    errEffSF = rhs.errEffSF;
    trigFlags = rhs.trigFlags;
  }
  return *this;
}

/*--------------------------------------------------------------------------------*/
//Copy constructor
/*--------------------------------------------------------------------------------*/
Electron::Electron(const Electron &rhs):
  Lepton(rhs),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
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
//Assignment operator
/*--------------------------------------------------------------------------------*/
Electron& Electron::operator=(const Electron &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
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
//Copy constructor
/*--------------------------------------------------------------------------------*/
Muon::Muon(const Muon &rhs):
  Lepton(rhs),
  isCombined(rhs.isCombined),
  thetaPV(rhs.thetaPV),
  etcone30(rhs.etcone30),
  ptcone30ElStyle(rhs.ptcone30ElStyle),
  ms_up(rhs.ms_up),
  ms_dn(rhs.ms_dn),
  id_up(rhs.id_up),
  id_dn(rhs.id_dn)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
/*--------------------------------------------------------------------------------*/
Muon& Muon::operator=(const Muon &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
    isCombined = rhs.isCombined;
    thetaPV = rhs.thetaPV;
    etcone30 = rhs.etcone30;
    ptcone30ElStyle = rhs.ptcone30ElStyle;
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
void Muon::setState(int sys)
{
  resetTLV();
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys_MS_UP ) sf = ms_up;
  else if( sys == NtSys_MS_DN ) sf = ms_dn;
  else if( sys == NtSys_ID_UP ) sf = id_up;
  else if( sys == NtSys_ID_DN ) sf = id_dn;
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
//Copy constructor
/*--------------------------------------------------------------------------------*/
Jet::Jet(const Jet &rhs):
  Particle(rhs),
  jvf(rhs.jvf),
  truthLabel(rhs.truthLabel),
  matchTruth(rhs.matchTruth),
  sv0(rhs.sv0),
  combNN(rhs.combNN),
  mv1(rhs.mv1),
  jes_up(rhs.jes_up),
  jes_dn(rhs.jes_dn),
  jer(rhs.jer)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
/*--------------------------------------------------------------------------------*/
Jet& Jet::operator=(const Jet &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    jvf  = rhs.jvf; 
    truthLabel = rhs.truthLabel;
    matchTruth = rhs.matchTruth;
    sv0 = rhs.sv0;
    combNN = rhs.combNN;
    mv1 = rhs.mv1;
    jes_up = rhs.jes_up;
    jes_dn = rhs.jes_dn;
    jer = rhs.jer;
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
//Copy constructor
/*--------------------------------------------------------------------------------*/
Tau::Tau(const Tau &rhs):
  Lepton(rhs),
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
  trueTau(rhs.trueTau)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
/*--------------------------------------------------------------------------------*/
Tau& Tau::operator=(const Tau &rhs)
{
  if (this != &rhs) {
    Lepton::operator=(rhs);
     author  = rhs.author; 
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
  }
  return *this;
}


/*--------------------------------------------------------------------------------*/
// Tau print
/*--------------------------------------------------------------------------------*/
void Tau::print() const
{
  cout.precision(2);
  cout << fixed << "Tau : q " << setw(2) << q << " Et " << setw(6) << Et() << " eta " << setw(5) << Eta()
       << " phi " << setw(5) << Phi()
       << " jetBDT " << jetBDTSigLoose << jetBDTSigMedium << jetBDTSigTight
       << " eleBDT " << eleBDTLoose << eleBDTMedium << eleBDTTight
       << " muVeto " << muonVeto
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}

/*--------------------------------------------------------------------------------*/
//Copy constructor
/*--------------------------------------------------------------------------------*/
Photon::Photon(const Photon &rhs):
  Particle(rhs),
  isConv(rhs.isConv)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
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
//Copy constructor
/*--------------------------------------------------------------------------------*/
Met::Met(const Met &rhs):
  TObject(rhs),
  Et(rhs.Et),
  phi(rhs.phi),
  refEle(rhs.refEle),
  refMuo(rhs.refMuo),
  refJet(rhs.refJet),
  softJet(rhs.softJet),
  refGamma(rhs.refGamma),
  refCell(rhs.refCell),
  sys(rhs.sys)
{
}
/*--------------------------------------------------------------------------------*/
//Assignment operator
/*--------------------------------------------------------------------------------*/
Met& Met::operator=(const Met &rhs)
{
  if (this != &rhs) {
    TObject::operator=(rhs);
     Et  = rhs.Et; 
     phi  = rhs.phi; 
     refEle= rhs.refEle; 
     refMuo= rhs.refMuo; 
     refJet= rhs.refJet; 
     softJet= rhs.softJet; 
     refGamma= rhs.refGamma; 
     refCell= rhs.refCell; 
     sys= rhs.sys; 

  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Event print
/*--------------------------------------------------------------------------------*/
void Event::print() const
{
  cout << "Run " << run << " Event " << event << " Stream " << streamName(stream) 
       << " w " << w << endl;
}

