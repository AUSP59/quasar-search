// SPDX-License-Identifier: Apache-2.0

#include "quasar/fs.hpp"
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>

static void prepare_dir(const std::filesystem::path& dir) {
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);
  for (int i=0;i<64;i++) {
    std::ofstream(dir / ("f" + std::to_string(i) + ".txt"))
      << std::string(1024*16, 'x') << "quasar" << std::string(1024*16, 'y');
  }
}

static void BM_Scan_Stream(benchmark::State& st) {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_macro_bench_stream";
  prepare_dir(dir);
  for (auto _ : st) {
    quasar::fs::ScanOptions o; o.jobs = 4; o.use_mmap = false;
    auto res = quasar::fs::scan_tree(dir, "quasar", o);
    benchmark::DoNotOptimize(res.size());
  }
  fs::remove_all(dir);
}
BENCHMARK(BM_Scan_Stream)->Iterations(1)->Repetitions(10);

static void BM_Scan_MMap_SIMD(benchmark::State& st) {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "quasar_macro_bench_mmap";
  prepare_dir(dir);
  for (auto _ : st) {
    quasar::fs::ScanOptions o; o.jobs = 4; o.use_mmap = true; o.simd_prefilter = true;
    auto res = quasar::fs::scan_tree(dir, "quasar", o);
    benchmark::DoNotOptimize(res.size());
  }
  fs::remove_all(dir);
}
BENCHMARK(BM_Scan_MMap_SIMD)->Iterations(1)->Repetitions(10);

BENCHMARK_MAIN();
