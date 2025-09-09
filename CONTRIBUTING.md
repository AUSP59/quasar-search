# Contributing Guide

Thank you for improving this C++ text-search toolkit. This guide explains standards, workflows, and expectations so your contribution ships fast and safely.

## Ground rules
- Be kind. Follow `CODE_OF_CONDUCT.md`.
- License: Apache-2.0. By contributing, you agree your work is under this license.
- Security issues: do **not** open public issues. Email: alanursapu@gmail.com (GitHub: @AUSP59).

## What to work on
- Issues labeled `good first issue`, `help wanted`, or any open bug.
- Performance, portability, correctness, tests, docs, tooling.
- Before large refactors or new features, open a design discussion issue.

## Development quickstart
Requirements:
- C++20 compiler (Clang ≥ 15, GCC ≥ 11, MSVC ≥ 19.36)
- CMake ≥ 3.24, Ninja (optional), Python 3.8+
- clang-format, clang-tidy
- Linux/macOS/Windows supported; x86_64 baseline; AVX2 optional

Build:
    git clone https://github.com/AUSP59/quasar-search.git
    cd quasar-search
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DQUASAR_ENABLE_TESTS=ON -DQUASAR_ENABLE_SIMD=ON
    cmake --build build -j

Run:
    ./build/quasar-grep --help
    ./build/quasar-grep "needle" path --count --json

Tests:
    ctest --test-dir build --output-on-failure

Sanitizers:
    cmake -S . -B build-asan -DQUASAR_ENABLE_ASAN=ON
    cmake --build build-asan -j
    ctest --test-dir build-asan

Fuzz (libFuzzer):
    cmake -S . -B build-fuzz -DQUASAR_ENABLE_FUZZ=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build-fuzz -j
    ./build-fuzz/fuzz_quasar

## Git workflow
- Fork and branch from `main`. Keep branches focused.
- Rebase on `main` before submitting. Avoid merge commits in PRs.
- Small PRs get reviewed faster.

Commit style (Conventional Commits):
    feat(search): add Two-Way fallback for short needles
    fix(fs): correct mmap window overlap at chunk boundaries
    perf(simd): add AVX2 prefilter with scalar verification path
    docs(cli): document --binary and --threads flags
    test: add regression for CRLF handling

Include a short rationale and links to issues. One logical change per commit.

## Pull requests
Checklist:
- Tests: unit and e2e updated; new code has tests.
- fmt/tidy: code is formatted and passes static checks.
- Docs: README/manpages/CLI help updated; changelog entry if user-visible.
- Benchmarks: include numbers if performance-related.
- Portability: builds on at least Linux and one of macOS/Windows.

Template:
    What
    Why
    How (approach, trade-offs)
    Tests (cases added/updated)
    Perf (methodology, results)
    Risks (fallbacks, flags)
    Docs (links to updated pages)

Review:
- Expect factual, actionable feedback.
- Address comments or explain trade-offs.
- Maintainers may request splitting or deferring parts.

## Coding standards
Language:
- C++20, no exceptions across CLI paths by default; library may use exceptions sparingly at boundaries documented below.
- Prefer `std::span`, `string_view`, and RAII.
- Avoid dynamic allocation in hot loops; pre-size buffers.

Style:
- clang-format: repo config is the source of truth.
- clang-tidy: baseline checks must pass; disable with local NOLINT only when justified and documented.

API:
- Public headers under `include/`. Keep stable and minimal.
- Use `QUASAR_EXPORT` for symbols; internal code in `quasar::detail`.
- C API: pure C headers, POD types, explicit ownership rules.

Errors:
- Library: `expected<T, error_code>` or documented exceptions; never both for the same API.
- CLI: exit codes 0/1 for success/not found, 2+ for errors.

Concurrency:
- Use the provided thread pool. Avoid creating ad-hoc threads.
- All shared structures must be thread-safe or confined.
- Document parallel flags and deterministic output guarantees.

I/O and scanning:
- Support both `mmap` and streaming. Auto-fallback when `mmap` fails.
- Chunking must include `needle.size() - 1` overlap. Test boundary cases.
- Respect `--binary` policy. Detect likely-binary files by heuristic, but allow override.

SIMD:
- Introduce new SIMD paths behind runtime feature checks.
- Always include a scalar verification gate for prefilters.
- Guard intrinsics with compile-time and runtime checks.
- Add microbenchmarks and correctness tests.

Text and encoding:
- Treat input as byte sequences by default.
- If adding UTF-8 features, keep them optional and well-documented.

## Testing
Unit tests:
- Cover success, edge, and failure paths.
- Include large-pattern, empty-pattern, and all-bytes cases.

E2E tests:
- CLI flags, globbing, JSON output, concurrency determinism.

Regression:
- Add a test for every fixed bug.

Coverage:
- Aim to keep or improve coverage for touched areas.

## Performance contributions
Methodology:
- Pin CPU governor to performance; disable turbo if needed.
- Report CPU model, cores, RAM, kernel, compiler, flags.
- Warm-up runs, then 5+ measured runs. Report median and p95.
- Control for I/O: use tmpfs/ramdisk or repeated reads.
- Provide dataset summary and license.

Report format:
    CPU: 12900K, 32GB, Linux 6.8, Clang 18
    Build: -O3 -march=x86-64-v3, QUASAR_ENABLE_SIMD=ON
    Command: quasar-grep -n --threads=16 --mmap PATTERN DIR
    Data: 20GB text logs, 60% ASCII, 40% UTF-8
    Throughput: median 3.2 GB/s (p95 3.1–3.3)
    Baselines: ripgrep 13.0 = 3.0 GB/s, GNU grep = 1.9 GB/s

## Static analysis and formatting
    pipx install pre-commit
    pre-commit install
    pre-commit run -a

    cmake -S . -B build-tidy -DQUASAR_ENABLE_TIDY=ON
    cmake --build build-tidy

## Documentation
- Update README, manpages in `man/`, and CLI `--help` output.
- Add developer docs for any new module or flag.
- Keep examples minimal and copy-pastable.

## Dependencies
- Minimize new deps. Prefer header-only, permissive licenses.
- Add third-party notices to `THIRD_PARTY_NOTICES`.
- Vendoring requires maintainer approval and supply-chain checks.

## Versioning and releases
- SemVer for the C API and CLI flags.
- Breaking changes require a major bump and migration notes.
- Add changelog entries under `Unreleased` then tag on release.

## Issue triage (maintainers)
- Triage labels: `bug`, `perf`, `feature`, `platform/*`, `simd`, `docs`, `good first issue`, `help wanted`, `needs-repro`, `blocked`.
- Acknowledge within 5 business days. Request minimal reproducer where needed.
- Close stale issues after 30 days of no response with a friendly note.

## Legal
- Sign-off required (DCO). Add to every commit:
      Signed-off-by: Your Name <alanursapu@gmail.com>

- Do not contribute content you cannot license under Apache-2.0.

## Contact
- Maintainer: @AUSP59
- Email: alanursapu@gmail.com

Thank you for making fast, correct, and portable search better.