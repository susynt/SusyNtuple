#include "SusyNtuple/TauId.h"
namespace Susy
{

using std::string;

string TauId2str(const TauId &t)
{
    string s = "Uknown";
    switch(t){
    case TauId::None            : s = "None"        ; break;
    case TauId::Loose           : s = "Loose"       ; break;
    case TauId::Medium          : s = "Medium"      ; break;
    case TauId::Tight           : s = "Tight"       ; break;
    case TauId::Invalid         : s = "Invalid"     ; break;
    }
    return s;
}

} // namespace Susy
