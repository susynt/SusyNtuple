// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_ANALYSISTYPE_H
#define SUSYNTUPLE_ANALYSISTYPE_H

#include <string>

namespace Susy
{
/// 2-lep or 3-lep flag
enum class AnalysisType {
    Ana_2Lep,  ///< Dilepton electroweak
    Ana_3Lep,  ///< trilepton electroweak
    Ana_2LepWH, ///< C1N2 in WH samesign+jets
        Ana_SS3L,   ///< Strong SS3L analysis 
    kUnknown
};
/// Human-readable names
std::string AnalysisType2str(const AnalysisType &a);

} // Susy
#endif
