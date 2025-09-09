// SPDX-License-Identifier: Apache-2.0
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

static void BM_Substr(benchmark::State& state) {
  std::string haystack(10000, 'a');
  haystack.replace(5000, 5, "alpha");
  for (auto _ : state) {
    auto pos = haystack.find("alpha");
    benchmark::DoNotOptimize(pos);
  }
}
BENCHMARK(BM_Substr);
BENCHMARK_MAIN();
