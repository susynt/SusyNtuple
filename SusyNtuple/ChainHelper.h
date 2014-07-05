#ifndef SusyNtuple_ChainHelper_h
#define SusyNtuple_ChainHelper_h

#include <fstream>

#include "TFile.h"
#include "TChain.h"



///    A set of static methods to help build a TChain from input root files
/**
   The chain can be built easily by specifying
   
   - single input root file
   - input directory of root files
   - a file with list of root files   
*/

class ChainHelper
{

  public:

    //ChainHelper(std::string treeName = "");
    //~ChainHelper(){};

    enum Status {
      GOOD = 0,
      BAD = 1
    };

    // Add a file - not very useful
    static Status addFile(TChain* chain, std::string file);

    // Add a fileList
    static Status addFileList(TChain* chain, std::string fileListName);

    // Add all files in a directory
    static Status addFileDir(TChain* chain, std::string fileDir);

    // Add a comma separated list of files in a string
    //static Status addCommaList(TChain* chain, std::string files);

};



#endif
