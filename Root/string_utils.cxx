#include "SusyNtuple/string_utils.h"

using std::string;

using namespace susy::utils;

//--------------------------------------
std::string susy::utils::rmLeadingTrailingWhitespaces(const std::string &str)
{
  string result;
  size_t startpos = str.find_first_not_of(" \t");
  size_t endpos = str.find_last_not_of(" \t");
  bool allSpaces = ( string::npos == startpos ) || ( string::npos == endpos);
  if(allSpaces)
      result = "";
  else
      result = str.substr(startpos, endpos-startpos+1);
  return result;
}
//--------------------------------------
