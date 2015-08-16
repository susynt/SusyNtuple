#ifndef SUSYNTUPLE_TAUSELECTOR_H
#define SUSYNTUPLE_TAUSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/TauId.h"


namespace Susy {

    class Tau;

    /// A class to select taus
    class TauSelector
    {
        public:
        TauSelector();
        TauSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        TauSelector& setAnalysis(const AnalysisType& analysis);

        bool isBaselineTau(const Tau* tau);
        bool isBaselineTau(const Tau& tau);

        bool isSignalTau(const Tau* tau, const TauId& jetId, const TauId& eleId, const TauId& muoId);
        bool isSignalTau(const Tau& tau, const TauId& jetId, const TauId& eleId, const TauId& muoId);

        bool isTauBDT(const Tau*, const TauId& jetId, const TauId& eleId, const TauId& muoId);
        bool isTauBDT(const Tau&, const TauId& jetId, const TauId& eleId, const TauId& muoId);
        bool tauPassBDT(const Tau&, bool isSignal);

        TauId signalTauId() { return m_signalTauId; }


        void check();

        //////////////////////////////////////
        // Analysis Selections
        //////////////////////////////////////
        float TAU_MIN_PT_BASELINE;          ///< minimum allowed pT for baseline taus (GeV)
        float TAU_MIN_PT_SIGNAL;            ///< minimum allowed pT for signal taus (GeV)

        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis;

        TauId m_signalTauId; ///< Toggle for which set of Taus to use as "signal" taus

        TauId m_tauEleBaseId;
        TauId m_tauJetBaseId;
        TauId m_tauMuoBaseId;

        TauId m_tauEleId;
        TauId m_tauJetId;
        TauId m_tauMuoId;

        //////////////////////////////////////
        // Available Analyses
        //////////////////////////////////////
        bool m_2lep;        ///< flag for 2L analysis
        bool m_3lep;        ///< flag for 3L analysis
        bool m_2lepWH;      ///< flag for 2L WH analysis
        bool m_SS3L;        ///< flag for the strong SS3L analysis
        bool m_stop2l;      ///< flag for the stop to two lepton analysis

        // set verbose
        bool m_verbose;

    }; // class

} // namespace Susy
#endif
