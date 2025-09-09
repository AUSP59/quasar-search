// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include <iostream>

static void usage() {
  std::cerr << "Usage: quasar-ls <path> [--ignore-glob PAT] [--ignore-from FILE] [--max-bytes N] [--follow-symlinks]\n";
}

int main(int argc, char** argv) {
  if (argc < 2) { usage(); return 2; }
  std::filesystem::path root = argv[1];
  quasar::fs::ScanOptions opts;
  for (int i=2;i<argc;i++) {
    std::string a = argv[i];
    if (a == "--ignore-glob" && i+1<argc) { opts.ignore_globs.emplace_back(argv[++i]); }
    else if (a == "--ignore-from" && i+1<argc) { opts.ignore_from = argv[++i]; }
    else if (a == "--max-bytes" && i+1<argc) { opts.max_bytes_per_file = std::stoull(argv[++i]); }
    else if (a == "--follow-symlinks") { opts.follow_symlinks = true; }
    else { std::cerr << "Unknown option: " << a << "\n"; usage(); return 2; }
  }
  size_t count = 0;
  quasar::fs::walk_tree(root, opts.follow_symlinks, [&](const std::filesystem::path& p){
    std::string ps = p.string();
    ++count;
    std::cout << ps << "\n";
  });
  std::cerr << "files_listed=" << count << "\n";
  return 0;
}