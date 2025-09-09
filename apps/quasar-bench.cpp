// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <chrono>
#include <iostream>
#include <random>

int main() {
  using clock = std::chrono::high_resolution_clock;
  std::mt19937_64 rng(42);
  std::string hay(8<<20, '\0'); // 8 MiB
  for (auto& c : hay) c = static_cast<char>('a' + (rng() % 26));
  std::string needle = "quasar";

  auto t0 = clock::now();
  auto v1 = quasar::search::find_all(hay, needle);
  auto t1 = clock::now();
  auto v2 = quasar::search::find_all_bmh(hay, needle);
  auto t2 = clock::now();

  auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

  std::cout << "two_way_ms=" << d1 << " bmh_ms=" << d2 << " matches1=" << v1.size() << " matches2=" << v2.size() << "\n";
  return 0;
}