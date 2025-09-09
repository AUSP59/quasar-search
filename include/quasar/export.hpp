// SPDX-License-Identifier: Apache-2.0
#pragma once
#if defined(_WIN32) && !defined(QUASAR_STATIC)
  #if defined(QUASAR_BUILD)
    #define QUASAR_API __declspec(dllexport)
  #else
    #define QUASAR_API __declspec(dllimport)
  #endif
#else
  #if defined(__GNUC__) || defined(__clang__)
    #define QUASAR_API __attribute__((visibility("default")))
  #else
    #define QUASAR_API
  #endif
#endif
