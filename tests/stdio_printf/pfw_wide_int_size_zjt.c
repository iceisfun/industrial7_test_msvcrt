/* TC
 * id:    pfw_wide_int_size_zjt
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Size Prefixes (z, j, t, ll, hh); C99 7.19.6.1; support added in VS2013 (msvcr120)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

TC_CASE(zu) { wchar_t b[64]; volatile size_t v = 123456; int n = _snwprintf(b, 64, L"%zu|%zx", v, v); CHECK_WSTR(b, L"123456|1e240"); CHECK_EQ_I(n, 12); }
TC_CASE(zd_neg) { wchar_t b[64]; volatile ptrdiff_t v = -77; _snwprintf(b, 64, L"%zd|%5zd", (size_t)v, (size_t)v); CHECK_WSTR(b, L"-77|  -77"); }
TC_CASE(jd) { wchar_t b[64]; volatile intmax_t v = INTMAX_MIN; volatile uintmax_t u = UINTMAX_MAX; _snwprintf(b, 64, L"%jd|%ju", v, u); CHECK_WSTR(b, L"-9223372036854775808|18446744073709551615"); }
TC_CASE(td) { wchar_t b[64]; volatile ptrdiff_t v = -5; _snwprintf(b, 64, L"%td|%tu", v, (ptrdiff_t)5); CHECK_WSTR(b, L"-5|5"); }
TC_CASE(lld) { wchar_t b[64]; volatile long long v = LLONG_MIN; _snwprintf(b, 64, L"%lld|%llx", v, (long long)0xabcdef0123LL); CHECK_WSTR(b, L"-9223372036854775808|abcdef0123"); }
TC_CASE(hhd_truncates) { wchar_t b[64]; volatile int v = 300, w = -129; _snwprintf(b, 64, L"%hhd|%hhu|%hhd", v, v, w); CHECK_WSTR(b, L"44|44|127"); }

int main(void) { tc_begin(); TC_RUN(zu); TC_RUN(zd_neg); TC_RUN(jd); TC_RUN(td); TC_RUN(lld); TC_RUN(hhd_truncates); return tc_end(); }
