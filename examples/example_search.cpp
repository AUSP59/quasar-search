// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <iostream>
int main() {
  auto hits = quasar::search::find_all("hello quasar world quasar", "quasar");
  std::cout << "matches=" << hits.size() << "\n";
  return 0;
}
