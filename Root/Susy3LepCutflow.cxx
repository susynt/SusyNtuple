#include <iomanip>
#include "TSystem.h"
#include "TCanvas.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/Susy3LepCutflow.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Susy3LepCutflow Constructor
/*--------------------------------------------------------------------------------*/
Susy3LepCutflow::Susy3LepCutflow() :
        m_sel(""),
        m_trigObj(0),
        m_nBaseLepMin(3),
        m_nBaseLepMax(3),
        m_nLepMin(3),
        m_nLepMax(3),
        m_nTauMin(0),
        m_nTauMax(0),
        m_baseLepMinDR(0.3),
        m_selectSFOS(false),
        m_vetoSFOS(false),
        m_selectZ(false),
        m_vetoZ(false),
        m_selectB(false),
        m_vetoB(false),
        m_metMin(-1),
        m_mtMin(-1),
        m_writeOut(false)
{
  n_readin        = 0;
  n_pass_hotSpot  = 0;
  n_pass_badJet   = 0;
  n_pass_badMuon  = 0;
  n_pass_cosmic   = 0;
  n_pass_feb      = 0;
  n_pass_nLep     = 0;
  n_pass_nTau     = 0;
  n_pass_trig     = 0;
  n_pass_sfos     = 0;
  n_pass_z        = 0;
  n_pass_met      = 0;
  n_pass_bJet     = 0;
  n_pass_mt       = 0;

  n_evt_tot       = 0;

  setAnaType(Ana_3Lep);

  if(m_writeOut) {
    out.open("event.dump");
  }
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::Begin(TTree* /*tree*/)
{
  SusyNtAna::Begin(0);
  if(m_dbg) cout << "Susy3LepCutflow::Begin" << endl;

  if(m_sel=="sr1") {
    m_vetoZ = true;
    m_vetoB = true;
    m_selectSFOS = true;
    m_metMin = 75;
  } 
  else {
    cout << "Susy3LepCutflow::ERROR - Unknown selection type [" << m_sel << "], terminating..." << endl;
    abort();
  }

  // Trigger logic
  m_trigObj = new TrilTrigLogic();
  m_trigObj->loadTriggerMaps();

  // Book histograms
  //bookHistos();
}

/*--------------------------------------------------------------------------------*/
// Init is called when TTree or TChain is attached
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::Init(TTree* tree)
{
  SusyNtAna::Init(tree);
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t Susy3LepCutflow::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  clearObjects();
  n_readin++;

  // Chain entry not the same as tree entry
  m_chainEntry++;
  if(m_dbg || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  //
  // Object selection
  //

  SusyNtSys ntSys = NtSys::NOM;
  bool subtractLepsFromIso = false;
  TauID tauID = TauID_medium;
  selectObjects(ntSys, subtractLepsFromIso, tauID);

  //
  // Event selection
  //

  if(!selectEvent(m_signalLeptons, m_signalTaus, m_signalJets, m_met)) return false;

  //
  // Event weighting
  //

  // Weight event to luminosity with cross section and pileup
  // New approach, using MCWeighter
  const Event* evt = nt.evt();
  MCWeighter::WeightSys wSys = MCWeighter::Sys_NOM;
  float w = SusyNtAna::mcWeighter().getMCWeight(evt, LUMI_A_L, wSys);

  // Lepton efficiency correction
  float lepSF = getLeptonSF(m_signalLeptons);
  float tauSF = getTauSF(m_signalTaus);

  // Apply btag efficiency correction if selecting on b-jets
  bool applyBTagSF = m_vetoB;
  float btagSF = applyBTagSF? nttools().bTagSF(evt, m_signalJets, evt->mcChannel) : 1;

  // Full event weight
  float fullWeight = w * lepSF * tauSF * btagSF;
  n_evt_tot += fullWeight;

  //cout << "mcSF   " << w << endl;
  //cout << "lepSF  " << lepSF << endl;
  //cout << "tauSF  " << tauSF << endl;
  //cout << "btagSF " << btagSF << endl;
  //cout << "full   " << fullWeight << endl;
  //cout << "n_evt_tot " << n_evt_tot << endl;

  //cout << "mcSF   " << w << endl;
  //cout << "lepSF  " << lepSF << endl;
  //cout << "tauSF  " << tauSF << endl;
  //cout << "btagSF " << btagSF << endl;
  //cout << "full   " << fullWeight << endl;
  //cout << "n_evt_tot " << n_evt_tot << endl;

  //
  // Plotting
  //

  // Fill histograms
  fillHistos(m_signalLeptons, m_signalTaus, m_signalJets, m_met, fullWeight);

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::Terminate()
{
  SusyNtAna::Terminate();
  if(m_dbg) cout << "Susy3LepCutflow::Terminate" << endl;

  finalizeHistos();

  dumpEventCounters();

  if(m_writeOut) {
    out.close();
  }
}

/*--------------------------------------------------------------------------------*/
// Book histograms
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::bookHistos()
{
  // Initialize histograms here
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::selectEvent(const LeptonVector& leptons, const TauVector& taus, 
                                  const JetVector& jets, const Met* met)
{
  const Event* evt = nt.evt();
  int flag = cleaningCutFlags();

  // Cleaning cuts
  if(!nttools().passHotSpot(flag)) return false;
  n_pass_hotSpot++;
  if(!nttools().passBadJet(flag)) return false;
  n_pass_badJet++;
  if(!nttools().passBadMuon(flag)) return false;
  n_pass_badMuon++;
  if(!nttools().passCosmic(flag)) return false;
  n_pass_cosmic++;
  if(!nttools().passDeadRegions(m_preJets, met, evt->run, evt->isMC)) return false;
  n_pass_feb++;
  if(!passNLepCut(leptons)) return false;
  n_pass_nLep++;
  if(!passNTauCut(taus)) return false;
  n_pass_nTau++;
  if(!passTrigger(leptons)) return false;
  n_pass_trig++;
  if(!passSFOSCut(leptons)) return false;
  n_pass_sfos++;
  if(!passZCut(leptons)) return false;
  n_pass_z++;
  if(!passMetCut(met)) return false;
  n_pass_met++;
  if(!passBJetCut()) return false;
  n_pass_bJet++;
  if(!passMtCut(leptons, met)) return false;
  n_pass_mt++;

  if(m_writeOut){
    out << nt.evt()->run << " " << nt.evt()->event << endl;
  }

  return true;
}

/*--------------------------------------------------------------------------------*/
// Fill histograms
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::fillHistos(const LeptonVector& leptons, const TauVector& taus,
                                 const JetVector& jets, const Met* met, float weight)
{
  // Fill histograms here
}

/*--------------------------------------------------------------------------------*/
// Finalize histograms
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::finalizeHistos()
{
  // Finalize histograms here
}

/*--------------------------------------------------------------------------------*/
// Analysis cuts
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passFCal()
{
  const Event* evt = nt.evt();
  if(nttools().m_jetSelector.hasJetInBadFCAL(m_baseJets, evt->run, evt->isMC)) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passNLepCut(const LeptonVector& leptons)
{
  uint nLep = leptons.size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passNTauCut(const TauVector& taus)
{
  uint nTau = taus.size();
  if(m_nTauMin>=0 && nTau < m_nTauMin) return false;
  if(m_nTauMax>=0 && nTau > m_nTauMax) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passTrigger(const LeptonVector& leptons) 
{
  if(!m_trigObj->passTriggerMatching(leptons, m_signalTaus, nt.evt())) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passSFOSCut(const LeptonVector& leptons)
{
  bool sfos = nttools().hasSFOS(leptons);
  if(m_vetoSFOS   &&  sfos) return false;
  if(m_selectSFOS && !sfos) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passMetCut(const Met* met)
{
  double missEt = met->lv().Et();
  if( m_metMin >= 0 && missEt < m_metMin ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passZCut(const LeptonVector& leptons)
{
  bool hasz = nttools().hasZ(leptons);
  if( m_vetoZ   &&  hasz ) return false;
  if( m_selectZ && !hasz ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passBJetCut( )
{
  bool hasB = nttools().hasBJet(m_signalJets);
  if( m_vetoB   &&  hasB ) return false;
  if( m_selectB && !hasB ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passMtCut(const LeptonVector& leptons, const Met* met)
{
  // Find the best Z candidate pair, use remaining lepton to form Mt
  if(m_mtMin > 0)
  {
    uint zl1, zl2;
    if(nttools().findBestZ(zl1, zl2, leptons)){
      for(uint iL=0; iL<leptons.size(); iL++) {
        if(iL!=zl1 && iL!=zl2) {
          if( nttools().Mt(leptons[iL],met) < m_mtMin ) return false;
        }
      }
    }
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
// Lepton efficiency scale factor
/*--------------------------------------------------------------------------------*/
float Susy3LepCutflow::getLeptonSF(const LeptonVector& leptons)
{
  float sf = 1.;
  for(uint i=0; i<leptons.size(); i++){
    const Lepton* lep = leptons[i];
    sf *= lep->effSF;
  }
  return sf;
}
/*--------------------------------------------------------------------------------*/
// Tau efficiency scale factor
/*--------------------------------------------------------------------------------*/
float Susy3LepCutflow::getTauSF(const TauVector& taus)
{
  float sf = 1.;
  for(uint i=0; i<taus.size(); i++){
    const Tau* tau = taus[i];
    sf *= tau->mediumEffSF;
  }
  return sf;
}

/*--------------------------------------------------------------------------------*/
// Event counters
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::dumpEventCounters()
{
  cout << endl;
  cout << "Susy3LepCutflow event counters"    << endl;
  cout << "read in     :  " << n_readin        << endl;
  cout << "pass HotSpot:  " << n_pass_hotSpot  << endl;
  cout << "pass BadJet :  " << n_pass_badJet   << endl;
  cout << "pass BadMu  :  " << n_pass_badMuon  << endl;
  cout << "pass Cosmic :  " << n_pass_cosmic   << endl;
  cout << "pass nLep   :  " << n_pass_nLep     << endl;
  cout << "pass trig   :  " << n_pass_trig     << endl;
  cout << "pass sfos   :  " << n_pass_sfos     << endl;
  cout << "pass z      :  " << n_pass_z        << endl;
  cout << "pass met    :  " << n_pass_met      << endl;
  cout << "pass b-jet  :  " << n_pass_bJet     << endl;
  cout << "pass mt     :  " << n_pass_mt       << endl;
  cout << endl;
  cout << "Weighted event yields"              << endl;
  cout << "A-L (20/fb) :  " << n_evt_tot       << endl;
}

/*--------------------------------------------------------------------------------*/
// Debug event
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::debugEvent()
{
  //uint run = nt.evt()->run;
  //uint evt = nt.evt()->event;
  //if(run==191139 && evt==140644832) return true;
  //if(run==180164&&evt==24769) return true;
  return false;
}

