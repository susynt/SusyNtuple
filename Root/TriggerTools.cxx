// ------------------------------------------- //
// Class/Tool to handle trigger information    //
// stored in SusyNt                            //
//                                             //
// author: Daniel Antrim                       //
// date  : April 6 2015                        //
// ------------------------------------------- //

#include "SusyNtuple/TriggerTools.h"

#include "TH1F.h"
#include "TFile.h"
#include "TChain.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/* Trigger "containers"                        */
/*   Used at the SusyNt writing stage to set   */
/*   which triggers are stored in the final    */
/*   output ntuple.                            */

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

////////////
// Run 1
////////////
const std::vector<std::string> triggers_run1 = {""};

////////////
// Run 2
////////////
const std::vector<std::string> triggers_run2 = {

    ////////////////////////////
    // MUON
    ////////////////////////////

    // muon trig
    //"HLT_mu6",
    //"HLT_mu6_msonly",
    //"HLT_mu10",
    //"HLT_mu14",
    //"HLT_mu18",
    "HLT_mu20",
    "HLT_mu20_iloose_L1MU15",
    "HLT_mu22",
    "HLT_mu24",
    "HLT_mu24_iloose",
    "HLT_mu24_imedium",
    "HLT_mu24_iloose_L1MU15",
    "HLT_mu24_ivarloose",
    "HLT_mu24_ivarloose_L1MU15",
    "HLT_mu24_ivarmedium",
    "HLT_mu26_imedium",
    "HLT_mu26_ivarmedium",
    "HLT_mu40",
    "HLT_mu50",
    "HLT_mu60_0eta105_msonly",

    // L1 items
    //"L1_MU10",
    //"L1_MU15",

    // dimuon trig
    //"HLT_2mu10",
    //"HLT_2mu14",
    "HLT_mu18_mu8noL1",
    "HLT_mu20_mu8noL1",
    "HLT_mu22_mu8noL1",
    "HLT_mu24_mu8noL1",
    //"HLT_2mu14_nomucomb",
    //"HLT_2mu10_nomucomb", 

    ////////////////////////////
    // ELECTRON
    ////////////////////////////
    
    // electron trig lh
    //"HLT_e7_lhmedium",
    //"HLT_e9_lhmedium",
    "HLT_e12_lhloose",
    "HLT_e12_lhloose_L1EM10VH",
    "HLT_e15_lhloose_L1EM13VH",
    "HLT_e17_lhloose",
    "HLT_e17_lhmedium",
    "HLT_e24_lhmedium_iloose_L1EM18VH",
    "HLT_e24_lhmedium_iloose_L1EM20VH",
    "HLT_e24_lhmedium_L1EM20VHI",
    "HLT_e24_lhmedium_L1EM20VH",
    "HLT_e24_lhtight_iloose",
    "HLT_e24_lhtight_nod0_ivarloose",
    "HLT_e24_lhtight_nod0_iloose",
    "HLT_e24_lhmedium_nod0_iloose_L1EM20VH",
    "HLT_e24_lhtight_ivarloose",
    "HLT_e24_lhtight_ivarloose_nod0",
    "HLT_e26_lhmedium_L1EM22VHI",
    "HLT_e26_lhtight_iloose",
    "HLT_e26_lhtight_nod0_iloose",
    "HLT_e26_lhtight_ivarloose",
    "HLT_e26_lhtight_ivarloose_nod0",
    "HLT_e60_lhmedium",
    "HLT_e60_lhmedium_nod0",
    "HLT_e120_lhloose",
    "HLT_e140_lhloose_nod0",

    // di-electron trig lh
    //"HLT_2e12_lhvloose_L12EM10VH",
    //"HLT_2e12_lhvloose_nod0_L12EM10VH",
    "HLT_2e12_lhloose_L12EM10VH",
    "HLT_2e15_lhloose_L12EM13VH",
    "HLT_2e15_lhvloose_L12EM13VH",
    "HLT_2e15_lhvloose_nod0_L12EM13VH",
    "HLT_2e17_lhloose",
    "HLT_2e17_lhvloose",
    "HLT_2e17_lhvloose_nod0",
    
    ////////////////////////////
    // ELE/MUON
    ////////////////////////////
    "HLT_e17_loose_mu14",
    "HLT_e17_lhloose_mu14",
    "HLT_e17_lhloose_nod0_mu14",
    "HLT_e7_lhmedium_mu24",
    "HLT_e7_lhmedium_nod0_mu24",
    "HLT_e26_lhmedium_L1EM22VHI_mu8noL1",
    "HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1",
    "HLT_e24_lhmedium_L1EM20VHI_mu8noL1",

    ////////////////////////////
    // MET
    ////////////////////////////

    "HLT_xe70",
    //"HLT_xe80_tc_lcw_L1XE50",
    "HLT_xe80_tc_lcw_L1XE50",
    "HLT_xe90_mht_L1XE50",
    "HLT_xe110_mht_L1XE50",
    "HLT_xe100_mht_L1XE50"
    
};

std::vector<std::string> getTrigNames(string set)
{ 
    if(set.compare("run1")==0) { 
        std::cout << "Attempting to store Run1 triggers which are no longer available" << std::endl;
        std::cout << " >>> Exitting." << std::endl;
        exit(1);
    }
    else if(set.compare("run2")==0) {
        std::cout << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << " Storing Run2 trigger set " << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << std::endl;
        return triggers_run2;
    }
    else {
        std::cout << "getTrigNames error: Requested set of triggers ("<< set << ") not available. Storing Run-2 triggers instead." << std::endl;
        return triggers_run2;
    }
}

// ---------- !! PARADIGM SHIFT !! ----------- //
// ---------- !! PARADIGM SHIFT !! ----------- //
// ---------- !! PARADIGM SHIFT !! ----------- //



/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/* Trigger tool                                */
/*    To be used at the analysis level of      */
/*    SusyNt                                   */

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

// ------------------------------------------- //
// Constructor                                 //
// ------------------------------------------- //
TriggerTools::TriggerTools(bool dbg) :
    m_dbg(dbg)
{
}

//----------------------------------------------------------
bool TriggerTools::init(const std::string &filename)
{
    bool success=false;
    if(m_dbg)
        cout<<" ------------------ "<<endl
            <<"Initializing Trigger"<<endl
            <<" ------------------ "<<endl;
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
// ------------------------------------------- //
// Build trigger-map 
// ------------------------------------------- //
// BinLabels <---> trigger name
// BinNumber <---> trigger bit number

void TriggerTools::buildTriggerMap(const TH1* trigHisto)
{
    for(int trigBin = 1; trigBin < trigHisto->GetNbinsX(); trigBin++) {
        string triggerChainName = trigHisto->GetXaxis()->GetBinLabel(trigBin);
        m_triggerMap[triggerChainName] = trigBin-1;
        
        if(m_dbg) {
            cout << "Trigger " << triggerChainName << " at bit " << m_triggerMap[triggerChainName] << endl;
        }
    }
}

// ------------------------------------------- //
// Test whether a given trigger has fired
// ------------------------------------------- //
bool TriggerTools::passTrigger(const TBits& triggerbits, const std::string &triggerName) const
// bool TriggerTools::passTrigger(TBits& triggerbits, std::string triggerName)
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
