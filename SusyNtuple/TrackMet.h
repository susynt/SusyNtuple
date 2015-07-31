// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_TRACKMET_H
#define SUSYNTUPLE_TRACKMET_H

#include "TObject.h"
#include "TLorentzVector.h"

namespace Susy
{
class TrackMet : public TObject
{
public:
    TrackMet() { clear(); }
    virtual ~TrackMet() {};
    /** Copy constructor */
    TrackMet(const TrackMet &);
    /** Assignment operator */
    TrackMet& operator=(const TrackMet &);
    
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
    int sys;                  ///< TrackMet stored in vector for each sys shift, this identifies TrackMet for specific shift

    /// print vars
    void print() const;

    /// Clear vars
    void clear()
        {
            Et = phi = sumet = 0;
            sys = 0;
        }

    ClassDef(TrackMet, 1);
};
} // Susy
#endif
