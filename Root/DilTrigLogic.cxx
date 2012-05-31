// Trigger methods to handle Susy Dilepton trigger logic

#include "SusyNtuple/DilTrigLogic.h"

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::DilTrigLogic(bool isMC){

  if( isMC ) loadTriggerMaps();

}
/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::~DilTrigLogic(){

  if( m_trigTool_mu18 ) delete m_trigTool_mu18;
  if( m_trigTool_mu18Med ) delete m_trigTool_mu18Med;
  if( m_trigTool_mu10L_not18 ) delete m_trigTool_mu18;
  if( m_trigTool_mu10L_not18Med ) delete m_trigTool_mu18;
  if( m_trigTool_mu18 ) delete m_trigTool_mu18;

}

/*--------------------------------------------------------------------------------*/
// Does event pass susy dilepton trigger logic?
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilTrig(LeptonVector leptons, int RunNumber, DataStream stream){

  // We ignore trigger logic in Monte Carlo
  if( stream == Stream_MC ) 
    return true;
  if( stream == Stream_Unknown){
    cout<<"Error: Stream is unknown! Returning false from passDilTrig."<<endl;
    return false;
  }

  // Set Vars for easy access
  clear();
  setPeriod(RunNumber);
  setLeptons(leptons);

  // For convenience
  bool isEgamma = stream == Stream_Egamma;
  EventType ET = getEventType(elecs,muons);
  //if(ET==UNKNOWNEVT) cout<<"Nlep: "<<leptons.size()<<endl;

  // Check:
  if( isEgamma && (ET == OSe || ET == SSe) )
    return passEE();
  if( !isEgamma && (ET == OSm || ET == SSm) )
    return passMM();
  if( ET == OSOF || ET == SSOF )
    return passEM(isEgamma);

  // Must not have matched if we get here
  return false;
  
}

/*--------------------------------------------------------------------------------*/
// Methods for dilepton event types
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passEE(){

  uint ne = elecs.size();

  if(ne == 0) return false;

  // Handle two elec trigger effectively
  float ePt0  = elecs.at(0)->Pt();
  float ePt1  = ne > 1 ? elecs.at(1)->Pt() : 0.0;
  uint flag0 = elecs.at(0)->trigFlags;
  uint flag1 = ne > 1 ? elecs.at(1)->trigFlags : 0;
  cout<<"e0: "<<ePt0<<endl;
  debugFlag(flag0);
  cout<<"e1: "<<ePt1<<endl;
  debugFlag(flag1);

  // Single Electron Trigger
  if(ePt0 > 25)
    return (flag0 & getSingleETrig());

  // Di-electron Trigger
  if(ePt0 <= 25 && ePt1 > 17)
    return ( (flag0 & getDoubleETrig()) && (flag1 & getDoubleETrig()) );
  
  // No match
  return false;

}
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passMM(){
 
  if(muons.size() == 0) return false;

  // This time we count what we have
  int nSingMu = 0;
  int nDiMu   = 0;
  
  for(uint im = 0; im<muons.size(); ++im){
    float mPt = muons.at(im)->Pt();
    uint flag = muons.at(im)->trigFlags;
    
    if(mPt > 20 && (flag & getSingleMTrig())) nSingMu++;
    if(mPt > 12 && (flag & getDoubleMTrig())) nDiMu++;

  }// end loop over muons
  
  // Match?
  if( nSingMu >= 1 || nDiMu >=2 ) return true;      

  // No Match
  return false;

}
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passEM(bool isEgamma){

  uint ne = elecs.size();
  uint nm = muons.size();

  if(ne == 0 || nm == 0) return false;

  // Gather necessary vars
  float ePt  = elecs.at(0)->Pt();
  float mPt  = muons.at(0)->Pt();
  uint eFlag = elecs.at(0)->trigFlags; 
  uint mFlag = muons.at(0)->trigFlags; 
  
  // Now for the logic!
  if(isEgamma){
    
    // Case1: Pt(e) > 25 and Pt(m) < 20
    if(ePt > 25 && mPt < 20)
      return (eFlag & getSingleETrig());
    
    // Case2: 15 < Pt(e) < 25 && Pt(m) > 20
    if(15 < ePt && ePt < 25 && mPt > 20)
      return ( (eFlag & getEMTrig()) && (mFlag & getEMTrig()) );

    // Case3: Pt(e) < 25 && Pt(m) > 20
    if(ePt < 25 && mPt > 20)
      return false;

  }// end if Egamma 

  else{
    
    // Case1: Pt(e) > 25 and trigger match, fail
    if(ePt > 25)
      return !( eFlag & getSingleETrig() );	

    // Case2: Pt(m) > 20
    if(mPt > 20)
      return (mFlag & getSingleMTrig());

  }// end if !Egamma

  // No match
  return false;

}

/*--------------------------------------------------------------------------------*/
// Electron specific methods
/*--------------------------------------------------------------------------------*/
uint DilTrigLogic::getSingleETrig(){

  if(A <= period && period <= J) return TRIG_e20_medium;
  else if(period == K)           return TRIG_e22_medium;
  else                           return TRIG_e22vh_medium1;

}
/*--------------------------------------------------------------------------------*/
uint DilTrigLogic::getDoubleETrig(){

  if(A <= period && period <= J) return TRIG_2e12_medium;
  else if(period == K)           return TRIG_2e12T_medium;
  else                           return TRIG_2e12Tvh_medium; // medium1 ?
  
}

/*--------------------------------------------------------------------------------*/
// Muon specific methods
/*--------------------------------------------------------------------------------*/
uint DilTrigLogic::getSingleMTrig(){

  if(A <= period && period <= H) return TRIG_mu18;
  else                           return TRIG_mu18_medium;

}
/*--------------------------------------------------------------------------------*/
uint DilTrigLogic::getDoubleMTrig(){

  return TRIG_2mu10_loose;

}

/*--------------------------------------------------------------------------------*/
// E-M specific methods
/*--------------------------------------------------------------------------------*/
uint DilTrigLogic::getEMTrig(){

  return TRIG_e10_medium_mu6;

}

/*--------------------------------------------------------------------------------*/
// Monte Carlo Weighting
/*--------------------------------------------------------------------------------*/
void DilTrigLogic::loadTriggerMaps()
{
  TFile* eleTrigFile = new TFile("$ROOTCOREDIR/data/DGTriggerReweight/electron_maps.root");
  TFile* muoTrigFile = new TFile("$ROOTCOREDIR/data/DGTriggerReweight/muon_triggermaps.root");
  
  // TODO: dilepton triggers                                                                                                                                                   
  m_elTrigWeightMap[TRIG_e20_medium]     = loadTrigWeighter(eleTrigFile, "e20_medium");
  m_elTrigWeightMap[TRIG_e22_medium]     = loadTrigWeighter(eleTrigFile, "e22_medium");
  m_elTrigWeightMap[TRIG_e22vh_medium1]  = loadTrigWeighter(eleTrigFile, "e22vh_medium1");
  m_elTrigWeightMap[TRIG_2e12_medium]    = loadTrigWeighter(eleTrigFile, "e12_medium");
  m_elTrigWeightMap[TRIG_2e12T_medium]   = loadTrigWeighter(eleTrigFile, "e12T_medium");
  m_elTrigWeightMap[TRIG_2e12Tvh_medium] = loadTrigWeighter(eleTrigFile, "e12Tvh_medium");

  m_muTrigWeightMap[TRIG_mu18]           = loadTrigWeighter(muoTrigFile, "mu18");
  m_muTrigWeightMap[TRIG_mu18_medium]    = loadTrigWeighter(muoTrigFile, "mu18_medium");
  m_muTrigWeightMap[TRIG_2mu10_loose]    = loadTrigWeighter(muoTrigFile, "mu10_loose");
}

/*--------------------------------------------------------------------------------*/
APReweightND* DilTrigLogic::loadTrigWeighter(TFile* f, TString chain)
{
  TString numName = "ths_"+chain+"_num";
  TString denName = "ths_"+chain+"_den";
  // muon file currently contains a typo                                                                                                                                       
  if (chain.Contains("mu")) numName = "ths_"+chain+"_nom";

  // Does this memory get cleaned up when the file closes?                                                                                                                     
  THnSparseD* num = (THnSparseD*) f->Get( numName );
  THnSparseD* den = (THnSparseD*) f->Get( denName );
  if(!num || !den){
    cout << "ERROR loading trig maps for chain " << chain << endl;
    abort();
  }
  return new APReweightND( den, num, true );
}
/*--------------------------------------------------------------------------------*/
APReweightND* DilTrigLogic::getEleTrigWeighter(uint trigFlag)
{
  map<int, APReweightND*>::iterator itr = m_elTrigWeightMap.find(trigFlag);
  if(itr==m_elTrigWeightMap.end()){
    cout << "ERROR - Electron trigger reweighter " << trigFlag << " doesn't exist" << endl;
    return 0;
  }
  return itr->second;
}
/*--------------------------------------------------------------------------------*/
APReweightND* DilTrigLogic::getMuoTrigWeighter(uint trigFlag)
{
  map<int, APReweightND*>::iterator itr = m_muTrigWeightMap.find(trigFlag);
  if(itr==m_muTrigWeightMap.end()){
    cout << "ERROR - Muon trigger reweighter " << trigFlag << " doesn't exist" << endl;
    return 0;
  }
  return itr->second;
}


/*--------------------------------------------------------------------------------*/
// Generic methods 
/*--------------------------------------------------------------------------------*/
void DilTrigLogic::setPeriod(int run){

  // Set the period based on the run which
  // can then be used to deterimine the trigger
  if(run < 177986)                        period = A; //DNE
  else if(177986 <= run && run <= 178109) period = B;
  else if(179710 <= run && run <= 180481) period = D;
  else if(180614 <= run && run <= 180776) period = E;
  else if(182013 <= run && run <= 182519) period = F;
  else if(182726 <= run && run <= 183462) period = G;
  else if(183544 <= run && run <= 184169) period = H;
  else if(185353 <= run && run <= 186493) period = I;
  else if(186516 <= run && run <= 186755) period = J;
  else if(186873 <= run && run <= 187815) period = K;
  else if(188921 <= run && run <= 190343) period = L;
  else if(190608 <= run && run <= 191933) period = M;
  else{
    cout<<"Error: Run not in and period coded in setPeriod()"<<endl;
    cout<<"Setting period to unknown, trigger logic will fail!"<<endl;
    period = UNKNOWNPERIOD;
  }

}

/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
void DilTrigLogic::setLeptons(LeptonVector leps){

  for(uint i=0; i<leps.size(); ++i){
    const Lepton* lep = leps.at(i);
    if(lep->isEle()) elecs.push_back( (Electron*) lep );
    else             muons.push_back( (Muon*)     lep );
  }
}

/*--------------------------------------------------------------------------------*/
EventType DilTrigLogic::getEventType(ElectronVector elecs, MuonVector muons){

  // Set the basic information first: 
  // 1.) sign
  // 2.) number of objects

  // Assumed dilepton for now:
  int sign = 1;
  for(uint ie=0; ie<elecs.size(); ++ie)
    sign = sign * elecs.at(ie)->q;
  for(uint im=0; im<muons.size(); ++im)
    sign = sign * muons.at(im)->q;

  int ne = elecs.size();
  int nm = muons.size();
  
  // Opposite-sign:
  if( sign == -1 ){
    if( ne == 1 && nm == 1 ) return OSOF;
    if( ne == 2 && nm == 0 ) return OSe;
    if( ne == 0 && nm == 2 ) return OSm;
  }
  
  // Same-sign:
  if( sign == 1 ){
    if( ne == 1 && nm == 1 ) return SSOF;
    if( ne == 2 && nm == 0 ) return SSe;
    if( ne == 0 && nm == 2 ) return SSm;
  }

  return UNKNOWNEVT;
}

/*--------------------------------------------------------------------------------*/
// Debug flag
/*--------------------------------------------------------------------------------*/
void DilTrigLogic::debugFlag(uint flag){

  cout<<"\tEF_e20_medium      "<< (flag & TRIG_e20_medium)     <<endl;
  cout<<"\tEF_e22_medium      "<< (flag & TRIG_e22_medium)     <<endl;
  cout<<"\tEF_e22vh_medium1   "<< (flag & TRIG_e22vh_medium1)  <<endl;
  cout<<"\tEF_2e12_medium     "<< (flag & TRIG_2e12_medium)    <<endl;
  cout<<"\tEF_2e12T_medium    "<< (flag & TRIG_2e12T_medium)   <<endl;
  cout<<"\tEF_e2e12Tvh_medium "<< (flag & TRIG_2e12Tvh_medium) <<endl;
  cout<<"\tEF_mu18            "<< (flag & TRIG_mu18)           <<endl;
  cout<<"\tEF_mu18_medium     "<< (flag & TRIG_mu18_medium)    <<endl;
  cout<<"\tEF_2mu10_loose     "<< (flag & TRIG_2mu10_loose)    <<endl;
  cout<<"\tEF_e10_medium_mu6  "<< (flag & TRIG_e10_medium_mu6) <<endl;


}
