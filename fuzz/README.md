# Fuzzing
Build with Clang and enable libFuzzer/ASan:
```bash
cmake -S . -B build -D QUASAR_ENABLE_FUZZ=ON -D CMAKE_CXX_COMPILER=clang++
cmake --build build -j
./build/fuzz/quasar-fuzz -runs=100000
```
