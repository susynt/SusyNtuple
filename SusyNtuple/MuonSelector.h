// -*- c++ -*-
#ifndef SUSYNTUPLE_MUONSELECTOR_H
#define SUSYNTUPLE_MUONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"
#include "SusyNtuple/MuonId.h"


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
            Method to retrieve the isolation requirement for signal muons.
            This is helpful for other tools that need to know how the
            the muons are configured.
        */
        Isolation signalIsolation() { return m_sigIso; } 
        /**
            Check that the input muon "mu" passes the quality
            level required for the analysis.
        */
        bool muPassQuality(const Muon* mu, bool signalQuality);
        /**
            Input muon "mu" is required to pass baseline selection
        */
        bool isBaselineMuon(const Muon* mu);
        /**
            Input muon "mu" is required to pass impact parameter and
            isolation cuts.
        */
        bool isSignalMuon(const Muon* mu);
        /**
            Check whether the input muon "mu" passes a given isolation quality
            set by IsolationSelectionTool (parameter m_sigIso below).
        */
        bool muPassIsolation(const Muon* mu);
        /**
            Input muon "mu" is required only to pass impact parameter cuts
        */
        bool isSemiSignalMuon(const Muon* mu);
        /**
            Get the input muon "mu"'s nominal efficiency SF
        */
        float effSF(const Muon* mu) { return effSF(*mu); }
        float effSF(const Muon& mu);
        /**
            Get the error on the input muon "mu"'/s efficiency SF
            for the requested systematic
        */
        float errEffSF(const Muon* mu, const NtSys::SusyNtSys sys) { return errEffSF(*mu, sys); }
        float errEffSF(const Muon& mu, const NtSys::SusyNtSys sys);

        void check();

        //////////////////////////////////////
        // Analysis Selections
        //////////////////////////////////////
        // pT/eta
        float MU_MIN_PT_BASELINE;           ///< minimum allowed pT for baseline muons (GeV)
        float MU_MIN_PT_SIGNAL;             ///< minimum allowed pT for signal muons (GeV)
        float MU_MAX_ETA_BASELINE;          ///< maximum allowed eta for baseline muons
        float MU_MAX_ETA_SIGNAL;            ///< maximum allowed eta for signal muons
        // isolation
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
        // IP
        float MU_MAX_D0SIG    ;             ///< maximum allowed d0sig
        float MU_MAX_Z0_SINTHETA;           ///< maximum allowed z0sinTheta

 
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis; // TODO : decide: bools or ana type? easier to check bools
        bool m_doIPCut;

        MuonId m_muBaseId;      ///< muon ID quality requirement for baseline muons
        MuonId m_muId;          ///< muon ID quality requirement for signal muons

        Isolation m_sigIso;     ///< muon isolation quality for signal muons (c.f. SusyNtuple/Isolation.h)

        //////////////////////////////
        // Available analyses
        //////////////////////////////
        bool m_2lep;
        bool m_3lep;
        bool m_2lepWH;
        bool m_SS3L;
        bool m_stop2l;

        // set verbose
        bool m_verbose;
    
    }; // class
} // end namespace

#endif
    
