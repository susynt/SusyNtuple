// Trigger methods to handle Susy Dilepton trigger logic

#include "SusyNtuple/DilTrigLogic.h"

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::DilTrigLogic(bool isMC)
{
  
  //if( isMC ) loadTriggerMaps();
  

}
/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
DilTrigLogic::~DilTrigLogic()
{
  
  if( m_trigTool_mu18 )           delete m_trigTool_mu18;
  if( m_trigTool_mu18Med )        delete m_trigTool_mu18Med;
  if( m_trigTool_mu10L_not18 )    delete m_trigTool_mu18;
  if( m_trigTool_mu10L_not18Med ) delete m_trigTool_mu18;
  if( m_trigTool_mu18 )           delete m_trigTool_mu18;
  
}

/*--------------------------------------------------------------------------------*/
// There are three potential ways to use the tool:
// 1.) passDilTrig -- will return result of event level trigger and trigger matching
// 2.) passDilEvtTrig -- will return result of the event level trigger
// 3.) passDilTrigMatch -- will return the result of the trigger matching of leptons
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilTrig(LeptonVector leptons, Event* evt)
{    
  return passDilEvtTrig(leptons, evt) && passDilTrigMatch(leptons, evt);
}
/*--------------------------------------------------------------------------------*/
// Pass the event level trigger
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passDilEvtTrig(LeptonVector leptons, Event* evt)
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
    return passEvtTrigger(evtTrigFlags, dtr);
  }
  if( (!isEg || isMC) && ET == ET_mm ){
    DilTriggerRegion dtr = getMMTrigRegion(leptons[0]->Pt(), leptons[1]->Pt());
    return passEvtTrigger(evtTrigFlags, dtr);
  }
  if( ET == ET_em || ET == ET_me ){
    const Lepton* elec = leptons[0]->isEle() ? leptons[0] : leptons[1];
    const Lepton* muon = leptons[0]->isEle() ? leptons[1] : leptons[0];
    DilTriggerRegion dtr = getEMTrigRegion(elec->Pt(), muon->Pt());
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
bool DilTrigLogic::passDilTrigMatch(LeptonVector leptons, Event* evt)
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
    return passTriggerMatch(leptons[0]->trigFlags, leptons[1]->trigFlags, dtr);
  }
  if( (!isEg || isMC) && ET == ET_mm ){
    DilTriggerRegion dtr = getMMTrigRegion(leptons[0]->Pt(), leptons[1]->Pt());
    return passTriggerMatch(leptons[0]->trigFlags, leptons[1]->trigFlags, dtr);
  }
  if( ET == ET_em || ET == ET_me ){
    const Lepton* elec = leptons[0]->isEle() ? leptons[0] : leptons[1];
    const Lepton* muon = leptons[0]->isEle() ? leptons[1] : leptons[0];
    DilTriggerRegion dtr = getEMTrigRegion(elec->Pt(), muon->Pt());
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
  if( 25 < pt0 && 10 < pt1 && pt1 < 14 ) return DTR_EE_B;
  // Unknown region
  return DTR_UNKNOWN;

}
/*--------------------------------------------------------------------------------*/
DilTriggerRegion DilTrigLogic::getMMTrigRegion(float pt0, float pt1)
{

  // Region A
  if( 18 < pt0 && 18 < pt1 )             return DTR_MM_A;
  // Region B
  if(18 < pt0 && 14 < pt1 && pt1 < 18)   return DTR_MM_B;
  // Region C
  if( 18 < pt0 && 8 < pt1 && pt1 < 14 )  return DTR_MM_C;
  // Region D
  if( 14 < pt0 && pt0 < 20 && 14 < pt1 ) return DTR_MM_D;
  // Unknown region
  return DTR_UNKNOWN;

}
/*--------------------------------------------------------------------------------*/
DilTriggerRegion DilTrigLogic::getEMTrigRegion(float ept, float mpt)
{

  // Region A
  if( 14 < ept && 8 < mpt)               return DTR_EM_A;
  // Region B
  if( 10 < ept && ept < 14 && 18 < mpt)  return DTR_EM_B;
  // Unknown region
  return DTR_UNKNOWN;
}

/*--------------------------------------------------------------------------------*/
// Check the Event level trigs based on region
/*--------------------------------------------------------------------------------*/
bool DilTrigLogic::passEvtTrigger(uint evtflag, DilTriggerRegion dtr)
{
  if( dtr == DTR_EE_A )  return (evtflag & TRIG_2e12Tvh_loose1);
  if( dtr == DTR_EE_B )  return (evtflag & TRIG_e24vh_medium1_e7_medium1);
  if( dtr == DTR_MM_A )  return (evtflag & TRIG_mu18_tight_mu8_EFFS);
  if( dtr == DTR_MM_B )  return (evtflag & TRIG_mu18_tight_mu8_EFFS) || (evtflag & TRIG_2mu13);
  if( dtr == DTR_MM_C )  return (evtflag & TRIG_mu18_tight_mu8_EFFS);
  if( dtr == DTR_MM_D )  return (evtflag & TRIG_2mu13);
  if( dtr == DTR_EM_A )  return (evtflag & TRIG_e12Tvh_medium1_mu8);
  if( dtr == DTR_EM_B )  return (evtflag & TRIG_mu18_tight_e7_medium1);

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

 
  // EE Regions
  if( dtr == DTR_EE_A ) 
    return (flag0 & TRIG_e12Tvh_loose1) || (flag1 & TRIG_e12Tvh_loose1);
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

}

