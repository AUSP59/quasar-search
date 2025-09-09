// SPDX-License-Identifier: Apache-2.0
#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build -D CMAKE_BUILD_TYPE=${1:-Release}
cmake --build build -j