# Performance Tuning

- Use `--jobs` ~ number of cores.
- Tune `--chunk` 512KiB..4MiB.
- Try `--mmap` and `--simd` when available (results identical).
- Measure with `--json` for p95/tps/mbps.
