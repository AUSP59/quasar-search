# Profiling Guide
- Use `scripts/profiling/flamegraph.sh` with `perf` to capture stack samples.
- Pin CPU frequency and minimize background noise.
- Prefer Release builds with frame pointers enabled (`-fno-omit-frame-pointer`).
