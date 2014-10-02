#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/SusyDefs.h" // SumwMapKey

#include "TFile.h"
#include "TLeaf.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>

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
int test_mc_weighter_with_generic_sample(const std::string &ntuple_filename)
{
    TFile *inputFile = TFile::Open(ntuple_filename.c_str());
    TTree *tree = ((inputFile && inputFile->IsOpen()) ?
                    static_cast<TTree*>(inputFile->Get("susyNt")) :
                    NULL);
    if(!tree) {
        cout<<"Cannot open input file, failing test"<<endl;
        return 1;
    }
    MCWeighter mcw(tree); // this picks up the default xsec files in SUSYTools
    unsigned int mcChannel, susyFinalState;
    tree->GetEntry(0); // just test on the values from the first entry
    if(TLeaf* l = tree->FindLeaf("mcChannel"))
        mcChannel = static_cast<unsigned int>(l->GetValue());
    else {
        cout<<"cannot get 'mcChannel' leaf"<<endl;
        return 1;
    }
    if(TLeaf* l = tree->FindLeaf("susyFinalState"))
        susyFinalState = static_cast<unsigned int>(l->GetValue());
    else {
        cout<<"cannot get 'susyFinalState' leaf"<<endl;
        return 1;
    }

    SumwMapKey key(mcChannel, susyFinalState);
    cout<<"checking (dsid,proc)=("<<key.first<<", "<<key.second<<") :"
        <<" has key "<<(mcw.sumwmapHasKey(key)?"true":"false")
        <<endl;
    cout<<"dumpSumwMap"<<endl;
    mcw.dumpSumwMap();
    cout<<"dumpXsecCache"<<endl;
    mcw.dumpXsecCache();
    mcw.dumpXsecDb();
    return 0;
}
//----------------------------------------------------------
int test_mc_weighter_with_susy_sample()
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
    // \todo: check the output and return a non-zero value when failing
    return 0;
}
//----------------------------------------------------------
int test_process_validator()
{
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
    // \todo: check the output and return a non-zero value when failing
    return 0;
}
//----------------------------------------------------------
int main(int argc, char **argv)
{
    if(argc>1) test_mc_weighter_with_generic_sample(argv[1]);
    else test_mc_weighter_with_susy_sample();
    test_process_validator();
    return 0;

}
