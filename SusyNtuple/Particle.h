// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_PARTICLE_H
#define SUSYNTUPLE_PARTICLE_H

#include "TLorentzVector.h"

namespace Susy
{
/// Particle class, base class for other object types
class Particle : public TLorentzVector
{
public:
    Particle(){ clear(); }
    virtual ~Particle(){};
    /** Copy constructor */
    Particle(const Particle &);
    /** Assignment operator */
    Particle& operator=(const Particle &);

    // Nominal pt, eta, phi, m, by request
    float pt;
    float eta;
    float phi;
    float m;
    void resetTLV(){ this->SetPtEtaPhiM(pt,eta,phi,m); };
    float DeltaRy(const Particle& p1) const;

    /// Systematic-shifted state for particles.
    /** Base class method simply resets */
    void setState(int sys){ resetTLV(); };

    void clear() {
      TLorentzVector::Clear();
      pt = eta = phi = m = 0;
    }

    /// Print method
    virtual void print() const {};

    /// Comparison operators for sorting, etc.
    inline bool operator > (const Particle & other) const {
      return Pt() > other.Pt();
    }
    inline bool operator < (const Particle & other) const {
      return Pt() < other.Pt();
    }

    ClassDef(Particle, 2);
  };
} // Susy
#endif
