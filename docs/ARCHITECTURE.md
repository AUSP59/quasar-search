# Architecture

- `include/quasar/` — public headers
- `src/` — implementations
- `apps/` — CLI entry points
- `tests/` — unit tests
- `docs/` — documentation set
- `sbom/` — CycloneDX generator
- `compliance/` — generated SBOMs and compliance artifacts

The library exposes high‑level search APIs. Internally we use a simple work‑stealing thread pool for parallel directory scanning. Algorithms are separated from I/O.