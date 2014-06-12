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

/// return a copy of the str where the leading and trailing space+tab have been dropped
std::string rmLeadingTrailingWhitespaces(const std::string &str);
/// given str, return a copy where contiguous whitespaces have been replaced by single whitespaces
/**
   Do we need to worry about tab vs. space? DG-2014-06-10
 */
std::string multipleSpaces2singleSpace(std::string str);
/// whether str contains substr
bool contains(const std::string &str, const std::string &substr);
/// split inputString in a vector of tokens
std::vector< std::string > tokenizeString(const std::string &inputString, char separator);
/// convert a string to a double
double string2double(const std::string &s);
/// convert a string to a double accounting for '*' multiplicative factors
double multiply(const std::string &str);
/// whether a string can be safely converted to an int
bool isInt(const std::string& s);
/// given a directory, return the list of its files
/**
   This function could be in another header.
 */
std::vector<std::string> filesFromDir(const std::string &dirname);

} // utils
} // susy
#endif
