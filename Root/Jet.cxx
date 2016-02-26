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
  idx(rhs.idx),
  jvf(rhs.jvf),
  jvt(rhs.jvt),
  detEta(rhs.detEta),
  emfrac(rhs.emfrac),
  truthLabel(rhs.truthLabel),
  matchTruth(rhs.matchTruth),
  nTracks(rhs.nTracks),
  sumTrkPt(rhs.sumTrkPt),
  bjet(rhs.bjet),
  effscalefact(rhs.effscalefact),
  mv1(rhs.mv1),
  mv2c20(rhs.mv2c20),
  sv1plusip3d(rhs.sv1plusip3d),
  jvtEff(rhs.jvtEff),
  jvtEff_up(rhs.jvtEff_up),
  jvtEff_dn(rhs.jvtEff_dn),
  isBadVeryLoose(rhs.isBadVeryLoose),
  isHotTile(rhs.isHotTile),
  bch_corr_jet(rhs.bch_corr_jet),
  bch_corr_cell(rhs.bch_corr_cell),
  isBadMediumBCH(rhs.isBadMediumBCH),
  isBadMediumBCH_up(rhs.isBadMediumBCH_up),
  isBadMediumBCH_dn(rhs.isBadMediumBCH_dn),
  isBadTightBCH(rhs.isBadTightBCH),
  jer(rhs.jer),
  groupedNP(rhs.groupedNP),
  FTSys(rhs.FTSys)
  /*
  bjes(rhs.bjes),
  effNp(rhs.effNp),
  etaInter(rhs.etaInter),
  flavor(rhs.flavor),
  pileup(rhs.pileup),
  punchThrough(rhs.punchThrough),
  singlePart(rhs.singlePart)
  // relativeNC(rhs.relativeNC),
  */
{ 
}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
Jet& Jet::operator=(const Jet &rhs)
{
  if (this != &rhs) {
    Particle::operator=(rhs);
    idx  = rhs.idx;
    jvf  = rhs.jvf; 
    jvt  = rhs.jvt; 
    detEta = rhs.detEta;
    emfrac = rhs.emfrac;
    truthLabel = rhs.truthLabel;
    matchTruth = rhs.matchTruth;
    bjet = rhs.bjet;
    effscalefact = rhs.effscalefact;
    nTracks = rhs.nTracks;
    sumTrkPt = rhs.sumTrkPt;
    mv1 = rhs.mv1;
    mv2c20 = rhs.mv2c20;
    sv1plusip3d = rhs.sv1plusip3d;
    jvtEff = rhs.jvtEff;
    jvtEff_up = rhs.jvtEff_up;
    jvtEff_dn = rhs.jvtEff_dn;
    isBadVeryLoose = rhs.isBadVeryLoose;
    isHotTile = rhs.isHotTile;
    bch_corr_jet = rhs.bch_corr_jet;
    bch_corr_cell = rhs.bch_corr_cell;
    isBadMediumBCH = rhs.isBadMediumBCH;
    isBadMediumBCH_up = rhs.isBadMediumBCH_up;
    isBadMediumBCH_dn = rhs.isBadMediumBCH_dn;
    isBadTightBCH = rhs.isBadTightBCH;
    jer = rhs.jer;
    groupedNP = rhs.groupedNP;
    FTSys = rhs.FTSys;
    /*
    bjes=rhs.bjes;
    effNp=rhs.effNp;
    etaInter=rhs.etaInter;
    flavor=rhs.flavor;
    pileup=rhs.pileup;
    punchThrough=rhs.punchThrough;
    singlePart=rhs.singlePart;
    //relativeNC=rhs.relativeNC;
    */
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
  
  else if( sys == NtSys::JET_GroupedNP_1_UP) sf = groupedNP[0];
  else if( sys == NtSys::JET_GroupedNP_1_DN) sf = groupedNP[1];
  else if( sys == NtSys::JET_GroupedNP_2_UP) sf = groupedNP[2];
  else if( sys == NtSys::JET_GroupedNP_2_DN) sf = groupedNP[3];
  else if( sys == NtSys::JET_GroupedNP_3_UP) sf = groupedNP[4];
  else if( sys == NtSys::JET_GroupedNP_3_DN) sf = groupedNP[5];
/*
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
*/
  else return;

  this->SetPtEtaPhiE(sf * this->Pt(), this->Eta(), this->Phi(), sf * this->E());
}
/*--------------------------------------------------------------------------------*/
// Flavor systematics
/*--------------------------------------------------------------------------------*/
float Jet::getFTSys(Susy::NtSys::SusyNtSys sys)
{
    float s= 1;

    if      ( sys == NtSys::FT_EFF_B_systematics_DN ) s= FTSys[0];
    else if ( sys == NtSys::FT_EFF_B_systematics_UP ) s= FTSys[1];
    else if ( sys == NtSys::FT_EFF_C_systematics_DN ) s= FTSys[2];
    else if ( sys == NtSys::FT_EFF_C_systematics_UP ) s= FTSys[3];
    else if ( sys == NtSys::FT_EFF_Light_systematics_DN ) s= FTSys[4];
    else if ( sys == NtSys::FT_EFF_Light_systematics_UP ) s= FTSys[5];
    else if ( sys == NtSys::FT_EFF_extrapolation_DN ) s= FTSys[6];
    else if ( sys == NtSys::FT_EFF_extrapolation_UP ) s= FTSys[7];
    else if ( sys == NtSys::FT_EFF_extrapolation_charm_DN ) s= FTSys[8];
    else if ( sys == NtSys::FT_EFF_extrapolation_charm_UP ) s= FTSys[9];
    /*  
    if      ( sys == NtSys::FT_Eigen_B_0_DN) s = FTSys[0];
    else if ( sys == NtSys::FT_Eigen_B_0_UP) s = FTSys[1];
    else if ( sys == NtSys::FT_Eigen_B_1_DN) s = FTSys[2];
    else if ( sys == NtSys::FT_Eigen_B_1_UP) s = FTSys[3];
    else if ( sys == NtSys::FT_Eigen_B_2_DN) s = FTSys[4];
    else if ( sys == NtSys::FT_Eigen_B_2_UP) s = FTSys[5];
    else if ( sys == NtSys::FT_Eigen_B_3_DN) s = FTSys[6];
    else if ( sys == NtSys::FT_Eigen_B_3_UP) s = FTSys[7];
    else if ( sys == NtSys::FT_Eigen_B_4_DN) s = FTSys[8];
    else if ( sys == NtSys::FT_Eigen_B_4_UP) s = FTSys[9];
    else if ( sys == NtSys::FT_Eigen_B_5_DN) s = FTSys[10];
    else if ( sys == NtSys::FT_Eigen_B_5_UP) s = FTSys[11];
    else if ( sys == NtSys::FT_Eigen_B_6_DN) s = FTSys[12];
    else if ( sys == NtSys::FT_Eigen_B_6_UP) s = FTSys[13];
    else if ( sys == NtSys::FT_Eigen_B_7_DN) s = FTSys[14];   
    else if ( sys == NtSys::FT_Eigen_B_7_UP) s = FTSys[15];
    else if ( sys == NtSys::FT_Eigen_B_8_DN) s = FTSys[16];
    else if ( sys == NtSys::FT_Eigen_B_8_UP) s = FTSys[17];
    else if ( sys == NtSys::FT_Eigen_B_9_DN) s = FTSys[18];
    else if ( sys == NtSys::FT_Eigen_B_9_UP) s = FTSys[19];

    else if ( sys == NtSys::FT_Eigen_C_0_DN) s = FTSys[20];
    else if ( sys == NtSys::FT_Eigen_C_0_UP) s = FTSys[21];
    else if ( sys == NtSys::FT_Eigen_C_1_DN) s = FTSys[22];
    else if ( sys == NtSys::FT_Eigen_C_1_UP) s = FTSys[23];
    else if ( sys == NtSys::FT_Eigen_C_2_DN) s = FTSys[24];
    else if ( sys == NtSys::FT_Eigen_C_2_UP) s = FTSys[25];
    else if ( sys == NtSys::FT_Eigen_C_3_DN) s = FTSys[26];
    else if ( sys == NtSys::FT_Eigen_C_3_UP) s = FTSys[27];

    else if ( sys == NtSys::FT_Eigen_Light_0_DN) s = FTSys[28];
    else if ( sys == NtSys::FT_Eigen_Light_0_UP) s = FTSys[29];
    else if ( sys == NtSys::FT_Eigen_Light_1_DN) s = FTSys[30];
    else if ( sys == NtSys::FT_Eigen_Light_1_UP) s = FTSys[31];
    else if ( sys == NtSys::FT_Eigen_Light_2_DN) s = FTSys[32];
    else if ( sys == NtSys::FT_Eigen_Light_2_UP) s = FTSys[33];
    else if ( sys == NtSys::FT_Eigen_Light_3_DN) s = FTSys[34];
    else if ( sys == NtSys::FT_Eigen_Light_3_UP) s = FTSys[35];
    else if ( sys == NtSys::FT_Eigen_Light_4_DN) s = FTSys[36];
    else if ( sys == NtSys::FT_Eigen_Light_4_UP) s = FTSys[37];
    else if ( sys == NtSys::FT_Eigen_Light_5_DN) s = FTSys[38];
    else if ( sys == NtSys::FT_Eigen_Light_5_UP) s = FTSys[39];
    else if ( sys == NtSys::FT_Eigen_Light_6_DN) s = FTSys[40];
    else if ( sys == NtSys::FT_Eigen_Light_6_UP) s = FTSys[41];
    else if ( sys == NtSys::FT_Eigen_Light_7_DN) s = FTSys[42];
    else if ( sys == NtSys::FT_Eigen_Light_7_UP) s = FTSys[43];
    else if ( sys == NtSys::FT_Eigen_Light_8_DN) s = FTSys[44];
    else if ( sys == NtSys::FT_Eigen_Light_8_UP) s = FTSys[45];
    else if ( sys == NtSys::FT_Eigen_Light_9_DN) s = FTSys[46];
    else if ( sys == NtSys::FT_Eigen_Light_9_UP) s = FTSys[47];
    else if ( sys == NtSys::FT_Eigen_Light_10_DN) s = FTSys[48];
    else if ( sys == NtSys::FT_Eigen_Light_10_UP) s = FTSys[49];
    else if ( sys == NtSys::FT_Eigen_Light_11_DN) s = FTSys[50];
    else if ( sys == NtSys::FT_Eigen_Light_11_UP) s = FTSys[51];
    */
    return s;

}

void Jet::setFTSys(Susy::NtSys::SusyNtSys sys, double scale=0.)
{
    if      ( sys == NtSys::FT_EFF_B_systematics_DN ) FTSys[0] =scale;
    else if ( sys == NtSys::FT_EFF_B_systematics_UP ) FTSys[1] =scale;
    else if ( sys == NtSys::FT_EFF_C_systematics_DN ) FTSys[2] =scale;
    else if ( sys == NtSys::FT_EFF_C_systematics_UP ) FTSys[3] =scale;
    else if ( sys == NtSys::FT_EFF_Light_systematics_DN ) FTSys[4] =scale;
    else if ( sys == NtSys::FT_EFF_Light_systematics_UP ) FTSys[5] =scale;
    else if ( sys == NtSys::FT_EFF_extrapolation_DN ) FTSys[6] =scale;
    else if ( sys == NtSys::FT_EFF_extrapolation_UP ) FTSys[7] =scale;
    else if ( sys == NtSys::FT_EFF_extrapolation_charm_DN ) FTSys[8] =scale;
    else if ( sys == NtSys::FT_EFF_extrapolation_charm_UP ) FTSys[9] =scale;

    /*
    if      ( sys == NtSys::FT_Eigen_B_0_DN) FTSys[0] = scale;
    else if ( sys == NtSys::FT_Eigen_B_0_UP) FTSys[1] = scale;
    else if ( sys == NtSys::FT_Eigen_B_1_DN) FTSys[2] = scale;
    else if ( sys == NtSys::FT_Eigen_B_1_UP) FTSys[3] = scale;
    else if ( sys == NtSys::FT_Eigen_B_2_DN) FTSys[4] = scale;
    else if ( sys == NtSys::FT_Eigen_B_2_UP) FTSys[5] = scale;
    else if ( sys == NtSys::FT_Eigen_B_3_DN) FTSys[6] = scale;
    else if ( sys == NtSys::FT_Eigen_B_3_UP) FTSys[7] = scale;
    else if ( sys == NtSys::FT_Eigen_B_4_DN) FTSys[8] = scale;
    else if ( sys == NtSys::FT_Eigen_B_4_UP) FTSys[9] = scale;
    else if ( sys == NtSys::FT_Eigen_B_5_DN) FTSys[10] = scale;
    else if ( sys == NtSys::FT_Eigen_B_5_UP) FTSys[11] = scale;
    else if ( sys == NtSys::FT_Eigen_B_6_DN) FTSys[12] = scale;
    else if ( sys == NtSys::FT_Eigen_B_6_UP) FTSys[13] = scale;
    else if ( sys == NtSys::FT_Eigen_B_7_DN) FTSys[14] = scale;   
    else if ( sys == NtSys::FT_Eigen_B_7_UP) FTSys[15] = scale;
    else if ( sys == NtSys::FT_Eigen_B_8_DN) FTSys[16] = scale;
    else if ( sys == NtSys::FT_Eigen_B_8_UP) FTSys[17] = scale;
    else if ( sys == NtSys::FT_Eigen_B_9_DN) FTSys[18] = scale;
    else if ( sys == NtSys::FT_Eigen_B_9_UP) FTSys[19] = scale;

    else if ( sys == NtSys::FT_Eigen_C_0_DN) FTSys[20] = scale;
    else if ( sys == NtSys::FT_Eigen_C_0_UP) FTSys[21] = scale;
    else if ( sys == NtSys::FT_Eigen_C_1_DN) FTSys[22] = scale;
    else if ( sys == NtSys::FT_Eigen_C_1_UP) FTSys[23] = scale;
    else if ( sys == NtSys::FT_Eigen_C_2_DN) FTSys[24] = scale;
    else if ( sys == NtSys::FT_Eigen_C_2_UP) FTSys[25] = scale;
    else if ( sys == NtSys::FT_Eigen_C_3_DN) FTSys[26] = scale;
    else if ( sys == NtSys::FT_Eigen_C_3_UP) FTSys[27] = scale;

    else if ( sys == NtSys::FT_Eigen_Light_0_DN) FTSys[28] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_0_UP) FTSys[29] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_1_DN) FTSys[30] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_1_UP) FTSys[31] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_2_DN) FTSys[32] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_2_UP) FTSys[33] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_3_DN) FTSys[34] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_3_UP) FTSys[35] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_4_DN) FTSys[36] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_4_UP) FTSys[37] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_5_DN) FTSys[38] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_5_UP) FTSys[39] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_6_DN) FTSys[40] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_6_UP) FTSys[41] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_7_DN) FTSys[42] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_7_UP) FTSys[43] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_8_DN) FTSys[44] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_8_UP) FTSys[45] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_9_DN) FTSys[46] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_9_UP) FTSys[47] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_10_DN) FTSys[48] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_10_UP) FTSys[49] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_11_DN) FTSys[50] = scale;
    else if ( sys == NtSys::FT_Eigen_Light_11_UP) FTSys[51] = scale;
    */
    return;
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

