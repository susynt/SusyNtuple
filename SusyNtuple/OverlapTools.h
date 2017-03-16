//  -*- c++ -*-
#ifndef SUSYNTUPLE_OVERLAPTOOLS_H
#define SUSYNTUPLE_OVERLAPTOOLS_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Isolation.h"

namespace Susy { class JetSelector; }
namespace Susy {

/// A class to  perform the overlap removal
/**
    The generic OverlapTools implements the generic definitions from
    AssociationUtils [1]. The SUSYTools configuration is default as
    specified in the SUSY Background Forum meeting on January 26, 2016 [2].

    Analysis-dependent criteria should be implemented in your
    analysis-specific class inheriting from Overlap_Removals.

    The analysis-specific tool should be instantiated with Overlap_Removals::build().

    For details on the design and implementation of this class, see the
    documentation for JetSelector.

    daniel.joseph.antrim@cern.ch, February 24, 2016
    davide.gerbaudo@gmail.com

    [1] https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils
    [2] https://indico.cern.ch/event/490240/attachments/1225853/1794468/ORsummary.pdf
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
                                JetVector& jets, TauVector& taus, PhotonVector& photons);

    /**
        the separate steps of OR removal implemented within "performOverlap(...)"
    */
    virtual void j_e_overlap(ElectronVector& electrons, JetVector& jets,
                                         double dR = 0.2, bool doBJetOR = true); ///< remove jet from electron
    virtual void e_j_overlap(ElectronVector& electrons, JetVector& jets,
                                        double dR = 0.4, bool doSlidingCone = false, bool applyJVT = true); ///< remove electron from jet
    virtual void j_m_overlap(JetVector& jets, MuonVector& muons,
                                         double dR = 0.2, bool doBJetOR = true, bool doGhost = true);
    virtual void m_j_overlap(MuonVector& muons, JetVector& jets,
                                        double dR = 0.4, bool doSlidingCone = false, bool applyJVT = true); ///< remove muon from jet
    virtual void e_m_overlap(ElectronVector& electrons, MuonVector& muons); ///< remove electron from muon (shared-track)
    virtual void e_m_overlap(ElectronVector& electrons, MuonVector& muons, float dR); ///< remove electron from muon (dR-based)
    virtual void m_e_overlap(MuonVector& muons, ElectronVector& electrons); ///< remove calo muons overlapping with electrons
    virtual void t_e_overlap(TauVector& taus, ElectronVector& electrons, double dR); ///< remove tau from electron
    virtual void t_m_overlap(TauVector& taus, MuonVector& muons, double dR); ///< remove tau from muon
    virtual void j_t_overlap(TauVector& taus, JetVector& jets, double dR); ///< remove jet from tau

    virtual void p_e_overlap(PhotonVector& photons, ElectronVector& electrons, double dR); ///< remove photons overlapping with electrons
    virtual void p_m_overlap(PhotonVector& photons, MuonVector& muons, double dR); ///< remove photons overlapping with muons
    virtual void j_p_overlap(JetVector& jets, PhotonVector& photons, double dR = 0.4); ///< remove jet overlapping with photon

    virtual void e_e_overlap(ElectronVector& electrons, double dR); ///< remove electron from electron
    virtual void m_m_overlap(MuonVector& muons, double dR); ///< remove muon from muon

    // helper methods

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
    /// jet selector needed for j_X OR
    JetSelector* jetSelector() const { return m_jetSelector; }
    OverlapTools& jetSelector(JetSelector* js) { m_jetSelector = js; return *this; }
    /// used within removeNonisolatedLeptons()
    bool leptonPassesIsolation(const Lepton* lep, const Isolation &iso);

    /// check if muon ID tracks are ghost associated with the input jet
    bool muonIsGhostMatched(const Muon* mu, const Jet* jet);

    /// check if the electron and muon have shared ID tracks
    bool eleMuSharedTrack(const Electron* el, const Muon* mu);

    /// calculate the cone size for a sliding cone size based on object pT
    float getSlidingDRCone(float pT, float c1 = 0.04, float c2 = 10. /*GeV*/, float maxConeSize = 0.4);

    /// check if a jet satisfies the b-jet criteria for OR
    bool isBJetOR(const Jet* jet);


    OverlapTools& setVerbose(bool v) { m_verbose = v; return *this; }
    OverlapTools& useOldOverlap(bool v) { m_useOldOverlap = v; return *this; }
    bool verbose() const { return m_verbose; }

protected :
    Isolation m_electronIsolation;
    Isolation m_muonIsolation;
    JetSelector *m_jetSelector;
    bool m_verbose;
    bool m_useOldOverlap;
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

/// implements OR procedure for 4L
class OverlapTools_4Lep : public OverlapTools
{
public:
  virtual void performOverlap(ElectronVector& electrons, MuonVector& muons, 
			      JetVector& jets, TauVector& taus, PhotonVector& photons);
  virtual void performOverlap(ElectronVector& electrons, MuonVector& muons, 
			      JetVector& jets, TauVector& taus, TauVector& LOOSEtaus, PhotonVector& photons, bool m_TauCtrlReg);
protected:

};


/// implements OR procedure from ATL-COM-PHYS-2014-221
class OverlapTools_2LepWH : public OverlapTools
{
};

/// implements OR procedure from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class OverlapTools_SS3L: public OverlapTools {
public:
    OverlapTools_SS3L(); ///< Default ctor
    virtual JetSelector* jetSelector() const { return m_jetSelector; }
    virtual OverlapTools_SS3L& jetSelector(JetSelector* js) { m_jetSelector = js; return *this; }
    /// different dR values
    virtual void performOverlap(ElectronVector& electrons, MuonVector& muons,
                                JetVector& jets, TauVector& taus);
    /// different from OverlapTools::j_e_overlap() : performs BjetOR
    virtual void j_e_overlap(ElectronVector& electrons, JetVector& jets, double dR);
protected:
    JetSelector *m_jetSelector; ///< needed for btag in j-e OR
};

/// implements OR procedure from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
class OverlapTools_Stop2L : public OverlapTools
{
};


}







#endif
