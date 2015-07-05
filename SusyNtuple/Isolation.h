#ifndef SUSYNTUPLE_ISOLATION_H
#define SUSYNTUPLE_ISOLATION_H

#include <string>

namespace Susy
{
enum Isolation{
    GradientLoose=0
    ,Gradient
    ,LooseTrackOnly
    ,Loose
    ,Tight
    ,IsolationInvalid
};

/// Human-readable names
std::string Isolation2str(const Isolation &iso);

} // namespace Susy
#endif
