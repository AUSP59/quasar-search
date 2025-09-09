#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-build/apps/quasar-grep}"
if [ ! -x "$BIN" ]; then
  echo "skip: binary not found at $BIN" >&2
  exit 0
fi
count=$("$BIN" --count alpha tests/data/sample.txt || echo "0")
echo "$count" > tests/out_alpha.count
diff -u tests/expected/sample_alpha.count tests/out_alpha.count
