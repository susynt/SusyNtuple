#include <set>

#include "TFile.h"
#include "TKey.h"
#include "TChainElement.h"
#include "TH1F.h"
#include "TSystem.h"

#include "Mt2/mt2_bisect.h" 

#include "SusyNtuple/SusyNtTools.h"

#include <cassert>

using namespace std;
using namespace Susy;

// TODO: implement a feature for sharing the tool, rather than making it static
BTagCalib* SusyNtTools::m_btagTool = NULL;

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() : 
        m_anaType(Ana_3Lep),
        m_doPtconeCut(true),
        m_doElEtconeCut(true),
        m_doMuEtconeCut(false),
        m_doIPCut(true)
	//m_btagTool(NULL)
{
  m_jvfTool = new JVFUncertaintyTool();
  m_jvfTool->UseGeV(true);
}

/*--------------------------------------------------------------------------------*/
// Method to configure bTag SF tool.  Necessary since 2L and 3L differ
// on whether or not JVF is used.  Also allow for different operating points
// to be toggled                                                            
/*--------------------------------------------------------------------------------*/
void SusyNtTools::configureBTagTool(string OP, float opVal, bool isJVF)
{
  // Initialize b-tag tool
  string rootcoredir = getenv("ROOTCOREBIN");
  string calibration = gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/BTagCalibration.env");
  string calibFolder = gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools/");
  m_btagTool = new BTagCalib("MV1", calibration, calibFolder, OP, isJVF, opVal);
}

/*--------------------------------------------------------------------------------*/
// Method to configure JVF uncertainty tool
/*--------------------------------------------------------------------------------*/
/*void SusyNtTools::configureJVFTool(string jetAlgo)
{
  m_jvfTool = new JVFUncertaintyTool(jetAlgo.c_str());
}*/

/*--------------------------------------------------------------------------------*/
// Get event weight, combine gen, pileup, xsec, and lumi weights
// Default weight uses A-D lumi
// You can supply a different luminosity, but the pileup weights will still correspond to A-D
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeight(const Event* evt, float lumi, 
                                  bool useSumwMap, const SumwMap* sumwMap,
                                  bool useProcSumw, bool useSusyXsec, 
                                  MCWeighter::WeightSys sys)
{
  // Method is deprecated
  static uint warningCount = 0;
  if(warningCount < 50){
    cout << "SusyNtTools::getEventWeight - WARNING - This method is deprecated. "
         << "You should update your code to use the MCWeighter class, as demonstrated "
         << "in Susy3LepCutflow" << endl;
    warningCount++;
    if(warningCount == 50){
      cout << "Warning was printed 50 times, and will now stop" << endl;
    }
  }
  if(!evt->isMC) return 1;
  else{
    float sumw = getSumw(evt, sumwMap, useSumwMap, useProcSumw);
    float xsec = getXsecTimesEff(evt, useSusyXsec, sys);
    float pupw = getPileupWeight(evt, sys);
    return evt->w * pupw * xsec * lumi / sumw;
  }
}

/*--------------------------------------------------------------------------------*/
// Get the sumw for this event
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getSumw(const Event* evt, const SumwMap* sumwMap, bool useSumwMap, bool useProcSumw)
{
  float sumw = evt->sumw;
  if(useSumwMap){
    if(sumwMap != NULL){
      // Map key is pair(mcid, proc)
      unsigned int mcid = evt->mcChannel;
      int procID = useProcSumw? evt->susyFinalState : 0;
      // Correct for procID == -1
      if(procID < 0) procID = 0;
      SumwMapKey key(mcid, procID);
      SumwMap::const_iterator sumwMapIter = sumwMap->find(key);
      if(sumwMapIter != sumwMap->end()) sumw = sumwMapIter->second;
      else{
        cout << "SusyNtTools::getEventWeight - ERROR - requesting to use sumw map but "
             << "mcid " << mcid << " proc " << procID << " not found!" << endl;
        abort(); // better to return a default, like -1?
      }
    }
    else{
      cout << "SusyNtTools::getEventWeight - ERROR - sumw map is NULL!" << endl;
      abort(); // better to return a default, like -1?
    }
  }
  return sumw;
}

/*--------------------------------------------------------------------------------*/
// Get the SUSYTools cross section for this sample
/*--------------------------------------------------------------------------------*/
SUSY::CrossSectionDB::Process SusyNtTools::getCrossSection(const Event* evt)
{
  using namespace SUSY;
  // Use one DB and map for all instances of this class
  typedef pair<int, int> intpair;
  typedef map<intpair, CrossSectionDB::Process> XSecMap;
  static string xsecDir = string(getenv("ROOTCOREBIN")) + "/data/SUSYTools/mc12_8TeV/";
  static CrossSectionDB xsecDB(xsecDir);
  static XSecMap xsecCache;
  if(evt->isMC){
    // SUSYTools expects 0 as default value, but we have existing tags with default of -1
    int proc = evt->susyFinalState > 0? evt->susyFinalState : 0;
    // Temporary bugfix for Wh nohadtau in n0150
    if(evt->mcChannel >= 177501 && evt->mcChannel <= 177528) proc = 125;
    const intpair k(evt->mcChannel, proc);
    // Check to see if we've cached this process yet.
    XSecMap::const_iterator iter = xsecCache.find(k);
    if(iter != xsecCache.end()){
      return iter->second;
    }
    else{
      // Hasn't been cached yet, load it from the DB
      CrossSectionDB::Process p = xsecDB.process(evt->mcChannel, proc);
      xsecCache[k] = p;
      return p;
    }
  }
  return CrossSectionDB::Process();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getXsecTimesEff(const Event* evt, bool useSusyXsec, MCWeighter::WeightSys sys)
{
  float xsec = evt->xsec;
  if(useSusyXsec){
    SUSY::CrossSectionDB::Process p = getCrossSection(evt);
    xsec = p.xsect() * p.kfactor() * p.efficiency();
    if(sys==MCWeighter::Sys_XSEC_UP)
      xsec *= (1. + p.relunc());
    else if(sys==MCWeighter::Sys_XSEC_DN)
      xsec *= (1. - p.relunc());
  }
  return xsec;
}
/*--------------------------------------------------------------------------------*/
// Get the pileup weight for this event
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getPileupWeight(const Event* evt, MCWeighter::WeightSys sys)
{
  if(sys==MCWeighter::Sys_PILEUP_UP) return evt->wPileup_up;
  else if(sys==MCWeighter::Sys_PILEUP_DN) return evt->wPileup_dn;
  else return evt->wPileup;
}

/*--------------------------------------------------------------------------------*/
// Full object selection methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt, 
                                     ElectronVector& preElecs, MuonVector& preMuons, JetVector& preJets,
                                     ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets, 
                                     SusyNtSys sys, bool selectTaus)
{
  // Preselection
  preElecs = getPreElectrons(susyNt, sys);
  preMuons = getPreMuons(susyNt, sys);
  preJets  = getPreJets(susyNt, sys);
  if(selectTaus) taus = getPreTaus(susyNt, sys);
  else taus.clear();

  // Baseline objects
  elecs = preElecs;
  muons = preMuons;
  jets  = preJets;

  // Overlap removal
  performOverlap(elecs, muons, taus, jets);

  // Remove MSFOS < 12 GeV
  removeSFOSPair(elecs, MLL_MIN);
  removeSFOSPair(muons, MLL_MIN);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt, ElectronVector& elecs,
                                     MuonVector& muons, TauVector& taus, JetVector& jets, 
                                     SusyNtSys sys, bool selectTaus)
{
  // Get all objects
  elecs = getPreElectrons(susyNt, sys);
  muons = getPreMuons(susyNt, sys);
  jets  = getPreJets(susyNt, sys);
  if(selectTaus) taus = getPreTaus(susyNt, sys);
  else taus.clear();

  //cout<<"Select Taus: "<<selectTaus<<" size: "<<taus.size()<<endl;

  // Now perform the overlap removals
  performOverlap(elecs, muons, taus, jets);

  // Do SFOS removal for Mll < 12 
  removeSFOSPair(elecs, MLL_MIN);
  removeSFOSPair(muons, MLL_MIN);
  // TODO: revisit this??
  //removeSFOSPair(taus, MLL_MIN);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                   const TauVector& baseTaus, const JetVector& baseJets,
				   ElectronVector& sigElecs, MuonVector& sigMuons, 
                                   TauVector& sigTaus, JetVector& sigJets, JetVector& sigJets2Lep, 
                                   uint nVtx, bool isMC, bool removeLepsFromIso, 
                                   TauID tauJetID, TauID tauEleID, TauID tauMuoID,
                                   SusyNtSys sys)
{
  // Set signal objects
  sigElecs = getSignalElectrons(baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso);
  sigMuons = getSignalMuons(baseMuons, baseElecs, nVtx, isMC, removeLepsFromIso);
  sigTaus  = getSignalTaus(baseTaus, tauJetID, tauEleID, tauMuoID);
  sigJets  = getSignalJets(baseJets, sys);
  sigJets2Lep = getSignalJets2Lep(baseJets, sys);
}
/*--------------------------------------------------------------------------------*/
// New signal tau prescription, fill both ID levels at once!
void SusyNtTools::getSignalObjects(const ElectronVector& baseElecs, const MuonVector& baseMuons,
                                   const TauVector& baseTaus, const JetVector& baseJets,
				   ElectronVector& sigElecs, MuonVector& sigMuons, 
                                   TauVector& mediumTaus, TauVector& tightTaus,
                                   JetVector& sigJets, JetVector& sigJets2Lep, 
                                   uint nVtx, bool isMC, bool removeLepsFromIso,
                                   SusyNtSys sys)
{
  // Set signal objects
  sigElecs = getSignalElectrons(baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso);
  sigMuons = getSignalMuons(baseMuons, baseElecs, nVtx, isMC, removeLepsFromIso);
  sigJets  = getSignalJets(baseJets, sys);
  sigJets2Lep = getSignalJets2Lep(baseJets, sys);

  getSignalTaus(baseTaus, mediumTaus, tightTaus);
}
/*--------------------------------------------------------------------------------*/
// This method cannot be used anymore. 
// Analyzers must store the baseline objects for cleaning cuts!
/*--------------------------------------------------------------------------------*/
/*void SusyNtTools::getSignalObjects(SusyNtObject* susyNt, ElectronVector& sigElecs, 
				   MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets, 
                                   JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC, 
                                   bool selectTaus, bool removeLepsFromIso, TauID tauID)
{
  // Temporary vectors for baseline objects
  ElectronVector elecs = getPreElectrons(susyNt, sys);
  MuonVector     muons = getPreMuons(susyNt, sys);
  JetVector      jets  = getPreJets(susyNt, sys);

  TauVector      taus;
  if(selectTaus) taus = getPreTaus(susyNt, sys);

  // Perform overlap removal
  performOverlap(elecs, muons, taus, jets);

  // Now set the signal objects
  sigElecs = getSignalElectrons(elecs, muons, nVtx, isMC, removeLepsFromIso);
  sigMuons = getSignalMuons(muons, elecs, nVtx, isMC, removeLepsFromIso);
  sigTaus  = getSignalTaus(taus, tauID);
  sigJets  = getSignalJets(jets);
  sigJets2Lep = getSignalJets2Lep(jets);
}*/

/*--------------------------------------------------------------------------------*/
void SusyNtTools::performOverlap(ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets)
{
  // Remove electrons from electrons
  e_e_overlap(elecs, E_E_DR);
  // Remove jets from electrons
  e_j_overlap(elecs, jets, J_E_DR, true);
  // Remove taus from electrons
  t_e_overlap(taus, elecs, T_E_DR);
  // Remove taus from muons
  t_m_overlap(taus, muons, T_M_DR);
  // Remove electrons from jets
  e_j_overlap(elecs, jets, E_J_DR, false);
  // Remove muons from jets
  m_j_overlap(muons, jets, M_J_DR);
  // Remove electrons and muons that overlap
  e_m_overlap(elecs, muons, E_M_DR);
  // Remove muons from muons
  m_m_overlap(muons, M_M_DR);
  // Remove jets from taus
  t_j_overlap(taus, jets, J_T_DR, true);
}

/*--------------------------------------------------------------------------------*/
// Methods to grab the Baseline objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getPreElectrons(SusyNtObject* susyNt, SusyNtSys sys)
{
  // Not sure if I want to pass SusyNt object around or not... but just do it this way
  // for now for lack of a more creative idea.
  ElectronVector elecs;
  for(uint ie=0; ie<susyNt->ele()->size(); ++ie){
    Electron* e = & susyNt->ele()->at(ie);
    e->setState(sys);
    
    // Apply any additional Selection
    if(e->Pt() < ELECTRON_PT_CUT) continue;

    // Save
    elecs.push_back(e);
  }

  return elecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getPreMuons(SusyNtObject* susyNt, SusyNtSys sys)
{
  MuonVector muons;
  for(uint im=0; im<susyNt->muo()->size(); ++im){
    Muon* mu = & susyNt->muo()->at(im);
    mu->setState(sys);

    // Apply any additional selection
    if(mu->Pt() < MUON_PT_CUT) continue;
    
    muons.push_back(mu);
  }

  return muons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getPreTaus(SusyNtObject* susyNt, SusyNtSys sys)
{
  TauVector taus;
  for(uint iTau=0; iTau < susyNt->tau()->size(); iTau++){
    Tau* tau = & susyNt->tau()->at(iTau);
    tau->setState(sys);

    // Apply any additional selection
    if(isSelectTau(tau)) taus.push_back(tau);
  }
  return taus;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getPreJets(SusyNtObject* susyNt, SusyNtSys sys)
{
  JetVector jets;
  for(uint ij=0; ij<susyNt->jet()->size(); ++ij){
    Jet* j = & susyNt->jet()->at(ij);
    j->setState(sys);

    // Apply any additional selection
    // At least jets of 20 GeV
    if(j->Pt() < JET_PT_CUT) continue;
    jets.push_back(j);
  }
  
  return jets;
}

/*--------------------------------------------------------------------------------*/
// Get Signal objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getSignalElectrons(const ElectronVector& baseElecs, const MuonVector& baseMuons, 
                                               uint nVtx, bool isMC, bool removeLepsFromIso)
{
  ElectronVector sigElecs;
  for(uint ie=0; ie<baseElecs.size(); ++ie){
    Electron* e = baseElecs.at(ie);
    if(isSignalElectron(e, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)){
      sigElecs.push_back(e);
    }
  }

  return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(const MuonVector& baseMuons, const ElectronVector& baseElecs, 
                                       uint nVtx, bool isMC, bool removeLepsFromIso)
{
  MuonVector sigMuons;
  for(uint im=0; im<baseMuons.size(); ++im){
    Muon* mu = baseMuons.at(im);
    if(isSignalMuon(mu, baseElecs, baseMuons, nVtx, isMC, removeLepsFromIso)){
      sigMuons.push_back(mu);
    }
  }

  return sigMuons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getSignalTaus(const TauVector& baseTaus, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
  TauVector sigTaus;
  for(uint iTau=0; iTau < baseTaus.size(); iTau++){
    Tau* tau = baseTaus[iTau];
    if(isSignalTau(tau, tauJetID, tauEleID, tauMuoID)){
      sigTaus.push_back(tau);
    }
  }
  return sigTaus;
}
/*--------------------------------------------------------------------------------*/
// New signal tau prescription, fill both ID levels at once!
void SusyNtTools::getSignalTaus(const TauVector& baseTaus, TauVector& mediumTaus, TauVector& tightTaus)
{
  for(uint iTau=0; iTau < baseTaus.size(); iTau++){
    Tau* tau = baseTaus[iTau];
    // For now, tight taus are defined as all tight criteria
    if(isSignalTau(tau, TauID_tight, TauID_tight, TauID_tight)){
      tightTaus.push_back(tau);
      mediumTaus.push_back(tau);
    }
    // For now, medium taus use loose ele BDT
    else if(isSignalTau(tau, TauID_medium, TauID_loose, TauID_medium)) mediumTaus.push_back(tau);
  }
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(const JetVector& baseJets, SusyNtSys sys)
{
  JetVector sigJets;
  for(uint ij=0; ij<baseJets.size(); ++ij){
    Jet* j = baseJets.at(ij);
    if(isSignalJet(j, sys)){
      sigJets.push_back(j);
    }
  }
  return sigJets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets2Lep(const JetVector& baseJets, SusyNtSys sys)
{
  JetVector sigJets;
  for(uint ij=0; ij<baseJets.size(); ++ij){
    Jet* j = baseJets.at(ij);
    if(isSignalJet2Lep(j, sys)){
      sigJets.push_back(j);
    }
  }
  return sigJets;
}
/*--------------------------------------------------------------------------------*/
PhotonVector SusyNtTools::getSignalPhotons(SusyNtObject* susyNt)
{
  // Currently only storing signal photons, so just a conv way to get them.
  PhotonVector sigPho;
  for(uint ip=0; ip<susyNt->pho()->size(); ip++){
    sigPho.push_back( &(susyNt->pho()->at(ip)) );
  }

  return sigPho;  
}

/*--------------------------------------------------------------------------------*/
// Get Met
/*--------------------------------------------------------------------------------*/
Met* SusyNtTools::getMet(SusyNtObject* susyNt, SusyNtSys sys)//, bool useNomPhiForMetSys)
{
  // Right now not being clever. Could maybe make sys index correspond to 
  // index on the array.
  
  Met* met = NULL;
  vector<Met>* metTmp = susyNt->met();
  for(uint i=0; i<metTmp->size(); i++){    
    if(metTmp->at(i).sys == sys){
      met = &(metTmp->at(i));
    
      // NEW: For NtSys_SCALEST_UP/NtSys_SCALEST_DOWN use nominal phi
      // Mar 1, 2013 -- No longer recommeneded, left over from HCP.
      //if(useNomPhiForMetSys && (sys == NtSys_SCALEST_UP || 
      //sys == NtSys_SCALEST_DN ||
      //sys == NtSys_RESOST)){
      //for(uint j=0; j<metTmp->size(); ++j){
      //if(metTmp->at(j).sys == NtSys_NOM){
      //met->phi = metTmp->at(j).phi;
      //break;
      //}
      //}
      //}

      return met;
    }
  }
  if(!met){
    cout << "Error: Unable to find met for given systematic!  Returning NULL!! " << sys << endl;
  }
  
  return met;
}

/*--------------------------------------------------------------------------------*/
// Method for applying tau BDT selection only
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isTauBDT(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
  // Jet BDT cut
  if(tauJetID == TauID_loose){
    if(tau->jetBDTSigLoose != 1) return false;
  } else if(tauJetID == TauID_medium){
    if(tau->jetBDTSigMedium != 1) return false;
  } else if(tauJetID == TauID_tight){
    if(tau->jetBDTSigTight != 1) return false;
  }

  // Ele BDT cut
  if(tauEleID == TauID_loose){
    if(tau->nTrack == 1 && tau->eleBDTLoose != 0) return false;
  } else if(tauEleID == TauID_medium){
    if(tau->nTrack == 1 && tau->eleBDTMedium != 0) return false;
  } else if(tauEleID == TauID_tight){
    if(tau->nTrack == 1 && tau->eleBDTTight != 0) return false;
  }

  // Muon veto
  if(tauMuoID >= TauID_medium){
    if(tau->muonVeto != 0) return false;
  }

  // none selection can be used for container taus
  //else if(id == TauID_none){
  //}
  //else{
  //  cout << "SusyNtTools::WARNING - tau ID " << id << " not supported!" << endl;
  //  return false;
  //}

  return true;
}

/*--------------------------------------------------------------------------------*/
// Check if lepton is selected
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSelectTau(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
  // At the moment, we only apply pt and BDT here.
  // TODO: Make the pt cut configurable
  if(tau->Pt() < TAU_PT_CUT) return false;
  return isTauBDT(tau, tauJetID, tauEleID, tauMuoID);
}

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalLepton(const Lepton* l, 
                                 const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                 uint nVtx, bool isMC, bool removeLepsFromIso)
{
  if(l->isEle()) return isSignalElectron( (Electron*) l, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
  else           return isSignalMuon( (Muon*) l, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso );
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* ele, 
                                   const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                   uint nVtx, bool isMC, bool removeLepsFromIso)
{
  if(!ele->tightPP) return false;

  // Impact parameter
  if(m_doIPCut){
    // All ana using unbiased IP cuts
    //if(fabs(ele->d0Sig(m_anaType == Ana_2Lep)) >= ELECTRON_D0SIG_CUT) return false;
    //if(fabs(ele->z0SinTheta(m_anaType == Ana_2Lep)) >= ELECTRON_Z0_SINTHETA_CUT) return false;
    float maxD0Sig = (m_anaType != Ana_2LepWH  ? ELECTRON_D0SIG_CUT : ELECTRON_D0SIG_CUT_WH);
    if(fabs(ele->d0Sig()) >= maxD0Sig) return false;
    if(fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;
  }

  float pt = ele->Pt();

  // Relative ptcone iso
  if(m_doPtconeCut){ // true by default
    float ptcone30 = elPtConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
    if(m_anaType == Ana_2LepWH){
      if(ptcone30/std::min(pt,ELECTRON_ISO_PT_THRS) >=  ELECTRON_PTCONE30_PT_WH_CUT) return false;
    }
    else
      if(ptcone30/pt >= ELECTRON_PTCONE30_PT_CUT) return false;
  }

  // Topo etcone isolation cut
  if(m_doElEtconeCut){ // true by default
    float etcone = elEtTopoConeCorr(ele, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
     if(m_anaType == Ana_2LepWH){
       if(etcone/std::min(pt,ELECTRON_ISO_PT_THRS) >= ELECTRON_TOPOCONE30_PT_WH_CUT) return false;
     }
     else 
       if(etcone/pt >= ELECTRON_TOPOCONE30_PT_CUT) return false;
  }

  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* mu, 
                               const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                               uint nVtx, bool isMC, bool removeLepsFromIso)
{
  // Impact parameter
  if(m_doIPCut){
    // All ana using unbiased IP
    //if(fabs(mu->d0Sig(m_anaType == Ana_2Lep)) >= MUON_D0SIG_CUT) return false;
    //if(fabs(mu->z0SinTheta(m_anaType == Ana_2Lep)) >= MUON_Z0_SINTHETA_CUT) return false;
    if(fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
    if(fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;
  }

  // ptcone isolation cut with pileup correction
  if(m_doPtconeCut){ // true by default
    if(m_anaType == Ana_3Lep){
      float ptcone30 = muPtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
      if(ptcone30/mu->Pt() >= MUON_PTCONE30_PT_CUT) return false;
    }
    else{
      float ptcone30 = mu->ptcone30ElStyle; // no corrections at the moment
      float pt = mu->Pt();
      if(m_anaType == Ana_2LepWH){
	if(ptcone30/std::min(pt,MUON_ISO_PT_THRS) >= MUON_PTCONE30ELSTYLE_PT_WH_CUT) return false;
      }
      else 
	if(ptcone30/mu->Pt() >= MUON_PTCONE30ELSTYLE_PT_CUT) return false;
    }
  }

  // etcone isolation cut - not applied by default, but here for studies
  if(m_doMuEtconeCut){ // FALSE by default
    float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
    if(m_doMuEtconeCut && etcone30/mu->Pt() >= MUON_ETCONE30_PT_CUT) return false;
  } else if(m_anaType == Ana_2LepWH) {
    float etcone30 = muEtConeCorr(mu, baseElectrons, baseMuons, nVtx, isMC, removeLepsFromIso);
    float pt = mu->Pt();
    if(pt==0.0 || (etcone30/std::min(pt,MUON_ISO_PT_THRS) >= MUON_ETCONE30_PT_WH_CUT)) return false;    
  }

  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalTau(const Tau* tau, TauID tauJetID, TauID tauEleID, TauID tauMuoID)
{
  // At the moment, signal taus only use additional BDT selection
  return isTauBDT(tau, tauJetID, tauEleID, tauMuoID);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSemiSignalElectron(const Electron* ele)
{
  if(!ele->tightPP) return false;
  // Impact parameter
  if(m_doIPCut){
    if(fabs(ele->d0Sig()) >= ELECTRON_D0SIG_CUT) return false;
    if(fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSemiSignalMuon(const Muon* mu)
{
  // Impact parameter
  if(m_doIPCut){
    if(fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
    if(fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
// Isolation corrections
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elPtConeCorr(const Electron* e, 
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                uint /*nVtx*/, bool /*isMC*/, bool removeLeps)
{
  float ptcone = e->ptcone30;
  if(removeLeps){
    for(uint iEl=0; iEl<baseElectrons.size(); iEl++){
      const Electron* e2 = baseElectrons[iEl];
      if(e==e2) continue;
      if( !isSemiSignalElectron(e2) ) continue;
      float dR = e->DeltaR(*e2);
      if(dR < 0.3) ptcone -= e2->trackPt;
    }
    for(uint iMu=0; iMu<baseMuons.size(); iMu++){
      const Muon* mu = baseMuons[iMu];
      if( !isSemiSignalMuon(mu) ) continue;
      float dR = e->DeltaR(*mu);
      if(dR < 0.3) ptcone -= mu->idTrackPt;
    }
  }
  return ptcone;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elEtTopoConeCorr(const Electron* e, 
                                    const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                    uint nVtx, bool isMC, bool removeLeps)
{
  float slope = isMC? ELECTRON_TOPOCONE30_SLOPE_MC : ELECTRON_TOPOCONE30_SLOPE_DATA;
  float etcone = e->topoEtcone30Corr - slope*nVtx;
  if(removeLeps){
    for(uint iEl=0; iEl<baseElectrons.size(); iEl++){
      const Electron* e2 = baseElectrons[iEl];
      if(e==e2) continue;
      if( !isSemiSignalElectron(e2) ) continue;
      float dR = e->DeltaR(*e2);
      if(dR < 0.28) etcone -= e2->clusE / cosh(e2->clusEta);
    }
  }
  return etcone;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muPtConeCorr(const Muon* mu, 
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                uint nVtx, bool isMC, bool removeLeps)
{
  float slope = isMC? MUON_PTCONE30_SLOPE_MC : MUON_PTCONE30_SLOPE_DATA;
  float ptcone = mu->ptcone30 - slope*nVtx;
  if(removeLeps){
    for(uint iEl=0; iEl<baseElectrons.size(); iEl++){
      const Electron* e = baseElectrons[iEl];
      if( !isSemiSignalElectron(e) ) continue;
      float dR = mu->DeltaR(*e);
      if(dR < 0.3) ptcone -= e->trackPt;
    }
    for(uint iMu=0; iMu<baseMuons.size(); iMu++){
      const Muon* mu2 = baseMuons[iMu];
      if(mu==mu2) continue;
      if( !isSemiSignalMuon(mu2) ) continue;
      float dR = mu->DeltaR(*mu2);
      if(dR < 0.3) ptcone -= mu2->idTrackPt;
    }
  }
  return ptcone;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muEtConeCorr(const Muon* mu, 
                                const ElectronVector& baseElectrons, const MuonVector& baseMuons, 
                                uint nVtx, bool isMC, bool removeLeps)
{
  float k1 = isMC? MUON_ETCONE30_K1_MC : MUON_ETCONE30_K1_DATA;
  float k2 = isMC? MUON_ETCONE30_K2_MC : MUON_ETCONE30_K2_DATA;
  float etcone = mu->etcone30 - k1*nVtx - k2*nVtx*nVtx;
  if(removeLeps){
    for(uint iEl=0; iEl<baseElectrons.size(); iEl++){
      const Electron* e = baseElectrons[iEl];
      if( !isSemiSignalElectron(e) ) continue;
      float dR = mu->DeltaR(*e);
      if(dR < 0.28) etcone -= e->clusE / cosh(e->clusEta);
    }
  }
  return etcone;
}

/*--------------------------------------------------------------------------------*/
// Signal jet selection
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet(const Jet* jet, SusyNtSys sys)
{
  // For now, 2lep analysis is not using this jet definition
  //float ptCut = m_anaType==Ana_2Lep? JET_SIGNAL_PT_CUT_2L : JET_SIGNAL_PT_CUT_3L;
    bool pass = false;
    if(jet) {
        float ptCut = JET_SIGNAL_PT_CUT_3L;
        pass = (jet->Pt() > ptCut
                && fabs(jet->Eta()) < JET_ETA_CUT
                && SusyNtTools::jetPassesJvfRequirement(jet, m_jvfTool, JET_JVF_PT, JET_JVF_ETA, JET_JVF_CUT, sys, m_anaType));
    } else {
        cout << "isSignalJet: invalid jet(" << jet << "), return " << pass << endl;
    }
    return pass;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Signal Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet2Lep(const Jet* jet, SusyNtSys sys)
{
    return (isCentralLightJet(jet, m_jvfTool, sys, m_anaType)
            || isCentralBJet(jet)
            || isForwardJet(jet));
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Central Light Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isCentralLightJet(const Susy::Jet* jet, JVFUncertaintyTool* jvfTool, SusyNtSys sys, AnalysisType anaType)
{
    // This function is mostly used by the 2L analyses. Needs to be reorganized...
    bool pass = false;
    if(jet) {
        float minJvf = JET_JVF_CUT;
        float maxJvtEta = JET_JVF_ETA;
        if(anaType == Ana_2Lep || anaType == Ana_2LepWH) {
            minJvf = JET_JVF_CUT_2L;
            maxJvtEta = JET_ETA_CUT_2L;
        }
        pass = (jet->Pt() > JET_PT_L20_CUT
                && fabs(jet->detEta) < JET_ETA_CUT_2L
                && jet->mv1 < MV1_80
                && SusyNtTools::jetPassesJvfRequirement(jet, jvfTool, JET_JVF_PT, maxJvtEta, minJvf, sys, anaType));
    } else {
        cout << "isCentralLightJet: invalid jet(" << jet << "), return " << pass << endl;
    }
    return pass;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton B Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isCentralBJet(const Jet* jet)
{
  if(jet->Pt() < JET_PT_B20_CUT) return false;
  if(fabs(jet->detEta) > JET_ETA_CUT_2L) return false;
  if(jet->mv1 < MV1_80) return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Forward Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isForwardJet(const Jet* jet)
{
  if(jet->Pt() < JET_PT_F30_CUT         ) return false;
  //if(fabs(jet->Eta()) < JET_ETA_CUT_2L  ) return false; 
  //if(fabs(jet->Eta()) > JET_ETA_MAX_CUT ) return false;
  if(fabs(jet->detEta) < JET_ETA_CUT_2L  ) return false; 
  if(fabs(jet->detEta) > JET_ETA_MAX_CUT ) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Count Number of 2 Lepton Central Light Jets
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCLJets(const JetVector& jets, JVFUncertaintyTool* jvfTool,
                                SusyNtSys sys, AnalysisType anaType)
{
  int ans = 0;

  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* j = jets.at(ij);
    if(isCentralLightJet(j, jvfTool, sys, anaType)){
      ans++;
    }
  }

  return ans;
}

/*--------------------------------------------------------------------------------*/
// Count Number of 2 Lepton B Jets
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCBJets(const JetVector& jets)
{
  int ans = 0;

  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* j = jets.at(ij);
    if(isCentralBJet(j)){
      ans++;
    }
  }

  return ans;
}

/*--------------------------------------------------------------------------------*/
// Count Number of 2 Lepton Forward Jets
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfFJets(const JetVector& jets)
{
  int ans = 0;

  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* j = jets.at(ij);
    if(isForwardJet(j)){
      ans++;
    }
  }

  return ans;
}

/*--------------------------------------------------------------------------------*/
// Pass 2 Lepton Jet Counts by Reference
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getNumberOf2LepJets(const JetVector& jets, int& Ncl, int& Ncb, int& Nf,
                                      SusyNtSys sys, AnalysisType anaType)
{
  Ncl = numberOfCLJets(jets, m_jvfTool, sys, anaType);
  Ncb = numberOfCBJets(jets);
  Nf  = numberOfFJets (jets);
  return;
}
/*--------------------------------------------------------------------------------*/
// Check for jet in bad FCAL region
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasJetInBadFCAL(const JetVector& baseJets, uint run, bool isMC)
{
  // Only applied to data in periods C1 to C8
  if(isMC || run<206248 || run>207332) return false;
  for(uint iJ=0; iJ<baseJets.size(); ++iJ){
    const Jet* j = baseJets.at(iJ);
    if(isBadFCALJet(j)) return true;
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Jet in bad FCAL
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isBadFCALJet(const Jet* jet)
{
  if(jet->Pt() > BAD_FCAL_PT  && 
     fabs(jet->Eta()) > BAD_FCAL_ETA &&
     jet->Phi() > BAD_FCAL_PHILOW && 
     jet->Phi() < BAD_FCAL_PHIHIGH)
    return true;
  return false;
}

/*--------------------------------------------------------------------------------*/
// Overlap Methods
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_e_overlap(ElectronVector& elecs, float minDr)
{
  uint nEl = elecs.size();
  if(nEl < 2) return;

  // Find all possible pairings
  static std::set<const Electron*> removeElecs;
  removeElecs.clear();
  for(uint iEl=0; iEl<nEl; iEl++){
    const Electron* e1 = elecs[iEl];
    for(uint jEl=iEl+1; jEl<nEl; jEl++){
      const Electron* e2 = elecs[jEl];
      if(e1->DeltaR(*e2) < minDr){
        if(e1->Pt() < e2->Pt()){
          removeElecs.insert(e1);
          break;
        }
        else{
          removeElecs.insert(e2);
        }
      } // dR
    } // e2 loop
  } // e1 loop

  // Remove electrons that overlap
  for(int iEl=nEl-1; iEl>=0; iEl--){
    if(removeElecs.find(elecs[iEl]) != removeElecs.end()){
      elecs.erase( elecs.begin() + iEl );
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_j_overlap(ElectronVector& elecs, JetVector& jets, float minDr, bool removeJets)
{
  if(elecs.size()==0 || jets.size()==0) return;

  for(int ie=elecs.size()-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if(e->DeltaR(*j) > minDr) continue;
      
      if(removeJets){
	jets.erase( jets.begin() + ij );
      }
      else{
	elecs.erase( elecs.begin() + ie );
	break;
      }
    }// end loop over jets
  }// end loop over electrons	  
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::m_j_overlap(MuonVector& muons, JetVector jets, float minDr)
{
  if(muons.size()==0 || jets.size()==0) return;

  for(int im=muons.size()-1; im>=0; im--){
    const Muon* mu = muons.at(im);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if(mu->DeltaR(*j) > minDr) continue;

      muons.erase( muons.begin() + im );
      break;

    }// end loop over jets
  }// end loop over muons
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_m_overlap(ElectronVector& elecs, MuonVector& muons, float minDr)
{
  uint nEl = elecs.size();
  uint nMu = muons.size();
  if(nEl==0 || nMu==0) return;

  // Electron muon overlap should be pretty rare,
  // so we can take advantage of that and optimize
  static std::set<const Electron*> removeElecs;
  static std::set<const Muon*> removeMuons;
  removeElecs.clear();
  removeMuons.clear();

  // In this case we will want to remove both the electron and the muon
  for(uint iEl=0; iEl<nEl; iEl++){
    const Electron* e = elecs[iEl];
    for(uint iMu=0; iMu<nMu; iMu++){
      const Muon* mu = muons[iMu];
      if(e->DeltaR(*mu) < minDr){
        removeElecs.insert(e);
        removeMuons.insert(mu);
      }
    }
  }

  // Remove those electrons flagged for removal
  if(removeElecs.size()){
    for(int iEl=nEl-1; iEl>=0; iEl--){
      if(removeElecs.find(elecs[iEl])!=removeElecs.end()){
        elecs.erase( elecs.begin() + iEl );
      }
    }
  }
  // Remove those muons flagged for removal
  if(removeMuons.size()){
    for(int iMu=nMu-1; iMu>=0; iMu--){
      if(removeMuons.find(muons[iMu])!=removeMuons.end()){
        muons.erase( muons.begin() + iMu );
      }
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::m_m_overlap(MuonVector& muons, float minDr)
{
  uint nMu = muons.size();
  if(nMu < 2) return;

  // If 2 muons overlap, toss them both!
  static std::set<const Muon*> removeMuons;
  removeMuons.clear();
  for(uint iMu=0; iMu<nMu; iMu++){
    const Muon* mu1 = muons[iMu];
    for(uint jMu=iMu+1; jMu<nMu; jMu++){
      const Muon* mu2 = muons[jMu];
      if(mu1->DeltaR(*mu2) < minDr){
        removeMuons.insert(mu1);
        removeMuons.insert(mu2);
      }
    }
  }
  for(int iMu=nMu-1; iMu>=0; iMu--){
    const Muon* mu = muons[iMu];
    if(removeMuons.find(mu) != removeMuons.end()){
      muons.erase( muons.begin() + iMu );
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::t_e_overlap(TauVector& taus, ElectronVector& elecs, float minDr)
{
  uint nTau = taus.size();
  uint nEle = elecs.size();
  if(nTau==0 || nEle==0) return;

  for(int iTau=nTau-1; iTau>=0; iTau--){
    const Tau* tau = taus[iTau];
    for(int iEl=nEle-1; iEl>=0; iEl--){
      const Electron* e = elecs[iEl];

      if(tau->DeltaR(*e) < minDr){
        taus.erase( taus.begin() + iTau );
        break;
      }

    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::t_m_overlap(TauVector& taus, MuonVector& muons, float minDr)
{
  uint nTau = taus.size();
  uint nMuo = muons.size();
  if(nTau==0 || nMuo==0) return;

  for(int iTau=nTau-1; iTau>=0; iTau--){
    const Tau* tau = taus[iTau];
    for(int iMu=nMuo-1; iMu>=0; iMu--){
      const Muon* mu = muons[iMu];

      if(tau->DeltaR(*mu) < minDr){
        taus.erase( taus.begin() + iTau );
        break;
      }

    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::t_j_overlap(TauVector& taus, JetVector& jets, float minDr, bool removeJets)
{
  uint nTau = taus.size();
  uint nJet = jets.size();
  if(nTau==0 || nJet==0) return;

  for(int iTau=nTau-1; iTau>=0; iTau--){
    const Tau* tau = taus.at(iTau);
    for(int iJet=jets.size()-1; iJet>=0; iJet--){
      const Jet* jet = jets.at(iJet);

      if(tau->DeltaR(*jet) < minDr){
        if(removeJets)
	  jets.erase( jets.begin() + iJet );
        else{
	  taus.erase( taus.begin() + iTau );
	  break;
        }
      }

    } // end loop over jets
  } // end loop over electrons	  
}

/*--------------------------------------------------------------------------------*/
// Methods to handle signal region cuts
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(ElectronVector& elecs, float MllCut)
{
  uint nEle = elecs.size();
  if(nEle < 2) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  ElectronVector electronsPass;

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nEle; i++){
    Electron* e1 = elecs[i];
    bool pass = true;
    for(uint j=0; j<nEle; j++){
      if(i==j) continue;
      Electron* e2 = elecs[j];
      if(isSFOS(e1,e2) && Mll(e1,e2) < MllCut){
        pass = false;
        break;
      }
    }
    if(pass) electronsPass.push_back(e1);
  }

  // Now replace the supplied vector with this cleaned vector
  elecs = electronsPass;
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(MuonVector& muons, float MllCut)
{
  uint nMu = muons.size();
  if(nMu < 2) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  MuonVector muonsPass;

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nMu; i++){
    Muon* m1 = muons[i];
    bool pass = true;
    for(uint j=0; j<nMu; j++){
      if(i==j) continue;
      Muon* m2 = muons[j];
      if(isSFOS(m1,m2) && Mll(m1,m2) < MllCut){
        pass = false;
        break;
      }
    }
    if(pass) muonsPass.push_back(m1);
  }

  // Now replace the supplied vector with this cleaned vector
  muons = muonsPass;
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(TauVector& taus, float MllCut)
{
  uint nTau = taus.size();
  if(nTau < 2) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  TauVector tausPass;

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nTau; i++){
    Tau* t1 = taus[i];
    bool pass = true;
    for(uint j=0; j<nTau; j++){
      if(i==j) continue;
      Tau* t2 = taus[j];
      if(isOppSign(t1,t2) && Mll(t1,t2) < MllCut){
        pass = false;
        break;
      }
    }
    if(pass) tausPass.push_back(t1);
  }

  // Now replace the supplied vector with this cleaned vector
  taus = tausPass;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::eventHasSusyPropagators(const std::vector< int > &pdgs,
                                          const std::vector< std::vector< int > > &parentIndices)
{
  // Loop over mc_* particles, find the C1 that is not a self-copy,
  // look at the mother and accept event only if it's Z0/gamma/W+-.
  // Code is not optmized in any meaningful way, feel free to do so
  // if you wish. Also please report any unwanted behavior to
  // amete@cern.ch
  //bool passDiagram = true;
  const int  kPgam(+22),  kPz(+23), kPw(+24), kPchargino1(1000024);
  size_t nParticles(pdgs.size());
  for(size_t ii=0; ii<nParticles; ++ii) {
    int pdg(TMath::Abs(pdgs.at(ii)));
    //if(passDiagram && pdg==kPchargino1)
    if(pdg==kPchargino1) {
      for(unsigned int jj=0; jj<parentIndices.at(ii).size(); ++jj) {
        int parenPdg(TMath::Abs(pdgs.at(parentIndices.at(ii).at(jj))));
        if(parenPdg==kPchargino1) break; // Self-copy
        if(parenPdg!=kPgam && parenPdg!=kPz && parenPdg!=kPw) { 
          return true;
          //passDiagram=false; 
          //break; 
        }
      } // end of parent loop
    }
  } // end of truth particle loop

  return false;
  //bool involvesSusyProp(!passDiagram);
  //return involvesSusyProp;
}
/*--------------------------------------------------------------------------------*/
// Event cleaning cut flags
/*--------------------------------------------------------------------------------*/
int SusyNtTools::cleaningCutFlags(int ntCutFlag,
                                  const MuonVector& preMuons, const MuonVector& baseMuons,
                                  const JetVector& preJets, const JetVector& baseJets)
{
  int cutMask = ECut_GRL | ECut_LarErr | ECut_TTC | ECut_GoodVtx | ECut_TileTrip;
  int cutFlags = ntCutFlag & cutMask;
  if(!hasBadMuon(preMuons))     cutFlags |= ECut_BadMuon;
  if(!hasCosmicMuon(baseMuons)) cutFlags |= ECut_Cosmic;
  if(!hasHotSpotJet(preJets))   cutFlags |= ECut_HotSpot;
  if(!hasBadJet(baseJets))      cutFlags |= ECut_BadJet;
  //cout << "cutFlags " << cutFlags << endl;
  return cutFlags;
}
/*--------------------------------------------------------------------------------*/
// Object level cleaning cut flag methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBadMuon(const MuonVector& preMuons)
{
  uint nMu = preMuons.size();
  for(uint i=0; i<nMu; i++){
    if(preMuons[i]->isBadMuon) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasCosmicMuon(const MuonVector& baseMuons)
{
  uint nMu = baseMuons.size();
  for(uint i=0; i<nMu; i++){
    if(baseMuons[i]->isCosmic) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasHotSpotJet(const JetVector& preJets)
{
  uint nJet = preJets.size();
  for(uint i=0; i<nJet; i++){
    if(preJets[i]->isHotTile) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBadJet(const JetVector& baseJets)
{
  uint nJet = baseJets.size();
  for(uint i=0; i<nJet; i++){
    if(baseJets[i]->isBadVeryLoose) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
// Pass Dead Region based on met, jets and run number
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passDeadRegions(const JetVector& preJets, const Met* met, int RunNumber, bool isMC)
{
  // Info taken from here:
  // https://indico.cern.ch/getFile.py/access?contribId=9&resId=0&materialId=slides&confId=223778
  // Loop over all selected jets with Pt > 30 GeV and BCH_Corr_JET > 0.05
  // If dPhi(met, jet) < 0.3, reject event
  // This is only done run number > 213863, when FEB turned off in HEC, and for all MC

  if( !(RunNumber > 213863 || isMC) ) return true;
  
  for(uint ij = 0; ij<preJets.size(); ++ij){
    const Jet* jet = preJets.at(ij);
    if( !(jet->Pt() > 40.) )          continue;
    if( !(jet->bch_corr_jet > 0.05) ) continue;
  
    if( fabs(jet->DeltaPhi( met->lv() )) < 0.3 ) return false;
  }

  return true;
}

/*--------------------------------------------------------------------------------*/
// Lepton flavor methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSameFlav(const Lepton* l1, const Lepton* l2)
{ return l1->isEle()==l2->isEle() && l1->isMu()==l2->isMu(); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Lepton* l1, const Lepton* l2)
{ return l1->q*l2->q < 0; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Tau* tau1, const Tau* tau2)
{ return tau1->q*tau2->q < 0; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOppSign(const Lepton* lep, const Tau* tau)
{ return lep->q*tau->q < 0; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFOS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && isOppSign(l1,l2); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFSS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && !isOppSign(l1,l2); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isOFOS(const Lepton* l1, const Lepton* l2)
{ return !isSameFlav(l1,l2) && isOppSign(l1,l2); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSFOS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isSFOS(leps[i],leps[j])) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSFSS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isSFSS(leps[i],leps[j])) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOFOS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isOFOS(leps[i],leps[j])) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isOppSign(leps[i],leps[j])) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(!isOppSign(leps[i],leps[j])) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasOS(const TauVector& taus){
  uint nTau = taus.size();
  for(uint i=0; i<nTau; i++){
    for(uint j=i+1; j<nTau; j++){
      if(isOppSign(taus[i], taus[j])) return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Mass calculation methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mll(const Particle* l1, const Particle* l2)
{ return (*l1 + *l2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
{ return (*l1 + *l2 + *l3).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mllll(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4)
{ return (*l1 + *l2 + *l3 + *l4).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mjj(const Jet* j1, const Jet* j2)
{ return (*j1 + *j2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlljj(const Lepton* l1, const Lepton* l2, const Jet* j1, const Jet* j2)
{ return (*l1 + *l2 + *j1 + *j2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mt(const TLorentzVector* lep, const Met* met)
{ return sqrt( 2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))) ); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const JetVector& jets, const Met* met, float jetPtCut)
{
  float meff = 0;
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
  }
  if(met) meff += met->Et;
  return meff;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const JetVector& jets, const Met* met, float jetPtCut)
{
  float meff = 0;
  for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
  }
  if(met) meff += met->Et;
  return meff;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met* met, float jetPtCut)
{
  float meff = 0;
  for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  for(uint i=0; i<taus.size(); i++) meff += taus[i]->Pt();
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
  }
  if(met) meff += met->Et;
  return meff;
}

/*--------------------------------------------------------------------------------*/
// Z selection methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{ return isSFOS(l1,l2) && fabs( Mll(l1,l2)-MZ ) < massWindow; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, float massWindow)
{ return (isSFOS(l1,l2) || isSFOS(l1,l3) || isSFOS(l2,l3)) && fabs( Mlll(l1,l2,l3)-MZ ) < massWindow; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4, float massWindow)
{ 
  // Require 2 SFOS pairs
  if(((isSFOS(l1,l2) && isSFOS(l3,l4)) || 
      (isSFOS(l1,l3) && isSFOS(l2,l4)) || 
      (isSFOS(l1,l4) && isSFOS(l2,l3))  ) &&
     fabs(Mllll(l1,l2,l3,l4)-MZ) < massWindow) return true;
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
{
  float mll=Mll(l1,l2);
  return (isSFOS(l1,l2) && mll>minMll && mll<maxMll);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps, float massWindow, bool useMultiLep)
{
  uint dummy;
  return hasZ(leps, &dummy, &dummy, massWindow, useMultiLep);
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps, 
		       uint* Zl2, uint* Zl1, 
		       float massWindow, bool useMultiLep)
{
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      // check for Z->ll
      if( isZ(leps[i], leps[j], massWindow) ){
	*Zl1=i;
	*Zl2=j;
	return true;
      }
      if(useMultiLep){
        for(uint k=j+1; k<nLep; k++){
          // check for Z->lll(l)
          if( isZ(leps[i], leps[j], leps[k], massWindow) ) return true;
          for(uint l=k+1; l<nLep; l++){
            // check for Z->llll
            if( isZ(leps[i], leps[j], leps[k], leps[l], massWindow) ) return true;
          }
        }
      }
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZlll(const LeptonVector& leps, float massWindow)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      for(uint k=j+1; k<leps.size(); k++){
        if(isZ(leps[i], leps[j], leps[k], massWindow)) return true;
      }
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZllll(const LeptonVector& leps, float massWindow)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      for(uint k=j+1; k<leps.size(); k++){
        for(uint l=k+1; l<leps.size(); l++){
          if(isZ(leps[i], leps[j], leps[k], leps[l], massWindow)) return true;
        }
      }
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZllZll(const LeptonVector& leps, 
			    uint* Z1l1, uint* Z1l2, uint* Z2l1, uint* Z2l2,
			    float massWindow)
{
  
  // Find first pair
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      if(isZ(leps[i], leps[j], massWindow)){
	*Z1l1=i;
	*Z1l2=j;
        // Find second pair
        for(uint k=0; k<leps.size(); k++){
          if(k==i || k==j) continue;
          for(uint l=k+1; l<leps.size(); l++){
            if(l==i || l==j) continue;
            if(isZ(leps[k], leps[l], massWindow)){
	      *Z2l1=k;
	      *Z2l2=l;
	      return true;
	    }
          }
        }
      }
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZWindow(const LeptonVector& leps, float minMll, float maxMll)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      if( isZWindow(leps[i],leps[j],minMll,maxMll) ) return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the pair closest to the Z mass
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestZ(uint& l1, uint& l2, const LeptonVector& leps)
{
  float minDM = -1;
  uint nLep = leps.size();
  for(uint i=0; i < nLep; i++){
    for(uint j=i+1; j < nLep; j++){
      if( !isSFOS(leps[i],leps[j]) ) continue;
      float dM = fabs( Mll(leps[i],leps[j]) - MZ );
      if(minDM<0 || dM<minDM){
        minDM = dM;
        l1 = i;
        l2 = j;
      }
    }
  }
  if(minDM<0) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestZ(uint& j1, uint& j2, const JetVector& jets)
{
  float minDM = -1;
  uint nJet = jets.size();
  for(uint i=0; i < nJet; i++){
    for(uint j=i+1; j < nJet; j++){
      float dM = fabs( Mjj(jets[i],jets[j]) - MZ );
      if(minDM<0 || dM<minDM){
        minDM = dM;
        j1 = i;
        j2 = j;
      }
    }
  }
  if(minDM<0) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the jet pair closest to the W mass
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::findBestW(uint& j1, uint& j2, const JetVector& jets)
{
  float minDM = -1;
  uint nJet = jets.size();
  for(uint i=0; i < nJet; i++){
    for(uint j=i+1; j < nJet; j++){
      float dM = fabs( Mjj(jets[i],jets[j]) - MZ );
      if(minDM<0 || dM<minDM){
        minDM = dM;
        j1 = i;
        j2 = j;
      }
    }
  }
  if(minDM<0) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// B-Jet methods
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numBJets(const JetVector& jets, float weight)
{
  int nBJet = 0;
  for(uint i=0; i<jets.size(); i++){
    if(isBJet(jets[i], weight)) nBJet++;
  }
  return nBJet;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBJet(const JetVector& jets, float weight)
{
  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    if(isBJet(jets[i], weight)) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isBJet(const Jet* jet, float weight)
{
  // Use MV1 algorithm
  return jet->mv1 > weight;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBJets(const JetVector& jets, float weight)
{
  JetVector bJets;
  for(uint i=0; i<jets.size(); i++){
    if(isBJet(jets[i], weight)) bJets.push_back(jets[i]);
  }
  return bJets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getBTagSFJets2Lep(const JetVector& baseJets)
{
  JetVector bTagSFJets;
  for(uint i=0; i<baseJets.size(); i++){
    Jet* jet = baseJets[i];
    if(jet->Pt() > JET_PT_B20_CUT && fabs(jet->detEta) < JET_ETA_CUT_2L){
      bTagSFJets.push_back(jet);
    }
  }
  return bTagSFJets;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::bTagSF(const Event* evt, const JetVector& jets, int mcID, BTagSys sys)
{
  if(!evt->isMC) return 1;
  
  if(!m_btagTool){
    if(m_anaType == Ana_2Lep || m_anaType == Ana_2LepWH) 
      configureBTagTool("0_3511", MV1_80, false);
    else
      configureBTagTool("0_3511", MV1_80, true);
  }

  static const float MEV = 1000;
  static vector<float>  pt_btag;
  static vector<float> eta_btag;
  static vector<float> val_btag;
  static vector<int> pdgid_btag;
  pt_btag.clear();
  eta_btag.clear();
  val_btag.clear();
  pdgid_btag.clear();

  bool isSherpa = isSherpaSample(mcID);

  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    pt_btag.push_back(  jets[i]->Pt()*MEV ); 
    eta_btag.push_back( jets[i]->Eta()    ); 
    val_btag.push_back( jets[i]->mv1      ); //Assume MV1 as input always
    pdgid_btag.push_back(jets[i]->truthLabel);
  }

  pair< vector<float>, vector<float> > wgtbtag = 
    m_btagTool->BTagCalibrationFunction(pt_btag, eta_btag,
                                        val_btag, pdgid_btag,
                                        isSherpa);
  
  if( sys == BTag_BJet_DN ) return wgtbtag.first.at(1);  
  if( sys == BTag_CJet_DN ) return wgtbtag.first.at(2);  
  if( sys == BTag_LJet_DN ) return wgtbtag.first.at(3);  
  if( sys == BTag_BJet_UP ) return wgtbtag.first.at(4); 
  if( sys == BTag_CJet_UP ) return wgtbtag.first.at(5); 
  if( sys == BTag_LJet_UP ) return wgtbtag.first.at(6); 

  return wgtbtag.first.at(0); 
}

/*--------------------------------------------------------------------------------*/
// MissingET Rel
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets, bool useForward)
{
  const TLorentzVector metLV = met->lv();
  float dPhi = TMath::Pi()/2.;

  for(uint il=0; il<leptons.size(); ++il)
    if( fabs(metLV.DeltaPhi( *leptons.at(il) )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *leptons.at(il) ));
  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* jet = jets.at(ij);
    if( !useForward && isForwardJet(jet) ) continue; // Use only central jets
    if( fabs(metLV.DeltaPhi( *jet )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *jet ));    
  }// end loop over jets
  
  return metLV.Et() * sin(dPhi);
}

/*--------------------------------------------------------------------------------*/
// Calculate MT2
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const LeptonVector& leptons, const Met* met)
{
  if( leptons.size() < 2 ) return -999;

  // necessary variables
  TLorentzVector metlv = met->lv();
  TLorentzVector l0    = *leptons.at(0);
  TLorentzVector l1    = *leptons.at(1);

  double pTMiss[3] = {0.0, metlv.Px(), metlv.Py()};
  double pA[3]     = {0.0, l0.Px(), l0.Py()};
  double pB[3]     = {0.0, l1.Px(), l1.Py()};
  
  // Create Mt2 object
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pA,pB,pTMiss);
  mt2_event.set_mn(0); // LSP mass = 0 is Generic
  
  return mt2_event.get_mt2();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const TLorentzVector* lep1, const TLorentzVector* lep2, const Met* met)
{
  // necessary variables
  TLorentzVector metLV = met->lv();

  double pTMiss[3] = {0.0, metLV.Px(), metLV.Py()};
  double pA[3]     = {0.0, lep1->Px(), lep1->Py()};
  double pB[3]     = {0.0, lep2->Px(), lep2->Py()};
  
  // Create Mt2 object
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pA,pB,pTMiss);
  mt2_event.set_mn(0); // LSP mass = 0 is Generic
  
  return mt2_event.get_mt2();
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met, 
			  bool zeroMass, float lspMass)
{
  // necessary variables
  TLorentzVector metLV = met->lv();

  double pTMiss[3] = {0.0, metLV.Px(), metLV.Py()};
  double pA[3]     = { (zeroMass) ? 0.0 : p1->M() , p1->Px(), p1->Py()};
  double pB[3]     = { (zeroMass) ? 0.0 : p2->M() , p2->Px(), p2->Py()};
  
  // Create Mt2 object
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pA,pB,pTMiss);
  mt2_event.set_mn(lspMass); 
  
  return mt2_event.get_mt2();

}

/*--------------------------------------------------------------------------------*/
// Calculate HT
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getHT(const JetVector& jets)
{
  float ht = 0;
  for(uint i=0; i<jets.size(); i++){
    float pt = jets[i]->Pt();
    if(pt > 40) ht += pt;
  }
  return ht;
}

/*--------------------------------------------------------------------------------*/
// Calculate HT
/*--------------------------------------------------------------------------------*/
float SusyNtTools::mljj(const LeptonVector& leptons, const JetVector& jets)
{
  if(jets.size() < 1 || leptons.size() < 2) return -999;

  const Lepton& l0 = *leptons.at(0);
  const Lepton& l1 = *leptons.at(1);
  TLorentzVector jj(*jets.at(0));
  if(jets.size() >= 2) jj += *jets.at(1);
  //TLorentzVector l0 = *leptons.at(0);
  //TLorentzVector l1 = *leptons.at(1);
  //TLorentzVector j0 = *jets.at(0);
  //TLorentzVector j1; //initilized to (0,0,0,0)
  //if(jets.size()>=2) j1 = *jets.at(1);
  //TLorentzVector jj = j0+j1;

  float dR1 = jj.DeltaR(l0);
  float dR2 = jj.DeltaR(l1);
  
  return (dR1<dR2) ? (jj+l0).M() : (jj+l1).M();
}


/*--------------------------------------------------------------------------------*/
// Calculate transverse thrust
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getThrT(const LeptonVector& leptons)
{
  float sumPx = 0;
  float sumPy = 0;
  float sumPt = 0;
  float max = 0;
    
  for(uint i=0; i<leptons.size(); i++) {
    sumPx += leptons[i]->Px();
    sumPy += leptons[i]->Py();
    sumPt += leptons[i]->Pt();
  }
  float nx = sumPx/sqrt(sumPx*sumPx + sumPy*sumPy);
  float ny = sumPy/sqrt(sumPx*sumPx + sumPy*sumPy);
    
  for(uint i=0; i<leptons.size(); i++) {
    max += fabs(leptons[i]->Px()*nx + leptons[i]->Py()*ny);
  }
  return max/sumPt;
} 

/*--------------------------------------------------------------------------------*/
// Top Tagging methods borrowed from SUSYTools. Yes this is duplication of code...
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met,
                             int opt, float ptJetCut, float mEffCut)
{
  // Defaults opt = 0, ptJetCut = 0 GeV, mEffCut = 100 GeV
  // Tagging is based on number of jets and at least two leptons
  if( leptons.size() < 2 ) return false;

  // Define some necessary vars
  uint njets         = jets.size();
  uint njetscan      = njets < 3 ? njets : 3; // Only consider up to 3 jets 
  TLorentzVector v1l = *leptons.at(0);
  TLorentzVector v2l = *leptons.at(1);
  double phi         = met->phi;
  double Et          = met->Et;
  TVector2 metv2 (Et * cos(phi), Et * sin(phi));

  // Check to see if our leptons and met is
  // consistent with a W candidate
  bool maybeW = toptag0j(v1l,v2l,metv2);
  if(!maybeW)            return true;
  if( jets.size() < 2 )  return true;
  
  // Check to see if we have top
  int npairs    = 0;
  for(uint i1=0; i1<njetscan-1; ++i1){
    if(i1==0 || opt==0){ // opt=0 implies compare all jets
      for(uint i2=i1+1; i2<njetscan; ++i2){
	TLorentzVector v1j = *jets.at(i1);
	TLorentzVector v2j = *jets.at(i2);
	bool maybeTop = toptag2j(ptJetCut,mEffCut,v1l,v2l,v1j,v2j,metv2,opt);
	if(maybeTop) npairs++;
      }// end loop over second jet
    }// end if options
  }// end loop over first jet

  if( npairs > 0 ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met)
{
  // Taken from TopTag.cxx in SUSYTools

  static const Double_t wmass = 80.4;
  double metx = met.X();
  double mety = met.Y();

  // First mct(ll)
  //---------------
  double mctll=calcMCT(v1l,v2l);

  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double rr=sqrt(pxus*pxus+pyus*pyus)/(2.*wmass);
  double fact=rr+sqrt(1+rr*rr);

  //cout << "mctll " << mctll << " cut value " << wmass*fact << std::endl;
  return mctll < wmass*fact ? true: false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag2j(double ptjcut, double meffcut, 
			   TLorentzVector v1l, TLorentzVector v2l,
			   TLorentzVector v1j, TLorentzVector v2j,
			   TVector2 met, int iopt ) 
{
  // Taken from TopTag.cxx in SUSYTools

  //
  const double tmass = 172.5;
  const double mljcut = 155.;
  const double mctjcut = 137.;
  //
  TLorentzVector vjb[2];
  double ml1j[2];
  double ml2j[2];
  //
  double metx = met.X();  double mety = met.Y();
  double ptl1=v1l.Pt();   double ptl2=v2l.Pt();
  double ptj1=v1j.Pt();   double ptj2=v2j.Pt();
  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double mefftop=ptj1+ptj2+ptl1+ptl2;

  if(ptj2 < ptjcut) return false;
  if(iopt == 0 && mefftop < meffcut) return false;

  // First part:
  //-------------
  vjb[0]=v1j;
  vjb[1]=v2j;
  double mctjj=calcMCT(vjb[0],vjb[1]);
  double pxusjl=vjb[0].Px()+vjb[1].Px()+pxus;
  double pyusjl=vjb[0].Py()+vjb[1].Py()+pyus;
  double rrj=sqrt(pxusjl*pxusjl+pyusjl*pyusjl)/(2.*tmass);
  double factj=rrj+sqrt(1+rrj*rrj);

  bool imct = mctjj < mctjcut*factj ? true : false;
  if(iopt==1) return imct;

  // Second part:
  //--------------
  for(int i=0;i<2;++i) {
    ml1j[i]=(v1l+vjb[i]).M();
    ml2j[i]=(v2l+vjb[i]).M();
  }
  int ncou=0;
  int icou1[2];
  int icou2[2];
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      if(i != j) {
	if(ml1j[i]<mljcut && ml2j[j]<mljcut) {
	  icou1[ncou]=i;
	  icou2[ncou]=j;
	  ncou++;
	}
      }
    }
  }
  bool imjl = ncou>0 ? true : false;

  // Third part: finally mct(ql,ql) for each coupling passing the mjl cut
  //-------------exploit the dependence of mct((jl),(jl)) on the maximum
  //             mass^2 of the two jl combinations 
  int ngcou=0;
  for(int i=0;i<ncou;++i) {
    int ij1=icou1[i];
    int ij2=icou2[i];
    TLorentzVector vtot1=vjb[ij1]+v1l;
    TLorentzVector vtot2=vjb[ij2]+v2l;
    double mctjl=calcMCT(vtot1,vtot2);
    double mm1=(vjb[ij1]+v1l).M();
    double mm2=(vjb[ij2]+v2l).M();
    double mmax2 = mm1>mm2 ? (mm1*mm1) : (mm2*mm2);
    double upl=mmax2/(tmass)+(tmass);
    //cout << " i " << " mctjl " << mctjl << " mmax2 " << mmax2 << 
    //" upl " << upl << endl;
    if(mctjl < upl*factj )  ngcou++;
  }

  bool imctjl = ngcou>0 ? true : false;
  //cout << " ll " << imctll << " ct " << imct << " jl " << imjl <<
  //" ctlj " << imctjl << endl;

  return imct & imjl & imctjl;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::calcMCT(TLorentzVector v1, TLorentzVector v2)
{
  float mct = (v1.Mt() + v2.Mt())*(v1.Mt() + v2.Mt()) - (v1-v2).Perp2();
  mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);
  return mct;
}

/*--------------------------------------------------------------------------------*/
void SusyNtTools::superRazor(const LeptonVector& leptons, const Susy::Met* met,
			     TVector3& vBETA_z, TVector3& pT_CM,
			     TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
			     double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
			     double& gamma_R, double&  dphi_vBETA_R_vBETA_T,
			     double& MDELTAR, double& costhetaRp1)
{
// MDR CALCULATION 
//
// Code written by Christopher Rogan <crogan@cern.ch>, 04-23-13
// Details given in paper (http://arxiv.org/abs/1310.4827) written by 
// Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
//
  if( leptons.size() < 2 ) return;
  
  // necessary variables
  TLorentzVector metlv = met->lv();
  TLorentzVector l0    = *leptons.at(0);
  TLorentzVector l1    = *leptons.at(1);

  //
  // Lab frame
  //
  //Longitudinal boost
  vBETA_z = (1./(l0.E()+l1.E()))*(l0+l1).Vect(); 
  vBETA_z.SetX(0.0);         
  vBETA_z.SetY(0.0);
  
  l0.Boost(-vBETA_z);
  l1.Boost(-vBETA_z);

  //pT of CM frame
  pT_CM = (l0+l1).Vect() + metlv.Vect();
  pT_CM.SetZ(0.0);     
  
  TLorentzVector ll = l0+l1;
  //invariant mass of the total event
  SHATR = sqrt( 2.*(ll.E()*ll.E() - ll.Vect().Dot(pT_CM) 
		   + ll.E()*sqrt( ll.E()*ll.E() + pT_CM.Mag2() - 2.*ll.Vect().Dot(pT_CM) )));
  
  vBETA_T_CMtoR = (1./sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;
  
  l0.Boost(-vBETA_T_CMtoR);
  l1.Boost(-vBETA_T_CMtoR);
  ll.Boost(-vBETA_T_CMtoR);  

  //
  //R-frame
  //
  dphi_LL_vBETA_T = fabs((ll.Vect()).DeltaPhi(vBETA_T_CMtoR));
  
  dphi_L1_L2 = fabs(l0.Vect().DeltaPhi(l1.Vect()));
  
  vBETA_R = (1./(l0.E()+l1.E()))*(l0.Vect() - l1.Vect());
  
  gamma_R = 1./sqrt(1.-vBETA_R.Mag2());
  
  dphi_vBETA_R_vBETA_T = fabs(vBETA_R.DeltaPhi(vBETA_T_CMtoR));
  
  l0.Boost(-vBETA_R);
  l1.Boost(vBETA_R);
 
  //
  //R+1 frame
  //
  MDELTAR = 2.*l0.E();
  costhetaRp1 = l0.Vect().Dot(vBETA_R)/(l0.Vect().Mag()*vBETA_R.Mag());

  return;
}
/*--------------------------------------------------------------------------------*/
// Check if mcID is a known sherpa sample
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSherpaSample(unsigned int mcID)
{
  // Sherpa ttbar dilep and semilep
  if(mcID >= 117800 && mcID <= 117809) return true;
  // Sherpa W/Z gamma
  if(mcID == 126739 || mcID == 126742 || mcID == 126854 || mcID == 126856 || mcID == 145161 || mcID == 145162) return true;
  // Sherpa diboson
  if(mcID >= 126892 && mcID <= 126895) return true;
  if(mcID == 126989) return true;
  // Z + LF/HF
  if(mcID >= 128975 && mcID <= 128977) return true;
  if(mcID >= 146820 && mcID <= 146822) return true;
  // W/Z inclusive
  if(mcID >= 147770 && mcID <= 147776) return true;

  return false;
}
//----------------------------------------------------------
bool SusyNtTools::jetPassesJvfRequirement(const Susy::Jet* jet, JVFUncertaintyTool* jvfTool,
                                          float maxPt, float maxEta, float nominalJvtThres,
                                          SusyNtSys sys, AnalysisType anaType)
{
    bool pass=false;
    // Check if the jet is valid
    if(jet) {
        // Set the necessary variables
        float pt(jet->Pt()), eta(jet->detEta);
        bool applyJvf(pt < maxPt && fabs(eta) < maxEta);
        float jvfThres(nominalJvtThres);
        bool jvfUp(sys == NtSys::JVF_UP), jvfDown(sys == NtSys::JVF_DN);
        // See if JVF need to be checked
        if (!applyJvf) return true;
        // Set the threshold for systematic variations, otherwise always use nominal
        if(jvfUp || jvfDown) {
            if(jvfTool) {
                // For JVF working point 0 there is no down variation. The recommendations is to use up variation,
                // and then symmetrize the uncertainty upstream. Therefore, we currently set down to nominal
                bool isPileUp = false; // Twiki [add link here] says to treat all jets as hardscatter
                if (fabs(nominalJvtThres) < 0.001 && jvfDown) { 
                    jvfThres = nominalJvtThres; 
                } else { 
                    jvfThres = jvfTool->getJVFcut(nominalJvtThres, isPileUp, pt, eta, jvfUp); 
                }
            } else {
                cout << "jetPassesJvfRequirement: error, jvfTool required (" << jvfTool << ")" <<endl;
                assert(jvfTool);
            }
        }
        // Apply JVF cut, this bit is analysis dependent
        if(anaType==Ana_2Lep || anaType==Ana_2LepWH) {
            pass = (fabs(jet->jvf) - 0.001 > jvfThres); 
        } else if (anaType==Ana_3Lep) {
            pass = (jet->jvf > jvfThres);
        } else {
            cout << "jetPassesJvfRequirement: Unknown anaType (" << anaType << "), return true" << endl;
            pass = true;
        }
    } else {
        cout << "jetPassesJvfRequirement: invalid inputs jet(" << jet << "), jvfTool(" << jvfTool << ")."
             << "Return " << pass << endl;
    }
    return pass;
}
//----------------------------------------------------------
