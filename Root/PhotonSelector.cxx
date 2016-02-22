#include "SusyNtuple/PhotonSelector.h"
#include "SusyNtuple/Photon.h"

#include <iostream>

using std::cout;
using std::endl;
using std::string;

namespace Susy {
// -------------------------------------------------------------------------------------------- //
PhotonSelector* PhotonSelector::build(const AnalysisType &a, bool verbose)
{
    PhotonSelector *p;
    switch(a) {
    case AnalysisType::Ana_2Lep :
        p = new PhotonSelector_2Lep();
        p->setSignalIsolation(Isolation::FixedCutTight);
        break;
    case AnalysisType::Ana_3Lep :
        p = new PhotonSelector_3Lep();
        p->setSignalIsolation(Isolation::FixedCutTight);
        break;
    case AnalysisType::Ana_2LepWH :
        p = new PhotonSelector_2LepWH();
        p->setSignalIsolation(Isolation::FixedCutTight);
        break;
    case AnalysisType::Ana_SS3L :
        p = new PhotonSelector_SS3L();
        p->setSignalIsolation(Isolation::FixedCutTight);
        break;
    case AnalysisType::Ana_Stop2L :
        p = new PhotonSelector_Stop2L();
        p->setSignalIsolation(Isolation::FixedCutTight);
        break;
    default :
        cout << "PhotonSelector::build(): unknown analysis type '" << AnalysisType2str(a) << "'"
             << " returning vanilla PhotonSelector" << endl;
        p = new PhotonSelector();
    } // switch
    return p;
}



// -------------------------------------------------------------------------------------------- //
// Constructor
// -------------------------------------------------------------------------------------------- //
PhotonSelector::PhotonSelector():
    m_signalIsolation(Isolation::IsolationInvalid),
    m_verbose(false)
{
}

// -------------------------------------------------------------------------------------------- //
bool PhotonSelector::isBaseline(const Photon* ph)
{
    bool pass = false;
    if(ph) {
        bool pass_author = (ph->authorPhoton || ph->authorAmbiguous); // reco. as photon OR (photon AND ele)
        pass = (ph->tight &&
                ph->Pt()        > 25 &&
                std::abs(ph->clusEta) < 2.37 &&
                pass_author);
    }
    return pass;
}
// -------------------------------------------------------------------------------------------- //
bool PhotonSelector::isSignal(const Photon* ph)
{
    bool pass = false;
    if(ph) {
        pass = (isBaseline(ph) &&
                ph->isoFixedCutTight);
    }
    return pass;
}
// -------------------------------------------------------------------------------------------- //
// begin PhotonSelector_2Lep Ana_2Lep
// -------------------------------------------------------------------------------------------- //

// -------------------------------------------------------------------------------------------- //
// begin PhotonSelector_3Lep Ana_3Lep
// -------------------------------------------------------------------------------------------- //

// -------------------------------------------------------------------------------------------- //
// begin PhotonSelector_2LepWH Ana_2LepWH
// -------------------------------------------------------------------------------------------- //

// -------------------------------------------------------------------------------------------- //
// begin PhotonSelector_SS3L Ana_SS3L
// -------------------------------------------------------------------------------------------- //

// -------------------------------------------------------------------------------------------- //
// begin PhotonSelector_Stop2L Ana_Stop2L
// -------------------------------------------------------------------------------------------- //

} // namespace Susy
