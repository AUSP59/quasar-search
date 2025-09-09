// SPDX-License-Identifier: Apache-2.0
#!/usr/bin/env python3
import json, os, time
from pathlib import Path

root = Path(__file__).resolve().parents[1]
files = []
for ext in (".hpp",".h",".cpp",".c",".md",".yml",".yaml",".cmake",".txt",".py"):
    for p in root.rglob(f"*{ext}"):
        if any(s in p.parts for s in [".git","build","out","dist"]):
            continue
        files.append(str(p.relative_to(root)))
components = [{
    "type": "file",
    "name": f,
    "version": "file",
    "purl": f"pkg:file/{f}"
} for f in sorted(files)]

sbom = {
  "bomFormat":"CycloneDX",
  "specVersion":"1.4",
  "version":1,
  "metadata":{
    "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
    "tools":[{"vendor":"quasar","name":"sbom-generator","version":"1.0"}],
    "component":{"type":"application","name":"quasar-apex","version":"10.0.0"}
  },
  "components": components
}
out = root/"compliance"
out.mkdir(exist_ok=True)
(out/"sbom.cdx.json").write_text(json.dumps(sbom, indent=2))
print(str(out/"sbom.cdx.json"))