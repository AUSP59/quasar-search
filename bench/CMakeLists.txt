# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(quasar-bench CXX)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.4
)
FetchContent_MakeAvailable(benchmark)

add_executable(bench_smoke bench_smoke.cpp)
target_link_libraries(bench_smoke PRIVATE benchmark::benchmark)
