#include "SusyNtuple/Lepton.h"

using Susy::Lepton;
using Susy::Particle;
 
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Lepton::Lepton(const Lepton &rhs):
  Particle(rhs),
  q(rhs.q),
  isBaseline(rhs.isBaseline),
  isSignal(rhs.isSignal),
  etcone20(rhs.etcone20),
  ptcone20(rhs.ptcone20),
  ptcone30(rhs.ptcone30),
  d0(rhs.d0),
  errD0(rhs.errD0),
  z0(rhs.z0),
  errZ0(rhs.errZ0),
  mcType(rhs.mcType),
  mcOrigin(rhs.mcOrigin),
  matched2TruthLepton(rhs.matched2TruthLepton),
  //truthMatchType(rhs.truthMatchType),
  truthType(rhs.truthType),
  effSF(rhs.effSF),
  errEffSF(rhs.errEffSF),
  trigFlags(rhs.trigFlags)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Lepton& Lepton::operator=(const Lepton &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    q = rhs.q; 
    isBaseline = rhs.isBaseline;
    isSignal = rhs.isSignal;
    etcone20 = rhs.etcone20;
    ptcone20 = rhs.ptcone20;
    ptcone30 = rhs.ptcone30;
    d0 = rhs.d0;
    errD0 = rhs.errD0;
    z0 = rhs.z0;
    errZ0 = rhs.errZ0;
    mcType = rhs.mcType;
    mcOrigin = rhs.mcOrigin;
    matched2TruthLepton = rhs.matched2TruthLepton;
    //truthMatchType = rhs.truthMatchType;
    truthType = rhs.truthType;
    effSF = rhs.effSF;
    errEffSF = rhs.errEffSF;
    trigFlags = rhs.trigFlags;
  }
  return *this;
}
