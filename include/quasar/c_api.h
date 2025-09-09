// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Returns the number of non-overlapping matches of `needle` in `haystack`.
// Both pointers may contain binary data; lengths must be provided.
size_t quasar_count_substring(const char* haystack, size_t hay_len,
                              const char* needle,   size_t needle_len);

#ifdef __cplusplus
} // extern "C"
#endif
