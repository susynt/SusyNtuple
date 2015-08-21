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

namespace Susy {
//----------------------------------------------------------
JetSelector::JetSelector():
/*
  \todo: these will be used internally when refactoring
    m_skip_pt(true),
    m_skip_eta(true),
    m_skip_jvf(true),
    m_min_pt(0.0),
    m_max_eta(0.0),
    m_max_jvf_eta(0.0),
    m_min_jvf(0.0),
*/
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_verbose(false)
{
}
//----------------------------------------------------------
JetSelector& JetSelector::setSystematic(const NtSys::SusyNtSys &s)
{
    m_systematic = s;
    return *this;
}
//----------------------------------------------------------
JetSelector& JetSelector::setAnalysis(const AnalysisType &a)
{
    m_analysis = a;
    switch(m_analysis){
    case AnalysisType::Ana_2Lep: /*\todo: set values*/ break;
    case AnalysisType::Ana_3Lep: /*\todo: set values*/ break;
    case AnalysisType::Ana_2LepWH: /*\todo: set values*/ break;
    case AnalysisType::Ana_SS3L: /*\todo: set values*/ break;
    default:
        cout<<"JetSelector::setAnalysis error: invalid analysis"
            <<" '"<<std::underlying_type<AnalysisType>::type(a)<<"'"
            <<" ("<<AnalysisType2str(a)<<")"
            <<"           will apply default jet selection"<<endl;
    }
    return *this;
}
//----------------------------------------------------------
JetSelector& JetSelector::setJvfTool(const JVFUncertaintyTool &t)
{
    m_jvftool = t;
    return *this;
}
//----------------------------------------------------------
bool JetSelector::isSignalJet(const Jet* jet)
{
  // For now, 2lep analysis is not using this jet definition
  //float ptCut = m_anaType==Ana_2Lep? JET_SIGNAL_PT_CUT_2L : JET_SIGNAL_PT_CUT_3L;
    bool pass = false;
    if(jet) {
        if(m_analysis==AnalysisType::Ana_2Lep){
            pass = (isCentralLightJet(jet) || isCentralBJet(jet) || isForwardJet(jet));
        } else {
            float ptCut = JET_SIGNAL_PT_CUT_3L;
            pass = (jet->Pt() > ptCut
                    && fabs(jet->Eta()) < JET_ETA_CUT
                    && jetPassesJvfRequirement(jet));
        }
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isSignalJet2Lep(const Jet* jet)
{
    cout<<"JetSelector::isSignalJet2Lep: obsolete, use JetSelector::isSignalJet"<<endl;
    return false;
}
//----------------------------------------------------------
bool JetSelector::isCentralLightJet(const Jet* jet)
{
    // This function is mostly used by the 2L analyses. Needs to be reorganized...
    bool pass = false;
    if(jet) {
        pass = (jet->Pt() > JET_PT_L20_CUT
                && fabs(jet->detEta) < JET_ETA_CUT_2L
                && jet->mv1 < MV1_80
                && JetSelector::jetPassesJvfRequirement(jet));
    } else {
        cout << "isCentralLightJet: invalid jet(" << jet << "), return " << pass << endl;
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isCentralBJet(const Jet* jet)
{
  if(jet->Pt() < JET_PT_B20_CUT) return false;
  if(fabs(jet->detEta) > JET_ETA_CUT_2L) return false;
  if(jet->mv1 < MV1_80) return false;

  return true;
}
//----------------------------------------------------------
bool JetSelector::isForwardJet(const Jet* jet)
{
  if(jet->Pt() < JET_PT_F30_CUT         ) return false;
  if(fabs(jet->detEta) < JET_ETA_CUT_2L  ) return false;
  if(fabs(jet->detEta) > JET_ETA_MAX_CUT ) return false;
  return true;
}
//----------------------------------------------------------
bool JetSelector::hasJetInBadFCAL(const JetVector& baseJets, uint run, bool isMC)
{
  // Only applied to data in periods C1 to C8
    if(isMC || run<206248 || run>207332) return false; // refactor this DG
  for(uint iJ=0; iJ<baseJets.size(); ++iJ){
    const Jet* j = baseJets.at(iJ);
    if(isBadFCALJet(j)) return true;
  }
  return false;
}
//----------------------------------------------------------
bool JetSelector::isBadFCALJet(const Jet* jet)
{
  if(jet->Pt() > BAD_FCAL_PT  &&
     fabs(jet->Eta()) > BAD_FCAL_ETA &&
     jet->Phi() > BAD_FCAL_PHILOW &&
     jet->Phi() < BAD_FCAL_PHIHIGH)
    return true;
  return false;
}
//----------------------------------------------------------
bool JetSelector::jetPassesJvfRequirement(const Jet* jet)
{
    float maxPt=JET_JVF_PT;
    float maxEta=JET_JVF_ETA;
    float nominalJvtThres=JET_JVF_CUT;
    float jvfThres(nominalJvtThres);
    if(m_analysis == AnalysisType::Ana_2Lep || m_analysis == AnalysisType::Ana_2LepWH) {
        nominalJvtThres = JET_JVF_CUT_2L;
        maxEta = JET_ETA_CUT_2L;
    }
    bool pass=false;
    if(jet) {
        float pt(jet->Pt()), eta(jet->detEta);
        bool applyJvf(pt < maxPt && fabs(eta) < maxEta);
        //bool jvfUp(m_systematic == NtSys::JVF_UP), jvfDown(m_systematic == NtSys::JVF_DN);
        //AT 05-09-15 JVF obsolete run-2
        bool jvfUp=false, jvfDown=false;
        if(jvfUp || jvfDown) {
            bool isPileUp = false; // Twiki [add link here] says to treat all jets as hardscatter
            jvfThres = m_jvftool.getJVFcut(nominalJvtThres, isPileUp, pt, eta, jvfUp);
        }
        // Ana_2Lep accepts jvf of -1 (corresponds to jet w/out tracks)
        bool acceptMinusOne(m_analysis==AnalysisType::Ana_2Lep);
        bool jvfIsMinusOne(fabs(jet->jvf + 1.0) < 1e-3);
        if(applyJvf) {
            pass = (jet->jvf > jvfThres || (acceptMinusOne && jvfIsMinusOne));
        } else {
            pass = true;
        }
    } else {
        cout<<"jetPassesJvfRequirement: invalid inputs jet("<<jet<<")."
            <<"Return " << pass << endl;
    }
    return pass;
}
//----------------------------------------------------------
size_t JetSelector::count_CL_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(),
                         std::bind1st(std::mem_fun(&JetSelector::isCentralLightJet), this));
}
//----------------------------------------------------------
size_t JetSelector::count_CB_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(), JetSelector::isCentralBJet);
}
//----------------------------------------------------------
size_t JetSelector::count_F_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(), JetSelector::isForwardJet);
}
//----------------------------------------------------------
} // Susy
