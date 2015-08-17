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
    ////////////////////////////////////
    else if( a == AnalysisType::Ana_SS3L ) {
        m_SS3L = true;

        // pt
        JET_MIN_PT_SIGNAL   = 20.;

        // eta
        JET_MAX_ETA         = 2.8;
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
        bool passEta = (jet->Eta() < JET_MAX_ETA);
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
//----------------------------------------------------------
// no longer using JVF
//JetSelector& JetSelector::setJvfTool(const JVFUncertaintyTool &t)
//{
//    m_jvftool = t;
//    return *this;
//}
//----------------------------------------------------------
//bool JetSelector::isSignalJet2Lep(const Jet* jet)
//{
//    cout<<"JetSelector::isSignalJet2Lep: obsolete, use JetSelector::isSignalJet"<<endl;
//    return false;
//}
//----------------------------------------------------------
//bool JetSelector::jetPassesJvfRequirement(const Jet* jet)
//{
//    float maxPt=JET_JVF_PT;
//    float maxEta=JET_JVF_ETA;
//    float nominalJvtThres=JET_JVF_CUT;
//    float jvfThres(nominalJvtThres);
//    if(m_analysis == AnalysisType::Ana_2Lep || m_analysis == AnalysisType::Ana_2LepWH) {
//        nominalJvtThres = JET_JVF_CUT_2L;
//        maxEta = JET_ETA_CUT_2L;
//    }
//    bool pass=false;
//    if(jet) {
//        float pt(jet->Pt()), eta(jet->detEta);
//        bool applyJvf(pt < maxPt && fabs(eta) < maxEta);
//        //bool jvfUp(m_systematic == NtSys::JVF_UP), jvfDown(m_systematic == NtSys::JVF_DN);
//        //AT 05-09-15 JVF obsolete run-2
//        bool jvfUp=false, jvfDown=false;
//        if(jvfUp || jvfDown) {
//            bool isPileUp = false; // Twiki [add link here] says to treat all jets as hardscatter
//            jvfThres = m_jvftool.getJVFcut(nominalJvtThres, isPileUp, pt, eta, jvfUp);
//        }
//        // Ana_2Lep accepts jvf of -1 (corresponds to jet w/out tracks)
//        bool acceptMinusOne(m_analysis==AnalysisType::Ana_2Lep);
//        bool jvfIsMinusOne(fabs(jet->jvf + 1.0) < 1e-3);
//        if(applyJvf) {
//            pass = (jet->jvf > jvfThres || (acceptMinusOne && jvfIsMinusOne));
//        } else {
//            pass = true;
//        }
//    } else {
//        cout<<"jetPassesJvfRequirement: invalid inputs jet("<<jet<<")."
//            <<"Return " << pass << endl;
//    }
//    return pass;
//}
////----------------------------------------------------------
//----------------------------------------------------------
//bool JetSelector::hasJetInBadFCAL(const JetVector& baseJets, uint run, bool isMC)
//{
//  // Only applied to data in periods C1 to C8
//    if(isMC || run<206248 || run>207332) return false; // refactor this DG
//  for(uint iJ=0; iJ<baseJets.size(); ++iJ){
//    const Jet* j = baseJets.at(iJ);
//    if(isBadFCALJet(j)) return true;
//  }
//  return false;
//}
////----------------------------------------------------------
//bool JetSelector::isBadFCALJet(const Jet* jet)
//{
//  if(jet->Pt() > BAD_FCAL_PT  &&
//     fabs(jet->Eta()) > BAD_FCAL_ETA &&
//     jet->Phi() > BAD_FCAL_PHILOW &&
//     jet->Phi() < BAD_FCAL_PHIHIGH)
//    return true;
//  return false;
//}
////----------------------------------------------------------

} // Susy namespace
