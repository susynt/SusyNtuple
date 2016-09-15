//  -*- c++ -*-
#ifndef SusyNtuple_SusyNtSys_h
#define SusyNtuple_SusyNtSys_h

#include <string>

namespace Susy {

namespace NtSys {

enum SusyNtSys {
  NOM             // Central value
  ,EG_RESOLUTION_ALL_DN
  ,EG_RESOLUTION_ALL_UP
  /*
  ,EG_RESOLUTION_LASTRESOLUTIONVARIATION
  ,EG_RESOLUTION_MATERIALCALO_DN
  ,EG_RESOLUTION_MATERIALCALO_UP
  ,EG_RESOLUTION_MATERIALCRYO_DN
  ,EG_RESOLUTION_MATERIALCRYO_UP
  ,EG_RESOLUTION_MATERIALGAP_DN
  ,EG_RESOLUTION_MATERIALGAP_UP
  ,EG_RESOLUTION_MATERIALID_DN
  ,EG_RESOLUTION_MATERIALID_UP
  ,EG_RESOLUTION_NOMINAL
  ,EG_RESOLUTION_NONE
  ,EG_RESOLUTION_PILEUP_DN
  ,EG_RESOLUTION_PILEUP_UP
  ,EG_RESOLUTION_SAMPLINGTERM_DN
  ,EG_RESOLUTION_SAMPLINGTERM_UP
  ,EG_RESOLUTION_ZSMEARING_DN
  ,EG_RESOLUTION_ZSMEARING_UP
  */
  ,EG_SCALE_ALL_DN
  ,EG_SCALE_ALL_UP
  /*
  ,EG_SCALE_G4_DN
  ,EG_SCALE_G4_UP
  ,EG_SCALE_L1GAIN_DN
  ,EG_SCALE_L1GAIN_UP
  ,EG_SCALE_L2GAIN_DN
  ,EG_SCALE_L2GAIN_UP
  ,EG_SCALE_LARCALIB_DN
  ,EG_SCALE_LARCALIB_UP
  ,EG_SCALE_LARELECCALIB_DN
  ,EG_SCALE_LARELECCALIB_UP
  ,EG_SCALE_LARELECUNCONV_DN
  ,EG_SCALE_LARELECUNCONV_UP
  ,EG_SCALE_LARUNCONVCALIB_DN
  ,EG_SCALE_LARUNCONVCALIB_UP
  ,EG_SCALE_LASTSCALEVARIATION
  ,EG_SCALE_MATCALO_DN
  ,EG_SCALE_MATCALO_UP
  ,EG_SCALE_MATCRYO_DN
  ,EG_SCALE_MATCRYO_UP
  ,EG_SCALE_MATID_DN
  ,EG_SCALE_MATID_UP
  ,EG_SCALE_NOMINAL
  ,EG_SCALE_NONE
  ,EG_SCALE_PEDESTAL_DN
  ,EG_SCALE_PEDESTAL_UP
  ,EG_SCALE_PS_DN
  ,EG_SCALE_PS_UP
  ,EG_SCALE_S12_DN
  ,EG_SCALE_S12_UP
  ,EG_SCALE_ZEESTAT_DN
  ,EG_SCALE_ZEESTAT_UP
  ,EG_SCALE_ZEESYST_DN
  ,EG_SCALE_ZEESYST_UP
  */

 // dantrim May23 2016 changing these names
 /*
  ,EL_EFF_ID_TotalCorrUncertainty_UP  // weight
  ,EL_EFF_ID_TotalCorrUncertainty_DN  // weight
  ,EL_EFF_Iso_TotalCorrUncertainty_UP // weight 
  ,EL_EFF_Iso_TotalCorrUncertainty_DN // weight 
  ,EL_EFF_Reco_TotalCorrUncertainty_UP  // weight
  ,EL_EFF_Reco_TotalCorrUncertainty_DN  // weight
  ,EL_EFF_Trigger_TotalCorrUncertainty_UP
  ,EL_EFF_Trigger_TotalCorrUncertainty_DN
 */
  ,EL_EFF_ID_TOTAL_Uncorr_DN
  ,EL_EFF_ID_TOTAL_Uncorr_UP
  ,EL_EFF_Iso_TOTAL_Uncorr_DN
  ,EL_EFF_Iso_TOTAL_Uncorr_UP
  ,EL_EFF_Reco_TOTAL_Uncorr_DN
  ,EL_EFF_Reco_TOTAL_Uncorr_UP
  ,EL_EFF_Trigger_TOTAL_Uncorr_DN
  ,EL_EFF_Trigger_TOTAL_Uncorr_UP
  /*
  ,EL_SCALE_MOMENTUM_DN
  ,EL_SCALE_MOMENTUM_UP
  */
  ,FT_EFF_B_systematics_UP
  ,FT_EFF_B_systematics_DN
  ,FT_EFF_C_systematics_UP
  ,FT_EFF_C_systematics_DN
  ,FT_EFF_Light_systematics_UP
  ,FT_EFF_Light_systematics_DN
  ,FT_EFF_extrapolation_UP
  ,FT_EFF_extrapolation_DN
  ,FT_EFF_extrapolation_charm_UP
  ,FT_EFF_extrapolation_charm_DN
  /*
  ,FT_Eigen_B_0_DN // All FT_ are weight
  ,FT_Eigen_B_0_UP
  ,FT_Eigen_B_1_DN
  ,FT_Eigen_B_1_UP
  ,FT_Eigen_B_2_DN
  ,FT_Eigen_B_2_UP
  ,FT_Eigen_B_3_DN
  ,FT_Eigen_B_3_UP
  ,FT_Eigen_B_4_DN
  ,FT_Eigen_B_4_UP
  ,FT_Eigen_B_5_DN
  ,FT_Eigen_B_5_UP
  ,FT_Eigen_B_6_DN
  ,FT_Eigen_B_6_UP
  ,FT_Eigen_B_7_DN
  ,FT_Eigen_B_7_UP
  ,FT_Eigen_B_8_DN
  ,FT_Eigen_B_8_UP
  ,FT_Eigen_B_9_DN
  ,FT_Eigen_B_9_UP
  ,FT_Eigen_C_0_DN
  ,FT_Eigen_C_0_UP
  ,FT_Eigen_C_1_DN
  ,FT_Eigen_C_1_UP
  ,FT_Eigen_C_2_DN
  ,FT_Eigen_C_2_UP
  ,FT_Eigen_C_3_DN
  ,FT_Eigen_C_3_UP
  ,FT_Eigen_Light_0_DN
  ,FT_Eigen_Light_0_UP
  ,FT_Eigen_Light_10_DN
  ,FT_Eigen_Light_10_UP
  ,FT_Eigen_Light_11_DN
  ,FT_Eigen_Light_11_UP
  ,FT_Eigen_Light_1_DN
  ,FT_Eigen_Light_1_UP
  ,FT_Eigen_Light_2_DN
  ,FT_Eigen_Light_2_UP
  ,FT_Eigen_Light_3_DN
  ,FT_Eigen_Light_3_UP
  ,FT_Eigen_Light_4_DN
  ,FT_Eigen_Light_4_UP
  ,FT_Eigen_Light_5_DN
  ,FT_Eigen_Light_5_UP
  ,FT_Eigen_Light_6_DN
  ,FT_Eigen_Light_6_UP
  ,FT_Eigen_Light_7_DN
  ,FT_Eigen_Light_7_UP
  ,FT_Eigen_Light_8_DN
  ,FT_Eigen_Light_8_UP
  ,FT_Eigen_Light_9_DN
  ,FT_Eigen_Light_9_UP
  */
  ,JER
  //NEW
  ,JET_GroupedNP_1_UP
  ,JET_GroupedNP_1_DN
  ,JET_GroupedNP_2_UP
  ,JET_GroupedNP_2_DN
  ,JET_GroupedNP_3_UP
  ,JET_GroupedNP_3_DN
  ,JET_JVTEff_UP
  ,JET_JVTEff_DN
  /*
  ,JET_BJES_Response_DN
  ,JET_BJES_Response_UP
  ,JET_EffectiveNP_1_DN
  ,JET_EffectiveNP_1_UP
  ,JET_EffectiveNP_2_DN
  ,JET_EffectiveNP_2_UP
  ,JET_EffectiveNP_3_DN
  ,JET_EffectiveNP_3_UP
  ,JET_EffectiveNP_4_DN
  ,JET_EffectiveNP_4_UP
  ,JET_EffectiveNP_5_DN
  ,JET_EffectiveNP_5_UP
  ,JET_EffectiveNP_6restTerm_DN
  ,JET_EffectiveNP_6restTerm_UP
  ,JET_EtaIntercalibration_Modelling_DN
  ,JET_EtaIntercalibration_Modelling_UP
  ,JET_EtaIntercalibration_TotalStat_DN
  ,JET_EtaIntercalibration_TotalStat_UP
  ,JET_Flavor_Composition_DN
  ,JET_Flavor_Composition_UP
  ,JET_Flavor_Response_DN
  ,JET_Flavor_Response_UP
  ,JET_Pileup_OffsetMu_DN
  ,JET_Pileup_OffsetMu_UP
  ,JET_Pileup_OffsetNPV_DN
  ,JET_Pileup_OffsetNPV_UP
  ,JET_Pileup_PtTerm_DN
  ,JET_Pileup_PtTerm_UP
  ,JET_Pileup_RhoTopology_DN
  ,JET_Pileup_RhoTopology_UP
  ,JET_PunchThrough_MC12_DN
  ,JET_PunchThrough_MC12_UP
 ,JET_SingleParticle_HighPt_DN
  ,JET_SingleParticle_HighPt_UP
  //,JET_RelativeNonClosure_MC12_DN
  //,JET_RelativeNonClosure_MC12_UP
  */
  //NEW
  ,MET_SoftCalo_Reso
  ,MET_SoftCalo_ScaleDown
  ,MET_SoftCalo_ScaleUp
  ,MET_SoftTrk_ResoPara
  ,MET_SoftTrk_ResoPerp
  ,MET_SoftTrk_ScaleDown
  ,MET_SoftTrk_ScaleUp
  //END NEW
  ,MUON_EFF_STAT_DN  //weight
  ,MUON_EFF_STAT_UP  //weight
  ,MUON_EFF_STAT_LOWPT_DN
  ,MUON_EFF_STAT_LOWPT_UP
  ,MUON_EFF_SYS_DN   //weight
  ,MUON_EFF_SYS_UP   //weight
  ,MUON_EFF_SYS_LOWPT_DN
  ,MUON_EFF_SYS_LOWPT_UP
  ,MUON_EFF_TRIG_STAT_DN
  ,MUON_EFF_TRIG_STAT_UP
  ,MUON_EFF_TRIG_SYST_DN
  ,MUON_EFF_TRIG_SYST_UP
  ,MUON_ISO_STAT_DN
  ,MUON_ISO_STAT_UP
  ,MUON_ISO_SYS_DN
  ,MUON_ISO_SYS_UP
  ,MUONS_ID_DN
  ,MUONS_ID_UP
  ,MUONS_MS_DN
  ,MUONS_MS_UP
  ,MUONS_SCALE_DN
  ,MUONS_SCALE_UP
  /*
  ,PH_SCALE_CONVEFFICIENCY_DN
  ,PH_SCALE_CONVEFFICIENCY_UP
  ,PH_SCALE_CONVFAKERATE_DN
  ,PH_SCALE_CONVFAKERATE_UP
  ,PH_SCALE_CONVRADIUS_DN
  ,PH_SCALE_CONVRADIUS_UP
  ,PH_SCALE_LEAKAGECONV_DN
  ,PH_SCALE_LEAKAGECONV_UP
  ,PH_SCALE_LEAKAGEUNCONV_DN
  ,PH_SCALE_LEAKAGEUNCONV_UP
  */
  ,TAUS_EFF_CONTJETID_STAT_DN  //weight
  ,TAUS_EFF_CONTJETID_STAT_UP  //weight
  ,TAUS_EFF_CONTJETID_SYST_DN  //weight
  ,TAUS_EFF_CONTJETID_SYST_UP  //weight
  ,TAUS_SME_TOTAL_DN
  ,TAUS_SME_TOTAL_UP
  
  //
  //The systematic below are internal to SusyNt.
  //
  ,TRIGSF_EL_UP    ///< Trigger Scale factor + error for el
  ,TRIGSF_EL_DN    ///< Trigger Scale factor - error for el
  ,TRIGSF_MU_UP    ///< Trigger Scale factor + error for mu
  ,TRIGSF_MU_DN    ///< Trigger Scale factor - error for mu
 
  ,PILEUP_UP       ///< Positive shift for mu 
  ,PILEUP_DN       ///< Negative shift for mu

  ,GEN             ///< Uncertainty due to generator (Alpgen versus Sherpa,...) 
                   ///< and patronshower/hadronization (Herwig versus Pythia) 
                   ///< (possibly includes PDF choice as well, i.e. CTEQ versus HERA,...) 
  ,GEN_UP          ///< Positive shift due to generator parameter variations 
                   ///< (ktfac, qfac, ptmin, Iqopt, scale); includes also shift in ISR/FSR
  ,GEN_DN          ///< Negative shift due to generator parameter variations 
  ,PDF_UP          ///< Positive shift due to PDF errorset
  ,PDF_DN          ///< Negative shift due to PDF errorset
  ,BKGMETHOD_UP    ///< Positive shift due to background estimation method
                   ///< (chargeflip, jetveto, fakeweights,...) 
  ,BKGMETHOD_DN    ///< Negative shift due to background estimation method 
  ,XS_UP           ///< Positive shift in cross-section uncertainty
  ,XS_DN           ///< Negative shift in cross-section uncertainty
  
  ,SYS_UNKNOWN
};
 
const std::string SusyNtSysNames[] = {
    "NOM"
    ,"EG_RESOLUTION_ALL_DN"
    ,"EG_RESOLUTION_ALL_UP"
    /*
    ,"EG_RESOLUTION_LASTRESOLUTIONVARIATION"
    ,"EG_RESOLUTION_MATERIALCALO_DN"
    ,"EG_RESOLUTION_MATERIALCALO_UP"
    ,"EG_RESOLUTION_MATERIALCRYO_DN"
    ,"EG_RESOLUTION_MATERIALCRYO_UP"
    ,"EG_RESOLUTION_MATERIALGAP_DN"
    ,"EG_RESOLUTION_MATERIALGAP_UP"
    ,"EG_RESOLUTION_MATERIALID_DN"
    ,"EG_RESOLUTION_MATERIALID_UP"
    ,"EG_RESOLUTION_NOMINAL"
    ,"EG_RESOLUTION_NONE"
    ,"EG_RESOLUTION_PILEUP_DN"
    ,"EG_RESOLUTION_PILEUP_UP"
    ,"EG_RESOLUTION_SAMPLINGTERM_DN"
    ,"EG_RESOLUTION_SAMPLINGTERM_UP"
    ,"EG_RESOLUTION_ZSMEARING_DN"
    ,"EG_RESOLUTION_ZSMEARING_UP"
    */
    ,"EG_SCALE_ALL_DN"
    ,"EG_SCALE_ALL_UP"
    /*
    ,"EG_SCALE_G4_DN"
    ,"EG_SCALE_G4_UP"
    ,"EG_SCALE_L1GAIN_DN"
    ,"EG_SCALE_L1GAIN_UP"
    ,"EG_SCALE_L2GAIN_DN"
    ,"EG_SCALE_L2GAIN_UP"
    ,"EG_SCALE_LARCALIB_DN"
    ,"EG_SCALE_LARCALIB_UP"
    ,"EG_SCALE_LARELECCALIB_DN"
    ,"EG_SCALE_LARELECCALIB_UP"
    ,"EG_SCALE_LARELECUNCONV_DN"
    ,"EG_SCALE_LARELECUNCONV_UP"
    ,"EG_SCALE_LARUNCONVCALIB_DN"
    ,"EG_SCALE_LARUNCONVCALIB_UP"
    ,"EG_SCALE_LASTSCALEVARIATION"
    ,"EG_SCALE_MATCALO_DN"
    ,"EG_SCALE_MATCALO_UP"
    ,"EG_SCALE_MATCRYO_DN"
    ,"EG_SCALE_MATCRYO_UP"
    ,"EG_SCALE_MATID_DN"
    ,"EG_SCALE_MATID_UP"
    ,"EG_SCALE_NOMINAL"
    ,"EG_SCALE_NONE"
    ,"EG_SCALE_PEDESTAL_DN"
    ,"EG_SCALE_PEDESTAL_UP"
    ,"EG_SCALE_PS_DN"
    ,"EG_SCALE_PS_UP"
    ,"EG_SCALE_S12_DN"
    ,"EG_SCALE_S12_UP"
    ,"EG_SCALE_ZEESTAT_DN"
    ,"EG_SCALE_ZEESTAT_UP"
    ,"EG_SCALE_ZEESYST_DN"
    ,"EG_SCALE_ZEESYST_UP"
    */
    ,"EL_EFF_ID_TOTAL_Uncorr_DN"
    ,"EL_EFF_ID_TOTAL_Uncorr_UP"
    ,"EL_EFF_Iso_TOTAL_Uncorr_DN"
    ,"EL_EFF_Iso_TOTAL_Uncorr_UP"
    ,"EL_EFF_Reco_TOTAL_Uncorr_DN"
    ,"EL_EFF_Reco_TOTAL_Uncorr_UP"
    ,"EL_EFF_Trigger_TOTAL_Uncorr_DN"
    ,"EL_EFF_Trigger_TOTAL_Uncorr_UP"
  //  ,"EL_EFF_ID_TotalCorrUncertainty_UP"
  //  ,"EL_EFF_ID_TotalCorrUncertainty_DN"
  //  ,"EL_EFF_Iso_TotalCorrUncertainty_UP"
  //  ,"EL_EFF_Iso_TotalCorrUncertainty_DN"
  //  ,"EL_EFF_Reco_TotalCorrUncertainty_UP"
  //  ,"EL_EFF_Reco_TotalCorrUncertainty_DN"
  //  ,"EL_EFF_Trigger_TotalCorrUncertainty_UP"
  //  ,"EL_EFF_Trigger_TotalCorrUncertainty_DN"
    /*
    ,"EL_SCALE_MOMENTUM_DN"
    ,"EL_SCALE_MOMENTUM_UP"
    */
    ,"FT_EFF_B_systematics_UP"
    ,"FT_EFF_B_systematics_DN"
    ,"FT_EFF_C_systematics_UP"
    ,"FT_EFF_C_systematics_DN"
    ,"FT_EFF_Light_systematics_UP"
    ,"FT_EFF_Light_systematics_DN"
    ,"FT_EFF_extrapolation_UP"
    ,"FT_EFF_extrapolation_DN"
    ,"FT_EFF_extrapolation_charm_UP"
    ,"FT_EFF_extrapolation_charm_DN"
    /*
    ,"FT_Eigen_B_0_DN"
    ,"FT_Eigen_B_0_UP"
    ,"FT_Eigen_B_1_DN"
    ,"FT_Eigen_B_1_UP"
    ,"FT_Eigen_B_2_DN"
    ,"FT_Eigen_B_2_UP"
    ,"FT_Eigen_B_3_DN"
    ,"FT_Eigen_B_3_UP"
    ,"FT_Eigen_B_4_DN"
    ,"FT_Eigen_B_4_UP"
    ,"FT_Eigen_B_5_DN"
    ,"FT_Eigen_B_5_UP"
    ,"FT_Eigen_B_6_DN"
    ,"FT_Eigen_B_6_UP"
    ,"FT_Eigen_B_7_DN"
    ,"FT_Eigen_B_7_UP"
    ,"FT_Eigen_B_8_DN"
    ,"FT_Eigen_B_8_UP"
    ,"FT_Eigen_B_9_DN"
    ,"FT_Eigen_B_9_UP"
    ,"FT_Eigen_C_0_DN"
    ,"FT_Eigen_C_0_UP"
    ,"FT_Eigen_C_1_DN"
    ,"FT_Eigen_C_1_UP"
    ,"FT_Eigen_C_2_DN"
    ,"FT_Eigen_C_2_UP"
    ,"FT_Eigen_C_3_DN"
    ,"FT_Eigen_C_3_UP"
    ,"FT_Eigen_Light_0_DN"
    ,"FT_Eigen_Light_0_UP"
    ,"FT_Eigen_Light_10_DN"
    ,"FT_Eigen_Light_10_UP"
    ,"FT_Eigen_Light_11_DN"
    ,"FT_Eigen_Light_11_UP"
    ,"FT_Eigen_Light_1_DN"
    ,"FT_Eigen_Light_1_UP"
    ,"FT_Eigen_Light_2_DN"
    ,"FT_Eigen_Light_2_UP"
    ,"FT_Eigen_Light_3_DN"
    ,"FT_Eigen_Light_3_UP"
    ,"FT_Eigen_Light_4_DN"
    ,"FT_Eigen_Light_4_UP"
    ,"FT_Eigen_Light_5_DN"
    ,"FT_Eigen_Light_5_UP"
    ,"FT_Eigen_Light_6_DN"
    ,"FT_Eigen_Light_6_UP"
    ,"FT_Eigen_Light_7_DN"
    ,"FT_Eigen_Light_7_UP"
    ,"FT_Eigen_Light_8_DN"
    ,"FT_Eigen_Light_8_UP"
    ,"FT_Eigen_Light_9_DN"
    ,"FT_Eigen_Light_9_UP"
    */
    ,"JER"
    ,"JET_GroupedNP_1_UP"
    ,"JET_GroupedNP_1_DN"
    ,"JET_GroupedNP_2_UP"
    ,"JET_GroupedNP_2_DN"
    ,"JET_GroupedNP_3_UP"
    ,"JET_GroupedNP_3_DN"
    ,"JET_JVTEff_UP"
    ,"JET_JVTEff_DN"
    /*
    ,"JET_BJES_Response_DN"
    ,"JET_BJES_Response_UP"
    ,"JET_EffectiveNP_1_DN"
    ,"JET_EffectiveNP_1_UP"
    ,"JET_EffectiveNP_2_DN"
    ,"JET_EffectiveNP_2_UP"
    ,"JET_EffectiveNP_3_DN"
    ,"JET_EffectiveNP_3_UP"
    ,"JET_EffectiveNP_4_DN"
    ,"JET_EffectiveNP_4_UP"
    ,"JET_EffectiveNP_5_DN"
    ,"JET_EffectiveNP_5_UP"
    ,"JET_EffectiveNP_6restTerm_DN"
    ,"JET_EffectiveNP_6restTerm_UP"
    ,"JET_EtaIntercalibration_Modelling_DN"
    ,"JET_EtaIntercalibration_Modelling_UP"
    ,"JET_EtaIntercalibration_TotalStat_DN"
    ,"JET_EtaIntercalibration_TotalStat_UP"
    ,"JET_Flavor_Composition_DN"
    ,"JET_Flavor_Composition_UP"
    ,"JET_Flavor_Response_DN"
    ,"JET_Flavor_Response_UP"
    ,"JET_Pileup_OffsetMu_DN"
    ,"JET_Pileup_OffsetMu_UP"
    ,"JET_Pileup_OffsetNPV_DN"
    ,"JET_Pileup_OffsetNPV_UP"
    ,"JET_Pileup_PtTerm_DN"
    ,"JET_Pileup_PtTerm_UP"
    ,"JET_Pileup_RhoTopology_DN"
    ,"JET_Pileup_RhoTopology_UP"
    ,"JET_PunchThrough_MC12_DN"
    ,"JET_PunchThrough_MC12_UP"
    ,"JET_SingleParticle_HighPt_DN"
    ,"JET_SingleParticle_HighPt_UP"
    //,"JET_RelativeNonClosure_MC12_DN"
    //,"JET_RelativeNonClosure_MC12_UP"
    */
    ,"MET_SoftCalo_Reso"
    ,"MET_SoftCalo_ScaleDown"
    ,"MET_SoftCalo_ScaleUp"
    ,"MET_SoftTrk_ResoPara"
    ,"MET_SoftTrk_ResoPerp"
    ,"MET_SoftTrk_ScaleDown"
    ,"MET_SoftTrk_ScaleUp"
    ,"MUON_EFF_STAT_DN"
    ,"MUON_EFF_STAT_UP"
    ,"MUON_EFF_STAT_LOWPT_DN"
    ,"MUON_EFF_STAT_LOWPT_UP"
    ,"MUON_EFF_SYS_DN"
    ,"MUON_EFF_SYS_UP"
    ,"MUON_EFF_SYS_LOWPT_DN"
    ,"MUON_EFF_SYS_LOWPT_UP"
    ,"MUON_EFF_TRIG_STAT_DN"
    ,"MUON_EFF_TRIG_STAT_UP"
    ,"MUON_EFF_TRIG_SYST_DN"
    ,"MUON_EFF_TRIG_SYST_UP"
    ,"MUON_ISO_STAT_DN"
    ,"MUON_ISO_STAT_UP"
    ,"MUON_ISO_SYS_DN"
    ,"MUON_ISO_SYS_UP"
    ,"MUONS_ID_DN"
    ,"MUONS_ID_UP"
    ,"MUONS_MS_DN"
    ,"MUONS_MS_UP"
    ,"MUONS_SCALE_DN"
    ,"MUONS_SCALE_UP"
    /*
    ,"PH_SCALE_CONVEFFICIENCY_DN"
    ,"PH_SCALE_CONVEFFICIENCY_UP"
    ,"PH_SCALE_CONVFAKERATE_DN"
    ,"PH_SCALE_CONVFAKERATE_UP"
    ,"PH_SCALE_CONVRADIUS_DN"
    ,"PH_SCALE_CONVRADIUS_UP"
    ,"PH_SCALE_LEAKAGECONV_DN"
    ,"PH_SCALE_LEAKAGECONV_UP"
    ,"PH_SCALE_LEAKAGEUNCONV_DN"
    ,"PH_SCALE_LEAKAGEUNCONV_UP"
    */
    ,"TAUS_EFF_CONTJETID_STAT_DN"
    ,"TAUS_EFF_CONTJETID_STAT_UP"
    ,"TAUS_EFF_CONTJETID_SYST_DN"
    ,"TAUS_EFF_CONTJETID_SYST_UP"
    ,"TAUS_SME_TOTAL_DN"
    ,"TAUS_SME_TOTAL_UP"
        
    //
    //SusyNt specific systematics
    //
    ,"TRIGSF_EL_UP"
    ,"TRIGSF_EL_DN"
    ,"TRIGSF_MU_UP"
    ,"TRIGSF_MU_DN"
    ,"PILEUP_UP"
    ,"PILEUP_DN"
    ,"GEN"
    ,"GEN_UP"
    ,"GEN_DN"
    ,"PDF_UP"
    ,"PDF_DN"
    ,"BKGMETHOD_UP"
    ,"BKGMETHOD_DN"
    ,"XS_UP"
    ,"XS_DN"
    ,"SYS_UNKNOWN"
};



} //NtSys

} // Susy


#endif
