// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include <cassert>
#include <fstream>
#include <filesystem>

int main() {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_fs_test";
  fs::create_directories(dir);
  // create files
  {
    std::ofstream(dir / "a.txt") << "hello quasar\nquasar apex\n";
    std::ofstream(dir / "b.txt") << "nothing here\n";
    std::ofstream(dir / "c.txt") << "quasar quasar quasar\n";
  }
  quasar::fs::ScanOptions opts;
  opts.jobs = 2;
  auto res = quasar::fs::scan_tree(dir, "quasar", opts);
  size_t total = 0;
  for (auto& r : res) total += r.matches;
  assert(total == 5);
  fs::remove_all(dir);
  return 0;
}