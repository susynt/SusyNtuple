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

#include "DGTriggerReweight/triggerReweight2Lep.h"

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
  DTR_MM_E,
  DTR_EM_A,
  DTR_EM_B,
  DTR_UNKNOWN,
  DTR_N
};

class DilTrigLogic
{

 public:

  // Default constructor and destructor for now
  DilTrigLogic(string period = "Moriond", bool useReweightUtils=true);
  virtual ~DilTrigLogic();

  // Three basic methods to interact with the trigger package:
  // 1.) return true if event and objects match to correct trigger.
  // 2.) return true if correct event trigger fired
  // 3.) return true if objects match to correct trigger
  bool passDilTrig(LeptonVector leptons, float met, Event* evt);
  bool passDilEvtTrig(LeptonVector leptons, float met, Event* evt);
  bool passDilTrigMatch(LeptonVector leptons, float met, Event* evt);

  // Regions taken from this talk: 
  // https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=199022
  // pt0 = leading Pt, pt1 = subleading Pt.
  DilTriggerRegion getEETrigRegion(float pt0, float pt1);
  DilTriggerRegion getMMTrigRegion(float pt0, float pt1, float met);
  DilTriggerRegion getEMTrigRegion(float ept, float mpt);

  // Methods to check Evt Trigger and trigger matching
  bool passEvtTrigger(uint evtflag, DilTriggerRegion dtr);
  bool passTriggerMatch(uint flag0, uint flag1, DilTriggerRegion dtr);

  // MC will need to be reweighted
  float getMCWeight();

  // Trigger reweighting
  double getTriggerWeight(LeptonVector leptons, bool isMC, 
			  float met, int njets, int NPV,
			  SusyNtSys sys = NtSys_NOM);
  double getTriggerWeightEE(LeptonVector leptons, SusyNtSys sys);
  double getTriggerWeightEM(LeptonVector leptons, int NPV, SusyNtSys sys);
  double getTriggerWeightMM(LeptonVector leptons, float met, 
			    int njets, int NPV, SusyNtSys sys);
  
  // Debug method
  void debugFlag(uint flag);

  // Currently we check that the mc events fall within
  // a given region, not actually checking the trigger.
  // If the user wants to use the MC
  // trigger as is, simply set this flag.
  void useMCTrigger(){ m_useMCTrig = true; };

  // New trigger has been added but is not ready for
  // official use.  This flag is to be set if we want
  // to use the trigger for data and also for MC 
  // reweighting.
  void useDiumuonMetTrigger(){ m_useDimuonMetTrigger = true; };

 protected:

  //    
  // Triger reweighting    
  //                          

  triggerReweight2Lep* m_triggerWeight;     // Trigger reweighting object

 private:
  
  bool m_useMCTrig;                         // flag to actually check MC trig
 
  //Period              period;             // Period
  //ElectronVector      m_elecs;              // Electron vector for easy access
  //MuonVector          m_muons;              // Muon vector for easy access
  
  bool m_useDimuonMetTrigger;               // True to use Met Trigger

};

#endif
