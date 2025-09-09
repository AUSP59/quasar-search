// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>

int main() {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_ignore_test";
  fs::create_directories(dir);
  std::ofstream(dir / "keep.txt") << "quasar";
  std::ofstream(dir / "skip.log") << "quasar";
  quasar::fs::ScanOptions o; o.ignore_globs.push_back("*.log");
  auto res = quasar::fs::scan_tree(dir, "quasar", o);
  size_t total = 0; for (auto& r: res) total += r.matches;
  assert(total == 1); // only keep.txt
  fs::remove_all(dir);
  return 0;
}