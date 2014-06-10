// Dear emacs, this is -*- c++ -*-
#ifndef SUSY_STRING_UTILS_H
#define SUSY_STRING_UTILS_H

/*
  String manipulation utility functions

  Mostly useful to parse text files.

  davide.gerbaudo@gmail.com
  June 2014
*/

#include <string>
#include <vector>

namespace susy{
namespace utils{

typedef std::vector< std::string > vstring_t;

std::string rmLeadingTrailingWhitespaces(const std::string &str);

} // utils
} // susy
#endif
