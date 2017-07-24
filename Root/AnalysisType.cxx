#include "SusyNtuple/AnalysisType.h"

namespace Susy
{

std::string AnalysisType2str(const AnalysisType &a)
{
    std::string out;
    switch(a) {
    case AnalysisType::Ana_2Lep   : out = "Ana_2Lep"     ; break;
    case AnalysisType::Ana_3Lep   : out = "Ana_3Lep"     ; break;
    case AnalysisType::Ana_4Lep   : out = "Ana_4Lep"     ; break;
    case AnalysisType::Ana_2LepWH : out = "Ana_2LepWH"   ; break;
    case AnalysisType::Ana_SS3L   : out = "Ana_SS3L"     ; break;
    case AnalysisType::Ana_Stop2L : out = "Ana_Stop2L"   ; break;
    case AnalysisType::Ana_WWBB   : out = "Ana_WWBB"     ; break;
    case AnalysisType::kUnknown   : out = "Unknown"      ; break;
    }
    return out;    
}

} // namespace Susy
