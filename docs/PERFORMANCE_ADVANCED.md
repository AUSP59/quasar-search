# Advanced Performance Notes

- Enable `-DQUASAR_ENABLE_LTO=ON` and PGO presets for production builds.
- Try `-DQUASAR_USE_MIMALLOC=ON` or `-DQUASAR_USE_JEMALLOC=ON` if allocator-sensitive.
- Review CI **bloaty** report to identify heavy translation units.
- Use `perf`/`Instruments`/`VTune` locally; CI captures flamegraphs when enabled.
