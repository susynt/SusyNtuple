#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/Jet.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits> // underlying_type

using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {
//----------------------------------------------------------
JetSelector* JetSelector::build(const AnalysisType &a, bool verbose)
{
    JetSelector *selector;
    switch(a) {
    case AnalysisType::Ana_2Lep   : selector = new JetSelector_2Lep(); break;
    case AnalysisType::Ana_3Lep   : selector = new JetSelector_3Lep(); break;
    case AnalysisType::Ana_4Lep   : selector = new JetSelector_4Lep(); break;
    case AnalysisType::Ana_2LepWH : selector = new JetSelector_2LepWH(); break;
    case AnalysisType::Ana_SS3L   : selector = new JetSelector_SS3L(); break;
    case AnalysisType::Ana_Stop2L : selector = new JetSelector_Stop2L(); break;
    default:
        cout<<"JetSelector::build(): unknown analysis type '"<<AnalysisType2str(a)<<"'"
            <<" returning vanilla JetSelector"<<endl;
        selector = new JetSelector();
    }
    return selector;
}
//----------------------------------------------------------
JetSelector::JetSelector() :
    m_systematic(NtSys::NOM),
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
bool JetSelector::isBaseline(const Jet* jet)
{
    return (jet->Pt() > 20.0);
}
//----------------------------------------------------------
bool JetSelector::isSignal(const Jet* jet)
{
    return (isCentralLight(jet) ||
            isCentralB(jet) ||
            isForward(jet));
}
//----------------------------------------------------------
bool JetSelector::isB(const Jet* jet)
{
    return ((jet->Pt()        > 20.0   ) &&
            (fabs(jet->Eta()) <  2.5   ) &&
            (jet->mv2c10      > mv2c10_77efficiency()) &&
            (passJvt(jet)));
}
//----------------------------------------------------------
bool JetSelector::isCentralLight(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()         > 20.0 &&
                fabs(jet->detEta) <  2.4 &&
                passJvt(jet) &&
                !isB(jet));
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isCentralB(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()         > 20.0 &&
                fabs(jet->detEta) <  2.4 &&
                passJvt(jet) &&
                isB(jet));
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isForward(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()         > 30.0 &&
                fabs(jet->detEta) >  2.4 &&
                fabs(jet->detEta) <  4.5 );
    }
    return pass;
}
//----------------------------------------------------------
/*
  Threshold as recommended at
  https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Jets
*/
bool JetSelector::passJvt(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = ((jet->jvt         >  0.59) ||
                (fabs(jet->Eta()) >  2.4)  ||
                (jet->Pt()         > 60.0) );
    }
    return pass;
}
//----------------------------------------------------------
/*
  Threshold as recommended at
  https://twiki.cern.ch/twiki/bin/view/AtlasProtected/FJVTCalibration
*/
bool JetSelector::passfJvt(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = ((jet->fjvt        > 0.4) ||
                (fabs(jet->Eta()) < 2.5)  ||
                (jet->Pt()        > 50.0) );
    }
    return pass;
}
//----------------------------------------------------------
size_t JetSelector::count_CL_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(),
                         std::bind1st(std::mem_fun(&JetSelector::isCentralLight), this));
}
//----------------------------------------------------------
size_t JetSelector::count_CB_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(),
                         std::bind1st(std::mem_fun(&JetSelector::isCentralB), this));
}
//----------------------------------------------------------
size_t JetSelector::count_F_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(),
                            std::bind1st(std::mem_fun(&JetSelector::isForward), this));
}
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_2Lep Ana_2Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_3Lep Ana_3Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_2LepWH Ana_2LepWH
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_4Lep Ana_4Lep
//----------------------------------------------------------
bool JetSelector_4Lep::isBaseline(const Jet* jet)
{
  return (jet->Pt() > 20.0);
}
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_SS3L Ana_SS3L
//----------------------------------------------------------
bool JetSelector_SS3L::isBaseline(const Jet* jet)
{
    bool pass = false;
    if(jet){
        pass = jet->Pt() > 20.0;
        // Note: abs(eta) < 2.8 criterion moved to `isSignal` so that
        //       overlap removal can be performed using baseline jets
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_SS3L::isSignal(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (isBaseline(jet) &&
                jet->Pt()        > 20.0 &&
                fabs(jet->Eta()) <  2.8 &&
                passJvt(jet)            );
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_SS3L::isB(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()        > 20.0    &&
                fabs(jet->Eta()) <  2.5    &&
                passJvt(jet)               &&
                jet->mv2c10      > mv2c10_70efficiency());
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_SS3L::isB_for_OR(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()        > 20.0    &&
                fabs(jet->Eta()) <  2.5    &&
                passJvt(jet)               &&
                jet->mv2c10      > mv2c10_77efficiency());
    }
    return pass;
}
//----------------------------------------------------------
// begin JetSelector_Stop2L Ana_Stop2L
//----------------------------------------------------------
bool JetSelector_Stop2L::isBaseline(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt() > 20.0 );
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_Stop2L::isSignal(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass =(jet->Pt()        > 20.0 &&
               fabs(jet->Eta()) <  2.5 &&
               passfJvt(jet) &&
               passJvt(jet));
    }
    return pass;
}
//----------------------------------------------------------
} // Susy namespace
