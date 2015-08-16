#include "SusyNtuple/Particle.h"

using namespace std;
using Susy::Particle;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Particle::Particle(const Particle &rhs):
  TLorentzVector(rhs),
  pt(rhs.pt),
  eta(rhs.eta),
  phi(rhs.phi),
  m(rhs.m)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Particle& Particle::operator=(const Particle &rhs)
{
  if (this != &rhs) {
    TLorentzVector::operator=(rhs);
    pt = rhs.pt; 
    eta = rhs.eta; 
    phi = rhs.phi; 
    m = rhs.m; 
  }
  return *this;
}

float Particle::DeltaRy(const Particle &rhs) const
{

    float thisRap = this->Rapidity();
    float thisPhi = this->Phi();
    float rhsRap = rhs.Rapidity();
    float rhsPhi = rhs.Phi();

    float dphi = TVector2::Phi_mpi_pi(thisPhi-rhsPhi);
    float drap = thisRap-rhsRap;
    return TMath::Sqrt(dphi*dphi + drap*drap);
}
