// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_EVENT_H
#define SUSYNTUPLE_EVENT_H

//SusyNtuple
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/SusyDefs.h" // DataStream
#include "SusyNtuple/TriggerTools.h" // DileptonTrigTuple

#include "TBits.h"
#include "TObject.h"

namespace Susy
{

/// Event class
class Event: public TObject
{
public:
    Event() :
        trigBits(m_nTriggerBits)
        {
            clear();
        }
    virtual ~Event(){};

    unsigned int run;   ///< run number
    unsigned int eventNumber; ///< event number
    unsigned int lb;          ///< lumi block number
    DataStream stream;        ///< DataStream enum, defined in SusyDefs.h
    int treatAsYear;          ///< SUSYObjDef::treatAsYear (equals either 2015 or 2016)

    bool isMC;                ///< is MC flag
    unsigned int mcChannel;   ///< MC channel ID number (mc run number)
    float w;                  ///< MC generator weight
    uint64_t initialNumberOfEvents;  ///< initial number of events processed before any xAOD skimming (before derivation process)
    double sumOfEventWeights;        ///< sum of MC event weights before any xAOD skimming (before derivation process)
    double sumOfEventWeightsSquared; ///< sum of MC event weights squared before any xAOD skimming (before derivation process)

    unsigned int larError;    ///< LAr error flag

    unsigned int nVtx;        ///< number of good vertices
    float avgMu;              ///< average interactions per bunch crossing
    float avgMuDataSF;        ///< average interactions per bunch crossing with Data SF applied

    /// SUSY subprocess ID's: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalUncertainties#Subprocess_IDs
    int susyFinalState;       ///< Susy process
    int hDecay;               ///< Higgs decay (see WhTruthExtractor)
    bool eventWithSusyProp;   ///< Event generated involving SUSY propagators
    ///< (See Serhan's msg http://goo.gl/ucwl9)
    int susySpartId1;         ///< SUSY sparticle 1 pdg ID
    int susySpartId2;         ///< SUSY sparticle 2 pdg ID
    float susy3BodyLeftPol;   ///< Left polarized stop
    float susy3BodyRightPol;  ///< Right polarized stop (70%)
    float susy3BodyOnlyMass;  ///< No new angle, test for just mass - ASM-2016-04-25


    //  sherpa 2.2 V+jets reweighting
    // see: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/CentralMC15ProductionList#NEW_Sherpa_v2_2_V_jets_NJet_rewe
    bool isSherpaVjetsSample;
    float sherpa22VjetsWeight;

    //unsigned int trigFlags; ///< Event level trigger bits
    long long trigFlags;      ///< Event level trigger bits

    TBits               trigBits;
    static const size_t m_nTriggerBits=64;

    // Dilepton trigger matching information
    std::map<DileptonTrigTuple, int> m_dilepton_trigger_matches; 

    /// Check trigger firing
    /** provide the trigger chain via bit mask, e.g. TRIG_mu18 */
    bool passTrig(long long mask, bool requireAll=true) const {
      if(requireAll) return (trigFlags & mask) == mask;
      else return mask == 0 || (trigFlags & mask) != 0;
    }

    // Event Flag to check for LAr, bad jet, etc. List found in SusyDefs.h under EventCheck
    // Use cutFlags instead
    //int evtFlag[NtSys_N];

    /// Event cleaning cut flags. The bits are defined in SusyDefs as EventCleaningCuts
    unsigned int cutFlags[NtSys::SYS_UNKNOWN];

    // Reweighting and scaling
    float wPileup;            ///< pileup weight for full dataset
    float wPileup_up;         ///< pileup weight shifted for systematic
    float wPileup_dn;         ///< pileup weight shifted for systematic
    float xsec;               ///< cross section * kfactor * efficiency, from SUSY db
    float errXsec;            ///< cross section uncertainty
    float sumw;               ///< Sum of generator weights

    /// PDF Systematic information

    /// MC weights
    // would like this to be a `std::map<std::string, float>, but would need something like an updated version of TruthTools
    // e.g. see https://svnweb.cern.ch/trac/atlasoff/browser/Generators/GenAnalysisTools/TruthTools?order=name
    std::vector<float> mcWeights;

    /// print event
    void print() const;

    /// Clear vars
    void clear(){
      run = eventNumber = lb = 0;
      stream = Stream_Unknown;
      treatAsYear = -1;
      isMC = false;
      mcChannel = w = 0;
      initialNumberOfEvents = sumOfEventWeights = sumOfEventWeightsSquared = 0;
      larError = 0;
      nVtx = avgMu = avgMuDataSF = trigFlags = 0;
      susyFinalState = 0;
      hDecay = -1;
      eventWithSusyProp = false;
      susySpartId1 = susySpartId2 = 0;
      susy3BodyLeftPol = susy3BodyRightPol = susy3BodyOnlyMass = 0;
      isSherpaVjetsSample = false;
      sherpa22VjetsWeight = 1;
      //memset(evtFlag,0,sizeof(evtFlag));
      memset(cutFlags,0,sizeof(cutFlags));
      wPileup = wPileup_up = wPileup_dn = 0;
      xsec = errXsec = sumw = 0;
      mcWeights.clear();
    }

    ClassDef(Event, 39);
  };
} // Susy
#endif
