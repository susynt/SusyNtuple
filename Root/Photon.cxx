#include "SusyNtuple/Photon.h"

using Susy::Photon;
/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Photon::Photon(const Photon &rhs):
  Particle(rhs),
  isConv(rhs.isConv),
  tight(rhs.tight),
  clusE(rhs.clusE),
  clusEta(rhs.clusEta),
  clusPhi(rhs.clusPhi),
  OQ(rhs.OQ),
  isoCone40CaloOnly(rhs.isoCone40CaloOnly),
  isoCone40(rhs.isoCone40),
  isoCone20(rhs.isoCone20),
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
    isConv  = rhs.isConv; 
    tight = rhs.tight;
    clusE = rhs.clusE;
    clusEta = rhs.clusEta;
    clusPhi = rhs.clusPhi;
    OQ = rhs.OQ;
    isoCone40CaloOnly = rhs.isoCone40CaloOnly;
    isoCone40 = rhs.isoCone40;
    isoCone20 = rhs.isoCone20;
    topoEtcone40 = rhs.topoEtcone40;
  }
  return *this;
}

