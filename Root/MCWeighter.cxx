#include "SusyNtuple/MCWeighter.h"

#include "SusyNtuple/Event.h"
#include "SusyNtuple/string_utils.h"
#include "SusyNtuple/vec_utils.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"
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
    m_xsecDB(gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str())),
    m_xsecDBdir(gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str()))
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
    m_xsecDB(gSystem->ExpandPathName(xsecDir.c_str())),
    m_xsecDBdir(xsecDir)
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
        cout << "MCWeighter::setSumwFromFILE    FATAL Provided file (" << file
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
bool MCWeighter::mapHasKey(SumwMapKey& key)
{
    return m_sumwMap.count(key);
}
// ------------------------------------------------------------------------- //
void MCWeighter::buildSumwMap(TTree* tree)
{
    if(tree->InheritsFrom("TChain")) {
        buildSumwMapFromChain(dynamic_cast<TChain*>(tree));
    }
    else buildSumwMapFromTree(tree);

    printSumwMap();

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
    int process = evt.susyFinalState;
    SumwMapKey key(mcid, process);

    if(sumwMethod()==Sumw_NT) {
        m_sumw += evt.sumOfEventWeights;
        m_sumwMap[key] = m_sumw;
        if(dbg()) cout << "MCWeighter::buildSumwMapFromTree    mcid: " << mcid
                    << "  running sumw: " << m_sumw << "(" << m_sumwMap[key] << ")" << endl;
        
    }
    else if(sumwMethod()==Sumw_FILE) {
        cout << "MCWeighter::buildSumwMapFromTree   Grabbing sumw from file: " << m_sumw_file << endl;
        getSumwFromFile(mcid);
    }
    m_sumw_map_built = true;
}
// ------------------------------------------------------------------------- //
void MCWeighter::getSumwFromFile(unsigned int mcid)
{
    m_sumw = m_default_sumw;

    std::ifstream infile(m_sumw_file);
    string line;

    unsigned int mcid_;
    int proc_;
    double sumw_;
    while(getline(infile, line)) {
        if(isCommentLine(line) || isEmptyLine(line)) continue;
        stringstream iss(line);
        int col = 0;
        while(iss) {
            iss >> mcid_ >> proc_ >> sumw_;
        }
        if(mcid_==mcid) {
            SumwMapKey key(mcid_, proc_);
            if(!mapHasKey(key)) {
                m_sumwMap[key] = sumw_;
            }
        }
    } // while

    if((sumw_ == m_default_sumw) || (sumw_ == 0)) {
        cout<<"MCWeighter::getSumwFromFile    FATAL Unable to find correct sumw"
            << " for MCID " << mcid << " while looking in file: "
            << m_sumw_file <<endl;
        cout<<"MCWeighter::getSumwFromFile    FATAL  --> Is the format of the file"
            << " correct (it should be two columns, separated by white-space) "
            << " or is the MCID not in the file?" << endl;
        exit(1);
    }
}
// ------------------------------------------------------------------------- //
double MCWeighter::getMCWeight(const Susy::Event* evt, const float lumi,
    Susy::NtSys::SusyNtSys sys, bool includePileup)
{

    double weight = 1.0;
    if(evt->isMC) {
        double sumw = getSumw(evt);
        float xsec = getXsecTimesEff(evt, sys);
        if(xsec<0) {
            cout << "MCWeighter::getMCWeight    FATAL Cross-section is negative! This "
                << "could be due to the SUSYTools database not having the sample info."
                << " (mcid=" << evt->mcChannel << ")" << endl;
            cout << "MCWeighter::getMCWeight    FATAL SUSYTools database: "
                << m_xsecDBdir << endl;
            exit(1);
        }
        float pupw = 1.0;
        if(includePileup) {
            pupw = getPileupWeight(evt, sys);
        }
        if(sumw!=0 && sumw>0) {
            weight = (evt->w * pupw * xsec * lumi) / sumw;
        }
        else {
            cout << "MCWeighter::getMCWeight    FATAL Trying to normalize an event "
                << "with an invalid sumw (sumw=" << sumw << ")" << endl;
            exit(1);
        }
    } // ismC

    return weight;

}
// ------------------------------------------------------------------------- //
double MCWeighter::getSumw(const Susy::Event* evt)
{
    double sumw = -1.0;
    if(!m_sumw_map_built) {
        cout << "MCWeighter::getSumw    SumwMap is not built yet!" << endl;
        exit(1);
    }
    unsigned int mcid = evt->mcChannel;
    int procid = evt->susyFinalState>0 ? evt->susyFinalState : 0;
    SumwMapKey key(mcid, procid);
    SumwMap::const_iterator iter = m_sumwMap.find(key);
    if(iter != m_sumwMap.end()) sumw = iter->second;
    else {
        cout << "MCWeighter::getSumw    FATAL Attempting to retrieve sumw for "
            << "mcid: " << mcid << "  procid: " << procid << "  but it is not"
            << " contained in the sumw map! Printing the currently built sumw map below." << endl;
        printSumwMap();
        exit(1);
    }
    return sumw;
}
// ------------------------------------------------------------------------- //
SUSY::CrossSectionDB::Process MCWeighter::getCrossSection(const Event* evt)
{
    using namespace SUSY;
    CrossSectionDB::Process process;
    if(evt->isMC) {
        unsigned int mcid = evt->mcChannel;
        int proc = evt->susyFinalState > 0 ? evt->susyFinalState : 0;
        const intpair k(mcid, proc);
        XSecMap::const_iterator iter = m_xsecCache.find(k);
        bool already_cached(iter != m_xsecCache.end());
        if(already_cached) { process = iter->second; }
        else {
            m_xsecCache[k] = process = m_xsecDB.process(mcid, proc);
        }
        if(dbg() && (process.xsect()<0 || process.kfactor()<0 || process.efficiency()<0 || process.relunc()<0)) {
            cout << "MCWeighter::getCrossSection    WARNING Process information for "
            << "mcid: " << mcid << "  procid: " << proc << " not found "
            << "in SUSYTools xsec database!" << endl;
            //exit(1);
        }
    }
    return process;
}
// ------------------------------------------------------------------------- //
float MCWeighter::getXsecTimesEff(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys)
{
    float xsec = 1.0;
    if(xsecMethod()==Xsec_ST) {
        SUSY::CrossSectionDB::Process p = getCrossSection(evt);
        float xs = p.xsect();
        float kf = p.kfactor();
        float ef = p.efficiency();
        //cout << "xs: " << xs << "  kf: " << kf << "  eff: " << ef << endl;
        xsec = xs * kf * ef;
        if(sys == Susy::NtSys::XS_UP) {
            xsec *= (1. + p.relunc());
        }
        else if(sys == Susy::NtSys::XS_DN) {
            xsec *= (1. - p.relunc());
        }
    }
    else {
        cout << "MCWeighter::getXsecTimesEff    XsecMethod not recognized!" << endl;
        exit(1);
    }

    return xsec;
    
}
// ------------------------------------------------------------------------- //
float MCWeighter::getPileupWeight(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys)
{
    if(sys == Susy::NtSys::PILEUP_UP) return evt->wPileup_up;
    else if(sys == Susy::NtSys::PILEUP_DN) return evt->wPileup_dn;
    else return evt->wPileup;
}
// ------------------------------------------------------------------------- //
void MCWeighter::printSumwMap() const
{
    cout.precision(8);
    cout<<"----------------------------------------"<<endl;
    cout<<" MCWeighter printSumwMap " <<endl;
    cout<<" mcid     |   process     |    sumw " << endl;
    cout<<"- - - - - - - - - - - - - - - - - - - - "<<endl;
    SumwMap::const_iterator iter;
    for(iter = m_sumwMap.begin(); iter != m_sumwMap.end(); iter++)
        cout << iter->first.dsid <<"          " << iter->first.proc << "           " << iter->second << endl; 
 //   for(auto& map : m_sumwMap) {
 //   cout << map.first.dsid << "          " << map.first.proc << "           " << map.second << endl;
 //   }
    cout<<"----------------------------------------"<<endl;
    cout.precision(6);
}
// ------------------------------------------------------------------------- //
bool MCWeighter::isCommentLine(const string &line)
{
    string strippedLine(Susy::utils::rmLeadingTrailingWhitespaces(line));
    return strippedLine.size() > 0 && strippedLine[0] == '#';
}
// ------------------------------------------------------------------------- //
bool MCWeighter::isEmptyLine(const std::string &line)
{
    return Susy::utils::rmLeadingTrailingWhitespaces(line).size() == 0;
}
