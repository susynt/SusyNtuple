#ifndef TRIGGERLIST_H
#define TRIGGERLIST_H

#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> single_muo_triggers_list =
{
    "HLT_mu20",
    "HLT_mu20_iloose_L1MU15",
    "HLT_mu22",
    "HLT_mu24",
    "HLT_mu24_iloose",
    "HLT_mu24_imedium",
    "HLT_mu24_iloose_L1MU15",
    "HLT_mu26_imedium",
    "HLT_mu40",
    "HLT_mu50",
    "HLT_mu60_0eta105_msonly"
};
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> di_muo_triggers_list =
{
    "HLT_mu18_mu8noL1",
    "HLT_mu20_mu8noL1",
    "HLT_mu22_mu8noL1",
    "HLT_mu24_mu8noL1"
};
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> single_ele_triggers_list =
{
    "HLT_e12_lhloose",
    "HLT_e12_lhloose_L1EM10VH",
    "HLT_e15_lhloose_L1EM13VH",
    "HLT_e17_lhloose",
    "HLT_e17_lhmedium",
    "HLT_e24_lhmedium_iloose_L1EM18VH",
    "HLT_e24_lhmedium_iloose_L1EM20VH",
    "HLT_e24_lhmedium_L1EM20VHI",
    "HLT_e24_lhtight_iloose",
    "HLT_e24_lhtight_nod0_iloose",
    "HLT_e24_lhmedium_nod0_iloose_L1EM20VH",
    "HLT_e24_lhtight_ivarloose",
    "HLT_e24_lhtight_nod0_ivarloose",
    "HLT_e26_lhmedium_L1EM22VHI",
    "HLT_e26_lhtight_iloose",
    "HLT_e26_lhtight_nod0_iloose",
    "HLT_e26_lhtight_ivarloose",
    "HLT_e26_lhtight_nod0_ivarloose",
    "HLT_e60_lhmedium",
    "HLT_e60_lhmedium_nod0"
};
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> di_ele_triggers_list =
{
    "HLT_2e12_lhloose_L12EM10VH",
    "HLT_2e15_lhloose_L12EM13VH",
    "HLT_2e15_lhvloose_L12EM13VH",
    "HLT_2e15_lhvloose_nod0_L12EM13VH",
    "HLT_2e17_lhloose",
    "HLT_2e17_lhvloose",
    "HLT_2e17_lhvloose_nod0"
};
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> ele_muo_triggers_list =
{
    "HLT_e17_lhloose_mu14",
    "HLT_e17_lhloose_nod0_mu14",
    "HLT_e7_lhmedium_mu24",
    "HLT_e7_lhmedium_nod0_mu24",
    "HLT_e26_lhmedium_L1EM22VHI_mu8noL1",
    "HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1",
    "HLT_e24_lhmedium_L1EM20VHI_mu8noL1"
};
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> met_triggers_list = 
{
    "HLT_xe70",
    "HLT_xe80_tc_lcw_L1XE50",
    "HLT_xe90_mht_L1XE50",
    "HLT_xe110_mht_L1XE50",
    "HLT_xe100_mht_L1XE50"
};

#endif
