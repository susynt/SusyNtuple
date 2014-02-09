#ifndef SusyNtuple_TrilTrigLogic_h
#define SusyNtuple_TrilTrigLogic_h

#include "TFile.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNt.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"


/*

    TrilTrigLogic - class to implement the multilepton trigger logic

*/

//using namespace Susy;

class TrilTrigLogic
{

  public:

    TrilTrigLogic();
    ~TrilTrigLogic();

    // Load the reweighters with maps from input files
    void loadTriggerMaps();
    APReweightND* loadTrigWeighter(TFile* f, TString chain);

    // Trigger cut without matching
    bool passEventTrigger(const Susy::Event* evt);

    // Trigger matching in data (and MC at the moment)
    //bool passTriggerMatching(const LeptonVector& leptons, Event* evt);
    bool passTriggerMatching(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, 
                             bool useDilepTrigs=true);
    bool matchLepTrigger(const Susy::Lepton* lep, int trigMask, float ptMin, float etaMax, 
                         bool accOnly=false);
    bool matchTauTrigger(const Susy::Tau* tau, int trigMask, float ptMin, bool accOnly=false);

    //bool matchLepTrigger(const Susy::Lepton* lep, int trigMask, float ptThreshold=0, 
    //                     bool accOnly=false) { 
    //  return lep->Pt() > ptThreshold && (accOnly || lep->matchTrig(trigMask)); 
    //}
    //bool matchTauTrigger(const Susy::Tau* tau, int trigMask, float ptThreshold=0, 
    //                     bool accOnly=false){
    //  return tau->Pt() > ptThreshold && (accOnly || tau->matchTrig(trigMask));
    //}

    // Trigger reweighting in MC - not currently setup
    float getTriggerWeight(const LeptonVector& leptons, const Susy::Event* evt);

    // Only apply pt threshold cuts for trigger acceptance
    void setAccOnly(bool doIt=true) { m_accOnly = doIt; }

    // Debug info
    void setDebug(bool doIt=true) { m_dbg = doIt; }

  protected:

    bool                m_accOnly;      // Only check trigger kinematic acceptance

    int                 m_dbg;          // Debug flag

};

#endif
