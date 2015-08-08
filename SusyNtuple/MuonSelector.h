// -*- c++ -*-
#ifndef SUSYNTUPLE_MUONSELECTOR_H
#define SUSYNTUPLE_MUONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"


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

        /**
            Given the AnalysisType set in "setAnalysis", sets the kinematic, 
            IP, and isolation requirements for muons.
        */
        void buildRequirements(const AnalysisType& ana);

        /**
            Input muon "mu" is required to pass baseline selection
        */
        bool isBaselineMuon(const Muon* mu);
        /**
            Input muon "mu" is required to pass impact parameter and
            isolation cuts.
        */
        bool isSignalMuon(const Muon* mu, const ElectronVector& baseElectrons,
                          const MuonVector& baseMuons, const unsigned int nVtx,
                          bool isMC, bool removeLepsFromIso);
        /**
            Check whether the input muon "mu" passes a given isolation quality
            set by IsolationSelectionTool (parameter m_sigIso below).
        */
        bool muPassIsolation(const Muon* mu);
        /**
            Check whether the input muon "mu" passes the analysis' ptcone30/pt
            isolation requirement.
        */
        bool muPassPtIso(const Muon* mu, const ElectronVector& baseElectrons,
                         const MuonVector& baseMuons, const unsigned int nVtx,
                         bool isMC, bool removeLepsFromIso);
        /**
            Return ptcone30 for the input muon "mu". If removeLepsFromIso=True
            removes any light-lepton contribution to the dR=0.3 cone surrounding
            "mu" if the light-lepton is "semiSignal".
        */
        float muPtConeCorr(const Muon* mu, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
        /**
            Input muon "mu" is required only to pass impact parameter cuts
        */
        bool isSemiSignalMuon(const Muon* mu);
        /**
            Return the pile-up corrected etcone30 of the input muon "mu".
            If removeLepsFromIso=True, removes any "semiSignal" electron
            contribution to the dR=0.28 cone around "mu".
        */
        float muEtConeCorr(const Muon* mu, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);


        float MU_MIN_PT;                    ///< minimum allowed muon pT
        float MU_MAX_ETA;                   ///< maximum allowed muon eta
        float MU_ISO_PT_THRS;               ///< muon iso pt threshold (can 2lepWH analyzer comment?)
        float MU_PTCONE30_SLOPE_DATA;       ///< pile-up correction term for ptcone30 (data)
        float MU_PTCONE30_SLOPE_MC;         ///< pile-up correction term for ptcone30 (MC)
        float MU_PTCONE30_PT_CUT;           ///< maximum allowed ptcon30/pt
        float MU_PTCONE30ELSTYLE_PT_CUT;    ///< dantrim May 4 : not sure what this is
        float MU_ETCONE30_K1_DATA;          ///< "1st-order" pile-up correction term for etcone30 (data)
        float MU_ETCONE30_K2_DATA;          ///< "2nd-order" pile-up correction term for etcone30 (data)
        float MU_ETCONE30_K1_MC;            ///< "1st-order" pile-up correction term for etcone30 (MC)
        float MU_ETCONE30_K2_MC;            ///< "2nd-order" pile-up correction term for etcone30 (MC)
        float MU_ETCONE30_PT_CUT;           ///< maximum allowed etcone30/pt
        float MU_MAX_D0SIG_CUT;             ///< maximum allowed d0sig
        float MU_MAX_Z0_SINTHETA;           ///< maximum allowed z0sinTheta
    
    
    
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis; // TODO : decide: bools or ana type? easier to check bools
        bool m_removeLepsFromIso;
        bool m_doIPCut;
        bool m_doPtconeCut;
        bool m_doElEtConeCut;
        bool m_doMuEtconeCut;

        Isolation m_sigIso;     ///< muon isolation quality for signal muons (c.f. SusyNtuple/Isolation.h)

        //////////////////////////////
        // Available analyses
        //////////////////////////////
        bool m_2lep;
        bool m_3lep;
        bool m_2lepWH;
        bool m_SS3L;

        // set verbose
        bool m_verbose;
    
    }; // class
} // end namespace

#endif
    
