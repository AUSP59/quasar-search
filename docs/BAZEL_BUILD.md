# Bazel Build (Skeleton)
This repository includes a minimal `WORKSPACE` and `BUILD.bazel` to build the library and CLI with Bazel.
Use Bazelisk for convenience:
```bash
bazelisk build //:quasar //:quasar-grep
```
Targets use `glob()`; adjust to your source layout if needed.
