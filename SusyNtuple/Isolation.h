#ifndef SUSYNTUPLE_ISOLATION_H
#define SUSYNTUPLE_ISOLATION_H

#include <string>

namespace Susy
{
enum class Isolation{
    GradientLoose=0
    ,Gradient
    ,LooseTrackOnly
    ,Loose
    ,Tight
    ,Cone40CaloOnly
    ,Cone40
    ,Cone20
    ,IsolationInvalid
};

/// Human-readable names
std::string Isolation2str(const Isolation &iso);

} // namespace Susy
#endif
