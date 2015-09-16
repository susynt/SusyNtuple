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
        case Isolation::LooseTrackOnly          : s = "LooseTrackOnly"  ; break;
        case Isolation::Loose                   : s = "Loose"           ; break;
        case Isolation::Tight                   : s = "Tight"           ; break;
        case Isolation::Cone40CaloOnly          : s = "Cone40CaloOnly"  ; break;
        case Isolation::Cone40                  : s = "Cone40"          ; break;
        case Isolation::Cone20                  : s = "Cone20"          ; break;
        case Isolation::IsolationInvalid        : s = "IsolationInvalid"; break;
    }
    return s;
}
}; // namespace Susy
