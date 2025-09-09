# Embedding Quasar Apex in your C++ app

After installing this project (`cmake --install build`), consume it with CMake:

```cmake
find_package(QuasarApex REQUIRED)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE QuasarApex::quasar)
```

Minimal example (`examples/embedding_demo.cpp`):

```cpp
#include <quasar/export.hpp>
#include <iostream>
int main() {
  std::cout << "Quasar embedded OK\n";
  return 0;
}
```
Build:
```bash
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build -j
```
