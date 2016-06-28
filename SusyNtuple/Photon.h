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

    /// Photon Selector IsEM flags
    bool loose;               ///< AsgPhotonIsEMSelector "Loose" WP flag
    bool tight;               ///< AsgPhotonIsEMSelector "Tight" WP flag

    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    float clusEtaBE;          ///< CaloCluster eta (2nd sampling) EMB/EMEC combined barycenter eta
    float clusPhiBE;          ///< CaloCluster phi (2nd sampling) EMB/EMEC combined barycenter phi 
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
      loose = tight = false;
      clusE = clusEta = clusPhi = clusEtaBE = clusPhiBE = 0;
      OQ = false;
      isoFixedCutTightCaloOnly = isoFixedCutTight = isoFixedCutLoose = false;
      topoEtcone40=0;
      Particle::clear();
    };

    ClassDef(Photon, 7);
};
} // Susy
#endif
