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
  ,EG_SCALE_ALL_DN
  ,EG_SCALE_ALL_UP
  ,EL_EFF_ID_TOTAL_Uncorr_DN
  ,EL_EFF_ID_TOTAL_Uncorr_UP
  ,EL_EFF_Iso_TOTAL_Uncorr_DN
  ,EL_EFF_Iso_TOTAL_Uncorr_UP
  ,EL_EFF_Reco_TOTAL_Uncorr_DN
  ,EL_EFF_Reco_TOTAL_Uncorr_UP
  ,EL_EFF_Trigger_TOTAL_Uncorr_DN
  ,EL_EFF_Trigger_TOTAL_Uncorr_UP
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
  ,JER
  ,JET_GroupedNP_1_UP
  ,JET_GroupedNP_1_DN
  ,JET_GroupedNP_2_UP
  ,JET_GroupedNP_2_DN
  ,JET_GroupedNP_3_UP
  ,JET_GroupedNP_3_DN
  ,JET_JVTEff_UP
  ,JET_JVTEff_DN
  ,MET_SoftCalo_Reso
  ,MET_SoftCalo_ScaleDown
  ,MET_SoftCalo_ScaleUp
  ,MET_SoftTrk_ResoPara
  ,MET_SoftTrk_ResoPerp
  ,MET_SoftTrk_ScaleDown
  ,MET_SoftTrk_ScaleUp
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
  ,MUON_ID_DN
  ,MUON_ID_UP
  ,MUON_MS_DN
  ,MUON_MS_UP
  ,MUON_SCALE_DN
  ,MUON_SCALE_UP
  ,MUON_TTVA_STAT_DN
  ,MUON_TTVA_STAT_UP
  ,MUON_TTVA_SYS_DN
  ,MUON_TTVA_SYS_UP
  ,MUON_SAGITTA_RESBIAS_DN
  ,MUON_SAGITTA_RESBIAS_UP
  ,MUON_SAGITTA_RHO_DN
  ,MUON_SAGITTA_RHO_UP
  ,MUON_BADMUON_STAT_DN
  ,MUON_BADMUON_STAT_UP
  ,MUON_BADMUON_SYS_DN
  ,MUON_BADMUON_SYS_UP
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
    ,"EG_SCALE_ALL_DN"
    ,"EG_SCALE_ALL_UP"
    ,"EL_EFF_ID_TOTAL_Uncorr_DN"
    ,"EL_EFF_ID_TOTAL_Uncorr_UP"
    ,"EL_EFF_Iso_TOTAL_Uncorr_DN"
    ,"EL_EFF_Iso_TOTAL_Uncorr_UP"
    ,"EL_EFF_Reco_TOTAL_Uncorr_DN"
    ,"EL_EFF_Reco_TOTAL_Uncorr_UP"
    ,"EL_EFF_Trigger_TOTAL_Uncorr_DN"
    ,"EL_EFF_Trigger_TOTAL_Uncorr_UP"
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
    ,"JER"
    ,"JET_GroupedNP_1_UP"
    ,"JET_GroupedNP_1_DN"
    ,"JET_GroupedNP_2_UP"
    ,"JET_GroupedNP_2_DN"
    ,"JET_GroupedNP_3_UP"
    ,"JET_GroupedNP_3_DN"
    ,"JET_JVTEff_UP"
    ,"JET_JVTEff_DN"
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
    ,"MUON_ID_DN"
    ,"MUON_ID_UP"
    ,"MUON_MS_DN"
    ,"MUON_MS_UP"
    ,"MUON_SCALE_DN"
    ,"MUON_SCALE_UP"
    ,"MUON_TTVA_STAT_DN"
    ,"MUON_TTVA_STAT_UP"
    ,"MUON_TTVA_SYS_DN"
    ,"MUON_TTVA_SYS_UP"
    ,"MUON_SAGITTA_RESBIAS_DN"
    ,"MUON_SAGITTA_RESBIAS_UP"
    ,"MUON_SAGITTA_RHO_DN"
    ,"MUON_SAGITTA_RHO_UP"
    ,"MUON_BADMUON_STAT_DN"
    ,"MUON_BADMUON_STAT_UP"
    ,"MUON_BADMUON_SYS_DN"
    ,"MUON_BADMUON_SYS_UP"
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
