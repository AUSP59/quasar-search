# Consuming Quasar Apex with CMake

## Using find_package
After installation (`cmake --install build`), use:

```cmake
find_package(QuasarApex REQUIRED)
add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE QuasarApex::quasar)
```

Ensure `CMAKE_PREFIX_PATH` includes the installation prefix or install to a standard system prefix.
