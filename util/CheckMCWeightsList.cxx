#include "SusyNtuple/string_utils.h"
#include "SusyNtuple/CheckWeights.h"
#include "SusyNtuple/ChainHelper.h"


#include "TChain.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;




// ------------------------------------------------------------------------ //
bool check_mc_weight(string filename, int check_number, int total)
{

    TChain* chain = new TChain("susyNt");
    ChainHelper::addInput(chain, filename, 0);
    //chain->ls();

    CheckWeights* weightChecker = new CheckWeights();
    weightChecker->setCheckNumber(check_number);
    weightChecker->setTotal(total);
    
    chain->Process(weightChecker, "", 1);

    delete chain;
    

    return true;
}

// ------------------------------------------------------------------------ //
int main(int argc, char **argv)
{
    string filelist_name = "";
    if(argc==2) filelist_name = argv[1]; 
    else {
        cout << "You may only provide a single command line argument: the name of filelist to be run over" << endl;
        exit(1);
    }

    cout << "Looping over input files..." << endl;
    ifstream fileListCount(filelist_name.c_str());
    if(!fileListCount.is_open()) {
        cout << "ERROR opening filelist " << filelist_name << endl;
        exit(1);
    }

    int n_total_files = 0;
    int n_checked = 0;
    string filename;
    while(fileListCount >> filename) {
        n_total_files++;
    }

    ifstream fileList(filelist_name.c_str());
    if(!fileList.is_open()) {
        cout << "ERROR opening filelist " << filelist_name << endl;
        exit(1);
    }
    while(fileList >> filename) {
        n_checked++;
        check_mc_weight(filename, n_checked, n_total_files);
    } // while

}
