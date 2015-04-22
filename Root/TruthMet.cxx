#include "SusyNtuple/TruthMet.h"

#include <iostream>
#include <iomanip>

using namespace std;
using Susy::TruthMet;

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
