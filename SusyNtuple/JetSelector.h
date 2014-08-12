//  -*- c++ -*-
#ifndef SUSY_JETSELECTOR_H
#define SUSY_JETSELECTOR_H

#include "SusyNtuple/SusyDefs.h"

class JVFUncertaintyTool;

namespace Susy {
///  A class to select jets
/**
   Each analysis has slightly different jet requirements.  This class
   tries to provide a centralized place where such requirements are
   defined.

   The user should call build_*analysis_selector (rather than using
   the default constructor).

   See test_JetSelector.cxx for an example of how this class can be used

   Note to self:
   design choices tested in https://gist.github.com/gerbaudo/7854402

   davide.gerbaudo@gmail.com, Aug 2014
*/

class Jet;

class JetSelector
{
public:
    /// dummy c'tor; prefer analysis-specific build_*_selector methods
    JetSelector(JVFUncertaintyTool* const jvftool, const SusyNtSys& systematic, const AnalysisType& analysis);
    /// jet selection used for the two-lepton study
    static JetSelector build_2L_analysis_selector(JVFUncertaintyTool* const, const SusyNtSys&, const AnalysisType&);
    /// jet selection used for the three-lepton study
    static JetSelector build_3L_analysis_selector(JVFUncertaintyTool* const, const SusyNtSys&, const AnalysisType&);
    /// jet selection used for the WH study
    static JetSelector build_WH_analysis_selector(JVFUncertaintyTool* const, const SusyNtSys&, const AnalysisType&);
    /// build a jvf tool; the user is responsible for its deletion
    static JVFUncertaintyTool* build_jvf_tool();
/*
  \todo: these will be used internally when refactoring

    bool isCentral(const Jet &j) { return fabs(j.detEta)<m_max_eta; }
    bool isCentralLight(const Jet &j) const { return true; }
    bool isCentralB(const Jet &j) const { return true; }
    bool isJvfConfirmed(const Jet &j) const { return true; }
    float minPt() const { return m_min_pt; }
    float maxEta() const { return m_max_eta; }
    JetSelector& requireMinPt(float v) { m_min_pt = v; return *this; }
    JetSelector& requireMaxEta(float v) { m_max_eta = v; return *this; }
*/
    // todo:
    // - make them const
    bool isSignalJet(const Jet* jet);
    bool isSignalJet2Lep(const Jet* jet);
    bool isCentralLightJet(const Jet* jet);
    static bool isCentralBJet(const Jet* jet);
    static bool isForwardJet(const Jet* jet);
    bool isBadFCALJet(const Jet* jet);
    bool hasJetInBadFCAL(const JetVector& baseJets, uint run, bool isMC);

    bool selects(const Jet* j) const { return false; } ///< placeholder
    bool verbose() const { return m_verbose; }
    JetSelector& setVerbose(bool v) { m_verbose = v; return *this; }

    bool jetPassesJvfRequirement(const Jet* jet, float maxPt, float maxEta, float nominalJvtThres);

    /// count central light jets \todo const (depends on jvf tool interface)
    size_t count_CL_jets(const JetVector &jets) /*const*/;
    /// count central b-tagged jets \todo const
    static size_t count_CB_jets(const JetVector &jets) /*const*/;
    /// count forward jets \todo const
    static size_t count_F_jets(const JetVector &jets) /*const*/;

private:
    /*
  \todo: these will be used internally when refactoring
    bool m_skip_pt;
    bool m_skip_eta;
    bool m_skip_jvf;
    float m_min_pt; ///< do not consider jets with pt below this value
    float m_max_eta; ///< do not consider jets with eta above this value
    float m_max_jvf_eta; ///< above this eta we cannot apply the JVF requirement
    float m_min_jvf; ///< below this value a jet is not JVF-confirmed
    */
    JVFUncertaintyTool* const m_jvftool; ///< jvf tool (note weird design --> weird const-ness)
    SusyNtSys m_systematic; ///< current syst variation
    AnalysisType m_analysis; ///< analysis
    bool m_verbose; ///< toggle verbose messages
};
//----------------------------------------------------------
/// wrapper functor to be used with std algorithms
/**
   Note to self: in order for this to be a well-behaved functor,
   I can only provide one operator()(const Lepton *l), and not
           (const Lepton *l) + (const Lepton &l).
   Otherwise I won't be able to use the std function adapters (namely
   not1). See Item 40 of S.Meyer's "Effective STL".
*/
class JetSelectorFunctor : public std::unary_function<const Jet*, bool>
{
public:
    JetSelectorFunctor(const JetSelector &js): m_selector(js) {};
    bool operator() (const Jet *j) const { return m_selector.selects(j); }
private:
    const JetSelector &m_selector;
};
//----------------------------------------------------------

} // Susy

#endif
