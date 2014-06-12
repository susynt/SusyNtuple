#include "SusyNtuple/string_utils.h"

#include <algorithm> // transform
#include <cstdlib> // strtod
#include <dirent.h> // DIR
#include <functional> // multiplies
#include <iostream>
#include <numeric> // accumulate
#include <iterator> // ostream_iterator, back_inserter
#include <sstream> // istringstream
#include <utility> // make_pair

using std::vector;
using std::string;
using std::cout;
using std::endl;

using namespace susy::utils;

//----------------------------------------------------------
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
//----------------------------------------------------------
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
std::string susy::utils::multipleSpaces2singleSpace(std::string str)
{ // from http://stackoverflow.com/questions/8362094/replace-multiple-spaces-with-one-space-in-a-string
  std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
 str.erase(new_end, str.end());
 return str;
}
//----------------------------------------------------------
bool susy::utils::contains(const std::string &str, const std::string &substr)
{
  return (str.find(substr)!=std::string::npos);
}
//----------------------------------------------------------
std::vector< std::string > susy::utils::tokenizeString(const std::string &inputString, char separator)
{
  vector<string> tokens;
  std::istringstream buffer(string(multipleSpaces2singleSpace(inputString)));
  for(string token; getline(buffer, token, separator); /*nothing*/ ) tokens.push_back(token);
  return tokens;
}
//----------------------------------------------------------
double susy::utils::string2double(const std::string &s) { return strtod(s.c_str(), NULL); }
//----------------------------------------------------------
double susy::utils::multiply(const std::string &str)
{
  if(!contains(str, "*")) return string2double(str);
  vector<string> tks(tokenizeString(str, '*'));
  vector<double> factors;
  std::transform(tks.begin(), tks.end(), std::back_inserter(factors), string2double);
  return std::accumulate(factors.begin(), factors.end(), 1.0, std::multiplies<double>());
}
//----------------------------------------------------------
bool susy::utils::isInt(const std::string& s)
{
  std::string rs(rmLeadingTrailingWhitespaces(s));
  if(rs.empty() || ((!isdigit(rs[0])) && (rs[0] != '-') && (rs[0] != '+'))) return false ;
  char * p ;
  strtol(rs.c_str(), &p, 10);
  return (*p == 0) ;
}
//----------------------------------------------------------
std::vector<std::string> susy::utils::filesFromDir(const std::string &dirname)
{
// from http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
    vector<string> filenames;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(dirname.c_str());
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf))){
            filenames.push_back(dirname+"/"+epdf->d_name);
        }
    }
    closedir(dpdf);
    return filenames;
}
//----------------------------------------------------------
