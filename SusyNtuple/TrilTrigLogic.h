#ifndef SusyNtuple_TrilTrigLogic_h
#define SusyNtuple_TrilTrigLogic_h

#include "TFile.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"


/*

    TrilTrigLogic - class to implement the god awful multilepton trigger logic

*/

class TrilTrigLogic
{

  public:

    TrilTrigLogic(Susy::SusyNtObject* nt);
    ~TrilTrigLogic();

    // Load the reweighters with maps from input files
    void loadTriggerMaps();
    APReweightND* loadTrigWeighter(TFile* f, TString chain);

    // Plateau flags tell which plateaus are satisfied
    void plateauFlags(const LeptonVector& leptons, bool& plat1E, bool& plat2E, bool& plat1M, bool& plat2M, bool& platEM);

    // Trigger matching in data
    uint getTrigger1E(uint run);
    uint getTrigger2E(uint run);
    uint getTrigger1M(uint run);
    uint getTrigger2M(uint run);
    uint getTriggerEM(uint run);
    bool passTriggerMatching(const LeptonVector& leptons);

    // Trigger reweighting in MC
    float getTriggerWeight(const LeptonVector& leptons);

    // Electron trigger efficiency requires a bin numbers (WTF??)
    double getEleEtaBin(float clusEta);

    // Debug info
    void setDebug(bool doIt=true) { m_dbg = doIt; }

    // Public reweighting tools, for now
    APReweightND*       rew_mu18;
    APReweightND*       rew_mu18Med;
    APReweightND*       rew_mu10L_not18;
    APReweightND*       rew_mu10L_not18Med;
    APReweightND*       rew_mu6_not18;
    APReweightND*       rew_mu6_not18Med;

    APReweightND*       rew_e20;
    APReweightND*       rew_e22;
    APReweightND*       rew_e22vh;
    APReweightND*       rew_e12;
    APReweightND*       rew_e12T;
    APReweightND*       rew_e12Tvh;
    APReweightND*       rew_e10;

  protected:

    Susy::SusyNtObject* m_nt;   // SusyNt interface

    float               m_lumis[Period_Num];
    float               m_lumiIK;

    int                 m_dbg;

};

#endif
