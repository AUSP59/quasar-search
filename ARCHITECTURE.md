
# Architecture

```
apps/ (CLI) ----> libquasar (search, fs, mmap, cpu, glob, metrics, instrumentation)
                 ^      ^
                 |      +-- search_dispatch (Two-Way/BMH/SIMD heuristic)
                 +-- thread_pool
```
Data flow (grep): walk_tree -> [stream|mmap] -> search_* -> results -> metrics (p95/tps/mbps) -> JSON/Prometheus.
Design: zero external deps in core; optional SIMD; safe fallbacks; portability across OS.
