# Threat Model (STRIDE)

## Assets
- Source code, release artifacts, CI secrets, package manifests, SBOM.

## Trust boundaries
- GitHub Actions runners, package registries, user environments.

## STRIDE
- **Spoofing**: Protected by branch protection, signed commits recommended, OIDC for cosign.
- **Tampering**: CI builds reproducible; SBOM and signatures detect changes.
- **Repudiation**: CI logs, release notes; provenance stored with artifacts.
- **Information Disclosure**: No telemetry; security disclosures via advisories.
- **Denial of Service**: CLI tools offline; rate-limit not applicable.
- **Elevation of Privilege**: Least-privilege tokens; dependency review and static analysis.

## Residual risks
- Supply chain of dependencies (mitigated via OSV/Trivy/Scorecard).
- User misconfiguration outside scope.
