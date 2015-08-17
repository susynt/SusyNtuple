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

/*--------------------------------------------------------------------------------*/
// Rapidity based DeltaR
/*--------------------------------------------------------------------------------*/
double Particle::DeltaRy(const Particle &rhs) const
{
    double delta_phi = TVector2::Phi_mpi_pi(this->Phi() - rhs.Phi());
    double delta_rap = this->Rapidity() - rhs.Rapidity();
    return TMath::Sqrt(delta_phi*delta_phi + delta_rap*delta_rap);
}
