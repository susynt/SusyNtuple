#include "SusyNtuple/vec_utils.h"

#include <set>
#include <algorithm> // set_intersection
#include <iostream>
#include <iterator>
#include <sstream>

using namespace susy::utils;

//----------------------------------
bool susy::utils::containsAnyOf(const vint_t &firstVec, const vint_t &subVec)
{
    typedef std::set< int > sint;
    sint a(firstVec.begin(), firstVec.end());
    sint b(subVec.begin(), subVec.end());
    vint_t intersection(std::min(a.size(), b.size()));
    vint_t::iterator it = std::set_intersection(a.begin(), a.end(),
                                                b.begin(), b.end(),
                                                intersection.begin());
    return it!=intersection.begin();
}
//----------------------------------
std::string susy::utils::vecToString(const vint_t &vec)
{
  std::stringstream ss;
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(ss,", "));
  return ss.str();
}
//----------------------------------
std::string susy::utils::vecVecToString(const vvint_t &vvec)
{
  std::stringstream ss;
  for(size_t i=0; i<vvec.size(); i++) ss<<"["<<susy::utils::vecToString(vvec[i])<<"], ";
  return ss.str();
}
//----------------------------------
void susy::utils::removeDuplicates(vint_t &vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}
//----------------------------------
