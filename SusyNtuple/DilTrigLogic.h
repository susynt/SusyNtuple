#ifndef DilTrigLogic_h
#define DilTrigLogic_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// This class will implement the dilepton trigger logic based  //
// on the leptons, run number and stream                       //
// The user will need to call one basic method:                //
// passDilTrig(Electrons,Muons,RunNumber,Stream)               //
// and it will return pass or fail based on kinematics         //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "TFile.h"
#include "THnSparse.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNt.h"
#include "SusyDefs.h"

using namespace Susy;
using namespace std;


enum DilTriggerRegion
{
  DTR_EE_A = 0,
  DTR_EE_B,
  DTR_MM_A,
  DTR_MM_B,
  DTR_MM_C,
  DTR_MM_D,
  DTR_EM_A,
  DTR_EM_B,
  DTR_UNKNOWN,
  DTR_N
};

class DilTrigLogic
{

 public:

  // Default constructor and destructor for now
  DilTrigLogic(bool isMC);
  virtual ~DilTrigLogic();

  // Three basic methods to interact with the trigger package:
  // 1.) return true if event and objects match to correct trigger.
  // 2.) return true if correct event trigger fired
  // 3.) return true if objects match to correct trigger
  bool passDilTrig(LeptonVector leptons, Event* evt);
  bool passDilEvtTrig(LeptonVector leptons, Event* evt);
  bool passDilTrigMatch(LeptonVector leptons, Event* evt);

  // Regions taken from this talk: 
  // https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=199022
  // pt0 = leading Pt, pt1 = subleading Pt.
  DilTriggerRegion getEETrigRegion(float pt0, float pt1);
  DilTriggerRegion getMMTrigRegion(float pt0, float pt1);
  DilTriggerRegion getEMTrigRegion(float ept, float mpt);

  // Methods to check Evt Trigger and trigger matching
  bool passEvtTrigger(uint evtflag, DilTriggerRegion dtr);
  bool passTriggerMatch(uint flag0, uint flag1, DilTriggerRegion dtr);

  // MC will need to be reweighted
  float getMCWeight();

  // Trigger reweighting
  //void loadTriggerMaps();
  //APReweightND* loadTrigWeighter(TFile* f, TString chain);
  //APReweightND* getEleTrigWeighter(uint trigFlag);
  //APReweightND* getMuoTrigWeighter(uint trigFlag);  
  
  // Debug method
  void debugFlag(uint flag);

 protected:

  //    
  // Triger reweighting    
  //                          

  // I first thought I could use a map of chain enum -> reweighter.
  // It's clear now that it won't work because a chain may need
  // more than one map depending on the period.  That's because   
  // other chains may change so the conditional efficiencies change. 

  std::map<int, APReweightND*>        m_elTrigWeightMap;
  std::map<int, APReweightND*>        m_muTrigWeightMap;

  // Instead, have one pointer for each reweighter.    
  APReweightND*       m_trigTool_mu18;
  APReweightND*       m_trigTool_mu18Med;
  APReweightND*       m_trigTool_mu10L_not18;
  APReweightND*       m_trigTool_mu10L_not18Med;
  APReweightND*       m_trigTool_mu6_not18;
  APReweightND*       m_trigTool_mu6_not18Med;

  APReweightND*       m_trigTool_e20;
  APReweightND*       m_trigTool_e22;
  APReweightND*       m_trigTool_e22vh;
  APReweightND*       m_trigTool_e12;
  APReweightND*       m_trigTool_e12T;
  APReweightND*       m_trigTool_e12Tvh;
  APReweightND*       m_trigTool_e10;

 private:
   
  //Period              period;             // Period
  //ElectronVector      m_elecs;              // Electron vector for easy access
  //MuonVector          m_muons;              // Muon vector for easy access

};

#endif
