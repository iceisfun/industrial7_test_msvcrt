/* TC
 * id:    pfi_int_len_i32
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf size prefixes (I32)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(d) { char b[64]; volatile int mn = INT_MIN; sprintf(b, "%I32d|%I32d", mn, INT_MAX); CHECK_STR(b, "-2147483648|2147483647"); }
TC_CASE(u) { char b[64]; volatile unsigned u = UINT_MAX; sprintf(b, "%I32u", u); CHECK_STR(b, "4294967295"); }
TC_CASE(x) { char b[64]; volatile int m = -1; sprintf(b, "%I32x|%I32X", m, m); CHECK_STR(b, "ffffffff|FFFFFFFF"); }
TC_CASE(o) { char b[64]; volatile unsigned u = UINT_MAX; sprintf(b, "%I32o", u); CHECK_STR(b, "37777777777"); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(u); TC_RUN(x); TC_RUN(o); return tc_end(); }
