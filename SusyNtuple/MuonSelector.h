// -*- c++ -*-
#ifndef SUSYNTUPLE_MUONSELECTOR_H
#define SUSYNTUPLE_MUONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"


namespace Susy {

    class Electron;
    class Muon;

    /// A class to select muons
    
    class MuonSelector
    {
        public :
        /// constructor
        MuonSelector();
        MuonSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        MuonSelector& setAnalysis(const AnalysisType& analysis);
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
        bool isSemiSignalMuon(const Muon* mu);
        float muEtConeCorr(const Muon* mu, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);


        float MU_MIN_PT;
        float MU_MAX_ETA;
        float MU_ISO_PT_THRS;
        float MU_PTCONE30_SLOPE_DATA;
        float MU_PTCONE30_SLOPE_MC;
        float MU_PTCONE30_PT_CUT;
        float MU_PTCONE30ELSTYLE_PT_CUT;
        float MU_ETCONE30_K1_DATA;
        float MU_ETCONE30_K2_DATA;
        float MU_ETCONE30_K1_MC;
        float MU_ETCONE30_K2_MC;
        float MU_ETCONE30_PT_CUT;
        float MU_MAX_D0SIG_CUT;
        float MU_MAX_Z0_SINTHETA;
    
    
    
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis; // TODO : decide: bools or ana type? easier to check bools
        bool m_removeLepsFromIso;
        bool m_doIPCut;
        bool m_doPtconeCut;
        bool m_doElEtConeCut;
        bool m_doMuEtconeCut;

        //////////////////////////////
        // Available analyses
        //////////////////////////////
        bool m_2lep;
        bool m_3lep;
        bool m_2lepWH;

        // set verbose
        bool m_verbose;
    
    }; // class
} // end namespace

#endif
    
