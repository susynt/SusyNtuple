// std
#include <iostream>

// SusyNtuple
#include "SusyNtuple/SusyKin.h"

#include "Mt2/mt2_bisect.h"

using namespace std;
using namespace Susy;

namespace SusyKin {

/* ------------------------------------------------------------------------------- */
/*  Event-level quantities [begin]                                                 */
/* ------------------------------------------------------------------------------- */

//////////////////////////////////////////////////////////////////
// Event-level cleaning cut methods
//////////////////////////////////////////////////////////////////
bool passGRL(int flags)         { return (flags & ECut_GRL);     }
bool passLArErr(int flags)      { return (flags & ECut_LarErr);  }
bool passTileErr(int flags)     { return (flags & ECut_TileErr); }
bool passTTCVeto(int flags)     { return (flags & ECut_TTC);     }
bool passBadMuon(int flags)     { return (flags & ECut_BadMuon); }
bool passJetCleaning(int flags) { return (flags & ECut_BadJet);  }
bool passGoodVtx(int flags)     { return (flags & ECut_GoodVtx); }
bool passCosmicVeto(int flags)  { return (flags & ECut_Cosmic);  }


//////////////////////////////////////////////////////////////////
// SUSY-ness
//////////////////////////////////////////////////////////////////
bool eventHasSusyPropagators(const std::vector<int>& pdgs,
            const std::vector<std::vector<int>>& parentIndices)
{
     // Loop over mc_* particles, find the C1 that is not a self-copy,
     // look at the mother and accept event only if it's Z0/gamma/W+-.
     // Code is not optmized in any meaningful way, feel free to do so
     // if you wish. Also please report any unwanted behavior to
     // amete@cern.ch
     const int  kPgam(+22), kPz(+23), kPw(+24), kPchargino1(1000024);
     size_t nParticles(pdgs.size());
     for (size_t ii = 0; ii < nParticles; ++ii) {
         int pdg(TMath::Abs(pdgs.at(ii)));
         if (pdg == kPchargino1) {
             for (unsigned int jj = 0; jj < parentIndices.at(ii).size(); ++jj) {
                 int parenPdg(TMath::Abs(pdgs.at(parentIndices.at(ii).at(jj))));
                 if (parenPdg == kPchargino1) break; // Self-copy
                 if (parenPdg != kPgam && parenPdg != kPz && parenPdg != kPw) {
                     return true;
                     //passDiagram=false;
                     //break;
                 }
             } // end of parent loop
         }
     } // end of truth particle loop

     return false;
}

/* ------------------------------------------------------------------------------- */
/*  Kinematic calculations [begin]                                                 */
/* ------------------------------------------------------------------------------- */

//////////////////////////////////////////////////////////////////
// SFOS removal methods
//////////////////////////////////////////////////////////////////

// electrons
void removeSFOSPair(ElectronVector& elecs, float MllCut)
{
    uint nEle = elecs.size();
    if (nEle<2) return;
    // remove all combinations of SFOS pairs with mll < MllCut
    ElectronVector electronsPass;
    // Use a double loop to check all combinations // dantrim : this is an odd way of doing things
    for(uint i=0; i<nEle; i++) {
        Electron* ei = elecs[i];
        bool pass = true;
        for(uint j=0; j<nEle; j++) {
            if(i==j) continue;
            Electron* ej = elecs[j];
            if(isSFOS(ei,ej) && (Mll(ei,ej) < MllCut)) {
                pass = false;
                break; // break out of j loop 
            }
        } // j
        if(pass) electronsPass.push_back(ei);
    } // i
    // replace supplied vector with the cleaned vector
    elecs = electronsPass;
}
// muons
void removeSFOSPair(MuonVector& muons, float MllCut)
{
    uint nMu = muons.size();
    if(nMu<2) return;
    // remove all combinations of SFOS pairs with mll < MllCut
    MuonVector muonsPass;
    // Use a double loop to check all combinations // dantrim : this is an odd way of doing things
    for(uint i=0; i<nMu; i++){
        Muon* mi = muons[i];
        bool pass = true;
        for(uint j=0; j<nMu; j++){
            if(i==j) continue;
            Muon* mj = muons[j];
            if(isSFOS(mi,mj) && (Mll(mi,mj) < MllCut)) {
                pass = false;
                break; // break out of j loop
            }
        } // j
        if(pass) muonsPass.push_back(mi);
    } // i
    // replace supplied vector with the cleaned vector
    muons = muonsPass;
}
// taus
void removeSFOSPair(TauVector& taus, float MllCut)
{
    uint nTau=taus.size();
    if(nTau<2) return;
    // remove all combinations of SFOS pairs with mll < MllCut
    TauVector tausPass;
    // Use a double loop to check all combinations
    for(uint i=0; i<nTau; i++) {
        Tau* ti = taus[i];
        bool pass = true;
        for(uint j=0; j<nTau; j++) {
            if(i==j) continue;
            Tau* tj = taus[j];
            if(isOppSign(ti, tj) && (Mll(ti,tj) < MllCut)) {
                pass = false;
                break; // break out of j loop
            }
        } // j
        if(pass) tausPass.push_back(ti);
    } // i
    // replaced supplied vector with the cleaned vector
    taus = tausPass;
}


//////////////////////////////////////////////////////////////////
// Lepton counting methods
//////////////////////////////////////////////////////////////////

bool hasNLeptonsPtThreshold(const LeptonVector& leps, int nLepCut, float ptCut)
{
    // check whether there are nLepCut many leptons with pT > ptCut
    int nPass=0;
    for(uint i=0; i<leps.size(); i++){
        if(leps.at(i)->Pt()<ptCut) continue;
        nPass++;
    }
    if(nPass >= nLepCut) return true;
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// Lepton flavor and sign methods
/////////////////////////////////////////////////////////////////////////////////

// check if two leptons are SF
bool isSameFlav(const Lepton* l1, const Lepton* l2)
{
    return isSameFlav(*l1, *l2);
}
bool isSameFlav(const Lepton& l1, const Lepton& l2)
{
    return ((l1.isEle() == l2.isEle()) && (l1.isMu() == l2.isMu()));
}

// check if two leptons are OS
bool isOppSign(const Lepton* l1, const Lepton* l2)
{
    return isOppSign(*l1, *l2);
}
bool isOppSign(const Lepton& l1, const Lepton& l2)
{
    return ( (l1.q*l2.q)<0 );
}

// check if two taus are opposite sign
bool isOppSign(const Tau* tau1, const Tau* tau2)
{
    return isOppSign(*tau1, *tau2);
}
bool isOppSign(const Tau& tau1, const Tau& tau2)
{
    return ( (tau1.q*tau2.q)<0 );
}

// check if lep+tau are opposite sign
bool isOppSign(const Lepton* lep, const Tau* tau)
{
    return isOppSign(*lep, *tau);
}
bool isOppSign(const Lepton& lep, const Tau& tau)
{
    return ( (lep.q*tau.q)<0 );
}
bool isOppSign(const Tau* tau, const Lepton* lep)
{
    return isOppSign(*tau, *lep);
}
bool isOppSign(const Tau& tau, const Lepton& lep)
{
    return ( (tau.q*lep.q)<0 );
}

// check if two leptons are SFOS
bool isSFOS(const Lepton* l1, const Lepton* l2)
{
    return isSFOS(*l1, *l2);
}
bool isSFOS(const Lepton& l1, const Lepton& l2)
{
    return ( isSameFlav(l1, l2) && isOppSign(l1, l2) );
}

// check if two leptons are SFSS
bool isSFSS(const Lepton* l1, const Lepton* l2)
{
    return isSFSS(*l1, *l2);
}
bool isSFSS(const Lepton& l1, const Lepton& l2)
{
    return ( isSameFlav(l1, l2) && !isOppSign(l1, l2) );
}

// check if two leptons are OFOS
bool isOFOS(const Lepton* l1, const Lepton* l2)
{
    return isOFOS(*l1, *l2);
}
bool isOFOS(const Lepton& l1, const Lepton& l2)
{
    return ( !isSameFlav(l1, l2) && isOppSign(l1, l2) );
}

// check if a set of leptons has an SFOS pair
bool hasSFOS(const LeptonVector& leps)
{
    uint nLep = leps.size();
    for(uint i=0; i<nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            if(isSFOS(leps[i], leps[j])) return true;
        } // j
    } // i
    return false;
}

// check if a set of leptons has an SFSS pair
bool hasSFSS(const LeptonVector& leps)
{
    uint nLep=leps.size();
    for(uint i=0; i<nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            if(isSFSS(leps[i],leps[j])) return true;
        } // j
    } // i
    return false;
}

// check if a set of leptons has an OFOS pair
bool hasOFOS(const LeptonVector& leps)
{
    uint nLep=leps.size();
    for(uint i=0; i<nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            if(isOFOS(leps[i],leps[j])) return true;
        } // j
    } // i
    return false;
}

// check if a set of leptons has an OS pair
bool hasOS(const LeptonVector& leps)
{
    uint nLep=leps.size();
    for(uint i=0; i<nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            if(isOppSign(leps[i],leps[j])) return true;
        } // j
    } // i
    return false;
}
bool hasOS(const TauVector& taus)
{
    uint nTau=taus.size();
    for(uint i=0; i<nTau; i++){
        for(uint j=i+1; j<nTau; j++){
            if(isOppSign(taus[i],taus[j])) return true;
        } // j
    } // i
    return false;
}

// check if a set of leptons has an SS pair
bool hasSS(const LeptonVector& leps)
{
    uint nLep=leps.size();
    for(uint i=0; i<nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            if(!isOppSign(leps[i],leps[j])) return true;
        } // j
    } // i
    return false;
}
bool hasSS(const TauVector& taus)
{
    uint nTau=taus.size();
    for(uint i=0; i<nTau; i++){
        for(uint j=i+1; j<nTau; j++){
            if(!isOppSign(taus[i],taus[j])) return true;
        } // j
    } // i
    return false;
}
    
/////////////////////////////////////////////////////////////////////////////////
// Lepton selection methods
/////////////////////////////////////////////////////////////////////////////////
// select SS lepton pair with highest pT
LeptonVector getSSLeptonPair(const LeptonVector& leps)
{
    // We assume that the input lepton vector is already pT sorted
    LeptonVector ssPair;
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if(isOppSign(leps.at(i),leps.at(j))) continue;
            ssPair.clear();
            ssPair.push_back(leps.at(i));
            ssPair.push_back(leps.at(j));
            break;
        }
    }
    return ssPair;
}

/////////////////////////////////////////////////////////////////////////////////
// Mass calculation methods
/////////////////////////////////////////////////////////////////////////////////

// invariant mass of two Particle objects
float Mll(const Particle* p1, const Particle* p2)
{
    return Mll(*p1, *p2);
}
float Mll(const Particle& p1, const Particle& p2)
{
    return (p1 + p2).M();
}

// tri-particle invariant mass
float Mlll(const Particle* p1, const Particle* p2, const Particle* p3)
{
    return Mlll(*p1, *p2, *p3);
}
float Mlll(const Particle& p1, const Particle& p2, const Particle& p3)
{
    return (p1 + p2 + p3).M();
}

// tetra-particle invariant mass
float Mllll(const Particle* p1, const Particle* p2, const Particle* p3, const Particle* p4)
{
    return Mllll(*p1, *p2, *p3, *p4);
}
float Mllll(const Particle& p1, const Particle& p2, const Particle& p3, const Particle& p4)
{
    return (p1 + p2 + p3 + p4).M();
}

// di-jet mass (Susy::Jet inherits from Susy::Particle so this method can be subs. with Mll above)
float Mjj(const Jet* j1, const Jet* j2)
{
    return Mjj(*j1, *j2);
}
float Mjj(const Jet& j1, const Jet& j2)
{
    return (j1 + j2).M();
}

// di-lepton, di-jet mass
float Mlljj(const Lepton* l1, const Lepton* l2, const Jet* j1, const Jet* j2)
{
    return Mlljj(*l1, *l2, *j1, *j2);
}
float Mlljj(const Lepton& l1, const Lepton&l2, const Jet& j1, const Jet& j2)
{
    return (l1 + l2 + j1 + j2).M();
}

// mljj -- invariant mass of (dijet + closest lepton)-system
float Mljj(const LeptonVector&  leptons, const JetVector& jets)
{
    if (jets.size() < 1 || leptons.size() < 2) return -999;

    const Lepton& l0 = *leptons.at(0);
    const Lepton& l1 = *leptons.at(1);
    TLorentzVector jj(*jets.at(0));
    if (jets.size() >= 2) jj += *jets.at(1);

    float dR1 = jj.DeltaR(l0);
    float dR2 = jj.DeltaR(l1);

    return (dR1 < dR2) ? (jj + l0).M() : (jj + l1).M();
}

// W-transverse mass
float Mt(const TLorentzVector* lep, const Met* met)
{
    return Mt(*lep, *met);
}
float Mt(const TLorentzVector& lep, const Met& met)
{
    return sqrt(2.*lep.Pt()*met.Et*(1-cos(lep.DeltaPhi((met.lv())))));
}

// effective mass -- jets + met
float Meff(const JetVector& jets, const Met* met, float jetPtCut)
{
    return Meff(jets, *met, jetPtCut);
}
float Meff(const JetVector& jets, const Met& met, float jetPtCut)
{
    float meff=0.;
    for(uint i=0; i<jets.size(); i++){
        if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if(&met) meff += met.Et;
    return meff;
}

// effective mass -- leptons + jets + met
float Meff(const LeptonVector& leps, const JetVector& jets, const Met* met, float jetPtCut)
{
    return Meff(leps, jets, *met, jetPtCut);
}
float Meff(const LeptonVector& leps, const JetVector& jets, const Met& met, float jetPtCut)
{
    float meff=0.;
    for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
    for(uint i=0; i<jets.size(); i++){
        if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if(&met) meff += met.Et;
    return meff;
}

// effective mass -- leptons + taus + jets + met
float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met* met, float jetPtCut)
{
    return Meff(leps, taus, jets, *met, jetPtCut);
}
float Meff(const LeptonVector& leps, const TauVector& taus, const JetVector& jets, const Met& met, float jetPtCut)
{
    float meff=0.;
    for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
    for(uint i=0; i<taus.size(); i++) meff += taus[i]->Pt();
    for(uint i=0; i<jets.size(); i++){
        if(jets[i]->Pt() > jetPtCut) meff += jets[i]->Pt();
    }
    if(&met) meff += met.Et;
    return meff;
}
    

/////////////////////////////////////////////////////////////////////////////////
// Z-selection methods
/////////////////////////////////////////////////////////////////////////////////

// dilepton, SFOS invariant mass within massWindow of Z
bool isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{
    return isZ(*l1, *l2, massWindow);
}
bool isZ(const Lepton& l1, const Lepton& l2, float massWindow)
{
    return ( isSFOS(l1, l2) && (fabs(Mll(l1, l2)-MZ) < massWindow) );
}

// trilepton invariant mass within massWindow of Z (there must be an SFOS pair)
bool isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, float massWindow)
{
    return isZ(*l1, *l2, *l3, massWindow);
}
bool isZ(const Lepton& l1, const Lepton& l2, const Lepton& l3, float massWindow)
{
    return ( (isSFOS(l1,l2) || isSFOS(l1,l3) || isSFOS(l2,l3)) && (fabs(Mlll(l1,l2,l3)-MZ) < massWindow) );
}

// tetra-lepton invariant mass within massWindow of Z (there must be 2 SFOS pairs)
bool isZ(const Lepton* l1, const Lepton* l2, const Lepton* l3, const Lepton* l4, float massWindow)
{
    return isZ(*l1, *l2, *l3, *l4, massWindow);
}
bool isZ(const Lepton& l1, const Lepton& l2, const Lepton& l3, const Lepton& l4, float massWindow)
{
    // Require 2 SFOS pairs
    if( ( (isSFOS(l1,l2) && isSFOS(l3,l4)) ||
          (isSFOS(l1,l3) && isSFOS(l2,l4)) ||
          (isSFOS(l1,l4) && isSFOS(l2,l3)) ) &&
          (fabs(Mllll(l1,l2,l3,l4) - MZ) < massWindow) ) return true;
    return false;
}

// check to see if two leptons have invariant mass within specified window around Z
bool isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
{
    return isZWindow(*l1, *l2, minMll, maxMll);
}
bool isZWindow(const Lepton& l1, const Lepton& l2, float minMll, float maxMll)
{
    float mll = Mll(l1,l2);
    return ( isSFOS(l1,l2) && (mll>minMll && mll<maxMll) );
}

// check to see if in a set of leptons there is an SFOS pair within the specified Z massWindow
bool hasZWindow(const LeptonVector& leps, float minMll, float maxMll)
{
    for(uint i=0; i<leps.size(); i++){
        for(uint j=i+1; j<leps.size(); j++){
            if(isZWindow(leps[i],leps[j], minMll, maxMll)) return true;
        } // j
    } // i
    return false;
}

bool hasZ(const LeptonVector& leps, float massWindow, bool useMultiLep)
{
    uint dummy1;
    uint dummy2;
    return hasZ(leps, &dummy1, &dummy2, massWindow, useMultiLep);
}
bool hasZ(const LeptonVector& leps, uint* Zl1, uint* Zl2, float massWindow, bool useMultiLep)
{
    return hasZ(leps, *Zl1, *Zl2, massWindow, useMultiLep);
}
bool hasZ(const LeptonVector& leps, uint& Zl1, uint& Zl2, float massWindow, bool useMultiLep)
{
    uint nLep=leps.size();
    for(uint i=0; i< nLep; i++){
        for(uint j=i+1; j<nLep; j++){
            // check for Z->ll
            if(isZ(leps[i], leps[j], massWindow)){
                Zl1=i;
                Zl2=j;
                return true;
            }
            if(useMultiLep){
                for(uint k=j+1; k<nLep; k++){
                    // check for Z->lll(l)
                    if(isZ(leps[i], leps[j], leps[k], massWindow)) return true;
                    for(uint l=k+1; l<nLep; l++){
                        // check for Z->llll
                        if(isZ(leps[i], leps[j], leps[k], leps[l], massWindow)) return true;
                    } // l
                } // k
            } // useMultiLep
        } // j
    }// i
    return false;
}

// check if three leptons are inside the massWindow around Z
bool hasZlll(const LeptonVector& leps, float massWindow)
{
    for(uint i=0; i<leps.size(); i++){
        for(uint j=i+1; j<leps.size(); j++){
            for(uint k=j+1; k<leps.size(); k++){
                if(isZ(leps[i], leps[j], leps[k], massWindow)) return true;
            } // k
        } // j
    } // k
    return false;
}

// check if four leptons are inside the massWindow around Z
bool hasZllll(const LeptonVector& leps, float massWindow)
{
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            for (uint k = j + 1; k < leps.size(); k++) {
                for (uint l = k + 1; l < leps.size(); l++) {
                    if (isZ(leps[i], leps[j], leps[k], leps[l], massWindow)) return true;
                }
            }
        }
    }
    return false;
}

// check if two Z decay leptonically, provide the lepton indices for each Z
bool hasZllZll(const LeptonVector& leps, uint* Z1l1, uint* Z1l2, uint* Z2l1, uint* Z2l2, float massWindow)
{
    return hasZllZll(leps, *Z1l1, *Z1l2, *Z2l1, *Z2l2, massWindow);
}
bool hasZllZll(const LeptonVector& leps, uint& Z1l1, uint& Z1l2, uint& Z2l1, uint& Z2l2, float massWindow)
{
    // find first pair
    for (uint i = 0; i < leps.size(); i++) {
        for (uint j = i + 1; j < leps.size(); j++) {
            if (isZ(leps[i], leps[j], massWindow)) {
                Z1l1 = i;
                Z1l2 = j;
                // find second pair
                for (uint k = 0; k < leps.size(); k++) {
                    if (k == i || k == j) continue;
                    for (uint l = k + 1; l < leps.size(); l++) {
                        if (l == i || l == j) continue;
                        if (isZ(leps[k], leps[l], massWindow)) {
                            Z2l1 = k;
                            Z2l2 = l;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// find indices for the lepton pair closest to the Z-mass
bool findBestZ(uint* l1, uint* l2, const LeptonVector& leps)
{
    return findBestZ(*l1, *l2, leps);
}
bool findBestZ(uint& l1, uint& l2, const LeptonVector& leps)
{
    float minDM = -1;
    uint nLep = leps.size();
    for (uint i = 0; i < nLep; i++) {
        for (uint j = i + 1; j < nLep; j++) {
            if (!isSFOS(leps[i], leps[j])) continue;
            float dM = fabs(Mll(leps[i], leps[j]) - MZ);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                l1 = i;
                l2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}

// find indices for the jet pair closest to the Z-mass
bool findBestZ(uint* j1, uint* j2, const JetVector& jets)
{
    return findBestZ(*j1, *j2, jets);
}
bool findBestZ(uint& j1, uint& j2, const JetVector& jets)
{
    float minDM = -1;
    uint nJet = jets.size();
    for (uint i = 0; i < nJet; i++) {
        for (uint j = i + 1; j < nJet; j++) {
            float dM = fabs(Mjj(jets[i], jets[j]) - MZ);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                j1 = i;
                j2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// W-selection methods
/////////////////////////////////////////////////////////////////////////////////

// find indices for the jet pair closest to the W-mass
bool findBestW(uint* j1, uint* j2, const JetVector& jets)
{
    return findBestW(*j1, *j2, jets);
}
bool findBestW(uint& j1, uint& j2, const JetVector& jets)
{
    float minDM = -1;
    uint nJet = jets.size();
    for (uint i = 0; i < nJet; i++) {
        for (uint j = i + 1; j < nJet; j++) {
            float dM = fabs(Mjj(jets[i], jets[j]) - MW);
            if (minDM < 0 || dM < minDM) {
                minDM = dM;
                j1 = i;
                j2 = j;
            }
        }
    }
    if (minDM < 0) return false;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// MissingET Rel (METrel METRel metrel)
/////////////////////////////////////////////////////////////////////////////////
float getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets)
{
    return getMetRel(*met, leptons, jets);
}
float getMetRel(const Met& met, const LeptonVector& leptons, const JetVector& jets)
{
    const TLorentzVector metLV = met.lv();
    float dPhi = TMath::Pi() / 2.;

    for (uint il = 0; il < leptons.size(); ++il)
        if (fabs(metLV.DeltaPhi(*leptons.at(il))) < dPhi)
            dPhi = fabs(metLV.DeltaPhi(*leptons.at(il)));
    for (uint ij = 0; ij < jets.size(); ++ij) {
        const Jet* jet = jets.at(ij);
        if (fabs(metLV.DeltaPhi(*jet)) < dPhi)
            dPhi = fabs(metLV.DeltaPhi(*jet));
    }// end loop over jets
    return metLV.Et() * sin(dPhi);
}

/////////////////////////////////////////////////////////////////////////////////
// MT2 calculation methods (mt2 Mt2)
/////////////////////////////////////////////////////////////////////////////////

// calculate leptonic mt2, leptons are assumed massless (massless LSP)
float getMT2(const LeptonVector& leptons, const Met* met)
{
    return getMT2(leptons, *met);
}
float getMT2(const LeptonVector& leptons, const Met& met)
{
    if (leptons.size() < 2) return -999;

    // necessary variables
    TLorentzVector metlv = met.lv();
    TLorentzVector l0 = *leptons.at(0);
    TLorentzVector l1 = *leptons.at(1);

    double pTMiss[3] = { 0.0, metlv.Px(), metlv.Py() };
    double pA[3] = { 0.0, l0.Px(), l0.Py() };
    double pB[3] = { 0.0, l1.Px(), l1.Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(0); // LSP mass = 0 is Generic

    return mt2_event.get_mt2();
}

// calculate mt2 using two visible objects p1 and p2 that are assumed massless (massless LSP)
float getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met)
{
    return getMT2(*p1, *p2, *met);
}
float getMT2(const TLorentzVector& p1, const TLorentzVector& p2, const Met& met)
{
    // necessary variables
    TLorentzVector metLV = met.lv();

    double pTMiss[3] = { 0.0, metLV.Px(), metLV.Py() };
    double pA[3] = { 0.0, p1.Px(), p1.Py() };
    double pB[3] = { 0.0, p2.Px(), p2.Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(0); // LSP mass = 0 is Generic

    return mt2_event.get_mt2();
}

// calculate mt2 using two visible objects p1 and p2, specifiy whether to treat massless (set LSP mass)
float getMT2(const TLorentzVector* p1, const TLorentzVector* p2, const Met* met, bool zeroMass, float lspMass)
{
    return getMT2(*p1, *p2, *met, zeroMass, lspMass);
}
float getMT2(const TLorentzVector& p1, const TLorentzVector& p2, const Met& met, bool zeroMass, float lspMass)
{
    // necessary variables
    TLorentzVector metLV = met.lv();

    double pTMiss[3] = { 0.0, metLV.Px(), metLV.Py() };
    double pA[3] = { (zeroMass) ? 0.0 : p1.M(), p1.Px(), p1.Py() };
    double pB[3] = { (zeroMass) ? 0.0 : p2.M(), p2.Px(), p2.Py() };

    // Create Mt2 object
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta(pA, pB, pTMiss);
    mt2_event.set_mn(lspMass);

    return mt2_event.get_mt2();

}

/////////////////////////////////////////////////////////////////////////////////
// MCT calculation methods Mct mct
/////////////////////////////////////////////////////////////////////////////////
float calcMCT(TLorentzVector v1, TLorentzVector v2)
{
    float mct = (v1.Mt() + v2.Mt())*(v1.Mt() + v2.Mt()) - (v1 - v2).Perp2();
    mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);
    return mct;
}

/////////////////////////////////////////////////////////////////////////////////
// HT calculation methods (Ht ht)
/////////////////////////////////////////////////////////////////////////////////

// ht -- jets
float getHT(const JetVector& jets, float jetPtCut)
{
    float ht = 0;
    for (uint i = 0; i < jets.size(); i++) {
        float pt = jets[i]->Pt();
        if (pt > jetPtCut) ht += pt;
    }
    return ht;
}

/////////////////////////////////////////////////////////////////////////////////
// Calculate transverse thrust Transverse Thrust
/////////////////////////////////////////////////////////////////////////////////
float getThrT(const LeptonVector& leptons)
{
    float sumPx = 0;
    float sumPy = 0;
    float sumPt = 0;
    float max = 0;

    for (uint i = 0; i < leptons.size(); i++) {
        sumPx += leptons[i]->Px();
        sumPy += leptons[i]->Py();
        sumPt += leptons[i]->Pt();
    }
    float nx = sumPx / sqrt(sumPx*sumPx + sumPy*sumPy);
    float ny = sumPy / sqrt(sumPx*sumPx + sumPy*sumPy);

    for (uint i = 0; i < leptons.size(); i++) {
        max += fabs(leptons[i]->Px()*nx + leptons[i]->Py()*ny);
    }
    return max / sumPt;
}

/////////////////////////////////////////////////////////////////////////////////
// Top Tagging top tagging methods
/////////////////////////////////////////////////////////////////////////////////

// di-lepton ttbar tagging method originally from SUSYTools
bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met, int opt, float ptJetCut, float mEffCut)
{
    return passTopTag(leptons, jets, *met, opt, ptJetCut, mEffCut);
}
bool passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met& met, int opt, float ptJetCut, float mEffCut)
{
    // Defaults opt = 0, ptJetCut = 0 GeV, mEffCut = 100 GeV
    // Tagging is based on number of jets and at least two leptons
    if (leptons.size() < 2) return false;

    // Define some necessary vars
    uint njets = jets.size();
    uint njetscan = njets < 3 ? njets : 3; // Only consider up to 3 jets
    TLorentzVector v1l = *leptons.at(0);
    TLorentzVector v2l = *leptons.at(1);
    double phi = met.phi;
    double Et = met.Et;
    TVector2 metv2(Et * cos(phi), Et * sin(phi));

    // Check to see if our leptons and met is
    // consistent with a W candidate
    bool maybeW = toptag0j(v1l, v2l, metv2);
    if (!maybeW)            return true;
    if (jets.size() < 2)  return true;

    // Check to see if we have top
    int npairs = 0;
    for (uint i1 = 0; i1 < njetscan - 1; ++i1) {
        if (i1 == 0 || opt == 0) { // opt=0 implies compare all jets
            for (uint i2 = i1 + 1; i2 < njetscan; ++i2) {
                TLorentzVector v1j = *jets.at(i1);
                TLorentzVector v2j = *jets.at(i2);
                bool maybeTop = toptag2j(ptJetCut, mEffCut, v1l, v2l, v1j, v2j, metv2, opt);
                if (maybeTop) npairs++;
            }// end loop over second jet
        }// end if options
    }// end loop over first jet

    if (npairs > 0) return false;
    return true;
}

bool toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met)
{
    // Taken from TopTag.cxx in SUSYTools

    static const Double_t wmass = 80.4;
    double metx = met.X();
    double mety = met.Y();

    // First mct(ll)
    //---------------
    double mctll = calcMCT(v1l, v2l);

    double pxus = v1l.Px() + v2l.Px() + metx;
    double pyus = v1l.Py() + v2l.Py() + mety;
    double rr = sqrt(pxus*pxus + pyus*pyus) / (2.*wmass);
    double fact = rr + sqrt(1 + rr*rr);

    //cout << "mctll " << mctll << " cut value " << wmass*fact << std::endl;
    return mctll < wmass*fact ? true : false;
}
bool toptag2j(double ptjcut, double meffcut, TLorentzVector v1l, TLorentzVector v2l, TLorentzVector v1j, TLorentzVector v2j, TVector2 met, int opt)
{
    // Taken from TopTag.cxx in SUSYTools

    //
    const double tmass = 172.5;
    const double mljcut = 155.;
    const double mctjcut = 137.;
    //
    TLorentzVector vjb[2];
    double ml1j[2];
    double ml2j[2];
    //
    double metx = met.X();  double mety = met.Y();
    double ptl1 = v1l.Pt();   double ptl2 = v2l.Pt();
    double ptj1 = v1j.Pt();   double ptj2 = v2j.Pt();
    double pxus = v1l.Px() + v2l.Px() + metx;
    double pyus = v1l.Py() + v2l.Py() + mety;
    double mefftop = ptj1 + ptj2 + ptl1 + ptl2;

    if (ptj2 < ptjcut) return false;
    if (opt == 0 && mefftop < meffcut) return false;

    // First part:
    //-------------
    vjb[0] = v1j;
    vjb[1] = v2j;
    double mctjj = calcMCT(vjb[0], vjb[1]);
    double pxusjl = vjb[0].Px() + vjb[1].Px() + pxus;
    double pyusjl = vjb[0].Py() + vjb[1].Py() + pyus;
    double rrj = sqrt(pxusjl*pxusjl + pyusjl*pyusjl) / (2.*tmass);
    double factj = rrj + sqrt(1 + rrj*rrj);

    bool imct = mctjj < mctjcut*factj ? true : false;
    if (opt == 1) return imct;

    // Second part:
    //--------------
    for (int i = 0; i < 2; ++i) {
        ml1j[i] = (v1l + vjb[i]).M();
        ml2j[i] = (v2l + vjb[i]).M();
    }
    int ncou = 0;
    int icou1[2];
    int icou2[2];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (i != j) {
                if (ml1j[i] < mljcut && ml2j[j] < mljcut) {
                    icou1[ncou] = i;
                    icou2[ncou] = j;
                    ncou++;
                }
            }
        }
    }
    bool imjl = ncou > 0 ? true : false;

    // Third part: finally mct(ql,ql) for each coupling passing the mjl cut
    //-------------exploit the dependence of mct((jl),(jl)) on the maximum
    //             mass^2 of the two jl combinations
    int ngcou = 0;
    for (int i = 0; i<ncou; ++i) {
        int ij1 = icou1[i];
        int ij2 = icou2[i];
        TLorentzVector vtot1 = vjb[ij1] + v1l;
        TLorentzVector vtot2 = vjb[ij2] + v2l;
        double mctjl = calcMCT(vtot1, vtot2);
        double mm1 = (vjb[ij1] + v1l).M();
        double mm2 = (vjb[ij2] + v2l).M();
        double mmax2 = mm1>mm2 ? (mm1*mm1) : (mm2*mm2);
        double upl = mmax2 / (tmass)+(tmass);
        //cout << " i " << " mctjl " << mctjl << " mmax2 " << mmax2 <<
        //" upl " << upl << endl;
        if (mctjl < upl*factj)  ngcou++;
    }

    bool imctjl = ngcou > 0 ? true : false;
    //cout << " ll " << imctll << " ct " << imct << " jl " << imjl <<
    //" ctlj " << imctjl << endl;

    return imct & imjl & imctjl;
}

/////////////////////////////////////////////////////////////////////////////////
// Super-Razor Super Razor Super razor super razor (leptonic) calculation
/////////////////////////////////////////////////////////////////////////////////
void superRazor(const LeptonVector& leptons, const Met* met, TVector3& vBETA_z, TVector3& pT_CM,
                TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                double& gamma_R, double& dphi_vBETA_R_vBETA_T,
                double& MDELTAR, double& costhetaRp1)
{

    return superRazor(leptons, *met, vBETA_z, pT_CM,
                        vBETA_T_CMtoR, vBETA_R,
                        SHATR, dphi_LL_vBETA_T, dphi_L1_L2,
                        gamma_R, dphi_vBETA_R_vBETA_T,
                        MDELTAR, costhetaRp1);
}
void superRazor(const LeptonVector& leptons, const Met& met, TVector3& vBETA_z, TVector3& pT_CM,
                TVector3& vBETA_T_CMtoR, TVector3& vBETA_R,
                double& SHATR, double& dphi_LL_vBETA_T, double& dphi_L1_L2,
                double& gamma_R, double& dphi_vBETA_R_vBETA_T,
                double& MDELTAR, double& costhetaRp1)
{
    //
    // Code written by Christopher Rogan <crogan@cern.ch>, 04-23-13
    // Details given in paper (http://arxiv.org/abs/1310.4827) written by
    // Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
    //
    if (leptons.size() < 2) return;

    // necessary variables
    TLorentzVector metlv = met.lv();
    TLorentzVector l0 = *leptons.at(0);
    TLorentzVector l1 = *leptons.at(1);

    //
    // Lab frame
    //
    //Longitudinal boost
    vBETA_z = (1. / (l0.E() + l1.E()))*(l0 + l1).Vect();
    vBETA_z.SetX(0.0);
    vBETA_z.SetY(0.0);

    l0.Boost(-vBETA_z);
    l1.Boost(-vBETA_z);

    //pT of CM frame
    pT_CM = (l0 + l1).Vect() + metlv.Vect();
    pT_CM.SetZ(0.0);

    TLorentzVector ll = l0 + l1;
    //invariant mass of the total event
    SHATR = sqrt(2.*(ll.E()*ll.E() - ll.Vect().Dot(pT_CM)
        + ll.E()*sqrt(ll.E()*ll.E() + pT_CM.Mag2() - 2.*ll.Vect().Dot(pT_CM))));

    vBETA_T_CMtoR = (1. / sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;

    l0.Boost(-vBETA_T_CMtoR);
    l1.Boost(-vBETA_T_CMtoR);
    ll.Boost(-vBETA_T_CMtoR);

    //
    //R-frame
    //
    dphi_LL_vBETA_T = fabs((ll.Vect()).DeltaPhi(vBETA_T_CMtoR));

    dphi_L1_L2 = fabs(l0.Vect().DeltaPhi(l1.Vect()));

    vBETA_R = (1. / (l0.E() + l1.E()))*(l0.Vect() - l1.Vect());

    gamma_R = 1. / sqrt(1. - vBETA_R.Mag2());

    dphi_vBETA_R_vBETA_T = fabs(vBETA_R.DeltaPhi(vBETA_T_CMtoR));

    l0.Boost(-vBETA_R);
    l1.Boost(vBETA_R);

    //
    //R+1 frame
    //
    MDELTAR = 2.*l0.E();
    costhetaRp1 = l0.Vect().Dot(vBETA_R) / (l0.Vect().Mag()*vBETA_R.Mag());

    return;
}


} // namespace SusyKin
