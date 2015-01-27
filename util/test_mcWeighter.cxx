#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/SusyDefs.h" // SumwMapKey

#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
   Just an executable to test MCWeighter by reading a few xsec values from SUSYTools and from a local file

   davide.gerbaudo@gmail.com
   Jun 2014
 */

void writeDummyFile(const string &filename)
{
    ofstream outFile;
    outFile.open(filename.c_str(), std::ofstream::out);
    outFile<<""<<endl; // empty line
    outFile<<"# comment line "<<endl; 
    outFile<<"105200  McAtNloJimmy_CT10_ttbar_LeptonFilter    253.00  1.000000    0.543   0.060000"<<endl; // known dset
    outFile<<"164324      125     11.4842271805      0.06812281078      1.0000000000      0.0751  "<<endl; // known dset
    outFile<<"654321      999     1.0                2.0                3.0               4.0     "<<endl; // dummy dset
    outFile<<"654321      999     1.  0              2.   0             3.    0           4.   0  "<<endl; // broken line
    outFile.close();
}
//----------------------------------------------------------
bool test_isSimplified()
{
    bool success=true;
    bool verbose=true;
    size_t num_failures = 0;
    vector<int> known_simplified_dsids = MCWeighter::dsidsForKnownSimpliedModelSamples(verbose);
    vector<int>::const_iterator dsid = known_simplified_dsids.begin();
    for(;dsid!=known_simplified_dsids.end(); ++dsid)
        if(!MCWeighter::isSimplifiedModel(*dsid, verbose)) {
            num_failures++;
            success = false;
        }
    int dummy_dsid = 1000;
    if(MCWeighter::isSimplifiedModel(dummy_dsid, verbose)){
        num_failures++;
        success = false;
    }
    cout<<"test_isSimplified: "<<(success ? "passed":"failed")<<" ("<<num_failures<<" failures)"<<endl;
    return success;
}
//----------------------------------------------------------
int main(int argc, char **argv)
{
    bool verbose=true;
    string dummyFilename="/tmp/dummy_xsec.txt";
    writeDummyFile(dummyFilename);
    MCWeighter mcw;
    mcw.parseAdditionalXsecFile(dummyFilename, verbose);
    mcw.parseAdditionalXsecDirectory("/tmp/dummy_xsecs/", verbose);
    SumwMapKey key(176558, 157); // test sample needed by Anyes
    cout<<"checking cache for (dsid,proc)=("<<key.first<<", "<<key.second<<") :"
        <<" has key "<<(mcw.sumwmapHasKey(key)?"true":"false")
        <<endl;
    cout<<"dumpSumwMap"<<endl;
    mcw.dumpSumwMap();
    cout<<"dumpXsecCache"<<endl;
    mcw.dumpXsecCache();
    mcw.dumpXsecDb();
    //cout<<"xsec for 105200: "<<mcw.getXsecTimesEff()<<endl; // bummer! can only test with Susy::Event...to be modified.

    MCWeighter::ProcessValidator validator;
    int procID=137;
    validator.validate(procID);
    validator.validate(procID);
    cout<<"validator.validate("<<procID<<").valid : "<<(validator.valid?"true":"false")<<", expect true"<<endl;
    procID=-1;
validator.validate(procID);
    cout<<"validator.validate("<<procID<<").valid : "<<(validator.valid?"true":"false")<<", expect false"<<endl;
    procID=137;
validator.validate(procID);
    procID=0;
validator.validate(procID);
    cout<<"validator.validate("<<procID<<").valid : "<<(validator.valid?"true":"false")<<", expect false"<<endl;

    cout<<"-------------------------"<<endl
        <<"test_isSimplified"<<endl
        <<"-------------------------"<<endl;
    test_isSimplified();

    return 0;

}
//----------------------------------------------------------
