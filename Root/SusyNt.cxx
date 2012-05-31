#include <iostream>
#include <iomanip>
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Electron print
/*--------------------------------------------------------------------------------*/
void Electron::print() const
{
  cout.precision(2);
  cout << fixed << "El : q " << setw(2) << q << " pt " << setw(6) << Pt() << " eta " << setw(5) << Eta() 
       << " phi " << setw(5) << Phi()
       << " tight " << tightPP << " type " 
       << mcType << " origin " << mcOrigin << " index " << idx 
       << " ees_up " << ees_up << " ees_dn " << ees_dn << " eer_up " << eer_up << " eer_dn " << eer_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
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
       << " ms_up " << ms_up << " ms_dn " << ms_dn << " id_up " << id_up << " id_dn " << id_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
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
       << " btag " << setw(5) << combNN
       << " jer " << jer << " jes_up " << jes_up << " jes_dn " << jes_dn
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
  cout << "Run " << run << " Event " << event << " Stream " << streamName(stream) << endl;
}
/*--------------------------------------------------------------------------------*/
// Electron Set State
/*--------------------------------------------------------------------------------*/
void Electron::setState(int sys)
{

  resetTLV();
  if(sys == NOM) return;
  
  float sf = 0;
  if( sys == EES_UP )      sf = ees_up;
  else if( sys == EES_DN ) sf = ees_dn;
  else if( sys == EER_UP ) sf = eer_up;
  else if( sys == EER_DN ) sf = eer_dn;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());

}
/*--------------------------------------------------------------------------------*/
// Muon Set State
/*--------------------------------------------------------------------------------*/
void Muon::setState(int sys)
{

  resetTLV();
  if(sys == NOM) return;
  
  float sf = 0;
  if( sys == MS_UP )      sf = ms_up;
  else if( sys == MS_DN ) sf = ms_dn;
  else if( sys == ID_UP ) sf = id_up;
  else if( sys == ID_DN ) sf = id_dn;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());

}

/*--------------------------------------------------------------------------------*/
// Jet Set State
/*--------------------------------------------------------------------------------*/
void Jet::setState(int sys)
{
  resetTLV();
  if(sys == NOM) return;
  
  float sf = 0;
  if( sys == JER )         sf = jer;
  else if( sys == JES_UP ) sf = jes_up;
  else if( sys == JES_DN ) sf = jes_dn;
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());

}

