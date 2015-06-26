#include "SusyNtuple/Isolation.h"

namespace Susy
{

using std::string;
std::string Isolation2str(const Isolation &iso)
{
    string s = "Unknown";
    switch(iso){
        case Isolation::GradientLoose           : s = "GradientLoose"   ; break;
        case Isolation::Gradient                : s = "Gradient"        ; break;
        case Isolation::VeryLoose               : s = "VeryLoose"       ; break;
        case Isolation::Loose                   : s = "Loose"           ; break;
        case Isolation::Tight                   : s = "Tight"           ; break;
        case Isolation::IsolationInvalid        : s = "IsolationInvalid"; break;
    }
    return s;
}
}; // namespace Susy
