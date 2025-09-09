# C++ Style Guide (Quasar Apex)
- C++20, no exceptions across public APIs; use return values/structs.
- RAII and const-correctness; prefer `std::string_view` for input.
- Namespaces: `quasar` (root), submodules concise.
- No dynamic allocation in inner loops; pre-reserve where sensible.
- All new code must come with tests (unit/integration).