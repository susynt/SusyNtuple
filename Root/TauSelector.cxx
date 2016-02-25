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
    case AnalysisType::Ana_2Lep   : selector = new TauSelector_2Lep();      break;
    case AnalysisType::Ana_3Lep   : selector = new TauSelector_3Lep();      break;
    case AnalysisType::Ana_4Lep   : selector = new TauSelector_4Lep();      break;
    case AnalysisType::Ana_2LepWH : selector = new TauSelector_2LepWH();    break;
    case AnalysisType::Ana_SS3L   : selector = new TauSelector_SS3L();      break;
    case AnalysisType::Ana_Stop2L : selector = new TauSelector_Stop2L();    break;
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
    return ( (std::abs(tau.Eta()) < 2.5) &&
             (tau.nTrack > 0) &&
             (tau.medium) );
}
//----------------------------------------------------------
bool TauSelector::isSignal(const Tau& tau)
{
    return (isBaseline(tau) &&
            std::abs(tau.Eta()) < 2.47 &&
            tau.Pt() > 20);
}
//----------------------------------------------------------
//----------------------------------------------------------
// begin TauSelector_2Lep Ana_2Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin TauSelector_3Lep Ana_3Lep
//----------------------------------------------------------

//----------------------------------------------------------
// begin TauSelector_2LepWH Ana_2LepWH
//----------------------------------------------------------

//----------------------------------------------------------
// begin TauSelector_4Lep Ana_4Lep
//----------------------------------------------------------
bool TauSelector_4Lep::isBaseline(const Tau& tau)
{
    return (tau.Pt() > 20.0 					&&
			std::abs(tau.Eta()) < 2.47			&& 
			(tau.nTrack == 1 || tau.nTrack ==3 )&&
			std::abs(tau.q) == 1 );
}
//----------------------------------------------------------
bool TauSelector_4Lep::isSignal(const Tau& tau)
{
    return (isBaseline(tau) &&
            tau.medium);
}

//----------------------------------------------------------
// begin TauSelector_SS3L Ana_SS3L
//----------------------------------------------------------

//----------------------------------------------------------
// begin TauSelector_Stop2L Ana_Stop2L
//----------------------------------------------------------

//----------------------------------------------------------
} // namespace Susy

