/* TC
 * id:    pfm_1_bare_dot_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4 (lone '.' is precision 0); MS docs printf precision
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define T(fmt, v, want) do { char b[64]; volatile unsigned tv = (v); int n = sprintf(b, fmt, tv); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(zero_empty) { T("[%.d]", 0, "[]"); T("[%.u]", 0, "[]"); T("[%.o]", 0, "[]"); T("[%.x]", 0, "[]"); T("[%.i]", 0, "[]"); }
TC_CASE(nonzero) { T("%.i", 5, "5"); T("%.X", 255, "FF"); T("%.d", 10, "10"); T("%.o", 8, "10"); }
TC_CASE(width_blank) { T("[%5.d]", 0, "[     ]"); T("[%-5.d]", 0, "[     ]"); T("[%5.d]", 7, "[    7]"); }
TC_CASE(hash_hex_zero) { T("[%#.x]", 0, "[]"); T("[%#.x]", 255, "[0xff]"); }

int main(void) { tc_begin(); TC_RUN(zero_empty); TC_RUN(nonzero); TC_RUN(width_blank); TC_RUN(hash_hex_zero); return tc_end(); }
