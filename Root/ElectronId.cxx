#include "SusyNtuple/ElectronId.h"
namespace Susy
{

using std::string;

std::string ElectronId2str(const ElectronId &e)
{
    string s = "Unknown";
    switch(e){
    case TightLLH          : s = "TightLLH"         ; break;
    case MediumLLH         : s = "MediumLLH"        ; break;
    case LooseLLH          : s = "LooseLLH"         ; break;
    case VeryLooseLLH      : s = "VeryLooseLLH"     ; break;
    case TightLLH_nod0     : s = "TightLLH_nod0"    ; break;
    case MediumLLH_nod0    : s = "MediumLLH_nod0"   ; break;
    case LooseLLH_nod0     : s = "LooseLLH_nod0"    ; break;
    case ElectronIdInvalid : s = "ElectronIdInvalid"; break;
    }
    return s;
}
} // Susy
