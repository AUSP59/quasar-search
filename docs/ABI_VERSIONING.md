# ABI Versioning & Symbol Policy

- Default symbol visibility is `hidden`; public APIs use `QUASAR_API`.
- On Linux you can enable an **ELF version script** with `-DQUASAR_ELF_VERSION_SCRIPT=ON`.
- Shared library properties (`VERSION` and `SOVERSION`) follow `MAJOR.MINOR.PATCH`, using `SOVERSION=MAJOR`.
- See `cmake/quasar.map` for version script example.
