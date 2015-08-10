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
  etconetopo20(rhs.etconetopo20),
  etconetopo30(rhs.etconetopo30),
  ptcone20(rhs.ptcone20),
  ptcone30(rhs.ptcone30),
  ptvarcone20(rhs.ptvarcone20),
  ptvarcone30(rhs.ptvarcone30),
  isoGradientLoose(rhs.isoGradientLoose),
  isoGradient(rhs.isoGradient),
  isoLooseTrackOnly(rhs.isoLooseTrackOnly),
  isoLoose(rhs.isoLoose),
  isoTight(rhs.isoTight),
  d0(rhs.d0),
  errD0(rhs.errD0),
  d0sigBSCorr(rhs.d0sigBSCorr),
  z0(rhs.z0),
  errZ0(rhs.errZ0),
  mcType(rhs.mcType),
  mcOrigin(rhs.mcOrigin),
  matched2TruthLepton(rhs.matched2TruthLepton),
  //truthMatchType(rhs.truthMatchType),
  truthType(rhs.truthType),
  effSF(rhs.effSF),
  //errEffSF(rhs.errEffSF),
  trigBits(rhs.trigBits)
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
    etconetopo20 = rhs.etconetopo20;
    etconetopo30 = rhs.etconetopo30;
    ptcone20 = rhs.ptcone20;
    ptcone30 = rhs.ptcone30;
    ptvarcone20  = rhs.ptvarcone20;
    ptvarcone30  = rhs.ptvarcone30;
    isoGradientLoose = rhs.isoGradientLoose;
    isoGradient = rhs.isoGradient;
    isoLooseTrackOnly = rhs.isoLooseTrackOnly;
    isoLoose = rhs.isoLoose;
    isoTight = rhs.isoTight;
    d0 = rhs.d0;
    errD0 = rhs.errD0;
    d0sigBSCorr = rhs.d0sigBSCorr;
    z0 = rhs.z0;
    errZ0 = rhs.errZ0;
    mcType = rhs.mcType;
    mcOrigin = rhs.mcOrigin;
    matched2TruthLepton = rhs.matched2TruthLepton;
    //truthMatchType = rhs.truthMatchType;
    truthType = rhs.truthType;
    effSF = rhs.effSF;
    //errEffSF = rhs.errEffSF;
    trigBits = rhs.trigBits;
  }
  return *this;
}
