// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

int main() {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_binary_test";
  fs::create_directories(dir);
  // binary file with NUL
  std::vector<char> data = {'x','\0','y','q','u','a','s','a','r'};
  std::ofstream f(dir / "bin.dat", std::ios::binary); f.write(data.data(), data.size());
  std::ofstream(dir / "text.txt") << "quasar";
  quasar::fs::ScanOptions o; o.binary_mode = quasar::fs::ScanOptions::BinaryMode::Skip;
  auto res = quasar::fs::scan_tree(dir, "quasar", o);
  size_t total = 0; for (auto& r: res) total += r.matches;
  assert(total == 1); // only text.txt matched
  fs::remove_all(dir);
  return 0;
}