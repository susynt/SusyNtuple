#include "SusyNtuple/Photon.h"

using Susy::Photon;
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Photon::Photon(const Photon &rhs):
  Particle(rhs),
  author(rhs.author),
  authorPhoton(rhs.authorPhoton),
  authorAmbiguous(rhs.authorAmbiguous),
  isConv(rhs.isConv),
  loose(rhs.loose),
  tight(rhs.tight),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
  clusPhi(rhs.clusPhi),
  clusEtaBE(rhs.clusEtaBE),
  clusPhiBE(rhs.clusPhiBE),
  OQ(rhs.OQ),
  isoFixedCutTightCaloOnly(rhs.isoFixedCutTightCaloOnly),
  isoFixedCutTight(rhs.isoFixedCutTight),
  isoFixedCutLoose(rhs.isoFixedCutLoose),
  topoEtcone40(rhs.topoEtcone40)
{
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Photon& Photon::operator=(const Photon &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    author = rhs.author;
    authorPhoton = rhs.authorPhoton;
    authorAmbiguous = rhs.authorAmbiguous;
    isConv  = rhs.isConv; 
    loose = rhs.loose;
    tight = rhs.tight;
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
    clusPhi = rhs.clusPhi;
    clusEtaBE = rhs.clusEtaBE;
    clusPhiBE = rhs.clusPhiBE;
    OQ = rhs.OQ;
    isoFixedCutTightCaloOnly = rhs.isoFixedCutTightCaloOnly;
    isoFixedCutTight = rhs.isoFixedCutTight;
    isoFixedCutLoose = rhs.isoFixedCutLoose;
    topoEtcone40 = rhs.topoEtcone40;
  }
  return *this;
}

