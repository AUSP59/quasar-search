# Build Presets

Use `CMakePresets.json` to configure repeatable builds:

- `dev`, `release` basic builds
- Sanitizers: `asan`, `ubsan`, `tsan`, `msan`
- Performance: `pgo-generate`, `pgo-use`, `lto`
- `static` for static linking

Example:
```bash
cmake --preset release && cmake --build build/release -j
```
