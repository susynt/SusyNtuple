//  -*- c++ -*-
#ifndef SUSY_MUONID_H
#define SUSY_MUONID_H

#include <string>

namespace Susy
{
enum MuonId{
    VeryLoose=0
    ,Loose
    ,Medium
    ,Tight
    ,MuonIdInvalid
    };
// static const MuonId MuonId_first() { return MuonId::VeryLoose; }
// static const MuonId MuonId_last() { return MuonId::MuonIdInvalid; }
/* /// assumes consecutive values without gaps */
/* static const size_t MuonId_num_values() { return 1 + static_cast<size_t>(MuonId_last()) - static_cast<size_t>(MuonId_first());} */

/// Human-readable names
std::string MuonId2str(const MuonId &m);
} // Susy
#endif
