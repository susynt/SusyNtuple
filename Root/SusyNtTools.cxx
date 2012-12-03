#include <set>

#include "SusyNtuple/SusyNtTools.h"

// Common Packages
#include "Mt2/mt2_bisect.h" 
#include "SusyNtuple/BTagCalib.h"

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() : 
        m_anaType(Ana_3Lep),
        m_doPtconeCut(true),
        m_doElEtconeCut(true),
        m_doMuEtconeCut(false),
        m_doIPCut(true)
{
}

/*--------------------------------------------------------------------------------*/
// Get event weight, combine gen, pileup, xsec, and lumi weights
// Default weight uses A-D lumi
// You can supply a different luminosity, but the pileup weights will still correspond to A-D
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeight(const Event* evt, float lumi)
{
  if(!evt->isMC) return 1;
  else return evt->w * evt->wPileup * evt->xsec * lumi / evt->sumw;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeightFixed(unsigned int mcChannel, const Event* evt, float lumi)
{
  if(!evt->isMC) return 1;
  float sumw = evt->sumw;
  float xsec = evt->xsec;
  if(mcChannel==147770) sumw = 2.47983337636e+13;
  //if(mcChannel==147771) sumw = 2.35447863214e+13;
  //if(mcChannel==147772) sumw = 1.05702941e+13;
  if(mcChannel==157814) xsec = 1.702;
  else if(mcChannel==157815) xsec = 1.687;
  else if(mcChannel==157816) xsec = 1.702;
  else if(mcChannel==157817) xsec = 9.557000;
  else if(mcChannel==157818) xsec = 9.554000;
  else if(mcChannel==157819) xsec = 9.557000;
  return evt->w * evt->wPileup * xsec * lumi / sumw;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeightAB3(const Event* evt)
{
  if(!evt->isMC) return 1;
  else return evt->w * evt->wPileupAB3 * evt->xsec * LUMI_A_B3 / evt->sumw;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeightAB(const Event* evt)
{
  if(!evt->isMC) return 1;
  else return evt->w * evt->wPileupAB * evt->xsec * LUMI_A_B14 / evt->sumw;
}

/*--------------------------------------------------------------------------------*/
// 'Overview' functions to make it easy for user to grab all objects
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susyNt, ElectronVector& elecs,
                                     MuonVector& muons, TauVector& taus, JetVector& jets, 
                                     SusyNtSys sys, bool selectTaus)
{
  // Get all objects
  elecs = getPreElectrons(susyNt, sys);
  muons = getPreMuons(susyNt, sys);
  jets  = getPreJets(susyNt, sys);
  if(selectTaus) taus  = getPreTaus(susyNt, sys);
  else taus.clear();

  // Now perform the overlap removals
  performOverlap(elecs, muons, taus, jets);

  // Do SFOS removal for Mll < 12 
  removeSFOSPair(elecs, MLL_MIN);
  removeSFOSPair(muons, MLL_MIN);
  // TODO: revisit this??
  removeSFOSPair(taus, MLL_MIN);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(ElectronVector baseElecs, MuonVector baseMuons,
                                   TauVector baseTaus, JetVector baseJets,
				   ElectronVector& sigElecs, MuonVector& sigMuons, 
                                   TauVector& sigTaus, JetVector& sigJets, 
                                   JetVector& sigJets2Lep, uint nVtx, bool isMC)
{
  // Set signal objects
  sigElecs = getSignalElectrons(baseElecs, nVtx, isMC);
  sigMuons = getSignalMuons(baseMuons, nVtx, isMC);
  sigTaus  = getSignalTaus(baseTaus);
  sigJets  = getSignalJets(baseJets);
  sigJets2Lep = getSignalJets2Lep(baseJets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(SusyNtObject* susyNt, ElectronVector& sigElecs, 
				   MuonVector& sigMuons, TauVector& sigTaus, JetVector& sigJets, 
                                   JetVector& sigJets2Lep, SusyNtSys sys, uint nVtx, bool isMC, 
                                   bool selectTaus)
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
  sigElecs = getSignalElectrons(elecs, nVtx, isMC);
  sigMuons = getSignalMuons(muons, nVtx, isMC);
  sigTaus  = getSignalTaus(taus);
  sigJets  = getSignalJets(jets);
  sigJets2Lep = getSignalJets2Lep(jets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::performOverlap(ElectronVector& elecs, MuonVector& muons, TauVector& taus, JetVector& jets)
{
  // Remove electrons from electrons
  e_e_overlap(elecs, E_E_DR);

  // Remove jets from electrons
  e_j_overlap(elecs, jets, J_E_DR, true);

  // TODO: Use a switch for tau OR
  // Remove taus from electrons
  t_e_overlap(taus, elecs, T_E_DR);

  // Remove taus from muons
  t_m_overlap(taus, muons, T_M_DR);

  // Remove jets from taus
  t_j_overlap(taus, jets, J_T_DR, true);
  
  // Remove electrons from jets
  e_j_overlap(elecs, jets, E_J_DR, false);

  // Remove muons from jets
  m_j_overlap(muons, jets, M_J_DR);

  // Remove electrons and muons that overlap
  e_m_overlap(elecs, muons, E_M_DR);

  // Remove muons from muons
  m_m_overlap(muons, M_M_DR);
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
    Muon* m = & susyNt->muo()->at(im);
    m->setState(sys);

    // Apply any additional selection
    if(m->Pt() < MUON_PT_CUT) continue;
    
    // Save
    muons.push_back(m);
  }

  return muons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getPreTaus(SusyNtObject* susyNt, SusyNtSys sys)
{
  // No tau systematics yet
  TauVector taus;
  for(uint iTau=0; iTau < susyNt->tau()->size(); iTau++){
    Tau* tau = & susyNt->tau()->at(iTau);

    // Apply any additional selection
    // Don't need anything here until we add tau systematics

    taus.push_back(tau);
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
ElectronVector SusyNtTools::getSignalElectrons(ElectronVector baseElecs, uint nVtx, bool isMC)
{
  ElectronVector sigElecs;
  for(uint ie=0; ie<baseElecs.size(); ++ie){
    Electron* e = baseElecs.at(ie);
    if(isSignalElectron(e, nVtx, isMC)){
      sigElecs.push_back(e);
    }
  }

  return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(MuonVector baseMuons, uint nVtx, bool isMC)
{
  MuonVector sigMuons;
  for(uint im=0; im<baseMuons.size(); ++im){
    Muon* m = baseMuons.at(im);
    if(isSignalMuon(m, nVtx, isMC)){
      sigMuons.push_back(m);
    }
  }

  return sigMuons;
}
/*--------------------------------------------------------------------------------*/
TauVector SusyNtTools::getSignalTaus(TauVector baseTaus)
{
  TauVector sigTaus;
  for(uint iTau=0; iTau < baseTaus.size(); iTau++){
    Tau* tau = baseTaus[iTau];
    if(isSignalTau(tau)){
      sigTaus.push_back(tau);
    }
  }
  return sigTaus;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(JetVector baseJets)
{
  JetVector sigJets;
  for(uint ij=0; ij<baseJets.size(); ++ij){
    Jet* j = baseJets.at(ij);
    if(isSignalJet(j)){
      sigJets.push_back(j);
    }
  }
  return sigJets;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets2Lep(JetVector baseJets)
{
  JetVector sigJets;
  for(uint ij=0; ij<baseJets.size(); ++ij){
    Jet* j = baseJets.at(ij);
    if(isSignalJet2Lep(j)){
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
Susy::Met* SusyNtTools::getMet(SusyNtObject* susyNt, SusyNtSys sys, bool useNomPhiForMetSys)
{
  // Right now not being clever. Could maybe make sys index correspond to 
  // index on the array.
  
  Met* met = NULL;
  vector<Met>* metTmp = susyNt->met();
  for(uint i=0; i<metTmp->size(); i++){    
    if(metTmp->at(i).sys == sys){
      met = &(metTmp->at(i));
    
      // NEW: For NtSys_SCALEST_UP/NtSys_SCALEST_DOWN use nominal phi
      if(useNomPhiForMetSys && (sys == NtSys_SCALEST_UP || 
				sys == NtSys_SCALEST_DN ||
				sys == NtSys_RESOST)){
	for(uint j=0; j<metTmp->size(); ++j){
	  if(metTmp->at(j).sys == NtSys_NOM){
	    met->phi = metTmp->at(j).phi;
	    break;
	  }
        }
      }
      return met;
    }
  }
  if(!met){
    cout << "Error: Unable to find met for given systematic!  Returning NULL!!" << sys << endl;
  }
  
  return met;
}

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalLepton(const Lepton* l, uint nVtx, bool isMC){
  if( l->isEle() ) return isSignalElectron( (Electron*) l, nVtx, isMC);
  else             return isSignalMuon( (Muon*) l, nVtx, isMC );
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* ele, uint nVtx, bool isMC)
{
  if(!ele->tightPP) return false;

  float pt = ele->Pt();

  // Relative ptcone iso
  if(m_doPtconeCut && ele->ptcone30/pt >= ELECTRON_PTCONE30_PT_CUT) return false;

  // Topo etcone isolation cut
  float etcone = elEtTopoConeCorr(ele, nVtx, isMC);
  if(m_doElEtconeCut && etcone/pt >= ELECTRON_TOPOCONE30_PT_CUT) return false;

  // Trying new sliding etcone iso
  //float a = 0.0067;
  //float b = 0.133;
  //float a = 0.005; // 'looser' cuts
  //float b = 0.15;  // 'looser' cuts
  //float c = 0.3;
  //if(m_doElEtconeCut && etcone/pt >= min(a*pt+b,c)) return false;

  // Impact parameter
  if(m_doIPCut){
    if(fabs(ele->d0Sig()) >= ELECTRON_D0SIG_CUT) return false;
    if(fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;
    // unbiased IP
    //if(fabs(ele->d0Sig(true)) >= 5) return false;
    //if(fabs(ele->z0SinTheta(true)) >= 0.4) return false;
  }

  // 2011 cuts, for testing
  //if(ele->ptcone20/pt > 0.1) return false;
  //if(fabs(ele->d0Sig()) >= 6) return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* mu, uint nVtx, bool isMC)
{
  // ptcone isolation cut with pileup correction
  if(m_doPtconeCut && muPtConeCorr(mu,nVtx,isMC)/mu->Pt() >= MUON_PTCONE30_PT_CUT) return false;

  // etcone isolation cut - not applied by default, but here for studies
  if(m_doMuEtconeCut && muEtConeCorr(mu,nVtx,isMC)/mu->Pt() >= MUON_ETCONE30_PT_CUT) return false;

  // Impact parameter
  if(m_doIPCut){
    if(fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
    if(fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;
    // unbiased IP
    //if(fabs(mu->d0Sig(true)) >= 3) return false;      // tighter, more standard
    //if(fabs(mu->d0Sig(true)) >= 3.5) return false;
    //if(fabs(mu->z0SinTheta(true)) >= 0.4) return false;
  }

  // 2011 cuts, for testing
  //if(mu->ptcone20 >= 1.8) return false;
  //if(fabs(mu->d0Sig()) >= 3) return false;

  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalTau(const Tau* tau, TauID id)
{
  // Loose cuts - probably shouldn't be used for signal taus
  if(id == TauID_loose){
    if(tau->eleBDTLoose != 0) return false;
    if(tau->jetBDTSigLoose != 1) return false;
  }
  else if(id == TauID_medium){
    if(tau->muonVeto != 0) return false;
    if(tau->eleBDTMedium != 0) return false;
    if(tau->jetBDTSigMedium != 1) return false;
  }
  else if(id == TauID_tight){
    if(tau->muonVeto != 0) return false;
    if(tau->eleBDTTight != 0) return false;
    if(tau->jetBDTSigTight != 1) return false;
  }
  else{
    cout << "SusyNtTools::WARNING - tau ID " << id << " not supported!" << endl;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elEtTopoConeCorr(const Electron* e, uint nVtx, bool isMC){
  float slope = isMC? ELECTRON_TOPOCONE30_SLOPE_MC : ELECTRON_TOPOCONE30_SLOPE_DATA;
  return e->topoEtcone30Corr - slope*nVtx;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muPtConeCorr(const Muon* m, uint nVtx, bool isMC){
    float slope = isMC? MUON_PTCONE30_SLOPE_MC : MUON_PTCONE30_SLOPE_DATA;
    return m->ptcone30 - slope*nVtx;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muEtConeCorr(const Muon* m, uint nVtx, bool isMC){
  float k1 = isMC? MUON_ETCONE30_K1_MC : MUON_ETCONE30_K1_DATA;
  float k2 = isMC? MUON_ETCONE30_K2_MC : MUON_ETCONE30_K2_DATA;
  return m->etcone30 - k1*nVtx - k2*nVtx*nVtx;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet(const Jet* jet)
{
  // Two lep and three lep have different requirements
  // TODO: These will likely be merged
  float ptCut = m_anaType==Ana_2Lep? JET_SIGNAL_PT_CUT_2L : JET_SIGNAL_PT_CUT_3L;
  
  if(jet->Pt() < ptCut)         return false;
  if(fabs(jet->Eta()) > JET_ETA_CUT) return false;
  if(jet->jvf < JET_JVF_CUT)         return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Signal Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet2Lep(const Jet* jet)
{
  if(
     !isCentralLightJet(jet) &&
     !isCentralBJet(jet)     &&
     !isForwardJet(jet)
    ) return false;

  return true; 
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Central Light Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isCentralLightJet(const Susy::Jet* jet)
{
  if(jet->Pt() < JET_PT_L25_CUT     )  return false;
  if(fabs(jet->Eta()) > JET_ETA_CUT )  return false;
  if(jet->jvf < JET_JVF_CUT_2L      )  return false;
  if(jet->mv1 > MV1_85              )  return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton B Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isCentralBJet(const Susy::Jet* jet)
{
  if(jet->Pt() < JET_PT_B20_CUT        )  return false;
  if(fabs(jet->Eta()) > JET_ETA_CUT )  return false;
  if(jet->mv1 < MV1_85              )  return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
// Check if given Jet is 2 Lepton Forward Jet
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isForwardJet(const Susy::Jet* jet)
{
  if(jet->Pt() < JET_PT_F30_CUT         ) return false;
  if(fabs(jet->Eta()) < JET_ETA_CUT     ) return false; 
  if(fabs(jet->Eta()) > JET_ETA_MAX_CUT ) return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
// Count Number of 2 Lepton Central Light Jets
/*--------------------------------------------------------------------------------*/
int SusyNtTools::numberOfCLJets(const JetVector& jets)
{
  int ans = 0;

  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* j = jets.at(ij);
    if(isCentralLightJet(j)){
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
void SusyNtTools::getNumberOf2LepJets(const JetVector& jets, int& Ncl, int& Ncb, int& Nf)
{
   Ncl = numberOfCLJets(jets);
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
bool SusyNtTools::isBadFCALJet(const Susy::Jet* jet)
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

  /*
  for(int ie1=elecs.size()-1; ie1>=0; ie1--){
    const Electron* e1 = elecs.at(ie1);
    for(int ie2=ie1-1; ie2>=0; ie2--){
      const Electron* e2 = elecs.at(ie2);
      if(e1->DeltaR(*e2) > minDr) continue;
      
      if(e1->Pt() < e2->Pt()){
	elecs.erase( elecs.begin() + ie1 );
	break;
      }
      else
	elecs.erase( elecs.begin() + ie2 );
    }
  }
  */
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
    const Muon* m = muons.at(im);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if(m->DeltaR(*j) > minDr) continue;

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
      const Muon* m = muons[iMu];
      if(e->DeltaR(*m) < minDr){
        removeElecs.insert(e);
        removeMuons.insert(m);
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

  // Old approach which one removes at most one pair per lepton

  /*
  // In this case we will want to remove both the electron and the muon
  for(int ie=nEl-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int im=nMu-1; im>=0; im--){
      const Muon* m = muons.at(im);
      if(e->DeltaR(*m) > minDr) continue;
      
      elecs.erase( elecs.begin() + ie );
      muons.erase( muons.begin() + im );
      break;

    }// end loop over muons
  }// end loop over jets
  */
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

  /*
  for(int iMu=muons.size()-1; iMu>=0; iMu--){
    const Muon* mu1 = muons.at(iMu);
    for(int jMu=iMu-1; jMu>=0; jMu--){
      const Muon* mu2 = muons.at(jMu);
      if(mu1->DeltaR(*mu2) > minDr) continue;
      
      muons.erase( muons.begin() + iMu );
      muons.erase( muons.begin() + jMu );
      iMu--;
      break;
    } 
  } 
  */
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
      if(isSFOS(t1,t2) && Mll(t1,t2) < MllCut){
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
// Lepton flavor methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSameFlav(const Lepton* l1, const Lepton* l2)
{ return l1->isEle()==l2->isEle() && l1->isMu()==l2->isMu() && l1->isTau()==l2->isTau(); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFOS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q < 0); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFSS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q > 0); }
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
// Mass calculation methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mll(const Lepton* l1, const Lepton* l2)
{ return (*l1 + *l2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
{ return (*l1 + *l2 + *l3).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mjj(const Jet* j1, const Jet* j2)
{ return (*j1 + *j2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlljj(const Lepton* l1, const Lepton* l2, const Jet* j1, const Jet* j2)
{ return (*l1 + *l2 + *j1 + *j2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mt(const Lepton* lep, const Met* met)
{ return sqrt( 2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))) ); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const JetVector& jets, const Met* met)
{
  float meff = 0;
  for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > 40) meff += jets[i]->Pt();
  }
  meff += met->Et;
  return meff;
}

/*--------------------------------------------------------------------------------*/
// Z selection methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{ return isSFOS(l1,l2) && fabs( Mll(l1,l2)-MZ ) < massWindow; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
{
  float mll=Mll(l1,l2);
  return (isSFOS(l1,l2) && mll>minMll && mll<maxMll);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps, float massWindow, bool ignoreTau)
{
  for(uint i=0; i<leps.size(); i++){
    if(ignoreTau && leps[i]->isTau()) continue;
    for(uint j=i+1; j<leps.size(); j++){
      if( isZ(leps[i],leps[j],massWindow) ) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZWindow(const LeptonVector& leps, float minMll, float maxMll, bool ignoreTau)
{
  for(uint i=0; i<leps.size(); i++){
    if(ignoreTau && leps[i]->isTau()) continue;
    for(uint j=i+1; j<leps.size(); j++){
      if( isZWindow(leps[i],leps[j],minMll, maxMll) ) return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the lepton pair closest to the Z mass
/*--------------------------------------------------------------------------------*/
void SusyNtTools::bestZ(uint& l1, uint& l2, const LeptonVector& leps, bool ignoreTau)
{
  float minDM = -1;
  uint nLep = leps.size();
  for(uint i=0; i < nLep; i++){
    if(ignoreTau && leps[i]->isTau()) continue;
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
  if(minDM<0){
    cout << "bestZ : WARNING : No SFOS candidates!" << endl;
    abort();
  }
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
bool SusyNtTools::hasBJet(const JetVector& jets, float weight)
{
  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    if(isBJet(jets[i])) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isBJet(const Jet* jet, float weight)
{
  // Switching to MV1
  return jet->mv1 > weight;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::bTagSF(const Event* evt, const JetVector& jets, bool useNoJVF,
			  std::string taggerName, std::string OP, float opval,
			  BTagSys sys)
{
  if(!evt->isMC) return 1;
  static const float MEV = 1000;
  static vector<float>  pt_btag;
  static vector<float> eta_btag;
  static vector<float> val_btag;
  static vector<int> pdgid_btag;
  pt_btag.clear();
  eta_btag.clear();
  val_btag.clear();
  pdgid_btag.clear();

  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    pt_btag.push_back(  jets[i]->Pt()*MEV ); 
    eta_btag.push_back( jets[i]->Eta()    ); 
    val_btag.push_back( jets[i]->mv1      ); //Assume MV1 as input always
    pdgid_btag.push_back(jets[i]->truthLabel);
  }

  string rootcoredir = getenv("ROOTCOREDIR"); 
  string envFile;

  if(useNoJVF) envFile = rootcoredir + "/data/SusyNtuple/BTagCalibration_noJVF.env";
  else         envFile = rootcoredir + "/data/SusyNtuple/BTagCalibration_wJVF.env";
  string datFile = rootcoredir + "/data/SusyNtuple/";
  pair<vector<float>,vector<float> > wgtbtag;
  wgtbtag = BTagCalib::BTagCalibrationFunction(pt_btag,
					       eta_btag,
					       val_btag,
					       pdgid_btag,
					       taggerName,
					       OP,
					       opval, 
					       envFile, 
					       datFile); 
  
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
float SusyNtTools::getMT2(const LeptonVector& leptons, const Susy::Met* met)
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
