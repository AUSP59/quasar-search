// SPDX-License-Identifier: Apache-2.0
#include "quasar/metrics_export.hpp"
#include "quasar/instrumentation.hpp"
#include "quasar/fs.hpp"
#include "quasar/log.hpp"
#include "quasar/metrics.hpp"
#include <iostream>
#include <chrono>
#include <cstdlib>

static void usage() {
  std::cerr << "Usage: quasar-grep <pattern> <path> [--count] [--jobs N] [--follow-symlinks] [--chunk BYTES] [--mmap] [--simd] [--metrics-out FILE] [--prom-file FILE] [--ignore-glob PAT] [--ignore-from FILE] [--binary-mode skip|text|all] [--max-bytes N] [--progress] [--json] [--trace]\n";
}

int main(int argc, char** argv) {
  if (argc < 3) { usage(); return 2; }
  std::string needle = argv[1];
  std::filesystem::path root = argv[2];
  quasar::fs::ScanOptions opts;
  for (int i=3;i<argc;i++) {
    std::string a = argv[i];
    if (a == "--count") opts.count_only = true;
    else if (a == "--json") opts.json = true;
    else if (a == "--follow-symlinks") opts.follow_symlinks = true;
    else if (a == "--jobs" && i+1<argc) { opts.jobs = std::stoul(argv[++i]); }
    else if (a == "--mmap") { opts.use_mmap = true; }
    else if (a == "--simd") { opts.simd_prefilter = true; }
    else if (a == "--metrics-out" && i+1<argc) { opts.metrics_out = argv[++i]; }
    else if (a == "--prom-file" && i+1<argc) { opts.prom_file = argv[++i]; }
    else if (a == "--ignore-glob" && i+1<argc) { opts.ignore_globs.emplace_back(argv[++i]); }
    else if (a == "--ignore-from" && i+1<argc) { opts.ignore_from = argv[++i]; }
    else if (a == "--binary-mode" && i+1<argc) { std::string v = argv[++i]; if (v=="skip") opts.binary_mode = quasar::fs::ScanOptions::BinaryMode::Skip; else if (v=="text") opts.binary_mode = quasar::fs::ScanOptions::BinaryMode::Text; else if (v=="all") opts.binary_mode = quasar::fs::ScanOptions::BinaryMode::All; else { std::cerr << "Invalid --binary-mode" << "\n"; return 2; } }
    else if (a == "--max-bytes" && i+1<argc) { opts.max_bytes_per_file = std::stoull(argv[++i]); }
    else if (a == "--progress") { opts.progress = true; }
    else if (a == "--trace") { /* handled via env in instrumentation */ }
    else if (a == "--chunk" && i+1<argc) { opts.chunk_size = std::stoull(argv[++i]); }
    else { std::cerr << "Unknown option: " << a << "\n"; usage(); return 2; }
  }
  auto t0 = std::chrono::steady_clock::now();
  bool trace = std::getenv("QUASAR_TRACE") != nullptr; quasar::ins::Span sp("grep", trace); sp.attrs["needle"] = needle;
  auto results = quasar::fs::scan_tree(root, needle, opts);
  auto t1 = std::chrono::steady_clock::now();

  size_t grand = 0;
  std::vector<double> latencies;
  uint64_t total_bytes = 0;
  for (const auto& r : results) {
    grand += r.matches;
    total_bytes += r.bytes;
    latencies.push_back(static_cast<double>(r.duration_ms));
    if (opts.count_only) {
      std::cout << r.matches << "\t" << r.path.string() << "\n";
    } else {
      std::cout << r.path.string() << ":" << r.matches << " (" << r.duration_ms << " ms)\n";
    }
  }
  auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  double p95 = quasar::metrics::percentile(latencies, 95.0);
  if (opts.count_only) std::cout << grand << "\tTOTAL\n";
  
double seconds = total_ms / 1000.0;
double tps = seconds > 0 ? results.size() / seconds : 0.0;
double mbps = seconds > 0 ? (total_bytes / (1024.0*1024.0)) / seconds : 0.0;
if (opts.prom_file) { quasar::metrics_export::write_prom(opts.prom_file, results.size(), total_bytes, (double)total_ms, p95, tps, mbps); }
  if (opts.json) {
  std::cout << "{"
            << "\"total_matches\":" << grand << ","
            << "\"files\":" << results.size() << ","
            << "\"bytes\":" << total_bytes << ","
            << "\"elapsed_ms\":" << total_ms << ","
            << "\"p95_ms\":" << p95 << ","
            << "\"tps\":" << tps << ","
            << "\"mbps\":" << mbps
            << "}
";
}
  return (grand > 0) ? 0 : 1;
}