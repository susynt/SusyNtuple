#include "SusyNtuple/TauSelector.h"
#include "SusyNtuple/Tau.h"

#include <iostream>

using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {

//----------------------------------------------------------
TauSelector* TauSelector::build(const AnalysisType &a, bool verbose)
{
    TauSelector *selector;
    switch(a) {
    case AnalysisType::Ana_2Lep   : selector = new TauSelector_2Lep(); break;
    case AnalysisType::Ana_3Lep   : selector = new TauSelector_3Lep(); break;
    case AnalysisType::Ana_2LepWH : selector = new TauSelector_2LepWH(); break;
    case AnalysisType::Ana_SS3L   : selector = new TauSelector_SS3L(); break;
    case AnalysisType::Ana_Stop2L : selector = new TauSelector_Stop2L(); break;
    default:
        cout<<"TauSelector::build(): unknown analysis type '"<<AnalysisType2str(a)<<"'"
            <<" returning vanilla TauSelector"<<endl;
        selector = new TauSelector();
    }
    return selector;
}
//----------------------------------------------------------
TauSelector::TauSelector():
    m_verbose(false)
{
}
//----------------------------------------------------------
bool TauSelector::isBaseline(const Tau& tau)
{
    // \todo update with Run2 recommendations
    // DG-2015-09-22
    // the current implementation is a relic from Run1.
    // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SusyObjectDefinitionsr2013TeV#Taus

    return (tau.Pt() > 20.0 &&
            passBdtBaseline(tau));
}
//----------------------------------------------------------
bool TauSelector::isSignal(const Tau& tau)
{
    return (isBaseline(tau) &&
            passBdtSignal(tau) );
}
//----------------------------------------------------------
bool TauSelector::passBdtBaseline(const Tau& tau)
{
    bool fail_ele = (tau.nTrack == 1 && tau.eleBDTLoose != 0);
    return (tau.jetBDTSigMedium == 1 &&
            tau.muonVeto == 0 &&
            !fail_ele );
}
//----------------------------------------------------------
bool TauSelector::passBdtSignal(const Tau& tau)
{
    bool fail_ele = (tau.nTrack == 1 && tau.eleBDTLoose != 0);
    return (tau.jetBDTSigMedium == 1 &&
            tau.muonVeto == 0 &&
            !fail_ele);
}
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
// begin JetSelector_SS3L
//----------------------------------------------------------

//----------------------------------------------------------
// begin JetSelector_Stop2L
//----------------------------------------------------------

//----------------------------------------------------------
} // namespace Susy

