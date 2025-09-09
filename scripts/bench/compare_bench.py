#!/usr/bin/env python3
import json, sys
def load(p): 
    with open(p) as f: return json.load(f)
def idx(js): 
    return {b['name']: b for b in js.get('benchmarks', []) if 'real_time' in b}
def compare(a, b, threshold_pct=10.0):
    ia, ib = idx(a), idx(b)
    regressions = []
    for name in ia:
        if name in ib:
            ra, rb = ia[name]['real_time'], ib[name]['real_time']
            if rb > 0 and (ra - rb)/rb < -(threshold_pct/100.0):
                regressions.append((name, ra, rb))
    return regressions
if __name__ == "__main__":
    if len(sys.argv)<3:
        print("usage: compare_bench.py BASE.json NEW.json [threshold_pct]")
        sys.exit(2)
    base = load(sys.argv[1]); new = load(sys.argv[2])
    thr = float(sys.argv[3]) if len(sys.argv)>3 else 10.0
    reg = compare(new, base, thr)
    if reg:
        print("REGRESSIONS (>{}% worse):".format(thr))
        for n, a, b in reg:
            print(f"{n}: new={a:.2f} base={b:.2f} (real_time)")
        sys.exit(1)
    print("No regressions beyond threshold.")
