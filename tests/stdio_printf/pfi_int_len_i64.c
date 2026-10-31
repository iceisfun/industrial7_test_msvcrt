/* TC
 * id:    pfi_int_len_i64
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf size prefixes (I64)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(d) { char b[64]; volatile __int64 mn = INT64_MIN, mx = INT64_MAX; sprintf(b, "%I64d|%I64d", mn, mx); CHECK_STR(b, "-9223372036854775808|9223372036854775807"); }
TC_CASE(u) { char b[64]; volatile unsigned __int64 u = UINT64_MAX; sprintf(b, "%I64u", u); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(x) { char b[64]; volatile __int64 m = -1; sprintf(b, "%I64x|%I64X", m, m); CHECK_STR(b, "ffffffffffffffff|FFFFFFFFFFFFFFFF"); }
TC_CASE(o) { char b[64]; volatile unsigned __int64 u = UINT64_MAX; sprintf(b, "%I64o", u); CHECK_STR(b, "1777777777777777777777"); }
TC_CASE(i) { char b[64]; volatile __int64 m = -4294967296LL; sprintf(b, "%I64i", m); CHECK_STR(b, "-4294967296"); }
TC_CASE(zero_pad) { char b[64]; volatile __int64 m = -123456789012LL; sprintf(b, "%020I64d", m); CHECK_STR(b, "-0000000123456789012"); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(u); TC_RUN(x); TC_RUN(o); TC_RUN(i); TC_RUN(zero_pad); return tc_end(); }
