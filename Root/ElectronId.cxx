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
    case ElectronId::LooseLLHBLayer     : s = "LooseLLHBLayer"   ; break;
    case ElectronId::LooseLLH           : s = "LooseLLH"         ; break;
    case ElectronId::VeryLooseLLH       : s = "VeryLooseLLH"     ; break;
    case ElectronId::ElectronIdInvalid : s = "ElectronIdInvalid"; break;
    }
    return s;
}
} // Susy
