// -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRONSELECTOR_H
#define SUSYNTUPLE_ELECTRONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/ElectronId.h"
#include "SusyNtuple/Isolation.h"


namespace Susy {


    class Electron;
    class Muon;

    /// A class to select electrons
    class ElectronSelector
    {
        public:
        ElectronSelector();
        ElectronSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        ElectronSelector& setAnalysis(const AnalysisType& analysis);
        /**
            Method to retrieve the isolation requirement for signal electrons.
            This is helpful for other tools that need to know how the
            electrons are defined.
        */
        Isolation signalIsolation() { return m_sigIso; }
        /**
            Input electron "ele" is required to pass ID quality flags,
        */
        bool isBaselineElectron(const Electron* ele);
        /**
            Input electron "ele" is required to pass ID quality flags,
            impact parameter cuts, and isolation cuts.
        */
        bool isSignalElectron(const Electron* ele);
        /**
            Input electron "ele" is required only to pass ID quality 
            and impact parameter cuts.
        */
        bool isSemiSignalElectron(const Electron* ele); 
        /**
            Check whether the input electron "ele" passes a given ID quality 
            requirement.
            If "signalQuality" is set true, will check against the analysis'
            signal electron quality requirement. If set false, this will 
            check against the analysis' baseline electron quality requirement.
            Possible IDs: veryLooseLH, looseLH, mediumLH, tightLH,
                          looseLH_nod0, mediumLH_nod0, tightLH_nod0
        */
        bool elecPassID(const Electron* ele, bool signalQuality);
        /**
            Check whether the input electron "ele" passes a given isolation quality
            set by IsolationSelectionTool. 
        */
        bool elecPassIsolation(const Electron* ele);
        /**
            Return ptcone30 for input electron "ele". If removeLeptsFromIso=True
            removes any light-lepton pT contribution to the dR=0.3 cone 
            surrounding "ele" if the light-lepton is "semiSignal".
        */
        float elPtConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
        /**
            Return the pile-up corrected etconetopo30 of the input electron "ele". 
            If removeLepsFromIso=True, removes any "semiSignal" electron contribution 
            to the dR=0.28 cone around "ele".
        */
        float elEtTopoConeCorr(const Electron* ele, const ElectronVector& baseElectrons,
                           const MuonVector& baseMuons, const unsigned int nVtx,
                           bool isMC, bool removeLepsFromIso);
    
        
        /////////////////////////////////////////
        // Analysis Selections 
        /////////////////////////////////////////
        // pT/eta
        float EL_MIN_PT_BASELINE;       ///< minimum allowed pT for baseline electrons (GeV)
        float EL_MIN_PT_SIGNAL;         ///< minimum allowed pT for signal electrons (GeV)
        float EL_MAX_ETA_BASELINE;      ///< maximum allowed eta for baseline electrons
        float EL_MAX_ETA_SIGNAL;        ///< maximum allowed eta for signal electrons
        // Isolation
        float EL_ISO_PT_THRS;
        float EL_PTCONE30_PT_CUT;       ///< maximum allowed ptcone30/pt 
        float EL_TOPOCONE30_PT_CUT;     ///< maximum allowed etconetopo30/pt 
        // IP
        float EL_MAX_D0SIG;         ///< maximum allowed d0sig
        float EL_MAX_Z0_SINTHETA;       ///< maximum allowed z0sinTheta
        // places you don't want to hang around
        float EL_MIN_CRACK_ETA;         ///< minium crack eta
        float EL_MAX_CRACK_ETA;         ///< maximum crack eta

 
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis;
        bool m_vetoCrackRegion; 
        bool m_doIPCut;
        
        ElectronId m_eleBaseId;    ///< electron quality requirement (selected from eleID enum)
        ElectronId m_eleId;        ///< electron quality requirement (selected from eleID enum)
        Isolation m_sigIso;        ///< electron isolation qualiy for signal electrons (c.f. SusyNtuple/Isolation.h)

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

} // namepsace Susy

#endif


