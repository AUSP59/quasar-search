#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-build/apps/quasar-grep}"
MAX_KB="${MAX_SIZE_KB:-10240}"
if [ ! -f "$BIN" ]; then
  echo "Binary not found: $BIN"; exit 0
fi
size_kb=$(du -k "$BIN" | cut -f1)
echo "Binary size: ${size_kb} KB (limit ${MAX_KB} KB)"
if [ "$size_kb" -gt "$MAX_KB" ]; then
  echo "ERROR: Binary size exceeds limit"; exit 1
fi
echo "OK: within size budget"
