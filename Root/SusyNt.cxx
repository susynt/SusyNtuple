#include <iostream>
#include <iomanip>
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Electron Set State
/*--------------------------------------------------------------------------------*/
void Electron::setState(int sys)
{
  resetTLV();
  if(sys == NtSys_NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys_EES_UP ) sf = ees_up;
  else if( sys == NtSys_EES_DN ) sf = ees_dn;
  else if( sys == NtSys_EER_UP ) sf = eer_up;
  else if( sys == NtSys_EER_DN ) sf = eer_dn;
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
// Event print
/*--------------------------------------------------------------------------------*/
void Event::print() const
{
  cout << "Run " << run << " Event " << event << " Stream " << streamName(stream) 
       << " w " << w << endl;
}

