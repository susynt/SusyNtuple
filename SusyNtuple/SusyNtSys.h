//  -*- c++ -*-
#ifndef SusyNtuple_SusyNtSys_h
#define SusyNtuple_SusyNtSys_h

#include <string>
#include <map>

namespace Susy {

namespace NtSys {

enum SusyNtSys {

    ////////////////////////////////////////////
    // NOMINAL
    ////////////////////////////////////////////
    NOM

    ////////////////////////////////////////////
    // EGAMMA
    ////////////////////////////////////////////
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
    ,EL_EFF_Trigger_TOTAL_DN
    ,EL_EFF_Trigger_TOTAL_UP

    ////////////////////////////////////////////
    // FLAVOR TAGGING
    ////////////////////////////////////////////
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

    ////////////////////////////////////////////
    // JET ENERGY RESOLUTION
    ////////////////////////////////////////////
    ,JER

    ////////////////////////////////////////////
    // JET ENERGY SCALE (STRONGLY REDUCED)
    ////////////////////////////////////////////
    ,JET_GroupedNP_1_UP
    ,JET_GroupedNP_1_DN
    ,JET_GroupedNP_2_UP
    ,JET_GroupedNP_2_DN
    ,JET_GroupedNP_3_UP
    ,JET_GroupedNP_3_DN

    ////////////////////////////////////////////
    // JET ETA INTERCALIBRATION NON CLOSURE
    ////////////////////////////////////////////
    ,JET_EtaIntercalibration_UP
    ,JET_EtaIntercalibration_DN

    ////////////////////////////////////////////
    // JVT EFFICIENCY
    ////////////////////////////////////////////
    ,JET_JVTEff_UP
    ,JET_JVTEff_DN

    ////////////////////////////////////////////
    // MET SOFT TERM
    ////////////////////////////////////////////
    ,MET_SoftCalo_Reso
    ,MET_SoftCalo_ScaleDown
    ,MET_SoftCalo_ScaleUp
    ,MET_SoftTrk_ResoPara
    ,MET_SoftTrk_ResoPerp
    ,MET_SoftTrk_ScaleDown
    ,MET_SoftTrk_ScaleUp

    ////////////////////////////////////////////
    // MUON
    ////////////////////////////////////////////
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

    ////////////////////////////////////////////
    // TAUS
    ////////////////////////////////////////////
    ,TAUS_EFF_CONTJETID_STAT_DN  //weight
    ,TAUS_EFF_CONTJETID_STAT_UP  //weight
    ,TAUS_EFF_CONTJETID_SYST_DN  //weight
    ,TAUS_EFF_CONTJETID_SYST_UP  //weight
    ,TAUS_SME_TOTAL_DN
    ,TAUS_SME_TOTAL_UP

    ////////////////////////////////////////////
    // PRW
    ////////////////////////////////////////////
    ,PILEUP_UP       ///< Positive shift for mu 
    ,PILEUP_DN       ///< Negative shift for mu

    ////////////////////////////////////////////
    // XSEC
    ////////////////////////////////////////////
    ,XS_UP
    ,XS_DN

    ////////////////////////////////////////////
    // NOPE NOPE NOPE TURN BACK
    ////////////////////////////////////////////
    ,SYS_UNKNOWN
};

const std::map<SusyNtSys, std::string> SusyNtSysNames {

    ////////////////////////////////////////////
    // NOMINAL
    ////////////////////////////////////////////
    {NOM,                               "NOM"},

    ////////////////////////////////////////////
    // EGAMMA
    ////////////////////////////////////////////
    {EG_RESOLUTION_ALL_DN,              "EG_RESOLUTION_ALL_DN"},
    {EG_RESOLUTION_ALL_UP,              "EG_RESOLUTION_ALL_UP"},
    {EG_SCALE_ALL_DN,                   "EG_SCALE_ALL_DN"},
    {EG_SCALE_ALL_UP,                   "EG_SCALE_ALL_UP"},
    {EL_EFF_ID_TOTAL_Uncorr_DN,         "EL_EFF_ID_TOTAL_Uncorr_DN"},
    {EL_EFF_ID_TOTAL_Uncorr_UP,         "EL_EFF_ID_TOTAL_Uncorr_UP"},
    {EL_EFF_Iso_TOTAL_Uncorr_DN,        "EL_EFF_Iso_TOTAL_Uncorr_DN"},
    {EL_EFF_Iso_TOTAL_Uncorr_UP,        "EL_EFF_Iso_TOTAL_Uncorr_UP"},
    {EL_EFF_Reco_TOTAL_Uncorr_DN,       "EL_EFF_Reco_TOTAL_Uncorr_DN"},
    {EL_EFF_Reco_TOTAL_Uncorr_UP,       "EL_EFF_Reco_TOTAL_Uncorr_UP"},
    {EL_EFF_Trigger_TOTAL_DN,           "EL_EFF_Trigger_TOTAL_DN"},
    {EL_EFF_Trigger_TOTAL_UP,           "EL_EFF_Trigger_TOTAL_UP"},

    ////////////////////////////////////////////
    // FLAVOR TAGGING
    ////////////////////////////////////////////
    {FT_EFF_B_systematics_UP,           "FT_EFF_B_systematics_UP"},
    {FT_EFF_B_systematics_DN,           "FT_EFF_B_systematics_DN"},
    {FT_EFF_C_systematics_UP,           "FT_EFF_C_systematics_UP"},
    {FT_EFF_C_systematics_DN,           "FT_EFF_C_systematics_DN"},
    {FT_EFF_Light_systematics_UP,       "FT_EFF_Light_systematics_UP"},
    {FT_EFF_Light_systematics_DN,       "FT_EFF_Light_systematics_DN"},
    {FT_EFF_extrapolation_UP,           "FT_EFF_extrapolation_UP"},
    {FT_EFF_extrapolation_DN,           "FT_EFF_extrapolation_DN"},
    {FT_EFF_extrapolation_charm_UP,     "FT_EFF_extrapolation_charm_UP"},
    {FT_EFF_extrapolation_charm_DN,     "FT_EFF_extrapolation_charm_DN"},

    ////////////////////////////////////////////
    // JET ENERGY RESOLUTION
    ////////////////////////////////////////////
    {JER,                               "JER"},

    ////////////////////////////////////////////
    // JET ENERGY SCALE (STRONGLY REDUCED)
    ////////////////////////////////////////////
    {JET_GroupedNP_1_UP,                "JET_GroupedNP_1_UP"},
    {JET_GroupedNP_1_DN,                "JET_GroupedNP_1_DN"},
    {JET_GroupedNP_2_UP,                "JET_GroupedNP_2_UP"},
    {JET_GroupedNP_2_DN,                "JET_GroupedNP_2_DN"},
    {JET_GroupedNP_3_UP,                "JET_GroupedNP_3_UP"},
    {JET_GroupedNP_3_DN,                "JET_GroupedNP_3_DN"},

    ////////////////////////////////////////////
    // JET ETA INTERCALIBRATION NON CLOSURE
    ////////////////////////////////////////////
    {JET_EtaIntercalibration_UP,        "JET_EtaIntercalibration_UP"},
    {JET_EtaIntercalibration_DN,        "JET_EtaIntercalibration_DN"},

    ////////////////////////////////////////////
    // JVT EFFICIENCY
    ////////////////////////////////////////////
    {JET_JVTEff_UP,                     "JET_JVTEff_UP"},
    {JET_JVTEff_DN,                     "JET_JVTEff_DN"},

    ////////////////////////////////////////////
    // MET SOFT TERM
    ////////////////////////////////////////////
    {MET_SoftCalo_Reso,                 "MET_SoftCalo_Reso"},
    {MET_SoftCalo_ScaleDown,            "MET_SoftCalo_ScaleDown"},
    {MET_SoftCalo_ScaleUp,              "MET_SoftCalo_ScaleUp"},
    {MET_SoftTrk_ResoPara,              "MET_SoftTrk_ResoPara"},
    {MET_SoftTrk_ResoPerp,              "MET_SoftTrk_ResoPerp"},
    {MET_SoftTrk_ScaleDown,             "MET_SoftTrk_ScaleDown"},
    {MET_SoftTrk_ScaleUp,               "MET_SoftTrk_ScaleUp"},

    ////////////////////////////////////////////
    // MUON
    ////////////////////////////////////////////
    {MUON_EFF_STAT_DN,                  "MUON_EFF_STAT_DN"},
    {MUON_EFF_STAT_UP,                  "MUON_EFF_STAT_UP"},
    {MUON_EFF_STAT_LOWPT_DN,            "MUON_EFF_STAT_LOWPT_DN"},
    {MUON_EFF_STAT_LOWPT_UP,            "MUON_EFF_STAT_LOWPT_UP"},
    {MUON_EFF_SYS_DN,                   "MUON_EFF_SYS_DN"},
    {MUON_EFF_SYS_UP,                   "MUON_EFF_SYS_UP"},
    {MUON_EFF_SYS_LOWPT_DN,             "MUON_EFF_SYS_LOWPT_DN"},
    {MUON_EFF_SYS_LOWPT_UP,             "MUON_EFF_SYS_LOWPT_UP"},
    {MUON_EFF_TRIG_STAT_DN,             "MUON_EFF_TRIG_STAT_DN"},
    {MUON_EFF_TRIG_STAT_UP,             "MUON_EFF_TRIG_STAT_UP"},
    {MUON_EFF_TRIG_SYST_DN,             "MUON_EFF_TRIG_SYST_DN"},
    {MUON_EFF_TRIG_SYST_UP,             "MUON_EFF_TRIG_SYST_UP"},
    {MUON_ISO_STAT_DN,                  "MUON_ISO_STAT_DN"},
    {MUON_ISO_STAT_UP,                  "MUON_ISO_STAT_UP"},
    {MUON_ISO_SYS_DN,                   "MUON_ISO_SYS_DN"},
    {MUON_ISO_SYS_UP,                   "MUON_ISO_SYS_UP"},
    {MUON_ID_DN,                        "MUON_ID_DN"},
    {MUON_ID_UP,                        "MUON_ID_UP"},
    {MUON_MS_DN,                        "MUON_MS_DN"},
    {MUON_MS_UP,                        "MUON_MS_UP"},
    {MUON_SCALE_DN,                     "MUON_SCALE_DN"},
    {MUON_SCALE_UP,                     "MUON_SCALE_UP"},
    {MUON_TTVA_STAT_DN,                 "MUON_TTVA_STAT_DN"},
    {MUON_TTVA_STAT_UP,                 "MUON_TTVA_STAT_UP"},
    {MUON_TTVA_SYS_DN,                  "MUON_TTVA_SYS_DN"},
    {MUON_TTVA_SYS_UP,                  "MUON_TTVA_SYS_UP"},
    {MUON_SAGITTA_RESBIAS_DN,           "MUON_SAGITTA_RESBIAS_DN"},
    {MUON_SAGITTA_RESBIAS_UP,           "MUON_SAGITTA_RESBIAS_UP"},
    {MUON_SAGITTA_RHO_DN,               "MUON_SAGITTA_RHO_DN"},
    {MUON_SAGITTA_RHO_UP,               "MUON_SAGITTA_RHO_UP"},
    {MUON_BADMUON_STAT_DN,              "MUON_BADMUON_STAT_DN"},
    {MUON_BADMUON_STAT_UP,              "MUON_BADMUON_STAT_UP"},
    {MUON_BADMUON_SYS_DN,               "MUON_BADMUON_SYS_DN"},
    {MUON_BADMUON_SYS_UP,               "MUON_BADMUON_SYS_UP"},

    ////////////////////////////////////////////
    // TAUS
    ////////////////////////////////////////////
    {TAUS_EFF_CONTJETID_STAT_DN,        "TAUS_EFF_CONTJETID_STAT_DN"},
    {TAUS_EFF_CONTJETID_STAT_UP,        "TAUS_EFF_CONTJETID_STAT_UP"},
    {TAUS_EFF_CONTJETID_SYST_DN,        "TAUS_EFF_CONTJETID_SYST_DN"},
    {TAUS_EFF_CONTJETID_SYST_UP,        "TAUS_EFF_CONTJETID_SYST_UP"},
    {TAUS_SME_TOTAL_DN,                 "TAUS_SME_TOTAL_DN"},
    {TAUS_SME_TOTAL_UP,                 "TAUS_SME_TOTAL_UP"},

    ////////////////////////////////////////////
    // PRW
    ////////////////////////////////////////////
    {PILEUP_UP,                         "PILEUP_UP"},
    {PILEUP_DN,                         "PILEUP_DN"},

    ////////////////////////////////////////////
    // XSEC
    ////////////////////////////////////////////
    {XS_UP,                             "XS_UP"},
    {XS_DN,                             "XS_DN"},

    ////////////////////////////////////////////
    // HOLD YOUR HORSES
    ////////////////////////////////////////////
    {SYS_UNKNOWN,                       "SYS_UNKNOWN"}
};

} //NtSys

} // Susy


#endif
