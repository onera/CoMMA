#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

#include <assert.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;

template<typename CoMMAIndexType, typename CoMMACardType>
inline unordered_set<CoMMAIndexType> d_keys_to_set(
    const unordered_map<CoMMAIndexType, CoMMACardType> &dict) {
  unordered_set<CoMMAIndexType> s_neighbours_of_seed = {};
  for (const auto &i_k_v : dict) {
    s_neighbours_of_seed.insert(i_k_v.first);
  }
  return s_neighbours_of_seed;
}

#endif  // COMMA_PROJECT_UTIL_H
