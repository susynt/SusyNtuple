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
  class Tau;
  class Jet;
  class Photon;
  class Met;
}

//-----------------------------------------------------------------------------------
// Global constants
//-----------------------------------------------------------------------------------

// This should not be defined in this pkg.  It is very misleading.
//const double GeV = 1000.;
const float MZ = 91.2;

const float LUMI_A_B3  = 1037.;
const float LUMI_A_B14 = 5835.;
const float LUMI_A_D7  = 9632.86;
const float LUMI_A_D   = 10454.;
const float LUMI_A_E   = 13021.8;

//-----------------------------------------------------------------------------------
// Convenience typedefs
//-----------------------------------------------------------------------------------
typedef unsigned int uint;
typedef std::vector<std::string> stringvector;

/*
typedef std::vector<const Susy::Lepton*>   LeptonVector;
typedef std::vector<const Susy::Electron*> ElectronVector;
typedef std::vector<const Susy::Muon*>     MuonVector;
typedef std::vector<const Susy::Photon*>   PhotonVector;
typedef std::vector<const Susy::Tau*>      TauVector;
typedef std::vector<const Susy::Jet*>      JetVector;
*/
typedef std::vector<Susy::Lepton*>   LeptonVector;
typedef std::vector<Susy::Electron*> ElectronVector;
typedef std::vector<Susy::Muon*>     MuonVector;
typedef std::vector<Susy::Photon*>   PhotonVector;
typedef std::vector<Susy::Tau*>      TauVector;
typedef std::vector<Susy::Jet*>      JetVector;

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
  Period_A = 0,
  Period_B1,
  Period_B2,
  Period_B3,
  Period_B4,
  Period_B5,
  Period_B6,
  Period_B7,
  Period_B8,
  Period_B9,
  Period_B10,
  Period_B11,
  Period_B12,
  Period_B13,
  Period_B14,
  Period_Num
};

// MC periods have less granularity than data
enum McPeriod
{
  McPeriod_AB = 0,
  McPeriod_Num
};

// 2-lep or 3-lep flag
enum AnalysisType 
{
  Ana_2Lep = 0,
  Ana_3Lep,
  Ana_N
};

// SUSY D3PD tag, for controlling which branch names to grab
enum D3PDTag
{
  D3PD_p1032 = 0,
  D3PD_p1181,
  D3PD_N
};

// Enum for systematic checks
enum SusyNtSys 
{
  NtSys_NOM = 0,
  //NtSys_EES_UP,      // Electron Scale + sigma
  //NtSys_EES_DN,      // Electron Scale - sigma
  NtSys_EES_Z_UP,    // Electron Scale Zsys + sigma
  NtSys_EES_Z_DN,    // Electron Scale Zsys - sigma
  NtSys_EES_MAT_UP,  // Electron Scale Material + sigma
  NtSys_EES_MAT_DN,  // Electron Scale Material - sigma
  NtSys_EES_PS_UP,   // Electron Scale Presampler + sigma
  NtSys_EES_PS_DN,   // Electron Scale Presampler - sigma
  NtSys_EES_LOW_UP,  // Electron Scale Low Pt + sigma
  NtSys_EES_LOW_DN,  // Electron Scale Low Pt - sigma
  NtSys_EER_UP,      // Electron Resolution + sigma
  NtSys_EER_DN,      // Electron Resolution - sigma
  NtSys_MS_UP,       // Muon MS track + sigma
  NtSys_MS_DN,       // Muon MS track - sigma
  NtSys_ID_UP,       // Muon ID track + sigma
  NtSys_ID_DN,       // Muon ID track - sigma
  NtSys_JES_UP,      // Jet Energy Scale + sigma
  NtSys_JES_DN,      // Jet Energy Scale - sigma
  NtSys_JER,         // Jet Energy Resolution (gaussian)
  NtSys_SCALEST_UP,  // Met scale soft term + sigma
  NtSys_SCALEST_DN,  // Met scale soft term - sigma
  // Only one of these now???
  //NtSys_RESOST_UP,    // Met resolution soft term + sigma
  //NtSys_RESOST_DN,    // Met resolution soft term - sigma
  NtSys_RESOST,      // Met resolution soft term + sigma
  //NtSys_TRIGSF_UP,    // Trigger Scale factor + error
  //NtSys_TRIGSF_DN,    // Trigger Scale factor - error
  NtSys_TRIGSF_EL_UP,    // Trigger Scale factor + error for el
  NtSys_TRIGSF_EL_DN,    // Trigger Scale factor - error for el
  NtSys_TRIGSF_MU_UP,    // Trigger Scale factor + error for mu
  NtSys_TRIGSF_MU_DN,    // Trigger Scale factor - error for mu
  NtSys_N
};

// Human-readable names for enum SusyNtSys 
const std::string SusyNtSystNames[NtSys_N] = {
  "NOM",
  "EES_Z_UP",    
  "EES_Z_DN",    
  "EES_MAT_UP",  
  "EES_MAT_DN",  
  "EES_PS_UP",   
  "EES_PS_DN",   
  "EES_LOW_UP",  
  "EES_LOW_DN",  
  "EER_UP",      
  "EER_DN",      
  "MS_UP",       
  "MS_DN",       
  "ID_UP",       
  "ID_DN",       
  "JES_UP",      
  "JES_DN",      
  "JER",         
  "SCALEST_UP",  
  "SCALEST_DN",  
  "RESOST",
  "TRIGSF_EL_UP",
  "TRIGSF_EL_DN",
  "TRIGSF_MU_UP",
  "TRIGSF_MU_DN"
};

//Enum for BTag SF & systematics
enum BTagSys
{
  BTag_NOM=0,
  BTag_BJet_DN,
  BTag_CJet_DN,
  BTag_LJet_DN,
  BTag_BJet_UP,
  BTag_CJet_UP,
  BTag_LJet_UP,
  BTag_N
};

// Human-readable names for enum BTagSys 
const std::string BTagSysNames[BTag_N] = {
  "BTag_NOM",
  "BTag_BJet_DN",
  "BTag_CJet_DN",
  "BTag_LJet_DN",
  "BTag_BJet_UP",
  "BTag_CJet_UP",
  "BTag_LJet_UP"
};

// Enum for checking event level cuts
// Poorly named...
// This will be phased out
enum EventCheck 
{
  PASS_LAr      = 1<<0,
  PASS_BadJet   = 1<<1,
  PASS_BadMuon  = 1<<2,
  PASS_Cosmic   = 1<<3,
  PASS_Event    = 1<<4,
  PASS_HotSpot  = 1<<5
};

// Enum for event cleaning cuts
enum EventCleaningCuts
{
  ECut_GRL      = 1 << 0,
  ECut_TTC      = 1 << 1,
  ECut_LarErr   = 1 << 2,
  ECut_TileErr  = 1 << 3,
  ECut_GoodVtx  = 1 << 4,
  ECut_HotSpot  = 1 << 5,
  ECut_BadJet   = 1 << 6,
  ECut_BadMuon  = 1 << 7,
  ECut_Cosmic   = 1 << 8
};

// Way to check Event Types
// Could make it a flag to also store Pt info...
enum DiLepEvtType 
{ 
  ET_ee = 0,
  ET_mm,
  ET_em,
  ET_me,
  ET_Unknown,
  ET_N
};

//-----------------------------------------------------------------------------------
// Global functions - most of them moved to SusyNtTools
//-----------------------------------------------------------------------------------
std::string streamName(DataStream);
DataPeriod  getDataPeriod(uint run);
McPeriod    getMcPeriod(uint run);
bool        isPeriodAB3(uint run);

//bool hasBJet(const JetVector& jets, float weight=MV1_60);
//bool isBJet(const Susy::Jet* jet, float weight=MV1_60);

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
  // 2012 triggers
  BIT_e7_medium1 = 0,
  BIT_e12Tvh_loose1,
  BIT_e12Tvh_medium1,
  BIT_e24vh_medium1,
  BIT_e24vhi_medium1,
  BIT_2e12Tvh_loose1,
  BIT_e24vh_medium1_e7_medium1,

  BIT_mu8,
  BIT_mu13,
  BIT_mu18_tight,
  BIT_mu24i_tight,
  BIT_2mu13,
  BIT_mu18_tight_mu8_EFFS,

  BIT_e12Tvh_medium1_mu8,
  BIT_mu18_tight_e7_medium1,

  // Photon Triggers
  BIT_g20_loose,
  BIT_g40_loose,
  BIT_g60_loose,
  BIT_g80_loose,
  BIT_g100_loose,
  BIT_g120_loose,

  N_TRIG
};

//
// Trigger bit masks - could in principle represent multiple chains at once
//

// 2012 Trigger bit masks
const uint TRIG_e7_medium1              = 1<<BIT_e7_medium1;
const uint TRIG_2e12Tvh_loose1          = 1<<BIT_2e12Tvh_loose1;
const uint TRIG_e12Tvh_medium1          = 1<<BIT_e12Tvh_medium1;
const uint TRIG_e24vh_medium1           = 1<<BIT_e24vh_medium1;
const uint TRIG_e24vhi_medium1          = 1<<BIT_e24vhi_medium1;
const uint TRIG_e24vh_medium1_e7_medium1= 1<<BIT_e24vh_medium1_e7_medium1;
const uint TRIG_e12Tvh_loose1           = 1<<BIT_e12Tvh_loose1;

const uint TRIG_mu8                     = 1<<BIT_mu8;
const uint TRIG_mu13                    = 1<<BIT_mu13;
const uint TRIG_mu18_tight              = 1<<BIT_mu18_tight;
const uint TRIG_mu24i_tight             = 1<<BIT_mu24i_tight;
const uint TRIG_2mu13                   = 1<<BIT_2mu13;
const uint TRIG_mu18_tight_mu8_EFFS     = 1<<BIT_mu18_tight_mu8_EFFS;

const uint TRIG_e12Tvh_medium1_mu8      = 1<<BIT_e12Tvh_medium1_mu8;
const uint TRIG_mu18_tight_e7_medium1   = 1<<BIT_mu18_tight_e7_medium1;


// Photon Trigger big masks
const uint TRIG_g20_loose               = 1<<BIT_g20_loose;
const uint TRIG_g40_loose               = 1<<BIT_g40_loose;
const uint TRIG_g60_loose               = 1<<BIT_g60_loose;
const uint TRIG_g80_loose               = 1<<BIT_g80_loose;
const uint TRIG_g100_loose              = 1<<BIT_g100_loose;
const uint TRIG_g120_loose              = 1<<BIT_g120_loose;

// Trigger chain names, for convenience
stringvector getTrigChains();


//-----------------------------------------------------------------------------------
// Standard Analysis Cuts 
//-----------------------------------------------------------------------------------

// These cuts are defined for 2-lep and 3-lep. If there are differences
// an extention of _2L or _3L is added.  We could also separate all cuts
// into 2L and 3L separately, but that leads to duplications

// Pt
const float ELECTRON_PT_CUT  = 10; // GeV
const float MUON_PT_CUT      = 10; // GeV

const float MLL_MIN          = 12;

// TODO: Do we have a common jet pt cut now?
const float JET_PT_CUT           = 20; // GeV
const float JET_SIGNAL_PT_CUT_3L = 20; // GeV
const float JET_SIGNAL_PT_CUT_2L = 30; // GeV

// Eta
const float ELECTRON_ETA_CUT = 2.47; // Currently applied in SusyNtMaker
const float MUON_ETA_CUT     = 2.4;  // Currently applied in SusyNtMaker
const float JET_ETA_CUT      = 2.5;  // Cut in SusyNtMaker is 4.9 Maybe change?
                                     // --> No, 2.5 is SIGNAL JET eta!!
const float JET_JVF_CUT      = 0.5; 
 
const float JET_JVF_CUT_2L   = 0.2;  
const float JET_PT_L25_CUT   = 25;
const float JET_PT_B20_CUT   = 20;
const float JET_PT_F30_CUT   = 30;
const float JET_ETA_MAX_CUT  = 4.5;  

// Signal Requirements
const float ELECTRON_PTCONE30_PT_CUT       = 0.16; 
const float ELECTRON_TOPOCONE30_SLOPE_DATA = 0.02015;
const float ELECTRON_TOPOCONE30_SLOPE_MC   = 0.01794;
const float ELECTRON_TOPOCONE30_PT_CUT     = 0.18;
const float ELECTRON_D0SIG_CUT             = 5.0;
const float ELECTRON_Z0_SINTHETA_CUT       = 0.4;

const float MUON_PTCONE30_SLOPE_DATA = 0.01098;
const float MUON_PTCONE30_SLOPE_MC   = 0.00627;
const float MUON_PTCONE30_PT_CUT     = 0.12;
const float MUON_ETCONE30_K1_DATA    = 0.0648;
const float MUON_ETCONE30_K2_DATA    = 0.00098;
const float MUON_ETCONE30_K1_MC      = 0.0692;
const float MUON_ETCONE30_K2_MC      = 0.00076;
const float MUON_ETCONE30_PT_CUT     = 0.12;
const float MUON_D0SIG_CUT           = 3.0;
const float MUON_Z0_SINTHETA_CUT     = 1.0;//0.2;

// Cuts for overlap
const float E_E_DR = 0.1;
const float J_E_DR = 0.2;
const float T_E_DR = 0.2;
const float T_M_DR = 0.2;
const float J_T_DR = 0.2;
const float E_J_DR = 0.4;
const float M_J_DR = 0.4;
const float E_M_DR = 0.1;
const float M_M_DR = 0.05;

// B-tagging working points
// Recently updated following this twiki:
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BtagAnalysis172#LC_Jets
const float MV1_60 = 0.980;
const float MV1_85 = 0.122;

// BAD FCAL
const float BAD_FCAL_LUMI=1.18034;
const int   BAD_FCAL_RUN1=206348;
const int   BAD_FCAL_RUN2=207332;

const float BAD_FCAL_PT=20;
const float BAD_FCAL_ETA=3.2;
const float BAD_FCAL_PHILOW=1.6;
const float BAD_FCAL_PHIHIGH=3.1;



#endif
