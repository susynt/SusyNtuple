#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/string_utils.h"
#include <iostream>

using namespace std;

/*--------------------------------------------------------------------------------*/
// Build TChain from input fileList
/*--------------------------------------------------------------------------------*/
ChainHelper::Status ChainHelper::addFile(TChain* chain, string file)
{
  if(!file.empty()) chain->Add(file.c_str());
  return GOOD;
}
/*--------------------------------------------------------------------------------*/
// Build TChain from input fileList
/*--------------------------------------------------------------------------------*/
ChainHelper::Status ChainHelper::addFileList(TChain* chain, string fileListName)
{
  if(!fileListName.empty()){
    ifstream fileList(fileListName.c_str());
    if(!fileList.is_open()){
      cout << "ERROR opening fileList " << fileListName << endl;
      return BAD;
    }
    string fileName;
    while(fileList >> fileName){
      // Add protection against file read errors
      if(chain->Add(fileName.c_str(), -1)==0){
        cerr << "ChainHelper ERROR adding file " << fileName << endl;
        return BAD;
      }
    }
    fileList.close();
  }
  return GOOD;
}

/*--------------------------------------------------------------------------------*/
// Build TChain from input directory
/*--------------------------------------------------------------------------------*/
ChainHelper::Status ChainHelper::addFileDir(TChain* chain, string fileDir)
{
  if(!fileDir.empty()){
    string files = fileDir + "/*";
    chain->Add(files.c_str());
  }
  return GOOD;
}
//----------------------------------------------------------
ChainHelper::Status ChainHelper::addInput(TChain* chain, const std::string &input, bool verbose)
{
    Status status=GOOD;
    using namespace Susy::utils;
    if(contains(input, ",")){
        size_t num_added=0;
        std::vector< std::string > tokens = tokenizeString(input, ',');
        for(size_t i=0; i<tokens.size(); ++i)
            if(GOOD==addInput(chain, tokens[i], verbose)) num_added++;
        status = (num_added==tokens.size() ? GOOD : BAD);
    } else if(inputIsFile(input)){
        if(verbose) cout<<"ChainHelper::addInput adding file "<<input<<endl;
        status = ChainHelper::addFile(chain, input);
    }
    else if(inputIsList(input)){
        if(verbose) cout<<"ChainHelper::addInput adding list "<<input<<endl;
        status = ChainHelper::addFileList(chain, input);
    }
    else if(inputIsDir(input)){
        if(verbose) cout<<"ChainHelper::addInput adding dir "<<input<<endl;
        status = ChainHelper::addFileDir(chain, input);
    }
    else {
        cout<<"ChainHelper::addInput: cannot determine whether the input is a file/filelist/dir"<<endl
            <<"Provide a valid input or use addFile/addFileList/addFileDir"<<endl;
        status = BAD;
    }
    return status;
}
//----------------------------------------------------------
bool ChainHelper::inputIsFile(const std::string &input)
{
    return Susy::utils::contains(Susy::utils::rmLeadingTrailingWhitespaces(input), ".root");
}
//----------------------------------------------------------
bool ChainHelper::inputIsList(const std::string &input)
{
    return Susy::utils::endswith(Susy::utils::rmLeadingTrailingWhitespaces(input), ".txt");
}
//----------------------------------------------------------
bool ChainHelper::inputIsDir(const std::string &input)
{
    return Susy::utils::endswith(Susy::utils::rmLeadingTrailingWhitespaces(input), "/");
}
//----------------------------------------------------------
