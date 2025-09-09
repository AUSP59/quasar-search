#!/usr/bin/env python3
import sys, json

def load(path):
  with open(path) as f: return json.load(f)

def components(doc):
  # Support SPDX & CycloneDX simple cases
  if 'components' in doc:  # CycloneDX
    return { (c.get('name'), c.get('version')) for c in doc['components'] if isinstance(c, dict) }
  pkgs = []
  for i in doc.get('packages', []):  # SPDX
    n = i.get('name'); v = i.get('versionInfo'); pkgs.append((n,v))
  return set(pkgs)

def main(a,b):
  A = components(load(a)); B = components(load(b))
  new = B - A
  removed = A - B
  print("New components:", sorted([f"{n}@{v}" for n,v in new if n]))
  print("Removed components:", sorted([f"{n}@{v}" for n,v in removed if n]))
  if new:
    sys.exit(0)

if __name__ == "__main__":
  if len(sys.argv)<3:
    print("usage: diff.py BASE.json NEW.json"); sys.exit(2)
  main(sys.argv[1], sys.argv[2])
