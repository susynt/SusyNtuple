#include <iomanip>
#include "TFile.h"
#include "SusyNtuple/SusyNtTruthAna.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyNtTruthAna Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTruthAna::SusyNtTruthAna() : 
        nt(m_entry),
        m_entry(0),
        m_dbg(1)
{
}

/*--------------------------------------------------------------------------------*/
// Attach tree (actually a TChain)
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::Init(TTree* tree)
{
  if(m_dbg) cout << "SusyNtTruthAna::Init" << endl;
  m_tree = tree;
  nt.ReadFrom(tree);
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
// When running with PROOF Begin() is only called on the client.
// The tree argument is deprecated (on PROOF 0 is passed).
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::Begin(TTree* /*tree*/)
{
  if(m_dbg) cout << "SusyNtTruthAna::Begin" << endl;

  m_chainEntry = -1;

  // Start the timer
  m_timer.Start();

}

/*--------------------------------------------------------------------------------*/
// Main process loop function - This is just an example for testing
/*--------------------------------------------------------------------------------*/
Bool_t SusyNtTruthAna::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  clearTruthObjects();

  // Chain entry not the same as tree entry
  m_chainEntry++;
  
  if(m_dbg || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  // Dump variables from the tree for testing
  if(m_dbg){
    cout << "num ntuple tpr: " << nt.tpr()->size() << endl;
    cout << "num ntuple tjt: " << nt.tjt()->size() << endl;
  }

  // select baseline and signal objects
  selectTruthObjects();

  // Dump some event information
  cout << "============================================" << endl;
  dumpEvent();
  dumpBaselineTruthObjects();
  dumpSignalTruthObjects();

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called during
// a query. It always runs on the client, it can be used to present
// the results graphically or save the results to file.
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::Terminate()
{
  if(m_dbg) cout << "SusyNtTruthAna::Terminate" << endl;

  // Stop the timer
  m_timer.Stop();
  dumpTimer();
}
/*--------------------------------------------------------------------------------*/
// Clear objects
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::clearTruthObjects()
{
  m_baseTruthElectrons.clear();
  m_baseTruthMuons.clear();
  m_baseTruthTaus.clear();
  m_baseTruthJets.clear();
  m_signalTruthElectrons.clear();
  m_signalTruthMuons.clear();
  m_signalTruthTaus.clear();
  m_signalTruthJets.clear();
  m_truthMet = NULL;
}
/*--------------------------------------------------------------------------------*/
// Select baseline and signal objects
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::selectTruthObjects()
{
  if(m_dbg) cout << "SusyNtTruthAna::selectTruthObjects" << endl;

  // Baseline objects
  getBaselineTruthObjects(&nt, m_baseTruthElectrons, m_baseTruthMuons, m_baseTruthTaus, m_baseTruthJets);

  // Signal objects
  getSignalTruthObjects  (m_baseTruthElectrons, m_baseTruthMuons, m_baseTruthTaus, m_baseTruthJets,
                          m_signalTruthElectrons, m_signalTruthMuons, m_signalTruthTaus, m_signalTruthJets);

  // Get Met
  m_truthMet = getTruthMet(&nt);
}
/*--------------------------------------------------------------------------------*/
// Get baseline objects - Carry over to SusyNtTools
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::getBaselineTruthObjects(SusyNtObject* susyNt, 
                                             TruthParticleVector& truthElectrons, 
                                             TruthParticleVector& truthMuons, 
                                             TruthParticleVector& truthTaus, 
                                             TruthJetVector& truthJets)
{
  if(m_dbg) cout << "SusyNtTruthAna::getBaselineTruthObjects" << endl;

  truthElectrons = getPreTruthLeptons(susyNt,11);
  truthMuons     = getPreTruthLeptons(susyNt,13);
  truthTaus      = getPreTruthLeptons(susyNt,15);
  truthJets      = getPreTruthJets   (susyNt   );

  // Do we really need overlap removal at the truth level
  // I'm not 100% sold on this, so I skip at the moment

  // Do SFOS removal for Mll < 20 GeV
  removeSFOSPair(truthElectrons);
  removeSFOSPair(truthMuons    );
  removeSFOSPair(truthTaus     ); 
}
/*--------------------------------------------------------------------------------*/
// Methods to grab the Baseline objects
/*--------------------------------------------------------------------------------*/
TruthParticleVector SusyNtTruthAna::getPreTruthLeptons(Susy::SusyNtObject* susyNt,
                                                       int pdgId)
{
  if(m_dbg) cout << "SusyNtTruthAna::getPreTruthLeptons" << endl;

  TruthParticleVector truthLeptons;
  for(uint index=0; index<susyNt->tpr()->size(); ++index){
    TruthParticle* particle = & susyNt->tpr()->at(index);
    if(fabs(particle->pdgId) != pdgId ) continue;
    // Electrons
    if(fabs(particle->pdgId) == 11) {
      if( particle->Pt() < TRUTH_ELECTRON_PT_CUT || 
          fabs(particle->Eta()) > TRUTH_ELECTRON_ETA_CUT ) continue;
      truthLeptons.push_back(particle);
    }
    // Muons
    else if(fabs(particle->pdgId) == 13) {
      if( particle->Pt() < TRUTH_MUON_PT_CUT || 
          fabs(particle->Eta()) > TRUTH_MUON_ETA_CUT ) continue;
      truthLeptons.push_back(particle);
    }
    // Taus
    else if(fabs(particle->pdgId) == 15) {
      if( particle->Pt() < TRUTH_TAU_PT_CUT || 
          fabs(particle->Eta()) > TRUTH_TAU_ETA_CUT ) continue;
      truthLeptons.push_back(particle);
    }
  }

  return truthLeptons;
}
/*--------------------------------------------------------------------------------*/
TruthJetVector      SusyNtTruthAna::getPreTruthJets     (Susy::SusyNtObject* susyNt)
{
  if(m_dbg) cout << "SusyNtTruthAna::getPreTruthJets" << endl;

  TruthJetVector truthJets;
  for(uint index=0; index<susyNt->tjt()->size(); ++index){
    TruthJet* jet = & susyNt->tjt()->at(index);
    if( jet->Pt() < TRUTH_JET_CENTRAL_PT_CUT || 
        fabs(jet->Eta()) > TRUTH_JET_ETA_MAX_CUT ) continue;
    truthJets.push_back(jet);
  }

  return truthJets;
}
/*--------------------------------------------------------------------------------*/
// Get signal objects - Carry over to SusyNtTools
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::getSignalTruthObjects(TruthParticleVector& truthBaseElectrons, 
                                           TruthParticleVector& truthBaseMuons, 
                                           TruthParticleVector& truthBaseTaus, 
                                           TruthJetVector&      truthBaseJets,
                                           TruthParticleVector& truthSignalElectrons, 
                                           TruthParticleVector& truthSignalMuons, 
                                           TruthParticleVector& truthSignalTaus, 
                                           TruthJetVector&      truthSignalJets)
{
  if(m_dbg) cout << "SusyNtTruthAna::getSignalTruthObjects" << endl;

  // Leptons are identical for baseline and signal
  truthSignalElectrons = getSignalTruthLeptons(truthBaseElectrons);
  truthSignalMuons     = getSignalTruthLeptons(truthBaseMuons    );
  truthSignalTaus      = getSignalTruthLeptons(truthBaseTaus     );
  truthSignalJets      = getSignalTruthJets   (truthBaseJets     );
}
/*--------------------------------------------------------------------------------*/
// Methods to grab the Signal objects
/*--------------------------------------------------------------------------------*/
TruthParticleVector SusyNtTruthAna::getSignalTruthLeptons(TruthParticleVector& truthBaseLeptons)
{
  if(m_dbg) cout << "SusyNtTruthAna::getSignalTruthLeptons" << endl;

  TruthParticleVector truthSignalLeptons;
  for(uint index=0; index<truthBaseLeptons.size(); ++index){
    TruthParticle* particle = truthBaseLeptons.at(index);
    truthSignalLeptons.push_back(particle);
  }
  return truthSignalLeptons;
}
/*--------------------------------------------------------------------------------*/
TruthJetVector SusyNtTruthAna::getSignalTruthJets   (TruthJetVector& truthBaseJets)
{
  if(m_dbg) cout << "SusyNtTruthAna::getSignalTruthJets" << endl;

  TruthJetVector truthSignalJets;
  for(uint index=0; index<truthBaseJets.size(); ++index){
    TruthJet* jet = truthBaseJets.at(index);
    if( jet->Pt() < TRUTH_JET_FORWARD_PT_CUT && fabs(jet->Eta()) > 2.5 ) continue;
    truthSignalJets.push_back(jet);
  }

  return truthSignalJets;
}

/*--------------------------------------------------------------------------------*/
// Get Met - Carry over to SusyNtTools
/*--------------------------------------------------------------------------------*/
Susy::TruthMet* SusyNtTruthAna::getTruthMet(SusyNtObject* susyNt)
{
  if(m_dbg) cout << "SusyNtTruthAna::getTruthMet" << endl;
  
  TruthMet* truthMet = NULL;

  truthMet = &(susyNt->tmt()->at(0));

  if(!truthMet) {
    cout << "Error :: Unable to retrieve truth MET!!" << endl;
  }

  return truthMet;
}
/*--------------------------------------------------------------------------------*/
// Methods to handle signal region cuts
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::removeSFOSPair(TruthParticleVector& leptons, float MllCut)
{
  uint nLep = leptons.size();
  if(nLep < 2) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  TruthParticleVector leptonsPass;

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nLep; i++){
    TruthParticle* l1 = leptons[i];
    bool pass = true;
    for(uint j=0; j<nLep; j++){
      if(i==j) continue;
      TruthParticle* l2 = leptons[j];
      if( l1->charge*l2->charge < 0 && (*l1 + *l2).M() < MllCut ){
        pass = false;
        break;
      }
    }
    if(pass) leptonsPass.push_back(l1);
  }

  // Now replace the supplied vector with this cleaned vector
  leptons = leptonsPass;
}

/*--------------------------------------------------------------------------------*/
// Dump various information
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::dumpTimer()
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
void SusyNtTruthAna::dumpEvent()
{
  nt.evt()->print();
}
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::dumpBaselineTruthObjects()
{

  uint nTruEle = m_baseTruthElectrons.size();
  if(nTruEle>0){
    cout << "Baseline Truth Electrons" << endl;
    for(uint iTruEle=0;iTruEle<nTruEle;iTruEle++){
      cout << "  ";
      m_baseTruthElectrons[iTruEle]->print();
    }
  }

  uint nTruMu = m_baseTruthMuons.size();
  if(nTruMu>0){
    cout << "Baseline Truth Muons" << endl;
    for(uint iTruMu=0;iTruMu<nTruMu;iTruMu++){
      cout << "  ";
      m_baseTruthMuons[iTruMu]->print();
    }
  }

  uint nTruTau = m_baseTruthTaus.size();
  if(nTruTau>0){
    cout << "Baseline Truth Taus" << endl;
    for(uint iTruTau=0;iTruTau<nTruTau;iTruTau++){
      cout << "  ";
      m_baseTruthTaus[iTruTau]->print();
    }
  }

  uint nTruJet = m_baseTruthJets.size();
  if(nTruJet>0){
    cout << "Baseline Truth Jets" << endl;
    for(uint iTruJet=0;iTruJet<nTruJet;iTruJet++){
      cout << "  ";
      m_baseTruthJets[iTruJet]->print();
    }
  }

}
/*--------------------------------------------------------------------------------*/
void SusyNtTruthAna::dumpSignalTruthObjects()
{
  uint nTruEle = m_signalTruthElectrons.size();
  if(nTruEle>0){
    cout << "Signal Truth Electrons" << endl;
    for(uint iTruEle=0;iTruEle<nTruEle;iTruEle++){
      cout << "  ";
      m_signalTruthElectrons[iTruEle]->print();
    }
  }

  uint nTruMu = m_signalTruthMuons.size();
  if(nTruMu>0){
    cout << "Signal Truth Muons" << endl;
    for(uint iTruMu=0;iTruMu<nTruMu;iTruMu++){
      cout << "  ";
      m_signalTruthMuons[iTruMu]->print();
    }
  }

  uint nTruTau = m_signalTruthTaus.size();
  if(nTruTau>0){
    cout << "Signal Truth Taus" << endl;
    for(uint iTruTau=0;iTruTau<nTruTau;iTruTau++){
      cout << "  ";
      m_signalTruthTaus[iTruTau]->print();
    }
  }

  uint nTruJet = m_signalTruthJets.size();
  if(nTruJet>0){
    cout << "Signal Truth Jets" << endl;
    for(uint iTruJet=0;iTruJet<nTruJet;iTruJet++){
      cout << "  ";
      m_signalTruthJets[iTruJet]->print();
    }
  }

}
