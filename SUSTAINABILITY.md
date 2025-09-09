# Sustainability Guide
- Prefer `-O2` over `-O3` unless perf gains are measured.
- Provide `--no-color` for low-energy terminals and avoid needless redraw.
- Use `quasar-bench` harness and pin CPU governor to `performance` during benchmarks.
- Document CPU/OS/compiler, and set `SOURCE_DATE_EPOCH` for determinism.
