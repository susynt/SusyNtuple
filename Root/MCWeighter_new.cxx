#include "SusyNtuple/MCWeighter.h"

#include "SusyNtuple/Event.h"
#include "SusyNtuple/string_utils.h"
#include "SusyNtuple/vec_utils.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TIter.h"
#include "TChainElement.h"
#include "TString.h"

//std/stl
#include <iostream>
#warning dantrim check all the includes are they necessary
#include <cstdlib> // atoi
#include <iterator> // distance
#include <sstream> // std::ostringstream
#include <string>
#include <fstream>

using namespace std;
using namespace Susy;


// ------------------------------------------------------------------------- //
//  MCWeighter
// ------------------------------------------------------------------------- //
MCWeighter::MCWeighter() :
    m_dbg(false),
    m_sumw_method(Sumw_NT),
    m_xsec_method(Xsec_ST),
    m_sumw(-1),
    m_default_sumw(-1),
    m_sumw_file(""),
    m_sumw_map_built(false),
    m_xsecDB(gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str()))
{
}

MCWeighter::MCWeighter(TTree* tree, string xsecDir) :
    m_dbg(false),
    m_sumw_method(Sumw_NT),
    m_xsec_method(Xsec_ST),
    m_sumw(-1),
    m_default_sumw(-1),
    m_sumw_file(""),
    m_sumw_map_built(false),
    m_xsecDB(gSystem->ExpandPathName(xsecDir.c_str()))
{
    if(tree) {
        buildSumwMap(tree);
    }
    else {
        cout<<"MCWeighter    Provided TTree object is null. Exitting."<<endl;
        exit(1);
    }
}
// ------------------------------------------------------------------------- //
void MCWeighter::setSumwFromFILE(string file)
{
    bool exists = std::ifstream(file).good();
    if(!exists) {
        cout << "MCWeighter::setSumwFromFILE    ERROR Provided file (" << file
                << ") is not found. Exitting." << endl;
        exit(1);
    }

    // set the method to file
    m_sumw_method = Sumw_FILE;
    m_sumw_file = file;

}
// ------------------------------------------------------------------------- //
const Susy::Event& MCWeighter::readFirstEvent(TTree* tree)
{
    Susy::Event* evt = 0;
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("event",1);
    tree->SetBranchAddress("event", &evt);
    tree->GetEntry(0);
    return *evt;
}
// ------------------------------------------------------------------------- //
void MCWeighter::buildSumwMap(TTree* tree)
{
    if(tree->InheritsFrom("TChain")) {
        buildSumwMapFromChain(dynamic_cast<TChain*>(tree));
    }
    else buildSumwMapFromTree(tree);

}
// ------------------------------------------------------------------------- //
void MCWeighter::buildSumwMapFromChain(TChain* chain)
{
    // Loop over the files in the tchain
    TObjArray* files = chain->GetListOfFiles();
    TIter next(files);
    TChainElement* chainElement = 0;
    while ((chainElement = (TChainElement*)next())) {
        TString fileTitle = chainElement->GetTitle();
        TFile* f = TFile::Open(fileTitle.Data());
        //grab the tree
        TTree* tree = (TTree*)f->Get("susyNt");
        buildSumwMapFromTree(tree);
        f->Close();
        delete f;
    }
}
// ------------------------------------------------------------------------- //
void MCWeighter::buildSumwMapFromTree(TTree* tree)
{
    const Event& evt = MCWeighter::readFirstEvent(tree);
    unsigned int mcid = evt.mcChannel;

    if(sumwMethod()==Sumw_NT) {
        m_sumw += evt.sumOfEventWeights;
        if(dbg()) cout << "MCWeighter::buildSumwMapFromTree    mcid: " << mcid
                    << "  running sumw: " << m_sumw << endl;
        
    }
    else if(sumwMethod()==Sumw_FILE) {
        m_sumw = getSumwFromFile(mcid);
    }
}
// ------------------------------------------------------------------------- //
void MCWeighter::getSumwFromFile(mcid)
{
    m_sumw = m_default_sumw;

    std::ifstream infile(m_sumw_file);
    string line;

    int mcid_;
    int proc_;
    double sumw_;
    while(getline(infile, line)) {
        istringstream iss(line);
        int col = 0;
        while(iss) {
            if(col==0) iss >> mcid_;
            else if(col==2) { iss >> proc_; }
            else if(col==4) { iss >> sumw_; }
            col++;
            if(col>4) break;
        }
        if(mcid_==mcid) {
            SumwMapKey key(mcid_, proc_);
            m_sumwMap[key] = sumw_;
        }
    } // while

    if((sumw_ == m_default_sumw) || (sumw_ == 0)) {
        cout<<"MCWeighter::getSumwFromFile    ERROR Unable to find correct sumw"
            << " for MCID " << mcid << " while looking in file: "
            << m_sumw_file <<endl;
        cout<<"MCWeighter::getSumwFromFile    ERROR  --> Is the format of the file"
            << " correct (it should be two columns, separated by white-space) "
            << " or is the MCID not in the file?" << endl;
        exit(1);
    }

    return sumw_;
}
