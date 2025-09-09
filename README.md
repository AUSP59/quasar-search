# Quasar Search

Fast, portable text-search toolkit in modern C++. Grep-style CLI plus a small library. Exact substring search with Two-Way/BMH core, optional AVX2 prefilter, mmap/streaming I/O, multithreaded directory walk, JSON output, metrics, and strong CI. Apache-2.0.

## Status
Actively developed. Production-hardening and feature parity in progress. Stable for experiments, tooling, and benchmarks.

## Features
- Grep-like CLI: recursive search, count, JSON lines, include/exclude globs
- Algorithms: Two-Way, Boyer–Moore–Horspool, SIMD prefilter (AVX2) with scalar verification
- I/O: mmap and streaming with automatic fallback
- Parallelism: thread pool, deterministic output ordering
- Heuristics: binary-file detection and policy control
- Metrics: progress and throughput reporting
- Library: minimal C and C++ APIs for embedding
- Cross-platform: Linux, macOS, Windows (x86_64); works without SIMD

## Getting Started

### Requirements
- CMake ≥ 3.24
- C++20 compiler: Clang ≥ 15, GCC ≥ 11, or MSVC ≥ 19.36
- Ninja (optional), Python 3.8+ for tooling
- AVX2 optional at runtime

### Build from source
~~~bash
git clone https://github.com/AUSP59/quasar-search.git
cd quasar-search
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DQUASAR_ENABLE_TESTS=ON -DQUASAR_ENABLE_SIMD=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
~~~

### Quick start
~~~bash
# Find a needle across a tree
./build/quasar-grep "ERROR" /var/log --threads=8

# Count matches only
./build/quasar-grep "See roadmap in README" . --count

# Machine-readable output
./build/quasar-grep "session" data --json > hits.ndjson
~~~

### CLI usage (common flags)
~~~bash
quasar-grep [OPTIONS] <pattern> [path...]
  -n, --line-number          Show line numbers
  -r, --recursive            Recurse into directories (default when path is a dir)
      --threads <N>          Worker threads (default: CPU cores)
      --mmap | --no-mmap     Force enable/disable mmap
      --simd | --no-simd     Enable AVX2 prefilter if available
      --binary <policy>      skip | text | all   (default: skip)
      --include <glob>       Include only matching paths (repeatable)
      --exclude <glob>       Exclude matching paths (repeatable)
      --count                Print match counts only
      --json                 Emit JSONL records
      --stats                Print summary metrics to stderr
      --help                 Show help
~~~

### JSON output schema
~~~json
{"path":"./app/log.txt","line":123,"match":"session started","byte_offset":4567}
{"path":"./bin/core","binary":true,"policy":"skip"}
{"summary":{"files":1024,"scanned_bytes":2147483648,"matches":420}}
~~~

## Library

### C API (example)
~~~c
#include "quasar/c_api.h"
#include <stdio.h>

int main() {
    const char* hay = "abc abc abc";
    const char* ned = "abc";
    size_t count = 0;
    if (quasar_count_substring(hay, 11, ned, 3, &count) == QUASAR_OK) {
        printf("%zu\n", count); // 3
    }
    return 0;
}
~~~

### CMake integration
~~~cmake
find_package(quasar CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE quasar::quasar)
target_compile_features(myapp PRIVATE cxx_std_20)
~~~

## Performance

### Tips
- Prefer `--mmap` on local files. Use streaming for network mounts.
- Pin `--threads` to physical cores. Keep I/O saturated but avoid thrashing.
- Filter early with `--include/--exclude` globs to reduce syscalls.
- For repeatable runs, warm caches or use a ramdisk.

### Benchmark protocol
~~~bash
# Example harness
taskset -c 0-15 ./build/quasar-grep "needle" dataset --threads=16 --mmap --stats \
  2> bench.log | wc -l
~~~
Report CPU, RAM, kernel, compiler, flags, data size, encoding distribution, median and p95 throughput, and baselines (e.g., ripgrep, GNU grep).

## Architecture
- Core search: byte-sequence exact matching; Two-Way for general cases, BMH for longer needles
- SIMD prefilter: AVX2 scans candidate windows, scalar verifier confirms
- File system: walker with glob filters, mmap/stream chunker with overlap = needle_size - 1
- Concurrency: sharded work queue; stable output ordering channel
- Error handling: explicit status codes in CLI; library returns error codes or expected-like results
- CPU detection: runtime feature probe gates SIMD paths

## Configuration Matrix (CMake options)
| Option                   | Default | Purpose                                   |
|-------------------------|---------|-------------------------------------------|
| QUASAR_ENABLE_SIMD      | ON      | Compile SIMD prefilter if toolchain permits |
| QUASAR_ENABLE_TESTS     | ON      | Build unit and e2e tests                  |
| QUASAR_ENABLE_FUZZ      | OFF     | Build fuzz targets                        |
| QUASAR_ENABLE_TIDY      | OFF     | Enable clang-tidy target                  |
| QUASAR_ENABLE_ASAN      | OFF     | AddressSanitizer build                    |

## Testing
~~~bash
ctest --test-dir build --output-on-failure
# Sanitizers
cmake -S . -B build-asan -DQUASAR_ENABLE_ASAN=ON
cmake --build build-asan -j && ctest --test-dir build-asan
# Fuzz
cmake -S . -B build-fuzz -DQUASAR_ENABLE_FUZZ=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-fuzz -j && ./build-fuzz/fuzz_quasar
~~~

## Roadmap
- Optional regex backend (PCRE2/RE2)
- UTF-8 aware search modes
- More SIMD variants and ARM NEON
- Richer JSON schema and machine metrics
- Packaged releases for major distros

## Troubleshooting
- Empty results: check `--binary` policy, globs, and case sensitivity.
- Slow runs: disable antivirus on working dir, try `--mmap`, tune `--threads`.
- Crashes on old CPUs: run with `--no-simd` or build with `-DQUASAR_ENABLE_SIMD=OFF`.

## Contributing
See `CONTRIBUTING.md`. Follow the Code of Conduct. Small, focused PRs are preferred. For large features, open a design discussion first.

## Security
Do not report security issues in public. Email: alanursapu@gmail.com with subject `[SECURITY]`. You will receive an acknowledgment and timeline.

## License
Apache-2.0. See `LICENSE` and `NOTICE`. Copyright (c) 2025 AUSP59.

## Maintainer and Contact
- Maintainer: AUSP59
- Email: alanursapu@gmail.com

## Acknowledgments
Inspired by classic algorithms (Crochemore–Perrin Two-Way, Boyer–Moore family) and modern search tools. Thanks to contributors and the open-source community.