//  -*- c++ -*-
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
const float MW = 80.4;

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

enum DataStream {
    Stream_Unknown = 0,
    Stream_MC,
    Stream_PhysicsMain,
    Stream_Muons,
    Stream_Egamma,
    Stream_JetTauEtmiss,
    Stream_N
};

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
    ECut_TileTrip = 1 << 10,
    ECut_SCTErr = 1 << 11
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

// for pointer sorting
bool comparePt(const TLorentzVector* p1, const TLorentzVector* p2);
// find lepton in a collection
bool findLepton(const Susy::Lepton* lep, const LeptonVector& leptons);

// Dilepton specific
DiLepEvtType getDiLepEvtType(const LeptonVector& leptons);

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
