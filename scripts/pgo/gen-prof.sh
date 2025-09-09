// SPDX-License-Identifier: Apache-2.0
#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build-pgo -D CMAKE_BUILD_TYPE=Release -D QUASAR_ENABLE_TESTS=ON
cmake --build build-pgo -j
LLVM_PROFILE_FILE="quasar-%p.profraw" ctest --test-dir build-pgo --output-on-failure || true
echo "Merge .profraw with llvm-profdata then rebuild with -fprofile-instr-use (configure this in your toolchain)."
