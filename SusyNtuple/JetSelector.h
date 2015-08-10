//  -*- c++ -*-
#ifndef SUSY_JETSELECTOR_H
#define SUSY_JETSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"

#include "JVFUncertaintyTool/JVFUncertaintyTool.h"

namespace Susy {
///  A class to select jets
/**
   Each analysis has slightly different jet requirements. This class
   provides a centralized place where such requirements are defined.

   See test_JetSelector.cxx for an example of how this class can be used

   Note to self:
   design choices tested in https://gist.github.com/gerbaudo/7854402

   davide.gerbaudo@gmail.com, Aug 2014
*/

class Jet;

class JetSelector
{
public:
    /// Vanilla selector
    JetSelector();
    /// specify the current syst
    JetSelector& setSystematic(const NtSys::SusyNtSys&);
    /// specify the jet requirements for a specific analysis
    /**
       This is where all the thresholds are set.
     */
    JetSelector& setAnalysis(const AnalysisType&);
    /**
        Given the AnalysisType set in "setAnalysis", set the kinematic
        requirements and default cut-values.
    */
    void buildRequirements(const AnalysisType& ana);

    /// if you don't want to use the default tool
    /**
       Used for example when you are dealing with non-standard jet
       collections, see JVFUncertaintyTool default constructor.
     */
//    JetSelector& setJvfTool(const JVFUncertaintyTool &t);
//    static float defaultMinPt() { return JET_PT_CUT; }
//    static float defaultCentralEtaMax() { return 0.0; }; ///< \todo
//    static float defaultBtagMinValue() { return 0.0; }; ///< \todo
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
//    bool isSignalJet2Lep(const Jet* jet);
    bool isCentralLightJet(const Jet* jet);
    bool isCentralBJet(const Jet* jet);
    bool isForwardJet(const Jet* jet);
//    bool isBadFCALJet(const Jet* jet);
//    bool hasJetInBadFCAL(const JetVector& baseJets, uint run, bool isMC);

    bool selects(const Jet* j) const { return false; } ///< placeholder
    bool verbose() const { return m_verbose; }
    JetSelector& setVerbose(bool v) { m_verbose = v; return *this; }

    // The jet-vertex-fraction requirement: usually applied to low-pt central jets
//    bool jetPassesJvfRequirement(const Jet* jet);
    bool jetPassesJvtRequirement(const Jet* jet);

    /// count central light jets \todo const (depends on jvf tool interface)
    size_t count_CL_jets(const JetVector &jets) /*const*/;
    /// count central b-tagged jets \todo const
    size_t count_CB_jets(const JetVector &jets) /*const*/;
    /// count forward jets \todo const
    size_t count_F_jets(const JetVector &jets) /*const*/;

//    float m_min_pt; ///< do not consider jets with pt below this value
//    float m_max_eta; ///< do not consider jets with eta above this value
//    float m_min_jvf; ///< below this jvf value a jet is not JVF-confirmed
//    float m_max_jvf_eta; ///< above this eta we cannot apply the JVF requirement

    float JET_PT_CUT;               ///< basline jet pT cut (do not consider jets with pT below this value)
    float JET_SIGNAL_PT_CUT;        ///< signal jet pT cut
    float JET_PT_L25_CUT;           ///< signal CL25 jet pT cut (name is redundant?)
    float JET_PT_L20_CUT;           ///< signal CL20 jet pT cut (name is redundant?)
    float JET_PT_B20_CUT;           ///< signal CL b-jet pT cut
    float JET_PT_F30_CUT;           ///< signal forward-jet pT cut
    float JET_ETA_CUT;              ///< signal jet maximum allowed eta
    float JET_ETA_MAX_CUT;          ///< maximum allowed eta for forward jets (eta cuts should be renamed)
    float JET_JVT_CUT;              ///< JVT threshold value
    float JET_JVT_ETA_CUT;          ///< eta threshold for applying JVT cut (eta must be below this to apply JVT)
    float JET_JVT_PT_CUT;           ///< pT threshold for applying JVT cut (pT must be below this to apply JVT)
    


//    static constexpr float JET_PT_CUT;
//    static constexpr float JET_SIGNAL_PT_CUT_3L;
//    static constexpr float JET_SIGNAL_PT_CUT_2L;
//    static constexpr float JET_SIGNAL_PT_CUT_SS3L;
//    static constexpr float JET_ETA_CUT;
//    static constexpr float JET_JVT_CUT;
//    static constexpr float JET_JVT_ETA_CUT;
//    static constexpr float JET_JVT_PT_CUT ;
//    static constexpr float JET_JVF_CUT;
//    static constexpr float JET_JVF_CUT_2L;
//    static constexpr float JET_JVF_PT;
//    static constexpr float JET_JVF_ETA;
//    static constexpr float JET_PT_L25_CUT;
//    static constexpr float JET_PT_L20_CUT;
//    static constexpr float JET_PT_B20_CUT;
//    static constexpr float JET_PT_F30_CUT;
//    static constexpr float JET_ETA_MAX_CUT;
//    static constexpr float JET_ETA_CUT_2L;
//    static constexpr float JET_ETA_CUT_SS3L;

private:
    JVFUncertaintyTool m_jvftool; ///< jvf tool (note weird design --> weird const-ness)
    NtSys::SusyNtSys m_systematic; ///< current syst variation
    AnalysisType m_analysis; ///< analysis

    //////////////////////////////
    // Available analyses
    //////////////////////////////
    bool m_2lep;    ///< flag for 2L analysis
    bool m_3lep;    ///< flag for 3L analysis
    bool m_2lepWH;  ///< flag for 2L WH analysis
    bool m_SS3L;    ///< flag for the strong SS3L analysis

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
