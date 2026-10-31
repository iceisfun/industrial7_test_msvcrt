/* TC
 * id:    pfi_int_len_l
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf type field (l = long, 32-bit on Windows)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(ld) { char b[64]; volatile long mn = LONG_MIN, mx = LONG_MAX; sprintf(b, "%ld|%ld", mn, mx); CHECK_STR(b, "-2147483648|2147483647"); }
TC_CASE(lu) { char b[64]; volatile unsigned long u = ULONG_MAX; sprintf(b, "%lu", u); CHECK_STR(b, "4294967295"); }
TC_CASE(lx) { char b[64]; volatile long m = -1L; sprintf(b, "%lx|%lX", m, m); CHECK_STR(b, "ffffffff|FFFFFFFF"); }
TC_CASE(lo) { char b[64]; volatile unsigned long u = ULONG_MAX; sprintf(b, "%lo|%#lo", u, 8UL); CHECK_STR(b, "37777777777|010"); }
TC_CASE(li) { char b[64]; volatile long m = LONG_MIN; sprintf(b, "%li|%+li", m, 5L); CHECK_STR(b, "-2147483648|+5"); }

int main(void) { tc_begin(); TC_RUN(ld); TC_RUN(lu); TC_RUN(lx); TC_RUN(lo); TC_RUN(li); return tc_end(); }
