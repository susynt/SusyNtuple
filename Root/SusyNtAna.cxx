#include <iomanip>
#include "TFile.h"
#include "SusyNtuple/SusyNtAna.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyNtAna Constructor
/*--------------------------------------------------------------------------------*/
SusyNtAna::SusyNtAna() : 
        nt(m_entry),
        m_entry(0),
        m_selectTaus(true),
        m_printFreq(50000),
        m_dbg(0),
        m_dbgEvt(false),
        m_duplicate(false)
{
}

/*--------------------------------------------------------------------------------*/
// Attach tree (normally a TChain)
/*--------------------------------------------------------------------------------*/
void SusyNtAna::Init(TTree* tree)
{
  if(m_dbg) cout << "SusyNtAna::Init" << endl;
  m_tree = tree;
  nt.ReadFrom(tree);
  m_mcWeighter.buildSumwMap(tree);
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
// When running with PROOF Begin() is only called on the client.
// The tree argument is deprecated (on PROOF 0 is passed).
/*--------------------------------------------------------------------------------*/
void SusyNtAna::Begin(TTree* /*tree*/)
{
  if(m_dbg) cout << "SusyNtAna::Begin" << endl;

  m_chainEntry = -1;

  // Start the timer
  m_timer.Start();

  //Debug event - load event list
  if(m_dbgEvt) loadEventList();
}

/*--------------------------------------------------------------------------------*/
// Main process loop function - This is just an example for testing
/*--------------------------------------------------------------------------------*/
Bool_t SusyNtAna::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  clearObjects();

  // Chain entry not the same as tree entry
  //static Long64_t chainEntry = -1;
  m_chainEntry++;
  
  if(m_dbg || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->eventNumber << " ****" << endl;
  }

  //Debug this event - check if should be processed
  if(m_dbgEvt && !processThisEvent(nt.evt()->run, nt.evt()->eventNumber)) return kFALSE;

  // Dump variables from the tree for testing
  if(m_dbg){
    cout << "num ntuple ele: " << nt.ele()->size() << endl;
    cout << "num ntuple muo: " << nt.muo()->size() << endl;
    cout << "num ntuple tau: " << nt.tau()->size() << endl;
    cout << "num ntuple jet: " << nt.jet()->size() << endl;
    //cout << "Met:          " << nt.met()->Pt()   << endl;
  }
  
  //Check Duplicate run:event
  if(!nt.evt()->isMC && checkDuplicate()){
    if(isDuplicate(nt.evt()->run, nt.evt()->eventNumber))  return kFALSE;
  }

  // select baseline and signal objects
  selectObjects();

  nt.evt()->print();
  dumpBaselineObjects();
  dumpSignalObjects();

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called during
// a query. It always runs on the client, it can be used to present
// the results graphically or save the results to file.
/*--------------------------------------------------------------------------------*/
void SusyNtAna::Terminate()
{
  if(m_dbg) cout << "SusyNtAna::Terminate" << endl;

  // Stop the timer
  m_timer.Stop();
  dumpTimer();
}

/*--------------------------------------------------------------------------------*/
// Load Event list of run/event to process. Use to debug events
/*--------------------------------------------------------------------------------*/
void SusyNtAna::loadEventList(const std::string filename)
{
  int run, event;
  FILE* eventsFile=fopen(filename.c_str(),"r");
  int nEvtDbg=0;
  while(fscanf(eventsFile,"%i %i \n",&run, &event) != EOF){
    cout << "Adding run-event " << run << " " << event << endl; 
    addRunEvent(m_eventList, run, event);
    nEvtDbg++;
  }
  std::cout << " >>> Debugging " << nEvtDbg << " events " << std::endl;
}
/*--------------------------------------------------------------------------------*/
// Process selected events only
/*--------------------------------------------------------------------------------*/
bool SusyNtAna::processThisEvent(unsigned int run, unsigned int event)
{
  if(m_eventList.size()==0) return true;
  return checkRunEvent(m_eventList, run, event);
}
bool SusyNtAna::checkRunEvent(const RunEventMap &runEventMap, unsigned int run, unsigned int event)
{
  RunEventMap::const_iterator eventSetIter = runEventMap.find(run);
  return eventSetIter != runEventMap.end() && 
    eventSetIter->second->find(event) != eventSetIter->second->end();
}
bool SusyNtAna::checkAndAddRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event)
{
  set<unsigned int> *&eventSet = runEventMap[run];
  if ( !eventSet )
    eventSet = new set<unsigned int>();
  return !eventSet->insert(event).second;
}

/*--------------------------------------------------------------------------------*/
// Check for duplicate run/event pair
// Designed for data
// For MC, need to add a check on a event var, since duplice run/event may occur
/*--------------------------------------------------------------------------------*/
bool SusyNtAna::isDuplicate(unsigned int run, unsigned int event){

  if(m_eventListDuplicate.size()==0) addRunEvent(m_eventListDuplicate, run, event);
  else{
    if(checkRunEvent(m_eventListDuplicate, run, event)){
      cout << "WARNING Duplicate event - SKIPING IT !!!" << run << " " << event << endl;
      return true;
    }
    else addRunEvent(m_eventListDuplicate, run, event);
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
/*float SusyNtAna::getEventWeightFixed(unsigned int mcChannel, float lumi)
{
  return SusyNtTools::getEventWeightFixed(mcChannel, nt.evt(), lumi);
}*/
/*--------------------------------------------------------------------------------*/
/*float SusyNtAna::getEventWeightAB3()
{
  return SusyNtTools::getEventWeightAB3(nt.evt());
}*/
/*--------------------------------------------------------------------------------*/
/*float SusyNtAna::getEventWeightAB()
{
  return SusyNtTools::getEventWeightAB(nt.evt());
}*/

/*--------------------------------------------------------------------------------*/
// Clear objects
/*--------------------------------------------------------------------------------*/
void SusyNtAna::clearObjects()
{
  // pre objects
  m_preElectrons.clear();
  m_preMuons.clear();
  m_preTaus.clear();
  m_preJets.clear();
  m_preLeptons.clear();

  // baseline objects
  m_baseElectrons.clear();
  m_baseMuons.clear();
  m_baseTaus.clear();
  m_baseJets.clear();
  m_baseLeptons.clear();

  // signal objects
  m_signalElectrons.clear();
  m_signalMuons.clear();
  m_signalTaus.clear();
  m_signalJets.clear();
  m_signalLeptons.clear();

  // met
  m_met = NULL;
  m_trackMet = NULL;

  //////////////////
  // old:
  //m_baseElectrons.clear();
  //m_baseMuons.clear();
  //m_baseTaus.clear();
  //m_baseLeptons.clear();
  //m_baseJets.clear();
  //m_signalElectrons.clear();
  //m_signalMuons.clear();
  //m_signalLeptons.clear();
  //m_signalTaus.clear();
  //m_signalJets.clear();
  //m_signalJets2Lep.clear();
  //m_mediumTaus.clear();
  //m_tightTaus.clear();
  //m_met = NULL;
  //m_trackMet = NULL;
  // old
  //////////////////
}
/*--------------------------------------------------------------------------------*/
// Select baseline and signal leptons
/*--------------------------------------------------------------------------------*/
void SusyNtAna::selectObjects(SusyNtSys sys, TauId signalTauID)
{
  ///////////////////////////////////////
  // Empty the object vectors
  ///////////////////////////////////////
  clearObjects();

  ///////////////////////////////////////
  // Get the Pre-Selection.
  // Systematic variation applied here.
  ///////////////////////////////////////
  m_nttools.getPreObjects(&nt, sys, m_preElectrons, m_preMuons, m_preJets, m_preTaus);

  ///////////////////////////////////////
  // Get the Baseline Objects
  ///////////////////////////////////////
  m_nttools.getBaselineObjects(m_preElectrons, m_preMuons, m_preJets, m_preTaus,
                               m_baseElectrons, m_baseMuons, m_baseJets, m_baseTaus);
  ///////////////////////////////////////
  // do OR 
  ///////////////////////////////////////
  m_nttools.overlapTool().performOverlap(m_baseElectrons, m_baseMuons, m_baseTaus, m_baseJets);

  ///////////////////////////////////////
  // SFOS removal
  ///////////////////////////////////////
  if(m_nttools.doSFOSRemoval()){
    m_nttools.removeSFOSPairs(m_baseElectrons, m_baseMuons);
  }

  ///////////////////////////////////////
  // Get the Signal Objects
  ///////////////////////////////////////
  m_nttools.getSignalObjects(m_baseElectrons, m_baseMuons, m_baseJets, m_baseTaus,
                              m_signalElectrons, m_signalMuons, m_signalJets, m_signalTaus, signalTauID);

  ///////////////////////////////////////
  // Build Lepton vectors
  ///////////////////////////////////////
  m_nttools.buildLeptons(m_preLeptons, m_preElectrons, m_preMuons);
  m_nttools.buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
  m_nttools.buildLeptons(m_signalLeptons, m_signalElectrons, m_signalMuons);

  ///////////////////////////////////////
  // Grab met
  ///////////////////////////////////////
  SusyNtSys metSys = sys;
  //AT 05-09-15 JVF obsolete run-2
  //if(sys==NtSys::JVF_UP || sys==NtSys::JVF_DN) metSys = NtSys::NOM;
  m_met = m_nttools.getMet(&nt, metSys);
  m_trackMet = m_nttools.getTrackMet(&nt, metSys);

 //////////////////////////////////////////////////////////////////////////////////
 //// old:
 // m_nttools.getBaselineObjects(&nt, m_preElectrons, m_preMuons, m_preJets, 
 //                              m_baseElectrons, m_baseMuons, m_baseTaus, m_baseJets, 
 //                              sys, m_selectTaus);

 // // Now grab Signal objects
 // // New signal tau prescription, fill both ID levels at once
 // m_nttools.getSignalObjects(m_baseElectrons, m_baseMuons, m_baseTaus, m_baseJets,
 //                            m_signalElectrons, m_signalMuons,
 //                            m_mediumTaus, m_tightTaus,
 //                            m_signalJets, m_signalJets2Lep,
 //                            nt.evt()->nVtx, nt.evt()->isMC, sys);
 // m_signalTaus = m_nttools.m_tauSelector.signalTauId() == TauId::Tight ? m_tightTaus : m_mediumTaus;
 // //m_signalTaus = signalTauID == TauId::Tight ? m_tightTaus : m_mediumTaus;

 // // old
 // ////////////////////////////////////////////////////////////////////////////////


}
/*--------------------------------------------------------------------------------*/
// Get on-the-fly cleaning cut flags. Select objects first!
/*--------------------------------------------------------------------------------*/
int SusyNtAna::cleaningCutFlags()
{
  return nt.evt()->cutFlags[NtSys::NOM];
  //return m_nttools.cleaningCutFlags(nt.evt()->cutFlags[NtSys::NOM],
  //                                  m_preMuons, m_baseMuons,
  //                                  m_preJets, m_baseJets);
}


/*--------------------------------------------------------------------------------*/
// Dump timer information
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpTimer()
{
  cout << endl;
  double realTime = m_timer.RealTime();
  double cpuTime  = m_timer.CpuTime();
  int hours = int(realTime / 3600);
  realTime -= hours * 3600;
  int min   = int(realTime / 60);
  realTime -= min * 60;
  int sec   = int(realTime);

  float speed = (m_chainEntry+1)/m_timer.RealTime()/1000;

  printf("---------------------------------------------------\n");
  printf(" Number of events processed: %d \n",(int)m_chainEntry+1);
  printf("\t Analysis time: Real %d:%02d:%02d, CPU %.3f      \n", hours, min, sec, cpuTime);
  printf("\t Analysis speed [kHz]: %2.3f                     \n",speed);
  printf("---------------------------------------------------\n\n");
}

/*--------------------------------------------------------------------------------*/
// Event and object dumps
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpEvent()
{
  nt.evt()->print();
  //nt.met()->print();
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpNtupleObjects()
{
  uint nEle = nt.ele()->size();
  if(nEle>0){
    cout << "Ntuple electrons" << endl;
    for(uint iEl=0; iEl<nEle; iEl++){
      cout << "  ";
      nt.ele()->at(iEl).print();
    }
  }
  uint nMu = nt.muo()->size();
  if(nMu>0){
    cout << "Ntuple muons" << endl;
    for(uint iMu=0; iMu<nMu; iMu++){
      cout << "  ";
      nt.muo()->at(iMu).print();
    }
  }
  uint nTau = nt.tau()->size();
  if(nTau>0){
    cout << "Ntuple taus" << endl;
    for(uint iTau=0; iTau<nTau; iTau++){
      cout << "  ";
      nt.tau()->at(iTau).print();
    }
  }
  uint nJet = nt.jet()->size();
  if(nJet>0){
    cout << "Ntuple jets" << endl;
    for(uint iJ=0; iJ<nJet; iJ++){
      cout << "  ";
      nt.jet()->at(iJ).print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpBaselineObjects()
{
  dumpBaselineLeptons();
  dumpBaselineJets();
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpBaselineLeptons()
{
  uint nEle = m_baseElectrons.size();
  if(nEle>0){
    cout << "Baseline electrons" << endl;
    for(uint iEl=0; iEl<nEle; iEl++){
      cout << "  ";
      m_baseElectrons[iEl]->print();
    }
  }
  uint nMu = m_baseMuons.size();
  if(nMu>0){
    cout << "Baseline muons" << endl;
    for(uint iMu=0; iMu<nMu; iMu++){
      cout << "  ";
      m_baseMuons[iMu]->print();
    }
  }
  uint nTau = m_baseTaus.size();
  if(nTau>0){
    cout << "Baseline taus" << endl;
    for(uint iTau=0; iTau<nTau; iTau++){
      cout << "  ";
      m_baseTaus[iTau]->print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpBaselineJets()
{
  uint nJet = m_baseJets.size();
  if(nJet>0){
    cout << "Baseline jets" << endl;
    for(uint iJ=0; iJ<nJet; iJ++){
      cout << "  ";
      m_baseJets[iJ]->print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpSignalObjects()
{
  dumpSignalLeptons();
  dumpSignalJets();
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpSignalLeptons()
{
  uint nEle = m_signalElectrons.size();
  if(nEle>0){
    cout << "Signal electrons" << endl;
    for(uint iEl=0; iEl < m_signalElectrons.size(); iEl++){
      cout << "  ";
      m_signalElectrons[iEl]->print();
    }
  }
  uint nMu = m_signalMuons.size();
  if(nMu>0){
    cout << "Signal muons" << endl;
    for(uint iMu=0; iMu < m_signalMuons.size(); iMu++){
      cout << "  ";
      m_signalMuons[iMu]->print();
    }
  }
  uint nTau = m_signalTaus.size();
  if(nTau>0){
    cout << "Signal taus" << endl;
    for(uint iTau=0; iTau<nTau; iTau++){
      cout << "  ";
      m_signalTaus[iTau]->print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpSignalJets()
{
  uint nJet = m_signalJets.size();
  if(nJet>0){
    cout << "Signal jets" << endl;
    for(uint iJ=0; iJ < m_signalJets.size(); iJ++){
      cout << "  ";
      m_signalJets[iJ]->print();
    }
  }
}
