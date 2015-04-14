// -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRONSELECTOR_H
#define SUSYNTUPLE_ELECTRONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"





namespace Susy {
    /// A class to select electrons


    class ElectronSelector
    {
        public:
        ElectronSelector(const SusyNtSys& systematic, const AnalysisType& analysis);
        void buildRequirements(const AnalysisType& ana);
        bool isSignalElectron(const Electron* ele, const ElectronVector& baseElectrons,
                              const MuonVector& baseMuons, const unsigned int nVtx,
                              bool isMC, bool removeLepsFromIso);
        bool isSemiSignalElectron(const Electron* ele);
        float elPtConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
        float elEtConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
    
        
    
        const float EL_MIN_PT;
        const float EL_MAX_ETA;
        const float EL_ISO_PT_THRS;
        const float EL_PTCONE30_PT_CUT;
        const float EL_TOPOCONE30_SLOPE_DATA_CUT;
        const float EL_TOPOCONE30_SLOPE_MC_CUT;
        const float EL_TOPOCONE30_PT_CUT;
        const float EL_MAX_D0SIG_CUT;
        const float EL_MAX_Z0_SINTHETA;
    
    
    
        protected :
        const SusyNtSys m_systematic;
        const AnalysisType m_analysis;
        const bool m_doIPCut;
        const bool m_doPtconeCut;
        const bool m_doElEtconeCut;
        const bool m_doMuEtconeCut;

        ///////////////////////////////
        // Available analyses
        ///////////////////////////////
        const bool m_2lep;
        const bool m_3lep;
        const bool m_2lepWH;

        // set verbose
        const bool m_verbose;
    
    }; // class

} // namepsace Susy

#endif


