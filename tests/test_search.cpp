// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <iostream>
#include <cassert>

static void check_vec(const std::vector<size_t>& a, const std::vector<size_t>& b) {
  assert(a.size() == b.size());
  for (size_t i = 0; i < a.size(); ++i) assert(a[i] == b[i]);
}

int main() {
  using quasar::search::find_all;
  using quasar::search::find_all_bmh;
  {
    auto v = find_all("aaaaa", "aa");
    check_vec(v, {0,1,2,3});
  }
  {
    auto v = find_all_bmh("ababa", "aba");
    check_vec(v, {0,2});
  }
  {
    auto v = find_all("", "x");
    check_vec(v, {});
  }
  {
    auto v = find_all_bmh("hello world", "world");
    check_vec(v, {6});
  }
  std::cout << "OK\n";
  return 0;
}