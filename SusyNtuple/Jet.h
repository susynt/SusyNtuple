// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_JET_H
#define SUSYNTUPLE_JET_H

#include "SusyNtuple/Particle.h"
#include "SusyNtuple/SusyNtSys.h"

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
    int nTracks;              ///< Number of tracks associated with jet

    // btagging
    bool bjet;                ///< Is b-jet a la SUSYTools (70% w.p.)
    float effscalefact;       ///< B-tag SF a la SUSYTools (70% w.p.)  
    float mv1;                ///< MV1 btag weight
    float mv2c20;             ///< MV2c20 btag weight
    float sv1plusip3d;        ///< SV1plusIP3D btag weight

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
    float jer;                       ///< jet energy resolution
    std::vector<float> groupedNP;    ///< Reduced 3*2 JES systematics
    std::vector<float> FTSys; ///< Flavor Tagger syst: B(10*2), C(4*2), light(12*2) jet systematics

    //ADD SYS!!! 18x2 + JER
    /*
    std::vector<float> bjes;
    std::vector<float> effNp;
    std::vector<float> etaInter;
    std::vector<float> flavor;
    std::vector<float> pileup;
    std::vector<float> punchThrough;
    std::vector<float> singlePart;
    //std::vector<float> relativeNC;
    */

    // Shift energy for systematic
    void setState(int sys);

    // Return flavor tag systematics
    float getFTSys(Susy::NtSys::SusyNtSys sys);
    void  setFTSys(Susy::NtSys::SusyNtSys sys, double scale);

    // Print method
    void print() const;

    // Clear vars
    void clear(){
      jvf = jvt = truthLabel = nTracks = 0;
      matchTruth = false;
      bjet = false;
      effscalefact = 0.;
      detEta = 0;
      emfrac = 0;
      mv1 = sv1plusip3d = mv2c20 = 0;
      isBadVeryLoose = isHotTile = false;
      bch_corr_jet = bch_corr_cell = 0;
      isBadMediumBCH = isBadMediumBCH_up = isBadMediumBCH_dn = isBadTightBCH = false;

      jer = 0;
      groupedNP.resize(3*2,0.0);
      FTSys.resize(26*2,0.0);

      /*
      bjes.resize(2,0.0);
      effNp.resize(6*2,0.0);
      etaInter.resize(2*2,0.0);
      flavor.resize(2*2,0.0);
      pileup.resize(4*2,0.0);
      punchThrough.resize(2,0.0);
      //relativeNC.resize(2,0.0);
      singlePart.resize(2,0.0);
      */
     

      Particle::clear();
    }

    ClassDef(Jet, 18);
};
} // Susy
#endif
