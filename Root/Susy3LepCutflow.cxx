#include <iomanip>
#include "TCanvas.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/Susy3LepCutflow.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Susy3LepCutflow Constructor
/*--------------------------------------------------------------------------------*/
Susy3LepCutflow::Susy3LepCutflow() :
        m_nLepMin   (  3  ),
        m_nLepMax   (  3  ),
        m_selectZ   (false),
        m_vetoZ     (false),
        m_selectB   (false),
        m_vetoB     (false),
        m_selectSFOS(false),
        m_vetoSFOS  (false),
        m_metMin    ( -1  ),
        m_minMt     ( -1  ),
        m_writeOut  (false)
{
  n_readin        = 0;
  n_pass_LAr      = 0;
  n_pass_BadJet   = 0;
  n_pass_BadMuon  = 0;
  n_pass_Cosmic   = 0;
  n_pass_nLep     = 0;
  n_pass_trig     = 0;
  n_pass_sfos     = 0;
  n_pass_met      = 0;
  n_pass_z        = 0;
  n_pass_bjet     = 0;
  n_pass_mt       = 0;

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
  if(m_dbg) cout << "Susy3LepCutflow::Begin" << endl;

  if(m_sel=="sr1") {
    m_vetoZ      = true;
    m_vetoB      = true;
    m_selectSFOS = true;
    m_metMin     = 75;
  } else {
    cout << "Susy3LepCutflow::ERROR - Unknown selection type [" << m_sel << "], terminating..." << endl;
    abort();
  }

  m_trigObj = new TrilTrigLogic(&nt);
  m_trigObj->loadTriggerMaps();
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
  static Long64_t chainEntry = -1;
  chainEntry++;
  if(m_dbg || chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  // select signal objects
  selectObjects();

  // Check Event
  if(!selectEvent(m_signalLeptons, m_signalJets, m_met)) return false;

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::Terminate()
{
  if(m_dbg) cout << "Susy3LepCutflow::Terminate" << endl;

  dumpEventCounters();

  if(m_writeOut) {
    out.close();
  }
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::selectEvent(const LeptonVector& leptons, const JetVector& jets, const Met* met)
{
  // In this method place all event selection cuts.
  int flag = nt.evt()->evtFlag[NtSys_NOM];

  if( !passLAr(flag            ))  return false;
  n_pass_LAr++;
  if( !passBadJet(flag         ))  return false;
  n_pass_BadJet++;
  if( !passBadMuon(flag        ))  return false;
  n_pass_BadMuon++;
  if( !passCosmic (flag        ))  return false;
  n_pass_Cosmic++;
  if( !passNLepCut(leptons     ))  return false;
  if( !passTrigger(leptons     ))  return false;
  if( !passSFOSCut(leptons     ))  return false;
  if( !passMetCut (met         ))  return false;
  if( !passZCut   (leptons     ))  return false;
  if( !passBJetCut(            ))  return false;
  if( !passMtCut  (leptons,met ))  return false;

  if( m_writeOut ) {
    out << nt.evt()->run << " " << nt.evt()->event << endl;
  }

  return true;
}
/*--------------------------------------------------------------------------------*/
// Analysis cuts
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passNLepCut(const LeptonVector& leptons)
{
  uint nLep = leptons.size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  n_pass_nLep++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passTrigger(const LeptonVector& leptons) 
{
  if(!m_trigObj->passTriggerMatching(leptons)) return false;
  n_pass_trig++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passSFOSCut(const LeptonVector& leptons)
{
  bool sfos = hasSFOS(leptons);
  if(m_vetoSFOS   &&  sfos) return false;
  if(m_selectSFOS && !sfos) return false;
  n_pass_sfos++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passMetCut(const Met* met)
{
  double missEt = met->lv().Et();
  if( m_metMin >= 0 && missEt < m_metMin ) return false;
  n_pass_met++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passZCut(const LeptonVector& leptons)
{
  bool hasz = hasZ(leptons);
  if( m_vetoZ   &&  hasz ) return false;
  if( m_selectZ && !hasz ) return false;
  n_pass_z++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passBJetCut( )
{
  bool hasB = hasBJet(m_signalJets);
  if( m_vetoB   &&  hasB ) return false;
  if( m_selectB && !hasB ) return false;
  n_pass_bjet++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::passMtCut(const LeptonVector& leptons, const Met* met)
{
  // Find the best Z candidate pair, use remaining lepton to form Mt
  if(m_minMt > 0)
  {
    uint zl1, zl2;
    bestZ(zl1, zl2, leptons);

    for(uint iL=0; iL<leptons.size(); iL++)
    {
      if(iL!=zl1 && iL!=zl2)
      {
        if( Mt(leptons[iL],met) < m_minMt ) return false;
      }
    }
  }

  n_pass_mt++;
  return true;
}
/*--------------------------------------------------------------------------------*/
// Event counters
/*--------------------------------------------------------------------------------*/
void Susy3LepCutflow::dumpEventCounters()
{
  cout << endl;
  cout << "Susy3LepCutflow event counters"    << endl;
  cout << "read in     :  " << n_readin        << endl;
  //cout << "pass LAr    :  " << n_pass_LAr      << endl;
  //cout << "pass BadJet :  " << n_pass_BadJet   << endl;
  //cout << "pass BadMu  :  " << n_pass_BadMuon  << endl;
  //cout << "pass Cosmic :  " << n_pass_Cosmic   << endl;
  cout << "pass nLep   :  " << n_pass_nLep     << endl;
  cout << "pass trig   :  " << n_pass_trig     << endl; 
  cout << "pass sfos   :  " << n_pass_sfos     << endl;
  cout << "pass met    :  " << n_pass_met      << endl;
  cout << "pass z      :  " << n_pass_z        << endl;
  cout << "pass b-jet  :  " << n_pass_bjet     << endl;
  cout << "pass mt     :  " << n_pass_mt       << endl;
}

/*--------------------------------------------------------------------------------*/
// Debug event
/*--------------------------------------------------------------------------------*/
bool Susy3LepCutflow::debugEvent()
{
  uint run = nt.evt()->run;
  uint evt = nt.evt()->event;
  //if(run==191139 && evt==140644832) return true;
  if(run==180164&&evt==24769) return true;

  return false;
}
