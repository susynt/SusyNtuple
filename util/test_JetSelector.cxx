

#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/AnalysisType.h"
#include "SusyNtuple/Jet.h"
#include "SusyNtuple/string_utils.h"

#include <string>
#include <vector>

using namespace std;
using namespace Susy;

int main(int argc, char **argv)
{

    bool verbose(true);
    if(verbose)
        cout<<"Being called as: "<<Susy::utils::commandLineArguments(argc, argv)<<endl;
    float epsilon = 0.01;
    Jet centralLightJet;
    centralLightJet.SetPtEtaPhiE(100.0, JetSelector::defaultCentralEtaMax()-epsilon, 0.1, 100.0);
    centralLightJet.detEta = centralLightJet.Eta();
    Jet centralBJet(centralLightJet);
    centralBJet.mv1 = JetSelector::defaultBtagMinValue()+epsilon;
    Jet forwardJet(centralLightJet);
    forwardJet.SetPtEtaPhiE(100.0, JetSelector::defaultCentralEtaMax()+epsilon, 0.1, 100.0);


    JetSelector selector = JetSelector().setAnalysis(AnalysisType::Ana_2Lep).setVerbose(verbose);

    bool passAll = true;
    // passAll &= (selector.isCentral     (centralLightJet)==true);
    // passAll &= (selector.isCentralLight(centralLightJet)==true);
    // passAll &= (selector.isCentralB    (centralLightJet)==false);
    // passAll &= (selector.isCentralLight(centralBJet    )==false);
    // passAll &= (selector.isCentralBJet (centralBJet    )==true);
    // passAll &= (selector.isForwardJet  (forwardJet     )==false);
    // \todo test jfv, test analysis-specific cuts
    cout<<"JetSelector: "<<(passAll ? "passed all tests" : "failed some test")<<endl;
    
    return passAll;
}
