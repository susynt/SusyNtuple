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
const std::vector<std::string> triggers_run1 = {
    // 2012
    // electron triggers
    "EF_e7_medium1",
    "EF_e12Tvh_loose1",
    "EF_e12Tvh_medium1",
    "EF_e24vh_medium1",
    "EF_e24vhi_medium1",
    "EF_2e12Tvh_loose1",
    "EF_e24vh_medium1_e7_medium1",

    // muon triggers
    "EF_mu4T",
    "EF_mu6",
    "EF_mu8",
    "EF_mu13",
    "EF_mu18_tight",
    "EF_mu24",
    "EF_mu24i_tight",
    "EF_2mu6",
    "EF_2mu13",
    "EF_mu18_tight_mu8_EFFS",
    "EF_e12Tvh_medium1_mu8",
    "EF_mu18_tight_e7_medium1",
    "EF_3mu6",
    "EF_e18vh_medium1_2e7T_medium1",
    "EF_mu18_tight_2mu4_EFFS",
    "EF_2e7T_medium1_mu6",
    "EF_e7T_medium1_2mu6",
    
    // photon triggers
    "EF_g20_loose",
    "EF_g40_loose",
    "EF_g60_loose",
    "EF_g80_loose",
    "EF_g100_loose",
    "EF_g120_loose",

    // tau triggers
    "EF_tau20_medium1",
    "EF_tau20Ti_medium1",
    "EF_tau29Ti_medium1",
    "EF_tau29Ti_medium1_tau20Ti_medium1",
    "EF_tau20Ti_medium1_e18vh_medium1",
    "EF_tau20_medium1_mu15",

    // lep-tau matching
    "EF_e18vh_medium1",
    "EF_mu15",

    // MET trigger
    "EF_2mu8_EFxe40wMu_tclcw",
    "EF_xe80_tclcw_loose",
    
    // jet + met
    "EF_j110_aftchad_xe90_tclcw_loose",
    "EF_j80_a4tchad_xe100_tclcw_loose",
    "EF_j80_a4tchad_xe70_tclcw_dphi2j45xe10",

    // more triggers for ISR analysis
    "EF_mu4T_j65_a4tchad_xe70_tclcw_veryloose",
    "EF_2mu4T_xe60_tclcw",
    "EF_2mu8_EFxe40_tclcw",
    "EF_e24vh_medium1_EFxe35_tclcw",
    "EF_mu24_j65_a4tchad_EFxe40_tclcw",
    "EF_mu24_j65_a4tchad_EFxe40wMu_tclcw"
};

////////////
// Run 2
////////////
const std::vector<std::string> triggers_run2 = {

    // Trigger list last update:
    //     dantrim July 30 2015

    ////////////////////////////
    // MUON
    ////////////////////////////

    // muon trig
    "HLT_mu6",
    "HLT_mu6_msonly",
    "HLT_mu10",
    "HLT_mu14",
    "HLT_mu18",
    "HLT_mu20",
    "HLT_mu20_iloose_L1MU15",
    "HLT_mu22",
    "HLT_mu24",
    "HLT_mu24_imedium",
    "HLT_mu24_iloose_L1MU15",
    "HLT_mu26_imedium",
    "HLT_mu50",
    "HLT_mu60_0eta105_msonly",

    // L1 items
    "L1_MU10",
    "L1_MU15",

    // dimuon trig
    "HLT_2mu10",
    "HLT_2mu14",
    "HLT_mu18_mu8noL1",
    "HLT_mu20_mu8noL1",
    "HLT_mu22_mu8noL1",
    "HLT_mu24_mu8noL1",

    ////////////////////////////
    // ELECTRON
    ////////////////////////////

    // electron trig cut
    "HLT_e12_loose_L1EM10VH",
    "HLT_e15_loose_L1EM13VH",
    "HLT_e17_loose",

    // electron trig lh
    "HLT_e7_lhmedium",
    "HLT_e9_lhmedium",
    "HLT_e12_lhloose",
    "HLT_e12_lhloose_L1EM10VH",
    "HLT_e15_lhloose_L1EM13VH",
    "HLT_e17_lhloose",
    "HLT_e17_lhmedium",
    "HLT_e24_lhmedium_iloose_L1EM18VH",
    "HLT_e24_lhmedium_iloose_L1EM20VH",
    "HLT_e24_lhmedium_L1EM20VHI",
    "HLT_e24_lhtight_iloose",
    "HLT_e26_lhmedium_L1EM22VHI",
    "HLT_e26_lhtight_iloose",
    "HLT_e60_lhmedium",

    // di-electron trig lh
    "HLT_2e12_lhvloose_L12EM10VH",
    "HLT_2e12_lhloose_L12EM10VH",
    "HLT_2e15_lhloose_L12EM13VH",
    "HLT_2e17_lhloose",
    
    ////////////////////////////
    // ELE/MUON
    ////////////////////////////
    "HLT_e17_lhloose_mu14",
    "HLT_e7_lhmedium_mu24",
    "HLT_e26_lhmedium_L1EM22VHI_mu8noL1",
    "HLT_e24_lhmedium_L1EM20VHI_mu8noL1",

    ////////////////////////////
    // MET
    ////////////////////////////

    "HLT_xe35",
    "HLT_xe35_pueta",
    "HLT_xe50",
    "HLT_xe50_pueta",
    "HLT_xe70",
    "HLT_xe70_pueta",
    "HLT_xe80_tc_lcw_L1XE50",
    "HLT_xe100",
    "HLT_xe100_pueta"
    
};

std::vector<std::string> getTrigNames(string set)
{ 
    if(set.compare("run1")==0) { 
        std::cout << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << " Storing Run1 trigger set " << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << std::endl;
        return triggers_run1;
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
