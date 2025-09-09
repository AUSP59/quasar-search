// SPDX-License-Identifier: Apache-2.0
#include "quasar/fs.hpp"
#include "quasar/search.hpp"
#include "quasar/search_dispatch.hpp"
#include "quasar/mmap.hpp"
#include "quasar/glob.hpp"
#include "quasar/metrics_export.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <mutex>

namespace quasar::fs {

static bool is_binary_prefix(const std::string& s) {
  // Heuristic: any NUL within first 4 KiB -> binary
  size_t n = s.size() < 4096 ? s.size() : 4096;
  for (size_t i=0;i<n;++i) if (s[i] == '\0') return true;
  return false;
}

static void load_ignores(const char* path, std::vector<std::string>& out) {
  if (!path) return;
  std::ifstream in(path);
  if (!in) return;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#') continue;
    out.emplace_back(line);
  }
}

void walk_tree(const std::filesystem::path& root,
               bool follow_symlinks,
               const std::function<void(const std::filesystem::path&)>& on_file) {
  std::filesystem::directory_options opts = std::filesystem::directory_options::skip_permission_denied;
  if (follow_symlinks) opts |= std::filesystem::directory_options::follow_directory_symlink;
  if (std::filesystem::is_regular_file(root)) { on_file(root); return; }
  if (!std::filesystem::exists(root)) return;
  for (auto it = std::filesystem::recursive_directory_iterator(root, opts);
       it != std::filesystem::recursive_directory_iterator(); ++it) {
    std::error_code ec;
    const auto& p = it->path();
    if (it->is_symlink(ec) && !follow_symlinks) continue;
    if (it->is_regular_file(ec)) on_file(p);
  }
}

static size_t scan_stream(std::istream& in, std::string_view needle, size_t chunk_size, bool use_simd) {
  if (needle.empty()) return 0;
  const size_t CHUNK = chunk_size ? chunk_size : (1 << 20);
  size_t total = 0;
  std::string carry;
  for (;;) {
    std::string chunk;
    chunk.resize(CHUNK);
    in.read(chunk.data(), CHUNK);
    size_t got = static_cast<size_t>(in.gcount());
    chunk.resize(got);
    if (got == 0) break;
    std::string current;
    current.reserve(carry.size() + chunk.size());
    current.append(carry);
    current.append(chunk);
    auto v = quasar::search::find_all_best(current, needle, use_simd);
    for (size_t pos : v) if (pos >= carry.size()) ++total;
    if (needle.size() > 1) {
      size_t keep = std::min(needle.size() - 1, current.size());
      carry.assign(current.data() + current.size() - keep, keep);
    } else {
      carry.clear();
    }
  }
  return total;
}

size_t scan_file(const std::filesystem::path& path, std::string_view needle) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return 0;
  return scan_stream(in, needle, 1 << 20, false);
}

std::vector<FileResult> scan_tree(const std::filesystem::path& rootp,
                                  std::string_view needle,
                                  const ScanOptions& opts) {
  std::vector<FileResult> out;
  std::mutex out_mu;
  std::atomic<size_t> total{0};
  std::atomic<uint64_t> total_bytes{0};
  size_t jobs = opts.jobs ? opts.jobs : std::max<size_t>(1, std::thread::hardware_concurrency());

  std::vector<std::string> ignores = opts.ignore_globs;
  load_ignores(opts.ignore_from, ignores);
  size_t files_total = 0, files_done = 0;
  quasar::thread_pool pool(jobs);
  walk_tree(rootp, opts.follow_symlinks, [&](const std::filesystem::path& p){
    // apply ignore globs
    std::string ps = p.string();
    for (auto& g : ignores) { if (quasar::glob::match(g, ps)) return; }
    ++files_total;
    pool.submit([&, p]{
      std::error_code ec;
      uint64_t fsz = std::filesystem::file_size(p, ec);
      if (opts.max_bytes_per_file && fsz > opts.max_bytes_per_file) { ++files_done; return; }
      auto t0 = std::chrono::steady_clock::now();
      size_t n = 0;
      if (opts.use_mmap) {
        quasar::mmap::View v{};
        if (quasar::mmap::map_file(p, v)) {
          std::string_view sv(v.data, v.size);
          auto matches = quasar::search::find_all_best(sv, needle, opts.simd_prefilter);
          n = matches.size();
          quasar::mmap::unmap(v);
        }
      } else {
        std::ifstream in(p, std::ios::binary);
        if (in) {
          // Peek first chunk for binary heuristic
          std::string head; head.resize(std::min<size_t>(4096, opts.chunk_size));
          in.read(head.data(), head.size()); size_t got = (size_t)in.gcount(); head.resize(got);
          bool bin = is_binary_prefix(head);
          in.clear(); in.seekg(0);
          if (bin && opts.binary_mode == ScanOptions::BinaryMode::Skip) { ++files_done; return; }
          n = scan_stream(in, needle, opts.chunk_size, opts.simd_prefilter);
        }
      }
      auto t1 = std::chrono::steady_clock::now();
      uint64_t ms = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

      if (n) {
        std::lock_guard<std::mutex> lk(out_mu);
        out.push_back(FileResult{p, n, fsz, ms});
      }
      total.fetch_add(n);
      total_bytes.fetch_add(fsz);
      ++files_done;
      if (opts.progress && files_total) {
        double pct = (100.0 * files_done) / (double)files_total;
        std::cerr << "{\"progress_pct\":" << pct << ",\"file\":\"" << p.string() << "\"}" << "\n";
      }
    });
  });
  pool.wait_idle();
  if (opts.json) {
    std::cerr << "{"total_matches": " << total.load()
              << ", "files": " << out.size()
              << ", "bytes": " << total_bytes.load()
              << "}\n";
  }
  return out;
}

} // namespace quasar::fs