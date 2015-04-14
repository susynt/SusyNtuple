// -*- c++ -*-
#ifndef SUSYNTUPLE_MUONSELECTOR_H
#define SUSYNTUPLE_MUONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"


namespace Susy {
    /// A class to select muons
    
    class MuonSelector
    {
        public :
        /// constructor
        MuonSelector(const SusyNtSys& systematic, const AnalysisType& analysis);
        void buildRequirements(const AnalysisType& ana);
        bool isSignalMuon(const Muon* mu, const ElectronVector& baseElectrons,
                          const MuonVector& baseMuons, const unsigned int nVtx,
                          bool isMC, bool removeLepsFromIso);
        bool muPassPtIso(const Muon* mu, const ElectronVector& baseElectrons,
                         const MuonVector& baseMuons, const unsigned int nVtx,
                         bool isMC, bool removeLepsFromIso);
        float muPtConeCorr(const Muon* mu, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
        bool isSemiSignalMuon(const Muon& mu);
        float muEtConeCorr(const Muon* mu, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);


        const float MU_MIN_PT;
        const float MU_MAX_ETA;
        const float MU_ISO_PT_THRS;
        const float MU_PTCONE30_SLOPE_DATA;
        const float MU_PTCONE30_SLOPE_MC;
        const float MU_PTCONE30_PT_CUT;
        const float MU_PTCONE30ELSTYLE_PT_CUT;
        const float MU_ETCONE30_K1_DATA;
        const float MU_ETCONE30_K2_DATA;
        const float MU_ETCONE30_K1_MC;
        const float MU_ETCONE30_K2_MC;
        const float MU_ETCONE30_PT_CUT;
        const float MU_D0SIG_CUT;
        const float MU_MAX_Z0_SINTHETA;
    
    
    
        protected :
        const SusyNtSys m_systematic;
        const AnalysisType m_analysis; // TODO : decide: bools or ana type? easier to check bools
        const bool m_doIPCut;
        const bool m_doPtconeCut;
        const bool m_doElEtConeCut;
        const bool m_doMuEtconeCut;

        //////////////////////////////
        // Available analyses
        //////////////////////////////
        const bool m_2lep;
        const bool m_3lep;
        const bool m_2lepWH;

        // set verbose
        const bool m_verbose;
    
    }; // class
} // end namespace

#endif
    
