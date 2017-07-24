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
  else return "Unknown";
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
