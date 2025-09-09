# Security Response Process

1. **Intake**: Private report via GitHub Security Advisories (preferred) or email from `.well-known/security.txt`.
2. **Triage**: Assign CVSS base score, determine affected versions, create private patch branch.
3. **Fix & Verify**: Add tests, run CI (sanitizers, fuzzing), validate with OSV and CodeQL.
4. **Coordinated Release**: Tag release, publish SBOM, signatures (cosign OIDC) and provenance.
5. **Disclosure**: Publish advisory with credits and CVSS vector. Backport if needed.
6. **Postmortem**: Document learnings; update docs and hardening.
SLAs: see `SECURITY.md`.
