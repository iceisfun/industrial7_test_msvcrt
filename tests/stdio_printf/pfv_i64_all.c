/* TC
 * id:    pfv_i64_all
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf size prefixes I64
 */
#include "tc.h"
#include <stdio.h>
#include <stdint.h>

TC_CASE(I64d_min) { char b[64]; volatile __int64 v = INT64_MIN; sprintf(b, "%I64d", v); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(I64u_max) { char b[64]; volatile unsigned __int64 v = UINT64_MAX; sprintf(b, "%I64u", v); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(I64x) { char b[64]; volatile unsigned __int64 v = 0x123456789abcdefULL; sprintf(b, "%I64x|%I64X", v, v); CHECK_STR(b, "123456789abcdef|123456789ABCDEF"); }
TC_CASE(wide_I64)
{
    wchar_t b[64]; volatile __int64 v = -5000000000LL;
    int n = _snwprintf(b, 64, L"%I64d", v); b[63] = 0;
    CHECK_WSTR(b, L"-5000000000"); CHECK_EQ_I(n, 11);
}

int main(void) { tc_begin(); TC_RUN(I64d_min); TC_RUN(I64u_max); TC_RUN(I64x); TC_RUN(wide_I64); return tc_end(); }
