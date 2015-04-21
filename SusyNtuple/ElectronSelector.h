// -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRONSELECTOR_H
#define SUSYNTUPLE_ELECTRONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"


namespace Susy {
    /// A class to select electrons

    class Electron;
    class Muon;

    class ElectronSelector
    {
        public:
        ElectronSelector();
        ElectronSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        ElectronSelector& setAnalysis(const AnalysisType& analysis);
        void buildRequirements(const AnalysisType& ana);
        bool isSignalElectron(const Electron* ele, const ElectronVector& baseElectrons,
                              const MuonVector& baseMuons, const unsigned int nVtx,
                              bool isMC, bool removeLepsFromIso);
        bool isSemiSignalElectron(const Electron* ele);
        float elPtConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
        float elEtTopoConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
    
        
    
        float EL_MIN_PT;
        float EL_MAX_ETA;
        float EL_ISO_PT_THRS;
        float EL_PTCONE30_PT_CUT;
        float EL_TOPOCONE30_SLOPE_DATA_CUT;
        float EL_TOPOCONE30_SLOPE_MC_CUT;
        float EL_TOPOCONE30_PT_CUT;
        float EL_MAX_D0SIG_CUT;
        float EL_MAX_Z0_SINTHETA;
    
    
    
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis;
        bool m_doIPCut;
        bool m_doPtconeCut;
        bool m_doElEtconeCut;
        bool m_doMuEtconeCut;

        ///////////////////////////////
        // Available analyses
        ///////////////////////////////
        bool m_2lep;
        bool m_3lep;
        bool m_2lepWH;

        // set verbose
        bool m_verbose;
    
    }; // class

} // namepsace Susy

#endif


