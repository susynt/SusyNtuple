#ifndef SusyNtuple_SusyKin_h
#define SusyNtuple_SusyKin_h

// stl
#include <iostream>

// SusyNtuple
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"

using namespace Susy;

namespace SusyKin {

/* ----------------------------------------------------------------- */
/*  Event- and Object-level quantities [begin]                       */
/* ----------------------------------------------------------------- */

/////////////////////////////////////////////
// SUSY-ness
/////////////////////////////////////////////
bool eventHasSusyPropagators(const std::vector<int> &pdgs, const std::vector<std::vector<int>> &parentIndices);

/* ----------------------------------------------------------------- */
/*  Kinematic calculations [begin]                                   */
/* ----------------------------------------------------------------- */

//////////////////////////////////////////////
// SFOS removal methods
//////////////////////////////////////////////
void removeSFOSPair(ElectronVector& elecs, float MllCut);
void removeSFOSPair(MuonVector& muons, float MllCut);
void removeSFOSPair(TauVector& taus, float MllCut);

//////////////////////////////////////////////
// Lepton counting methods
//////////////////////////////////////////////
bool hasNLeptonsPtThreshold(const LeptonVector& leps, int nLepCut, float ptCut);

//////////////////////////////////////////////
// Lepton flavor and sign methods
//////////////////////////////////////////////
// check if two leptons are SF
bool isSameFlav(const Lepton* l1, const Lepton* l2);
bool isSameFlav(const Lepton& l1, const Lepton& l2);
// check if two leptons are OS
bool isOppSign(const Lepton* l1, const Lepton* l2);
bool isOppSign(const Lepton& l1, const Lepton& l2);
// check if two taus are OS
bool isOppSign(const Tau* tau1, const Tau* tau2);
bool isOppSign(const Tau& tau1, const Tau& tau2);
// check if a lepton-tau pair are OS
bool isOppSign(const Lepton* l1, const Tau* tau1);
bool isOppSign(const Lepton& l1, const Tau& tau1);
bool isOppSign(const Tau* tau1, const Lepton* l2);
bool isOppSign(const Tau& tau1, const Lepton& l2);

// check if two leptons form an SFOS pair
bool isSFOS(const Lepton* l1, const Lepton* l2);
bool isSFOS(const Lepton& l1, const Lepton& l2);
// check if two leptons form an SFSS pair
bool isSFSS(const Lepton* l1, const Lepton* l2);
bool isSFSS(const Lepton& l1, const Lepton& l2);
// check if two leptons form an OFOS pair
bool isOFOS(const Lepton* l1, const Lepton* l2);
bool isOFOS(const Lepton& l1, const Lepton& l2);

// check if a set of leptons has an SFOS pair
bool hasSFOS(const LeptonVector& leps);
// check if a set of leptons has an SFSS pair
bool hasSFSS(const LeptonVector& leps);
// check if a set of leptons has an OFOS pair
bool hasOFOS(const LeptonVector& leps);

// check if a set of leptons has an OS pair
bool hasOS(const LeptonVector& leps);
// check if a set of taus has an OS pair
bool hasOS(const TauVector& taus);

// check if a set of leptons has an SS pair
bool hasSS(const LeptonVector& leps);
// check if a set of taus has an SS pair
bool hasSS(const TauVector& taus);

//////////////////////////////////////////////
// Lepton selection methods
//////////////////////////////////////////////
// select SS lepton pair with highest pT
LeptonVector getSSLeptonPair(const LeptonVector& leps);


//////////////////////////////////////////////
// Mass calculation methods
//////////////////////////////////////////////
float Mll(const Particle* p1, const Particle* p2);
float Mll(const Particle& p1, const Particle& p2);

float Mlll(const Particle* p1, const Particle* p2, const Particle* p3);
float Mlll(const Particle& p1, const Particle& p2, const Particle& p3);

float Mllll(const Particle* p1, const Particle* p2, const Particle* p3, const Particle* p4);
float Mllll(const Particle& p1, const Particle& p2, const Particle& p3, const Particle& p4);

// di-jet mass
float Mjj(const Jet* j1, const Jet* j2);
float Mjj(const Jet& j1, const Jet& j2);

// di-lepton, d-jet mass
float Mlljj(const Lepton* l1, const Lepton* l2, const Jet* j1, const Jet* j2);
float Mlljj(const Lepton& l1, const Lepton& l2, const Jet& j1, const Jet& j2);

// mljj -- invariant mass of (dijet + closest lepton)-system
float Mljj(const LeptonVector& leptons, const JetVector& jets);

// W-transverse mass
float Mt(const TLorentzVector* lep, const Met* met);
float Mt(const TLorentzVector& lep, const Met& met);

// effective mass -- jets + met
float Meff(const JetVector& jets, const Met* met, float jetPtCut=40.);
float Meff(const JetVector& jets, const Met& met, float jetPtCut=40.);

// effecitve mass -- leptons + jets + met
float Meff(const LeptonVector& leps, const JetVector& jets, const Met* met, float jetPtCut=40.);
float Meff(const LeptonVector& leps, const JetVector& jets, const Met& met, float jetPtCut=40.);

// effective mass -- leptons + taus + jets + met
float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met* met, float jetPtCut=40.);
float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met& met, float jetPtCut=40.);

//////////////////////////////////////////////
// Z-selection methods
//////////////////////////////////////////////
// dilepton invariant mass within massWindow of Z
bool isZ(const Lepton* l1, const Lepton* l2, float massWindow = 10.);
bool isZ(const Lepton& l1, const Lepton& l2, float massWindow = 10.);

// trilepton invariant mass within massWindow of Z (there must be an SFOS pair)
bool isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, float massWindow = 10.);
bool isZ(const Lepton& l1, const Lepton& l2, const Lepton& l3, float massWindow = 10.);

// tetra-lepton invariant mass within massWindow of Z (there must be 2 SFOS pairs)
bool isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4, float massWindow = 10.);
bool isZ(const Lepton& l1, const Lepton& l2, const Lepton& l3, const Lepton& l4, float massWindow = 10.);

// check to see if two leptons have invariant mass within specified window around Z
bool isZWindow(const Lepton* l1, const Lepton* l2, float minMll = MZ-10., float maxMll = MZ+10.);
bool isZWindow(const Lepton& l1, const Lepton& l2, float minMll = MZ-10., float maxMll = MZ+10.);

// check to see if in a set of leptons there is an SFOS pair within the specified Z massWindow
bool hasZWindow(const LeptonVector& leps, float minMll = MZ-10., float maxMll = MZ+10.);

// check to see if a set of leptons is within the Z-window
bool hasZ(const LeptonVector& leps, float massWindow = 10., bool useMultiLep = false);
bool hasZ(const LeptonVector& leps, uint* Zl1, uint* Zl2, float massWindow = 10., bool useMultiLep = false);
bool hasZ(const LeptonVector& leps, uint& Zl1, uint& Zl2, float massWindow = 10., bool useMultiLep = false);

// check if three leptons are inside the massWindow around Z
bool hasZlll(const LeptonVector& leps, float massWindow = 10.);

// check if four leptons are inside the massWindow around Z
bool hasZllll(const LeptonVector& leps, float massWindow = 10.);

// check if two Z decay leptonically, provide the lepton indices for each Z
bool hasZllZll(const LeptonVector& leps, uint* Z1l1, uint* Z1l2, uint* Z2l1, uint* Z2l2, float massWindow = 10.);
bool hasZllZll(const LeptonVector& leps, uint& Z1l1, uint& Z1l2, uint& Z2l1, uint& Z2l2, float MassWindow = 10.);

// find indices for the lepton pair closest to the Z-mass
bool findBestZ(uint* l1, uint* l2, const LeptonVector& leps);
bool findBestZ(uint& l1, uint& l2, const LeptonVector& leps);

// find indices for the jet pair closest to the Z-mass
bool findBestZ(uint* j1, uint* j2, const JetVector& jets);
bool findBestZ(uint& j1, uint& j2, const JetVector& jets);

//////////////////////////////////////////////
// W-selection methods
//////////////////////////////////////////////
// find indices for the jet pair closest to the W-mass
bool findBestW(uint* j1, uint* j2, const JetVector& jets);
bool findBestW(uint& j1, uint& j2, const JetVector& jets);

//////////////////////////////////////////////
// MissingET Rel (METrel METRel metrel)
//////////////////////////////////////////////
float getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets);
float getMetRel(const Met& met, const LeptonVector& leptons, const JetVector& jets);

//////////////////////////////////////////////
// MT2 calculation methods (mt2 Mt2)
//////////////////////////////////////////////
// calculate lepton mt2, leptons are assumed massless (massless LSP)
float getMT2(const LeptonVector& leptons, const Met* met);
float getMT2(const LeptonVector& leptons, const Met& met);

// calculate mt2 using two visible objects p1 and p2 that are assumed massless (massless LSP)
float getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met);
float getMT2(const TLorentzVector& p1, const TLorentzVector& p2, const Met& met);

// calculate mt2 using two visible objects p1 and p2, specifiy whether to treat massless (set LSP mass)
float getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met, bool zeroMass, float lspMass = 0);
float getMT2(const TLorentzVector& p1, const TLorentzVector& p2, const Met& met, bool zeroMass, float lspMass = 0);

//////////////////////////////////////////////
// MCT calculation methods Mct mct
//////////////////////////////////////////////
float calcMCT(TLorentzVector v1, TLorentzVector v2);

//////////////////////////////////////////////
// HT calculation methods (Ht ht)
//////////////////////////////////////////////
// ht -- jets
float getHT(const JetVector& jets, float jetPtCut = 40.);

//////////////////////////////////////////////
// Calculate transverse thrust Transverse Thrust
//////////////////////////////////////////////
float getThrT(const LeptonVector& leptons);


//////////////////////////////////////////////
// Top Tagging top tagging methods
//////////////////////////////////////////////
// di-leptonic ttbar tagging method originally from SUSYTools
bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met, int op = 0, float ptJetCut = 0, float mEffCut = 100);
bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met& met, int op = 0, float ptJetCut = 0, float mEffCut = 100);
bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met);
bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l, TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int opt);

//////////////////////////////////////////////
// Super-Razor Super Razor Super razor super razor (leptonic) calculation
//////////////////////////////////////////////
void superRazor(const LeptonVector& leptons, const Met* met, TVector3& vBETA_z, TVector3& pT_CM,
                TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                double& gamma_R, double& dphi_vBETA_R_vBETA_T,
                double& MDELTAR, double& costhetaRp1);
void superRazor(const LeptonVector& leptons, const Met& met, TVector3& vBETA_z, TVector3& pT_CM,
                TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                double& gamma_R, double& dphi_vBETA_R_vBETA_T,
                double& MDELTAR, double& costhetaRp1);



} // SusyKin









#endif
