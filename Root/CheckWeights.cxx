#include <iomanip>
#include "TSystem.h"
#include "SusyNtuple/CheckWeights.h"
#include "SusyNtuple/CheckWeights.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// CheckWeights Constructor
/*--------------------------------------------------------------------------------*/
CheckWeights::CheckWeights() :
    m_check_number(0),
    m_total(0)
{
  //setAnaType(AnalysisType::Ana_3Lep);
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void CheckWeights::Begin(TTree* /*tree*/)
{
  SusyNtAna::Begin(0);
}

/*--------------------------------------------------------------------------------*/
// Init is called when TTree or TChain is attached
/*--------------------------------------------------------------------------------*/
void CheckWeights::Init(TTree* tree)
{
  SusyNtAna::Init(tree);
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t CheckWeights::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  SusyNtAna::clearObjects();

  // New approach, using MCWeighter
  const Event* evt = nt.evt();

  cout << "CheckWeights    [" << m_check_number << "/" << m_total << "] Checking xsec * eff * kfactor for DSID: " << evt->mcChannel; // << endl;

  MCWeighter::WeightSys wSys = MCWeighter::Sys_NOM;
  //float w = SusyNtAna::mcWeighter().getMCWeight(evt, LUMI_A_A3, wSys);
  float xsec = SusyNtAna::mcWeighter().getXsecTimesEff(evt, wSys);

  if(xsec<0) {
    cout << "  --> WARNING MCWeighter unable to find xsec!" << endl;
    //cout << "CheckWeights    WARNING MCWeighter unable to find xsec for DSID " << evt->mcChannel << endl;
  }
  else { cout << "  --> OK" << endl; }

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void CheckWeights::Terminate()
{
  //SusyNtAna::Terminate();
}
