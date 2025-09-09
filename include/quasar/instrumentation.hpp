// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <chrono>
#include <iostream>
#include <map>
#include <string>

namespace quasar::ins {

struct Span {
  std::string name;
  std::chrono::steady_clock::time_point t0;
  std::map<std::string, std::string> attrs;
  bool enabled = false;

  explicit Span(std::string n, bool en = false): name(std::move(n)), t0(std::chrono::steady_clock::now()), enabled(en) {}
  ~Span() {
    if (!enabled) return;
    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cerr << "{"trace":"quasar","span":"" << name << "","duration_ms":" << ms;
    for (auto &kv : attrs) {
      std::cerr << ","" << kv.first << "":"" << kv.second << """;
    }
    std::cerr << "}" << "\n";
  }
};

} // namespace quasar::ins