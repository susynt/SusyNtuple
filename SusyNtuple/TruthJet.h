// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TRUTHJET_H
#define SUSYNTUPLE_TRUTHJET_H

#include "SusyNtuple/Particle.h"

namespace Susy
{
/// TruthJet
class TruthJet : public Particle
{
public:
    TruthJet(){ clear(); }
    virtual ~TruthJet(){};
    TruthJet(const TruthJet &);
    /** Assignment operator */
    TruthJet& operator=(const TruthJet &);

    int flavor;

    // Print method
    void print() const;

    void clear(){
      flavor = 0;
      Particle::clear();
    }

    ClassDef(TruthJet,1);
};
} // Susy
#endif
