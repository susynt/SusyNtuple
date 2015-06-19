#include <algorithm>
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;



/*--------------------------------------------------------------------------------*/
// Data stream
/*--------------------------------------------------------------------------------*/
string streamName(DataStream stream)
{
  if     (stream==Stream_MC) return "MC";
  else if(stream==Stream_PhysicsMain) return "PhysicsMain";
  //else if(stream==Stream_Muons) return "Muons";
  //else if(stream==Stream_Egamma) return "Egamma";
  //else if(stream==Stream_JetTauEtmiss) return "JetTauEtmiss";
  else return "Unknown";
}

/*--------------------------------------------------------------------------------*/
// Data period
/*--------------------------------------------------------------------------------*/
DataPeriod getDataPeriod(uint run)
{
  if(run>=200804 && run<=201556) return Period_A;
  else if(run>=202660 && run<=205113) return Period_B;
  else if(run>=206248 && run<=207397) return Period_C;
  else if(run>=207447 && run<=209025) return Period_D;
  else if(run>=209074 && run<=210308) return Period_E;
  else if(run>=211522 && run<=212272) return Period_G;
  else if(run>=212619 && run<=213359) return Period_H;
  else if(run>=213431 && run<=213819) return Period_I;
  else if(run>=213900 && run<=215091) return Period_J;
  else if(run>=215414 && run<=215643) return Period_L;
  else return Period_Num;
}
/*--------------------------------------------------------------------------------*/
McPeriod getMcPeriod(uint)
{
  return McPeriod_AB;
}
/*--------------------------------------------------------------------------------*/
bool isPeriodAB3(uint run)
{
  DataPeriod period = getDataPeriod(run);
  //if(period >= Period_A && period <= Period_B3) return true;
  if(period>=Period_A && period<=Period_B && run<=203195) return true;
  else return false;
}

/*--------------------------------------------------------------------------------*/
// Event Type functions
/*--------------------------------------------------------------------------------*/
DiLepEvtType getDiLepEvtType(const LeptonVector& leptons)
{

  int ne = 0; 
  int nm = 0;
  for(uint i=0; i<leptons.size(); ++i){
    bool isE = leptons.at(i)->isEle();
    if(isE)  ne++;
    if(!isE) nm++;
  }

  if( ne == 2 && nm == 0 ) return ET_ee;
  if( ne == 0 && nm == 2 ) return ET_mm;
  if( ne == 1 && nm == 1 ){
    const Lepton* el = leptons[0]->isEle() ? leptons[0] : leptons[1];
    const Lepton* mu = leptons[0]->isEle() ? leptons[1] : leptons[0];
    return el->Pt() > mu->Pt() ? ET_em : ET_me;
  }
  cout << "Error: Unable to classify dilepton event with "
       << ne << " electrons and " << nm << " muons!" << endl;

  return ET_Unknown;
}


/*--------------------------------------------------------------------------------*/
// helper function for sorting particle pointers by pt
/*--------------------------------------------------------------------------------*/
bool comparePt(const TLorentzVector* p1, const TLorentzVector* p2) 
{ return p1->Pt() > p2->Pt(); }

/*--------------------------------------------------------------------------------*/
// Find a lepton in a collection
/*--------------------------------------------------------------------------------*/
bool findLepton(const Lepton* lep, const LeptonVector& leptons) { 
  return std::find(leptons.begin(), leptons.end(), lep) != leptons.end(); 
}

/*--------------------------------------------------------------------------------*/
// Trigger chain names
/*--------------------------------------------------------------------------------*/
stringvector getTrigChains()
{
  stringvector triggers;
  triggers.resize(N_TRIG);

  triggers[BIT_e7_medium1]                      = "EF_e7_medium1";
  triggers[BIT_e12Tvh_loose1]                   = "EF_e12Tvh_loose1";
  triggers[BIT_e12Tvh_medium1]                  = "EF_e12Tvh_medium1";
  triggers[BIT_e24vh_medium1]                   = "EF_e24vh_medium1";
  triggers[BIT_e24vhi_medium1]                  = "EF_e24vhi_medium1";
  triggers[BIT_2e12Tvh_loose1]                  = "EF_2e12Tvh_loose1";
  triggers[BIT_e24vh_medium1_e7_medium1]        = "EF_e24vh_medium1_e7_medium1";

  triggers[BIT_mu8]                             = "EF_mu8";
  triggers[BIT_mu13]                            = "EF_mu13";
  triggers[BIT_mu18_tight]                      = "EF_mu18_tight";
  triggers[BIT_mu24i_tight]                     = "EF_mu24i_tight";
  triggers[BIT_2mu13]                           = "EF_2mu13";
  triggers[BIT_mu18_tight_mu8_EFFS]             = "EF_mu18_tight_mu8_EFFS";

  triggers[BIT_e12Tvh_medium1_mu8]              = "EF_e12Tvh_medium1_mu8";
  triggers[BIT_mu18_tight_e7_medium1]           = "EF_mu18_tight_e7_medium1";

  triggers[BIT_g20_loose]                       = "EF_g20_loose";
  triggers[BIT_g40_loose]                       = "EF_g40_loose";
  triggers[BIT_g60_loose]                       = "EF_g60_loose";
  triggers[BIT_g80_loose]                       = "EF_g80_loose";
  triggers[BIT_g100_loose]                      = "EF_g100_loose";
  triggers[BIT_g120_loose]                      = "EF_g120_loose";

  triggers[BIT_tau20_medium1]                   = "EF_tau20_medium1";
  triggers[BIT_tau20Ti_medium1]                 = "EF_tau20Ti_medium1";
  triggers[BIT_tau29Ti_medium1]                 = "EF_tau29Ti_medium1";
  triggers[BIT_tau29Ti_medium1_tau20Ti_medium1] = "EF_tau29Ti_medium1_tau20Ti_medium1";
  triggers[BIT_tau20Ti_medium1_e18vh_medium1]   = "EF_tau20Ti_medium1_e18vh_medium1";
  triggers[BIT_tau20_medium1_mu15]              = "EF_tau20_medium1_mu15";

  triggers[BIT_e18vh_medium1]                   = "EF_e18vh_medium1";
  triggers[BIT_mu15]                            = "EF_mu15";

  triggers[BIT_2mu8_EFxe40wMu_tclcw]            = "EF_2mu8_EFxe40wMu_tclcw";

  triggers[BIT_mu6]                             = "EF_mu6";
  triggers[BIT_2mu6]                            = "EF_2mu6";
  triggers[BIT_e18vh_medium1_2e7T_medium1]      = "EF_e18vh_medium1_2e7T_medium1";
  triggers[BIT_3mu6]                            = "EF_3mu6";
  triggers[BIT_mu18_tight_2mu4_EFFS]            = "EF_mu18_tight_2mu4_EFFS";
  triggers[BIT_2e7T_medium1_mu6]                = "EF_2e7T_medium1_mu6";
  triggers[BIT_e7T_medium1_2mu6]                = "EF_e7T_medium1_2mu6";

  triggers[BIT_xe80_tclcw_loose]                = "EF_xe80_tclcw_loose";

  triggers[BIT_j110_a4tchad_xe90_tclcw_loose]   = "EF_j110_a4tchad_xe90_tclcw_loose";
  triggers[BIT_j80_a4tchad_xe100_tclcw_loose]   = "EF_j80_a4tchad_xe100_tclcw_loose";
  triggers[BIT_j80_a4tchad_xe70_tclcw_dphi2j45xe10] = "EF_j80_a4tchad_xe70_tclcw_dphi2j45xe10";

  triggers[BIT_mu4T]                              = "EF_mu4T";
  triggers[BIT_mu24]                              = "EF_mu24";
  triggers[BIT_mu4T_j65_a4tchad_xe70_tclcw_veryloose] = "EF_mu4T_j65_a4tchad_xe70_tclcw_veryloose";
  triggers[BIT_2mu4T_xe60_tclcw]                  = "EF_2mu4T_xe60_tclcw";
  triggers[BIT_2mu8_EFxe40_tclcw]                 = "EF_2mu8_EFxe40_tclcw";
  triggers[BIT_e24vh_medium1_EFxe35_tclcw]        = "EF_e24vh_medium1_EFxe35_tclcw";
  triggers[BIT_mu24_j65_a4tchad_EFxe40_tclcw]     = "EF_mu24_j65_a4tchad_EFxe40_tclcw";
  triggers[BIT_mu24_j65_a4tchad_EFxe40wMu_tclcw]  = "EF_mu24_j65_a4tchad_EFxe40wMu_tclcw";

  return triggers;
}

