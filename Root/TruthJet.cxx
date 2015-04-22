#include "SusyNtuple/TruthJet.h"

#include <iostream>
#include <iomanip>

using namespace std;

using Susy::TruthJet;
using Susy::Particle;

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
