# Optional Memory Allocators

Enable alternative allocators at configure time:
- `-DQUASAR_USE_MIMALLOC=ON`
- `-DQUASAR_USE_JEMALLOC=ON`

Best-effort detection links against the allocator if available and defines `QUASAR_USE_*`.
