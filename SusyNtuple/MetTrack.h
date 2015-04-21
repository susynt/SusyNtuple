// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_METTRACK_H
#define SUSYNTUPLE_METTRACK_H

#include "TObject.h"
#include "TLorentzVector.h"

namespace Susy
{
class MetTrack : public TObject
{
public:
    MetTrack() { clear(); }
    virtual ~MetTrack() {};
    /** Copy constructor */
    MetTrack(const MetTrack &);
    /** Assignment operator */
    MetTrack& operator=(const MetTrack &);
    
    float Et;
    float phi;
    float sumet;

    /// Build a TLorentzVector on the fly
    const TLorentzVector lv() const
        {
            TLorentzVector tlv;
            tlv.SetPtEtaPhiE(this->Et, 0, this->phi, this->Et);
            return tlv;
        }

    // I'm not a big fan of the way this is setup, would like to improve it someday
    int sys;                  ///< MetTrack stored in vector for each sys shift, this identifies MetTrack for specific shift

    /// print vars
    void print() const;

    /// Clear vars
    void clear()
        {
            Et = phi = sumet = 0;
            sys = 0;
        }

    ClassDef(MetTrack, 1);
};
} // Susy
#endif
