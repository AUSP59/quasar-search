# Quasar Apex Whitepaper (Concise)

## Abstract
Quasar Apex is a portable C++ text search toolkit and CLI optimized for correctness, clarity, and practical performance. It provides a clean baseline (Two‑Way and BMH) plus parallel directory scanning. The design favors maintainability and verification over opaque micro‑optimizations.

## Design Principles
- Measure first; prefer algorithmic wins before micro‑tuning.
- Separate I/O from search kernels.
- Deterministic outputs and tests.

## Roadmap (non‑binding)
- Optional SIMD prefilters (SSE4.2/AVX2) behind feature flags.
- Pluggable regex engines.
- On‑device provenance and release signing.