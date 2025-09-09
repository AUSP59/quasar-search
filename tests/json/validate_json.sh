#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-build/apps/quasar-grep}"
SCHEMA="schemas/quasar_grep.schema.json"
TMP="$(mktemp)"
printf "alpha\nbeta\n" > sample.txt
"$BIN" --json alpha sample.txt > "$TMP"
python3 - <<'PY' "$TMP" "$SCHEMA"
import json, sys
from jsonschema import validate
doc = json.load(open(sys.argv[1]))
schema = json.load(open(sys.argv[2]))
validate(doc, schema)
print("JSON validates against schema.")
PY
