# Release Quality Gate
Before tagging a release, ensure:
- CI (build/test) green on Linux/macOS/Windows
- CodeQL, cppcheck, clang-analyzer, clang-tidy clean
- ASan/UBSan jobs passing
- Coverage >= 80% (gcovr gate)
- SBOM generated and uploaded
- OSV scan clean
- Gitleaks clean
- Docs (Doxygen) build and gh-pages deploy (if enabled)
- Packages/build artifacts reproducible (see REPRODUCIBLE_BUILDS.md)
