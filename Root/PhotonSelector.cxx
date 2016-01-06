#include "SusyNtuple/PhotonSelector.h"
#include "SusyNtuple/SusyNt.h"

using Susy::PhotonSelector;
using Susy::Photon;
using Susy::NtSys::SusyNtSys;
using std::cout;
using std::endl;
using std::string;

namespace Susy {

// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
PhotonSelector::PhotonSelector() :
    //////////////////////////
    // default selection
    //////////////////////////

    //////////////////////////
    m_systematic(NtSys::NOM),
    m_analysis(AnalysisType::kUnknown),
    m_sigIso(Isolation::IsolationInvalid),
    m_2lep(false),
    m_3lep(false),
    m_2lepWH(false),
    m_SS3L(false),
    m_verbose(false)
{
}
// -------------------------------------------------------------------------------------------- //
PhotonSelector& PhotonSelector::setAnalysis(const AnalysisType &a)
{
    
    //////////////////////////////////////
    // Set analysis-specific cuts
    //////////////////////////////////////

    //////////////////////////////////////
    // 2L-Analysis
    //////////////////////////////////////
    if( a == AnalysisType::Ana_2Lep ) {
        m_2lep = true;
        m_sigIso = Isolation::FixedCutTight;
    }
    //////////////////////////////////////
    // 3L-Analysis
    //////////////////////////////////////
    else if ( a == AnalysisType::Ana_3Lep ) {
        m_3lep = true;
        m_sigIso = Isolation::FixedCutTight;
    }
    //////////////////////////////////////
    // WH-Analysis
    //////////////////////////////////////
    else if ( a == AnalysisType::Ana_2LepWH ) {
        m_2lepWH = true;
        m_sigIso = Isolation::FixedCutTight;
    }
    //////////////////////////////////////
    // SS3L-Analysis
    //////////////////////////////////////
    else if ( a == AnalysisType::Ana_SS3L ) {
        m_SS3L = true;
        m_sigIso = Isolation::FixedCutTight;
    }
    //////////////////////////////////////
    // Didn't set AnalysisType
    //////////////////////////////////////
    else if( a == AnalysisType::kUnknown ) {
        string error = "PhotonSelector::setAnalysis error: ";
        cout << error << "The AnalysisType has not been set for PhotonSelector. Check that you properly call" << endl;
        cout << error << "'setAnalysis' for PhotonSelector for your analysis." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    //////////////////////////////////////
    // Gone fishin'
    //////////////////////////////////////
    else {
        string error = "PhotonSelector::setAnalysis error: ";
        cout << error << "PhotonSelector is not configured for the AnalysisType (" << AnalysisType2str(a) << ")  provided in " << endl;
        cout << error << "'setAnalysis'! Be sure that your AnalysisType is provided for in SusyNtuple/AnalysisType.h" << endl;
        cout << error << "and that you provide the necessary requirements for your analysis in 'setAnalysis'." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }

    // check that the isolation has been set
    if( m_sigIso == Isolation::IsolationInvalid ) {
        string error = "PhotonSelector::setAnalysis error: ";
        cout << error << "You have not configured the isolation quality for your photon selection." << endl;
        cout << error << ">>> Exiting." << endl;
        exit(1);
    }
    
    m_analysis = a;

    if(m_verbose)
        cout << "PhotonSelector    Configured photon selection for AnalysisType " << AnalysisType2str(m_analysis) << endl;

    return *this;
}

// -------------------------------------------------------------------------------------------- //
//
} // namespace Susy
