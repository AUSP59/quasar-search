// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include "quasar/cpu.hpp"
#include <cstddef>
#include <vector>
#include <string_view>

#if defined(__x86_64__) || defined(_M_X64)
  #include <immintrin.h>
#endif

namespace quasar::search {

static inline std::vector<size_t> scalar_find_first(std::string_view hay, char c) {
  std::vector<size_t> pos;
  for (size_t i=0;i<hay.size();++i) if (hay[i]==c) pos.push_back(i);
  return pos;
}

static inline std::vector<size_t> simd_find_first_avx2(std::string_view hay, char c) {
#if defined(__AVX2__)
  std::vector<size_t> pos;
  const __m256i needle = _mm256_set1_epi8((char)c);
  const size_t n = hay.size();
  size_t i = 0;
  for (; i + 31 < n; i += 32) {
    __m256i block = _mm256_loadu_si256((const __m256i*)(hay.data() + i));
    __m256i cmp = _mm256_cmpeq_epi8(block, needle);
    uint32_t mask = (uint32_t)_mm256_movemask_epi8(cmp);
    while (mask) {
      int bit = __builtin_ctz(mask);
      pos.push_back(i + (size_t)bit);
      mask &= mask - 1;
    }
  }
  for (; i < n; ++i) if (hay[i]==c) pos.push_back(i);
  return pos;
#else
  (void)c; return scalar_find_first(hay, c);
#endif
}

std::vector<size_t> find_all_simd_prefilter(std::string_view hay, std::string_view needle);

std::vector<size_t> find_all_simd_prefilter(std::string_view hay, std::string_view needle) {
  std::vector<size_t> out;
  if (needle.size()==0 || hay.size() < needle.size()) return out;
  const char first = needle[0];
  auto feats = quasar::cpu::detect();
#if defined(__AVX2__)
  auto hits = feats.avx2 ? simd_find_first_avx2(hay, first) : scalar_find_first(hay, first);
#else
  auto hits = scalar_find_first(hay, first);
#endif
  for (size_t i : hits) {
    if (i + needle.size() > hay.size()) continue;
    if (std::string_view(hay.data()+i, needle.size()) == needle) out.push_back(i);
  }
  return out;
}

} // namespace quasar::search