//  -*- c++ -*-
#ifndef SUSY_ELECTRONID_H
#define SUSY_ELECTRONID_H

#include <string>

namespace Susy
{

enum ElectronId {
    TightLLH=0
    ,MediumLLH
    ,LooseLLHBLayer
    ,LooseLLH
    ,VeryLooseLLH
    ,TightLLH_nod0
    ,MediumLLH_nod0
    ,LooseLLH_nod0
    ,ElectronIdInvalid
};

/// Human-readable names
std::string ElectronId2str(const ElectronId &e);

//   static const ElectronId ElectronId_first() { return ElectronId::TightLLH; }
//   static const ElectronId ElectronId_last() { return ElectronId::ElectronIdInvalid; }
//   /// assumes consecutive values without gaps
// // static const size_t ElectronId_num_values = (1 + static_cast<size_t>(ElectronId_last()) - static_cast<size_t>(ElectronId_first()));

} // Susy
#endif
