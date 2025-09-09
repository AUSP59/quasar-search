# Performance Tuning
- Enable `-DQUASAR_ENABLE_NATIVE=ON` to use local CPU features.
- Use PGO two-phase build:
  1) `-DQUASAR_ENABLE_PGO_GENERATE=ON`, run representative workloads.
  2) Reconfigure with `-DQUASAR_ENABLE_PGO_USE=ON`.
- Use `--threads N` and pin CPU, disable turbo when benchmarking.
- Prefer `RelWithDebInfo` for profiling; `Release` for production.
