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
//----------------------------------------------------------
JetSelector* JetSelector::build(const AnalysisType &a, bool verbose)
{
    JetSelector *selector;
    switch(a) {
    case AnalysisType::Ana_2Lep   : selector = new JetSelector_2Lep(); break;
    case AnalysisType::Ana_3Lep   : selector = new JetSelector_3Lep(); break;
    case AnalysisType::Ana_2LepWH : selector = new JetSelector_2LepWH(); break;
    case AnalysisType::Ana_SS3L   : selector = new JetSelector_ss3l(); break;
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
bool JetSelector::isBaselineJet(const Jet* jet)
{
    return (jet->Pt() > 20.0);
}
//----------------------------------------------------------
bool JetSelector::isSignalJet(const Jet* jet)
{
    return (isCentralLightJet(jet) ||
            isCentralBJet(jet) ||
            isForwardJet(jet));
}
//----------------------------------------------------------
bool JetSelector::isBJet(const Jet* jet)
{
    return ((jet->Pt()        > 20.0   ) &&
            (fabs(jet->Eta()) <  2.5   ) &&
            (jet->mv2c20      > -0.4434) && // 77% eff
            (passJvt(jet)));
}
//----------------------------------------------------------
bool JetSelector::isCentralLightJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()         > 20.0 &&
                fabs(jet->detEta) <  2.4 &&
                passJvt(jet) &&
                !isBJet(jet));
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isCentralBJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()         > 20.0 &&
                fabs(jet->detEta) <  2.4 &&
                passJvt(jet) &&
                isBJet(jet));
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector::isForwardJet(const Jet* jet)
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
        pass = ((jet->jvt          >  0.64) ||
                (fabs(jet->detEta) >  2.4)  ||
                (jet->Pt()         > 50.0)  );
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
    return std::count_if(jets.begin(), jets.end(),
                         std::bind1st(std::mem_fun(&JetSelector::isCentralBJet), this));
}
//----------------------------------------------------------
size_t JetSelector::count_F_jets(const JetVector &jets)
{
    return std::count_if(jets.begin(), jets.end(),
                            std::bind1st(std::mem_fun(&JetSelector::isForwardJet), this));
}
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_2Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_3Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_2LepWH
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_ss3l
//----------------------------------------------------------
bool JetSelector_ss3l::isBaselineJet(const Jet* jet)
{
    bool pass = false;
    if(jet){
        pass = (jet->Pt()        > 20.0 &&
                fabs(jet->Eta()) <  2.8 );
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_ss3l::isSignalJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (isBaselineJet(jet) &&
                jet->Pt()        > 20.0 &&
                fabs(jet->Eta()) <  2.8 &&
                passJvt(jet)            );
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_ss3l::isBJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (jet->Pt()        > 20.0    &&
                fabs(jet->Eta()) <  2.4    &&
                passJvt(jet)               &&
                jet->mv2c20      > -0.0436 ); // 70% eff
    }
    return pass;
}
//----------------------------------------------------------
// begin JetSelector_Stop2L
// TODO Danny please check values
//----------------------------------------------------------
bool JetSelector_Stop2L::isBaselineJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass = (fabs(jet->Eta()) <  2.4 &&
                jet->Pt()        > 20.0 );
    }
    return pass;
}
//----------------------------------------------------------
bool JetSelector_Stop2L::isSignalJet(const Jet* jet)
{
    bool pass = false;
    if(jet) {
        pass =((jet->Pt()        > 20.0) &&
               (fabs(jet->Eta()) >  2.8) &&
               passJvt(jet));
    }
    return pass;
}
//----------------------------------------------------------
} // Susy namespace
