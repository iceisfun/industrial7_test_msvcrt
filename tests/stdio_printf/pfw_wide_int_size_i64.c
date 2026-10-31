/* TC
 * id:    pfw_wide_int_size_i64
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf size prefixes: I64 (all CRTs, since VC6)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile long long i64min = (-9223372036854775807LL - 1), i64max = 9223372036854775807LL;
static volatile unsigned long long u64max = 0xFFFFFFFFFFFFFFFFULL, x64 = 0x123456789ABCDEF0ULL;
TC_CASE(I64d_min_max) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%I64d|%I64d", i64min, i64max); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"-9223372036854775808|9223372036854775807"); CHECK_EQ_I(n, (int)(sizeof(L"-9223372036854775808|9223372036854775807")/sizeof(wchar_t)) - 1); }
TC_CASE(I64u_max) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%I64u", u64max); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"18446744073709551615"); CHECK_EQ_I(n, (int)(sizeof(L"18446744073709551615")/sizeof(wchar_t)) - 1); }
TC_CASE(I64x) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%I64x|%I64X", x64, x64); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"123456789abcdef0|123456789ABCDEF0"); CHECK_EQ_I(n, (int)(sizeof(L"123456789abcdef0|123456789ABCDEF0")/sizeof(wchar_t)) - 1); }
int main(void) { tc_begin(); TC_RUN(I64d_min_max); TC_RUN(I64u_max); TC_RUN(I64x); return tc_end(); }
