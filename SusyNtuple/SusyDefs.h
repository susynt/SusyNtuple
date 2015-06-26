#ifndef SusyNtuple_SusyDefs_h
#define SusyNtuple_SusyDefs_h

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

namespace Susy {
    class Lepton;
    class Electron;
    class Muon;
    class Tau;
    class Jet;
    class Photon;
    class Met;
    class TruthParticle;
    class TruthJet;
    class TruthMet;
}

//-----------------------------------------------------------------------------------
// Global constants
//-----------------------------------------------------------------------------------

const float MZ = 91.2;

// Run 2 Luminosities [pb-1]
// see data15_13TeV : https://atlas-tagservices.cern.ch/tagservices/RunBrowser/runBrowserReport/rBR_Period_Report.php
const float LUMI_A_A3 = 1.28; ///< "all good" luminosity for periods A1-A3
const float LUMI_A_A4 = 8.33; ///< "all good" luminosity for periods A1-A4

// Run 1 Luminosities
//const float LUMI_A_B3 = 1037.;
//const float LUMI_A_B14 = 5835.;
//const float LUMI_A_D7 = 9632.86;
//const float LUMI_A_D = 10454.;
//const float LUMI_A_E = 13021.8;
//
//const float LUMI_A_L = 20339.8; // 20690.4;

//-----------------------------------------------------------------------------------
// Convenience typedefs
//-----------------------------------------------------------------------------------
typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::vector<std::string> stringvector;

// By request, but I don't like this
/*
typedef std::vector<const Susy::Lepton*>        LeptonVector;
typedef std::vector<const Susy::Electron*>      ElectronVector;
typedef std::vector<const Susy::Muon*>          MuonVector;
typedef std::vector<const Susy::Photon*>        PhotonVector;
typedef std::vector<const Susy::Tau*>           TauVector;
typedef std::vector<const Susy::Jet*>           JetVector;
typedef std::vector<const Susy::TruthParticle*> TruthParticleVector;
typedef std::vector<const Susy::TruthJet*>      TruthJetVector;
*/
typedef std::vector<Susy::Lepton*>        LeptonVector;
typedef std::vector<Susy::Electron*>      ElectronVector;
typedef std::vector<Susy::Muon*>          MuonVector;
typedef std::vector<Susy::Photon*>        PhotonVector;
typedef std::vector<Susy::Tau*>           TauVector;
typedef std::vector<Susy::Jet*>           JetVector;
typedef std::vector<Susy::TruthParticle*> TruthParticleVector;
typedef std::vector<Susy::TruthJet*>      TruthJetVector;

//-----------------------------------------------------------------------------------
// Global enums
//-----------------------------------------------------------------------------------

/// Data stream
//enum DataStream {
//    Stream_Unknown = 0,
//    Stream_MC,
//    Stream_Muons,
//    Stream_Egamma,
//    Stream_JetTauEtmiss,
//    Stream_N
//};

enum DataStream {
    Stream_Unknown = 0,
    Stream_MC,
    Stream_PhysicsMain,
    Stream_Muons,
    Stream_Egamma,
    Stream_JetTauEtmiss,
    Stream_N
};

/// Enum for Data Periods
enum DataPeriod {
    Period_A = 0,
    Period_B,
    Period_C,
    Period_D,
    Period_E,
    Period_G,
    Period_H,
    Period_I,
    Period_J,
    Period_L,
    Period_Num
};

/// MC periods have less granularity than data
/** Not currently relevant! */
enum McPeriod {
    McPeriod_AB = 0,
    McPeriod_Num
};

/// SUSY D3PD tag, for controlling which branch names to grab
enum D3PDTag {
    D3PD_p1032 = 0,
    D3PD_p1181,
    D3PD_p1328,
    D3PD_p1512,
    D3PD_N
};

/// Tau ID enum
enum TauID {
    TauID_none = 0,
    TauID_loose,
    TauID_medium,
    TauID_tight,
    TauID_N
};

/*
/// Enum for systematic checks
enum SusyNtSys
{
NtSys_NOM = 0,
//NtSys_EES_UP,      ///< Electron Scale + sigma
//NtSys_EES_DN,      ///< Electron Scale - sigma
NtSys_EES_Z_UP,    ///< Electron Scale Zsys + sigma
NtSys_EES_Z_DN,    ///< Electron Scale Zsys - sigma
NtSys_EES_MAT_UP,  ///< Electron Scale Material + sigma
NtSys_EES_MAT_DN,  ///< Electron Scale Material - sigma
NtSys_EES_PS_UP,   ///< Electron Scale Presampler + sigma
NtSys_EES_PS_DN,   ///< Electron Scale Presampler - sigma
NtSys_EES_LOW_UP,  ///< Electron Scale Low Pt + sigma
NtSys_EES_LOW_DN,  ///< Electron Scale Low Pt - sigma
NtSys_EER_UP,      ///< Electron Resolution + sigma
NtSys_EER_DN,      ///< Electron Resolution - sigma
NtSys_MS_UP,       ///< Muon MS track + sigma
NtSys_MS_DN,       ///< Muon MS track - sigma
NtSys_ID_UP,       ///< Muon ID track + sigma
NtSys_ID_DN,       ///< Muon ID track - sigma
NtSys_JES_UP,      ///< Jet Energy Scale + sigma
NtSys_JES_DN,      ///< Jet Energy Scale - sigma
NtSys_JER,         ///< Jet Energy Resolution (gaussian)
NtSys_SCALEST_UP,  ///< Met scale soft term + sigma
NtSys_SCALEST_DN,  ///< Met scale soft term - sigma
// Only one of these now???
//NtSys_RESOST_UP,    // Met resolution soft term + sigma
//NtSys_RESOST_DN,    // Met resolution soft term - sigma
NtSys_RESOST,      ///< Met resolution soft term + sigma
//NtSys_TRIGSF_UP,    // Trigger Scale factor + error
//NtSys_TRIGSF_DN,    // Trigger Scale factor - error
NtSys_TRIGSF_EL_UP,    ///< Trigger Scale factor + error for el
NtSys_TRIGSF_EL_DN,    ///< Trigger Scale factor - error for el
NtSys_TRIGSF_MU_UP,    ///< Trigger Scale factor + error for mu
NtSys_TRIGSF_MU_DN,    ///< Trigger Scale factor - error for mu

// Tau systematics added to the end
NtSys_TES_UP,      ///< Tau energy scale + sigma
NtSys_TES_DN,      ///< Tau energy scale - sigma

// Nothing is stored for these, but convenient to define
// the enum here for usage in selectObjects methods
NtSys_JVF_UP,      ///< Jet JVF cut + sigma
NtSys_JVF_DN,      ///< Jet JVF cut - sigma

NtSys_N
};

/// Human-readable names for enum SusyNtSys
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
"TRIGSF_MU_DN",
"TES_UP",
"TES_DN",
"JVF_UP",
"JVF_DN"
};
*/

///Enum for BTag SF & systematics
enum BTagSys {
    BTag_NOM = 0,
    BTag_BJet_DN,
    BTag_CJet_DN,
    BTag_LJet_DN,
    BTag_BJet_UP,
    BTag_CJet_UP,
    BTag_LJet_UP,
    BTag_N
};

/// Human-readable names for enum BTagSys 
const std::string BTagSysNames[BTag_N] = {
    "BTag_NOM",
    "BTag_BJet_DN",
    "BTag_CJet_DN",
    "BTag_LJet_DN",
    "BTag_BJet_UP",
    "BTag_CJet_UP",
    "BTag_LJet_UP"
};

// Oops, might actually put this in NtSys
/*enum JVFSys
{
JVF_NOM=0,
JVF_UP,
JVF_DN,
JVF_N
};

const std::string JVFSysNames[JVF_N] = {
"JVF_NOM",
"JVF_UP",
"JVF_DN"
};*/

/// Enum for event cleaning cuts
enum EventCleaningCuts {
    ECut_GRL = 1 << 0,
    ECut_TTC = 1 << 1,
    ECut_LarErr = 1 << 2,
    ECut_TileErr = 1 << 3,
    ECut_GoodVtx = 1 << 4,
    ECut_HotSpot = 1 << 5,
    ECut_BadJet = 1 << 6,
    ECut_BadMuon = 1 << 7,
    ECut_Cosmic = 1 << 8,
    ECut_SmartVeto = 1 << 9,
    ECut_TileTrip = 1 << 10
};

/// Way to check Event Types
/** Could make it a flag to also store Pt info... */
enum DiLepEvtType {
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

/// Trigger enums
/** try to respect backwards compatibility by adding to the end */
enum TrigBit {
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

    // Tau triggers
    BIT_tau20_medium1,
    BIT_tau20Ti_medium1,
    BIT_tau29Ti_medium1,
    BIT_tau29Ti_medium1_tau20Ti_medium1,
    BIT_tau20Ti_medium1_e18vh_medium1,
    BIT_tau20_medium1_mu15,

    // Missing trigger flags for lep-tau matching
    BIT_e18vh_medium1,
    BIT_mu15,

    // MissingEt trigger
    BIT_2mu8_EFxe40wMu_tclcw,

    // Triggers requested for ISR analysis

    // 3L triggers
    BIT_mu6,
    BIT_2mu6,
    BIT_e18vh_medium1_2e7T_medium1,
    BIT_3mu6,
    BIT_mu18_tight_2mu4_EFFS,
    BIT_2e7T_medium1_mu6,
    BIT_e7T_medium1_2mu6,
    // MET
    BIT_xe80_tclcw_loose,
    // Jet + MET
    BIT_j110_a4tchad_xe90_tclcw_loose,
    BIT_j80_a4tchad_xe100_tclcw_loose,
    BIT_j80_a4tchad_xe70_tclcw_dphi2j45xe10,

    // More triggers requested for ISR analysis
    BIT_mu4T,
    BIT_mu24,
    BIT_mu4T_j65_a4tchad_xe70_tclcw_veryloose,
    BIT_2mu4T_xe60_tclcw,
    BIT_2mu8_EFxe40_tclcw,
    BIT_e24vh_medium1_EFxe35_tclcw,
    BIT_mu24_j65_a4tchad_EFxe40_tclcw,
    BIT_mu24_j65_a4tchad_EFxe40wMu_tclcw,

    N_TRIG
};

//
// Trigger bit masks - could in principle represent multiple chains at once
//

// 2012 Trigger bit masks
const long long TRIG_e7_medium1 = 1LL << BIT_e7_medium1;
const long long TRIG_2e12Tvh_loose1 = 1LL << BIT_2e12Tvh_loose1;
const long long TRIG_e12Tvh_medium1 = 1LL << BIT_e12Tvh_medium1;
const long long TRIG_e24vh_medium1 = 1LL << BIT_e24vh_medium1;
const long long TRIG_e24vhi_medium1 = 1LL << BIT_e24vhi_medium1;
const long long TRIG_e24vh_medium1_e7_medium1 = 1LL << BIT_e24vh_medium1_e7_medium1;
const long long TRIG_e12Tvh_loose1 = 1LL << BIT_e12Tvh_loose1;

const long long TRIG_mu8 = 1LL << BIT_mu8;
const long long TRIG_mu13 = 1LL << BIT_mu13;
const long long TRIG_mu18_tight = 1LL << BIT_mu18_tight;
const long long TRIG_mu24i_tight = 1LL << BIT_mu24i_tight;
const long long TRIG_2mu13 = 1LL << BIT_2mu13;
const long long TRIG_mu18_tight_mu8_EFFS = 1LL << BIT_mu18_tight_mu8_EFFS;

const long long TRIG_e12Tvh_medium1_mu8 = 1LL << BIT_e12Tvh_medium1_mu8;
const long long TRIG_mu18_tight_e7_medium1 = 1LL << BIT_mu18_tight_e7_medium1;

// Photon Trigger bit masks
const long long TRIG_g20_loose = 1LL << BIT_g20_loose;
const long long TRIG_g40_loose = 1LL << BIT_g40_loose;
const long long TRIG_g60_loose = 1LL << BIT_g60_loose;
const long long TRIG_g80_loose = 1LL << BIT_g80_loose;
const long long TRIG_g100_loose = 1LL << BIT_g100_loose;
const long long TRIG_g120_loose = 1LL << BIT_g120_loose;

// Tau trigger bit masks
const long long TRIG_tau20_medium1 = 1LL << BIT_tau20_medium1;
const long long TRIG_tau20Ti_medium1 = 1LL << BIT_tau20Ti_medium1;
const long long TRIG_tau29Ti_medium1 = 1LL << BIT_tau29Ti_medium1;
const long long TRIG_tau29Ti_medium1_tau20Ti_medium1 = 1LL << BIT_tau29Ti_medium1_tau20Ti_medium1;
const long long TRIG_tau20Ti_medium1_e18vh_medium1 = 1LL << BIT_tau20Ti_medium1_e18vh_medium1;
const long long TRIG_tau20_medium1_mu15 = 1LL << BIT_tau20_medium1_mu15;

// Missing trigger flags for lep-tau matching
const long long TRIG_e18vh_medium1 = 1LL << BIT_e18vh_medium1;
const long long TRIG_mu15 = 1LL << BIT_mu15;

// Missing ET trigger(s)
const long long TRIG_2mu8_EFxe40wMu_tclcw = 1LL << BIT_2mu8_EFxe40wMu_tclcw;

// Triggers requested for ISR analysis
const long long TRIG_mu6 = 1LL << BIT_mu6;
const long long TRIG_2mu6 = 1LL << BIT_2mu6;
const long long TRIG_e18vh_medium1_2e7T_medium1 = 1LL << BIT_e18vh_medium1_2e7T_medium1;
const long long TRIG_3mu6 = 1LL << BIT_3mu6;
const long long TRIG_mu18_tight_2mu4_EFFS = 1LL << BIT_mu18_tight_2mu4_EFFS;
const long long TRIG_2e7T_medium1_mu6 = 1LL << BIT_2e7T_medium1_mu6;
const long long TRIG_e7T_medium1_2mu6 = 1LL << BIT_e7T_medium1_2mu6;
const long long TRIG_xe80_tclcw_loose = 1LL << BIT_xe80_tclcw_loose;
const long long TRIG_j110_a4tchad_xe90_tclcw_loose = 1LL << BIT_j110_a4tchad_xe90_tclcw_loose;
const long long TRIG_j80_a4tchad_xe100_tclcw_loose = 1LL << BIT_j80_a4tchad_xe100_tclcw_loose;
const long long TRIG_j80_a4tchad_xe70_tclcw_dphi2j45xe10 = 1LL << BIT_j80_a4tchad_xe70_tclcw_dphi2j45xe10;

const long long TRIG_mu4T = 1LL << BIT_mu4T;
const long long TRIG_mu24 = 1LL << BIT_mu24;
const long long TRIG_mu4T_j65_a4tchad_xe70_tclcw_veryloose = 1LL << BIT_mu4T_j65_a4tchad_xe70_tclcw_veryloose;
const long long TRIG_2mu4T_xe60_tclcw = 1LL << BIT_2mu4T_xe60_tclcw;
const long long TRIG_2mu8_EFxe40_tclcw = 1LL << BIT_2mu8_EFxe40_tclcw;
const long long TRIG_e24vh_medium1_EFxe35_tclcw = 1LL << BIT_e24vh_medium1_EFxe35_tclcw;
const long long TRIG_mu24_j65_a4tchad_EFxe40_tclcw = 1LL << BIT_mu24_j65_a4tchad_EFxe40_tclcw;
const long long TRIG_mu24_j65_a4tchad_EFxe40wMu_tclcw = 1LL << BIT_mu24_j65_a4tchad_EFxe40wMu_tclcw;

/// Trigger chain names, for convenience
stringvector getTrigChains();


//-----------------------------------------------------------------------------------
// Standard Analysis Cuts 
//-----------------------------------------------------------------------------------

// These cuts are defined for 2-lep and 3-lep. If there are differences
// an extention of _2L or _3L is added.  We could also separate all cuts
// into 2L and 3L separately, but that leads to duplications

// Pt
const float ELECTRON_PT_CUT = 10; // GeV
const float MUON_PT_CUT = 10; // GeV
const float TAU_PT_CUT = 20; // GeV

const float MLL_MIN = 12;

// Eta
const float ELECTRON_ETA_CUT = 2.47; // Currently applied in SusyNtMaker
const float MUON_ETA_CUT = 2.4;  // Currently applied in SusyNtMaker

// Signal Requirements
const float ELECTRON_ISO_PT_THRS = 60.0;
const float ELECTRON_PTCONE30_PT_CUT = 0.16;
const float ELECTRON_PTCONE30_PT_WH_CUT = 0.07;
const float ELECTRON_TOPOCONE30_SLOPE_DATA = 0.02015;
const float ELECTRON_TOPOCONE30_SLOPE_MC = 0.01794;
const float ELECTRON_TOPOCONE30_PT_CUT = 0.18;
const float ELECTRON_TOPOCONE30_PT_WH_CUT = 0.13;
const float ELECTRON_D0SIG_CUT = 5.0;
const float ELECTRON_D0SIG_CUT_WH = 3.0;
const float ELECTRON_Z0_SINTHETA_CUT = 0.4;

const float MUON_ISO_PT_THRS = 60.0;
const float MUON_PTCONE30_SLOPE_DATA = 0.01098;
const float MUON_PTCONE30_SLOPE_MC = 0.00627;
const float MUON_PTCONE30_PT_CUT = 0.12;
const float MUON_PTCONE30ELSTYLE_PT_CUT = 0.12;
const float MUON_PTCONE30ELSTYLE_PT_WH_CUT = 0.06;
const float MUON_ETCONE30_K1_DATA = 0.0648;
const float MUON_ETCONE30_K2_DATA = 0.00098;
const float MUON_ETCONE30_K1_MC = 0.0692;
const float MUON_ETCONE30_K2_MC = 0.00076;
const float MUON_ETCONE30_PT_CUT = 0.12;
const float MUON_ETCONE30_PT_WH_CUT = 0.14;//0.10;
const float MUON_D0SIG_CUT = 3.0;
const float MUON_Z0_SINTHETA_CUT = 1.0;//0.2;

// Cuts for overlap
const float E_E_DR = 0.05;
const float J_E_DR = 0.2;
const float T_E_DR = 0.2;
const float T_M_DR = 0.2;
const float J_T_DR = 0.2;
const float E_J_DR = 0.4;
const float M_J_DR = 0.4;
const float E_M_DR = 0.01;
const float M_M_DR = 0.05;

// B-tagging working points
// Recently updated following this twiki:
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BtagAnalysis172#LC_Jets
const float MV1_60 = 0.980;
const float MV1_85 = 0.122;
const float MV1_80 = 0.3511;
const float SV1_80 = 0.60; //  SV1 is NOT RECOMMNEDED for use with MC12a

// BAD FCAL
const float BAD_FCAL_LUMI = 1.18034;
const int   BAD_FCAL_RUN1 = 206348;
const int   BAD_FCAL_RUN2 = 207332;

const float BAD_FCAL_PT = 20;
const float BAD_FCAL_ETA = 3.2;
const float BAD_FCAL_PHILOW = 1.6;
const float BAD_FCAL_PHIHIGH = 3.1;

// Truth level cuts
const float TRUTH_ELECTRON_PT_CUT = 10.;
const float TRUTH_MUON_PT_CUT = 10.;
const float TRUTH_TAU_PT_CUT = 20.; // Correct??
const float TRUTH_JET_CENTRAL_PT_CUT = 20.;
const float TRUTH_JET_FORWARD_PT_CUT = 30.;

const float TRUTH_ELECTRON_ETA_CUT = 2.47;
const float TRUTH_MUON_ETA_CUT = 2.4;
const float TRUTH_TAU_ETA_CUT = 2.5;
const float TRUTH_JET_ETA_MAX_CUT = 4.5;

#endif
