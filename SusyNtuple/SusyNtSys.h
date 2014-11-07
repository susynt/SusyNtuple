#ifndef SusyNtuple_SusyNtSys_h
#define SusyNtuple_SusyNtSys_h


namespace Susy {

namespace NtSys {

enum SusyNtSys {
  NOM             // Central value
 
  //see EgammaCalibrationAndSmearingTool
  ,EL_SCALE_MOMENTUM_DOWN //electron only
  ,EL_SCALE_MOMENTUM_UP
  ,EG_RESOLUTION_ALL_DOWN //electron/photon
  ,EG_RESOLUTION_ALL_UP
  ,EG_RESOLUTION_LASTRESOLUTIONVARIATION
  ,EG_RESOLUTION_MATERIALCALO_DOWN
  ,EG_RESOLUTION_MATERIALCALO_UP
  ,EG_RESOLUTION_MATERIALCRYO_DOWN
  ,EG_RESOLUTION_MATERIALCRYO_UP
  ,EG_RESOLUTION_MATERIALGAP_DOWN
  ,EG_RESOLUTION_MATERIALGAP_UP
  ,EG_RESOLUTION_MATERIALID_DOWN
  ,EG_RESOLUTION_MATERIALID_UP
  ,EG_RESOLUTION_NOMINAL
  ,EG_RESOLUTION_NONE
  ,EG_RESOLUTION_PILEUP_DOWN
  ,EG_RESOLUTION_PILEUP_UP
  ,EG_RESOLUTION_SAMPLINGTERM_DOWN
  ,EG_RESOLUTION_SAMPLINGTERM_UP
  ,EG_RESOLUTION_ZSMEARING_DOWN
  ,EG_RESOLUTION_ZSMEARING_UP
  ,EG_SCALE_ALL_DOWN
  ,EG_SCALE_ALL_UP
  ,EG_SCALE_G4_DOWN
  ,EG_SCALE_G4_UP
  ,EG_SCALE_L1GAIN_DOWN
  ,EG_SCALE_L1GAIN_UP
  ,EG_SCALE_L2GAIN_DOWN
  ,EG_SCALE_L2GAIN_UP
  ,EG_SCALE_LARCALIB_DOWN
  ,EG_SCALE_LARCALIB_UP
  ,EG_SCALE_LARELECCALIB_DOWN
  ,EG_SCALE_LARELECCALIB_UP
  ,EG_SCALE_LARELECUNCONV_DOWN
  ,EG_SCALE_LARELECUNCONV_UP
  ,EG_SCALE_LARUNCONVCALIB_DOWN
  ,EG_SCALE_LARUNCONVCALIB_UP
  ,EG_SCALE_LASTSCALEVARIATION
  ,EG_SCALE_MATCALO_DOWN
  ,EG_SCALE_MATCALO_UP
  ,EG_SCALE_MATCRYO_DOWN
  ,EG_SCALE_MATCRYO_UP
  ,EG_SCALE_MATID_DOWN
  ,EG_SCALE_MATID_UP
  ,EG_SCALE_NOMINAL
  ,EG_SCALE_NONE
  ,EG_SCALE_PEDESTAL_DOWN
  ,EG_SCALE_PEDESTAL_UP
  ,EG_SCALE_PS_DOWN
  ,EG_SCALE_PS_UP
  ,EG_SCALE_S12_DOWN
  ,EG_SCALE_S12_UP
  ,EG_SCALE_ZEESTAT_DOWN
  ,EG_SCALE_ZEESTAT_UP
  ,EG_SCALE_ZEESYST_DOWN
  ,EG_SCALE_ZEESYST_UP
  // See MuonCalibrationAndSmearingTool
  ,MU_SF_STAT_DOWN
  ,MU_SF_STAT_UP
  ,MU_SF_SYS_DOWN
  ,MU_SF_SYS_UP
  ,MU_ID_DOWN
  ,MU_ID_UP
  ,MU_MS_DOWN
  ,MU_MS_UP
  ,MU_SCALE_DOWN
  ,MU_SCALE_UP
  
  ,TAUS_STAT_DOWN
  ,TAUS_STAT_UP
  ,TAUS_SYST_DOWN
  ,TAUS_SYST_UP
  ,TAUS_TOTAL_DOWN
  ,TAUS_TOTAL_UP
  
  //EIGEN* TO ADD ???
  ,JER_UP
  ,BJES_Response_DOWN
  ,BJES_Response_UP

  //AT:2014-11-06 OLD ONES - TO REMOVE/REPLACE

  //Object systematics - affect event selection 
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
  ,JER                // Jet energy resolution
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
    
    ,"EL_SCALE_MOMENTUM_DOWN"
    ,"EL_SCALE_MOMENTUM_UP"
    ,"EG_RESOLUTION_ALL_DOWN" //electron/photon
    ,"EG_RESOLUTION_ALL_UP"
    ,"EG_RESOLUTION_LASTRESOLUTIONVARIATION"
    ,"EG_RESOLUTION_MATERIALCALO_DOWN"
    ,"EG_RESOLUTION_MATERIALCALO_UP"
    ,"EG_RESOLUTION_MATERIALCRYO_DOWN"
    ,"EG_RESOLUTION_MATERIALCRYO_UP"
    ,"EG_RESOLUTION_MATERIALGAP_DOWN"
    ,"EG_RESOLUTION_MATERIALGAP_UP"
    ,"EG_RESOLUTION_MATERIALID_DOWN"
    ,"EG_RESOLUTION_MATERIALID_UP"
    ,"EG_RESOLUTION_NOMINAL"
    ,"EG_RESOLUTION_NONE"
    ,"EG_RESOLUTION_PILEUP_DOWN"
    ,"EG_RESOLUTION_PILEUP_UP"
    ,"EG_RESOLUTION_SAMPLINGTERM_DOWN"
    ,"EG_RESOLUTION_SAMPLINGTERM_UP"
    ,"EG_RESOLUTION_ZSMEARING_DOWN"
    ,"EG_RESOLUTION_ZSMEARING_UP"
    
    ,"EG_SCALE_ALL_DOWN"
    ,"EG_SCALE_ALL_UP"
    ,"EG_SCALE_G4_DOWN"
    ,"EG_SCALE_G4_UP"
    ,"EG_SCALE_L1GAIN_DOWN"
    ,"EG_SCALE_L1GAIN_UP"
    ,"EG_SCALE_L2GAIN_DOWN"
    ,"EG_SCALE_L2GAIN_UP"
    ,"EG_SCALE_LARCALIB_DOWN"
    ,"EG_SCALE_LARCALIB_UP"
    ,"EG_SCALE_LARELECCALIB_DOWN"
    ,"EG_SCALE_LARELECCALIB_UP"
    ,"EG_SCALE_LARELECUNCONV_DOWN"
    ,"EG_SCALE_LARELECUNCONV_UP"
    ,"EG_SCALE_LARUNCONVCALIB_DOWN"
    ,"EG_SCALE_LARUNCONVCALIB_UP"
    ,"EG_SCALE_LASTSCALEVARIATION"
    ,"EG_SCALE_MATCALO_DOWN"
    ,"EG_SCALE_MATCALO_UP"
    ,"EG_SCALE_MATCRYO_DOWN"
    ,"EG_SCALE_MATCRYO_UP"
    ,"EG_SCALE_MATID_DOWN"
    ,"EG_SCALE_MATID_UP"
    ,"EG_SCALE_NOMINAL"
    ,"EG_SCALE_NONE"
    ,"EG_SCALE_PEDESTAL_DOWN"
    ,"EG_SCALE_PEDESTAL_UP"
    ,"EG_SCALE_PS_DOWN"
    ,"EG_SCALE_PS_UP"
    ,"EG_SCALE_S12_DOWN"
    ,"EG_SCALE_S12_UP"
    ,"EG_SCALE_ZEESTAT_DOWN"
    ,"EG_SCALE_ZEESTAT_UP"
    ,"EG_SCALE_ZEESYST_DOWN"
    ,"EG_SCALE_ZEESYST_UP"

    ,"MU_SF_STAT_DOWN"
    ,"MU_SF_STAT_UP"
    ,"MU_SF_SYS_DOWN"
    ,"MU_SF_SYS_UP"
    ,"MU_ID_DOWN"
    ,"MU_ID_UP"
    ,"MU_MS_DOWN"
    ,"MU_MS_UP"
    ,"MU_SCALE_DOWN"
    ,"MU_SCALE_UP"
    
    ,"TAUS_STAT_DOWN"
    ,"TAUS_STAT_UP"
    ,"TAUS_SYST_DOWN"
    ,"TAUS_SYST_UP"
    ,"TAUS_TOTAL_DOWN"
    ,"TAUS_TOTAL_UP"
    
    ,"JER_UP"
    ,"BJES_Response_DOWN"
    ,"BJES_Response_UP"
    
    

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
    ,"JER"
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
