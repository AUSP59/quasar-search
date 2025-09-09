// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include "quasar/search_dispatch.hpp"
#include <cassert>
#include <string>

int main(){
  std::string hay = "aaaaaabaaaaaabaaaaaa";
  std::string n1 = "aa";
  std::string n2 = "baaa";
  auto a1 = quasar::search::find_all(hay, n1);
  auto b1 = quasar::search::find_all_best(hay, n1, false);
  assert(a1 == b1);
  auto a2 = quasar::search::find_all_bmh(hay, n2);
  auto b2 = quasar::search::find_all_best(hay, n2, true);
  assert(a2 == b2);
  return 0;
}