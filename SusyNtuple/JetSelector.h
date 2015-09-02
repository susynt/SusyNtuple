//  -*- c++ -*-
#ifndef SUSY_JETSELECTOR_H
#define SUSY_JETSELECTOR_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/AnalysisType.h"

namespace Susy {

class Jet;

///  A class to select jets
/**
   The generic JetSelector implements the generic definitions from
   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Jets

   Analysis-dependent criteria should be implemented in your
   analysis-specific class inheriting from JetSelector.

   The analysis-specific selector should be generated with JetSelector::build().

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
    /**
       Default values (AntiKt4EMTopoJets) for MV2c20 from
       https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_AntiKt4EMTopoJets
    */
    virtual bool isBJet(const Jet* jet);
    virtual bool isCentralLightJet(const Jet* jet);
    virtual bool isCentralBJet(const Jet* jet);
    virtual bool isForwardJet(const Jet* jet);
    /// The jet-vertex-fraction requirement: usually applied to low-pt central jets
    virtual bool jetPassesJvtRequirement(const Jet* jet); // TODO rename to passJvt

    virtual bool isBaselineJet(const Jet* jet); ///< often analsysi-dependent
    virtual bool isSignalJet(const Jet* jet); ///< often analsysi-dependent

    bool verbose() const { return m_verbose; }
    JetSelector& setVerbose(bool v) { m_verbose = v; return *this; }

    /// count central light jets \todo const (depends on jvf tool interface)
    virtual size_t count_CL_jets(const JetVector &jets) /*const*/;
    /// count central b-tagged jets \todo const
    virtual size_t count_CB_jets(const JetVector &jets) /*const*/;
    /// count forward jets \todo const
    virtual size_t count_F_jets(const JetVector &jets) /*const*/;

    /// For the overlap removal: a different b-tag threshold is used
    /**
       80% eff see
       https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#b_tagging
     */
    static float overlapRemovalBtagEffWP() { return -0.5911; }
private:
    NtSys::SusyNtSys m_systematic; ///< current syst variation
    bool m_verbose; ///< toggle verbose messages
    }; // class JetSelector


//----------------------------------------------------------
//
// End generic selector, begin analysis-specific ones
//
//----------------------------------------------------------

/// implements ATL-COM-PHYS-2013-911
class JetSelector_2Lep : public JetSelector
{
};
/// implements ATL-COM-PHYS-2013-888
class JetSelector_3Lep : public JetSelector
{
};

/// implements ATL-COM-PHYS-2014-221
class JetSelector_2LepWH : public JetSelector
{
};

/// implements https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
class JetSelector_ss3l : public JetSelector
{
    virtual bool isBaselineJet(const Jet* jet);
    virtual bool isSignalJet(const Jet* jet);
    virtual bool isBJet(const Jet* jet);
    virtual bool passJvt(const Jet* jet);
};

// TODO Danny add ref wiki
class JetSelector_Stop2L : public JetSelector
{
    virtual bool isBaselineJet(const Jet* jet);
    virtual bool isSignalJet(const Jet* jet);
};

} // Susy

#endif
