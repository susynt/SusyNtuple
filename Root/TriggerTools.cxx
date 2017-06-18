// ------------------------------------------- //
// Class/Tool to handle trigger information    //
// stored in SusyNt                            //
//                                             //
// author: Daniel Antrim                       //
// date  : April 6 2015                        //
// ------------------------------------------- //

//SusyNtuple
#include "SusyNtuple/TriggerTools.h"
#include "SusyNtuple/TriggerList.h"
#include "SusyNtuple/Event.h"
#include "SusyNtuple/Lepton.h"

//ROOT
#include "TH1F.h"
#include "TFile.h"
#include "TChain.h"

//std/stl
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;


//////////////////////////////////////////////////////////////////////////////
TriggerTools::TriggerTools(bool dbg) :
    m_dbg(dbg)
{
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::getTrigNames()
{ 
    //c.f. SusyNtuple/TriggerList.h

    vector<string> all_triggers;
    // add single muon triggers
    all_triggers.insert(all_triggers.end(), single_muo_triggers_list.begin(), single_muo_triggers_list.end());
    // add dimuon triggers
    all_triggers.insert(all_triggers.end(), di_muo_triggers_list.begin(), di_muo_triggers_list.end());
    // add single electron triggers
    all_triggers.insert(all_triggers.end(), single_ele_triggers_list.begin(), single_ele_triggers_list.end());
    // add di-electron triggers
    all_triggers.insert(all_triggers.end(), di_ele_triggers_list.begin(), di_ele_triggers_list.end());
    // add ele+muon dilepton triggers
    all_triggers.insert(all_triggers.end(), ele_muo_triggers_list.begin(), ele_muo_triggers_list.end());
    // add met triggers
    all_triggers.insert(all_triggers.end(), met_triggers_list.begin(), met_triggers_list.end());

    return all_triggers;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::single_muo_triggers()
{
    return single_muo_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::di_muo_triggers()
{
    return di_muo_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::single_ele_triggers()
{
    return single_ele_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::di_ele_triggers()
{
    return di_ele_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::ele_muo_triggers()
{
    return ele_muo_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TriggerTools::met_triggers()
{
    return met_triggers_list;
}
//////////////////////////////////////////////////////////////////////////////
bool TriggerTools::init(const std::string &filename)
{
    bool success=false;
    if(m_dbg) {
        cout << "-----------------------------------------------------------------" << endl;
        cout << "TriggerTools::init    Initializing internal trigger tool and map" << endl;
        cout << "-----------------------------------------------------------------" << endl;
    }
    TString nameTriggerHistogram("trig");
    TFile *f = TFile::Open(filename.c_str());
    TH1F *h = f ? static_cast<TH1F*>(f->Get(nameTriggerHistogram)) : nullptr;
    if(h) {
        m_triggerMap.clear();
        buildTriggerMap(h);
        success = true;
    } else {
        cout<<"TriggerTools::TriggerTools ERROR Cannot initialize TriggerTools!"
            <<" Missing one of the inputs:"
            <<" filename "<<filename<<" file "<<f<<" histo "<<h<< endl
            <<" Exiting."<<endl;
        exit(1);
    }
    if(m_dbg)
        cout<<" ------------------ "<<endl;
    if(f){
        f->Close();
        f->Delete();
    }
    return success;
}
//////////////////////////////////////////////////////////////////////////////
void TriggerTools::buildTriggerMap(const TH1* trigHisto)
{
    for(int trigBin = 1; trigBin < trigHisto->GetNbinsX(); trigBin++) {
        string triggerChainName = trigHisto->GetXaxis()->GetBinLabel(trigBin);
        m_triggerMap[triggerChainName] = trigBin-1;
        m_triggerMap_bit[m_triggerMap[triggerChainName]] = triggerChainName;

        if(m_dbg) {
            cout << "Trigger " << triggerChainName << " at bit " << m_triggerMap[triggerChainName] << endl;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
int TriggerTools::idx_of_trigger(std::string chain)
{
    const vector<string> trigs = getTrigNames();
    for(size_t trig_idx = 0; trig_idx < trigs.size(); trig_idx++) {
        if(trigs.at(trig_idx) == chain) {
            return trig_idx;
        }
    }
    cout << "TriggerTools::idx_of_trigger    WARNING Did not find index of requested trigger '" << chain << "'!" << endl;
    return -1;
}
//////////////////////////////////////////////////////////////////////////////
std::string TriggerTools::trigger_at_idx(int idx)
{
    return m_triggerMap_bit[idx];
}
//////////////////////////////////////////////////////////////////////////////
bool TriggerTools::passTrigger(const TBits& triggerbits, const std::string &triggerName) const
{
    bool pass = false;
    auto nameBit = m_triggerMap.find(triggerName);
    if(nameBit!=m_triggerMap.end()){
        pass = triggerbits.TestBitNumber(nameBit->second);
    }
    else {
        std::cout << "Trigger " << triggerName << " not available!!" << std::endl;
        std::cout << "Dumping available triggers and exitting." << std::endl;
        dumpTriggerInfo();
        exit(1);
    }
    return pass;
}

// ------------------------------------------- //
// Dump information about what triggers are
// stored in the SusyNt
// ------------------------------------------- //
void TriggerTools::dumpTriggerInfo() const
{
    // print them sorted by value, not by key (default for map)
    typedef std::pair<std::string, int> sint_t;
    vector<sint_t> sortedTrigs(m_triggerMap.begin(), m_triggerMap.end());
    std::sort(sortedTrigs.begin(), sortedTrigs.end(),
              [](const sint_t &a, const sint_t &b) -> bool { return a.second < b.second; });
    cout<<" // ---------------------------------- // "<<endl;
    cout<<"    Available triggers                    "<<endl;
    cout<<"    Name : Bit                            "<<endl;
    for(const sint_t &t : sortedTrigs) {
        cout<<"    "<<t.first<<" : "<<t.second<<endl;
    }
    cout<<" // ---------------------------------- // "<<endl;
}
