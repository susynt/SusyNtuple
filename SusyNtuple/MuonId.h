//  -*- c++ -*-
#ifndef SUSY_MUONID_H
#define SUSY_MUONID_H

#include <string>

namespace Susy
{
  enum class muID{
    VeryLoose=0
    ,Loose
    ,Medium
    ,Tight
    ,muIDInvalid
  };
  static const muID muID_first() { return muID::VeryLoose; }
  static const muID muID_last() { return muID::muIDInvalid; }
  /// assumes consecutive values without gaps
  static const size_t muID_num_values() { return 1 + static_cast<size_t>(muID_last()) - static_cast<size_t>(muID_first());}

  const std::string muIDNames[] = {
    "VeryLoose"
    ,"Loose"
    ,"Medium"
    ,"Tight"
    ,"muIDInvalid"
  };
} // Susy
#endif
