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

class DilTrigLogic
{

 public:

  // Default constructor and destructor for now
  DilTrigLogic(bool isMC);
  virtual ~DilTrigLogic();

  // Clear any vars stored
  void clear(){
    period = UNKNOWNPERIOD;
    elecs.clear();
    muons.clear();
  };

  // General method to obtain result
  bool passDilTrig(LeptonVector leptons, int RunNumber, DataStream stream);

  // Event Type methods
  bool passEE();
  bool passMM();
  bool passEM(bool isEgamma);

  // Electron specific methods
  uint getSingleETrig();
  uint getDoubleETrig();
  
  // Muon specific methods
  uint getSingleMTrig();
  uint getDoubleMTrig();

  // E-M specific methods
  uint getEMTrig();

  // MC will need to be reweighted
  float getMCWeight();

  // Trigger reweighting                                                                                                            
  void loadTriggerMaps();
  APReweightND* loadTrigWeighter(TFile* f, TString chain);
  APReweightND* getEleTrigWeighter(uint trigFlag);
  APReweightND* getMuoTrigWeighter(uint trigFlag);

  // Handle Effectively the periods
  // Maybe put enum in SusyDefs? Or possible assign to event obj.
  // Also, how to deal with 2012? A_11 and A_12?
  enum Period { A = 0, B, C, D, E, F, G, H, I, J, K, L, M, UNKNOWNPERIOD};
  void setPeriod(int run);

  // Add a method to set the electron and muon vector.
  // Will make it easier for methods to access 
  // the specific leptons it needs
  void setLeptons(LeptonVector leps);
  
  // Return the type of event this is
  // Event types defined in SusyDefs.h
  EventType getEventType(ElectronVector elecs, MuonVector muons);

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
   
  Period              period;             // Period
  ElectronVector      elecs;              // Electron vector for easy access
  MuonVector          muons;              // Muon vector for easy access

};

#endif
