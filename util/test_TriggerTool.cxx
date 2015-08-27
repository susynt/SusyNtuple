#include "SusyNtuple/TriggerTools.h"
#include "SusyNtuple/ChainHelper.h"

#include <iostream>
#include <string>

using namespace std;

/**
   Test TriggerTools by building a trigger map

   davide.gerbaudo@gmail.com
   Aug 2015
 */

//----------------------------------------------------------
int main(int argc, char **argv)
{
    if(argc<2) {
        cout<<"usage: "<<argv[0]<<" <input>"<<endl
            <<"(see ChainHelper::addInput for possible formats)"<<endl;
        return 1;
    }
    bool verbose=true;
    string input = argv[1];
    TriggerTools tt;
    if(verbose) tt.m_dbg = true;
    tt.init(ChainHelper::firstFile(input, verbose));
    tt.dumpTriggerInfo();

    return 0;
}
//----------------------------------------------------------
