# Container Hardening

- Use the provided `container/seccomp.json` and run with `--security-opt seccomp=container/seccomp.json`.
- Run as non-root; set `USER` in the Dockerfile or with `--user` at runtime.
- Use `--read-only` filesystem and mount only required paths as `:ro`.
