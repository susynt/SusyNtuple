#ifndef SUSYNTUPLE_PHOTONSELECTOR_H
#define SUSYNTUPLE_PHOTONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"

namespace Susy {

    class Photon;

    /// A class to select photons
    class PhotonSelector
    {
        public :
        PhotonSelector();
        PhotonSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        PhotonSelector& setAnalysis(const AnalysisType& analysis);


        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis;

        Isolation m_sigIso;                 ///< photon isolation quality for photons

        ///////////////////////////////
        // Available analyses
        ///////////////////////////////
        bool m_2lep;    ///< flag for 2L analysis
        bool m_3lep;    ///< flag for 3L analysis
        bool m_2lepWH;  ///< flag for 2L WH analysis
        bool m_SS3L;    ///< flag for the strong SS3L analysis 

        // set verbose
        bool m_verbose;
    
    }; // class


} // namespace Susy

#endif
