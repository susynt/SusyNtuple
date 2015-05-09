#include "SusyNtuple/MuonId.h"

namespace Susy
{
using std::string;
std::string MuonId2str(const MuonId &m)
{
    string s = "Unknown";
    switch(m){
    case MuonId::VeryLoose     : s =  "VeryLoose"    ; break;
    case MuonId::Loose         : s =  "Loose"        ; break;
    case MuonId::Medium        : s =  "Medium"       ; break;
    case MuonId::Tight         : s =  "Tight"        ; break;
    case MuonId::MuonIdInvalid : s =  "MuonIdInvalid"; break;
  }
    return s;
}
} // Susy
