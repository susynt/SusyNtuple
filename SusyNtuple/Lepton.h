// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_LEPTON_H
#define SUSYNTUPLE_LEPTON_H

#include "SusyNtuple/Particle.h"

#include "TBits.h"

namespace Susy
{
/// Lepton class, common to electrons and muons
class Lepton : public Particle
{
public:
    Lepton() :
        trigBits(m_nTriggerBits)
        {
            clear();
        }
    virtual ~Lepton(){};
    /** Copy constructor */
    Lepton(const Lepton &);
    /** Assignment operator */
    Lepton& operator=(const Lepton &);

    // public member vars
    int q;                    ///< Charge
    bool isBaseline; ///< flag computed by SUSYTools_xAOD
    bool isSignal; ///< flag computed by SUSYTools_xAOD

    //Iso var -- same fer ele & mu
    //Etcone to be drop later in run-2
    float etconetopo20;       ///< etconetopo iso
    float etconetopo30;       ///< etconetopo iso
    float ptcone20;           ///< ptcone iso
    float ptcone30;           ///< ptcone iso
    float ptvarcone20;        ///< ptvarcone20 iso
    float ptvarcone30;        ///< ptvarcone30 iso


    float d0;                 ///< d0 extrapolated to PV
    float errD0;              ///< Uncertainty on d0
    float d0sigBSCorr;        ///< beam spot corrected d0Sig
    float z0;                 ///< z0 extrapolated to PV
    float errZ0;              ///< Uncertainty on z0

    unsigned int mcType;      ///< MCTruthClassifier particle type
    unsigned int mcOrigin;    ///< MCTruthClassifier particle origin

    bool matched2TruthLepton; ///< flag from RecoTruthMatch::Matched2TruthLepton
    int truthType;            ///< RecoTruthMatch::type

    float effSF;              ///< Efficiency scale factor  (for electron from LLH)
    float errEffSF;           ///< Uncertainty on the efficiency scale factor (for electron from LLH)

    TBits   trigBits;         ///< TBits to store matched trigger chains
    static const size_t m_nTriggerBits=64;

    /// Methods to return impact parameter variables
    /** Note that these are not absolute valued! */
    float d0Sig() const {
      return d0/errD0;
    }
    float z0SinTheta() const {
      return z0 * sin(Theta());
    }

    // Polymorphism, baby!!
    virtual bool isEle() const { return false; }
    virtual bool isMu()  const { return false; }
    void setState(int sys){ resetTLV(); }

    /// Print method
    virtual void print() const {};

    /// Clear vars
    void clear(){
        q = 0;
        etconetopo20 = etconetopo30
            = ptcone20 = ptcone30 
            = ptvarcone20 = ptvarcone30 = 0;
        d0 = errD0 = d0sigBSCorr = z0 = errZ0 = 0;
        mcType = mcOrigin = 0;
        matched2TruthLepton = false;
        truthType = -1;
        effSF = 1;
        errEffSF = 0;
        isBaseline = isSignal = false;
        Particle::clear();
    }

    ClassDef(Lepton, 18);
};
} // Susy
#endif
