//  -*- c++ -*-
#ifndef SUSYNTUPLE_OVERLAPTOOLS_H
#define SUSYNTUPLE_OVERLAPTOOLS_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"

namespace Susy {

/// A class to  perform the overlap removal
/**
   The generic OverlapTools implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Overlap_Removals

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from Overlap_Removals.

   The analysis-specific tool should be instantiated with Overlap_Removals::build().

   For details on the design and implementation of this class, see the
   documentation for JetSelector.

   daniel.joseph.antrim@cern.ch
   davide.gerbaudo@gmail.com, Sep 2015
*/
class OverlapTools
{
public :
    /// provide analysis-specific OR tool (or vanilla one if analysis is unknown)
    /**
       The user owns the selector (i.e. should use std::shared_ptr
       or delete it when done with it).
    */
    static OverlapTools* build(const AnalysisType &a, bool verbose);
    OverlapTools(); ///< Default ctor
    virtual ~OverlapTools(){};  ///< dtor (for now we don't have anything to delete)
    // main overlap removal function, performs all removals
    /**
       includes several of the member functions below.
    */
    virtual void performOverlap(ElectronVector& electrons, MuonVector& muons,
                                TauVector& taus, JetVector& jets);
    virtual void j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR); ///< remove jet from electron
    virtual void e_j_overlap(ElectronVector& electrons, JetVector& jets, double dR); ///< remove electron from jet
    virtual void m_j_overlap(MuonVector& muons, JetVector& jets, double dR); ///< remove muon from jet
    virtual void e_m_overlap(ElectronVector& electrons, MuonVector& muons, double dR); ///< remove electron from muon
    virtual void e_e_overlap(ElectronVector& electrons, double dR); ///< remove electron from electron
    virtual void m_m_overlap(MuonVector& muons, double dR); ///< remove muon from muon
    virtual void t_e_overlap(TauVector& taus, ElectronVector& electrons, double dR); ///< remove tau from electron
    virtual void t_m_overlap(TauVector& taus, MuonVector& muons, double dR); ///< remove tau from muon
    virtual void j_t_overlap(TauVector& taus, JetVector& jets, double dR); ///< remove jet from tau
    /// remove non-isolated leptons from the input collections
    /**
       Some analyses might want to perform the overlap removal using
       isolated leptons only. In this case, they need to call this
       function within performOverlap() before calling all the
       a_b_overlap() functions. They also need to call
       setElectronIsolation() and setMuonIsolation() beforehand, with
       the values that can be retrieved from the ElectronSelector and
       MuonSelector.
     */
    virtual void removeNonisolatedLeptons(ElectronVector& electrons, MuonVector& muons);
    /// electron isolation used in leptonPassesIsolation()
    OverlapTools& setElectronIsolation( Isolation eleIso ) { m_electronIsolation = eleIso; return *this; }
    /// muon isolation used in leptonPassesIsolation()
    OverlapTools& setMuonIsolation( Isolation muIso ) { m_muonIsolation = muIso; return *this; }
    /// used within removeNonisolatedLeptons()
    bool leptonPassesIsolation(const Lepton* lep, const Isolation &iso);

protected :
    Isolation m_electronIsolation;
    Isolation m_muonIsolation;
    bool m_verbose;
}; // class OverlapTools

//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements OR procedure from ATL-COM-PHYS-2013-911
class OverlapTools_2Lep : public OverlapTools
{
};
/// implements OR procedure from ATL-COM-PHYS-2013-888
class OverlapTools_3Lep : public OverlapTools
{
};

/// implements OR procedure from ATL-COM-PHYS-2014-221
class OverlapTools_2LepWH : public OverlapTools
{
};

/// implements OR procedure from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class OverlapTools_SS3L: public OverlapTools {
    virtual void performOverlap(ElectronVector& electrons, MuonVector& muons,
                                TauVector& taus, JetVector& jets);
    /// different from OverlapTools::j_e_overlap() : performs BjetOR
    virtual void j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR);
};

/// implements OR procedure from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
class OverlapTools_Stop2L : public OverlapTools
{
};



}







#endif
