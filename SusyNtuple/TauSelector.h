// -*- c++ -*-
#ifndef SUSYNTUPLE_TAUSELECTOR_H
#define SUSYNTUPLE_TAUSELECTOR_H

#include "SusyNtuple/AnalysisType.h"

namespace Susy {

class Tau;

/// A class to select tau
/**
   The generic TauSelector implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Taus

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from TauSelector.

   The analysis-specific selector should be instantiated with TauSelector::build().

   For details on the design and implementation of this class, see the
   documentation for JetSelector.

   davide.gerbaudo@gmail.com, Sep 2015
*/
class TauSelector
{
public:
    /// provide analysis-specific selector (or vanilla one if analysis is unknown)
    /**
       The user owns the selector (i.e. should use std::shared_ptr
       or delete it when done with it).
    */
    static TauSelector* build(const AnalysisType &a, bool verbose);
    TauSelector(); ///< Default ctor
    virtual ~TauSelector() {}; ///< dtor (for now we don't have anything to delete)

    /// whether tau passes the signal criteria
    /**
       Usually pt+bdt
    */
    virtual bool isBaseline(const Tau& tau);
    /// wraps above
    bool isBaseline(const Tau* tau) { return isBaseline(*tau); }
    /// whether tau passes the signal criteria
    virtual bool isSignal(const Tau& tau);
    /// wraps above
    virtual bool isSignal(const Tau* tau) { return isSignal(*tau); }

protected :
    /// whether it should be verbose
    bool m_verbose;

}; // TauSelector

//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements tau selection for ATL-COM-PHYS-2013-911
// Ana_2Lep
class TauSelector_2Lep : public TauSelector
{
};

/// implements tau selection for ATL-COM-PHYS-2013-888
// Ana_3Lep
class TauSelector_3Lep : public TauSelector
{
};

/// 4Leptons search
class TauSelector_4Lep : public TauSelector
{
	virtual bool isBaseline(const Tau& tau);
    virtual bool isSignal(const Tau& tau);
};

/// implements tau selection for ATL-COM-PHYS-2014-221
// Ana_2LepWH
class TauSelector_2LepWH : public TauSelector
{
};

/// implements tau selection from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
// Ana_SS3L
class TauSelector_SS3L : public TauSelector
{
};

/// implements tau selection from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
// Ana_Stop2L
class TauSelector_Stop2L : public TauSelector
{
};

} // namespace Susy
#endif
