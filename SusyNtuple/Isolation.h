//  -*- c++ -*-
#ifndef SUSYNTUPLE_ISOLATION_H
#define SUSYNTUPLE_ISOLATION_H

#include <string>

namespace Susy
{
enum class Isolation{
    GradientLoose=0 // lepton
    ,Gradient // lepton
    ,LooseTrackOnly // lepton
    ,Loose // lepton
    ,FixedCutTightTrackOnly // lepton
    ,FixedCutTight // photon
    ,FixedCutTightCaloOnly // photon
    ,FixedCutLoose // photon
    ,IsolationInvalid
};

/// Human-readable names
std::string Isolation2str(const Isolation &iso);

} // namespace Susy
#endif
