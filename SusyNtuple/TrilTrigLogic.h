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

using namespace Susy;

class TrilTrigLogic
{

  public:

    TrilTrigLogic();
    ~TrilTrigLogic();

    // Load the reweighters with maps from input files
    void loadTriggerMaps();
    APReweightND* loadTrigWeighter(TFile* f, TString chain);

    // Trigger matching in data (and MC at the moment)
    bool passTriggerMatching(const LeptonVector& leptons, Event* evt);

    // Trigger reweighting in MC
    float getTriggerWeight(const LeptonVector& leptons, Event* evt);

    // Debug info
    void setDebug(bool doIt=true) { m_dbg = doIt; }

  protected:
    int                 m_dbg;

};

#endif
