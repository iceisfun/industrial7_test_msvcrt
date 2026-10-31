/* TC
 * id:    pfi_int_i_same_as_d
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters / flag directives
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

#define T(fmt, v, want) do { char b[64]; volatile int tc_v = (v); int n = sprintf(b, fmt, tc_v); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want)-1)); } while (0)

TC_CASE(values) { T("%i", -5, "-5"); T("%i", 0, "0"); T("%i", 123456, "123456"); T("%i", INT_MIN, "-2147483648"); T("%i", INT_MAX, "2147483647"); }
TC_CASE(modifiers) { T("[%6i]", 42, "[    42]"); T("[%-6i]", 42, "[42    ]"); T("[%.4i]", 42, "[0042]"); T("[%+i]", 42, "[+42]");
  T("[% i]", 42, "[ 42]"); T("[%06i]", -42, "[-00042]"); T("[%.0i]", 0, "[]"); }
TC_CASE(same_as_d) {
  static const int vals[] = { 0, 1, -1, 99, -99, 100000, INT_MAX, INT_MIN };
  static const char *const fi[] = { "%i", "%8i", "%-8i|", "%+i", "% i", "%.6i", "%010i", "%+8.4i" };
  static const char *const fd[] = { "%d", "%8d", "%-8d|", "%+d", "% d", "%.6d", "%010d", "%+8.4d" };
  size_t i, j;
  for (i = 0; i < sizeof vals / sizeof *vals; i++) for (j = 0; j < sizeof fi / sizeof *fi; j++) {
    char a[64], b[64]; volatile int v = vals[i];
    sprintf(a, fi[j], v); sprintf(b, fd[j], v);
    CHECK_STR(a, b);
  }
}

int main(void) { tc_begin(); TC_RUN(values); TC_RUN(modifiers); TC_RUN(same_as_d); return tc_end(); }
