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

    // MET terms
    float refEle_et;        ///< Ref electron term et
    float refEle_phi;       ///< Ref electron term phi
    float refEle_sumet;     ///< Ref elctron sumet

    float refMuo_et;        ///< Ref muon term et
    float refMuo_phi;       ///< Ref muon term phi
    float refMuo_sumet;     ///< Ref muon term sumet

    float refJet_et;        ///< Ref jet term et
    float refJet_phi;       ///< Ref jet term phi
    float refJet_sumet;     ///< Ref jet term sumet

    float softTerm_et;      ///< Softterm et
    float softTerm_phi;     ///< Softterm phi
    float softTerm_sumet;   ///< Softterm sumet

    // I'm not a big fan of the way this is setup, would like to improve it someday
    int sys;                  ///< TrackMet stored in vector for each sys shift, this identifies TrackMet for specific shift

    /// print vars
    void print() const;

    /// Clear vars
    void clear()
        {
            Et = phi = sumet = 0.;
            // ele term
            refEle_et = refEle_phi = refEle_sumet = 0.;
            // muon term
            refMuo_et = refMuo_phi = refMuo_sumet = 0.;
            // jet term
            refJet_et = refJet_phi = refJet_sumet = 0.;
            // soft term
            softTerm_et = softTerm_phi = softTerm_sumet = 0.;

            sys = 0;
        }

    ClassDef(TrackMet, 2);
};
} // Susy
#endif
