// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_PHOTON_H
#define SUSYNTUPLE_PHOTON_H

#include "SusyNtuple/Particle.h"

namespace Susy
{
/// Photon class
class Photon : public Particle
{
public:
    Photon() { clear(); }
    virtual ~Photon(){};
    Photon(const Photon &);
    /** Assignment operator */
    Photon& operator=(const Photon &);

    /// Conversion Information
    bool isConv;

    //AT 2014-10-29: new v2
    bool  tight;
    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    bool  OQ;                 ///< GoodOQ

    float topoEtcone40;

    // Systematics - not current supported??
    //float pes_up;        // Photon Energy Scale up
    //float pes_dn;        // Photon Energy Scale down
    //float per_up;        // Photon Energy Resolution up
    //float per_dn;        // Photon Energy Resolution down
    //void setState(int sys){ resetTLV();};

    /// Print method
    void print() const {};

    /// Clear
    void clear(){
      //pes_up = pes_dn = per_up = per_dn = 0;
      isConv = false;
      tight = false;
      clusE = clusEta = clusPhi = 0;
      OQ = false;
      topoEtcone40=0;
      Particle::clear();
    };

    ClassDef(Photon, 2);
};
} // Susy
#endif
