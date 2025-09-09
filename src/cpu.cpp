// SPDX-License-Identifier: Apache-2.0
#include "quasar/cpu.hpp"
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
  #if defined(_MSC_VER)
    #include <intrin.h>
  #endif
#endif

namespace quasar::cpu {

static void cpuid(unsigned int leaf, unsigned int subleaf, unsigned int regs[4]) {
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
  int r[4];
  __cpuidex(r, static_cast<int>(leaf), static_cast<int>(subleaf));
  regs[0]=r[0]; regs[1]=r[1]; regs[2]=r[2]; regs[3]=r[3];
#elif defined(__GNUC__) || defined(__clang__)
  unsigned int a,b,c,d;
  __asm__ __volatile__ ("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(leaf), "c"(subleaf));
  regs[0]=a; regs[1]=b; regs[2]=c; regs[3]=d;
#else
  regs[0]=regs[1]=regs[2]=regs[3]=0;
#endif
}

Features detect() {
  Features f{};
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
  unsigned int r[4] = {0,0,0,0};
  cpuid(1, 0, r);
  f.sse2  = (r[3] & (1u<<26)) != 0;
  f.sse42 = (r[2] & (1u<<20)) != 0;
  cpuid(7, 0, r);
  f.avx2 = (r[1] & (1u<<5)) != 0;
#endif
  return f;
}

} // namespace quasar::cpu