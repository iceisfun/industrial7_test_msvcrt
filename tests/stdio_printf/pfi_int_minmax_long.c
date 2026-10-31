/* TC
 * id:    pfi_int_minmax_long
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs data type ranges (long is 32-bit)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(size) { CHECK_EQ_I(sizeof(long), 4); }
TC_CASE(ld) { char b[64]; volatile long mn = LONG_MIN, mx = LONG_MAX; sprintf(b, "%ld|%ld", mn, mx); CHECK_STR(b, "-2147483648|2147483647"); }
TC_CASE(ulong_max) { char b[64]; volatile unsigned long u = ULONG_MAX; sprintf(b, "%lu|%lx|%lo", u, u, u); CHECK_STR(b, "4294967295|ffffffff|37777777777"); }
TC_CASE(minus1) { char b[64]; volatile long v = -1L; sprintf(b, "%lu", v); CHECK_STR(b, "4294967295"); }

int main(void) { tc_begin(); TC_RUN(size); TC_RUN(ld); TC_RUN(ulong_max); TC_RUN(minus1); return tc_end(); }
