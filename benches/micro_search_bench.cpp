// SPDX-License-Identifier: Apache-2.0

#include "quasar/search.hpp"
#include <benchmark/benchmark.h>
#include <string>
#include <random>

static std::string make_text(size_t n, int seed=42) {
  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> dist(97, 122);
  std::string s; s.resize(n);
  for (size_t i=0;i<n;++i) s[i] = (char)dist(rng);
  return s;
}

static void BM_TwoWay_ShortNeedle(benchmark::State& st) {
  auto hay = make_text(1<<20);
  std::string needle = "abc";
  for (auto _ : st) {
    auto v = quasar::search::find_all(hay, needle);
    benchmark::DoNotOptimize(v.size());
  }
}
BENCHMARK(BM_TwoWay_ShortNeedle)->Iterations(1)->Repetitions(15);

static void BM_BMH_LongNeedle(benchmark::State& st) {
  auto hay = make_text(1<<20);
  std::string needle = "abcdefghij";
  for (auto _ : st) {
    auto v = quasar::search::find_all_bmh(hay, needle);
    benchmark::DoNotOptimize(v.size());
  }
}
BENCHMARK(BM_BMH_LongNeedle)->Iterations(1)->Repetitions(15);

BENCHMARK_MAIN();
