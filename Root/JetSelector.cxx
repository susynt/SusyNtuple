#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/SusyNt.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits> // underlying_type

using Susy::JetSelector;
using Susy::Jet;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {
// ---------------------------------------------------------------------------- //
//   Constructor
// ---------------------------------------------------------------------------- //
JetSelector::JetSelector() :
    // benchmark MV2c20 tagger cuts: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_AntiKt4EMTopoJets

    ///////////////////////////////
    // default selection
    ///////////////////////////////
    JET_MIN_PT_BASELINE(20.0), // GeV
    JET_MIN_PT_SIGNAL(30.0), // GeV
    JET_MIN_PT_L25(25.0), // GeV
    JET_MIN_PT_L20(20.0), // GeV
    JET_MIN_PT_B20(20.0), // GeV
    JET_MIN_PT_F30(30.0), // GeV
    JET_MAX_ETA(2.4),
    JET_MAX_ETA_FORWARD(4.5),
    JET_JVT_CUT(0.64),
    JET_JVT_ETA_CUT(2.4),
    JET_JVT_PT_CUT(50.0), // GeV
    JET_MV2C20_85(-0.7887),
    JET_MV2C20_80(-0.5911),
    JET_MV2C20_77(-0.4434),
    JET_MV2C20_70(-0.0436),
    JET_MV2C20_60(0.4496),
    JET_MV2C20_OR(JET_MV2C20_80),
    ///////////////////////////////
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_SS3L(false),
    m_stop2l(false),
    m_verbose(false)
{
}
//----------------------------------------------------------
JetSelector& JetSelector::setAnalysis(const AnalysisType &a)
{

    ////////////////////////////////////
    // Set analysis-specific cuts
    ////////////////////////////////////
    
    ////////////////////////////////////
    // 2L-ANALYSIS
    ////////////////////////////////////
    if( a == AnalysisType::Ana_2Lep ) {
        m_2lep = true;

        // use defaults
        JET_MIN_PT_SIGNAL = 20.;
    }
    ////////////////////////////////////
    // 3L-ANALYSIS
    ////////////////////////////////////
    else if( a == AnalysisType::Ana_3Lep ) {
        m_3lep = true;

        // pt
        JET_MIN_PT_SIGNAL   = 20.;
    }
    ////////////////////////////////////
    // WH-ANALYSIS
    ////////////////////////////////////
    else if( a == AnalysisType::Ana_2LepWH ) {
        m_2lepWH = true;

        // use defaults
    }
    ////////////////////////////////////
    // SS3L-ANALYSIS
    // values from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSameSignLeptonsJetsRun2
    ////////////////////////////////////
    else if( a == AnalysisType::Ana_SS3L ) {
        m_SS3L = true;
        JET_MIN_PT_BASELINE = 20.0;
        JET_MIN_PT_SIGNAL   = 20.0;
        JET_MAX_ETA         = 2.8;
        // jvt
        JET_JVT_CUT         = 0.64;
        JET_JVT_ETA_CUT     = 2.4;
        JET_JVT_PT_CUT      = 50.0;
        // bjet
        JET_MIN_PT_B20      = 20.0;
        JET_MV2C20_80       = -0.5911;
        JET_MV2C20_70       = -0.0436;
        JET_MV2C20_OR       = JET_MV2C20_80;
    }
    ////////////////////////////////////
    // Stop2L-ANALYSIS
    ////////////////////////////////////
    else if( a == AnalysisType::Ana_Stop2L ) {
        m_stop2l = true;

        //pt
        JET_MIN_PT_SIGNAL = 20.;

        //eta
        JET_MAX_ETA = 2.8;
    }
    ////////////////////////////////////
    // Didn't set AnalysisType
    ////////////////////////////////////
    else if( a == AnalysisType::kUnknown ) {
        string error = "JetSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for JetSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for JetSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    ////////////////////////////////////
    // Gone fishin'
    ////////////////////////////////////
    else {
        string error = "JetSelector::setAnalysis error: ";
        cout << error << "JetSelector is not configured for the AnalysisType (" << AnalysisType2str(a) << ") provided in" << endl;
        cout << error << "'setAnalysis'! Be sure that your AnalysisType is provided for in SusyNtuple/AnalysisType.h" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    m_analysis = a;

    if(m_verbose)
        cout << "JetSelector    Configured jet selection for AnalysisType " << AnalysisType2str(m_analysis) << endl;

    return *this;
}

//----------------------------------------------------------
JetSelector& JetSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
//----------------------------------------------------------
bool JetSelector::isBaselineJet(const Jet* jet)
{
    if(m_2lep || m_2lepWH || m_3lep) {
        return (jet->Pt() > JET_MIN_PT_BASELINE);
    } else {
        bool passEta = (fabs(jet->Eta()) < JET_MAX_ETA);
        bool passPt  = (jet->Pt() > JET_MIN_PT_BASELINE);
        return (passEta && passPt);
    }
}
//----------------------------------------------------------
bool JetSelector::isSignalJet(const Jet* jet)
{
    JetSelector::check();

    bool pass = false;
    if(jet) {
        if(m_2lep || m_2lepWH) {
            pass = (isCentralLightJet(jet) || isCentralBJet(jet) || isForwardJet(jet));
        }
        else if(m_3lep || m_SS3L) {
            pass = ((jet->Pt() > JET_MIN_PT_SIGNAL)
                     && (fabs(jet->Eta()) < JET_MAX_ETA)
                     && jetPassesJvtRequirement(jet));
        }
    } // if(jet)

    return pass;
}
//----------------------------------------------------------
bool JetSelector::isBJet(const Jet* jet)
{
    ///////////////////////////////
    // pT requirement
    ///////////////////////////////
    if(jet->Pt() < 20.0)    return false;

    ///////////////////////////////
    // eta requirement
    ///////////////////////////////
    if(fabs(jet->Eta()) > 2.5) return false;

    ///////////////////////////////
    // jvt requirement
    ///////////////////////////////
    if(!jetPassesJvtRequirement(jet)) return false;

    ///////////////////////////////
    // mv2c20 cut
    ///////////////////////////////
    if(jet->mv2c20 < JET_MV2C20_77) return false;

    return true;
}
//----------------------------------------------------------
bool JetSelector::isCentralLightJet(const Jet* jet)
{
    JetSelector::check();

    //////////////////////////////
    // pt requirement
    //////////////////////////////
    if(jet->Pt() < JET_MIN_PT_L20)       return false;

    //////////////////////////////
    // detector eta requirement
    //////////////////////////////
    if(fabs(jet->detEta) > JET_MAX_ETA)  return false;

    //////////////////////////////
    // JVT requirement
    //////////////////////////////
    if(!jetPassesJvtRequirement(jet))    return false;

    //////////////////////////////
    // MV2c20 requirement
    //   (a la SUSYTools)
    //////////////////////////////
    if(jet->bjet)                        return false;

    return true;

}
//----------------------------------------------------------
bool JetSelector::isCentralBJet(const Jet* jet)
{
    JetSelector::check();

    //////////////////////////////
    // pt requirement
    //////////////////////////////
    if(jet->Pt() < JET_MIN_PT_B20)            return false;
 
    //////////////////////////////
    // detector eta requirement
    //////////////////////////////
    if(fabs(jet->detEta) > JET_MAX_ETA)       return false;

    //////////////////////////////
    // JVT requirement
    //////////////////////////////
    if(!jetPassesJvtRequirement(jet))         return false;

    //////////////////////////////
    // MV2c20 requirement
    //   (a la SUSYTools)
    //////////////////////////////
    if(!jet->bjet)                            return false;

    return true;
}
//----------------------------------------------------------
bool JetSelector::isForwardJet(const Jet* jet)
{
    JetSelector::check();

    //////////////////////////////
    // pt requirement
    //////////////////////////////
    if(jet->Pt() < JET_MIN_PT_F30)            return false;

    //////////////////////////////
    // jet in forward direction?
    //////////////////////////////
    if(fabs(jet->detEta) < JET_MAX_ETA)       return false;

    //////////////////////////////
    // jet eta below the max?
    //////////////////////////////
    if(fabs(jet->detEta) > JET_MAX_ETA_FORWARD)   return false;

    return true;
}
//----------------------------------------------------------
bool JetSelector::jetPassesJvtRequirement(const Jet* jet)
{
    JetSelector::check();

    return ((jet->jvt > JET_JVT_CUT)
                || (fabs(jet->detEta) > JET_JVT_ETA_CUT)
                || (jet->Pt() > JET_JVT_PT_CUT));
}
//----------------------------------------------------------
size_t JetSelector::count_CL_jets(const JetVector &jets)
{
    JetSelector::check();

    return std::count_if(jets.begin(), jets.end(),
                         std::bind1st(std::mem_fun(&JetSelector::isCentralLightJet), this));
}
//----------------------------------------------------------
size_t JetSelector::count_CB_jets(const JetVector &jets)
{
    JetSelector::check();

    return std::count_if(jets.begin(), jets.end(), 
                         std::bind1st(std::mem_fun(&JetSelector::isCentralBJet), this));
}
//----------------------------------------------------------
size_t JetSelector::count_F_jets(const JetVector &jets)
{
    JetSelector::check();

    return std::count_if(jets.begin(), jets.end(),
                            std::bind1st(std::mem_fun(&JetSelector::isForwardJet), this));
}
//----------------------------------------------------------
void JetSelector::check()
{
    if(m_analysis == AnalysisType::kUnknown) {
        string error = "JetSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for JetSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for JetSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    return;
}
////----------------------------------------------------------

} // Susy namespace
