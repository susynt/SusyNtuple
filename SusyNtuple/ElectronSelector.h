// -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRONSELECTOR_H
#define SUSYNTUPLE_ELECTRONSELECTOR_H

#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/ElectronId.h"
#include "SusyNtuple/Isolation.h"


namespace Susy {
class Electron;

/// A class to select electrons
/**
   The generic ElectronSelector implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Electrons

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from ElectronSelector.

   The analysis-specific selector should be instantiated with ElectronsSelector::build().

   For details on the design and implementation of this class, see the
   documentation for JetSelector.

   davide.gerbaudo@gmail.com, Sep 2015
*/
class ElectronSelector
{
public:
    static ElectronSelector* build(const AnalysisType &a, bool verbose);
    ElectronSelector(); ///< Default ctor
    virtual ~ElectronSelector() {}; ///< dtor (for now we don't have anything to delete)

    ElectronSelector& setAnalysis(const AnalysisType& analysis);

    /// whether el passes the baseline criteria
    /**
       Usually ID + pt + eta + d0sig
     */
    virtual bool isBaseline(const Electron* el);
    /// whether el passes the signal criteria
    /**
       Usually baseline + impact parameter + isolation.
    */
    virtual bool isSignal(const Electron* el);

    virtual bool passIpCut(const Electron &el);
    virtual bool outsideCrackRegion(const Electron &el);

    /// id of signal electron, used to determine err SF
    ElectronId signalId() const { return m_signalId; }
    /// set signal isolation
    /**
       Note: the value you set here should match whatever you have in
       _your_ (overriding) implementation of isSignalElectron()
     */
    ElectronSelector& setSignalId(const ElectronId &v) { m_signalId = v; return *this; }
    /// Retrieve the isolation requirement for signal electrons.
    /**
       This is helpful for other tools that need to know how the
       electrons are defined.
    */
    Isolation signalIsolation() const { return m_signalIsolation; }
    /// set signal isolation
    /**
       Note: the value you set here should match whatever you have in
       _your_ (overriding) implementation of isSignalElectron()
     */
    ElectronSelector& setSignalIsolation(const Isolation &v) { m_signalIsolation = v; return *this; }

    /**
       Get the input electron "ele"'s nominal efficicncy SF
    */
    float effSF(const Electron& ele);
    /// wraps above
    float effSF(const Electron* ele) { return effSF(*ele); }

    ///  Error on the efficiency for the requested systematic
    float errEffSF(const Electron& ele, const NtSys::SusyNtSys sys);
    /// wraps above
    float errEffSF(const Electron* ele, const NtSys::SusyNtSys sys) { return errEffSF(*ele, sys); }

protected :
    ElectronId m_signalId;       ///< electron quality requirement (selected from eleID enum)
    Isolation m_signalIsolation; ///< electron isolation qualiy for signal electrons (c.f. SusyNtuple/Isolation.h)
    bool m_verbose;

}; // end ElectronSelector

//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements electron selection for ATL-COM-PHYS-2013-911
class ElectronSelector_2Lep : public ElectronSelector
{
};

/// implements electron selection for ATL-COM-PHYS-2013-888
class ElectronSelector_3Lep : public ElectronSelector
{
};

/// implements electron selection for 4L
class ElectronSelector_4Lep : public ElectronSelector
{
    virtual bool isSignal(const Electron* el);
};

/// implements electron selection for ATL-COM-PHYS-2014-221
class ElectronSelector_2LepWH : public ElectronSelector
{
    virtual bool passIpCut(const Electron &el);
};

/// implements electron selection from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class ElectronSelector_SS3L : public ElectronSelector
{
    virtual bool isBaseline(const Electron* el);
    virtual bool isSignal(const Electron* el);
};

/// implements electron selection from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
class ElectronSelector_Stop2L : public ElectronSelector
{
    virtual bool passIpCut(const Electron  &el);
    virtual bool isSignal(const Electron* el);
};

} // Susy

#endif


