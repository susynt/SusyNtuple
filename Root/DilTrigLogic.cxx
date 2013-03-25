// Trigger methods to handle Susy Dilepton trigger logic

#include "SusyNtuple/DilTrigLogic.h"

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::DilTrigLogic(string period, bool useReweightUtils) :
  m_triggerWeight(NULL),
  m_useMCTrig(false),
  m_useDimuonMetTrigger(false)
{

  cout<<"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="<<endl;
  cout<<"Initializing DilTrigLogic:"<<endl;
  //cout<<"\tisMC:               "<<isMC<<endl;
  cout<<"\tperiod for Weights: "<<period<<endl;
  cout<<"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="<<endl;

  //if( isMC ){

  // Create trigger reweight object
  m_triggerWeight = new triggerReweight2Lep();
  string directory = "$ROOTCOREDIR/../DGTriggerReweight/data";
  m_triggerWeight->setDbg(1);
  if(useReweightUtils)
    m_triggerWeight->initialize(directory, period, true, true);//To turn on ReweightUtils 
  else
    m_triggerWeight->initialize(directory, period);//To turn off ReweightUtils 
  m_triggerWeight->setDbg(0);
  

}
/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::~DilTrigLogic()
{
  
  if(m_triggerWeight) delete m_triggerWeight;
  
}

/*--------------------------------------------------------------------------------*/
// There are three potential ways to use the tool:
// 1.) passDilTrig -- will return result of event level trigger and trigger matching
// 2.) passDilEvtTrig -- will return result of the event level trigger
// 3.) passDilTrigMatch -- will return the result of the trigger matching of leptons
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilTrig(LeptonVector leptons, float met, Event* evt)
{    
  return passDilEvtTrig(leptons, met, evt) && passDilTrigMatch(leptons, met, evt);
}
/*--------------------------------------------------------------------------------*/
// Pass the event level trigger
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilEvtTrig(LeptonVector leptons, float met, Event* evt)
{

  // Make sure there is only two leptons.
  // Code needs to classify dilepton type, and getDiLepEvtType require just 2 leptons.
  if( leptons.size() != 2 ) return false;

  DataStream stream = evt->stream;

  // If unknown stream, then return
  if( stream == Stream_Unknown){
    cout<<"Error: Stream is unknown! Returning false from passDilTrig."<<endl;
    return false;
  }

  // Useful Vars
  DiLepEvtType ET = getDiLepEvtType(leptons);
  uint evtTrigFlags = evt->trigFlags;
  bool isEg = stream == Stream_Egamma;
  bool isMC = stream == Stream_MC;

  if( (isEg || isMC) && ET == ET_ee ){
    DilTriggerRegion dtr = getEETrigRegion(leptons[0]->Pt(), leptons[1]->Pt());
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;
    return passEvtTrigger(evtTrigFlags, dtr);
  }
  if( (!isEg || isMC) && ET == ET_mm ){
    DilTriggerRegion dtr = getMMTrigRegion(leptons[0]->Pt(), leptons[1]->Pt(), met);
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;
    return passEvtTrigger(evtTrigFlags, dtr);
  }
  if( ET == ET_em || ET == ET_me ){
    const Lepton* elec = leptons[0]->isEle() ? leptons[0] : leptons[1];
    const Lepton* muon = leptons[0]->isEle() ? leptons[1] : leptons[0];
    DilTriggerRegion dtr = getEMTrigRegion(elec->Pt(), muon->Pt());
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;

    bool pass = passEvtTrigger(evtTrigFlags, dtr);
    if( !isMC &&  isEg && dtr != DTR_EM_A ) return false; // Egamma stream from Region A
    if( !isMC && !isEg && dtr != DTR_EM_B ) return false; // Muon stream from Region B
    return pass;
  }
  
  return false;
}

/*--------------------------------------------------------------------------------*/
// Pass the event level trigger
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilTrigMatch(LeptonVector leptons, float met, Event* evt)
{

  // Make sure there is only two leptons.
  // Code needs to classify dilepton type, and getDiLepEvtType require just 2 leptons.
  if( leptons.size() != 2 ) return false;

  DataStream stream = evt->stream;

  // If unknown stream, then return
  if( stream == Stream_Unknown){
    cout<<"Error: Stream is unknown! Returning false from passDilTrig."<<endl;
    return false;
  }

  // Useful Vars
  DiLepEvtType ET = getDiLepEvtType(leptons);
  bool isEg = stream == Stream_Egamma;
  bool isMC = stream == Stream_MC;

  if( (isEg || isMC) && ET == ET_ee ){
    DilTriggerRegion dtr = getEETrigRegion(leptons[0]->Pt(), leptons[1]->Pt());
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;
    return passTriggerMatch(leptons[0]->trigFlags, leptons[1]->trigFlags, dtr);
  }
  if( (!isEg || isMC) && ET == ET_mm ){
    DilTriggerRegion dtr = getMMTrigRegion(leptons[0]->Pt(), leptons[1]->Pt(), met);
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;
    return passTriggerMatch(leptons[0]->trigFlags, leptons[1]->trigFlags, dtr);
  }
  if( ET == ET_em || ET == ET_me ){
    const Lepton* elec = leptons[0]->isEle() ? leptons[0] : leptons[1];
    const Lepton* muon = leptons[0]->isEle() ? leptons[1] : leptons[0];
    DilTriggerRegion dtr = getEMTrigRegion(elec->Pt(), muon->Pt());
    if( isMC && !m_useMCTrig ) return dtr != DTR_UNKNOWN;

    bool pass = passTriggerMatch(elec->trigFlags, muon->trigFlags, dtr);
    if( !isMC &&  isEg && dtr != DTR_EM_A ) return false; // Egamma stream from Region A
    if( !isMC && !isEg && dtr != DTR_EM_B ) return false; // Muon stream from Region B
    return pass;
  }
  
  return false;
}

/*--------------------------------------------------------------------------------*/
// Get the Dilepton trigger region based on the Pt
/*--------------------------------------------------------------------------------*/
DilTriggerRegion DilTrigLogic::getEETrigRegion(float pt0, float pt1)
{

  // Region A
  if( 14 < pt0 && 14 < pt1 ) return DTR_EE_A;
  // Region B
  if( 25 < pt0 && 10 < pt1 && pt1 <= 14 ) return DTR_EE_B;
  // Unknown region
  return DTR_UNKNOWN;

}
/*--------------------------------------------------------------------------------*/
DilTriggerRegion DilTrigLogic::getMMTrigRegion(float pt0, float pt1, float met)
{

  // Region A
  if( 18 < pt0 && 18 < pt1 )              return DTR_MM_A;
  // Region B
  if(18 < pt0 && 14 < pt1 && pt1 <= 18)   return DTR_MM_B;
  // Region C
  if( 18 < pt0 && 8 < pt1 && pt1 <= 14 )  return DTR_MM_C;
  // Region D
  if( 14 < pt0 && pt0 <= 18 && 14 < pt1 ) return DTR_MM_D;
  // Region E
  if( m_useDimuonMetTrigger && 8 < pt0 && pt0 <= 18 && 
      8 < pt1 && pt1 <= 14 && met > 70 )  return DTR_MM_E;
  // Unknown region
  return DTR_UNKNOWN;

}
/*--------------------------------------------------------------------------------*/
DilTriggerRegion DilTrigLogic::getEMTrigRegion(float ept, float mpt)
{

  // Region A
  if( 14 < ept && 8 < mpt)                return DTR_EM_A;
  // Region B
  if( 10 < ept && ept <= 14 && 18 < mpt)  return DTR_EM_B;
  // Unknown region
  return DTR_UNKNOWN;
}

/*--------------------------------------------------------------------------------*/
// Check the Event level trigs based on region
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passEvtTrigger(uint evtflag, DilTriggerRegion dtr)
{
  if( dtr == DTR_UNKNOWN) return false;
  if( dtr == DTR_EE_A )   return (evtflag & TRIG_2e12Tvh_loose1);
  if( dtr == DTR_EE_B )   return (evtflag & TRIG_e24vh_medium1_e7_medium1);
  if( dtr == DTR_MM_A )   return (evtflag & TRIG_mu18_tight_mu8_EFFS);
  if( dtr == DTR_MM_B )   return (evtflag & TRIG_mu18_tight_mu8_EFFS) || (evtflag & TRIG_2mu13);
  if( dtr == DTR_MM_C )   return (evtflag & TRIG_mu18_tight_mu8_EFFS);
  if( dtr == DTR_MM_D )   return (evtflag & TRIG_2mu13);
  if( dtr == DTR_MM_E )   return m_useDimuonMetTrigger && (evtflag & TRIG_2mu8_EFxe40wMu_tclcw);
  if( dtr == DTR_EM_A )   return (evtflag & TRIG_e12Tvh_medium1_mu8);
  if( dtr == DTR_EM_B )   return (evtflag & TRIG_mu18_tight_e7_medium1);

  return false;
}
/*--------------------------------------------------------------------------------*/
// Check trigger matching based on region
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passTriggerMatch(uint flag0, uint flag1, DilTriggerRegion dtr)
{
  // IMPORTANT: flag0 and flag1 mean different things between mm/ee and em
  // For ee and mm flag0 corresponds to leading pt lepton and flag1 to subleading
  // For em flag0 = eflag and flag1 = mflag.  
  // If you use this method outside of DilTrigger package you must be careful to
  // adhere to this convention in order to get em trigger logic correct.

  // If Unknown return false
  if(dtr == DTR_UNKNOWN) 
    return false;
 
  // EE Regions
  if( dtr == DTR_EE_A ) 
    return (flag0 & TRIG_e12Tvh_loose1) && (flag1 & TRIG_e12Tvh_loose1);
  if( dtr == DTR_EE_B ) 
    return (flag0 & TRIG_e24vh_medium1_e7_medium1) && (flag1 & TRIG_e24vh_medium1_e7_medium1);

  // MM Regions
  if( dtr == DTR_MM_A ){
    bool match2lep =(flag0 & TRIG_mu18_tight_mu8_EFFS) && (flag1 & TRIG_mu18_tight_mu8_EFFS);
    bool match1lep = (flag0 & TRIG_mu18_tight) || (flag1 & TRIG_mu18_tight);
    return match2lep && match1lep;
  }
  if( dtr == DTR_MM_B ){
    bool match2lep0   = (flag0 & TRIG_mu13) && (flag1 & TRIG_mu13);
    bool match2lep1   = (flag0 & TRIG_mu18_tight_mu8_EFFS) && (flag1 & TRIG_mu18_tight_mu8_EFFS);
    bool matchLeading = (flag0 & TRIG_mu18_tight);
    return ( match2lep0 || (match2lep1 && matchLeading) );
  }
  if( dtr == DTR_MM_C ){
    bool match2lep    = (flag0 & TRIG_mu18_tight_mu8_EFFS) && (flag1 & TRIG_mu18_tight_mu8_EFFS);
    bool matchLeading = (flag0 & TRIG_mu18_tight);
    return  match2lep && matchLeading;
  }
  if( dtr == DTR_MM_D )
    return (flag0 & TRIG_mu13) && (flag1 & TRIG_mu13);

  if( dtr == DTR_MM_E )
    return m_useDimuonMetTrigger && 
      (flag0 & TRIG_2mu8_EFxe40wMu_tclcw) && 
      (flag1 & TRIG_2mu8_EFxe40wMu_tclcw);
  
  // EM Regions
  if( dtr == DTR_EM_A ){
    bool ePass   = (flag0 & TRIG_e12Tvh_medium1);
    bool mPass   = (flag1 & TRIG_mu8);
    return ePass && mPass;
  }
  if( dtr == DTR_EM_B ){
    bool ePass   = (flag0 & TRIG_e7_medium1); 
    bool mPass   = (flag1 & TRIG_mu18_tight);
    return ePass && mPass;
  }

  // Not in region
  return false;

}

/*--------------------------------------------------------------------------------*/
// Methods to get the trigger weight for MC
/*--------------------------------------------------------------------------------*/
double DilTrigLogic::getTriggerWeight(LeptonVector leptons, bool isMC, 
				      float met, int njets, int NPV, 
				      SusyNtSys sys)
{

  // If it is data, return, as tool not initialized
  if( !isMC ) return 1.0;

  // Only valid for 2 leptons
  if(leptons.size() != 2) return 0.;

  // Need to get the weight based on dilepton type
  DiLepEvtType ET = getDiLepEvtType(leptons);
  
  if(ET == ET_ee) 
    return getTriggerWeightEE(leptons, sys);
  if(ET == ET_mm) 
    return getTriggerWeightMM(leptons, met, njets, NPV, sys);
  if(ET == ET_em || ET == ET_me)
    return getTriggerWeightEM(leptons, NPV, sys);

  // If here unable to classify the event: Print error message
  // and return 0
  cout<<"***********************************************************"<<endl;
  cout<<"Error: Unable to classify dilepton type in getTriggerWeight"<<endl;
  cout<<"\tReturning a weight of 0"<<endl;
  cout<<"***********************************************************"<<endl;
  return 0.;
  
}
/*--------------------------------------------------------------------------------*/
double DilTrigLogic::getTriggerWeightEE(LeptonVector leptons, SusyNtSys sys)
{

  if( !m_triggerWeight ){
    cout<<"Error: m_triggerWeight not initialized!"<<endl;
    cout<<"Returning a weight of 0"<<endl;
    return 0.;
  }

  int trigSys = 0; 
  if(sys == NtSys_TRIGSF_EL_UP)      trigSys = 1;
  else if(sys == NtSys_TRIGSF_EL_DN) trigSys = -1;

  double pt0  = leptons[0]->Pt() * 1000.;
  double eta0 = leptons[0]->Eta();
  double pt1  = leptons[1]->Pt() * 1000.;
  double eta1 = leptons[1]->Eta();
  
  return m_triggerWeight->triggerReweightEE(pt0, eta0, pt1, eta1, trigSys);

}
/*--------------------------------------------------------------------------------*/
double DilTrigLogic::getTriggerWeightMM(LeptonVector leptons, float met, 
					int njets, int NPV, SusyNtSys sys)
{

  if( !m_triggerWeight ){
    cout<<"Error: m_triggerWeight not initialized!"<<endl;
    cout<<"Returning a weight of 0"<<endl;
    return 0.;
  }

  int trigSys = 0;
  if(sys == NtSys_TRIGSF_MU_UP)      trigSys = 1;
  else if(sys == NtSys_TRIGSF_MU_DN) trigSys = -1; 

  double pt0  = leptons[0]->Pt() * 1000.;
  double eta0 = leptons[0]->Eta();
  double phi0 = leptons[0]->Phi();
  int isComb0 = ((Muon*) leptons[0])->isCombined;
  double pt1  = leptons[1]->Pt() * 1000.;
  double eta1 = leptons[1]->Eta();
  double phi1 = leptons[1]->Phi();
  int isComb1 = ((Muon*) leptons[1])->isCombined;
  
  return m_triggerWeight->triggerReweightMM(pt0, eta0, phi0, isComb0, pt1, eta1, phi1, isComb1, 
					    trigSys, NPV, met, njets, m_useDimuonMetTrigger);

}
/*--------------------------------------------------------------------------------*/
double DilTrigLogic::getTriggerWeightEM(LeptonVector leptons, int NPV, SusyNtSys sys)
{

  if( !m_triggerWeight ){
    cout<<"Error: m_triggerWeight not initialized!"<<endl;
    cout<<"Returning a weight of 0"<<endl;
    return 0.;
  }

  int trigSys = 0;
  if(sys == NtSys_TRIGSF_EL_UP)      trigSys = 1;
  else if(sys == NtSys_TRIGSF_EL_DN) trigSys = -1;
  else if(sys == NtSys_TRIGSF_MU_UP) trigSys = 2;
  else if(sys == NtSys_TRIGSF_MU_DN) trigSys = -2;

  bool isElec0 = leptons[0]->isEle();

  double elpt  = (isElec0 ? leptons[0]->Pt()  : leptons[1]->Pt()) * 1000.;
  double eleta = isElec0 ? leptons[0]->Eta() : leptons[1]->Eta();
  double mupt  = (isElec0 ? leptons[1]->Pt()  : leptons[0]->Pt()) * 1000.;
  double mueta = isElec0 ? leptons[1]->Eta() : leptons[0]->Eta();
  double muphi = isElec0 ? leptons[1]->Phi() : leptons[0]->Phi();
  int isComb   = isElec0 ? 
    ((Muon*) leptons[1])->isCombined : 
    ((Muon*) leptons[0])->isCombined;
  
    
  return m_triggerWeight->triggerReweightEMU(elpt, eleta, mupt, mueta, muphi, isComb, 
					     trigSys, NPV);
  
}

/*--------------------------------------------------------------------------------*/
// Debug flag
/*--------------------------------------------------------------------------------*/
void DilTrigLogic::debugFlag(uint flag){

  cout << "\tEF_e7_medium1               " << (flag & TRIG_e7_medium1)               << endl;
  cout << "\tEF_e12Tvh_medium1           " << (flag & TRIG_e12Tvh_medium1)           << endl;
  cout << "\tEF_e24vh_medium1            " << (flag & TRIG_e24vh_medium1)            << endl;
  cout << "\tEF_e24vhi_medium1           " << (flag & TRIG_e24vhi_medium1)           << endl;
  cout << "\tEF_2e12Tvh_loose1           " << (flag & TRIG_2e12Tvh_loose1)           << endl;
  cout << "\tEF_e24vh_medium1_e7_medium1 " << (flag & TRIG_e24vh_medium1_e7_medium1) << endl;
  cout << "\tEF_mu8                      " << (flag & TRIG_mu8)                      << endl;
  cout << "\tEF_mu18_tight               " << (flag & TRIG_mu18_tight)               << endl;
  cout << "\tEF_mu24i_tight              " << (flag & TRIG_mu24i_tight)              << endl;
  cout << "\tEF_2mu13                    " << (flag & TRIG_2mu13)                    << endl;
  cout << "\tEF_mu18_tight_mu8_EFFS      " << (flag & TRIG_mu18_tight_mu8_EFFS)      << endl;
  cout << "\tEF_e12Tvh_medium1_mu8       " << (flag & TRIG_e12Tvh_medium1_mu8)       << endl;
  cout << "\tEF_mu18_tight_e7_medium1    " << (flag & TRIG_mu18_tight_e7_medium1)    << endl;
  cout << "\tEF_2mu8_EFxe40wMu_tclcw     " << (flag & TRIG_2mu8_EFxe40wMu_tclcw)     << endl;
}

