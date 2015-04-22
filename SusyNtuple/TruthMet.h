// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TRUTHMET_H
#define SUSYNTUPLE_TRUTHMET_H

#include "TObject.h"
#include "TLorentzVector.h"

namespace Susy
{
/// TruthMet class
class TruthMet : public TObject
{
public:
    TruthMet(){ clear(); }
    virtual ~TruthMet(){};
    /** Copy constructor */
    TruthMet(const TruthMet &);
    /** Assignment operator */
    TruthMet& operator=(const TruthMet &);

    float Et;
    float phi;

    /// Build a TLorentzVector on the fly
    const TLorentzVector lv() const {
      TLorentzVector tlv;
      tlv.SetPtEtaPhiE(this->Et,0,this->phi,this->Et);
      return tlv;
    }

    // print vars
    void print() const;

    // Clear vars
    void clear(){
      Et  = 0;
      phi = 0;
    }

    ClassDef(TruthMet, 1);
};
} // Susy
#endif
