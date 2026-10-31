/* TC
 * id:    pfi_int_snwprintf_lengths
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Type Field Characters / Size Prefixes (h, l, I64; ll from VS2005? see note)
 * note:  %lld is only asserted for CRT >= 80.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <limits.h>

TC_CASE(h) { wchar_t b[64]; _snwprintf(b, 64, L"%hd|%hu|%hd", 65535, -1, 32768); CHECK_WSTR(b, L"-1|65535|-32768"); }
TC_CASE(l) { wchar_t b[64]; _snwprintf(b, 64, L"%ld|%lu|%lx", -7L, 4000000000UL, 0xabcdUL); CHECK_WSTR(b, L"-7|4000000000|abcd"); }
TC_CASE(i64)
{
    wchar_t b[96]; volatile long long mn = LLONG_MIN, mx = LLONG_MAX;
    _snwprintf(b, 96, L"%I64d|%I64d", mn, mx);
    CHECK_WSTR(b, L"-9223372036854775808|9223372036854775807");
}
TC_CASE(i64x) { wchar_t b[64]; volatile unsigned long long v = 0x123456789abcdefULL; _snwprintf(b, 64, L"%I64x|%I64X", v, v); CHECK_WSTR(b, L"123456789abcdef|123456789ABCDEF"); }
TC_CASE(ll)
{
#if TC_AT_LEAST(80)
    wchar_t b[96]; volatile long long mn = LLONG_MIN; volatile unsigned long long u = ULLONG_MAX;
    _snwprintf(b, 96, L"%lld|%llu|%llx", mn, u, u);
    CHECK_WSTR(b, L"-9223372036854775808|18446744073709551615|ffffffffffffffff");
#else
    TC_SKIP("ll not asserted before msvcr80");
#endif
}
int main(void) { tc_begin(); TC_RUN(h); TC_RUN(l); TC_RUN(i64); TC_RUN(i64x); TC_RUN(ll); return tc_end(); }
