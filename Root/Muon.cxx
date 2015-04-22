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

