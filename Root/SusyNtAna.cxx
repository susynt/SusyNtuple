#include <iomanip>
#include "TFile.h"
#include "SusyNtuple/SusyNtAna.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyNtAna Constructor
/*--------------------------------------------------------------------------------*/
SusyNtAna::SusyNtAna() : 
        SusyNtTools(),
        nt(m_entry),
        m_entry(0),
        m_dbg(0)
{
}

/*--------------------------------------------------------------------------------*/
// Attach tree (actually a TChain)
/*--------------------------------------------------------------------------------*/
void SusyNtAna::Init(TTree* tree)
{
  if(m_dbg) cout << "SusyNtAna::Init" << endl;
  m_tree = tree;
  nt.ReadFrom(tree);
  //evtlist.open("evtListTest.out");
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
}

/*--------------------------------------------------------------------------------*/
// Main process loop function - This is just an example for testing
/*--------------------------------------------------------------------------------*/
Bool_t SusyNtAna::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  clearObjects();

  //if(nt.evt()->event != 24969) return kTRUE;

  // Chain entry not the same as tree entry
  //static Long64_t chainEntry = -1;
  m_chainEntry++;
  if(m_dbg || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  // Dump variables from the tree for testing
  if(m_dbg){
    cout << "num base ele: " << nt.ele()->size() << endl;
    cout << "num base muo: " << nt.muo()->size() << endl;
    cout << "num base jet: " << nt.jet()->size() << endl;
    //cout << "Met:          " << nt.met()->Pt()   << endl;
  }

  // select signal objects
  selectObjects();

  if(m_signalLeptons.size() >= 3){
    
    //evtlist << nt.evt()->run << " " << nt.evt()->event << " " << m_signalElectrons.size() << " "
	    //<< m_signalMuons.size() << " " << m_signalJets.size() << endl;

  }

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
// Get event weight, combine gen, pileup, xsec, and lumi weights
/*--------------------------------------------------------------------------------*/
float SusyNtAna::getEventWeight()
{
  // If data, these should all be one
  const Event* evt = nt.evt();
  return evt->w * evt->wPileup * evt->xsec * evt->lumiSF;
}

/*--------------------------------------------------------------------------------*/
// Clear objects
/*--------------------------------------------------------------------------------*/
void SusyNtAna::clearObjects()
{
  m_baseElectrons.clear();
  m_signalElectrons.clear();
  m_baseMuons.clear();
  m_signalMuons.clear();
  m_baseLeptons.clear();
  m_signalLeptons.clear();
  m_signalJets.clear();
  m_met = NULL;
}
/*--------------------------------------------------------------------------------*/
// Select baseline and signal leptons
/*--------------------------------------------------------------------------------*/
void SusyNtAna::selectObjects(SusyNtSys sys)
{

  // Get the Baseline objets
  JetVector m_baseJets;
  getBaselineObjects(&nt, m_baseElectrons, m_baseMuons, m_baseJets, sys);

  // Now grab Signal objects
  getSignalObjects(m_baseElectrons, m_baseMuons, m_baseJets,
		   m_signalElectrons, m_signalMuons, m_signalJets);

  // Grab met
  m_met = getMet(&nt, sys);

  // Build Lepton vectors
  buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
  buildLeptons(m_signalLeptons, m_signalElectrons, m_signalMuons);

  // sort leptons by pt
  std::sort(m_baseLeptons.begin(), m_baseLeptons.end(), comparePt);
  std::sort(m_signalLeptons.begin(), m_signalLeptons.end(), comparePt);

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

  float speed = m_chainEntry/m_timer.RealTime()/1000;

  printf("---------------------------------------------------\n");
  printf(" Number of events processed: %d \n",(int)m_chainEntry);
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
void SusyNtAna::dumpBaselineObjects()
{
  cout << "Baseline electrons" << endl;
  for(uint iEl=0; iEl < m_baseElectrons.size(); iEl++){
    cout << "  ";
    m_baseElectrons[iEl]->print();
  }
  cout << "Baseline muons" << endl;
  for(uint iMu=0; iMu < m_baseMuons.size(); iMu++){
    cout << "  ";
    m_baseMuons[iMu]->print();
  }
  // baseline jets are already stored in the SusyNt
  cout << "Baseline jets" << endl;
  uint nJet = nt.jet()->size();
  for(uint iJ=0; iJ < nJet; iJ++){
    cout << "  ";
    nt.jet()->at(iJ).print();
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtAna::dumpSignalObjects()
{
  cout << "Signal electrons" << endl;
  for(uint iEl=0; iEl < m_signalElectrons.size(); iEl++){
    cout << "  ";
    m_signalElectrons[iEl]->print();
  }
  cout << "Signal muons" << endl;
  for(uint iMu=0; iMu < m_signalMuons.size(); iMu++){
    cout << "  ";
    m_signalMuons[iMu]->print();
  }
  cout << "Signal jets" << endl;
  for(uint iJ=0; iJ < m_signalJets.size(); iJ++){
    cout << "  ";
    m_signalJets[iJ]->print();
  }
}
