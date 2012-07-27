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
  if     (run <= 201556) return Period_A;
  else if(run <= 202798) return Period_B1;
  else if(run <= 203027) return Period_B2;
  else if(run <= 203195) return Period_B3;
  else if(run <= 203524) return Period_B4;
  else if(run <= 203680) return Period_B5;
  else if(run <= 203792) return Period_B6;
  else if(run <= 203876) return Period_B7;
  else if(run <= 204073) return Period_B8;
  else if(run <= 204158) return Period_B9;
  else if(run <= 204442) return Period_B10;
  else if(run <= 204668) return Period_B11;
  else if(run <= 205017) return Period_B12;
  else if(run <= 205071) return Period_B13;
  else return Period_B14;
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
  if(period >= Period_A && period <= Period_B3) return true;
  else return false;
}

/*--------------------------------------------------------------------------------*/
// Flavor functions
/*--------------------------------------------------------------------------------*/
//bool isSameFlav(const Lepton* l1, const Lepton* l2)
//{ return l1->isEle()==l2->isEle() && l1->isMu()==l2->isMu(); }

//bool isSFOS(const Lepton* l1, const Lepton* l2)
//{ return isSameFlav(l1,l2) && (l1->q*l2->q < 0); }

//bool isSFSS(const Lepton* l1, const Lepton* l2)
//{ return isSameFlav(l1,l2) && (l1->q*l2->q > 0); }

//bool hasSFOS(const LeptonVector& leps){
  //uint nLep = leps.size();
  //for(uint i=0; i<nLep; i++){
    //for(uint j=i+1; j<nLep; j++){
      //if(isSFOS(leps[i],leps[j])) return true;
    //}
  //}
  //return false;
//}

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
// Mass functions
/*--------------------------------------------------------------------------------*/
//float Mll(const Lepton* l1, const Lepton* l2)
//{ return (*l1 + *l2).M(); }

//float Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
//{ return (*l1 + *l2 + *l3).M(); }

//float Mt(const Lepton* lep, const Met* met)
//{ return sqrt( 2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))) ); }

//float Meff(const LeptonVector& leps, const JetVector& jets, const Met* met)
//{
  //float meff = 0;
  //for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  //for(uint i=0; i<jets.size(); i++){
    //if(jets[i]->Pt() > 40) meff += jets[i]->Pt();
  //}
  //meff += met->Et;
  //return meff;
//}

//bool isZ(const Lepton* l1, const Lepton* l2, float massWindow)
//{ return isSFOS(l1,l2) && fabs( Mll(l1,l2)-MZ ) < massWindow; }

//bool isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
//{ 
  //float mll=Mll(l1,l2);
  //return (isSFOS(l1,l2) && mll>minMll && mll<maxMll); 
//}

//bool hasZ(const LeptonVector& leps, float massWindow)
//{
  //for(uint i=0; i<leps.size(); i++){
    //for(uint j=i+1; j<leps.size(); j++){
      //if( isZ(leps[i],leps[j],massWindow) ) return true;
    //}
  //}
  //return false;
//}
//bool hasZWindow(const LeptonVector& leps, float minMll, float maxMll)
//{
  //for(uint i=0; i<leps.size(); i++){
    //for(uint j=i+1; j<leps.size(); j++){
      //if( isZWindow(leps[i],leps[j],minMll, maxMll) ) return true;
    //}
  //}
  //return false;
//}


//void bestZ(uint& l1, uint& l2, const LeptonVector& leps)
//{
  //float minDM = -1;
  //uint nLep = leps.size();
  //for(uint i=0; i < nLep; i++){
    //for(uint j=i+1; j < nLep; j++){

      //if( !isSFOS(leps[i],leps[j]) ) continue;
      //float dM = fabs( Mll(leps[i],leps[j]) - MZ );

      //if(minDM<0 || dM<minDM){
        //minDM = dM;
        //l1 = i;
        //l2 = j;
      //}
    //}
  //}
  //if(minDM<0){
    //cout << "bestZ : WARNING : No SFOS candidates!" << endl;
    //abort();
  //}
//}

/*--------------------------------------------------------------------------------*/
// Jet functions
/*--------------------------------------------------------------------------------*/
//bool hasBJet(const JetVector& jets, float weight)
//{
  //uint nJet = jets.size();
  //for(uint i=0; i<nJet; i++){
    ////if( jets[i]->combNN > weight ) return true;
    ////if( jets[i]->mv1 > weight ) return true;
    //if(isBJet(jets[i])) return true;
  //}
  //return false;
//}
/*--------------------------------------------------------------------------------*/
//bool isBJet(const Jet* jet, float weight)
//{ 
  //// Switching to MV1
  //return jet->mv1 > weight; 
//}

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

  triggers[BIT_e7_medium1]              = "EF_e7_medium1";
  triggers[BIT_e12Tvh_loose1]           = "EF_e12Tvh_loose1";
  triggers[BIT_e12Tvh_medium1]          = "EF_e12Tvh_medium1";
  triggers[BIT_e24vh_medium1]           = "EF_e24vh_medium1";
  triggers[BIT_e24vhi_medium1]          = "EF_e24vhi_medium1";
  triggers[BIT_2e12Tvh_loose1]          = "EF_2e12Tvh_loose1";
  triggers[BIT_e24vh_medium1_e7_medium1]= "EF_e24vh_medium1_e7_medium1";

  triggers[BIT_mu8]                     = "EF_mu8";
  triggers[BIT_mu13]                    = "EF_mu13";
  triggers[BIT_mu18_tight]              = "EF_mu18_tight";
  triggers[BIT_mu24i_tight]             = "EF_mu24i_tight";
  triggers[BIT_2mu13]                   = "EF_2mu13";
  triggers[BIT_mu18_tight_mu8_EFFS]     = "EF_mu18_tight_mu8_EFFS";

  triggers[BIT_e12Tvh_medium1_mu8]      = "EF_e12Tvh_medium1_mu8";
  triggers[BIT_mu18_tight_e7_medium1]   = "EF_mu18_tight_e7_medium1";

  return triggers;
}

