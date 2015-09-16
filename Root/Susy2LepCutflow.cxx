#include <iomanip>
#include "TCanvas.h"
#include "SusyNtuple/Susy2LepCutflow.h"
#include "SusyNtuple/KinematicTools.h"
#include "SusyNtuple/JetSelector.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Susy2LepCutflow Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepCutflow::Susy2LepCutflow() :
        m_input_chain(0),
        m_nLepMin(2),
        m_nLepMax(2),
        m_cutNBaseLep(true),
	m_ET(ET_Unknown)
{
  n_readin       = 0;
  n_pass_grl     = 0;
  n_pass_LAr     = 0;
  n_pass_tileErr = 0;
  n_pass_ttc     = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_goodVtx = 0;
  n_pass_Cosmic  = 0;

  // The rest are channel specific.
  for(int i=0; i<ET_N; ++i){
    n_pass_nLep[i]    = 0;
    n_pass_trig[i]    = 0;
    n_pass_flavor[i]  = 0;
    n_pass_mll[i]     = 0;
    n_pass_ss[i]      = 0;
    n_pass_os[i]      = 0;
    
    // SR1
    n_pass_SR1jv[i]   = 0;
    n_pass_SR1Zv[i]   = 0;
    n_pass_SR1MET[i]  = 0;
    
    // SR2
    n_pass_SR2jv[i]   = 0;
    n_pass_SR2MET[i]  = 0;
    
    // SR3
    n_pass_SR3ge2j[i] = 0;
    n_pass_SR3Zv[i]   = 0;
    n_pass_SR3bjv[i]  = 0;
    n_pass_SR3mct[i]  = 0;
    n_pass_SR3MET[i]  = 0;
    
    // SR4
    n_pass_SR4jv[i]        = 0;
    n_pass_SR4MET[i]       = 0;
    n_pass_SR4Zv[i]        = 0;
    n_pass_SR4L0pt[i]      = 0;
    n_pass_SR4SUMpt[i]     = 0;
    n_pass_SR4dPhiMETLL[i] = 0;
    n_pass_SR4dPhiMETL1[i] = 0;
    
    // SR5
    n_pass_SR5jv[i]    = 0;
    n_pass_SR5Zv[i]    = 0;
    n_pass_SR5MET[i]   = 0;
    n_pass_SR5MT2[i]   = 0;
  }

  m_nttools.setAnaType(AnalysisType::Ana_2Lep);

}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void Susy2LepCutflow::Begin(TTree* /*tree*/)
{
  SusyNtAna::Begin(0);
  if(m_dbg) cout << "Susy2LepCutflow::Begin" << endl;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t Susy2LepCutflow::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  SusyNtAna::clearObjects();
  m_ET = ET_Unknown;
  n_readin++;

  //if(!debugEvent()) return kTRUE;
  //cout<<"-----------------------------------"<<endl;
  //cout<<"RuN: "<<nt.evt()->run<<" Event "<<nt.evt()->event<<endl;

  // Chain entry not the same as tree entry
  //static Long64_t chainEntry = -1;
  m_chainEntry++;
  if(m_dbg || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->eventNumber << " ****" << endl;
  }

  // select signal objects
  SusyNtAna::selectObjects();
  //dumpBaselineObjects();
  //dumpSignalObjects();

  // Check that the event passes event cleaning cuts
  int flags = nt.evt()->cutFlags[NtSys::NOM];
  if(!passEventCleaning(flags, m_preMuons, m_baseMuons, m_baseJets)) return false;
  // Check that the event passes basic object selection
  if(!selectEvent(m_signalLeptons, m_baseLeptons)) return false;

  // Count SS and OS
  if(sameSign(m_signalLeptons))     n_pass_ss[m_ET]++;
  
  if(oppositeSign(m_signalLeptons)) n_pass_os[m_ET]++;
  
  // Check Signal regions
  passSR1(m_signalLeptons, m_signalJets, m_met);
  passSR2(m_signalLeptons, m_signalJets, m_met);
  passSR3(m_signalLeptons, m_signalJets, m_met);
  passSR4(m_signalLeptons, m_signalJets, m_met);
  passSR5(m_signalLeptons, m_signalJets, m_met);
    
  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void Susy2LepCutflow::Terminate()
{
  SusyNtAna::Terminate();
  if(m_dbg) cout << "Susy2LepCutflow::Terminate" << endl;

  dumpEventCounters();
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passEventCleaning(int flags, const MuonVector& preMuons, const MuonVector& baseMuons,
                                                const JetVector& baseJets)
{
  // grl
  if( !m_nttools.passGRL(flags) )               return false;
    n_pass_grl++;
  // noisy lar
  if( !m_nttools.passLarErr(flags) )            return false;
    n_pass_LAr++;
  // tile error
  if( !m_nttools.passTileErr(flags) )           return false;
    n_pass_tileErr++;
  // ttc veto
  if( !m_nttools.passTTC(flags) )               return false;
    n_pass_ttc++;
  // event has primary vertex
  if( !m_nttools.passGoodVtx(flags) )           return false;
    n_pass_goodVtx++;

  //////////////////////////////////////////////////////////
  // These cuts depend on the analysis' defition of baseline
  // muons (c.f. SusyNtuple/MuonSelector) and the specified 
  // overlap removal procedure (c.f. SusyNtuple/OverlapTools)
  // so cannot be stored as EventFlags when wrting SusyNt

  // veto events with "bad" muons
  if( !m_nttools.passBadMuon(preMuons) )      return false;
    n_pass_BadMuon++;
  // veto events with cosmic muons
  if( !m_nttools.passCosmicMuon(baseMuons) )  return false;
    n_pass_Cosmic++;
  // veto events with "bad" jets
  if( !m_nttools.passJetCleaning(baseJets) )  return false;
    n_pass_BadJet++;

  return true;

}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::selectEvent(const LeptonVector& leptons, const LeptonVector& baseLeps)
{
  if(!passNBaseLepCut(baseLeps))                return false;
  
  // Get Event Type to continue cutflow
  m_ET = getDiLepEvtType(baseLeps);
  if(m_ET == ET_me) m_ET = ET_em;
  
  //if( !passTrigger(baseLeps, m_met) )           return false;  
  n_pass_flavor[m_ET]++;
  if( !passNLepCut(leptons) )                   return false;
  if( !passMll(leptons) )                       return false;

  return true;
}
/*--------------------------------------------------------------------------------*/
// Signal Region 1 
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passSR1(const LeptonVector& leptons, const JetVector& jets, const Met *met)
{
  // Only for OS events
  if( !oppositeSign(leptons) )           return false;  

  // Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR1jv[m_ET]++;
  
  // Reject events with mll in Z window
  if( !passZVeto(leptons))               return false;
  n_pass_SR1Zv[m_ET]++;

  // Reject if Met_rel < 100
  if( !passMETRel(met,leptons,jets) )    return false;
  n_pass_SR1MET[m_ET]++;

  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passSR2(const LeptonVector& leptons, const JetVector& jets, const Met *met)
{
  // Only SS
  if( !sameSign(leptons) )               return false;
  
  // CHeck Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR2jv[m_ET]++;

  // Check MET rel > 100
  if( !passMETRel(met,leptons,jets) )    return false;
  n_pass_SR2MET[m_ET]++;

  return true;

}

/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passSR3(const LeptonVector& leptons, const JetVector& jets, const Met *met)
{

  // Only SFOS
  if( !oppositeSign(leptons) )           return false;
  if( !sameFlavor(leptons) )             return false;

  // Require at least 2 jets Pt > 30
  if( !passge2Jet(jets) )                return false;
  n_pass_SR3ge2j[m_ET]++;

  // Apply a Zveto
  if( !passZVeto(leptons) )              return false;
  n_pass_SR3Zv[m_ET]++;

  // Apply b jet veto
  if( !passbJetVeto(jets) )              return false;
  n_pass_SR3bjv[m_ET]++;

  // Veto top-tag events 
  if( !kin::passTopTag(leptons,jets,met) )    return false;
  n_pass_SR3mct[m_ET]++;

  // MetRel > 50
  if( !passMETRel(met,leptons,jets,50) ) return false;
  n_pass_SR3MET[m_ET]++;


  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passSR4(const LeptonVector& leptons, const JetVector& jets, const Met *met)
{

  // OS only
  if( !oppositeSign(leptons) )           return false;

  // Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR4jv[m_ET]++;

  // MetRel > 40
  if( !passMETRel(met,leptons,jets,40) ) return false;
  n_pass_SR4MET[m_ET]++;
  
  // Z Veto
  if( !passZVeto(leptons) )              return false;
  n_pass_SR4Zv[m_ET]++;

  // Leading lepton Pt > 50
  float pt0 = leptons.at(0)->Pt();
  if( pt0 < 50 )                         return false;
  n_pass_SR4L0pt[m_ET]++;
  
  // Sum of Pt > 100
  float pt1 = leptons.at(1)->Pt();
  if( pt0 + pt1 < 100 )                  return false;
  n_pass_SR4SUMpt[m_ET]++;
  
  // dPhi(met, ll) > 2.5
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = (*leptons.at(0) + *leptons.at(1));
  if( !passdPhi(metlv, ll, 2.5) )    return false;
  n_pass_SR4dPhiMETLL[m_ET]++;

  // dPhi(met, l1) > 0.5
  TLorentzVector l1 = *leptons.at(1);
  if( !passdPhi(metlv, l1, 0.5) )    return false;
  n_pass_SR4dPhiMETL1[m_ET]++;

  return true;

}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passSR5(const LeptonVector& leptons, const JetVector& jets, const Met *met)
{
  // For OS
  if( !oppositeSign(leptons) )          return false;  

  // Check Jet Veto
  if( !passJetVeto(jets) )              return false;
  n_pass_SR5jv[m_ET]++;

  // Check Z Veto
  if( !passZVeto(leptons) )             return false;
  n_pass_SR5Zv[m_ET]++;

  // Check METRel > 40
  if( !passMETRel(met,leptons,jets,40) ) return false;
  n_pass_SR5MET[m_ET]++;

  // Check MT2 > 90
  if( !passMT2(leptons, met, 90) )      return false;
  n_pass_SR5MT2[m_ET]++;
  
  return true;

}
/*--------------------------------------------------------------------------------*/
// Generic cuts
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passNLepCut(const LeptonVector& leptons)
{
  uint nLep = leptons.size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  n_pass_nLep[m_ET]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passNBaseLepCut(const LeptonVector& baseLeptons)
{
  if(m_cutNBaseLep){
    uint nLep = baseLeptons.size();
    if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
    if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passTrigger(const LeptonVector& leptons, const Met* met)
{
  
  if(leptons.size() < 1){
    n_pass_trig[m_ET]++;
    return true;
  }

  // this is basic example of using the SusyNtuple/Trigger class 
  // --> check that any of the single lepton triggers fired
  //     and make sure that all leptons are matched to a fired trigger
  
  // currently (as of June 8 2015) stored lepton triggers
  std::vector<std::string> single_ele = { "e5_etcut",          "e5_lhtight",            "e9_etcut",           "e9_lhtight",
                                          "e14_etcut",         "e20_medium",            "e24_medium1_iloose", "e24_loose1",
                                          "e24_tight_iloose",  "e25_lhvloose_L1EM15",   "e26_tight_iloose",
                                          "e28_tight1_iloose", "e30_etcut_L1EM15",      "e40_etcut_L1EM15",   "e60_medium",
                                          "e60_loose1",        "e60_medium1",           "e60_lhmedium"
                                        };
  std::vector<std::string> di_ele     = { "2e12_loose1",       "2e12_loose_L12EM10VH",  "2e17_loose1",        "2e17_lhloose" };
  std::vector<std::string> single_muo = { "mu4",  "mu6",          "mu10", "mu14", "mu18", 
                                          "mu20", "mu22",         "mu24", "mu24_imedium",
                                          "mu26", "mu26_imedium", "mu50", "mu60_0eta105_msonly"
                                        };
  std::vector<std::string> di_muo     = { "2mu4", "2mu6", "2mu10", "2mu14", "mu24_mu8noL1",
                                          "mu14_iloose_mu14", "mu20_imedium_mu8noL1", "mu20_iloose_mu8noL1"
                                        };
  std::vector<std::string> el_mu      = { "e17_loose_mu14", "e17_medium_mu12" };

  std::vector<std::string> lepton_triggers = single_ele;
  lepton_triggers.insert(lepton_triggers.end(), di_ele.begin(), di_ele.end());
  lepton_triggers.insert(lepton_triggers.end(), single_muo.begin(), single_muo.end());
  lepton_triggers.insert(lepton_triggers.end(), di_muo.begin(), di_muo.end());
  lepton_triggers.insert(lepton_triggers.end(), el_mu.begin(), el_mu.end());

  std::vector<std::string> single_lep_triggers = single_ele;
  single_lep_triggers.insert(single_lep_triggers.end(), single_muo.begin(), single_muo.end());

  TriggerTools &ntTrig = nttools().triggerTool();

  //////////////////////////////////////////////////////
  // check if any single lepton trigger has been fired
  //////////////////////////////////////////////////////
  bool trig_has_fired = false;
  std::vector<std::string> fired_triggers;
  for(unsigned int iTrig = 0; iTrig < single_lep_triggers.size(); iTrig++){
    std::string trig = "HLT_" + single_lep_triggers[iTrig];
    if(ntTrig.passTrigger(nt.evt()->trigBits, trig)) {
        trig_has_fired = true;
        fired_triggers.push_back(trig);
    }
  }
  //////////////////////////////////////////////////////
  // check to make sure that both leptons are matched 
  // to a fired trigger
  //////////////////////////////////////////////////////
  bool lep_1_ismatched = false;
  bool lep_2_ismatched = false;
  for(unsigned int iLep = 0; iLep < leptons.size(); iLep++) {
    Susy::Lepton* lep = leptons.at(iLep);
    for(unsigned int iTrig = 0; iTrig < fired_triggers.size(); iTrig++){
        std::string trig = fired_triggers[iTrig];
        if(ntTrig.passTrigger(lep->trigBits, trig) && iLep==0) lep_1_ismatched = true;
        if(ntTrig.passTrigger(lep->trigBits, trig) && iLep==1) lep_2_ismatched = true;
    }
  }
  if(trig_has_fired && lep_1_ismatched && lep_2_ismatched) {
    n_pass_trig[m_ET]++;
    return true;
  }
  else { return false; }

  ////int run         = nt.evt()->run;
  ////DataStream strm = nt.evt()->stream;
  ////if( m_trigObj->passDilTrig(leptons, run, strm) ){
  //if( m_trigObj->passDilTrig(leptons, met->Et, nt.evt()) ){
  //  n_pass_trig[m_ET]++;
  //  return true;
  //}
  //return false;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::sameFlavor(const LeptonVector& leptons)
{
  if( leptons.size() < 2 ) return false;
  return (leptons.at(0)->isMu() == leptons.at(1)->isMu());
  //return (leptons.at(0)->isEle() && leptons.at(1)->isEle());
  //return (leptons.at(0)->isMu() && leptons.at(1)->isMu());
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::oppositeFlavor(const LeptonVector& leptons)
{
  if( leptons.size() < 2 ) return false;
  return !(leptons.at(0)->isMu() == leptons.at(1)->isMu());
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::sameSign(const LeptonVector& leptons)
{
  if( leptons.size() < 2 ) return false;
  return leptons.at(0)->q * leptons.at(1)->q > 0;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::oppositeSign(const LeptonVector& leptons)
{
  if( leptons.size() < 2 ) return false;
  return leptons.at(0)->q * leptons.at(1)->q < 0;

}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passMll(const LeptonVector& leptons, float mll)
{
  if( leptons.size() < 2 ) return false;
  if( (*leptons.at(0) + *leptons.at(1)).M() < mll ) return false;
  n_pass_mll[m_ET]++;
  return true;
}
 
/*--------------------------------------------------------------------------------*/
// Signal region cuts
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passJetVeto(const JetVector& jets)
{
  bool failjet = false;
  for(uint i=0; i<jets.size(); ++i){
    const Jet* jet = jets.at(i);
    if( jet->Pt() < 20         ) continue;
    if( fabs(jet->Eta()) > 2.5 ) continue;
    if( jet->jvf < 0.75        ) continue;
    failjet = true;
    break;
  }
  
  if( failjet ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passbJetVeto(const JetVector& jets)
{
  bool hasbjet = false;
  for(uint i=0; i<jets.size(); ++i){
    const Jet* jet = jets.at(i);
    if( jet->mv2c20 > JetSelector::mv2c20_77efficiency())  // this is a dummy BJet tag
        hasbjet = true;
    break;
  }
  
  if( hasbjet ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passge2Jet(const JetVector& jets)
{
  // Excessive methods!!!!!! 
  return (jets.size() >= 2);
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passZVeto(const LeptonVector& leptons, float Zlow, float Zhigh)
{
  if( leptons.size() < 2 ) return false;
  float mll = (*leptons.at(0) + *leptons.at(1)).M();
  if( Zlow < mll && mll < Zhigh ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passMETRel(const Met *met, const LeptonVector& leptons, 
				 const JetVector& jets, float metMax){
  
  if( kin::getMetRel(met,leptons,jets) < metMax ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passdPhi(TLorentzVector v0, TLorentzVector v1, float cut)
{
  return v0.DeltaPhi(v1) > cut;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::passMT2(const LeptonVector& leptons, const Met* met, float cut)
{
  float mT2 = kin::getMT2(leptons, met);
  return (mT2 > cut);
}

/*--------------------------------------------------------------------------------*/
// Event counters
/*--------------------------------------------------------------------------------*/
void Susy2LepCutflow::dumpEventCounters()
{
  cout << endl;
  cout << "Susy2LepCutflow event counters"     << endl;
  cout << "read in        :" << n_readin       << endl;
  cout << "pass GRL       :" << n_pass_grl     << endl;
  cout << "pass LAr       :" << n_pass_LAr     << endl;
  cout << "pass tile err  :" << n_pass_tileErr << endl;
  cout << "pass ttc veto  :" << n_pass_ttc     << endl;
  cout << "pass primVtx   :" << n_pass_goodVtx << endl;
  cout << "pass BadMu     :" << n_pass_BadMuon << endl;
  cout << "pass Cosmic    :" << n_pass_Cosmic  << endl;
  cout << "pass BadJet    :" << n_pass_BadJet  << endl;

  string v_ET[ET_N] = {"ee","mm","em","Unknown"};
  for(int i=0; i<ET_N; ++i){
    cout << "************************************" << endl;
    cout << "For dilepton type: " << v_ET[i]       << endl;
    
    cout << "pass trig:     " << n_pass_trig[i]    << endl;
    cout << "pass SF:       " << n_pass_flavor[i]  << endl;
    cout << "pass nLep:     " << n_pass_nLep[i]    << endl;
    cout << "pass mll:      " << n_pass_mll[i]     << endl;
    cout << "pass OS:       " << n_pass_os[i]      << endl;
    cout << "pass SS:       " << n_pass_ss[i]      << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR1 JV:   " << n_pass_SR1jv[i]   << endl;
    cout << "pass SR1 ZV:   " << n_pass_SR1Zv[i]   << endl;
    cout << "pass SR1 MET:  " << n_pass_SR1MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR2 JV:   " << n_pass_SR2jv[i]   << endl;
    cout << "pass SR2 MET:  " << n_pass_SR2MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR3 >=2j: " << n_pass_SR3ge2j[i] << endl;
    cout << "pass SR3 ZV:   " << n_pass_SR3Zv[i]   << endl;
    cout << "pass SR3 bV:   " << n_pass_SR3bjv[i]  << endl;
    cout << "pass SR3 mct:  " << n_pass_SR3mct[i]  << endl;
    cout << "pass SR3 MET:  " << n_pass_SR3MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR4 JV:           " << n_pass_SR4jv[i]        << endl;
    cout << "pass SR4 MET:          " << n_pass_SR4MET[i]       << endl;
    cout << "pass SR4 ZV:           " << n_pass_SR4Zv[i]        << endl;
    cout << "pass SR4 l0 Pt:        " << n_pass_SR4L0pt[i]      << endl;
    cout << "pass SR4 Sum Pt:       " << n_pass_SR4SUMpt[i]     << endl;
    cout << "pass SR4 dPhi(Met,ll): " << n_pass_SR4dPhiMETLL[i] << endl;
    cout << "pass SR4 dPhi(Met,l1): " << n_pass_SR4dPhiMETL1[i] << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR5 JV:   " << n_pass_SR5jv[i]   << endl;
    cout << "pass SR5 ZV:   " << n_pass_SR5Zv[i]   << endl;
    cout << "pass SR5 MET:  " << n_pass_SR5MET[i]  << endl;
    cout << "pass SR Mt2:   " << n_pass_SR5MT2[i]  << endl;
  }

}

/*--------------------------------------------------------------------------------*/
// Debug event
/*--------------------------------------------------------------------------------*/
bool Susy2LepCutflow::debugEvent()
{
  uint run = nt.evt()->run;
  uint evt = nt.evt()->eventNumber;
  //if(run==191139 && evt==140644832) return true;
  //if(run==180164&&evt==24769) return true;
  //if(run==189751 && evt==7934) return true;
  if(run==178109 && evt==65564070) return true;
  if(run==178109 && evt==15017138) return true;
  return false;
}
