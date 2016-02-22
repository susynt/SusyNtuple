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

    /// Author information
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EGammaIdentificationRun2#Photon_authors
    int author;
    bool authorPhoton;           ///< photon reconstructed exclusively as a photon (author == 4)
    bool authorAmbiguous;        ///< photon reconstructed both as electron and photon (author == 16)

    /// Conversion Information
    bool isConv;

    //AT 2014-10-29: new v2
    bool  tight;
    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    bool  OQ;                 ///< GoodOQ

    // Isolation flags
    bool isoFixedCutTightCaloOnly; ///< FixedCutTightCaloOnly WP
    bool isoFixedCutTight;         ///< FixedCutTight WP
    bool isoFixedCutLoose;         ///< FixedCutLoose WP

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
      author = 0;
      authorPhoton = authorAmbiguous = false;
      isConv = false;
      tight = false;
      clusE = clusEta = clusPhi = 0;
      OQ = false;
      isoFixedCutTightCaloOnly = isoFixedCutTight = isoFixedCutLoose = false;
      topoEtcone40=0;
      Particle::clear();
    };

    ClassDef(Photon, 6);
};
} // Susy
#endif
