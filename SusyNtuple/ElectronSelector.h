// -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRONSELECTOR_H
#define SUSYNTUPLE_ELECTRONSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"


namespace Susy {

    enum class electronId {
        veryLooseLLH,
        looseLLH,
        mediumLLH,
        tightLLH,
        looseLLH_nod0,
        mediumLLH_nod0,
        tightLLH_nod0,
        kUnknown
    };

    /// A class to select electrons

    class Electron;
    class Muon;

    class ElectronSelector
    {
        public:
        ElectronSelector();
        ElectronSelector& setSystematic(const NtSys::SusyNtSys& systematic);
        ElectronSelector& setAnalysis(const AnalysisType& analysis);
        /**
            Given the AnalysisType set in "setAnalysis", sets the kinematic,
            IP, and isolation requirements for electrons.
        */
        void buildRequirements(const AnalysisType& ana);
        /**
            Input electron "ele" is required to pass ID quality flags,
            impact parameter cuts, and isolation cuts.
        */
        bool isSignalElectron(const Electron* ele, const ElectronVector& baseElectrons,
                              const MuonVector& baseMuons, const unsigned int nVtx,
                              bool isMC, bool removeLepsFromIso);
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
            Possible IDs: veryLooseLLH, looseLLH, mediumLLH, tightLLH,
                          looseLLH_nod0, mediumLLH_nod0, tightLLH_nod0
        */
        bool elecPassID(const Electron* ele, bool signalQuality);
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
    
        
    
        float EL_MIN_PT;                        ///< minimum allowed electron pT
        float EL_MAX_ETA;                       ///< maximum allowed electron eta
        float EL_ISO_PT_THRS;                   ///< electron iso pt threshold ? (can 2lepWH analyzer comment?)
        float EL_PTCONE30_PT_CUT;               ///< maximum allowed ptcone30/pt 
        float EL_TOPOCONE30_SLOPE_DATA_CUT;     ///< pile-up correction term for topoetcone30 (data)
        float EL_TOPOCONE30_SLOPE_MC_CUT;       ///< pile-up correction term for topoetcone30 (MC)
        float EL_TOPOCONE30_PT_CUT;             ///< maximum allowed etconetopo30/pt 
        float EL_MAX_D0SIG_CUT;                 ///< maximum allowed d0sig
        float EL_MAX_Z0_SINTHETA;               ///< maximum allowed z0sinTheta
    
    
    
        protected :
        NtSys::SusyNtSys m_systematic;
        AnalysisType m_analysis;
        bool m_removeLepsFromIso;  
        bool m_doIPCut;
        bool m_doPtconeCut;
        bool m_doElEtconeCut;
        bool m_doMuEtconeCut;
        
        electronId m_eleID;    ///< electron quality requirement (selected from eleID enum)

        ///////////////////////////////
        // Available analyses
        ///////////////////////////////
        bool m_2lep;    ///< flag for 2L analysis
        bool m_3lep;    ///< flag for 3L analysis
        bool m_2lepWH;  ///< flag for 2L WH analysis

        // set verbose
        bool m_verbose;
    
    }; // class

} // namepsace Susy

#endif


