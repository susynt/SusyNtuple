#ifndef SusyNtuple_SusyNtSys_h
#define SusyNtuple_SusyNtSys_h

#include <string>

namespace Susy {

namespace NtSys {

enum SusyNtSys {
  NOM             // Central value
  ,EG_RESOLUTION_ALL_DN
  ,EG_RESOLUTION_ALL_UP
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
  ,EG_SCALE_ALL_DN
  ,EG_SCALE_ALL_UP
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
  ,EL_EFF_CorrUncertainty_DN
  ,EL_EFF_CorrUncertainty_UP
  ,EL_EFF_UncorrUncertainty_DN
  ,EL_EFF_UncorrUncertainty_UP
  ,EL_SCALE_MOMENTUM_DN
  ,EL_SCALE_MOMENTUM_UP
  ,FT_Eigen_B_0_DN
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
  ,JER
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
  ,JET_RelativeNonClosure_MC12_DN
  ,JET_RelativeNonClosure_MC12_UP
  ,JET_SingleParticle_HighPt_DN
  ,JET_SingleParticle_HighPt_UP
  ,MUONSFSTAT_DN
  ,MUONSFSTAT_UP
  ,MUONSFSYS_DN
  ,MUONSFSYS_UP
  ,MUONS_ID_DN
  ,MUONS_ID_UP
  ,MUONS_MS_DN
  ,MUONS_MS_UP
  ,MUONS_SCALE_DN
  ,MUONS_SCALE_UP
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
  ,TAUS_EFF_CONTJETID_STAT_DN
  ,TAUS_EFF_CONTJETID_STAT_UP
  ,TAUS_EFF_CONTJETID_SYST_DN
  ,TAUS_EFF_CONTJETID_SYST_UP
  ,TAUS_SME_TOTAL_DN
  ,TAUS_SME_TOTAL_UP
  
  //AT:2014-11-06 OLD ONES - TO REMOVE/REPLACE
  ,EES_LOW_UP           // Positive shift in electron energy scale (LOW)
  ,EES_LOW_DN         // Negative shift in electron energy scale (LOW)
  ,EES_MAT_UP           // Positive shift in electron energy scale (Mat)
  ,EES_MAT_DN         // Negative shift in electron energy scale (Mat)
  ,EES_PS_UP            // Positive shift in electron energy scale (PS)
  ,EES_PS_DN          // Negative shift in electron energy scale (PS)
  ,EES_Z_UP             // Positive shift in electron energy scale (Zee)
  ,EES_Z_DN           // Negative shift in electron energy scale (Zee)
  ,EER_UP              // Positive shift in electron energy resolution
  ,EER_DN            // Negative shift in electron energy resolution
  ,MS_UP              // Positive shift in muon energy scale
  ,MS_DN            // Negative shift in muon energy scale
  ,ID_UP              // Positive shift in muon ID resolution
  ,ID_DN            // Negative shift in muon ID resolution
  ,TES_UP              // Positive shift in TES variation 
  ,TES_DN            // Negative shift in TES variation
  ,JES_UP              // Positive Jet energy scale
  ,JES_DN            // Negative Jet energy scale
  ,SCALEST_UP          // Positive shift in MET soft term scale
  ,SCALEST_DN        // Negative shift in MET soft term scale
  ,RESOST             // MET soft term resolution 
  //Event weight systematics 
  ,TRIGSF_EL_UP    ///< Trigger Scale factor + error for el
  ,TRIGSF_EL_DN    ///< Trigger Scale factor - error for el
  ,TRIGSF_MU_UP    ///< Trigger Scale factor + error for mu
  ,TRIGSF_MU_DN    ///< Trigger Scale factor - error for mu
  ,JVF_UP              // Positive shift in Jet Vertex Fraction
  ,JVF_DN            // Negative shift in Jet Vertex Fraction 

  /*
  ,ESFUP              // Positive shift in electron efficiency
  ,ESFDOWN            // Negative shift in electron efficiency
  ,MEFFUP             // Positive shift in muon efficiency
  ,MEFFDOWN           // Negative shift in muon efficiency
  ,ETRIGREWUP         // Positive shift in electron trigger weights
  ,ETRIGREWDOWN       // Negative shift in electron trigger weights
  ,MTRIGREWUP         // Positive shift in muon trigger weights
  ,MTRIGREWDOWN       // Negative shift in muon trigger weights
  ,TTRIGSFUP          // Positive shift in tau trigger scale factor                              
  ,TTRIGSFDOWN        // Negative shift in tau trigger scale factor
  ,BJETUP             // Positive shift in btag scale factor
  ,BJETDOWN           // Negative shift in btag scale factor
  ,CJETUP             // Positive shift in ctag scale factor
  ,CJETDOWN           // Negative shift in ctag scale factor
  ,BMISTAGUP          // Positive shift in ltag scale factor
  ,BMISTAGDOWN        // Negative shift in ltag scale factor

  ,PILEUPUP           // Positive shift for mu 
  ,PILEUPDOWN         // Negative shift for mu
  ,LUMI               // Uncertainty due to luminosity (no tree/weight needed for Limit setting)
 
  //Others typically needed at the analysis level when filling HFT 
  ,GEN                // Uncertainty due to generator (Alpgen versus Sherpa,...) and patronshower/hadronization (Herwig versus Pythia) (possibly includes PDF choice as well, i.e. CTEQ versus HERA,...) 
  ,GENUP              // Positive shift due to generator parameter variations (ktfac, qfac, ptmin, Iqopt, scale); includes also shift in ISR/FSR
  ,GENDOWN            // Negative shift due to generator parameter variations (ktfac, qfac, ptmin, Iqopt, scale) ;includes also shift in ISR/FSR
  ,PDFERRUP           // Positive shift due to PDF errorset
  ,PDFERRDOWN         // Negative shift due to PDF errorset
  ,BKGMETHODUP        // Positive shift due to background estimation method (chargeflip, jetveto, fakeweights,...) 
  ,BKGMETHODDOWN      // Negative shift due to background estimation method (chargeflip, jetveto, fakeweights,...) 
  ,XSUP               // Positive shift in theoretical cross-section uncertainty
  ,XSDOWN             // Negative shift in theoretical cross-section uncertainty
  
  //Fake Syst.
  ,FakeTauBGSyst      // Uncertainty on fake tau background estimation
  */
  ,SYSUNKNOWN
};
 
const std::string SusyNtSysNames[] = {
    "NOM"
    ,"EG_RESOLUTION_ALL_DN"
    ,"EG_RESOLUTION_ALL_UP"
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
    ,"EG_SCALE_ALL_DN"
    ,"EG_SCALE_ALL_UP"
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
    ,"EL_EFF_CorrUncertainty_DN"
    ,"EL_EFF_CorrUncertainty_UP"
    ,"EL_EFF_UncorrUncertainty_DN"
    ,"EL_EFF_UncorrUncertainty_UP"
    ,"EL_SCALE_MOMENTUM_DN"
    ,"EL_SCALE_MOMENTUM_UP"
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
    ,"JER"
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
    ,"JET_RelativeNonClosure_MC12_DN"
    ,"JET_RelativeNonClosure_MC12_UP"
    ,"JET_SingleParticle_HighPt_DN"
    ,"JET_SingleParticle_HighPt_UP"
    ,"MUONSFSTAT_DN"
    ,"MUONSFSTAT_UP"
    ,"MUONSFSYS_DN"
    ,"MUONSFSYS_UP"
    ,"MUONS_ID_DN"
    ,"MUONS_ID_UP"
    ,"MUONS_MS_DN"
    ,"MUONS_MS_UP"
    ,"MUONS_SCALE_DN"
    ,"MUONS_SCALE_UP"
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
    ,"TAUS_EFF_CONTJETID_STAT_DN"
    ,"TAUS_EFF_CONTJETID_STAT_UP"
    ,"TAUS_EFF_CONTJETID_SYST_DN"
    ,"TAUS_EFF_CONTJETID_SYST_UP"
    ,"TAUS_SME_TOTAL_DN"
    ,"TAUS_SME_TOTAL_UP"
        
    //Old ones
    ,"EES_LOW_UP"
    ,"EES_LOW_DOWN"
    ,"EES_MAT_UP"
    ,"EES_MAT_DOWN"
    ,"EES_PS_UP"
    ,"EES_PS_DOWN"
    ,"EES_Z_UP"
    ,"EES_Z_DOWN"
    ,"EER_UP"
    ,"EER_DOWN"
    ,"MS_UP"
    ,"MS_DOWN"
    ,"ID_UP"
    ,"ID_DOWN"
    ,"TES_UP"
    ,"TES_DOWN"
    ,"JES_UP"
    ,"JES_DOWN"
    ,"SCALEST_UP"
    ,"SCALEST_DOWN"
    ,"RESOST"

    ,"TRIGSF_EL_UP"
    ,"TRIGSF_EL_DN"
    ,"TRIGSF_MU_UP"
    ,"TRIGSF_MU_DN"
    ,"JVF_UP"
    ,"JVF_DN"

    /*
    ,"ESFUP"
    ,"ESFDOWN"
    ,"MEFFUP"
    ,"MEFFDOWN"
    ,"ETRIGREWUP"
    ,"ETRIGREWDOWN"
    ,"MTRIGREWUP"
    ,"MTRIGREWDOWN"
    ,"TTRIGSFUP"
    ,"TTRIGSFDOWN"
    ,"BJETUP"
    ,"BJETDOWN"
    ,"CJETUP"
    ,"CJETDOWN"
    ,"BMISTAGUP"
    ,"BMISTAGDOWN"

    ,"PILEUPUP"
    ,"PILEUPDOWN"
    ,"LUMI"

    ,"GEN"
    ,"GENUP"
    ,"GENDOWN"
    ,"PDFERRUP"
    ,"PDFERRDOWN"
    ,"BKGMETHODUP"
    ,"BKGMETHODDOWN"
    ,"XSUP"
    ,"XSDOWN"
    
    ,"FakeTauBGSyst"
    */
    ,"SYSUNKNOWN"
};



} //NtSys

} // Susy


#endif
