#!/usr/bin/env bash
set -euo pipefail
ENFORCE="${ENFORCE_PINNING:-false}"
bad=0
while IFS= read -r -d '' f; do
  if grep -E "uses:\s+[^@]+@v[0-9]+" -n "$f"; then
    echo "::warning file=$f::Unpinned action tag detected (@vX). Prefer commit SHA."
    bad=$((bad+1))
  fi
done < <(find .github/workflows -name '*.yml' -print0)
echo "Unpinned count: $bad"
if [[ "$ENFORCE" == "true" && $bad -gt 0 ]]; then
  echo "Failing due to ENFORCE_PINNING=true"; exit 1
fi
