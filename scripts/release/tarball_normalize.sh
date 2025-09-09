#!/usr/bin/env bash
set -euo pipefail
: "${SOURCE_DATE_EPOCH:=$(date +%s)}"
OUT="${1:-source.tar.gz}"
TMP="$(mktemp -d)"
git ls-files > "$TMP/files.txt"
tar --sort=name --mtime="@$SOURCE_DATE_EPOCH" --owner=0 --group=0 --numeric-owner -czf "$OUT" -T "$TMP/files.txt"
echo "Wrote $OUT with SOURCE_DATE_EPOCH=$SOURCE_DATE_EPOCH"
