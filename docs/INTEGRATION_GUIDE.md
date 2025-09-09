# Integration Guide
## CMake (recommended)
```cmake
find_package(quasar CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE quasar::quasar)
```

## pkg-config
A `.pc` file can be provided by distros; link with `pkg-config --cflags --libs quasar`.
