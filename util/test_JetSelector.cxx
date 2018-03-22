

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
    // two eta values to test central/fw jets:
    // high-eta is usually from 2.4-2.8 to 4.5
    float lowEtaCalo(0.5), highEtaCalo(3.5);

    Jet centralLightJet;
    centralLightJet.SetPtEtaPhiE(100.0, lowEtaCalo, 0.1, 100.0);
    centralLightJet.detEta = centralLightJet.Eta();
    centralLightJet.mv2c20 = -1.0; // low (~-1) mva output is not btagged


    Jet centralBJet(centralLightJet);
    centralBJet.mv2c20 = +1.0; // high (~1) mva output is btagged

    Jet forwardJet(centralLightJet);
    forwardJet.SetPtEtaPhiE(100.0, highEtaCalo, 0.1, 100.0);


    JetSelector *selector = JetSelector::build(AnalysisType::Ana_2Lep, verbose);
    JetSelector &s = *selector;

    bool passAll = true;
    //passAll &= (s.isCentralLight(&centralLightJet)==true);
    passAll &= (s.isB           (&centralLightJet)==false);
    //passAll &= (s.isCentralLight(&centralBJet    )==false);
    passAll &= (s.isB           (&centralBJet    )==true);
    passAll &= (s.isForward     (&forwardJet     )==false);
    // \todo test jfv, test analysis-specific cuts
    cout<<"JetSelector: "<<(passAll ? "passed all tests" : "failed some test")<<endl;

    delete selector;

    return passAll;
}
