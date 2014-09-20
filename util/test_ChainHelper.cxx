#include "SusyNtuple/ChainHelper.h"

#include "TChain.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
   Test ChainHelper by adding files in a few ways

   davide.gerbaudo@gmail.com
   Jul 2014
 */

const string treename = "susyNt";
const string dirname = "/tmp/dummy_dir/";
const string dummyFilename = "/tmp/dummy_list.txt";
void writeDummyFiles(const string &filename)
{    
    gSystem->Exec(("mkdir -p "+dirname).c_str());
    ofstream outFile;
    outFile.open(filename.c_str(), std::ofstream::out);
    for(int i=0; i<4; ++i){
        TString rfname;
        rfname.Form("%s/file%d.root", dirname.c_str(), i);
        TFile f(rfname, "recreate");
        f.cd();
        TTree t(treename.c_str(), "dummy title");
        t.Write();
        f.Close();
        outFile<<rfname.Data()<<endl;    
    }
    outFile.close();
}
//----------------------------------------------------------
int main(int argc, char **argv)
{
    bool verbose=true;
    writeDummyFiles(dummyFilename);
    TChain chain(treename.c_str());
    cout<<endl<<"Adding single file:"<<endl;
    ChainHelper::addInput(&chain, (dirname+"/file0.root").c_str(), verbose);
    chain.Print();
    cout<<endl<<"Adding filelist:"<<endl;
    ChainHelper::addInput(&chain, dummyFilename, verbose);
    chain.Print();
    cout<<endl<<"Adding directory:"<<endl;
    ChainHelper::addInput(&chain, dirname.c_str(), verbose);
    chain.Print();
    
    return 0;
}
//----------------------------------------------------------
