#include "SusyNtuple/MCWeighter.h"

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
    TTree *noTree=NULL;
    MCWeighter mcw(noTree); // this picks up the default xsec files in SUSYTools
    mcw.parseAdditionalXsecFile(dummyFilename, verbose);
    mcw.parseAdditionalXsecDirectory("/tmp/dummy_xsecs/", verbose);
    //cout<<"xsec for 105200: "<<mcw.getXsecTimesEff()<<endl; // bummer! can only test with Susy::Event...to be modified.
    return 0;
}
//----------------------------------------------------------
