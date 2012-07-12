#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "TFile.h"
#include "TChain.h"
#include "TROOT.h"
#include "TSystem.h"

TChain* nt;
TFile* f;


void browseSusyNt(const char* fileName){

  nt = new TChain("susyNt");
  nt->Add(fileName);
  f = nt->GetFile(); //grab current file
}

bool browseSusyNt(string fileList, int nFiles=99999)
{
  char name[500];
  FILE* ntupleList;
  ntupleList=fopen(fileList.c_str(),"r");
  if(!ntupleList){
    cerr << "Could not load file containing ntuple file list " << fileList.c_str() << endl;
    return false;
  }

  int nF=0;
  nt = new TChain("susyNt");
  while(fscanf(ntupleList,"%s \n",name) != EOF && nF<nFiles){
    nt->Add(name);
    cout << "Adding Ntuple " << name << " entries " << nt->GetEntries() << endl; 
    nF++;
  }

  if( fclose(ntupleList) == EOF ){
    cerr << "Could not close file containing ntuple file list " << fileList.c_str() << endl;
    return false;
  }
  
  f = nt->GetFile(); //grab current file
  return true;
}
