#ifndef SusyCommon_SusyDefs_h
#define SusyCommon_SusyDefs_h

#include <iostream>
#include <fstream>
#include <string>

#include "TChain.h"
#include "TLorentzVector.h"


//-----------------------------------------------------------------------------------
//  SusyDefs
//
//  common definitions for SUSY analysis code
//-----------------------------------------------------------------------------------

namespace Susy
{
  class Lepton;
  class Electron;
  class Muon;
  class Jet;
  class Met;
}

//-----------------------------------------------------------------------------------
// Global constants
//-----------------------------------------------------------------------------------
const float GeV = 1000.;
const float MZ = 91.2;

//-----------------------------------------------------------------------------------
// Convenience typedefs
//-----------------------------------------------------------------------------------
typedef unsigned int uint;
typedef std::vector<std::string> stringvector;

typedef std::vector<const Susy::Lepton*>   LeptonVector;
typedef std::vector<const Susy::Electron*> ElectronVector;
typedef std::vector<const Susy::Muon*>     MuonVector;
typedef std::vector<const Susy::Jet*>      JetVector;

//-----------------------------------------------------------------------------------
// Global enums
//-----------------------------------------------------------------------------------
enum DataStream
{
  Stream_Unknown = 0,
  Stream_MC,
  Stream_Muons,
  Stream_Egamma,
  Stream_N
};

// Enum for Data Periods
enum DataPeriod
{
  Period_B = 0,
  Period_D,
  Period_E,
  Period_F,
  Period_G,
  Period_H,
  Period_I,
  Period_J,
  Period_K,
  Period_L,
  Period_M,
  Period_Num
};

// MC periods have less granularity than data
enum McPeriod
{
  McPeriod_BD = 0,
  McPeriod_EH,
  McPeriod_IK,
  McPeriod_LM,
  McPeriod_Num
};

// 2-lep or 3-lep flag
enum AnalysisType 
{
    Ana_2Lep = 0,
    Ana_3Lep,
    Ana_N
};

// Enum for systematic checks
enum SYSTEMATIC {NOM = 0,
		 EES_UP,      // Electron Scale + sigma
		 EES_DN,      // Electron Scale - sigma
		 EER_UP,      // Electron Resolution + sigma
		 EER_DN,      // Electron Resolution - sigma
		 MS_UP,       // Muon MS track + sigma
		 MS_DN,       // Muon MS track - sigma
		 ID_UP,       // Muon ID track + sigma
		 ID_DN,       // Muon ID track - sigma
		 JES_UP,      // Jet Energy Scale + sigma
		 JES_DN,      // Jet Energy Scale - sigma
		 JER,         // Jet Energy Resolution (gaussian)
		 Syst_N
};

// Enum for checking event level cuts
enum EventCheck {PASS_LAr      = 1<<0,
		 PASS_BadJet   = 1<<1,
		 PASS_BadMuon  = 1<<2,
		 PASS_Cosmic   = 1<<3,
		 PASS_Event    = 1<<4
};

// Way to check Event Types
// Could make it a flag to also store Pt info...
enum DiLepEvtType { 
  ET_ee = 0,
  ET_mm,
  ET_em,
  ET_N
};

//-----------------------------------------------------------------------------------
// LepInfo - a simple, transient class for interacting with leptons in SusyNt
// Not yet sure if I'm going to use this class
//-----------------------------------------------------------------------------------
class LepInfo
{
  public:
    bool isEle;         // is electron
    uint idx;           // idx in SusyNt collection (either nt.ele or nt.muo)
    bool isSignal;      // lepton passes signal cuts
    Susy::Lepton* l;    // pointer to the Lepton in SusyNt

    // Comparison operators for sorting leptons by pt
    bool operator > (const LepInfo & other) const;
    bool operator < (const LepInfo & other) const;
};

//-----------------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------------
std::string streamName(DataStream);
DataPeriod  getDataPeriod(uint run);
McPeriod    getMcPeriod(uint run);

bool isSameFlav(const Susy::Lepton* l1, const Susy::Lepton* l2);
bool isSFOS(const Susy::Lepton* l1, const Susy::Lepton* l2);
bool isSFSS(const Susy::Lepton* l1, const Susy::Lepton* l2);
bool hasSFOS(const LeptonVector& leps);

float Mll(const Susy::Lepton* l1, const Susy::Lepton* l2);
float Mlll(const Susy::Lepton* l1, const Susy::Lepton* l2, const Susy::Lepton* l3);
float Mt(const Susy::Lepton* lep, const Susy::Met* met);
float Meff(const LeptonVector& leps, const JetVector& jets, const Susy::Met* met);

bool isZ(const Susy::Lepton* l1, const Susy::Lepton* l2, float massWindow=10.);
bool hasZ(const LeptonVector& leps, float massWindow=10.);
void bestZ(uint& l1, uint& l2, const LeptonVector& leps);

bool hasBJet(const JetVector& jets, float weight=1.8);

// for pointer sorting
bool comparePt(const TLorentzVector* p1, const TLorentzVector* p2);
// find lepton in a collection
bool findLepton(const Susy::Lepton* lep, const LeptonVector& leptons);

// Dilepton specific
DiLepEvtType getDiLepEvtType(const LeptonVector& leptons);

//-----------------------------------------------------------------------------------
// Trigger flags
//-----------------------------------------------------------------------------------

// Trigger enums - try to respect backwards compatibility by adding to the end
enum TrigBit
{
  BIT_e20_medium = 0,
  BIT_e22_medium,
  BIT_e22vh_medium1,
  BIT_2e12_medium,
  BIT_2e12T_medium,
  BIT_2e12Tvh_medium,
  BIT_mu18,
  BIT_mu18_medium,
  BIT_2mu10_loose,
  BIT_e10_medium_mu6,
  N_TRIG
};

/*enum ElecTrigBit
{
  BIT_e10_medium = 0,
  BIT_e12_medium,
  BIT_e20_medium,
  BIT_e22_medium,
  BIT_e22vh_medium1,
  BIT_2e12_medium,
  BIT_2e12T_medium,
  BIT_2e12Tvh_medium,
  N_EL_TRIG
};
enum MuonTrigBit
{
  BIT_mu6 = 0,
  BIT_mu10_loose,
  BIT_mu18,
  BIT_mu18_medium,
  BIT_2mu10_loose,
  N_MU_TRIG
};*/

// Trigger bit masks - can represent multiple chains at once
// electron
const uint TRIG_e20_medium      = 1<<BIT_e20_medium;
const uint TRIG_e22_medium      = 1<<BIT_e22_medium;
const uint TRIG_e22vh_medium1   = 1<<BIT_e22vh_medium1;
const uint TRIG_2e12_medium     = 1<<BIT_2e12_medium;
const uint TRIG_2e12T_medium    = 1<<BIT_2e12T_medium;
const uint TRIG_2e12Tvh_medium  = 1<<BIT_2e12Tvh_medium;
// muon
const uint TRIG_mu18            = 1<<BIT_mu18;
const uint TRIG_mu18_medium     = 1<<BIT_mu18_medium;
const uint TRIG_2mu10_loose     = 1<<BIT_2mu10_loose;
// e-mu
const uint TRIG_e10_medium_mu6  = 1<<BIT_e10_medium_mu6;

// Trigger chain names, for convenience
stringvector getTrigChains();
stringvector getEleTrigChains();
stringvector getMuTrigChains();


//-----------------------------------------------------------------------------------
// Standard Analysis Cuts 
//-----------------------------------------------------------------------------------

// These cuts are defined for 2-lep and 3-lep. If there are differences
// an extention of _2L or _3L is added.  We could also separate all cuts
// into 2L and 3L separately, but that leads to duplications

// Pt
const float ELECTRON_PT_CUT  = 10; // GeV
const float MUON_PT_CUT      = 10; // GeV
const float JET_PT_CUT_3L    = 20; // GeV
const float JET_PT_CUT_2L    = 30; // GeV

// Eta
const float ELECTRON_ETA_CUT = 2.47; // Currently applied in SusyNtMaker
const float MUON_ETA_CUT     = 2.4;  // Currently applied in SusyNtMaker
const float JET_ETA_CUT      = 2.5;  // Cut in SusyNtMaker is 4.9 Maybe change?

// Signal Requiremtns
const float ELECTRON_PTCONE20_ET_CUT = 0.1; 
const float ELECTRON_D0SIG_CUT       = 6.0;
const float MUON_PTCONE20_CUT        = 1.8;
const float MUON_D0SIG_CUT           = 3.0;

// Cuts for overlap
const float E_E_DR = 0.1;
const float J_E_DR = 0.2;
const float E_J_DR = 0.4;
const float M_J_DR = 0.4;
const float E_M_DR = 0.1;


#endif
