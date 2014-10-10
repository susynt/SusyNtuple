// Dear emacs, this is -*- c++ -*-
#ifndef SUSY_VEC_UTILS_H
#define SUSY_VEC_UTILS_H

/*
  Vector utility functions
  
  Most of these are useful to manipulate the mc truth table.

  davide.gerbaudo@gmail.com
  April 2014
*/

#include <algorithm>
#include <string>
#include <vector>

namespace susy{
namespace utils{

typedef std::vector< int > vint_t;
typedef std::vector< vint_t > vvint_t;

/// pointer to the end of an array
/**
   Useful for example to initialize vec from array
   \code
   const string tmp_strings[] = {"aaa", "bb", "c"};
   vector<string> strings(strings, end(strings));
   \endcode
   from http://stackoverflow.com/questions/4268886/initialize-a-vector-array-of-strings
*/
template<typename T, size_t N>
T * end(T (&ra)[N]) {
    return ra + N;
}

/// returns true if vector contains val
template < class T >
bool contains(const std::vector<T> &v, const T& val) {
    return std::find(v.begin(), v.end(), val)!=v.end();
}
//! returns true if the intersection of the two vectors is non-empty
bool containsAnyOf(const vint_t &firstVec, const vint_t &subVec);
//! convert vint_t to string
std::string vecToString(const vint_t &vec);
//! convert vvint_t to string
std::string vecVecToString(const vvint_t &vvec);
//! remove duplicates from vint_t
void removeDuplicates(vint_t &vec);

//! filter a vector (remove elements for which Predicate is true)
template < template <typename, typename> class Container,
           typename Predicate,
           typename Allocator,
           typename A
           >
Container<A, Allocator> filter(Container<A, Allocator> const & container, Predicate const & pred) {
  Container<A, Allocator> filtered(container);
  filtered.erase(remove_if(filtered.begin(), filtered.end(), pred), filtered.end());
  return filtered;
}


} // utils
} // susy
#endif
