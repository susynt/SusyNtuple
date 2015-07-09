#include "SusyNtuple/ElectronId.h"
namespace Susy
{

using std::string;

std::string ElectronId2str(const ElectronId &e)
{
    string s = "Unknown";
    switch(e){
    case ElectronId::TightLH           : s = "TightLH"         ; break;
    case ElectronId::MediumLH          : s = "MediumLH"        ; break;
    case ElectronId::LooseLH           : s = "LooseLH"         ; break;
    case ElectronId::VeryLooseLH       : s = "VeryLooseLH"     ; break;
    case ElectronId::TightLH_nod0      : s = "TightLH_nod0"    ; break;
    case ElectronId::MediumLH_nod0     : s = "MediumLH_nod0"   ; break;
    case ElectronId::LooseLH_nod0      : s = "LooseLH_nod0"    ; break;
    case ElectronId::ElectronIdInvalid : s = "ElectronIdInvalid"; break;
    }
    return s;
}
} // Susy
