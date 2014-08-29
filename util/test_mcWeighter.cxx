#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/SusyDefs.h" // SumwMapKey

#include "TFile.h"
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
int main(int argc, char **argv)
{
    bool verbose=true;
    string dummyFilename="/tmp/dummy_xsec.txt";
    writeDummyFile(dummyFilename);
    TFile *inputFile = TFile::Open("/var/tmp/susynt_dev/data/" // test sample needed by Anyes
                                   "ntup_c1c1_n0150/user.gerbaudo."
                                   "mc12_8TeV.176558.Herwigpp_simplifiedModel_wC_slep_noWcasc_50."
                                   "SusyNt.e1708_s1499_s1504_r3658_r3549_p1512_n0150/"
                                   "user.gerbaudo.028813._00001.susyNt.root");
    TTree *tree = ((inputFile && inputFile->IsOpen()) ?
                    static_cast<TTree*>(inputFile->Get("susyNt")) :
                    NULL);
    MCWeighter mcw(tree); // this picks up the default xsec files in SUSYTools
    mcw.parseAdditionalXsecFile(dummyFilename, verbose);
    mcw.parseAdditionalXsecDirectory("/tmp/dummy_xsecs/", verbose);
    SumwMapKey key(176558, 157);
    cout<<"checking (dsid,proc)=("<<key.first<<", "<<key.second<<") :"
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



    return 0;

}
//----------------------------------------------------------
