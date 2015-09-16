// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TRUTHPARTICLE_H
#define SUSYNTUPLE_TRUTHPARTICLE_H

#include "SusyNtuple/Particle.h"

namespace Susy
{
/// TruthParticle
class TruthParticle : public Particle
{
public:
    TruthParticle(){ clear(); }
    virtual ~TruthParticle(){};
    TruthParticle(const TruthParticle &);
    /** Assignment operator */
    TruthParticle& operator=(const TruthParticle &);

    int pdgId;
    int charge;
    int status;
    int motherPdgId;

    // Print method
    void print() const;

    void clear(){
      pdgId  = 0;
      charge = 0;
      status = 0;
      motherPdgId = 0;
      Particle::clear();
    }

    ClassDef(TruthParticle,1);
};
} // Susy
#endif
