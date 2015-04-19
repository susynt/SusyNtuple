// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_JET_H
#define SUSYNTUPLE_JET_H

#include "SusyNtuple/Particle.h"

namespace Susy
{
/// Jet class
class Jet : public Particle
{
public:
    Jet(){ clear(); }
    virtual ~Jet(){};
    Jet(const Jet &);
    /** Assignment operator */
    Jet& operator=(const Jet &);

    float jvf;                ///< Jet vertex fraction
    float jvt;                ///< Jet vertex tagger
    float detEta;             ///< Detector eta
    float emfrac;             ///< EM fraction
    int truthLabel;           ///< Flavor truth label
    bool matchTruth;          ///< Matches truth jet

    // btagging
    float sv0;                ///< SV0 btag weight
    float combNN;             ///< JetFitterCombNN btag weight
    float mv1;                ///< MV1 btag weight
    float sv1plusip3d;        ///< SV1plusIP3D btag weight
    float jfit_mass;          ///< secondary vtx mass (dev DG Aug13, will remove when done)
    float sv0p_mass;          ///< ???
    float svp_mass;           ///< ???

    // Flags/variables for cleaning
    bool isBadVeryLoose;      ///< bad jet flag computed with SUSYTools
    bool isHotTile;           ///< tile hot spot flag computed with SUSYTools
    float bch_corr_jet;       ///< Needed for dead region veto
    float bch_corr_cell;      ///< For studying BCH cleaning prescription

    bool isBadMediumBCH;      ///< BCH cleaning flag, https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BCHCleaningTool
    bool isBadMediumBCH_up;   ///< BCH cleaning flag
    bool isBadMediumBCH_dn;   ///< BCH cleaning flag
    bool isBadTightBCH;       ///< BCH cleaning flag

    // Systematics
    float jes_up;             ///< jet energy scale up  //AT obsolete?
    float jes_dn;             ///< jet energy scale down//AT obsolete?
    float jer;                ///< jet energy resolution

    //ADD SYS!!! 18x2 + JER
    std::vector<float> bjes;
    std::vector<float> effNp;
    std::vector<float> etaInter;
    std::vector<float> flavor;
    std::vector<float> pileup;
    std::vector<float> punchThrough;
    std::vector<float> relativeNC;
    std::vector<float> singlePart;

    // Jet-Met Weights
    float met_wpx;
    float met_wpy;

    // Shift energy for systematic
    void setState(int sys);

    // Print method
    void print() const;

    // Clear vars
    void clear(){
      jvf = jvt = truthLabel = 0;
      matchTruth = false;
      detEta = 0;
      emfrac = 0;
      sv0 = combNN = mv1 = sv1plusip3d = 0;
      jfit_mass = sv0p_mass = svp_mass = 0;
      isBadVeryLoose = isHotTile = false;
      bch_corr_jet = bch_corr_cell = 0;
      isBadMediumBCH = isBadMediumBCH_up = isBadMediumBCH_dn = isBadTightBCH = false;
      jer = jes_up = jes_dn = 0;
      met_wpx = met_wpy = 0;

      bjes.resize(2,0.0);
      effNp.resize(6*2,0.0);
      etaInter.resize(2*2,0.0);
      flavor.resize(2*2,0.0);
      pileup.resize(4*2,0.0);
      punchThrough.resize(2,0.0);
      relativeNC.resize(2,0.0);
      singlePart.resize(2,0.0);

      Particle::clear();
    }

    ClassDef(Jet, 13);
};
} // Susy
#endif
