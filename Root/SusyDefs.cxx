#include <algorithm>
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"

using namespace std;
using namespace Susy;



/*--------------------------------------------------------------------------------*/
// LepInfo comparison operators
/*--------------------------------------------------------------------------------*/
inline bool LepInfo::operator > (const LepInfo & other) const
{
  return l->Pt() > other.l->Pt();
}
inline bool LepInfo::operator < (const LepInfo & other) const
{
  return l->Pt() < other.l->Pt();
}

/*--------------------------------------------------------------------------------*/
// Data stream
/*--------------------------------------------------------------------------------*/
string streamName(DataStream stream)
{
  if     (stream==Stream_MC) return "MC";
  else if(stream==Stream_Muons) return "Muons";
  else if(stream==Stream_Egamma) return "Egamma";
  else return "Unknown";
}

/*--------------------------------------------------------------------------------*/
// Data period
/*--------------------------------------------------------------------------------*/
DataPeriod getDataPeriod(uint run)
{
  if(run <= 178109) return Period_B;
  else if(run <= 180481) return Period_D;
  else if(run <= 180776) return Period_E;
  else if(run <= 182519) return Period_F;
  else if(run <= 183462) return Period_G;
  else if(run <= 184169) return Period_H;
  else if(run <= 186493) return Period_I;
  else if(run <= 186755) return Period_J;
  else if(run <= 187815) return Period_K;
  else if(run <= 190343) return Period_L;
  else return Period_M;
}
/*--------------------------------------------------------------------------------*/
McPeriod getMcPeriod(uint run)
{
  if(run <= 180481) return McPeriod_BD;
  else if(run <= 184169) return McPeriod_EH;
  else if(run <= 187815) return McPeriod_IK;
  else return McPeriod_LM;
}

/*--------------------------------------------------------------------------------*/
// Flavor functions
/*--------------------------------------------------------------------------------*/
bool isSameFlav(const Lepton* l1, const Lepton* l2)
{ return l1->isEle()==l2->isEle() && l1->isMu()==l2->isMu(); }

bool isSFOS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q < 0); }

bool isSFSS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q > 0); }

bool hasSFOS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isSFOS(leps[i],leps[j])) return true;
    }
  }
  return false;
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

  if( ne == 1 && nm == 1 ) return ET_em;
  if( ne == 2 && nm == 0 ) return ET_ee;
  if( ne == 0 && nm == 2 ) return ET_mm;
  cout<<"Error: Unable to classify dilepton event with "
      <<ne<<" electrons and "<<nm<<" muons!"<<endl;

  return ET_Unknown;
}

/*--------------------------------------------------------------------------------*/
// Mass functions
/*--------------------------------------------------------------------------------*/
float Mll(const Lepton* l1, const Lepton* l2)
{ return (*l1 + *l2).M(); }

float Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
{ return (*l1 + *l2 + *l3).M(); }

float Mt(const Lepton* lep, const Met* met)
{ return sqrt( 2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))) ); }

float Meff(const LeptonVector& leps, const JetVector& jets, const Met* met)
{
  float meff = 0;
  for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > 40) meff += jets[i]->Pt();
  }
  meff += met->Et;
  return meff;
}


bool isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{ return isSFOS(l1,l2) && fabs( Mll(l1,l2)-MZ ) < massWindow; }

bool hasZ(const LeptonVector& leps, float massWindow)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      if( isZ(leps[i],leps[j],massWindow) ) return true;
    }
  }
  return false;
}

void bestZ(uint& l1, uint& l2, const LeptonVector& leps)
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
  if(minDM<0){
    cout << "bestZ : WARNING : No SFOS candidates!" << endl;
    abort();
  }
}

/*--------------------------------------------------------------------------------*/
// Jet functions
/*--------------------------------------------------------------------------------*/
bool hasBJet(const JetVector& jets, float weight)
{
  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    if( jets[i]->combNN > weight ) return true;
  }
  return false;
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
  triggers[BIT_e20_medium]      = "EF_e20_medium";
  triggers[BIT_e22_medium]      = "EF_e22_medium";
  triggers[BIT_e22vh_medium1]   = "EF_e22vh_medium1";
  triggers[BIT_2e12_medium]     = "EF_2e12_medium";
  triggers[BIT_2e12T_medium]    = "EF_2e12T_medium";
  triggers[BIT_2e12Tvh_medium]  = "EF_2e12Tvh_medium";

  triggers[BIT_mu18]            = "EF_mu18";
  triggers[BIT_mu18_medium]     = "EF_mu18_medium";
  triggers[BIT_2mu10_loose]     = "EF_2mu10_loose";

  triggers[BIT_e10_medium_mu6]  = "EF_e10_medium_mu6";

  triggers[BIT_e24vhi_medium1]          = "EF_e24vhi_medium1";
  triggers[BIT_2e12Tvh_loose1]          = "EF_2e12Tvh_loose1";
  triggers[BIT_e24vh_medium1_e7_medium1]= "EF_e24vh_medium1_e7_medium1";

  triggers[BIT_mu18_tight]              = "EF_mu18_tight";
  triggers[BIT_mu24i_tight]             = "EF_mu24i_tight";
  triggers[BIT_2mu13]                   = "EF_2mu13";
  triggers[BIT_e12Tvh_medium1_mu8]      = "EF_e12Tvh_medium1_mu8";
  triggers[BIT_mu18_tight_e7_medium1]   = "EF_mu18_tight_e7_medium1";

  return triggers;
}
/*--------------------------------------------------------------------------------*/
/*stringvector getEleTrigChains()
{
  stringvector elTriggers;
  elTriggers.resize(N_EL_TRIG);
  elTriggers[BIT_e10_medium]     = "EF_e10_medium";
  elTriggers[BIT_e12_medium]     = "EF_e12_medium";
  elTriggers[BIT_e20_medium]     = "EF_e20_medium";
  elTriggers[BIT_e22_medium]     = "EF_e22_medium";
  elTriggers[BIT_e22vh_medium1]  = "EF_e22vh_medium1";
  elTriggers[BIT_2e12_medium]    = "EF_2e12_medium";
  elTriggers[BIT_2e12T_medium]   = "EF_2e12T_medium";
  elTriggers[BIT_2e12Tvh_medium] = "EF_2e12Tvh_medium";
  return elTriggers;
}*/
/*--------------------------------------------------------------------------------*/
/*stringvector getMuTrigChains()
{
  stringvector muTriggers;
  muTriggers.resize(N_MU_TRIG);
  muTriggers[BIT_mu6]           = "EF_mu6";
  muTriggers[BIT_mu10_loose]    = "EF_mu10_loose";
  muTriggers[BIT_mu18]          = "EF_mu18";
  muTriggers[BIT_mu18_medium]   = "EF_mu18_medium";
  muTriggers[BIT_2mu10_loose]   = "EF_2mu10_loose";
  return muTriggers;
}*/

