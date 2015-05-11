#include "SusyNtuple/Jet.h"

#include <iostream>
#include <iomanip>

using namespace std;

using Susy::Jet;
using Susy::Particle;

/*--------------------------------------------------------------------------------*/
// Copy constructor
/*--------------------------------------------------------------------------------*/
Jet::Jet(const Jet &rhs):
  Particle(rhs),
  jvf(rhs.jvf),
  jvt(rhs.jvt),
  detEta(rhs.detEta),
  emfrac(rhs.emfrac),
  truthLabel(rhs.truthLabel),
  matchTruth(rhs.matchTruth),
  nTracks(rhs.nTracks),
  sv0(rhs.sv0),
  combNN(rhs.combNN),
  mv1(rhs.mv1),
  sv1plusip3d(rhs.sv1plusip3d),
  jfit_mass(rhs.jfit_mass),
  sv0p_mass(rhs.sv0p_mass),
  svp_mass (rhs.svp_mass ),
  isBadVeryLoose(rhs.isBadVeryLoose),
  isHotTile(rhs.isHotTile),
  bch_corr_jet(rhs.bch_corr_jet),
  bch_corr_cell(rhs.bch_corr_cell),
  isBadMediumBCH(rhs.isBadMediumBCH),
  isBadMediumBCH_up(rhs.isBadMediumBCH_up),
  isBadMediumBCH_dn(rhs.isBadMediumBCH_dn),
  isBadTightBCH(rhs.isBadTightBCH),
  jes_up(rhs.jes_up),
  jes_dn(rhs.jes_dn),
  jer(rhs.jer),
  bjes(rhs.bjes),
  effNp(rhs.effNp),
  etaInter(rhs.etaInter),
  flavor(rhs.flavor),
  pileup(rhs.pileup),
  punchThrough(rhs.punchThrough),
  // relativeNC(rhs.relativeNC),
  singlePart(rhs.singlePart)
{ 
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Jet& Jet::operator=(const Jet &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    jvf  = rhs.jvf; 
    jvt  = rhs.jvt; 
    detEta = rhs.detEta;
    emfrac = rhs.emfrac;
    truthLabel = rhs.truthLabel;
    matchTruth = rhs.matchTruth;
    nTracks = rhs.nTracks;
    sv0 = rhs.sv0;
    combNN = rhs.combNN;
    mv1 = rhs.mv1;
    jfit_mass = rhs.jfit_mass;
    sv0p_mass = rhs.sv0p_mass;
    svp_mass  = rhs.svp_mass ;
    isBadVeryLoose = rhs.isBadVeryLoose;
    isHotTile = rhs.isHotTile;
    bch_corr_jet = rhs.bch_corr_jet;
    bch_corr_cell = rhs.bch_corr_cell;
    isBadMediumBCH = rhs.isBadMediumBCH;
    isBadMediumBCH_up = rhs.isBadMediumBCH_up;
    isBadMediumBCH_dn = rhs.isBadMediumBCH_dn;
    isBadTightBCH = rhs.isBadTightBCH;
    jes_up = rhs.jes_up;
    jes_dn = rhs.jes_dn;
    jer = rhs.jer;
    bjes=rhs.bjes;
    effNp=rhs.effNp;
    etaInter=rhs.etaInter;
    flavor=rhs.flavor;
    pileup=rhs.pileup;
    punchThrough=rhs.punchThrough;
    //relativeNC=rhs.relativeNC;
    singlePart=rhs.singlePart;
  }
  return *this;
}
/*--------------------------------------------------------------------------------*/
// Jet Set State
/*--------------------------------------------------------------------------------*/
void Jet::setState(int sys)
{
  resetTLV();
  if(sys == NtSys::NOM) return;
  
  float sf = 0;
  if     ( sys == NtSys::JER) sf = jer;
  else if( sys == NtSys::JET_BJES_Response_DN) sf = bjes[0];
  else if( sys == NtSys::JET_BJES_Response_UP) sf = bjes[1];
  else if( sys == NtSys::JET_EffectiveNP_1_DN) sf = effNp[0];
  else if( sys == NtSys::JET_EffectiveNP_1_UP) sf = effNp[1];
  else if( sys == NtSys::JET_EffectiveNP_2_DN) sf = effNp[2];
  else if( sys == NtSys::JET_EffectiveNP_2_UP) sf = effNp[3];
  else if( sys == NtSys::JET_EffectiveNP_3_DN) sf = effNp[4];
  else if( sys == NtSys::JET_EffectiveNP_3_UP) sf = effNp[5];
  else if( sys == NtSys::JET_EffectiveNP_4_DN) sf = effNp[6];
  else if( sys == NtSys::JET_EffectiveNP_4_UP) sf = effNp[7];
  else if( sys == NtSys::JET_EffectiveNP_5_DN) sf = effNp[8];
  else if( sys == NtSys::JET_EffectiveNP_5_UP) sf = effNp[9];
  else if( sys == NtSys::JET_EffectiveNP_6restTerm_DN) sf = effNp[10];
  else if( sys == NtSys::JET_EffectiveNP_6restTerm_UP) sf =  effNp[11];
  else if( sys == NtSys::JET_EtaIntercalibration_Modelling_DN) sf = etaInter[0];
  else if( sys == NtSys::JET_EtaIntercalibration_Modelling_UP) sf = etaInter[1];
  else if( sys == NtSys::JET_EtaIntercalibration_TotalStat_DN) sf = etaInter[2];
  else if( sys == NtSys::JET_EtaIntercalibration_TotalStat_UP) sf = etaInter[3];
  else if( sys == NtSys::JET_Flavor_Composition_DN) sf = flavor[0];
  else if( sys == NtSys::JET_Flavor_Composition_UP) sf = flavor[1];
  else if( sys == NtSys::JET_Flavor_Response_DN) sf = flavor[2];
  else if( sys == NtSys::JET_Flavor_Response_UP) sf = flavor[3];
  else if( sys == NtSys::JET_Pileup_OffsetMu_DN) sf = pileup[0];
  else if( sys == NtSys::JET_Pileup_OffsetMu_UP) sf =  pileup[1];
  else if( sys == NtSys::JET_Pileup_OffsetNPV_DN) sf = pileup[2];
  else if( sys == NtSys::JET_Pileup_OffsetNPV_UP) sf = pileup[3];
  else if( sys == NtSys::JET_Pileup_PtTerm_DN) sf =  pileup[4];
  else if( sys == NtSys::JET_Pileup_PtTerm_UP) sf =  pileup[5];
  else if( sys == NtSys::JET_Pileup_RhoTopology_DN) sf =  pileup[6];
  else if( sys == NtSys::JET_Pileup_RhoTopology_UP) sf =  pileup[7];
  else if( sys == NtSys::JET_PunchThrough_MC12_DN) sf = punchThrough[0];
  else if( sys == NtSys::JET_PunchThrough_MC12_UP) sf = punchThrough[1];
  //else if( sys == NtSys::JET_RelativeNonClosure_MC12_DN) sf = relativeNC[0];
  //else if( sys == NtSys::JET_RelativeNonClosure_MC12_UP) sf = relativeNC[1];
  else if( sys == NtSys::JET_SingleParticle_HighPt_DN) sf = singlePart[0];
  else if( sys == NtSys::JET_SingleParticle_HighPt_UP) sf = singlePart[1];
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Jet print
/*--------------------------------------------------------------------------------*/
void Jet::print() const
{
  cout.precision(2);
  cout << fixed << "Jet : pt " << setw(6) << Pt() << " eta " << setw(5) << Eta()
       << " phi " << setw(5) << Phi()
       << " jvf " << setw(4) << jvf
       << " jvt " << setw(4) << jvt
       << " mv1 " << setw(5) << mv1
       // No way do we want to see this every time we print
       //<< " jer " << jer << " jes_up " << jes_up << " jes_dn " << jes_dn
       << endl;
  cout.precision(6);
  cout.unsetf(ios_base::fixed);
}

