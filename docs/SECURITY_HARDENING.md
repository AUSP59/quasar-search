# Security Hardening
- Build with hardening flags (see CMake option `QUASAR_ENABLE_HARDENING`).
- Strip symbols in release packages; keep separate debug artifacts.
- On Linux, prefer RELRO/now, noexecstack; ensure PIE when applicable.
