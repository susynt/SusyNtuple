#include "SusyNtuple/Tau.h"

#include <iostream>
#include <iomanip>

using namespace std;
using Susy::Tau;
using Susy::Particle;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Tau::Tau(const Tau &rhs):
  //Lepton(rhs),
  Particle(rhs),
  q(rhs.q),
  nTrack(rhs.nTrack),
  loose(rhs.loose),
  medium(rhs.medium),
  tight(rhs.tight),
  isTruthMatched(rhs.isTruthMatched),
  truthPdgId(rhs.truthPdgId),
  truthNProngs(rhs.truthNProngs),
  truthCharge(rhs.truthCharge),
  isHadronicTau(rhs.isHadronicTau),
  truthType(rhs.truthType),
  truthOrigin(rhs.truthOrigin),
  looseEffSF(rhs.looseEffSF),
  errLooseEffSF(rhs.errLooseEffSF),
  mediumEffSF(rhs.mediumEffSF),
  errMediumEffSF(rhs.errMediumEffSF),
  tightEffSF(rhs.tightEffSF),
  errTightEffSF(rhs.errTightEffSF),
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
    Particle::operator=(rhs);
    q = rhs.q;
    nTrack = rhs.nTrack;
    loose = rhs.loose;
    medium = rhs.medium;
    tight = rhs.tight;
    isTruthMatched = rhs.isTruthMatched;
    truthPdgId = rhs.truthPdgId;
    truthNProngs = rhs.truthNProngs;
    truthCharge = rhs.truthCharge;
    isHadronicTau = rhs.isHadronicTau;
    truthType = rhs.truthType;
    truthOrigin = rhs.truthOrigin;
    looseEffSF = rhs.looseEffSF;
    errLooseEffSF = rhs.errLooseEffSF;
    mediumEffSF = rhs.mediumEffSF;
    errMediumEffSF = rhs.errMediumEffSF;
    tightEffSF = rhs.tightEffSF;
    errTightEffSF = rhs.errTightEffSF;
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
       << " truthMatched " << isTruthMatched
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}
