cmake_minimum_required(VERSION 3.20)
project(quasar_search VERSION 10.0.0 LANGUAGES CXX)

# Options
option(QUASAR_ENABLE_TESTS "Build tests" ON)
option(QUASAR_ENABLE_SANITIZERS "Enable ASan/UBSan" OFF)
option(QUASAR_ENABLE_COVERAGE "Enable GCOV/LCOV coverage (Linux/GCC/Clang)" OFF)
option(QUASAR_NATIVE_OPT "Enable -march=native for local builds" OFF)
option(QUASAR_ENABLE_MMAP "Enable memory-mapped scanning" ON)
option(QUASAR_ENABLE_SIMD "Enable SIMD prefilter where supported" ON)
option(QUASAR_ENABLE_FUZZ "Build a simple libFuzzer target if available" OFF)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

# Global flags
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
  add_compile_options(-fno-operator-names)
  add_link_options()
  if (QUASAR_NATIVE_OPT)
    add_compile_options(-march=native)
  endif()
  if (QUASAR_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  endif()
  if (QUASAR_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(--coverage)
    add_link_options(--coverage)
  endif()
endif()

# Library
file(GLOB QUASAR_HEADERS CONFIGURE_DEPENDS include/quasar/*.hpp)
add_library(quasar STATIC
  src/search.cpp
  src/search_simd.cpp
  src/thread_pool.cpp
  src/fs.cpp
  src/cpu.cpp
  src/mmap.cpp
  src/search_dispatch.cpp
)
target_include_directories(quasar PUBLIC include)
target_compile_features(quasar PUBLIC cxx_std_20)

# Apps
add_executable(quasar-grep apps/quasar-grep.cpp)
add_executable(quasar-ls apps/quasar-ls.cpp)
target_link_libraries(quasar-grep PRIVATE quasar)
target_link_libraries(quasar-ls PRIVATE quasar)

add_executable(quasar-bench apps/quasar-bench.cpp)
target_link_libraries(quasar-bench PRIVATE quasar)

add_executable(quasar-cpu apps/quasar-cpu.cpp)

# Install
install(TARGETS quasar quasar-grep quasar-bench quasar-cpu
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Tests
if (QUASAR_ENABLE_TESTS)
  enable_testing()
  add_executable(quasar-tests tests/test_search.cpp)
  
  add_executable(quasar-fs-tests tests/test_fs.cpp)
  add_test(NAME quasar-fs-tests COMMAND quasar-fs-tests)
  target_link_libraries(quasar-tests PRIVATE quasar)
  add_test(NAME quasar-tests COMMAND quasar-tests)
endif()

# Packaging (CPack)
set(CPACK_PACKAGE_NAME "quasar-apex")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)

# Fuzzing (Clang/libFuzzer)
if (QUASAR_ENABLE_FUZZ AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(quasar-fuzz tests/fuzz_search.cpp)
  target_link_libraries(quasar-fuzz PRIVATE quasar)
  target_compile_options(quasar-fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
  target_link_options(quasar-fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
endif()


# Security hardening (GNU/Clang on Unix)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin")
  add_compile_options(-fstack-protector-strong -D_FORTIFY_SOURCE=2)
  add_link_options(-Wl,-z,relro,-z,now)
endif()

# Additional tests
if (QUASAR_ENABLE_TESTS)
  add_executable(quasar-dispatch-tests tests/test_dispatch.cpp)
  target_link_libraries(quasar-dispatch-tests PRIVATE quasar)
  add_test(NAME quasar-dispatch-tests COMMAND quasar-dispatch-tests)

  add_executable(quasar-mmap-tests tests/test_mmap.cpp)
  target_link_libraries(quasar-mmap-tests PRIVATE quasar)
  add_test(NAME quasar-mmap-tests COMMAND quasar-mmap-tests)
endif()

# Coverage target with gcovr (Linux/GCC/Clang)
if (QUASAR_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND CMAKE_SYSTEM_NAME MATCHES "Linux")
  find_program(GCOVR_EXE gcovr)
  if (GCOVR_EXE)
    add_custom_target(coverage
      COMMAND ${GCOVR_EXE} -r ${CMAKE_SOURCE_DIR} --xml -o coverage.xml
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating coverage.xml")
  endif()
endif()

# CPack generators (zip, tgz; deb/rpm if available)
set(CPACK_PACKAGE_CONTACT "Quasar Apex Contributors")
set(CPACK_GENERATOR "TGZ;ZIP;DEB;RPM")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Quasar Apex Contributors")
include(CPack)


# Install targets and headers
include(GNUInstallDirs)
install(TARGETS quasar quasar-grep quasar-bench quasar-cpu quasar-ls
  EXPORT quasarTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/quasar DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export CMake package
install(EXPORT quasarTargets
  FILE quasarTargets.cmake
  NAMESPACE quasar::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/quasar)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/quasarConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/quasarConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/quasarConfig.cmake @ONLY)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/quasarConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/quasarConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/quasar)

# pkg-config
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/quasar-apex.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/quasar-apex.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/quasar-apex.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)


# Reproducible builds (if env SOURCE_DATE_EPOCH is set)
if(DEFINED ENV{SOURCE_DATE_EPOCH})
  message(STATUS "SOURCE_DATE_EPOCH detected: reproducible build flags enabled")
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}=.)
  endif()
endif()


if (QUASAR_ENABLE_TESTS)
  add_executable(quasar-ignore-tests tests/test_ignore.cpp)
  target_link_libraries(quasar-ignore-tests PRIVATE quasar)
  add_test(NAME quasar-ignore-tests COMMAND quasar-ignore-tests)

  add_executable(quasar-binary-tests tests/test_binary.cpp)
  target_link_libraries(quasar-binary-tests PRIVATE quasar)
  add_test(NAME quasar-binary-tests COMMAND quasar-binary-tests)
endif()


include(FetchContent)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
FetchContent_Declare(benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3)
FetchContent_MakeAvailable(benchmark)

# Microbenchmarks (algorithms)
add_executable(quasar-microbench benches/micro_search_bench.cpp)
target_link_libraries(quasar-microbench PRIVATE quasar benchmark::benchmark)

# Macrobench (pipeline realistic)
add_executable(quasar-macrobench benches/macro_pipeline_bench.cpp)
target_link_libraries(quasar-macrobench PRIVATE quasar benchmark::benchmark)


# ---- QUASAR_ENABLE_HARDENING ----

option(QUASAR_ENABLE_HARDENING "Enable hardening flags on supported platforms" ON)
if (QUASAR_ENABLE_HARDENING)
  if (CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fstack-protector-strong -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS -fPIC)
    add_link_options(-Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack)
  endif()
endif()



# ---- PKGCONFIG_GENERATION ----

include(GNUInstallDirs)
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/quasar-apex.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/quasar-apex.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/quasar-apex.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)



# ---- FUZZ_TARGET ----

if (QUASAR_ENABLE_FUZZ)
  # Try libFuzzer with Clang
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_executable(quasar-fuzz fuzz/search_fuzz.cpp)
    target_link_libraries(quasar-fuzz PRIVATE quasar)
    target_compile_options(quasar-fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
    target_link_options(quasar-fuzz PRIVATE -fsanitize=fuzzer,address,undefined)
  endif()
endif()



# ---- C_API_SOURCES ----

# Add C API implementation to the main library
if (TARGET quasar)
  target_sources(quasar PRIVATE src/c_api.cpp)
endif()



# ---- BUILD_EXAMPLES ----

option(QUASAR_BUILD_EXAMPLES "Build examples" ON)
if (QUASAR_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()



# ---- ENABLE_C_AND_C_API_TEST ----

if (QUASAR_ENABLE_TESTS)
  enable_language(C)
  add_executable(quasar-c-api-test tests/test_c_api.c)
  target_link_libraries(quasar-c-api-test PRIVATE quasar)
  add_test(NAME quasar-c-api-test COMMAND quasar-c-api-test)
endif()


# ---- MSVC/AppleClang hardening ----

if (QUASAR_ENABLE_HARDENING)
  if (MSVC)
    # MSVC hardening
    add_compile_options(/sdl /guard:cf /Qspectre)
    add_link_options(/guard:cf)
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    # AppleClang/macOS: stack protector strong (FORTIFY is glibc-specific)
    add_compile_options(-fstack-protector-strong)
  endif()
endif()


# ---- CLI smoke tests ----
include(CTest)
if (QUASAR_ENABLE_TESTS)
  add_test(NAME quasar_cli_smoke_help COMMAND quasar-grep --help)
endif()


# ---- COMPLETIONS & MAN INSTALL ----
# Install shell completions
install(FILES completions/quasar-grep.bash DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/bash-completion/completions RENAME quasar-grep)
install(FILES completions/quasar-grep.zsh  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/zsh/site-functions RENAME _quasar-grep)
install(FILES completions/quasar-grep.fish DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/fish/vendor_completions.d RENAME quasar-grep.fish)

# Install man pages
install(FILES man/quasar-grep.1 man/quasar-ls.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)


# ---- LTO / IPO (portable) ----
option(QUASAR_ENABLE_LTO "Enable Link-Time Optimization/IPO where supported" OFF)
if (QUASAR_ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_err)
  if (ipo_supported)
    # Enable IPO on the main library if it exists
    if (TARGET quasar)
      set_property(TARGET quasar PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
  endif()
endif()


# ---- Reproducible build flags (portable) ----
option(QUASAR_REPRO_BUILD "Enable reproducible build flags where supported" ON)
if (QUASAR_REPRO_BUILD)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}=. -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=. -fno-record-gcc-switches)
  elseif (MSVC)
    add_compile_options(/Brepro)
  endif()
endif()


# ---- Static linking (opt-in, best-effort) ----
option(QUASAR_STATIC_LINK "Attempt static linking for Linux/musl-friendly builds" OFF)
if (QUASAR_STATIC_LINK AND (CMAKE_SYSTEM_NAME STREQUAL "Linux"))
  add_link_options(-static -static-libgcc -static-libstdc++)
endif()

# ---- Build info file ----
include(FindGit)
set(GIT_SHA "unknown")
if (GIT_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_SHA OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET)
endif()
file(WRITE "${CMAKE_BINARY_DIR}/buildinfo.txt"
"compiler=${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}\n"
"build_type=${CMAKE_BUILD_TYPE}\n"
"system=${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}\n"
"git=${GIT_SHA}\n"
)
install(FILES "${CMAKE_BINARY_DIR}/buildinfo.txt" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/quasar")

# ---- CPack debug symbols (opt-in) ----
option(QUASAR_PACKAGE_DEBUG_SYMBOLS "Include debug symbols in packages (best-effort)" OFF)
if (QUASAR_PACKAGE_DEBUG_SYMBOLS)
  set(CPACK_STRIP_FILES OFF)
endif()


# ---- Symbol visibility & export header ----
option(QUASAR_VISIBILITY_HIDDEN "Default to hidden symbols for shared libs" ON)
if (QUASAR_VISIBILITY_HIDDEN)
  set(CMAKE_C_VISIBILITY_PRESET hidden)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

# Provide export header support if main target exists
include(GenerateExportHeader OPTIONAL)
if (COMMAND generate_export_header AND TARGET quasar)
  generate_export_header(quasar BASE_NAME QUASAR)
  target_include_directories(quasar PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endif()

# ---- Version header (configured) ----
set(QUASAR_VERSION_MAJOR 10 CACHE STRING "Major version")
set(QUASAR_VERSION_MINOR 0 CACHE STRING "Minor version")
set(QUASAR_VERSION_PATCH 0 CACHE STRING "Patch version")
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.hpp.in ${CMAKE_BINARY_DIR}/generated/quasar/version.hpp @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/generated/quasar/version.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/quasar)


# ---- Performance Options: PGO & Native CPU (opt-in) ----
option(QUASAR_ENABLE_PGO_GENERATE "Build with PGO generate flags (phase 1)" OFF)
option(QUASAR_ENABLE_PGO_USE "Build with PGO use flags (phase 2)" OFF)
option(QUASAR_ENABLE_NATIVE "Enable -march=native for this machine" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if (QUASAR_ENABLE_PGO_GENERATE)
    add_compile_options(-fprofile-generate)
    add_link_options(-fprofile-generate)
  endif()
  if (QUASAR_ENABLE_PGO_USE)
    add_compile_options(-fprofile-use -fprofile-correction)
    add_link_options(-fprofile-use -fprofile-correction)
  endif()
  if (QUASAR_ENABLE_NATIVE)
    add_compile_options(-march=native)
  endif()
endif()


# ---- CMake package config (find_package QuasarApex) ----
include(CMakePackageConfigHelpers)
set(QUASARAPEX_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/QuasarApex")
if (TARGET quasar)
  install(TARGETS quasar
    EXPORT QuasarApexTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(EXPORT QuasarApexTargets
        NAMESPACE QuasarApex::
        DESTINATION ${QUASARAPEX_INSTALL_CONFIGDIR})

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/QuasarApexConfig.cmake.in
  ${CMAKE_BINARY_DIR}/QuasarApexConfig.cmake
  INSTALL_DESTINATION ${QUASARAPEX_INSTALL_CONFIGDIR})

write_basic_package_version_file(${CMAKE_BINARY_DIR}/QuasarApexConfigVersion.cmake
  VERSION ${QUASAR_VERSION_MAJOR}.${QUASAR_VERSION_MINOR}.${QUASAR_VERSION_PATCH}
  COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_BINARY_DIR}/QuasarApexConfig.cmake
  ${CMAKE_BINARY_DIR}/QuasarApexConfigVersion.cmake
  DESTINATION ${QUASARAPEX_INSTALL_CONFIGDIR})


# ---- ELF version script (Linux, optional) ----
option(QUASAR_ELF_VERSION_SCRIPT "Use linker version script for ELF symbol versioning" OFF)
if (QUASAR_ELF_VERSION_SCRIPT AND UNIX AND NOT APPLE)
  set(QUASAR_SONAME "${QUASAR_VERSION_MAJOR}")
  if (TARGET quasar)
    set_target_properties(quasar PROPERTIES
      VERSION "${QUASAR_VERSION_MAJOR}.${QUASAR_VERSION_MINOR}.${QUASAR_VERSION_PATCH}"
      SOVERSION "${QUASAR_VERSION_MAJOR}")
    target_link_options(quasar PRIVATE "-Wl,--version-script=${CMAKE_SOURCE_DIR}/cmake/quasar.map")
  endif()
endif()


# ---- Compile-time ergonomics ----
option(QUASAR_ENABLE_UNITY "Enable CMake Unity builds to speed up compilation" OFF)
set(CMAKE_UNITY_BUILD ${QUASAR_ENABLE_UNITY})

option(QUASAR_ENABLE_PCH "Enable precompiled headers for faster builds" OFF)
if (QUASAR_ENABLE_PCH AND TARGET quasar)
  target_precompile_headers(quasar PRIVATE <vector> <string> <string_view> <optional> <memory> <cstdint>)
endif()

option(QUASAR_WARN_STRICT "Enable strict warnings (best-effort) and optionally treat as errors in CI" ON)
if (QUASAR_WARN_STRICT)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
    if (DEFINED ENV{CI})  # Treat as errors only in CI
      add_compile_options(-Werror)
    endif()
  elseif (MSVC)
    add_compile_options(/W4)
  endif()
endif()

option(QUASAR_MSVC_STATIC_RUNTIME "Link static MSVC runtime (/MT)" OFF)
if (MSVC AND QUASAR_MSVC_STATIC_RUNTIME)
  foreach(flag_var CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
    string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
  endforeach()
endif()


# ---- Install man page & shell completions (opt-in) ----
option(QUASAR_INSTALL_MAN "Install man pages" ON)
option(QUASAR_INSTALL_COMPLETIONS "Install shell completions" ON)

include(GNUInstallDirs)

if (QUASAR_INSTALL_MAN)
  install(FILES ${CMAKE_SOURCE_DIR}/man/quasar-grep.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

if (QUASAR_INSTALL_COMPLETIONS)
  install(FILES ${CMAKE_SOURCE_DIR}/completions/bash/quasar-grep DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/bash-completion/completions RENAME quasar-grep)
  install(FILES ${CMAKE_SOURCE_DIR}/completions/zsh/_quasar-grep DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/zsh/site-functions)
  install(FILES ${CMAKE_SOURCE_DIR}/completions/fish/quasar-grep.fish DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/fish/vendor_completions.d)
endif()


# ---- pkg-config file generation ----
configure_file(${CMAKE_SOURCE_DIR}/cmake/QuasarApex.pc.in ${CMAKE_BINARY_DIR}/QuasarApex.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/QuasarApex.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)


# ---- Size optimization (opt-in) ----
option(QUASAR_OPT_FOR_SIZE "Optimize for binary size" OFF)
if (QUASAR_OPT_FOR_SIZE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Os -ffunction-sections -fdata-sections)
  add_link_options(-Wl,--gc-sections)
endif()
