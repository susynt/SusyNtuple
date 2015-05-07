#include "SusyNtuple/Met.h"

#include <iostream>
#include <iomanip>

using namespace std;

using Susy::Met;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Met::Met(const Met &rhs):
  TObject(rhs),
  Et(rhs.Et),
  phi(rhs.phi),
  sumet(rhs.sumet),
  refEle_et(rhs.refEle_et),
  refEle_phi(rhs.refEle_phi),
  refEle_sumet(rhs.refEle_sumet),
  refGamma_et(rhs.refGamma_et),
  refGamma_phi(rhs.refGamma_phi),
  refGamma_sumet(rhs.refGamma_sumet),
  refTau_et(rhs.refTau_et),
  refTau_phi(rhs.refTau_phi),
  refTau_sumet(rhs.refTau_sumet),
  refJet_et(rhs.refJet_et),
  refJet_phi(rhs.refJet_phi),
  refJet_sumet(rhs.refJet_sumet),
  softTerm_et(rhs.softTerm_et),
  softTerm_phi(rhs.softTerm_phi),
  softTerm_sumet(rhs.softTerm_sumet),
  refMuo_et(rhs.refMuo_et),
  refMuo_phi(rhs.refMuo_phi),
  refMuo_sumet(rhs.refMuo_sumet),
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
    refEle_et = rhs.refEle_et; 
    refEle_phi = rhs.refEle_phi; 
    refEle_sumet = rhs.refEle_sumet; 
    refGamma_et = rhs.refGamma_et; 
    refGamma_phi = rhs.refGamma_phi; 
    refGamma_sumet = rhs.refGamma_sumet;
    refTau_et = rhs.refTau_et; 
    refTau_phi = rhs.refTau_phi; 
    refTau_sumet = rhs.refTau_sumet; 
    refJet_et = rhs.refJet_et; 
    refJet_phi = rhs.refJet_phi; 
    refJet_sumet = rhs.refJet_sumet; 
    softTerm_et = rhs.softTerm_et;
    softTerm_phi = rhs.softTerm_phi;
    softTerm_sumet = rhs.softTerm_sumet;
    refMuo_et = rhs.refMuo_et; 
    refMuo_phi = rhs.refMuo_phi; 
    refMuo_sumet = rhs.refMuo_sumet; 
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

