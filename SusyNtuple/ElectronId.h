//  -*- c++ -*-
#ifndef SUSY_ELECTRONID_H
#define SUSY_ELECTRONID_H

namespace Susy
{
  enum eleID{
    TightLLH=0
    ,MediumLLH
    ,LooseLLH
    ,VeryLooseLLH
    ,TightLLH_nod0
    ,MediumLLH_nod0
    ,LooseLLH_nod0
    ,eleIDInvalid
  };
//   static const eleID eleID_first() { return eleID::TightLLH; }
//   static const eleID eleID_last() { return eleID::eleIDInvalid; }
//   /// assumes consecutive values without gaps
// // static const size_t eleID_num_values = (1 + static_cast<size_t>(eleID_last()) - static_cast<size_t>(eleID_first()));
// static const size_t eleID_num_values = 10;
  const std::string eleIDNames[] = {
    "TightLLH"
    ,"MediumLLH"
    ,"LooseLLH"
    ,"VeryLooseLLH"
    ,"TightLLH_nod0"
    ,"MediumLLH_nod0"
    ,"LooseLLH_nod0"
    ,"eleIDInvalid"
  };
} // Susy
#endif
