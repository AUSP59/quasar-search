# Workflow Hardening
- We insert `step-security/harden-runner` with `egress-policy: audit` to observe outbound calls.
- Minimize permissions in workflows; prefer `id-token: write` only where signing is required.
- Pin actions by major version or SHA where feasible and rely on Code Scanning for SARIF ingestion.
