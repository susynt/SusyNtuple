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

