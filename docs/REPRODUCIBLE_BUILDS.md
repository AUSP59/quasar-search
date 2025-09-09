# Reproducible Builds
- Use the devcontainer and fixed compiler versions for deterministic outputs.
- Avoid embedding timestamps; prefer SOURCE_DATE_EPOCH if applicable.
- Compare digests of built artifacts across clean environments.


## Build flags (portable)
Enable `-DQUASAR_REPRO_BUILD=ON` to add:
- GCC/Clang: `-ffile-prefix-map` and `-fmacro-prefix-map` to strip absolute paths, `-fno-record-gcc-switches`
- MSVC: `/Brepro`
Honor `SOURCE_DATE_EPOCH` in your environment for timestamps.
