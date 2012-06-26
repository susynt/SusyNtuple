#ifndef SusyNtuple_TrilTrigLogic_h
#define SusyNtuple_TrilTrigLogic_h

#include "TFile.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"


/*

    TrilTrigLogic - class to implement the multilepton trigger logic

*/

class TrilTrigLogic
{

  public:

    TrilTrigLogic(Susy::SusyNtObject* nt);
    ~TrilTrigLogic();

    // Load the reweighters with maps from input files
    void loadTriggerMaps();
    APReweightND* loadTrigWeighter(TFile* f, TString chain);

    // Trigger matching in data (and MC at the moment)
    bool passTriggerMatching(const LeptonVector& leptons);

    // Trigger reweighting in MC
    float getTriggerWeight(const LeptonVector& leptons);

    // Debug info
    void setDebug(bool doIt=true) { m_dbg = doIt; }

  protected:

    Susy::SusyNtObject* m_nt;   // SusyNt interface

    int                 m_dbg;

};

#endif
