/* SPDX-License-Identifier: Apache-2.0 */
#include "quasar/c_api.h"
#include <assert.h>
#include <string.h>

int main(void) {
  const char* s = "abc quasar abc quasar";
  size_t n = quasar_count_substring(s, strlen(s), "quasar", 6);
  assert(n == 2);
  return 0;
}
