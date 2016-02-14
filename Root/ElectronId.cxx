#include "SusyNtuple/ElectronId.h"
namespace Susy
{

using std::string;

std::string ElectronId2str(const ElectronId &e)
{
    string s = "Unknown";
    switch(e){
    case ElectronId::TightLLH           : s = "TightLLH"         ; break;
    case ElectronId::MediumLLH          : s = "MediumLLH"        ; break;
    case ElectronId::LooseLLH           : s = "LooseLLH"         ; break;
    case ElectronId::VeryLooseLLH       : s = "VeryLooseLLH"     ; break;
    case ElectronId::TightLLH_nod0      : s = "TightLLH_nod0"    ; break;
    case ElectronId::MediumLLH_nod0     : s = "MediumLLH_nod0"   ; break;
    case ElectronId::LooseLLH_nod0      : s = "LooseLLH_nod0"    ; break;
    case ElectronId::ElectronIdInvalid : s = "ElectronIdInvalid"; break;
    }
    return s;
}
} // Susy
