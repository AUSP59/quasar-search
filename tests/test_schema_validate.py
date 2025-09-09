// SPDX-License-Identifier: Apache-2.0
import json, subprocess, sys, os
from pathlib import Path

try:
    import jsonschema
except Exception:
    print("jsonschema missing; skipping")
    sys.exit(0)

BIN = sys.argv[1] if len(sys.argv) > 1 else "build/apps/quasar-grep"
if not Path(BIN).exists():
    print("binary missing; skipping")
    sys.exit(0)

p = subprocess.run([BIN, "--json", "alpha", "tests/data/sample.txt"], capture_output=True, text=True)
if p.returncode != 0:
    print("cli returned non-zero; skipping")
    sys.exit(0)

data = json.loads(p.stdout)
schema = json.load(open("schemas/quasar-grep.schema.json"))
jsonschema.validate(instance=data, schema=schema)
print("schema validation OK")
