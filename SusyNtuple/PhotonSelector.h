//  -*- c++ -*-
#ifndef SUSYNTUPLE_PHOTONSELECTOR_H
#define SUSYNTUPLE_PHOTONSELECTOR_H

#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"

namespace Susy {

    class Photon;

    /// A class to select photons
    /**
        The generic PhotonSelector implements the generic definitions from
        https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Photons

        Analysis-dependent criteria should be implemented in your
        analysis-specific class inheriting from PhotonSelector.

        The analysis-specific selector should be instantiated with PhotonSelector::build().
    */ 

    class PhotonSelector
    {
        public :
        /// provide analysis-specific selector (or vanila one of analysis is unknown)
        /**
            The user owns the selector (i.e. should use std::shared_ptr or delete it
            when doen with it).
        */
        static PhotonSelector* build(const AnalysisType &a, bool verbose);
        PhotonSelector(); ///< Default ctor
        virtual ~PhotonSelector() {}; ///< dtor (for now we don't have anything to delete)
        virtual bool isBaseline(const Photon* ph); ///< whether photon passes the baseline criteria
        /// whether photon passes the signal criteria
        virtual bool isSignal(const Photon* ph);

        Isolation signalIsolation() const { return m_signalIsolation; }

        /// set signal isolation
        /**
            Note: the value you set here should match whatever you have in _your_
            (overriding) implementation of isSignal()
        */
        PhotonSelector& setSignalIsolation(const Isolation &iso) { m_signalIsolation = iso; return *this; }

        PhotonSelector& setVerbose(const bool &v) { m_verbose = v; return *this; }
        bool verbose() const { return m_verbose; }

        protected :

        Isolation m_signalIsolation;
        bool m_verbose;
    }; // class PhotonSelector


// ------------------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
// ------------------------------------------------------------------

    /// Implement photon selector for Ana_2Lep
    class PhotonSelector_2Lep : public PhotonSelector
    {
    };

    /// Implement photon selector for Ana_3Lep
    class PhotonSelector_3Lep : public PhotonSelector
    {
    };

    /// Implement photon selector for Ana_4Lep
    class PhotonSelector_4Lep : public PhotonSelector
    {
    };

    /// Implement photon selector for Ana_2LepWH
    class PhotonSelector_2LepWH : public PhotonSelector
    {
    };

    /// Implement photon selector for Ana_SS3L
    class PhotonSelector_SS3L : public PhotonSelector
    {
    };

    /// Implement photon selector for Ana_Stop2L
    class PhotonSelector_Stop2L : public PhotonSelector
    {
        virtual bool isBaseline(const Photon* ph); 
        virtual bool isSignal(const Photon* ph);
    };

} // namespace Susy

#endif
