// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_MET_H
#define SUSYNTUPLE_MET_H

#include "TObject.h"
#include "TLorentzVector.h"

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
    // TODO: clean out the obsolete term
    float refEle_et;          ///< Ref electron term et
    float refEle_phi;         ///< Ref electron term phi
    float refEle_sumet;       ///< Ref electron sumet
    float refGamma_et;        ///< Ref gamma term et
    float refGamma_phi;       ///< Ref gamma term phi
    float refGamma_sumet;     ///< Ref gamma term sumet
    float refTau_et;          ///< Ref electron term et
    float refTau_phi;         ///< Ref electron term phi
    float refTau_sumet;       ///< Ref electron sumet
    float refJet_et;          ///< Ref jet term et
    float refJet_phi;         ///< Ref jet term phi
    float refJet_sumet;       ///< Ref jet term sumet 
    float softTerm_et;        ///< Softterm term et (cluster base or track based)
    float softTerm_phi;       ///< Softterm term phi
    float softTerm_sumet;     ///< Softterm term sumet
    float refMuo_et;          ///< Ref muon term et
    float refMuo_phi;         ///< Ref muon term phi
    float refMuo_sumet;       ///< Ref muon term sumet
   
    // I'm not a big fan of the way this is setup, would like to improve it someday
    int sys;                  ///< Met stored in vector for each sys shift, 
                              ///<this identifies met for specific shift in SusyNtSys

    /// print vars
    void print() const;

    /// Clear vars
    void clear(){
      Et = sumet = 0;
      refEle_et = refGamma_et = refTau_et = refJet_et = softTerm_et = refMuo_et = 0;
      refEle_sumet = refGamma_sumet = refTau_sumet = refJet_sumet = softTerm_sumet = refMuo_sumet = 0;

      // default of phi = 0 is annoying for met components
      phi = -5.;
      refEle_phi = -5.;
      refMuo_phi = -5.;
      refGamma_phi = -5.;
      refTau_phi = -5.;
      refJet_phi = -5.;
      softTerm_phi = -5.;  


      sys = 0;
    }

    ClassDef(Met, 6);
};
} // Susy
#endif
