// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <functional>
#include <optional>
#include <atomic>

namespace quasar::fs {

struct ScanOptions {
  bool follow_symlinks = false;
  size_t jobs = 0; // 0 = auto
  bool count_only = false;
  bool json = false;
  size_t chunk_size = 1 << 20; // default 1 MiB
  bool use_mmap = false;
  bool simd_prefilter = false;
  const char* metrics_out = nullptr; // optional JSON file
  const char* prom_file = nullptr;   // optional Prometheus text file
  enum class BinaryMode { Skip, Text, All };
  BinaryMode binary_mode = BinaryMode::Skip;
  std::vector<std::string> ignore_globs;
  const char* ignore_from = nullptr;
  uint64_t max_bytes_per_file = 0;
  bool progress = false;
};

struct FileResult {
  std::filesystem::path path;
  size_t matches = 0;
  uint64_t bytes = 0;
  uint64_t duration_ms = 0;
};

// Non-throwing walk; calls 'on_file(path)' for regular files.
void walk_tree(const std::filesystem::path& root,
               bool follow_symlinks,
               const std::function<void(const std::filesystem::path&)>& on_file);

// Scan a single file for 'needle', returns matches count.
size_t scan_file(const std::filesystem::path& path, std::string_view needle);

// Parallel scan across a tree; returns per-file results.
std::vector<FileResult> scan_tree(const std::filesystem::path& root,
                                  std::string_view needle,
                                  const ScanOptions& opts);

} // namespace quasar::fs