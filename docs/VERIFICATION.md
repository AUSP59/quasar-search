# Verifying Releases
- **Signatures**: `cosign verify-blob --certificate <.pem> --signature <.sig> <artifact>`
- **Provenance**: Use GitHub's "Provenance" tab or `gh attestation download` to fetch and verify.
- **SBOM**: Compare `SBOM.spdx` / `bom.xml` with artifacts; use `cosign attest --type spdx` for attestation if attached.
