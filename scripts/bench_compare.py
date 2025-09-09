// SPDX-License-Identifier: Apache-2.0

#!/usr/bin/env python3
import json, sys, math

THRESH = float(os.environ.get("BENCH_REGRESSION_PCT", "5.0"))  # percent allowed
def load(fname):
  with open(fname, 'r') as f: return json.load(f)

def extract_runs(data):
  # Collect per-benchmark times from iteration runs
  runs = {}
  for e in data.get("benchmarks", []):
    name = e.get("name","")
    if e.get("run_type") == "iteration":
      time = e.get("real_time", e.get("cpu_time"))
      unit = e.get("time_unit", "ns")
      # Normalize to ns
      factor = {"ns":1, "us":1000, "ms":1_000_000, "s":1_000_000_000}.get(unit,1)
      runs.setdefault(name, []).append(time*factor)
  return runs

def percentile(arr, p):
  arr = sorted(arr)
  if not arr: return float('nan')
  k = (len(arr)-1)*p/100.0
  f = math.floor(k); c = math.ceil(k)
  if f==c: return arr[int(k)]
  d0 = arr[f]*(c-k); d1 = arr[c]*(k-f)
  return d0+d1

def main():
  if len(sys.argv)<3:
    print("Usage: bench_compare.py BASE.json PR.json")
    sys.exit(2)
  base = extract_runs(load(sys.argv[1]))
  pr = extract_runs(load(sys.argv[2]))
  failed = []
  for name in sorted(set(base.keys()) & set(pr.keys())):
    p50b = percentile(base[name], 50)/1e6
    p95b = percentile(base[name], 95)/1e6
    p50p = percentile(pr[name], 50)/1e6
    p95p = percentile(pr[name], 95)/1e6
    def delta(a,b): return 100.0*(a-b)/b if b>0 else 0.0
    d50 = delta(p50p,p50b); d95 = delta(p95p,p95b)
    print(f"{name}: p50 {p50p:.3f}ms (Δ{d50:+.1f}%), p95 {p95p:.3f}ms (Δ{d95:+.1f}%) vs base {p50b:.3f}/{p95b:.3f}ms")
    if d50 > THRESH or d95 > THRESH:
      failed.append(name)
  if failed:
    print("REGRESSION: ", ", ".join(failed))
    sys.exit(1)
  print("OK: no regression beyond threshold")
  sys.exit(0)

if __name__=="__main__":
  main()
