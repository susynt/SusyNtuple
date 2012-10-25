#include "SusyNtuple/ChainHelper.h"
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
      //chain->Add(fileName.c_str());
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
