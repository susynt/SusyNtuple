#include "SusyNtuple/JetSelector.h"
#include "SusyNtuple/SusyNt.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits> // underlying_type

using Susy::AnalysisType;

namespace Susy {
//----------------------------------------------------------
std::string AnalysisType2str(const AnalysisType &a)
{
    std::string s;
    switch(a) {
    case AnalysisType::Ana_2Lep      : s = "Ana_2Lep";   break;
    case AnalysisType::Ana_3Lep      : s = "Ana_3Lep";   break;
    case AnalysisType::Ana_2LepWH    : s = "Ana_2LepWH"; break;
    case AnalysisType::Ana_SS3L      : s = "Ana_SS3L";   break;
    case AnalysisType::kUnknown      : s = "Unknown";    break;
    // no default, so that the compiler will warn us of un-handled cases
    }
    return s;
}
//----------------------------------------------------------
} // Susy
