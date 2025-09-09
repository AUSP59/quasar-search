// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>

int main() {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_mmap_test";
  fs::create_directories(dir);
  std::ofstream(dir / "big.txt") << std::string(1024*64, 'x') + std::string("quasar") + std::string(1024*64, 'y');
  quasar::fs::ScanOptions o; o.use_mmap = true;
  auto res = quasar::fs::scan_tree(dir, "quasar", o);
  size_t total = 0; for (auto& r: res) total += r.matches;
  assert(total == 1);
  fs::remove_all(dir);
  return 0;
}