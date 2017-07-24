// -*- c++ -*-
#ifndef SUSYNTUPLE_MUONSELECTOR_H
#define SUSYNTUPLE_MUONSELECTOR_H

#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"
#include "SusyNtuple/MuonId.h"


namespace Susy {

class Muon;

/// A class to select muons
/**
   The generic MuonSelector implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Muons

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from MuonSelector.

   The analysis-specific selector should be instantiated with MuonsSelector::build().

   For details on the design and implementation of this class, see the
   documentation for JetSelector.

   davide.gerbaudo@gmail.com, Sep 2015
*/

class MuonSelector
{
public :
    /// provide analysis-specific selector (or vanilla one if analysis is unknown)
    /**
       The user owns the selector (i.e. should use std::shared_ptr
       or delete it when done with it).
    */
    static MuonSelector* build(const AnalysisType &a, bool verbose);
    MuonSelector();  ///< Default ctor
    virtual ~MuonSelector() {}; ///< dtor (for now we don't have anything to delete)
    virtual bool isBaseline(const Muon* mu); ///< whether mu passes the baseline criteria
    /// whether mu passes the signal criteria
    /**
       Usually baseline + impact parameter + isolation.
    */
    virtual bool isSignal(const Muon* mu);
    /// whether mu is close enough to the primary vertex
    virtual bool passIpCut(const Muon* mu);
    /// nominal efficiency scale factor of mu
    virtual float effSF(const Muon& mu, const NtSys::SusyNtSys sys);
    /// wraps effSF() above
    virtual float effSF(const Muon* mu, const NtSys::SusyNtSys sys = NtSys::NOM) { return effSF(*mu, sys); }
    /// systematic error on the efficiency SF of mu
    virtual float errEffSF(const Muon& mu, const NtSys::SusyNtSys sys);
    /// wraps errEffSF() above
    virtual float errEffSF(const Muon* mu,
                           const NtSys::SusyNtSys sys)
        { return errEffSF(*mu, sys); }
    /// isolation required for signal muon
    Isolation signalIsolation() const { return m_signalIsolation; }
    /// set signal isolation
    /**
       Note: the value you set here should match whatever you have in
       _your_ (overriding) implementation of isSignal()
     */
    MuonSelector& setSignalIsolation(const Isolation &v) { m_signalIsolation = v; return *this; }
    /// id of signal muon, used to determine err SF
    MuonId signalId() const { return m_signalId; }
    /**
       Note: the value you set here should match whatever you have in
       _your_ (overriding) implementation of isSignal()
     */
    MuonSelector& setSignalId(const MuonId &v) { m_signalId = v; return *this; }
    MuonSelector& setVerbose(const bool &v) { m_verbose = v; return *this; }
    bool verbose() const { return m_verbose; }
protected :
    MuonId m_signalId; ///< required quality of the signal muon
    Isolation m_signalIsolation; ///< required to propagate to OverlapTools
    bool m_verbose;

}; // end MuonSelector

//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements muon selection for ATL-COM-PHYS-2013-911
class MuonSelector_2Lep : public MuonSelector
{
    bool isBaseline(const Muon* mu);
    virtual bool isSignal(const Muon* mu);
};

/// implements muon selection for ATL-COM-PHYS-2013-888
class MuonSelector_3Lep : public MuonSelector
{
    virtual bool isSignal(const Muon* mu);
};

/// implements muon selection for 4L
class MuonSelector_4Lep : public MuonSelector
{
    bool isBaseline(const Muon* mu);
};

/// implements muon selection for ATL-COM-PHYS-2014-221
class MuonSelector_2LepWH : public MuonSelector
{
    virtual bool isSignal(const Muon* mu);
};

/// implements https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class MuonSelector_SS3L : public MuonSelector
{
    virtual bool isBaseline(const Muon* mu);
    virtual bool isSignal(const Muon* mu);
};

/// implements Muon selection from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
class MuonSelector_Stop2L : public MuonSelector
{
    virtual bool isBaseline(const Muon* mu);
    virtual bool isSignal(const Muon* mu);
};

} // end namespace

#endif

