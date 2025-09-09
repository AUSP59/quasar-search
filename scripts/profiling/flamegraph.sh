#!/usr/bin/env bash
set -euo pipefail
BIN=${1:-build/apps/quasar-grep}
if ! command -v perf >/dev/null 2>&1; then
  echo "perf not found"
  exit 0
fi
sudo perf record -F 99 -g "$BIN" --help >/dev/null 2>&1 || true
sudo perf script > out.perf
echo "perf script written to out.perf"
