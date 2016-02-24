//  -*- c++ -*-
#ifndef SUSY_JETSELECTOR_H
#define SUSY_JETSELECTOR_H

#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/SusyDefs.h" // JetVector

namespace Susy {

class Jet;

///  A class to select jets
/**
   The generic JetSelector implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Jets

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from JetSelector.

   The analysis-specific selector should be instantiated with JetSelector::build().

   Please avoid having thresholds as input parameters of the selection
   functions (including default arguments). Prefer explicit values in
   the function definition, for example:
   \code{.cpp}
   JetSelector_myAnalysis::isSignalJet(const Jet* jet)
   {
       return (jet->Pt>20.0 && fabs(jet->Eta())<2.4);
   }
   \endcode
   This design has the following advantages:

   1. the default code does not have many 'if else' implementing the
   analysis-specific selection

   2. the common code can still be shared across groups

   3. the analysis-specific thresholds are explicitly written in the
   analysis-specific implementation of the overloaded function. This
   allows to almost copy-and-paste the recommendation from the
   analysis wiki.

   See test_JetSelector.cxx for an example of how this class can be used

   Note to self:
   design choices tested in https://gist.github.com/gerbaudo/7854402

   Note to self:
   since we usually store Jet* in vectors, all the is___() functions
   take a Jet* as input.

   \todo make the is__() functions const
   \todo convert the const Jet* argument to const Jet& (and skip all the if(jet) checks)

   davide.gerbaudo@gmail.com, Aug 2014
*/
class JetSelector
{
public:
    /// provide analysis-specific selector (or vanilla one if analysis is unknown)
    /**
       The user owns the selector (i.e. should use std::shared_ptr
       or delete it when done with it).
    */
    static JetSelector* build(const AnalysisType &a, bool verbose);

    JetSelector(); ///< Default ctor
    virtual ~JetSelector() {}; ///< dtor (for now we don't have anything to delete)
    JetSelector& setSystematic(const NtSys::SusyNtSys&); ///< set syst (needed for example for jvt)
    /// whether the jet is b-tagged
    virtual bool isB(const Jet* jet);
    virtual bool isCentralLight(const Jet* jet);
    virtual bool isCentralB(const Jet* jet);
    virtual bool isForward(const Jet* jet);
    /// The jet-vertex-fraction requirement: usually applied to low-pt central jets
    static bool passJvt(const Jet* jet);

    virtual bool isBaseline(const Jet* jet); ///< often analsysi-dependent
    virtual bool isSignal(const Jet* jet); ///< often analsysi-dependent

    bool verbose() const { return m_verbose; }
    JetSelector& setVerbose(bool v) { m_verbose = v; return *this; }
    /// count central light jets \todo const (depends on jvf tool interface)
    virtual size_t count_CL_jets(const JetVector &jets) /*const*/;
    /// count central b-tagged jets \todo const
    virtual size_t count_CB_jets(const JetVector &jets) /*const*/;
    /// count forward jets \todo const
    virtual size_t count_F_jets(const JetVector &jets) /*const*/;

    //@{
    /**
       b-tag operating points for AntiKt4EMTopoJets with MV2c20.
       See
       https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_AntiKt4EMTopoJets
    */
    static float mv2c20_70efficiency() { return -0.0436; } ///< threshold for 70% efficiency
    static float mv2c20_77efficiency() { return -0.4434; } ///< threshold for 77% efficiency
    static float mv2c20_80efficiency() { return -0.5911; } ///< threshold for 80% efficiency

    /// b-tag operating point used for the overlap removal.
    /**
       See
       https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#b_tagging
     */
    static float overlapRemovalBtagEffWP() { return mv2c20_80efficiency(); }
    //@}
private:
    NtSys::SusyNtSys m_systematic; ///< current syst variation
    bool m_verbose; ///< toggle verbose messages
    }; // class JetSelector


//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements jet selection from ATL-COM-PHYS-2013-911
class JetSelector_2Lep : public JetSelector
{
};
/// implements jet selection from ATL-COM-PHYS-2013-888
class JetSelector_3Lep : public JetSelector
{
};

/// implements jet selection from ATL-COM-PHYS-2014-221
class JetSelector_2LepWH : public JetSelector
{
};

/// implements jet selection from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class JetSelector_SS3L : public JetSelector
{
    virtual bool isBaseline(const Jet* jet);
    virtual bool isSignal(const Jet* jet);
    virtual bool isB(const Jet* jet);
public:
    virtual bool isB_for_OR(const Jet* jet);
};

/// implements jet selection from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DirectStop2Lepton
class JetSelector_Stop2L : public JetSelector
{
    virtual bool isBaseline(const Jet* jet);
    virtual bool isSignal(const Jet* jet);
};

} // Susy

#endif
