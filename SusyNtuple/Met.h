// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_MET_H
#define SUSYNTUPLE_MET_H

#include "TObject.h"

namespace Susy
{
/// Met class
/**
   No longer inherits from Particle or TLorentzVector
*/
class Met : public TObject
{
public:
    Met(){ clear(); }
    virtual ~Met(){};
    /** Copy constructor */
    Met(const Met &);
    /** Assignment operator */
    Met& operator=(const Met &);

    float Et;
    float phi;
    float sumet;

    /// Build a TLorentzVector on the fly
    const TLorentzVector lv() const {
      TLorentzVector tlv;
      tlv.SetPtEtaPhiE(this->Et,0,this->phi,this->Et);
      return tlv;
    }

    // MET Composition info - do we want TLorentzVectors, TVector2, or just floats?
    // TODO: clean out the obsolete terms
    float refEle;             ///< Ref electron term magnitude
    float refEle_etx;         ///< Ref electron term
    float refEle_ety;         ///< Ref electron term
    float refEle_sumet;       ///< Ref electron sumet
    float refMuo;             ///< Ref muon term magnitude
    float refMuo_etx;         ///< Ref muon term
    float refMuo_ety;         ///< Ref muon term
    float refMuo_sumet;       ///< Ref muon sumet
    float refJet;             ///< Ref jet term
    float refJet_etx;         ///< Ref jet term
    float refJet_ety;         ///< Ref jet term
    float refJet_sumet;       ///< Ref jet sumet
    float softJet;            ///< Soft jet term
    float softJet_etx;        ///< Soft jet term
    float softJet_ety;        ///< Soft jet term
    //float softJet_sumet;       // No Need, this term obsolete
    float refGamma;           ///< Ref gamma term
    float refGamma_etx;       ///< Ref gamma term
    float refGamma_ety;       ///< Ref gamma term
    float refGamma_sumet;     ///< Ref gamma term
    float refCell;            ///< Cellout term
    float refCell_etx;        ///< Cellout term
    float refCell_ety;        ///< Cellout term
    //float refCell_sumet;       // No Need, this term obsolete
    float softTerm;           ///< Soft term
    float softTerm_etx;       ///< Soft term
    float softTerm_ety;       ///< Soft term
    float softTerm_sumet;     ///< Soft term sumet

    // I'm not a big fan of the way this is setup, would like to improve it someday
    int sys;                  ///< Met stored in vector for each sys shift, this identifies met for specific shift

    /// print vars
    void print() const;

    /// Clear vars
    void clear(){
      Et = phi = sumet = 0;
      refEle = refMuo = refJet = softJet = refGamma = refCell = softTerm = 0;
      refEle_etx = refMuo_etx = refJet_etx = softJet_etx = refGamma_etx = refCell_etx = softTerm_etx = 0;
      refEle_ety = refMuo_ety = refJet_ety = softJet_ety = refGamma_ety = refCell_ety = softTerm_ety = 0;
      sys = 0;
      refEle_sumet = refMuo_sumet = refJet_sumet = refGamma_sumet = softTerm_sumet = 0;
    }

    ClassDef(Met, 4);
};
} // Susy
#endif
