#include "SusyNtuple/TruthParticle.h"

#include <iostream>
#include <iomanip>

using namespace std;

using Susy::TruthParticle;
using Susy::Particle;

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
