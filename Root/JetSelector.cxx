#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/SusyNt.h"

#include "JVFUncertaintyTool/JVFUncertaintyTool.h"

#include <cassert>
#include <iostream>

using Susy::JetSelector;
using Susy::Jet;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;

namespace Susy {
//----------------------------------------------------------
JetSelector::JetSelector(JVFUncertaintyTool* const jvftool,
                         const NtSys::SusyNtSys systematic,
                         const AnalysisType& analysis):
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
    m_jvftool(jvftool),
    m_systematic(systematic),
    m_analysis(analysis),
    m_verbose(false)
{
}
//----------------------------------------------------------
JetSelector JetSelector::build_2L_analysis_selector(JVFUncertaintyTool* const t,
                                                    const SusyNtSys &s,
                                                    const AnalysisType &a)
{
    return JetSelector(t, s, a)
        // .requireMinPt(20.0)
        // .requireMaxEta(2.5)
        ;
}
//----------------------------------------------------------
JetSelector JetSelector::build_3L_analysis_selector(JVFUncertaintyTool* const t,
                                                    const SusyNtSys &s,
                                                    const AnalysisType &a)
{
    return JetSelector(t, s, a)
        // .requireMinPt(20.0)
        // .requireMaxEta(2.5)
        ;
}
//----------------------------------------------------------
JetSelector JetSelector::build_WH_analysis_selector(JVFUncertaintyTool* const t,
                                                    const SusyNtSys &s,
                                                    const AnalysisType &a)
{
    return JetSelector(t, s, a)
        // .requireMinPt(20.0)
        // .requireMaxEta(2.5)
        ;
}
//----------------------------------------------------------
bool JetSelector::isSignalJet(const Jet* jet)
{
  // For now, 2lep analysis is not using this jet definition
  //float ptCut = m_anaType==Ana_2Lep? JET_SIGNAL_PT_CUT_2L : JET_SIGNAL_PT_CUT_3L;
    bool pass = false;
    if(jet) {
        float ptCut = JET_SIGNAL_PT_CUT_3L;
        pass = (jet->Pt() > ptCut
                && fabs(jet->Eta()) < JET_ETA_CUT
                && jetPassesJvfRequirement(jet, JET_JVF_PT, JET_JVF_ETA, JET_JVF_CUT));
    } else {
        cout << "isSignalJet: invalid jet(" << jet << "), return " << pass << endl;
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isSignalJet2Lep(const Jet* jet)
{
    return (isCentralLightJet(jet)
            || isCentralBJet(jet)
            || isForwardJet(jet));
}
//----------------------------------------------------------
bool JetSelector::isCentralLightJet(const Jet* jet)
{
    // This function is mostly used by the 2L analyses. Needs to be reorganized...
    bool pass = false;
    if(jet) {
        float minJvf = JET_JVF_CUT;
        float maxJvtEta = JET_JVF_ETA;
        if(m_analysis == Ana_2Lep || m_analysis == Ana_2LepWH) {
            minJvf = JET_JVF_CUT_2L;
            maxJvtEta = JET_ETA_CUT_2L;
        }
        pass = (jet->Pt() > JET_PT_L20_CUT
                && fabs(jet->detEta) < JET_ETA_CUT_2L
                && jet->mv1 < MV1_80
                && JetSelector::jetPassesJvfRequirement(jet, JET_JVF_PT, maxJvtEta, minJvf));
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

bool JetSelector::jetPassesJvfRequirement(const Jet* jet, float maxPt, float maxEta, float nominalJvtThres)
{
    bool pass=false;
    if(jet) {
        float pt(jet->Pt()), eta(jet->detEta);
        float jvfThres(nominalJvtThres);
        bool applyJvf(pt < maxPt && fabs(eta) < maxEta);
        bool jvfUp(m_systematic == NtSys::JVF_UP), jvfDown(m_systematic == NtSys::JVF_DN);
        if(jvfUp || jvfDown) {
            if(m_jvftool) {
                bool isPileUp = false; // Twiki [add link here] says to treat all jets as hardscatter
                jvfThres = m_jvftool->getJVFcut(nominalJvtThres, isPileUp, pt, eta, jvfUp);
            } else {
                cout<<"jetPassesJvfRequirement: error, jvfTool required ("<<m_jvftool<<")"<<endl;
                assert(m_jvftool);
            }
        }
        bool acceptMinusOne(m_analysis==Ana_2Lep);  // Ana_2Lep accepts jvf of -1 (corresponds to jet w/out tracks)
        bool jvfIsMinusOne(fabs(jet->jvf + 1.0) < 1e-3);
        if(applyJvf) {
            pass = (jet->jvf > jvfThres || (acceptMinusOne && jvfIsMinusOne));
        } else {
            pass = true;
        }
    } else {
        cout<<"jetPassesJvfRequirement: invalid inputs jet("<<jet<<"), jvfTool("<<m_jvftool<<")."
            <<"Return " << pass << endl;
    }
    return pass;
}
//----------------------------------------------------------

} // Susy
