# Security Policy

This project is a high-performance C++ text-search toolkit. Its primary attack surface is local file parsing, CLI argument handling, path traversal, concurrency, and optional SIMD code. We prioritize memory safety, predictable behavior, and secure releases.

## Report a Vulnerability

- Email: alanursapu@gmail.com
- GitHub: @AUSP59
- Subject line: [SECURITY] <short title>

Please include:
- A clear description and impact.
- A minimal reproducer, affected versions/commit, and platform.
- Crash logs, ASAN/UBSAN output, or backtraces.
- CVSS v3.1 vector if known.

We confirm receipt within 48 hours, provide an initial assessment within 5 business days, and target coordinated disclosure within 90 days (expedited for critical issues).

## Scope

In scope:
- Memory safety (OOB, UAF, double free), integer overflows.
- Path traversal, symlink race (TOCTOU), incorrect permission handling.
- Denial of service by crafted inputs (unbounded CPU/RAM), deadlocks.
- Information leaks via verbose errors or JSON output.
- Build, packaging, and release integrity (supply chain).
- Incorrect binary-file detection enabling unsafe behavior.
- CLI parsing leading to unsafe writes or traversal.

Out of scope:
- Vulnerabilities exclusively in optional third-party tools.
- Social engineering, physical attacks, or stolen tokens outside this repo.
- Hypothetical issues without a plausible exploit or measurable impact.

## Safe Harbor

We will not pursue or support legal action for good-faith research that:
- Respects privacy and does not exfiltrate data.
- Avoids service interruption and excessive resource use.
- Uses only accounts/assets you own or have permission to test.
- Stops and reports immediately upon finding a vulnerability.

## Severity and SLAs

We follow CVSS v3.1:
- Critical (9.0–10): Patch ASAP, public advisory on release.
- High (7.0–8.9): Patch within 14 days.
- Medium (4.0–6.9): Patch within 30 days.
- Low (0.1–3.9): Patch in the next regular release.

If a fix is complex, we may ship mitigations (flags, defaults, docs) first.

## Coordinated Disclosure

We coordinate a timeline and publish:
- GitHub Security Advisory (GHSA) with CVE when eligible.
- Affected versions, risk, workarounds, and credit.
- Reproducer and tests after patched releases are available.

## Supported Versions

- `main` branch.
- The latest stable release and the one prior.
Older versions may receive best-effort guidance only.

## Patch Process

1. Triage and reproduce privately.
2. Create a private fix branch with tests.
3. Run sanitizers, fuzz, static analysis, and cross-platform CI.
4. Prepare advisory text and release notes.
5. Cut patched releases and tags; publish advisory.
6. Credit reporters (opt-in).

## Responsible Use and Hardening (Users)

- Run with least privilege. Do not execute as root unless required.
- Prefer scanning local, trusted disks. Treat untrusted network filesystems cautiously.
- Set explicit limits:
    ~~~bash
    ulimit -v 2G -t 120
    ./quasar-grep --threads=8 --mmap --binary=skip ...
    ~~~
- Use deterministic flags for automated pipelines and JSON output for machine parsing.

## Secure Build Defaults (Maintainers)

Linux/Clang/GCC recommended flags:
    -D_FORTIFY_SOURCE=3 -O2 -fstack-protector-strong -fPIE -pie -Wl,-z,relro -Wl,-z,now -fno-omit-frame-pointer -D_GLIBCXX_ASSERTIONS

MSVC:
    /guard:cf /Qspectre /GS /sdl

CMake options:
    -DQUASAR_ENABLE_ASAN=ON -DQUASAR_ENABLE_UBSAN=ON -DQUASAR_ENABLE_TIDY=ON -DQUASAR_ENABLE_FUZZ=ON

## Testing and Tooling

- Sanitizers: ASAN, UBSAN on PRs that touch hot code.
- Fuzz: build corpus for CLI and core parsers; run in CI nightly.
- Static analysis: clang-tidy baseline plus security checks.
- Regressions: every fixed bug gets a dedicated test.
- Large-file and boundary tests: chunk overlap = needle_size-1.

Fuzz quickstart:
    ~~~bash
    cmake -S . -B build-fuzz -DQUASAR_ENABLE_FUZZ=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build-fuzz -j
    ./build-fuzz/fuzz_quasar
    ~~~

## Supply Chain Security

- Minimize dependencies. Prefer header-only, permissive licenses.
- Pin versions and verify checksums for vendored code.
- Generate SBOM and scan releases:
    ~~~bash
    syft packages dir:. -o spdx-json > sbom.spdx.json
    trivy fs --scanners vuln,secret,misconfig .
    ~~~
- Sign releases and artifacts (Git tags and checksums). Provide provenance.

## Reproducible Releases

- Deterministic builds where possible. Record compiler, flags, and target triplets.
- Publish checksums and signatures for archives and packages.
- Avoid embedding timestamps or host paths in release binaries when feasible.

## Reporting Template

    Title: <short description>
    Affected: commit/tag, OS/arch, build flags
    Severity: CVSS v3.1 vector and score (if available)
    Summary: what is wrong and why it matters
    Steps to Reproduce:
      1) ...
      2) ...
    PoC:
      <input files, command line, or script>
    Impact: crash, RCE, data leak, DoS, other
    Workarounds: flags, config, environment
    Credits: how you want to be acknowledged

## Incident Response

If active exploitation is suspected:
- Contact: alanursapu@gmail.com with subject `[SECURITY][URGENT]`.
- We may temporarily revert features, disable SIMD, or change defaults.
- We will notify users via releases and advisories.

## Acknowledgments

We credit reporters by handle or name upon request in the advisory and CHANGELOG.

## security.txt (optional)

Place at `.well-known/security.txt`:

    Contact: mailto:alanursapu@gmail.com
    Preferred-Languages: en
    Policy: https://github.com/AUSP59/quasar-search/security/policy
    Acknowledgments: https://github.com/AUSP59/quasar-search/blob/main/SECURITY.md
    Canonical: https://github.com/AUSP59/quasar-search/.well-known/security.txt

## Contact

- Maintainer: @AUSP59
- Email: alanursapu@gmail.com