#ifndef SUSY_TAUID_H
#define SUSY_TAUID_H

#include <string>

namespace Susy
{

enum class TauId {
    None,
    Loose,
    Medium,
    Tight,
    Invalid
};

/// Human-readable names
std::string TauId2str(const TauId &t);


} // namespace Susy


#endif
