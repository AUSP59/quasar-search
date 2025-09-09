// SPDX-License-Identifier: Apache-2.0
#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug -D QUASAR_ENABLE_COVERAGE=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
echo "If gcovr is installed, you can run: gcovr -r . --xml -o coverage.xml --html --html-details -o coverage.html"
