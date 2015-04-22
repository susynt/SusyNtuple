#include "SusyNtuple/AnalysisType.h"

namespace Susy
{

std::string AnalysisType2str(const AnalysisType &a)
{
    std::string n;
    switch(a) {
    case Ana_2Lep  : "Ana_2Lep"; break;
    case Ana_3Lep  : "Ana_3Lep" ; break;
    case Ana_2LepWH: "Ana_2LepWH"; break;
    case kUnknown  : "Unknown"; break;
    }
    return n;    
}
}
